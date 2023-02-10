#ifndef MULTHANDLER_H
#define MULTHANDLER_H

#include "Riostream.h"
#include "yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TFile.h"

class MultHandler
{
/*
 *  Class for handling multiplicity distribution
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing address of yaml file for configurations
 * 
 */
public:
    MultHandler() = default;
    MultHandler(YAML::Node ConfigFile);

    unsigned GetMultiplicity()          {return (this->*fMultFunction)();}

private:
    YAML::Node fConfigFile;
    
    unsigned GetConstMult()             {return fConstMult;}
    unsigned GetUniformMult()           {return gRandom->Integer(fMultRange[1]-fMultRange[0]+1) + fMultRange[0];}
    inline unsigned GetCustomMult();
    inline void CheckRange();

    //Multiplicity info
    unsigned fConstMult;
    string fMultType,fMultFile,fMultHistoName;
    vector<unsigned> fMultRange;
    TH1D* fMultHisto;
    unsigned (MultHandler::*fMultFunction)() = &MultHandler::GetConstMult;
};



#endif