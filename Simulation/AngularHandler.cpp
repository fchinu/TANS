#include "AngularHandler.h"

AngularHandler::AngularHandler(YAML::Node ConfigFile):
fConfigFile(ConfigFile),
//Angular ditribution settings
fDistributionType(fConfigFile["AngularDistr"]["DistrType"].as<std::string>()),
fConstDirection(fConfigFile["AngularDistr"]["DistrConst"].IsNull() ? std::vector<double>{} : fConfigFile["AngularDistr"]["DistrConst"].as<std::vector<double> >()),
fDistributionFile(fConfigFile["AngularDistr"]["DistrFile"].as<std::string>()),
fDistributionHistoName(fConfigFile["AngularDistr"]["DistrHisto"].as<std::string>()),
fSaveEta(fConfigFile["AngularDistr"]["SaveEta"].as<bool>()),
fSavePhi(fConfigFile["AngularDistr"]["SavePhi"].as<bool>())
{
    if (fDistributionType.find("kCustom") != std::string::npos)
    {
        TFile* infile = TFile::Open(fDistributionFile.c_str());
        fEtaHisto = (TH1D*)infile->Get(fDistributionHistoName.c_str());
        fEtaHisto->SetDirectory(0);
        infile->Close();
        fEtamax=fEtaHisto->GetXaxis()->GetXmax();
        fEtamin=fEtaHisto->GetXaxis()->GetXmin();
        ComputeThetas();
        fDistributionFunction=&AngularHandler::GetCustomDistribution;
    }
    else if (fDistributionType.find("kUniform") != std::string::npos)
        fDistributionFunction=&AngularHandler::GetUniformDistribution;

    else if (fDistributionType.find("kConst") != std::string::npos)
    {
        if (fConstDirection.size() != 3)
        {
            cout<<"\033[93mWarning: DistrConst is not a vector (dim!=3), setting it to (1,0,0)\033[0m \n";
            fConstDirection={1,0,0};
        }
        fDistributionFunction=&AngularHandler::GetConstDistribution;
    }
    else 
    {
        cout<<"\033[93mWarning: DistrType is not kConst, kCustom or kUniform, setting it to kConst with direction (1,0,0)\033[0m \n";
        fConstDirection={1,0,0};
        fDistributionFunction=&AngularHandler::GetConstDistribution; 
    }

}

inline std::vector<double> AngularHandler::GetUniformDistribution()
{
    double x,y,z;
    gRandom->Sphere(x,y,z,1);
    return {x,y,z};
}

inline std::vector<double> AngularHandler::GetCustomDistribution()
{
    double eta=fEtaHisto->GetRandom(), phi=2*TMath::Pi()*gRandom->Rndm();
    double theta=GetThetaFromVector(eta);
    return {TMath::Sin(theta)*TMath::Cos(phi),TMath::Sin(theta)*TMath::Sin(phi),TMath::Cos(theta)};
}

double AngularHandler::GetThetaFromEta(double eta)
{
/*
*  Function to transform pseudorapidity distribution to theta distribution
*/
    return 2*TMath::ATan(exp(-eta));
}

void AngularHandler::ComputeThetas()
{
    double step= (fEtamax-fEtamin)/fNbinsEta;
    for (unsigned i=0;i<fNbinsEta;i++)
        fThetas.push_back(GetThetaFromEta(fEtamin+step*i));
}