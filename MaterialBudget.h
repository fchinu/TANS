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
    
    const double GetHeight() const                               {return fHeight;}
    const double GetRadius() const                               {return fRadius;}
    const double GetLength() const                               {return fLength;}          
    const vector<double> GetGeometry() const                     {return {fHeight,fRadius,fLength};}
    const double GetDensity() const                              {return fDensity;}
    const vector<int> GetFeatures() const                        {return {fZ, fA};}

    MaterialBudget* SetGeometry(double height, double radius, double length);
    MaterialBudget* SetMaterial(double density, int z, int a);
    MaterialBudget* SetMaterial(string material);

    Particle* MultScattering(Particle* part); 

    virtual void Smearing() = 0;

    // friend bool operator>(const MaterialBudget& a, const MaterialBudget& b);

    // bool operator<(const MaterialBudget& a);
    // {if((this->GetRadius())>a.fRadius){return false;} return true;}
    // bool operator>(const MaterialBudget& a);
    // {if((this->GetRadius())<b.fRadius){return false;} return true;}

    /*bool operator <(const MaterialBudget& d) {
        if(fRadius < d.fRadius) {
            return true;
        }
        
        return false;
    }

    bool operator >(const MaterialBudget& e) {
        if(fRadius > e.fRadius) {
            return true;
        }
        
        return false;
    }*/

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



