#ifndef VERTEXHANDLER_H
#define VERTEXHANDLER_H

#include "Riostream.h"
#include "yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TFile.h"

class VertexHandler
{
/*
 *  Class for handling vertex creation
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing address of yaml file for configurations
 * 
 */
public:
    VertexHandler() = default;
    VertexHandler(YAML::Node ConfigFile);

    double GetXVertex()             {return gRandom->Gaus(0,fSigmaX);}
    double GetYVertex()             {return gRandom->Gaus(0,fSigmaY);}
    double GetZVertex()             {return (this->*fZFunction)();}

private:
    YAML::Node fConfigFile;
    
    double GetGausZ();
    double GetUniformZ()           {return (gRandom->Rndm()*(fRangeZ[1]-fRangeZ[0]) + fRangeZ[0]);} //TODO: add case MultRange is not defined



    //Vertex dispersion info
    string fDistrTypeZ;
    vector<double> fRangeZ;
    double fSigmaX,fSigmaY,fSigmaZ;

    double (VertexHandler::*fZFunction)() = &VertexHandler::GetGausZ;
};



#endif