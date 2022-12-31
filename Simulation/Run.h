#ifndef RUN_H
#define RUN_H

#include "Event.h"
#include"Reconstruction.h"
#include "yaml-cpp/yaml.h"
#include "TH1D.h"
#include <vector>
#include <string>

class Run: public TObject
{
public:
    Run(TString cfgFileName);
    TTree TreeGen;
    TTree TreeRec;

private:
    void RunConstMult(), RunUniformMult(), RunCustomMult(), CreateDetectors();
    YAML::Node fConfigFile;
    string fOutFileName;

    //Multiplicity info
    unsigned fNEvents,fConstMult;
    string fMultType,fMultFile,fMultHisto;
    vector<unsigned> fMultRange;

    //Vertex dispersion info
    double fSigmaZ,fSigmaX,fSigmaY;

    //Detectors info
    vector<MaterialBudget*> fDetectors;
    vector<bool> fIsDetector;
    vector<double> fRadii, fThickness, fLength;
    vector<string> fMaterial;

    //Reconstruction & simulation options
    bool fDoMultScattering, fDoSmearing;

    ClassDef(Run, 1)
};


#endif