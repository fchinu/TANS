#ifndef PARTICLE_H
#define PARTICLE_H

#include "TObject.h"
#include "TMath.h"
#include "Riostream.h"
#include "TRandom.h"
#include <vector>

class Particle : public TObject
{
/*
 *  Class for describing particles
 */

public:
    Particle ();                                                                    //default constructor
    Particle (double x, double y, double z, double px, double py, double pz);
    Particle (double* point, double* direction);                                    //array initialisation
    Particle (vector<double> point, vector<double> direction);                      //vector initialisation
    Particle (vector<double> point, TRandom* rndmptr);                              //random initialisation

    vector<double> GetDirection()       const             {return {fPx,fPy,fPz};}
    vector<double> GetPolarDirection()  const             {return {fTheta,fPhi};}
    vector<double> GetPoint()           const             {return {fX,fY,fZ};}

    Particle& SetDirection(vector<double> direction)            {SetDirection(direction[0],direction[1],direction[2]); return *this;}
    Particle& SetDirection(double *direction)                   {SetDirection(direction[0],direction[1],direction[2]); return *this;}
    Particle& SetDirection(double px, double py, double pz);
    Particle& SetPoint(vector<double> point)                    {SetPoint(point[0],point[1],point[2]); return *this;}
    Particle& SetPoint(double *point)                           {SetPoint(point[0],point[1],point[2]); return *this;}
    Particle& SetPoint(double x, double y, double z)            {fX=x; fY=y; fZ=z; return *this;}


private:
    double fX,fY,fZ;
    double fPx,fPy,fPz;
    double fTheta, fPhi;
    double ComputePhi(double x, double y);

    ClassDef(Particle, 1)
};

#endif