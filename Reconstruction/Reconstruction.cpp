#include "Reconstruction.h"
#include"TDirectory.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include <cmath>
 
ClassImp(Reconstruction)
 
Reconstruction::Reconstruction(TString cfgFileName):
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fOutFileName(fConfigFile["OutFileName"].as<std::string>()),
fTreeFileName(fConfigFile["Input"]["TreeFileName"].as<std::string>()),
fRecoTreeName(fConfigFile["Input"]["Reconstructed"]["TreeName"].as<std::string>()),
fRecoDet1Hits(fConfigFile["Input"]["Reconstructed"]["Det1HitsBranch"].as<std::string>()),
fRecoDet2Hits(fConfigFile["Input"]["Reconstructed"]["Det2HitsBranch"].as<std::string>()),
fGenTreeName(fConfigFile["Input"]["Generated"]["TreeName"].as<std::string>()),
fGenConfig(fConfigFile["Input"]["Generated"]["ConfigBranch"].as<std::string>()),
fDetectorsName(fConfigFile["Input"]["Detectors"].as<std::string>()),
fSaveHistoResVsMult(fConfigFile["SaveHistoResVsMult"].as<bool>()),
fSaveHistoResVsZTrue(fConfigFile["SaveHistoResVsZTrue"].as<bool>()),
fMaxPhi(fConfigFile["MaxPhi"].as<double>()),
fSigmaZ(fConfigFile["nSigmaZ"].as<double>())
{
    TStopwatch ReconstructionTime;
    ReconstructionTime.Start();

    for(int i=1; i<=fResolutionVsMultiplicity->GetNbinsX()+1; i++)
    {
        double LowEdgeMult = fResolutionVsMultiplicity->GetBinLowEdge(i);
        double UpperEdgeMult = LowEdgeMult + fResolutionVsMultiplicity->GetBinWidth(i);
        string title = "Multiplicity [" + std::to_string(LowEdgeMult) + ", " + std::to_string(UpperEdgeMult) + "]";
        fHistResVsMult.push_back(new TH1D(title.c_str(), title.c_str(), 500, -0.5, 0.5));
    }
    
    for(int i=1; i<=fResolutionVsZTrue->GetNbinsX()+1; i++)
    {
        double LowEdgeMult = fResolutionVsZTrue->GetBinLowEdge(i);
        double UpperEdgeMult = LowEdgeMult + fResolutionVsZTrue->GetBinWidth(i);
        string title = "ZTrue [" + std::to_string(LowEdgeMult) + ", " + std::to_string(UpperEdgeMult) + "]";
        fHistResVsZTrue.push_back(new TH1D(title.c_str(), title.c_str(), 500, -0.5, 0.5));
    }

    TFile* file = TFile::Open(fTreeFileName.c_str());

    fDetectors= *((DetectorHandler*)file->Get(fDetectorsName.c_str()));

    TTree* TreeReco=(TTree*)file->Get(fRecoTreeName.c_str());
    TreeReco->SetBranchAddress(fRecoDet1Hits.c_str(),&fIntersections1);
    TreeReco->SetBranchAddress(fRecoDet2Hits.c_str(),&fIntersections2);

    TTree* TreeGen=(TTree*)file->Get(fGenTreeName.c_str());

    TreeGen->Draw((fGenConfig+".z>>zgen").c_str(),"","goff");
    TH1F* zgen= (TH1F*)gDirectory->Get("zgen");
    fSigmaFromSimulation=zgen->GetRMS();
    delete zgen;

    TreeGen->SetBranchAddress(fGenConfig.c_str(),&fConfigs);

    for (int i=0; i<(TreeReco->GetEntries()); i++)
    {   
        if (i%10000 == 0)
            cout<<"Reconstructing event "<<i<<endl;
        fIntersections1 = nullptr;
        fIntersections2 = nullptr;
        fConfigs = nullptr;
        TreeReco->GetEntry(i);
        TreeGen->GetEntry(i);
        FindTracklets();
        MinDca();
        FillHistoResiduals();
        FillHistoEff();
        FillHistoEfficiencyVsZTrue();
        FillHistoResolutionVsZTrue();
        FillHistoResolutionVsMultiplicity();
        delete fIntersections1;
        delete fIntersections2;
        delete fConfigs;
        fIntersections1->clear();
        fIntersections2->clear();

        //fIntersections1.shrink_to_fit();
        //fIntersections2.shrink_to_fit();
        fTracklets.clear();
    }
    delete TreeReco;
    delete TreeGen;
    cout << "Ciao" << endl;

    file->Close();
    WriteResolutionHistos();
    WriteResolutionZTrueHistos();

    cout << "Ending Reco" << endl;
    //FindTracklets();
    //MinDca();
    //FillHistoResiduals();
    //FillHistoEff();
    //FillHistoEfficiencyVsZTrue();
    //FillHistoResolutionVsMultiplicity();
    //FillHistoResolutionVsZTrue();
    TFile outfile("outfile.root","recreate");
    fResiduals->Write();
    fResolutionVsZTrue->Write();
    fResolutionVsMultiplicity->Write();
    pEff->Write();
    pEffvsZ->Write();
    
    //gStyle->SetErrorX(0.);
//  //fResolutionVsMultiplicity->SetMarkerStyle(20);
    //fResolutionVsMultiplicity->SetMarkerColor(kBlue);
    //fResolutionVsMultiplicity->SetLineColor(kRed);
    //fResolutionVsMultiplicity->SetOption("histp");
    //fResolutionVsMultiplicity->Write();
    
    //fResolutionVsZTrue->SetMarkerStyle(16);
    //fResolutionVsZTrue->SetMarkerColor(kBlue);
    //fResolutionVsZTrue->SetLineColor(kRed);
    //fResolutionVsZTrue->SetOption("E1");
    
    //pEff->SetLineColor(kRed);
    //pEffvsZ->SetLineColor(kRed);
    //
    outfile.Close();
    ReconstructionTime.Stop();
    cout<<"Reconstruction time:"<<endl;
    ReconstructionTime.Print("u");
}

