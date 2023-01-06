#include "Event_ConstDistribution.h"

Event_ConstDistribution::Event_ConstDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z,vector<double> direction, TTree& gentree, TTree& rectree):
fDirection(direction)
{   
    fDetectors = detectors;
    fMultiplicity = multiplicity;
    CreateParticles(x,y,z);
    ProcessEvent(gentree, rectree);
}

void Event_ConstDistribution::CreateParticles(double x, double y, double z)
{
    while (fParticles.size()<fMultiplicity)          //Creates particles
    {
        Particle* temp = new Particle({x,y,z},fDirection);
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