#include "Run.h"

ClassImp(Run)

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fOutFileName(fConfigFile["OutFileName"].as<std::string>()),
fAutoFlushOpt(fConfigFile["AutoFlushOpt"].as<int>()),
fAutoSaveOpt(fConfigFile["AutoSaveOpt"].as<int>()),
fTreeGen("fTreeGen","fTreeGen"),
fTreeRec("fTreeRec","fTreeRec"),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),


fMultHandler(fConfigFile),       //Multiplicity handler
fAngularHandler(fConfigFile),    //Angular distribution settings
fVertexHandler(fConfigFile),     //Vertex dispersion settings
fDetectorHandler(fConfigFile),   //Detector settings

fVerbose(fConfigFile["Verbose"].as<bool>())
{
    TStopwatch SimulationTime, WritingTime;
    SimulationTime.Start();
    TFile hfile(fOutFileName.c_str(),"recreate");
    fTreeGen.SetDirectory(&hfile);
    fTreeRec.SetDirectory(&hfile);
    fTreeGen.SetAutoSave(fAutoSaveOpt);
    fTreeRec.SetAutoSave(fAutoSaveOpt);
    fTreeRec.SetAutoFlush(fAutoFlushOpt);
    fTreeGen.SetAutoFlush(fAutoFlushOpt);

    TreeSettings();
    Start();

    SimulationTime.Stop();
    WritingTime.Start();
    fDetectorHandler.Write("Detectors");
    fTreeGen.Write("fTreeGen",kWriteDelete);
    fTreeRec.Write("fTreeRec",kWriteDelete);
    hfile.Close();
    WritingTime.Stop();
    cout<<"Simulation time: "<<endl;
    SimulationTime.Print("u");
    cout<<"Writing time: "<<endl;
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
        Event(fDetectorHandler.GetDetectors(),fMultHandler.GetMultiplicity(), fVertexHandler.GetXVertex(),fVertexHandler.GetYVertex(),fVertexHandler.GetZVertex(),fAngularHandler,fTreeGen,fTreeRec);     
    }
}

void Run::TreeSettings()     //TODO: add default cases
{
/*
 *  Function that generates the array of detectors based on the ConfigFile info
 */
    std::vector<MaterialBudget::fPoint>*    genhits     = nullptr;          //used to define Branch type
    std::vector<MaterialBudget::fPoint>*    rechits     = nullptr;          //used to define Branch type
    std::vector<Event::fVertMult>*          config      = nullptr;        //used to define Branch type

    fTreeGen.Branch("Config",&config);                      //Branch for multipliciy and vertex position

    unsigned DetCount = 0;                                                  //counts number of detectors
    for (vector<bool>::size_type i=0; i<(fDetectorHandler.GetIsDetector()).size(); ++i)
    {
        if (fDetectorHandler.GetIsDetector()[i])
        {
            if (fVerbose)
                ++DetCount;
                fTreeGen.Branch((string("GenHits_")+std::to_string(DetCount)).c_str(),&genhits);
                fTreeRec.Branch((string("RecHits_")+std::to_string(DetCount)).c_str(),&rechits);
                fDetectorHandler.GetDetectors()[i]->SetBranchAddress(fTreeGen,fTreeRec,DetCount);    //SetsAddress of corresponding branches
        }
    }
}
