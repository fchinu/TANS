#ifndef EVENT_H
#define EVENT_H

#include <Riostream.h>
#include <TSystem.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <string>
#include <vector>
#include "TStopwatch.h"
#include "Particle.h"
#include "MaterialBudget.h"
#include "Detector.h"
#include "TFile.h"
#include "TTree.h" 
#include "TBranch.h"

class Event : public TObject
{
public:

    using fVertMult = struct{
        unsigned int multiplicity; double x, y, z;     //no need for theta in cilindrical coordinates
        void print() {cout<<"Molteplicita': "<< multiplicity <<"\t Coordinate vertice: x: "<< x <<", y: "<< y <<", z: "<< z <<endl;}
    };

    Event(vector<MaterialBudget*>, unsigned int, double, double, double);
    void ProcessingEvent(TTree&, TTree&, vector<fVertMult>*, vector<MaterialBudget::fPoint>*, vector<MaterialBudget::fPoint>*,
                            vector<MaterialBudget::fPoint>*, vector<MaterialBudget::fPoint>*);
    void EventVisual(vector<Particle*> particles);
    void FillTree(TTree& gentree, TTree& rectree, int j);  // privata?

private:
    vector<Particle*> fParticles;
    vector<MaterialBudget*> fDetectors;
    fVertMult fConfig;
    
    ClassDef(Event, 1)

};

#endif