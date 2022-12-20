#include "Detector.h"
#include "TStopwatch.h"
#include "Event_Visual.h"

void Event(unsigned multiplicity, double x, double y,double z)
{
    vector<Particle*> p;
    Detector det;
    det.SetGeometry(1,5,0.27);
    while (p.size()<multiplicity)
    {
        p.push_back(new Particle({x,y,z},gRandom));
        det.GetIntersection(p.back());
    }
    vector<Detector::fPoint> TrueHits = det.GetTrueHits();
    if (TrueHits.size()>0)
    {
    det.Smearing();
    vector<Detector::fPoint> RecoHits = det.GetRecoHits();
    }

    //RecoHits[0].print();
    Event_Visual(p);
    for (auto i:p)
        delete i;
}