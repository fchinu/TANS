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
fMaxPhi(fConfigFile["MaxPhi"].as<double>()),
fSigmaZ(fConfigFile["nSigmaZ"].as<double>())
{
    TStopwatch ReconstructionTime;
    ReconstructionTime.Start();
    // Open the file containing the tree.
    TFile* file = TFile::Open(fTreeFileName.c_str());

    fDetectors= *((DetectorHandler*)file->Get(fDetectorsName.c_str()));
    
    TTreeReader RecReader(fRecoTreeName.c_str(), file);
   
    // The branch "RecHits detector 1" contains MaterialBudget::fPoint.

    TTreeReaderValue<std::vector<MaterialBudget::fPoint>> intersect1(RecReader, fRecoDet1Hits.c_str());
    TTreeReaderValue<std::vector<MaterialBudget::fPoint>> intersect2(RecReader, fRecoDet2Hits.c_str());
 
    // Loop over all entries of the TTree or TChain.
    while (RecReader.Next()) {
       // Just access the data as if intersect1 and intersect2 were iterators (note the '*'
       // in front of them):
       fIntersections1.push_back(*intersect1);
       fIntersections2.push_back(*intersect2);
    }

    TTreeReader GenReader(fGenTreeName.c_str(), file);
   
    TTreeReaderValue<std::vector<Event::fVertMult>> configs(GenReader, fGenConfig.c_str());
 
    // Loop over all entries of the TTree or TChain.
    while (GenReader.Next()) {
       // Just access the data as if intersect1 and intersect2 were iterators (note the '*'
       // in front of them):
       fConfigs.push_back(*configs);
    }
    file->Close();
    
    FindTracklets();
    MinDca();
    FillHistoResiduals();
    FillHistoEff();
    FillHistoEfficiencyVsZTrue();
    FillHistoResolutionVsMultiplicity();
    FillHistoResolutionVsZTrue();

    TFile outfile("outfile.root","recreate");
    fResiduals->Write();
    
    //gStyle->SetErrorX(0.);

    //fResolutionVsMultiplicity->SetMarkerStyle(20);
    //fResolutionVsMultiplicity->SetMarkerColor(kBlue);
    //fResolutionVsMultiplicity->SetLineColor(kRed);
    //fResolutionVsMultiplicity->SetOption("histp");
    fResolutionVsMultiplicity->Write();
    
    //fResolutionVsZTrue->SetMarkerStyle(16);
    //fResolutionVsZTrue->SetMarkerColor(kBlue);
    fResolutionVsZTrue->SetLineColor(kRed);
    //fResolutionVsZTrue->SetOption("E1");
    fResolutionVsZTrue->Write();
    
    pEff->SetLineColor(kRed);
    pEff->Write();
    pEffvsZ->SetLineColor(kRed);
    pEffvsZ->Write();
    
    outfile.Close();
    ReconstructionTime.Stop();
    cout<<"Reconstruction time:"<<endl;
    ReconstructionTime.Print("u");
}
 
void Reconstruction::FindTracklets()
{
/*
 *  Function that fills fTracklets datamember containing,
 *  for each event the reconstructed tracklets
 */
    std::vector<MaterialBudget::fPoint> tracklet;
    for (unsigned i=0; i<fIntersections1.size(); ++i)
    {
        for (auto y: fIntersections1[i])
            for (auto j: fIntersections2[i])
                //The 2 hits are in the running window
                if(abs(y.phi-j.phi)<fMaxPhi)
                {
                    tracklet.push_back(y);
                    tracklet.push_back(j);
                }
        if (tracklet.size()>0)
            fTracklets.push_back(tracklet);
        else
            fTracklets.push_back({});
        tracklet.clear();
    }
}
 
