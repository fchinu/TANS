#ifndef DETECTOR_H
#define DETECTOR_H

#include "Particle.h"
#include "MaterialBudget.h"

class Detector : public MaterialBudget
{
public:
    using fPoint = struct{
        bool isIntersection=false; double x = 0, y = 0, z = 0, phi = 0;     //no need for theta in cilindrical coordinates
        void print() {cout<<"isIntersection: "<<isIntersection<<"\tx: "<<x<<"\ty: "<<y<<"\tz: "<<z<<"\tphi: "<<phi<<endl;}
    };

    Detector();
    ~Detector();

    virtual bool IsRiv() {return true;}
    fPoint GetIntersection(const Particle* particle,bool fill=true);
    vector<fPoint> GetTrueHits() const  {return fTrueHit;}
    vector<fPoint> GetRecoHits() const  {return fRecoHit;}
    void Smearing();
private:
    vector<fPoint> fTrueHit, fRecoHit;
    double fMuAngular=0, fSigmaAngular=3e-3;    //Parameters for smearing (in cm)
    double fMuZ=0, fSigmaZ=1.2e-2;              //Parameters for smearing (in cm)

    double ComputePhi(double x, double y);
};

#endif