void Reconstruction::WriteResolutionHistos()
{
    TFile ResolutionMultiplicity("ResolutionMultiplicity.root","recreate");
    for(int i=1; i<=fResolutionVsMultiplicity->GetNbinsX(); i++)
    {
        if(fHistResVsMult[i-1]->GetEntries()!=0){
            TF1* gaussian = new TF1("gaus", "gaus(0)", -0.5, 0.5); 
            gaussian->SetParameter(1,0);
            gaussian->SetParameter(2,0.007);
            fHistResVsMult[i-1]->Fit(gaussian, "MR");
            double c = gaussian->GetParameter(2);
            fResolutionVsMultiplicity->Fill(fResolutionVsMultiplicity->GetBinCenter(i), TMath::Sqrt((c*c)+(fHistResVsMult[i-1]->GetMean()*fHistResVsMult[i-1]->GetMean())));
            fResolutionVsMultiplicity->SetBinError(i, gaussian->GetParError(2));
            if (fSaveHistoResVsMult)  fHistResVsMult[i-1]->Write();
            delete gaussian;
            //histmultrange->Reset();
        }
    }
    ResolutionMultiplicity.Close();
}

void Reconstruction::WriteResolutionZTrueHistos()
{
    TFile ResolutionZTrue("ResolutionZTrue.root","recreate");
    for(int i=1; i<=fResolutionVsZTrue->GetNbinsX(); i++)
    {
        if(fHistResVsZTrue[i-1]->GetEntries()!=0){
            TF1* gaussian = new TF1("gaus", "gaus(0)", -0.5, 0.5); 
            fHistResVsZTrue[i-1]->Fit(gaussian, "MR");
            double c = gaussian->GetParameter(2);
            fResolutionVsZTrue->Fill(fResolutionVsZTrue->GetBinCenter(i), TMath::Sqrt((c*c)+(fHistResVsZTrue[i-1]->GetMean()*fHistResVsZTrue[i-1]->GetMean())));
            fResolutionVsZTrue->SetBinError(i, gaussian->GetParError(2));
            if (fSaveHistoResVsZTrue) fHistResVsZTrue[i-1]->Write();
            delete gaussian;
            //histmultrange->Reset();
        }
    }
    ResolutionZTrue.Close();
}
 
