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
        cout << "Invalid features for material budget, switching to multiple scattering phenomenon" << endl;
        fMultScat = true;
    }
    return *this;
}

void MaterialBudget::Interaction(Particle* part)
{
/*
 *  Function that handles the interaction between the material budget and a particle
 *  -------------------------
 *  Parameters:
 *  part: Particle*
 *      Pointer to particle
 * 
 */
    if(fMultScat)
        MultScattering(part);
}

Particle* MaterialBudget::MultScattering(Particle* part)
{
/*
 *  Function that handles the multiple scattering
 *  -------------------------
 *  Parameters:
 *  part: Particle*
 *      Pointer to particle
 * 
 */

    //Defines rotation matrix
    double rotation[3][3];
    vector<double> PolarDirection=part->GetPolarDirection();
    double sintheta=TMath::Sin(PolarDirection[0]), sinphi=TMath::Sin(PolarDirection[1]);
    double costheta=TMath::Cos(PolarDirection[0]), cosphi=TMath::Cos(PolarDirection[1]);
    rotation[0][0] = -sinphi;
    rotation[1][0] = cosphi;
    rotation[2][0] = 0.;
    rotation[0][1] = -cosphi*costheta;
    rotation[1][1] = -costheta*sinphi;
    rotation[2][1] = sintheta;
    rotation[0][2] = sintheta*cosphi;
    rotation[1][2] = sintheta*sinphi;
    rotation[2][2] = costheta;

    /*double l,beta,v;
    v = TMath::Sqrt(part->GetDirection()[0]*part->GetDirection()[0] + part->GetDirection()[1]*part->GetDirection()[1] + part->GetDirection()[2]*part->GetDirection()[2]);
    beta=1.;
    l = 4*(2.81794*pow(10,-15))*(1/137.035999)*(6.02214*pow(10,23))*(fZ*fZ*fDensity/fA)*log(183/pow(fZ,1/3));  // = 1/X0
    double thetanew = ((13.6*1.6*pow(10,-13))/(beta*3*pow(10,8)*v))*fZ*TMath::Sqrt(l*fThickness)*(1+0.038*log(l*fThickness));*/


    double thetaapprox = 0.001;             // 1mrad
    thetaapprox = gRandom->Gaus(0,thetaapprox);
    double phinew = (gRandom->Rndm())*2*TMath::Pi();

    double dir[3];                          // New direction in the old direction frame
    dir[0] = TMath::Sin(thetaapprox)*TMath::Cos(phinew);
    dir[1] = TMath::Sin(thetaapprox)*TMath::Sin(phinew);
    dir[2] = TMath::Cos(thetaapprox);

    vector<double> newdir(3,0);                  // New direction in the lab frame
    for(int i=0;i<3;i++)
        for(int j=0; j<3; j++)
            newdir[i] += rotation[i][j]*dir[j];
    MaterialBudget::fPoint intersection = GetLastIntersection(part);
    if (intersection.isIntersection)
    {
        part->SetPoint(fRadius*intersection.phi, fRadius*intersection.phi, intersection.z);
        part->SetDirection(newdir);
    }
    return part;
}

MaterialBudget::fPoint MaterialBudget::GetIntersection(const Particle* particle)
{
/*
 *  Function that returns the intersection with a particle
 *  -------------------------
 *  Parameters:
 *  part: Particle*
 *      Pointer to particle
 * 
 *  -------------------------
 *  Returns:
 *      MaterialBudget::fPoint containing intersection
 * 
 */
    vector<double> direction = particle->GetDirection();
    vector<double> point = particle->GetPoint();

    //Evaluates all different factors to evaluate intersection
    double den = (direction[0]*direction[0] + direction[1]*direction[1]);
    double b = (point[0]*direction[0] + point[1]*direction[1]);
    double delta = (b*b - den * (point[0]*point[0] + point[1]*point[1] - fRadius * fRadius));

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
    double x = point[0] + direction[0] * t;
    double y = point[1] + direction[1] * t;
    intersection.z = point[2] + direction[2] * t;
    intersection.phi = ComputePhi(x, y);
    return intersection;
}

double MaterialBudget::ComputePhi(double x, double y)
{
/*
 *  Helper function that evauates the azimuth
 *  -------------------------
 *  Parameters:
 *  x,y: double
 *      coordinates of a point
 * 
 *  -------------------------
 *  Returns:
 *      double containing azimuth of the point of coordinates (x,y)
 * 
 */
    double phi = TMath::ATan(y / x);
    if (x<0)
        phi+=TMath::Pi();
    return phi;
}
