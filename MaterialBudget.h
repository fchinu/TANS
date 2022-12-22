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
    MaterialBudget(double height, double radius, double length, double density, int z, int a);
    MaterialBudget(double height, double radius, double length, string material);
    
    double GetHeight() const                               {return fHeight;}
    double GetRadius() const                               {return fRadius;}
    double GetLength() const                               {return fLength;}          
    vector<double> GetGeometry() const                     {return {fHeight,fRadius,fLength};}
    double GetDensity() const                              {return fDensity;}
    vector<int> GetFeatures() const                        {return {fZ, fA};}
    virtual fPoint GetIntersection(const Particle* particle, bool);

    MaterialBudget& SetGeometry(double height, double radius, double length);
    MaterialBudget& SetMaterial(double density, int z, int a);
    MaterialBudget& SetMaterial(string material);

    virtual void Interaction(Particle* part);
    virtual bool IsDetector() const                        {return false;}

    virtual void FillTree(TTree& gentree, TTree& rectree){};
    Particle* MultScattering(Particle* part); 

    bool operator<(const MaterialBudget& a)               {return GetRadius()<a.GetRadius();}
    bool operator>(const MaterialBudget& a)               {return GetRadius()>a.GetRadius();}

protected:
    double fHeight;
    double fRadius;
    double fLength;
    double fDensity;
    int fZ;
    int fA;

    double ComputePhi(double x, double y);

    ClassDef(MaterialBudget, 1)
};

#endif



