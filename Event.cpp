#include "Detector.h"
#include "TStopwatch.h"
#include "Event_Visual.h"

void Event(unsigned multiplicity, double x=0, double y=0,double z= 0)
{
    TStopwatch w;
    w.Start();
    vector<Particle*> p;
    Detector det;
    while (p.size()<multiplicity)
    {
        p.push_back(new Particle({x,y,z},gRandom));
        det.GetIntersection(p.back());
    }
    vector<Detector::fPoint> TrueHits = det.GetTrueHits();
    det.Smearing();
    vector<Detector::fPoint> RecoHits = det.GetRecoHits();
    RecoHits[0].print();
    Event_Visual(p);
    for (auto i:p)
        delete i;
    w.Stop();
    w.Print("u");
}