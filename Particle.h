#ifndef PARTICLE_H
#define PARTICLE_H

#include "TObject.h"
#include "TMath.h"
#include "Riostream.h"
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

    const vector<double> GetDirection()                         {return {fPx,fPy,fPz};}
    const vector<double> GetPolarDirection()                    {return {fTheta,fPhi};}
    const vector<double> GetPoint()                             {return {fX,fY,fZ};}

    Particle* SetDirection(vector<double> direction)            {this->SetDirection(direction[0],direction[1],direction[2]); return this;}
    Particle* SetDirection(double *direction)                   {this->SetDirection(direction[0],direction[1],direction[2]); return this;}
    Particle* SetDirection(double px, double py, double pz);
    Particle* SetPoint(vector<double> point)                    {this->SetPoint(point[0],point[1],point[2]); return this;}
    Particle* SetPoint(double *point)                           {this->SetPoint(point[0],point[1],point[2]); return this;}
    Particle* SetPoint(double x, double y, double z)            {fX=x; fY=y; fZ=z; return this;}


private:
    double fX,fY,fZ;
    double fPx,fPy,fPz;
    double fTheta, fPhi;

    ClassDef(Particle, 1)
};

#endif