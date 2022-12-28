#include"Event.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<Event::fVertMult>+;
#endif

void TryEvent(){

    TFile hfile("Tree.root", "recreate");
    
    double xvert = 0.;
    double yvert = 0.;
    double zvert = 0.;
    unsigned int multiplicity = 1000;
    std::vector<MaterialBudget*> detectors;
    MaterialBudget* beampipe = new MaterialBudget(0.08, 3., 10000000., "Be", 1);
    Detector* detector1 = new Detector(0.02, 4., 27., "Si", 1, 1, 0);
    Detector* detector2 = new Detector(0.02, 7., 27., "Si", 1, 1, 0);
    detectors.push_back(beampipe);
    detectors.push_back(detector1);
    detectors.push_back(detector2);

    std::vector<Event::fVertMult>*          config      = nullptr;
    std::vector<MaterialBudget::fPoint>*    genhits1    = nullptr;
    std::vector<MaterialBudget::fPoint>*    genhits2    = nullptr;
    std::vector<MaterialBudget::fPoint>*    rechits1    = nullptr;
    std::vector<MaterialBudget::fPoint>*    rechits2    = nullptr;
    

    TTree gentree("gentree", "gentree");
    gentree.Branch("Config",&config);
    gentree.Branch("GenHits detector 1",&genhits1);
    gentree.Branch("GenHits detector 2",&genhits2);

    TTree rectree("rectree", "rectree");
    rectree.Branch("RecHits detector 1",&rechits1);
    rectree.Branch("RecHits detector 2",&rechits2);

    cout << "Everything initialised" << endl;

    Event(detectors, multiplicity, xvert, yvert, zvert, gentree, rectree);
    // Event(detectors, multiplicity, xvert, yvert, zvert, gentree, rectree);

    hfile.Write();
    hfile.Close();
}