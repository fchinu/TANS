#ifndef DETECTOR_H
#define DETECTOR_H

#include "Particle.h"
#include "MaterialBudget.h"

class Detector : public MaterialBudget
{
public:

    Detector();
    Detector(double thickness, double radius, double length, string material, bool multscat, bool smearing, bool noise);
    ~Detector();

    virtual bool IsDetector() const {return true;}
    virtual void Interaction(Particle* particle, int& detected, int& notdetected, int& smeared, int& notsmeared);

    Detector& SetStatus(bool smearing, bool noise) {fSmearing = smearing; fNoise = noise; return *this;}
    Detector& SetStatus(vector<bool> status);

    virtual void ClearData(){fTrueHit.clear(); fRecoHit.clear();}
    virtual void FillTree(TTree& gentree, const char* genbranchname, TTree& rectree, const char* rectreename);
    virtual void FillData(Particle*, int& detected, int& notdetected, int& smeared, int& notsmeared);
    virtual MaterialBudget::fPoint GetIntersection(const Particle* particle);
    MaterialBudget::fPoint GetSmearedIntersection(MaterialBudget::fPoint intersection);
    MaterialBudget::fPoint OldGetSmearedIntersection();
    vector<bool> GetStatus() const                      {return {fMultScat, fSmearing, fNoise};}
    vector<MaterialBudget::fPoint> GetTrueHits() const  {return fTrueHit;}
    vector<MaterialBudget::fPoint> GetRecoHits() const  {return fRecoHit;}
    
private:
    bool fSmearing, fNoise;
    // std::vector<MaterialBudget::fPoint>* fTrueHitPtr, fRecoHitPtr;
    std::vector<MaterialBudget::fPoint> fTrueHit, fRecoHit;
    double fMuAngular=0, fSigmaAngular=3e-3;    // Parameters for smearing (in cm)
    double fMuZ=0, fSigmaZ=1.2e-2;              // Parameters for smearing (in cm)

    // double ComputePhi(double x, double y);
};

#endif