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
        TH1D* histopseudorap = (TH1D*)infile->Get(fDistributionHistoName.c_str());
        histopseudorap->SetDirectory(0);
        infile->Close();
        SetThetaFromEta(histopseudorap);
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
    double theta=fThetaHisto->GetRandom(), phi=2*TMath::Pi()*gRandom->Rndm();
    return {TMath::Sin(theta)*TMath::Cos(phi),TMath::Sin(theta)*TMath::Sin(phi),TMath::Cos(theta)};
}

void AngularHandler::SetThetaFromEta(TH1D* histopseudorap)
{
/*
*  Function to transform pseudorapidity distribution to theta distribution
*/
    double mintheta=2*TMath::ATan(exp(-(histopseudorap->GetBinLowEdge(histopseudorap->GetNbinsX()) + histopseudorap->GetBinWidth(histopseudorap->GetNbinsX()))));
    cout<<"Mintetha "<<mintheta<<endl;
    double maxtheta=2*TMath::ATan(exp(-(histopseudorap->GetBinLowEdge(1))));  
    cout<<"Maxtetha "<<maxtheta<<endl;
    fThetaHisto = new TH1D("Theta","Theta",histopseudorap->GetNbinsX(),mintheta,maxtheta);
    for (int i=1; i<=histopseudorap->GetNbinsX();i++)
    {
        double theta=fThetaHisto->GetBinCenter(i);
        double eta=-TMath::Log(TMath::Tan(theta/2));
        double etadensity = histopseudorap->GetBinContent(histopseudorap->FindBin(eta));
        double derivative=TMath::Abs(1/TMath::Cos(theta/2)/TMath::Sin(theta/2));
        fThetaHisto->Fill(fThetaHisto->GetBinCenter(i),etadensity*derivative);
    }
}