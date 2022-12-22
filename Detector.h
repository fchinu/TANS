#ifndef DETECTOR_H
#define DETECTOR_H

#include "Particle.h"
#include "MaterialBudget.h"

class Detector : public MaterialBudget
{
public:

    Detector(bool multscat, bool smearing, bool noise);
    ~Detector();

    virtual bool IsDetector() const {return true;}
    virtual void Interaction(Particle* particle);

    void SetStatus(bool multscat, bool smearing, bool noise);

    virtual void FillTree(TTree& gentree, TTree& rectree);
    virtual MaterialBudget::fPoint GetIntersection(const Particle* particle,bool fill=true);
    MaterialBudget::fPoint GetSmearedIntersection();
    vector<bool> GetStatus() const                      {return {fMultScat, fSmearing, fNoise};}
    vector<MaterialBudget::fPoint> GetTrueHits() const  {return fTrueHit;}
    vector<MaterialBudget::fPoint> GetRecoHits() const  {return fRecoHit;}
    
private:
    bool fSmearing, fNoise;
    vector<MaterialBudget::fPoint> fTrueHit, fRecoHit;
    double fMuAngular=0, fSigmaAngular=3e-3;    // Parameters for smearing (in cm)
    double fMuZ=0, fSigmaZ=1.2e-2;              // Parameters for smearing (in cm)

    // double ComputePhi(double x, double y);
};

#endif