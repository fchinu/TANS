#include <Riostream.h>
#include <TSystem.h>
#include <TGeoManager.h>
#include <TMath.h>
#include <string>
#include <vector>
#include "Particle.h"
#include "Detector.h"

void Event_Visual(vector<Particle*> particles);
void Event(unsigned multiplicity, double x=0, double y=0,double z= 0);