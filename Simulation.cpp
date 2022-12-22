#include "Run.h"

void Simulation(TString ConfigFileName = "Config_Run.yaml")
{
    Run a(ConfigFileName);
    //TFile hfile("Tree.root","recreate");
    //TTree tree("tree","tree");
    //std::vector<double> numbers = {1.,2.,3.};
    //std::vector<double>* ptr=&numbers;
    //tree.Branch("numbers",&ptr);
    //tree.Fill();
    //std::vector<double> numbers2 = {10.,20.,30.};
    //ptr=&numbers2;
    //tree.Fill();
    //hfile.Write();
    //hfile.Close();
}