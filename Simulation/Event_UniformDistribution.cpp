#include "Event_UniformDistribution.h"

Event_UniformDistribution::Event_UniformDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z, TTree& gentree, TTree& rectree)
{   
    fDetectors = detectors;
    fMultiplicity = multiplicity;
    CreateParticles(x,y,z);
    ProcessEvent(gentree, rectree);
}

void Event_UniformDistribution::CreateParticles(double x, double y, double z)
{
    while (fParticles.size()<fMultiplicity)          //Creates particles
    {
        Particle* temp = new Particle({x,y,z},gRandom);
        fParticles.push_back(temp);
    }

    //Stores event infos
    fVertMult temp;
    temp.multiplicity = fMultiplicity;
    temp.x = x;
    temp.y = y;
    temp.z = z;
    fConfig.push_back(temp);
}