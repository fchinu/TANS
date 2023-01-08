#include "MultHandler.h"

MultHandler::MultHandler(YAML::Node ConfigFile):
fConfigFile(ConfigFile),
//Multiplicity settings
fMultType(fConfigFile["Multiplicity"]["MultType"].as<std::string>()),
fConstMult(fConfigFile["Multiplicity"]["MultConst"].IsNull() ? 0 : fConfigFile["Multiplicity"]["MultConst"].as<unsigned>()),
fMultFile(fConfigFile["Multiplicity"]["MultFile"].as<std::string>()),
fMultHistoName(fConfigFile["Multiplicity"]["MultHistoName"].as<std::string>()),
fMultRange(fConfigFile["Multiplicity"]["MultRange"].IsNull() ?  std::vector<unsigned>{} : fConfigFile["Multiplicity"]["MultRange"].as<std::vector<unsigned> >())
{
    if (fMultType.find("kConst") != std::string::npos)
        fMultFunction=&MultHandler::GetConstMult;
    else if (fMultType.find("kUniform") != std::string::npos)
        fMultFunction=&MultHandler::GetUniformMult;
    else if (fMultType.find("kCustom") != std::string::npos)
    {
        TFile* infile = TFile::Open(fMultFile.c_str());
        fMultHisto = (TH1D*)infile->Get(fMultHistoName.c_str());
        fMultHisto -> SetDirectory(0);
        infile->Close();
        fMultFunction=&MultHandler::GetCustomMult;
    }
}

inline unsigned MultHandler::GetCustomMult()
{
    unsigned mult;
    if (fMultRange.size()>0)
    {
        do {
            mult = fMultHisto->GetRandom();
        }while (mult < fMultRange[0] || mult > fMultRange[1]);
    }
    else
        mult = fMultHisto->GetRandom();
    return mult;
}