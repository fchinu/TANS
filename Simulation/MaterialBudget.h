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
/*
 *  Class for describing a cylindrical material budget
 *  -------------------------
 *  Parameters:
 *  thickness: double 
 *      Thickness of the layer
 * 
 *  radius: double 
 *      Radius of the layer
 * 
 *  lenght: double 
 *      Lenght of the layer
 * 
 *  density: double 
 *      Density of the layer
 * 
 *  z: int 
 *      Atomic number of the layer's material
 * 
 *  a: int 
 *      Mass number of the layer's material
 * 
 *  material: string
 *      String containing material symbol
 *      Currently only "Si" and "Be" are supported
 *  
 *  multscat: bool
 *      Enables multiple scattering
 * 
 */
public:

    //Struct for containing intersections with the material budget
    using fPoint = struct{
        bool isIntersection=false; double x = 0, y = 0, z = 0, phi = 0;     //no need for theta in cilindrical coordinates
        void print() {cout<<"isIntersection: "<<isIntersection<<"\tx: "<<x<<"\ty: "<<y<<"\tz: "<<z<<"\tphi: "<<phi<<endl;}
    };

    MaterialBudget();                                                       
    MaterialBudget(double thickness, double radius, double length, double density, int z, int a, bool multscat);
    MaterialBudget(double thickness, double radius, double length, string material, bool multscat);
    
    double          GetThickness()  const        {return fThickness;}
    double          GetRadius()     const        {return fRadius;}
    double          GetLength()     const        {return fLength;}          
    vector<double>  GetGeometry()   const        {return {fThickness,fRadius,fLength};}
    double          GetDensity()    const        {return fDensity;}
    vector<int>     GetFeatures()   const        {return {fZ, fA};}
    bool            GetStatus()     const        {return fMultScat;}

    virtual fPoint GetIntersection(const Particle* particle);
    //Used for detector class to not call GetIntersection twice per particle
    virtual fPoint GetLastIntersection(const Particle* particle)                    {return GetIntersection(particle);}

    MaterialBudget& SetStatus(bool multscat)                                        {fMultScat = multscat; return *this;}
    MaterialBudget& SetStatus(vector<bool> status);
    MaterialBudget& SetGeometry(double thickness, double radius, double length)     {fThickness = thickness; fRadius = radius; fLength = length; return *this;}
    MaterialBudget& SetMaterial(double density, int z, int a)                       {fDensity = density; fZ = z; fA = a; return *this;}
    MaterialBudget& SetMaterial(string material);

    virtual void Interaction(Particle* part);
    virtual bool IsDetector() const                         {return false;}

    virtual void SetBranchAddress(TTree& gentree,TTree& rectree, unsigned countdet) {}
    virtual void FillData(Particle* part)                   {}
    virtual void ClearData()                                {}
    Particle* MultScattering(Particle* part); 

    //Used to sort the material budgets to properly do multiple scattering
    bool operator<(const MaterialBudget& a)                 {return GetRadius()<a.GetRadius();}
    bool operator>(const MaterialBudget& a)                 {return GetRadius()>a.GetRadius();}

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



