#ifndef MATERIALBUDGET_H
#define MATERIALBUDGET_H

#include<iostream>
#include"TRandom3.h"
#include"Particle.h"

class MaterialBudget : public TObject
{

public:
    MaterialBudget();                                                       // default constructor
    MaterialBudget(double height, double radius, double length, double density, int z, int a);
    MaterialBudget(double height, double radius, double length, string material);
    
    double GetHeight() const                               {return fHeight;}
    double GetRadius() const                               {return fRadius;}
    double GetLength() const                               {return fLength;}          
    vector<double> GetGeometry() const                     {return {fHeight,fRadius,fLength};}
    double GetDensity() const                              {return fDensity;}
    vector<int> GetFeatures() const                        {return {fZ, fA};}

    MaterialBudget* SetGeometry(double height, double radius, double length);
    MaterialBudget* SetMaterial(double density, int z, int a);
    MaterialBudget* SetMaterial(string material);

    Particle* MultScattering(Particle* part); 

    bool operator<(const MaterialBudget& a)               {return GetRadius()<a.GetRadius();}
    bool operator>(const MaterialBudget& a)               {return GetRadius()>a.GetRadius();}

private:
    double fHeight;
    double fRadius;
    double fLength;
    double fDensity;
    int fZ;
    int fA;

    ClassDef(MaterialBudget, 1)
};

#endif



