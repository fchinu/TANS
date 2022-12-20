#include "Run.h"

Run::Run(TString cfgFileName) : 
fConfigFile(YAML::LoadFile(cfgFileName.Data())),
fNEvents(fConfigFile["NEvents"].as<unsigned>()),
fkMultType(fConfigFile["MultType"].as<std::string>())
{
    TStopwatch w;
    w.Start();
    if (fkMultType.find("kConst") != std::string::npos)
        RunConstMult();
    else if (fkMultType.find("kUniform") != std::string::npos)
        RunUniformMult();
    else if (fkMultType.find("kCustom") != std::string::npos)
        RunCustomMult();
    w.Stop();
    w.Print("u");
}

void Run::RunConstMult()
{
    fConstMult = fConfigFile["MultConst"].as<unsigned>();
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event(fConstMult, 0,0,0);     
    }
}

void Run::RunUniformMult()
{
    vector<unsigned> Range = fConfigFile["MultRange"].as<std::vector<unsigned> >();
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event(gRandom->Integer(Range[1]-Range[0]+1) + Range[0], 0,0,0);     
    }
}

void Run::RunCustomMult()
{
    vector<unsigned> Range = fConfigFile["MultRange"].as<std::vector<unsigned> >();
    string kMultFile = fConfigFile["MultFile"].as<std::string>();
    string MultHisto = fConfigFile["MultHisto"].as<std::string>();

    TFile* infile = TFile::Open(kMultFile.c_str());
    TH1D* histo = (TH1D*)infile->Get(MultHisto.c_str());
    unsigned mult;
    do {
        mult = histo->GetRandom();
    }while (mult < Range[0] || mult > Range[1]);
    
    for (unsigned i=0; i<fNEvents; i++)
    {
        Event(mult, 0,0,0);     
    }
}