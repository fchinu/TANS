#include "Detector.h"

ClassImp(Detector)

Detector::Detector(bool multscat, bool smearing, bool noise)
{
    SetStatus(multscat, smearing, noise);
}

void Detector::SetStatus(bool multscat, bool smearing, bool noise)
{
    fMultScat = multscat;
    fSmearing = smearing;
    fNoise = noise;
}

void Detector::Interaction(Particle* particle)
{
    fTrueHit.push_back(GetIntersection(particle,1));
    if(fSmearing){
        fRecoHit.push_back(GetSmearedIntersection());
    }
    if(fMultScat){
        MultScattering(particle);
    }
}

MaterialBudget::fPoint Detector::GetIntersection(const Particle* particle, bool fill)
{
    vector<double> direction = particle->GetDirection();
    vector<double> point = particle->GetPoint();

    //Evaluates all different factors to evaluate intersection
    double den = direction[0]*direction[0] + direction[1]*direction[1];
    double b = (point[0]*direction[1] + point[1]*direction[2]);
    double delta = b*b - den * (point[0]*point[0] + point[1]*point[1] - fRadius * fRadius);

    MaterialBudget::fPoint intersection;
    if (delta <= 0)     //particle going along z axis
    {
        intersection.isIntersection=false;
        return intersection;
    }

    double t = (TMath::Sqrt(delta)-b)/den;

    if (point[2] + direction[2] * t < -fHeight/2 || point[2] + direction[2] * t > fHeight/2)       //particle goes outside detector
    {
        intersection.isIntersection=false;
        return intersection;
    } 

    intersection.isIntersection=true;
    intersection.x = point[0] + direction[0] * t;
    intersection.y = point[1] + direction[1] * t;
    intersection.z = point[2] + direction[2] * t;
    intersection.phi = ComputePhi(intersection.x, intersection.y);

    if (fill)
        fTrueHit.push_back(intersection);

    return intersection;
}


MaterialBudget::fPoint Detector::GetSmearedIntersection()
{
    fRecoHit.clear();
    MaterialBudget::fPoint SmearedIntersection;
    for (const auto& i : fTrueHit)
    {
        double ar=gRandom->Gaus(0,fSigmaAngular);
        double zr=gRandom->Gaus(0,fSigmaZ);

        //smearing
        SmearedIntersection.phi = i.phi + ar/fRadius;
        SmearedIntersection.z = i.z + zr;

        if (SmearedIntersection.z < -fHeight/2 || SmearedIntersection.z > fHeight/2)
            continue;
        
        SmearedIntersection.isIntersection=true;
        SmearedIntersection.x = fRadius*TMath::Cos(SmearedIntersection.phi);
        SmearedIntersection.y = fRadius*TMath::Sin(SmearedIntersection.phi);

        fRecoHit.push_back(SmearedIntersection);
    }
    return SmearedIntersection;
}

void Detector::FillTree(TTree& gentree, TTree& rectree)
{
    gentree.SetBranchAddress("TrueHits", &fTrueHit);
    rectree.SetBranchAddress("RecoHits", &fRecoHit);

    gentree.Fill();
    rectree.Fill();
}

/*double Detector::ComputePhi(double x, double y)
{
    double phi = TMath::ATan(y / x);
    if (x<0)
        phi+=TMath::Pi();
    return phi;
}*/