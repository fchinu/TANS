#ifndef RUN_H
#define RUN_H

#include "Riostream.h"
#include "yaml-cpp/yaml.h"
#include "TFile.h"
#include "TH1D.h"
#include "TStopwatch.h"
#include "TString.h"
#include "Event_Visual.h"
#include <vector>
#include <string>

class Run
{
public:
    Run(TString cfgFileName);

private:
    void RunConstMult(), RunUniformMult(), RunCustomMult();
    YAML::Node fConfigFile;
    unsigned fConstMult;        //TODO: possibly remove these 3 datamembers
    unsigned fNEvents;
    string fkMultType;
};


#endif