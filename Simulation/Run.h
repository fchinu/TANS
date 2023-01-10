#ifndef RUN_H
#define RUN_H

#include "Event.h"
#include "yaml-cpp/yaml.h"
#include "TH1D.h"
#include "MultHandler.h"
#include "VertexHandler.h"
#include <vector>
#include <string>

class Run: public TObject
{
/*
 *  Class for executing a Run
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing yaml file for configurations
 * 
 */
public:
    Run() = default;
    Run(TString cfgFileName);
    ~Run() {for(auto&i : fDetectors) delete i;}

private:
    void Start(), CreateDetectors();
    YAML::Node fConfigFile;
    string fOutFileName;
    TTree fTreeGen;
    TTree fTreeRec;
    unsigned fNEvents;
    //Multiplicity info
    MultHandler fMultHandler;

    //Angular distribution settings
    AngularHandler fAngularHandler;

    //Vertex dispersion info
    VertexHandler fVertexHandler;

    //Detectors info
    vector<MaterialBudget*> fDetectors;
    vector<bool> fIsDetector;
    vector<double> fRadii, fThickness, fLength;
    vector<string> fMaterial;

    //Reconstruction & simulation options
    bool fDoMultScattering, fDoSmearing, fDoNoise;
    int fMeanNoise;

    bool fVerbose;

    ClassDef(Run, 1)
};


#endif