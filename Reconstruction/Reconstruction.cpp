#include "Reconstruction.h"
#include"TDirectory.h"
#include "TH1D.h"
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
fMaxPhi(fConfigFile["MaxPhi"].as<double>()),
fSigmaZ(fConfigFile["nSigmaZ"].as<double>())
{
    TStopwatch ReconstructionTime;
    ReconstructionTime.Start();
    // Open the file containing the tree.
    TFile* file = TFile::Open(fTreeFileName.c_str());
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
    cout << "Fine FindTracklets ended" << endl;
    MinDca();
    cout << "Fine MinDca" << endl;
    FillHistoResiduals();
    cout << "FillHistoResiduals ended" << endl;
    FillHistoEff();
    TFile outfile("outfile.root","recreate");
    FillHistoResolutionVsMultiplicity();
    cout << "FillHIstoResolutionVsMultiplicity ended" << endl;
    FillHistoResolutionVsZTrue();
    cout << "FillHIstoResolutionVsZTrue ended" << endl;
    fResiduals->Write();
    fResolutionVsMultiplicity->Write();
    pEff->Write();
    outfile.Close();
    ReconstructionTime.Stop();
    cout<<"Reconstruction time:"<<endl;
    ReconstructionTime.Print("u");
}
 
void Reconstruction::FindTracklets()
{
    //cout << "Entering FindTracklets" << endl;
    std::vector<MaterialBudget::fPoint> tracklet;
    for (unsigned i=0; i<fIntersections1.size(); ++i)
    {
        for (auto y: fIntersections1[i])
            for (auto j: fIntersections2[i])
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
 
void Reconstruction::VertexReco()
{
    cout << "Entering VertexReco" << endl;
    std::vector<double> vertex;
    for(auto i: fTracklets){
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        a = i[0].x - i[1].x;
        b = i[0].y - i[1].y;
        c = i[0].z - i[1].z;
 
        // value of parameter t for intersection between line and orthogonal plane passing for O
        double t = -(a*i[1].x + b*i[1].y + c*i[1].z)/(a*a + b*b + c*c);
 
        // vertex coordinates
        double x, y, z;
        x = i[1].x + a*t;
        y = i[1].y + b*t;
        z = i[1].z + c*t;
 
        vertex.push_back(x);
        vertex.push_back(y);
        vertex.push_back(z);
 
        fVertexes.push_back(vertex);
        vertex.clear();
    }
    for(auto i: fVertexes){
        cout << "Vertex coordinates: ";
        for(auto j: i){
            cout << j << " ";
        }
        cout << endl;
    }
}

void Reconstruction::MinDca()
{
    TH1D* histo = new TH1D("vertex", "vertex", 50, -15,15);
    vector<double> vertexTemp;
    for(auto& i: fTracklets)
    {
        if (i.size()==0)
        {
            fVertexesZ.push_back(nan(""));
            continue;
        }
        FillHistoMinDca(histo, i, vertexTemp);

        int histomax(histo->GetMaximumBin()), count(0);
        double xmin(histo->GetBinLowEdge(histomax)), xmax=xmin+histo->GetBinWidth(histomax), mean(0);
        
        for (auto& i : vertexTemp)
            if (i<xmax && i>xmin)
            {
                mean+=i;
                ++count;
            }
        fVertexesZ.push_back(mean/count);
        //cout<<"Vertex is:"<<fVertexesZ.back();
        histo->Reset();
        vertexTemp.clear();
    }
    delete histo;
}

void Reconstruction::FillHistoMinDca(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp)
{
    for (unsigned j=0;j<tracklets.size();j+=2)
    {
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        a = tracklets[j+1].x - tracklets[j].x;
        b = tracklets[j+1].y - tracklets[j].y;
        c = tracklets[j+1].z - tracklets[j].z;

        double t;
        t = - (a*tracklets[j].x + b*tracklets[j].y) / (a*a+b*b);    //t of closest approach

        vertextemp.push_back(tracklets[j].z + c*t);
        histo->Fill(vertextemp.back());
    }
}

void Reconstruction::FillHistoResiduals()
{
    for(unsigned i=0; i<fConfigs.size(); ++i)
    {
        //cout<<fVertexesZ[i]<<"\t"<<fConfigs[i][0].z<<endl;
        if (!isnan(fVertexesZ[i]))
            fResiduals->Fill(fVertexesZ[i]-fConfigs[i][0].z);
    }
}

void Reconstruction::FillHistoEff()
{
    TFile* file = TFile::Open(fTreeFileName.c_str());
    TTree* tree = (TTree*)file->Get(fGenTreeName.c_str());
    tree->Draw((fGenConfig+".z>>zgen").c_str(),"","goff");
    TH1F* zgen= (TH1F*)gDirectory->Get("zgen");
    double sigma=zgen->GetRMS();
    file->Close();
    for(unsigned i=0; i<fConfigs.size(); ++i)
    {
        //cout<<fVertexesZ[i]<<"\t"<<fConfigs[i][0].z<<"\t"<<TMath::Abs(fVertexesZ[i]-fConfigs[i][0].z)<<endl;
        if (!isnan(fVertexesZ[i]) && TMath::Abs(fVertexesZ[i]-fConfigs[i][0].z)<fSigmaZ*sigma)
            pEff->Fill(true,fConfigs[i][0].multiplicity);
        else
            pEff->Fill(false,fConfigs[i][0].multiplicity);
    }
}

void Reconstruction::FillHistoResolutionVsMultiplicity()
{
    cout << "Entering FillHIstoResolutionVsMultiplicity " << endl;
    TH1D* histmultrange = new TH1D("histmultrange", "histmultrange", 500, -0.5, 0.5);
    cout << "histmultrange created " << endl;
    for(int i=1; i<=fResolutionVsMultiplicity->GetNbinsX(); i++){
        
        double LowEdgeMult = fResolutionVsMultiplicity->GetBinLowEdge(i);
        double UpperEdgeMult = LowEdgeMult + fResolutionVsMultiplicity->GetBinWidth(i);
    
        for(int j=0; j<fConfigs.size(); j++){
            if(fConfigs[j][0].multiplicity>LowEdgeMult && fConfigs[j][0].multiplicity<UpperEdgeMult){
                histmultrange->Fill(fVertexesZ[j]-fConfigs[j][0].z);
            }
        }
        cout << "Resolution for multiplicity range: " << LowEdgeMult << "-" << UpperEdgeMult << endl;
        fResolutionVsMultiplicity->Fill(fResolutionVsMultiplicity->GetBinCenter(i), histmultrange->GetRMS());
        histmultrange->Reset();
    }
    
    delete histmultrange;

}

void Reconstruction::FillHistoResolutionVsZTrue()
{
    cout << "Entering FillHistoResolutionVsZTrue " << endl;

}

