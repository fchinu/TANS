#ifndef EVENT_CUSTOMDISTRIBUTION_H
#define EVENT_CUSTOMDISTRIBUTION_H
#include "Event.h"

class Event_CustomDistribution : public Event
{
    public:
        Event_CustomDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z,TH1D* pseudorap, TTree& gentree, TTree& rectree);
    private:
        virtual void CreateParticles(double x, double y, double z);
        TH1D* fPseudorapDistr = nullptr; 
};

#endif