void Reconstruction::MinDca()
{
/*
 *  Function reconstructs the Z coordinates of the vertex, by averaging the 
 *  Z coordinates of the tracks at their point of closest approach
 */
    TH1D* histo = new TH1D("vertex", "vertex", 30, -15,15);
    vector<double> vertexTemp;
    for(auto& i: fTracklets)
    {
        if (i.size()==0)
        {
            fVertexesZ.push_back(nan(""));
            continue;
        }
        FillHistoMinDca(histo, i, vertexTemp);

        int histomax(histo->GetMaximumBin());
        double xmin(histo->GetBinLowEdge(histomax)), xmax=xmin+histo->GetBinWidth(histomax);

        std::vector<double> vertexTemp1;
         for (auto i : vertexTemp){
            if (i<xmax && i>xmin){
                vertexTemp1.push_back(i);
            }
        }

        fVertexesZ.push_back(MeanRunningWindow(vertexTemp1));
        fVertexesZResolutions.push_back(histo->GetRMS());
        histo->Reset();
        vertexTemp.clear();
        vertexTemp1.clear();
    }
    delete histo;
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

void Reconstruction::FillHistoMinDca(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp)
{
    for (unsigned j=0;j<tracklets.size();j+=2)
    {
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        double x0 = tracklets[j].GetX(fDetectors.GetRadii()[1]);
        double y0 = tracklets[j].GetY(fDetectors.GetRadii()[1]);
        double x1 = tracklets[j+1].GetX(fDetectors.GetRadii()[2]);
        double y1 = tracklets[j+1].GetY(fDetectors.GetRadii()[2]);


        a = x1 - x0;
        b = y1 - y0;
        c = tracklets[j+1].z - tracklets[j].z;

        double t;
        t = - (a*x0 + b*y0) / (a*a+b*b);    //t of closest approach

        vertextemp.push_back(tracklets[j].z + c*t);
        histo->Fill(vertextemp.back());
    }
}

void Reconstruction::FillHistoIntersection(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp)
{
/*
 *  Function that reconstructs the vertex, by averaging the interstections
 *  between each track and a plane orthogonal to the track in the xy plane  
 *  and passing through the Z axis
 * 
 */
    for (unsigned j=0;j<tracklets.size();j+=2)
    {
        double a,b,c,phi,cosphi,sinphi; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        double x0 = tracklets[j].GetX(fDetectors.GetRadii()[1]);
        double y0 = tracklets[j].GetY(fDetectors.GetRadii()[1]);
        double x1 = tracklets[j+1].GetX(fDetectors.GetRadii()[2]);
        double y1 = tracklets[j+1].GetY(fDetectors.GetRadii()[2]);
        
        a = x1 - x0;
        b = y1 - y0;
        c = tracklets[j+1].z - tracklets[j].z;
        phi=(tracklets[j+1].phi + tracklets[j].phi)/2;
        cosphi=TMath::Cos(phi);
        sinphi=TMath::Sin(phi);

        double t;
        //t of intersection between track and plane
        t = - (cosphi*x0 + sinphi*y0) / (a*cosphi+b*sinphi);    

        vertextemp.push_back(tracklets[j].z + c*t);
        histo->Fill(vertextemp.back());
    }
}

void Reconstruction::FillHistoResiduals()
{
/*
 *  Function to fill the residuals histogram
 * 
 */
    for(unsigned i=0; i<fConfigs.size(); ++i)
    {
        if (!isnan(fVertexesZ[i]))
            fResiduals->Fill(fVertexesZ[i]-fConfigs[i][0].z);
    }
}

void Reconstruction::FillHistoEff()
{
/*
 *  Function to fill the residuals histogram
 * 
 */
    for(unsigned i=0; i<fConfigs.size(); ++i)
    {
        if (!isnan(fVertexesZ[i]) && TMath::Abs(fVertexesZ[i]-fConfigs[i][0].z)<fSigmaZ*fResiduals->GetRMS())
            pEff->Fill(true,fConfigs[i][0].multiplicity);
        else
            pEff->Fill(false,fConfigs[i][0].multiplicity);
    }
}

void Reconstruction::FillHistoEfficiencyVsZTrue()
{
    for(unsigned i=0; i<fConfigs.size(); ++i)
    {
        if (!isnan(fVertexesZ[i]) && TMath::Abs(fVertexesZ[i]-fConfigs[i][0].z)<fSigmaZ*fResiduals->GetRMS())
            pEffvsZ->Fill(true,fConfigs[i][0].z);
        else
            pEffvsZ->Fill(false,fConfigs[i][0].z);
    }
}

void Reconstruction::FillHistoResolutionVsMultiplicity()
{
    cout << "Entering FillHIstoResolutionVsMultiplicity " << endl;
    TFile ResolutionMultiplicity("ResolutionMultiplicity.root","recreate");
    for(int i=1; i<=fResolutionVsMultiplicity->GetNbinsX(); i++){
        
        double LowEdgeMult = fResolutionVsMultiplicity->GetBinLowEdge(i);
        double UpperEdgeMult = LowEdgeMult + fResolutionVsMultiplicity->GetBinWidth(i);
        
        char title[1024];
        sprintf(title, "Multiplicity [%2.2f, %2.2f]", LowEdgeMult, UpperEdgeMult);
        TH1D* histmultrange = new TH1D(title, title, 500, -0.5, 0.5);
    
        for(unsigned j=0; j<fConfigs.size(); j++){
            if(fConfigs[j][0].multiplicity>LowEdgeMult && fConfigs[j][0].multiplicity<UpperEdgeMult&& TMath::Abs(fVertexesZ[j]-fConfigs[j][0].z)<fSigmaZ*fResiduals->GetRMS()){
                histmultrange->Fill(fVertexesZ[j]-fConfigs[j][0].z);
            }
        }
        //TF1* gaussian = new TF1("gaus", "gaus(0)", -0.5, 0.5); 
        //histmultrange->Fit(gaussian, "MR");
        //double c = gaussian->GetParameter(2);
        fResolutionVsMultiplicity->Fill(fResolutionVsMultiplicity->GetBinCenter(i), TMath::Sqrt((histmultrange->GetRMS()*histmultrange->GetRMS())+(histmultrange->GetMean()*histmultrange->GetMean())));
        fResolutionVsMultiplicity->SetBinError(i, histmultrange->GetRMSError());
        histmultrange->Write();
        //histmultrange->Reset();
        delete histmultrange;
    }
    ResolutionMultiplicity.Close();
}

void Reconstruction::FillHistoResolutionVsZTrue()
{
    cout << "Entering FillHistoResolutionVsZTrue " << endl;
    // prendo tutti eventi con z in un certo range, faccio istogramma differenze tra ZTrue e ZReco e prendo come dato per l'istogramma
    // l'RMS di questo istogramma
    TFile ResolutionZTrue("ResolutionZTrue.root","recreate");
    for(int i=1; i<=fResolutionVsZTrue->GetNbinsX(); i++){
        
        double LowEdgeMult = fResolutionVsZTrue->GetBinLowEdge(i);
        double UpperEdgeMult = LowEdgeMult + fResolutionVsZTrue->GetBinWidth(i);

        char title[1024];
        sprintf(title, "ZTrue [%2.2f, %2.2f]", LowEdgeMult, UpperEdgeMult);
        TH1D* histzrange = new TH1D(title, title, 500, -0.5, 0.5);
    
        for(std::vector<std::vector<Event::<fVertMult> > >::size_type j=0; j<fConfigs.size(); j++){
            if(fConfigs[j][0].z>LowEdgeMult && fConfigs[j][0].z<UpperEdgeMult){
                histzrange->Fill(fVertexesZ[j]-fConfigs[j][0].z);
            }
        }
        fResolutionVsZTrue->Fill(fResolutionVsZTrue->GetBinCenter(i), TMath::Sqrt((histzrange->GetRMS()*histzrange->GetRMS())+(histzrange->GetMean()*histzrange->GetMean())));
        fResolutionVsZTrue->SetBinError(i, histzrange->GetRMSError());
        //histzrange->Write();
        histzrange->Write();
        //histzrange->Reset();
        delete histzrange;
    }
    ResolutionZTrue.Close();
}