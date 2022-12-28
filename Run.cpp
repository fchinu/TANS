#include "Run.h"

ClassImp(Run)

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),
fMultType(fConfigFile["MultType"].as<std::string>()),
fConstMult(fConfigFile["MultConst"].as<unsigned>()),
fMultFile(fConfigFile["MultFile"].as<std::string>()),
fMultHisto(fConfigFile["MultHisto"].as<std::string>()),
fMultRange(fConfigFile["MultRange"].as<std::vector<unsigned> >()),
fSigmaX(fConfigFile["SigmaX"].as<double>()),
fSigmaY(fConfigFile["SigmaY"].as<double>()),
fSigmaZ(fConfigFile["SigmaZ"].as<double>()),
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThickness(fConfigFile["Thickness"].as<std::vector<double> >()),
fLength(fConfigFile["Lenght"].as<std::vector<double> >()),
fMaterial(fConfigFile["Material"].as<std::vector<string> >()),
TreeGen("TreeGen","TreeGen"),
TreeRec("TreeRec","TreeRec")
{
    TStopwatch w;
    //vector<Event::fVertMult> Config;
    //vector<Event::fVertMult>* ptrConfig=&Config;
    TFile hfile("Tree.root","recreate");
    //TreeGen.Branch("Config",&ptrConfig);
    TreeGen.Branch("GenHits1",&ptrGenHits1);
    TreeGen.Branch("GenHits2",&ptrGenHits2);

    TreeRec.Branch("RecHits1",&ptrRecHits1);
    TreeRec.Branch("RecHits2",&ptrRecHits2);
    w.Start();
    CreateDetectors();
    if (fMultType.find("kConst") != std::string::npos)
        RunConstMult();
    else if (fMultType.find("kUniform") != std::string::npos)
        RunUniformMult();
    else if (fMultType.find("kCustom") != std::string::npos)
        RunCustomMult();            //TODO: default case
    hfile.Write();
    hfile.Close();
    w.Stop();
    w.Print("u");
}

void Run::RunConstMult()            //TODO: check case "MultConst" is not defined
{
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event a(fDetectors,fConstMult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec,&Config,ptrGenHits1,ptrGenHits2,ptrRecHits1,ptrRecHits1);     
    }
}

void Run::RunUniformMult()          //TODO: check case "MultRange" is not defined
{
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event a(fDetectors,gRandom->Integer(fMultRange[1]-fMultRange[0]+1) + fMultRange[0], gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec,&Config,ptrGenHits1,ptrGenHits2,ptrRecHits1,ptrRecHits1);     
    }
}

void Run::RunCustomMult()           //TODO: check case "fMultRange" is not defined
{
    TFile* infile = TFile::Open(fMultFile.c_str());
    TH1D* histo = (TH1D*)infile->Get(fMultHisto.c_str());
    unsigned mult;    
    for (unsigned i=0; i<fNEvents; i++)
    {
        do {
            mult = histo->GetRandom();
        }while (mult < fMultRange[0] || mult > fMultRange[1]);
        Event a(fDetectors,mult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),TreeGen,TreeRec,&Config,ptrGenHits1,ptrGenHits2,ptrRecHits1,ptrRecHits1);     
    }
}


void Run::CreateDetectors()     //TODO: add default cases
{
    for (vector<bool>::size_type i=0; i<fIsDetector.size(); i++)
    {
        Detector* DetTemp = new Detector;
        MaterialBudget* MatTemp = new MaterialBudget;
        if (fIsDetector[i])
        {
            DetTemp->SetGeometry(fThickness[i],fRadii[i],fLength[i]).SetMaterial(fMaterial[i]);
            fDetectors.push_back(DetTemp);
        }
        else
        {
            MatTemp->SetGeometry(fThickness[i],fRadii[i],fLength[i]).SetMaterial(fMaterial[i]);
            fDetectors.push_back(MatTemp);
        }
    }
    std::sort(fDetectors.begin(),fDetectors.end());
}