void Reconstruction::FindTracklets()
{
/*
 *  Function that fills fTracklets datamember containing,
 *  for each event the reconstructed tracklets
 */
    for (auto y = fIntersections1->begin(); y != fIntersections1->end(); ++y)
        for (auto j = fIntersections2->begin(); j != fIntersections2->end(); ++j)
            //The 2 hits are in the running window
            if(abs(y->phi-j->phi)<fMaxPhi)
            {
                fTracklets.push_back(*y);
                fTracklets.push_back(*j);
            }
}
 
void Reconstruction::MinDca()
{
/*
 *  Function reconstructs the Z coordinates of the vertex, by averaging the 
 *  Z coordinates of the tracks at their point of closest approach
 */
    
    vector<double> vertexTemp;

    if (fTracklets.size()==0)
    {
        fVertexesZ=nan("");
        return;
    }
    FillHistoMinDca(vertexTemp);
    int histomax(fVertexhisto->GetMaximumBin());
    double xmin(fVertexhisto->GetBinLowEdge(histomax)), xmax=xmin+fVertexhisto->GetBinWidth(histomax);
    std::vector<double> vertexTemp1;
    for (auto i : vertexTemp){
        if (i<xmax && i>xmin){
            vertexTemp1.push_back(i);
        }
    }
    fVertexesZ=MeanRunningWindow(vertexTemp1);
    fVertexesZResolutions=fVertexhisto->GetRMS();
    fVertexhisto->Reset();
}

double Reconstruction::MeanRunningWindow(vector<double> vertexes)
{
    double windowhalfwidth = 0.25;
    int maxfreq = 0, freq = 0;
    double mean, vertex;
    for (auto i : vertexes){
        double x1 = i-windowhalfwidth;
        double x2 = i+windowhalfwidth;
        for(auto j : vertexes){
            if(j<x2 && j>x1){
                freq++;
                mean+=j;
            }
        }
        if(freq>maxfreq){
            maxfreq = freq;
            vertex = mean/freq;
        }    
        freq = 0;
        mean = 0.;
    }
    return vertex;
}

void Reconstruction::FillHistoMinDca(vector<double>& vertextemp)
{
    for (unsigned j=0;j<fTracklets.size();j+=2)
    {
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        double x0 = fTracklets[j].GetX(fDetectors.GetRadii()[1]);
        double y0 = fTracklets[j].GetY(fDetectors.GetRadii()[1]);
        double x1 = fTracklets[j+1].GetX(fDetectors.GetRadii()[2]);
        double y1 = fTracklets[j+1].GetY(fDetectors.GetRadii()[2]);

        a = x1 - x0;
        b = y1 - y0;
        c = fTracklets[j+1].z - fTracklets[j].z;

        double t;
        t = - (a*x0 + b*y0) / (a*a+b*b);    //t of closest approach
        vertextemp.push_back(fTracklets[j].z + c*t);
        fVertexhisto->Fill(vertextemp.back());

    }
}

//void Reconstruction::FillHistoIntersection(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp)
//{
///*
// *  Function that reconstructs the vertex, by averaging the interstections
// *  between each track and a plane orthogonal to the track in the xy plane  
// *  and passing through the Z axis
// * 
// */
//    for (unsigned j=0;j<tracklets.size();j+=2)
//    {
//        double a,b,c,phi,cosphi,sinphi; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
//        double x0 = tracklets[j].GetX(fDetectors.GetRadii()[1]);
//        double y0 = tracklets[j].GetY(fDetectors.GetRadii()[1]);
//        double x1 = tracklets[j+1].GetX(fDetectors.GetRadii()[2]);
//        double y1 = tracklets[j+1].GetY(fDetectors.GetRadii()[2]);
//        
//        a = x1 - x0;
//        b = y1 - y0;
//        c = tracklets[j+1].z - tracklets[j].z;
//        phi=(tracklets[j+1].phi + tracklets[j].phi)/2;
//        cosphi=TMath::Cos(phi);
//        sinphi=TMath::Sin(phi);
//
//        double t;
//        //t of intersection between track and plane
//        t = - (cosphi*x0 + sinphi*y0) / (a*cosphi+b*sinphi);    
//
//        vertextemp.push_back(tracklets[j].z + c*t);
//        histo->Fill(vertextemp.back());
//    }
//}
//
void Reconstruction::FillHistoResiduals()
{
/*
 *  Function to fill the residuals histogram
 * 
 */
    if (!isnan(fVertexesZ))
        fResiduals->Fill(fVertexesZ-(*fConfigs)[0].z);
}

