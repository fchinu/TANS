#ifndef EVENT_CONSTDISTRIBUTION_H
#define EVENT_CONSTDISTRIBUTION_H
#include "Event.h"

class Event_ConstDistribution : public Event
{
    public:
        Event_ConstDistribution(const vector<MaterialBudget*>& detectors, unsigned multiplicity, double x, double y, double z,vector<double> direction, TTree& gentree, TTree& rectree);
    private:
        virtual void CreateParticles(double x, double y, double z); 
        vector<double> fDirection;
};

#endif