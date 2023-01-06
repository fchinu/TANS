#include "Event_CustomDistribution.h"

Event_CustomDistribution::Event_CustomDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z,TH1D* pseudorap, TTree& gentree, TTree& rectree):
fPseudorapDistr(pseudorap)
{   
    fDetectors = detectors;
    fMultiplicity = multiplicity;
    CreateParticles(x,y,z);
    ProcessEvent(gentree, rectree);
}

void Event_CustomDistribution::CreateParticles(double x, double y, double z)
{
    while (fParticles.size()<fMultiplicity)          //Creates particles
    {
        Particle* temp = new Particle({x,y,z},fPseudorapDistr);
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