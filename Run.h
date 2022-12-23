#ifndef RUN_H
#define RUN_H

#include "Event.h"
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

    vector<Event::fVertMult> Config;
    vector<MaterialBudget::fPoint>   GenHits1,GenHits2, RecHits1,RecHits2;
    vector<MaterialBudget::fPoint>*  ptrGenHits1=&GenHits1;
    vector<MaterialBudget::fPoint>*  ptrGenHits2=&GenHits2;
    vector<MaterialBudget::fPoint>*  ptrRecHits1=&RecHits1;
    vector<MaterialBudget::fPoint>*  ptrRecHits2=&RecHits2;

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
    vector<MaterialBudget*> fDetectors;
    vector<bool> fIsDetector;
    vector<double> fRadii, fThickness, fLength;
    vector<string> fMaterial;

    ClassDef(Run, 1)
};


#endif