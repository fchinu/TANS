#ifndef ANGULARHANDLER_H
#define ANGULARHANDLER_H

#include "Riostream.h"
#include "../yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TFile.h"
#include "TMath.h"

class AngularHandler
{
/*
 *  Class for handling particles' angular distribution
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing address of yaml file for configurations
 * 
 */
public:
    AngularHandler() = default;
    AngularHandler(YAML::Node ConfigFile);
    ~AngularHandler() {delete fEtaHisto;}

    bool SaveEta()         {return fSaveEta;}
    bool SavePhi()         {return fSavePhi;}
    std::vector<double> GetDirection()                         {return (this->*fDistributionFunction)();}

private:
    YAML::Node fConfigFile;

    std::vector<double> GetConstDistribution()             {return fConstDirection;}
    inline std::vector<double> GetUniformDistribution();
    inline std::vector<double> GetCustomDistribution();
    double GetThetaFromEta(double eta);
    double GetThetaFromVector(double eta)                  {return fThetas[int((eta-fEtamin)*fNbinsEta/(fEtamax-fEtamin))];}
    void ComputeThetas();

    double fEtamin,fEtamax;
    unsigned fNbinsEta=100000;
    vector<double> fThetas= {};

    //Distribution info
    std::vector<double> fConstDirection;
    string fDistributionType,fDistributionFile,fDistributionHistoName;
    vector<unsigned> fDistributionRange;
    TH1D* fEtaHisto = nullptr;
    std::vector<double> (AngularHandler::*fDistributionFunction)() = &AngularHandler::GetConstDistribution;
    bool fSaveEta, fSavePhi;
};



#endif