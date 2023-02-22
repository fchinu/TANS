#ifndef DETECTORHANDLER_H
#define DETECTORHANDLER_H

#include "Riostream.h"
#include "../yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TObject.h"
#include "TRandom.h"
#include "TFile.h"
#include "TMath.h"
#include "Detector.h"

class DetectorHandler : public TObject
{

public:
    DetectorHandler() = default;
    DetectorHandler(YAML::Node ConfigFile);
    ~DetectorHandler() {for(auto&i : fDetectors) delete i;};

    std::vector<MaterialBudget*> GetDetectors()         {return fDetectors;}
    std::vector<bool> GetIsDetector()                   {return fIsDetector;}
    std::vector<double> GetRadii()                      {return fRadii;}


private:
    YAML::Node fConfigFile;                                 //!

    void GenerateDetectors();

    //Detectors info
    std::vector<MaterialBudget*> fDetectors;                //!
    std::vector<bool> fIsDetector;                          //!
    std::vector<double> fRadii;                             //
    std::vector<double> fThickness, fLength;             //!
    std::vector<string> fMaterial;                         //!

    //Reconstruction & simulation options
    bool fDoMultScattering, fDoSmearing, fDoNoise;          //!
    int fMeanNoise;                                         //!

    bool fVerbose;                                          //!

    ClassDef(DetectorHandler, 1)
};

#endif