#ifndef RUN_H
#define RUN_H

#include "Event.h"
#include "yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TH1D.h"
#include "MultHandler.h"
#include "VertexHandler.h"
#include "DetectorHandler.h"
#include <vector>
#include <string>

class Run: public TObject
{
/*
 *  Class for executing a Run
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing address of yaml file for configurations
 * 
 */
public:
    Run() = default;
    Run(TString cfgFileName);
    //~Run() {for(auto&i : fDetectors) delete i;}

private:
    void Start(), TreeSettings();
    YAML::Node fConfigFile;
    string fOutFileName;
    TTree fTreeGen;
    TTree fTreeRec;
    unsigned fNEvents;
    int fAutoSaveOpt, fAutoFlushOpt;
    
    MultHandler fMultHandler;               //!Multiplicity info
    AngularHandler fAngularHandler;         //!Angular distribution settings
    VertexHandler fVertexHandler;           //!Vertex dispersion info
    DetectorHandler fDetectorHandler;       //Detector info

    bool fVerbose;

    ClassDef(Run, 1)
};


#endif