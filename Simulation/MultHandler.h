#ifndef MULTHANDLER_H
#define MULTHANDLER_H

#include "Riostream.h"
#include "yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TFile.h"

class MultHandler
{
public:
    MultHandler() = default;
    MultHandler(YAML::Node ConfigFile);

    unsigned GetMultiplicity()          {return (this->*fMultFunction)();}

private:
    YAML::Node fConfigFile;
    
    unsigned GetConstMult()             {return fConstMult;}
    unsigned GetUniformMult()           {return gRandom->Integer(fMultRange[1]-fMultRange[0]+1) + fMultRange[0];} //TODO: add case MultRange is not defined
    inline unsigned GetCustomMult();

    //Multiplicity info
    unsigned fConstMult;
    string fMultType,fMultFile,fMultHistoName;
    vector<unsigned> fMultRange;
    TH1D* fMultHisto;
    unsigned (MultHandler::*fMultFunction)() = &MultHandler::GetConstMult;
};



#endif