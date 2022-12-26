#ifndef MATERIALBUDGET_H
#define MATERIALBUDGET_H

#include<iostream>
#include"TRandom3.h"
#include "TFile.h"
#include "TTree.h" 
#include "TBranch.h"
#include "Particle.h"

class MaterialBudget : public TObject
{

public:

    using fPoint = struct{
        bool isIntersection=false; double x = 0, y = 0, z = 0, phi = 0;     //no need for theta in cilindrical coordinates
        void print() {cout<<"isIntersection: "<<isIntersection<<"\tx: "<<x<<"\ty: "<<y<<"\tz: "<<z<<"\tphi: "<<phi<<endl;}
    };

    MaterialBudget();                                                       // default constructor
    MaterialBudget(double thickness, double radius, double length, double density, int z, int a, bool multscat);
    MaterialBudget(double thickness, double radius, double length, string material, bool multscat);
    
    double GetThickness() const                               {return fThickness;}
    double GetRadius() const                               {return fRadius;}
    double GetLength() const                               {return fLength;}          
    vector<double> GetGeometry() const                     {return {fThickness,fRadius,fLength};}
    double GetDensity() const                              {return fDensity;}
    vector<int> GetFeatures() const                        {return {fZ, fA};}
    bool GetStatus() const                                 {return fMultScat;}

    virtual fPoint GetIntersection(const Particle* particle, bool);

    MaterialBudget& SetStatus(bool multscat)               {fMultScat = multscat; return *this;}
    MaterialBudget& SetStatus(vector<bool> status);
    MaterialBudget& SetGeometry(double thickness, double radius, double length){fThickness = thickness; fRadius = radius; fLength = length; return *this;}
    MaterialBudget& SetMaterial(double density, int z, int a) {fDensity = density; fZ = z; fA = a; return *this;}
    MaterialBudget& SetMaterial(string material);

    virtual void Interaction(Particle* part);
    virtual bool IsDetector() const                        {return false;}

    virtual void FillTree(TTree& gentree, TTree& rectree){};
    Particle* MultScattering(Particle* part); 

    bool operator<(const MaterialBudget& a)               {return GetRadius()<a.GetRadius();}
    bool operator>(const MaterialBudget& a)               {return GetRadius()>a.GetRadius();}

protected:
    double fThickness;
    double fRadius;
    double fLength;
    double fDensity;
    int fZ;
    int fA;
    bool fMultScat;

    double ComputePhi(double x, double y);

    ClassDef(MaterialBudget, 1)
};

#endif



