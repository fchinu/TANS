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

class Run: public TObject
{
public:
    Run(TString cfgFileName);

private:
    void RunConstMult(), RunUniformMult(), RunCustomMult(), CreateDetectors();
    YAML::Node fConfigFile;

    //Multiplicity info
    unsigned fNEvents,fConstMult;
    string fMultType,fMultFile,fMultHisto;
    vector<unsigned> fMultRange;

    //Vertex dispersion info
    double fSigmaZ,fSigmaX,fSigmaY;

    //Detectors info
    vector<MaterialBudget> fDetectors;
    vector<bool> fIsDetector;
    vector<double> fRadii, fThickness, fLenght;
    vector<string> fMaterial;
};


#endif