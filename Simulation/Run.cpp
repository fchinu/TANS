#include "Run.h"
#include "Reconstruction.h"

ClassImp(Run)

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fOutFileName(fConfigFile["OutFileName"].as<std::string>()),
fTreeGen("fTreeGen","fTreeGen"),
fTreeRec("fTreeRec","fTreeRec"),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),

//Multiplicity settings
fMultType(fConfigFile["MultType"].as<std::string>()),
fConstMult(fConfigFile["MultConst"].IsNull() ? 0 : fConfigFile["MultConst"].as<unsigned>()),
fMultFile(fConfigFile["MultFile"].as<std::string>()),
fMultHisto(fConfigFile["MultHisto"].as<std::string>()),
fMultRange(fConfigFile["MultRange"].IsNull() ?  std::vector<unsigned>{} : fConfigFile["MultRange"].as<std::vector<unsigned> >()),

//Vertex dispersion settings
fSigmaX(fConfigFile["SigmaX"].as<double>()),
fSigmaY(fConfigFile["SigmaY"].as<double>()),
fSigmaZ(fConfigFile["SigmaZ"].as<double>()),

//Material budgets settings
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThickness(fConfigFile["Thickness"].as<std::vector<double> >()),
fLength(fConfigFile["Lenght"].as<std::vector<double> >()),
fMaterial(fConfigFile["Material"].as<std::vector<string> >()),
fDoMultScattering(fConfigFile["DoMultScattering"].as<bool>()),
fDoSmearing(fConfigFile["DoSmearing"].as<bool>()),

fVerbose(fConfigFile["Verbose"].as<bool>())
{
    TStopwatch SimulationTime, WritingTime;
    SimulationTime.Start();

    std::vector<Event::fVertMult>* config = nullptr;        //used to define Branch type
    fTreeGen.Branch("Config",&config);                      //Branch for multipliciy and vertex position

    CreateDetectors();
    if (fMultType.find("kConst") != std::string::npos)
        RunConstMult();
    else if (fMultType.find("kUniform") != std::string::npos)
        RunUniformMult();
    else if (fMultType.find("kCustom") != std::string::npos)
        RunCustomMult();            //TODO: default case
    SimulationTime.Stop();
    
    WritingTime.Start();
    TFile hfile(fOutFileName.c_str(),"recreate");
    fTreeGen.Write();
    fTreeRec.Write();
    Reconstruction analysis;
    hfile.Close();
    WritingTime.Stop();
    cout<<"Simulation time:"<<endl;
    SimulationTime.Print("u");
    cout<<"Writing time:"<<endl;
    WritingTime.Print("u");
}

void Run::RunConstMult()
{
/*
 *  Function that runs fNEvents with constant multiplicity
 */
    if (fConstMult == 0)
    {
        cout<<"\033[93mWarning: MultConst was set to 0 or null, setting it to 10 instead\033[0m \n";
        fConstMult=10;
    }
    for (unsigned i=0; i<fNEvents; i++)
    {
        if (i%10000==0 && i!=0 && fVerbose)
            cout<<"Processing event "<<i<<endl;
        Event(fDetectors,fConstMult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),fTreeGen,fTreeRec);     
    }
}

void Run::RunUniformMult()          //TODO: check case "MultRange" is not defined
{
/*
 *  Function that runs fNEvents with multiplicity distributed evenly in the range (fMultRange[0],fMultRange[1])
 */
    for (unsigned i=0; i<fNEvents; i++)
    {
        if (i%10000==0 && i!=0 && fVerbose)
            cout<<"Processing event "<<i<<endl;
        Event(fDetectors,gRandom->Integer(fMultRange[1]-fMultRange[0]+1) + fMultRange[0], gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),fTreeGen,fTreeRec);     
    }
}

void Run::RunCustomMult()
{
/*
 *  Function that runs fNEvents with multiplicity distributed according to an histogram
 *  If fMultRange is define, multiplicity is restricted to that range
 */
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

        if (i%10000==0 && i!=0 && fVerbose)
            cout<<"Processing event "<<i<<endl;
        Event(fDetectors,mult, gRandom->Gaus(0,fSigmaX),gRandom->Gaus(0,fSigmaY),gRandom->Gaus(0,fSigmaZ),fTreeGen,fTreeRec);     
    }
}


void Run::CreateDetectors()     //TODO: add default cases
{
/*
 *  Function that generates the array of detectors based on the ConfigFile info
 */
    std::vector<MaterialBudget::fPoint>*    genhits     = nullptr;          //used to define Branch type
    std::vector<MaterialBudget::fPoint>*    rechits     = nullptr;          //used to define Branch type

    unsigned DetCount = 0;                                                  //counts number of detectors
    for (vector<bool>::size_type i=0; i<fIsDetector.size(); ++i)
    {
        if (fIsDetector[i])
        {
            if (fVerbose)
                cout<<"Creating Detector"<<endl;
            fDetectors.push_back(new Detector(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering,fDoSmearing,0));
            ++DetCount;
            fTreeGen.Branch((string("GenHits_")+std::to_string(DetCount)).c_str(),&genhits);
            fTreeRec.Branch((string("RecHits_")+std::to_string(DetCount)).c_str(),&rechits);
            fDetectors[i]->SetBranchAddress(fTreeGen,fTreeRec,DetCount);    //SetsAddress of corresponding branches
        }
        else
        {
            if (fVerbose)
                cout<<"Creating MaterialBudget"<<endl;
            fDetectors.push_back(new MaterialBudget(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering));
        }
    }

    //Sorts detectors/materialbudgets based on the radii
    std::sort(fDetectors.begin(),fDetectors.end(),[] (MaterialBudget const *const a, MaterialBudget const *const b) { return a->GetRadius() < b->GetRadius(); });
}