void Reconstruction::FillHistoEff()
{
/*
 *  Function to fill the residuals histogram
 * 
 */
    //Reconstructs the sigma from the data, to count only reconstructed
    //vertexes less than fSigmaZ away from generated vertex

    if (TMath::Abs((*fConfigs)[0].z)<fSigmaZ*fSigmaFromSimulation)
    {
        if (!isnan(fVertexesZ))
            pEff->Fill(true,(*fConfigs)[0].multiplicity);
        else
            pEff->Fill(false,(*fConfigs)[0].multiplicity);
    }

}

void Reconstruction::FillHistoEfficiencyVsZTrue()
{

    if (TMath::Abs((*fConfigs)[0].z)<fSigmaZ*fSigmaFromSimulation)
    {
        if (!isnan(fVertexesZ))
            pEffvsZ->Fill(true,(*fConfigs)[0].z);
        else
            pEffvsZ->Fill(false,(*fConfigs)[0].z);
    }
}

void Reconstruction::FillHistoResolutionVsMultiplicity()
{
    if(TMath::Abs((*fConfigs)[0].z)<fSigmaZ*fSigmaFromSimulation)
        fHistResVsMult[fResolutionVsMultiplicity->FindBin((*fConfigs)[0].multiplicity)-1]->Fill(fVertexesZ-(*fConfigs)[0].z);

}

void Reconstruction::FillHistoResolutionVsZTrue()
{
    if(TMath::Abs((*fConfigs)[0].z)<fSigmaZ*fSigmaFromSimulation)
        fHistResVsZTrue[fResolutionVsZTrue->FindBin((*fConfigs)[0].z)-1]->Fill(fVertexesZ-(*fConfigs)[0].z);

}

//void Reconstruction::FillHistoResolutionVsZTrue()
//{
//    cout << "Entering FillHistoResolutionVsZTrue " << endl;
//    // prendo tutti eventi con z in un certo range, faccio istogramma differenze tra ZTrue e ZReco e prendo come dato per l'istogramma
//    // l'RMS di questo istogramma
//    TFile ResolutionZTrue("ResolutionZTrue.root","recreate");
//    for(int i=1; i<=fResolutionVsZTrue->GetNbinsX(); i++){
//        
//        double LowEdgeMult = fResolutionVsZTrue->GetBinLowEdge(i);
//        double UpperEdgeMult = LowEdgeMult + fResolutionVsZTrue->GetBinWidth(i);
//
//        char title[1024];
//        sprintf(title, "ZTrue [%2.2f, %2.2f]", LowEdgeMult, UpperEdgeMult);
//        TH1D* histzrange = new TH1D(title, title, 500, -0.5, 0.5);
//    
//        for(std::vector<std::vector<Event::fVertMult> >::size_type j=0; j<fConfigs.size(); j++){
//            if(fConfigs[j][0].z>LowEdgeMult && fConfigs[j][0].z<UpperEdgeMult){
//                histzrange->Fill(fVertexesZ[j]-fConfigs[j][0].z);
//            }
//        }
//        TF1* gaussian = new TF1("gaus", "gaus(0)", -0.5, 0.5); 
//        histzrange->Fit(gaussian, "MR");
//        double c = gaussian->GetParameter(2);
//        fResolutionVsZTrue->Fill(fResolutionVsZTrue->GetBinCenter(i), TMath::Sqrt((c*c)+(histzrange->GetMean()*histzrange->GetMean())));
//        fResolutionVsZTrue->SetBinError(i, histzrange->GetRMSError());
//        //histzrange->Write();
//        histzrange->Write();
//        //histzrange->Reset();
//        delete histzrange;
//    }
//    ResolutionZTrue.Close();
//}

