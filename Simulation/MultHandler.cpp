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
    if (fMultType.find("kCustom") != std::string::npos)
    {
        TFile* infile = TFile::Open(fMultFile.c_str());
        fMultHisto = (TH1D*)infile->Get(fMultHistoName.c_str());
        fMultHisto -> SetDirectory(0);
        infile->Close();
        fMultFunction=&MultHandler::GetCustomMult;
    }
    else if (fMultType.find("kUniform") != std::string::npos)
    {
        CheckRange();
        fMultFunction=&MultHandler::GetUniformMult;
    }
    else 
        fMultFunction=&MultHandler::GetConstMult;
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

inline bool MultHandler::CheckRange()
{
/*
 *  Function that checks whether MultRange is properly defined
 * 
 */
    if (fMultRange.size()<2)
    {
        cout<<"\033[93mWarning: MultRange size < 2, setting it to [0-50]\033[0m \n";
        fMultRange={0,50};
    }
    if (fMultRange[0]<0 || fMultRange[1]<0)
    {
        cout<<"\033[93mWarning: MultRange has negative values, setting it to [0-50]\033[0m \n";
        fMultRange={0,50};
    }
    if (fMultRange[0]>fMultRange[1])
    {
        cout<<"\033[93mWarning: MultRange is not sorted, setting it to [0-50]\033[0m \n";
        fMultRange={0,50};
    }
}