#ifndef EVENT_H
#define EVENT_H

#include "Particle.h"
#include "MaterialBudget.h"
#include <Riostream.h>
#include <TSystem.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <string>
#include <vector>
#include "Detector.h"
#include "TStopwatch.h"

class Event : public TObject
{
public:

    using fVertMult = struct{
        unsigned int multiplicity; double x, y, z;     //no need for theta in cilindrical coordinates
        void print() {cout<<"Molteplicita': "<< multiplicity <<"\t Coordinate vertice: x: "<< x <<", y: "<< y <<", z: "<< z <<endl;}
    };

    Event(vector<MaterialBudget*>, unsigned int, double, double, double);
    // void EventSignal();
    void EventVisual(vector<Particle*> particles);
    void FillTree(MaterialBudget* detector);  // privata?

private:
    vector<Particle*> fParticles;
    vector<MaterialBudget*> fDetectors;
    fVertMult fConfig;
    
    ClassDef(Event, 1)

};

#endif