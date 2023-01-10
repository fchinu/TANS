#include "Run.h"

ClassImp(Run)

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fOutFileName(fConfigFile["OutFileName"].as<std::string>()),
fTreeGen("fTreeGen","fTreeGen"),
fTreeRec("fTreeRec","fTreeRec"),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),

//Multiplicity handler
fMultHandler(fConfigFile),

//Angular distribution settings
fAngularHandler(fConfigFile),

//Vertex dispersion settings
fVertexHandler(fConfigFile),

//Material budgets settings
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThickness(fConfigFile["Thickness"].as<std::vector<double> >()),
fLength(fConfigFile["Lenght"].as<std::vector<double> >()),
fMaterial(fConfigFile["Material"].as<std::vector<string> >()),
fDoMultScattering(fConfigFile["DoMultScattering"].as<bool>()),
fDoSmearing(fConfigFile["DoSmearing"].as<bool>()),
fDoNoise(fConfigFile["DoNoise"].as<bool>()),
fMeanNoise(fConfigFile["MeanNoise"].as<int>()),

fVerbose(fConfigFile["Verbose"].as<bool>())
{
    TStopwatch SimulationTime, WritingTime;
    SimulationTime.Start();

    std::vector<Event::fVertMult>* config = nullptr;        //used to define Branch type
    fTreeGen.Branch("Config",&config);                      //Branch for multipliciy and vertex position

    CreateDetectors();
    Start();

    SimulationTime.Stop();
    
    WritingTime.Start();
    TFile hfile(fOutFileName.c_str(),"recreate");
    fTreeGen.Write();
    fTreeRec.Write();
    hfile.Close();
    WritingTime.Stop();
    cout<<"Simulation time:"<<endl;
    SimulationTime.Print("u");
    cout<<"Writing time:"<<endl;
    WritingTime.Print("u");
}

void Run::Start()
{
/*
 *  Function that runs fNEvents with constant multiplicity
 */
    for (unsigned i=1; i<=fNEvents; i++)
    {
        if (i%10000==0 && fVerbose)
            cout<<"Processing event "<<i<<endl;
        Event(fDetectors,fMultHandler.GetMultiplicity(), fVertexHandler.GetXVertex(),fVertexHandler.GetYVertex(),fVertexHandler.GetZVertex(),fAngularHandler,fTreeGen,fTreeRec);     
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
            fDetectors.push_back(new Detector(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering,fDoSmearing,fDoNoise,fMeanNoise));
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
