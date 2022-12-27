#include "Detector.h"

Detector::Detector() : MaterialBudget() {};

Detector::Detector(double thickness, double radius, double length, string material, bool multscat=1, bool smearing=1, bool noise=0)
: MaterialBudget(thickness, radius, length, material, multscat=1)
{
    // *fTrueHitPtr = &fTrueHit;
    // *fRecoHitPtr = &fRecoHit;
    SetStatus(smearing, noise);
}

Detector::~Detector()
{
}

Detector& Detector::SetStatus(vector<bool> status)
{
    if(status.size() == 2){  
        fSmearing = status[0];
        fNoise = status[1];
    }
    else{
        cout << "Invalid features for detector, switching on smearing and noise phenomena" << endl;
        fSmearing = true;
        fNoise = true;
    }
    return *this;
}

void Detector::Interaction(Particle* particle)
{
    fTrueHit.push_back(GetIntersection(particle,false));
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

    if (point[2] + direction[2] * t < -fThickness/2 || point[2] + direction[2] * t > fThickness/2)       //particle goes outside detector
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

        if (SmearedIntersection.z < -fThickness/2 || SmearedIntersection.z > fThickness/2)
            continue;
        
        SmearedIntersection.isIntersection=true;
        SmearedIntersection.x = fRadius*TMath::Cos(SmearedIntersection.phi);
        SmearedIntersection.y = fRadius*TMath::Sin(SmearedIntersection.phi);

        fRecoHit.push_back(SmearedIntersection);
    }
    return SmearedIntersection;
}

void Detector::FillTree(TTree& gentree, const char* genbranchname, TTree& rectree, const char* recbranchname)
{
    cout<<"TrueTreeSize = "<<fTrueHit.size()<<endl;
    cout<<"RecTreeSize = "<<fRecoHit.size()<<endl;
    cout << "Filling tree branches..." << endl;
    std::vector<MaterialBudget::fPoint>* fTrueHitPtr = &fTrueHit;
    std::vector<MaterialBudget::fPoint>* fRecoHitPtr = &fRecoHit;

    gentree.SetBranchAddress(genbranchname, &fTrueHitPtr);
    rectree.SetBranchAddress(recbranchname, &fRecoHitPtr);

    gentree.Fill();
    cout << "GenTree filled" << endl;
    rectree.Fill();
    cout << "RecTree filled" << endl;
}
