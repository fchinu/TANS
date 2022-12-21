#include "Event.h"

ClassImp(Event)

Event::Event(vector<MaterialBudget*> detectors, unsigned int multiplicity, double x, double y, double z)
{
    while (fParticles.size()<multiplicity)
    {
        fParticles.push_back(new Particle({x,y,z},gRandom));
    }
    for(unsigned int i=0; i < detectors.size(); i++){
        fDetectors.push_back(detectors[i]);
    }
    fConfig.multiplicity = multiplicity;
    fConfig.x = x;
    fConfig.y = y;
    fConfig.z = z;
}

void Event::ProcessingEvent(TTree* tree)
{
    TStopwatch w;
    w.Start();
    /*for (int i = 0; i<fParticles.size(); i++){
        for(int j = 0; j<fDetectors.size(); j++){
            if(fDetectors[j]->IsDetector()){
                fDetectors[j]->GetIntersection(fParticles[i]);
                fDetectors[j]->GetSmearedIntersection();
            }
            fDetectors[j]->MultScattering(fParticles[i]);
            FillTree(tree, j);
        }
    }*/
    w.Stop();
    w.Print("u");
}

/*void Event::FillTree(TTree* tree, int z){
    if(!tree->GetBranch("Config")){
        tree->SetBranchAddress("Config", &fConfig);
        tree->SetBranchAddress("TrueHits", &fDetectors[z]->GetTrueHits().back());
        tree->SetBranchAddress("RecoHits", &fDetectors[z]->GetRecoHits().back());
    }
    else{
        tree->SetBranchAddress("TrueHits", &fDetectors[z]->GetTrueHits().back());
        tree->SetBranchAddress("RecoHits", &fDetectors[z]->GetRecoHits().back());
    }
    tree->Fill();
}*/

void Event::EventVisual(vector<Particle*> particles)
{
    gSystem->Load("libGeom");

    TGeoManager *geom = new TGeoManager("world", "the simplest geometry");
    TGeoMaterial *mat = new TGeoMaterial("Vacuum",0,0,0);
    TGeoMaterial *Fe = new TGeoMaterial("Fe",55.845,26,7.87);
    TGeoMedium   *med = new TGeoMedium("Vacuum",1,mat);
    TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);
    
    TGeoVolume *top=gGeoManager->MakeBox("Top",med,15.,15.,30.);
    TGeoVolume *Outer=gGeoManager->MakeTubs("Outer",Iron,6.99,7.01,27.,90,360);
    TGeoVolume *Inner=gGeoManager->MakeTubs("Inner",Iron,3.99,4.01,27.,90,360);
    TGeoVolume *BeamPipe=gGeoManager->MakeTubs("BeamPipe",Iron,2.96,3.04,27.,90,360);

    //double phi =32, theta=63;           //gamma = phi(polar)-90, theta(here)=theta(polar)+90
    //double rad_phi= TMath::DegToRad()*phi, rad_theta=TMath::DegToRad()*theta;
    //double len1=8/TMath::Sin(rad_theta)/2;
    std::string Trackstr = "Track";
    unsigned count=0;
    
    top->SetLineColor(kBlue);
    gGeoManager->SetTopVisible(0); // the TOP is invisible
    Outer->SetLineColor(kOrange);
    top->AddNode(Outer,1);
    Inner->SetLineColor(kBlue);
    top->AddNode(Inner,1);
    BeamPipe->SetLineColor(kMagenta);
    BeamPipe->SetFillStyle(1001);
    BeamPipe->SetFillColor(kMagenta);
    //top->SetVisibility(kFALSE);
    top->AddNode(BeamPipe,1);

    for (auto& i : particles)
    {
        vector<double> dir = i->GetPolarDirection(); 
        dir[0] = 180 - dir[0]*TMath::RadToDeg();
        dir[1] = dir[1]*TMath::RadToDeg() - 90;

        double critangle=TMath::ATan(20/27)*TMath::RadToDeg();
        critangle=14;

        if(dir[0] < (180 - critangle)  && dir[0]>critangle)
        {
            double rad_phi= TMath::DegToRad()*dir[1], rad_theta=TMath::DegToRad()*dir[0];
            double len1=8/TMath::Sin(rad_theta)/2;
            TGeoVolume *Track=gGeoManager->MakeTubs((Trackstr+std::to_string(count)).c_str(),Iron,0,0.05,len1,0,360);
            top->AddNode(Track,count, new TGeoCombiTrans(-len1*TMath::Sin(rad_phi)*TMath::Sin(rad_theta),len1*TMath::Cos(rad_phi)*TMath::Sin(rad_theta),-len1*TMath::Cos(rad_theta),new TGeoRotation("ha_8",dir[1],dir[0],0))); // 45,90,0
            ++count;
        }
    }
    
    gGeoManager->SetTopVolume(top);
    gGeoManager->SetVisLevel(3);
    gGeoManager->CloseGeometry();
    top->Draw("ogl");

}