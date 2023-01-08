#include "VertexHandler.h"

VertexHandler::VertexHandler(YAML::Node ConfigFile):
fConfigFile(ConfigFile),

//Vertex dispersion settings
fDistrTypeZ(fConfigFile["Vertex"]["DistrTypeZ"].as<std::string>()),
fRangeZ(fConfigFile["Vertex"]["RangeZ"].IsNull() ?  std::vector<double>{} : fConfigFile["Vertex"]["RangeZ"].as<std::vector<double> >()),
fSigmaX(fConfigFile["Vertex"]["SigmaX"].as<double>()),
fSigmaY(fConfigFile["Vertex"]["SigmaY"].as<double>()),
fSigmaZ(fConfigFile["Vertex"]["SigmaZ"].as<double>())
{
    if (fDistrTypeZ.find("kUniform") != std::string::npos)
        fZFunction=&VertexHandler::GetUniformZ;
    else 
        fZFunction=&VertexHandler::GetGausZ;
}

inline double VertexHandler::GetGausZ()
{
    double z;
    if (fRangeZ.size()>0)
    {
        do {
            z=gRandom->Gaus(0,fSigmaZ);
        }while (z < fRangeZ[0] || z > fRangeZ[1]);
    }
    else
        z=gRandom->Gaus(0,fSigmaZ);
    return z;
}