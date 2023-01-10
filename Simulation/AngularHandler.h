#ifndef ANGULARHANDLER_H
#define ANGULARHANDLER_H

#include "Riostream.h"
#include "yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TFile.h"
#include "TMath.h"

class AngularHandler
{
public:
    AngularHandler() = default;
    AngularHandler(YAML::Node ConfigFile);
    ~AngularHandler() {delete fThetaHisto;}

    std::vector<double> GetDirection()                         {return (this->*fDistributionFunction)();}

private:
    YAML::Node fConfigFile;

    std::vector<double> GetConstDistribution()             {return fConstDirection;}
    inline std::vector<double> GetUniformDistribution();    //TODO: add case DistributionRange is not defined
    inline std::vector<double> GetCustomDistribution();
    void SetThetaFromEta(TH1D* histopseudorap);

    //Distributioniplicity info
    std::vector<double> fConstDirection;
    string fDistributionType,fDistributionFile,fDistributionHistoName;
    vector<unsigned> fDistributionRange;
    TH1D* fThetaHisto = nullptr;
    std::vector<double> (AngularHandler::*fDistributionFunction)() = &AngularHandler::GetConstDistribution;
};



#endif