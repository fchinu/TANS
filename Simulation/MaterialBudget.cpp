#include "MaterialBudget.h"

ClassImp(MaterialBudget)

MaterialBudget::MaterialBudget(): fThickness(0.), fRadius(0.), fLength(0.), fDensity(0.), fZ(0), fA(0), fMultScat(true){}

MaterialBudget::MaterialBudget(double thickness, double radius, double length, double density, int z, int a, bool multscat = true)
{
    if(thickness<=0) cout << "Material Budget thickness null or negative. Setting value equal to 1. \n";
    fThickness = 1.;

    if(radius<=0) cout << "Material Budget radius null or negative. Setting value equal to 1. \n";
    fRadius = 1.;

    SetGeometry(thickness,radius, 0.27).SetMaterial(density, z, a).SetStatus(multscat);
}

MaterialBudget::MaterialBudget(double thickness, double radius, double length, string material, bool multscat = true)
{
    if(thickness<=0) 
    {
        cout << "Material Budget thickness null or negative. Setting value equal to 1. \n";
        fThickness = 1.;
    }

    if(radius<=0) 
    {
        cout << "Material Budget radius null or negative. Setting value equal to 1. \n";
        fRadius = 1.;
    }

    SetGeometry(thickness, radius, length).SetMaterial(material).SetStatus(multscat);
}

MaterialBudget& MaterialBudget::SetMaterial(string a)
{
    for(auto & i:a) i=tolower(i);

    if(a=="be"){
        fDensity = 1.85; // g/cm3
        fZ = 4;
        fA = 8; 
    }

    else if(a=="si"){
        fDensity = 3.74; // g/cm3
        fZ = 14;
        fA = 28;
    }
    else{
        cout << "Unknown material: setting density, z, A equal to 1" << endl;
        fDensity = 1;
        fZ = 1;
        fA = 1;
    }
    return *this;
}

MaterialBudget& MaterialBudget::SetStatus(vector<bool> status)
{
    if(status.size() == 1){  
        fMultScat = status.back();
    }
    else{
        cout << "Invalid features for material budget, switching on multiple scattering phenomenon" << endl;
        fMultScat = true;
    }
    return *this;
}

void MaterialBudget::Interaction(Particle* part, int& detected, int& notdetected, int& smeared, int& notsmeared)
{
    FillData(part, detected, notdetected, smeared, notsmeared);
    if(fMultScat){
        MultScattering(part);
    }
}

Particle* MaterialBudget::MultScattering(Particle* part)
{
    double rotation[3][3];

    rotation[0][0] = -TMath::Sin(part->GetPolarDirection()[1]);
    rotation[1][0] = TMath::Cos(part->GetPolarDirection()[1]);
    rotation[2][0] = 0.;
    rotation[0][1] = -TMath::Cos(part->GetPolarDirection()[1])*TMath::Cos(part->GetPolarDirection()[0]);
    rotation[1][1] = -TMath::Cos(part->GetPolarDirection()[0])*TMath::Sin(part->GetPolarDirection()[1]);
    rotation[2][1] = TMath::Sin(part->GetPolarDirection()[0]);
    rotation[0][2] = TMath::Sin(part->GetPolarDirection()[0])*TMath::Cos(part->GetPolarDirection()[1]);
    rotation[1][2] = TMath::Sin(part->GetPolarDirection()[0])*TMath::Sin(part->GetPolarDirection()[1]);
    rotation[2][2] = TMath::Cos(part->GetPolarDirection()[0]);

    /*double l,beta,v;
    v = TMath::Sqrt(part->GetDirection()[0]*part->GetDirection()[0] + part->GetDirection()[1]*part->GetDirection()[1] + part->GetDirection()[2]*part->GetDirection()[2]);
    beta=1.;
    l = 4*(2.81794*pow(10,-15))*(1/137.035999)*(6.02214*pow(10,23))*(fZ*fZ*fDensity/fA)*log(183/pow(fZ,1/3));  // = 1/X0
    double thetanew = ((13.6*1.6*pow(10,-13))/(beta*3*pow(10,8)*v))*fZ*TMath::Sqrt(l*fThickness)*(1+0.038*log(l*fThickness));*/

    double thetaapprox = 0.001; // 1mrad
    thetaapprox = gRandom->Gaus(0,thetaapprox);
    double phinew = (gRandom->Rndm())*2*M_PI;
    double dir[3]; // nuova direzione nel sistema di riferimento legato a direzione precedente
    dir[0] = TMath::Sin(thetaapprox)*TMath::Cos(phinew);
    dir[1] = TMath::Sin(thetaapprox)*TMath::Sin(phinew);
    dir[2] = TMath::Cos(thetaapprox);

    vector<double> newdir; // nuova direzione nel sistema di riferimento del lab
    double count = 0.;
    for(int i=0;i<3;i++){
        count = 0.;
        for(int j=0; j<3; j++){
            count += rotation[i][j]*dir[j];
        }
        newdir.push_back(count);
    }
    MaterialBudget::fPoint intersection = GetIntersection(part);
    part->SetPoint(intersection.x, intersection.y, intersection.z);
    part->SetDirection(newdir);
    return part;
}

MaterialBudget::fPoint MaterialBudget::GetIntersection(const Particle* particle)
{
    vector<double> direction = particle->GetDirection();
    vector<double> point = particle->GetPoint();

    //Evaluates all different factors to evaluate intersection
    double den = direction[0]*direction[0] + direction[1]*direction[1];
    double b = (point[0]*direction[0] + point[1]*direction[1]);
    double delta = b*b - den * (point[0]*point[0] + point[1]*point[1] - fRadius * fRadius);

    MaterialBudget::fPoint intersection;
    if (delta <= 0)     //particle going along z axis
    {
        intersection.isIntersection=false;
        return intersection;
    }

    double t = (TMath::Sqrt(delta)-b)/den;

    if (point[2] + direction[2] * t < -fLength/2 || point[2] + direction[2] * t > fLength/2)       // particle goes outside detector
    {
        intersection.isIntersection=false;
        return intersection;
    } 

    intersection.isIntersection=true;
    intersection.x = point[0] + direction[0] * t;
    intersection.y = point[1] + direction[1] * t;
    intersection.z = point[2] + direction[2] * t;
    intersection.phi = ComputePhi(intersection.x, intersection.y);
    return intersection;
}

double MaterialBudget::ComputePhi(double x, double y)
{
    double phi = TMath::ATan(y / x);
    if (x<0)
        phi+=TMath::Pi();
    return phi;
}
