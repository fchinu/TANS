#include "Run.h"

ClassImp(Run)

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fOutFileName(fConfigFile["OutFileName"].as<std::string>()),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),
fMultType(fConfigFile["MultType"].as<std::string>()),
fConstMult(fConfigFile["MultConst"].IsNull() ? 0 : fConfigFile["MultConst"].as<unsigned>()),
fMultFile(fConfigFile["MultFile"].as<std::string>()),
fMultHisto(fConfigFile["MultHisto"].as<std::string>()),
fMultRange(fConfigFile["MultRange"].IsNull() ?  std::vector<unsigned>{} : fConfigFile["MultRange"].as<std::vector<unsigned> >()),
fSigmaX(fConfigFile["SigmaX"].as<double>()),
fSigmaY(fConfigFile["SigmaY"].as<double>()),
fSigmaZ(fConfigFile["SigmaZ"].as<double>()),
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThickness(fConfigFile["Thickness"].as<std::vector<double> >()),
fLength(fConfigFile["Lenght"].as<std::vector<double> >()),
fMaterial(fConfigFile["Material"].as<std::vector<string> >()),
fDoMultScattering(fConfigFile["DoMultScattering"].as<bool>()),
fDoSmearing(fConfigFile["DoSmearing"].as<bool>()),
TreeGen("TreeGen","TreeGen"),
TreeRec("TreeRec","TreeRec")
{
    TStopwatch w;
    std::vector<Event::fVertMult>*          config      = nullptr;
    std::vector<MaterialBudget::fPoint>*    genhits1    = nullptr;
    std::vector<MaterialBudget::fPoint>*    genhits2    = nullptr;
    std::vector<MaterialBudget::fPoint>*    rechits1    = nullptr;
    std::vector<MaterialBudget::fPoint>*    rechits2    = nullptr;
    

    TreeGen.Branch("Config",&config);
    TreeGen.Branch("GenHits detector 1",&genhits1);
    TreeGen.Branch("GenHits detector 2",&genhits2);

    TreeRec.Branch("RecHits detector 1",&rechits1);
    TreeRec.Branch("RecHits detector 2",&rechits2);

    w.Start();
    CreateDetectors();
    if (fMultType.find("kConst") != std::string::npos)
        RunConstMult();
    else if (fMultType.find("kUniform") != std::string::npos)
        RunUniformMult();
    else if (fMultType.find("kCustom") != std::string::npos)
        RunCustomMult();            //TODO: default case
    TFile hfile(fOutFileName.c_str(),"recreate");
    TreeGen.Write();
    TreeRec.Write();
    hfile.Close();
    TFile* hfileptr = &hfile;
    Reconstruction analysis(hfileptr,2);
    w.Stop();
    w.Print("u");
}

void Run::RunConstMult()
{
    if (fConstMult == 0)
    {
        cout<<"\033[93mWarning: MultConst was set to 0 or null, setting it to 10 instead\033[0m \n";
        fConstMult=10;
    }
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event a(fDetectors,fConstMult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec);     
    }
}

void Run::RunUniformMult()          //TODO: check case "MultRange" is not defined
{
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event a(fDetectors,gRandom->Integer(fMultRange[1]-fMultRange[0]+1) + fMultRange[0], gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec);     
    }
}

void Run::RunCustomMult()
{
    TFile* infile = TFile::Open(fMultFile.c_str());
    TH1D* histo = (TH1D*)infile->Get(fMultHisto.c_str());
    histo -> SetDirectory(0);
    infile->Close();
    unsigned mult;    
    for (unsigned i=0; i<fNEvents; i++)
    {
        if (fMultRange.size()>0)
        {
            do {
                mult = histo->GetRandom();
            }while (mult < fMultRange[0] || mult > fMultRange[1]);
        }
        else
            mult = histo->GetRandom();
        Event a(fDetectors,mult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec);     
    }
}


void Run::CreateDetectors()     //TODO: add default cases
{
    for (vector<bool>::size_type i=0; i<fIsDetector.size(); i++)
    {
        if (fIsDetector[i])
        {
            cout<<"Creating Detector"<<endl;
            //DetTemp.SetGeometry(fThickness[i],fRadii[i],fLength[i]).SetMaterial(fMaterial[i]);
            fDetectors.push_back(new Detector(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering,fDoSmearing,0));
        }
        else
        {
            cout<<"Creating MaterialBudget"<<endl;
            //MatTemp.SetGeometry(fThickness[i],fRadii[i],fLength[i]).SetMaterial(fMaterial[i]);
            fDetectors.push_back(new MaterialBudget(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering));
        }
    }
    std::sort(fDetectors.begin(),fDetectors.end(),[] (MaterialBudget const *const a, MaterialBudget const *const b) { return a->GetRadius() < b->GetRadius(); });
}
