#ifndef EVENT_UNIFORMDISTRIBUTION_H
#define EVENT_UNIFORMDISTRIBUTION_H
#include "Event.h"

class Event_UniformDistribution : public Event
{
    public:
        Event_UniformDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z, TTree& gentree, TTree& rectree);
    private:
        virtual void CreateParticles(double x, double y, double z); 
};

#endif