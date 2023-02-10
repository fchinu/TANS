#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include"Run.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include "TEfficiency.h"
#include "DetectorHandler.h"
#include "yaml-cpp-yaml-cpp-0.6.0/include/yaml-cpp/yaml.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <string>
 
class Reconstruction: public TObject
{
/*
 *  Class for executing reconstruction analysis
 *  -------------------------
 *  Parameters:
 *  cfgFileName: string containing address of yaml file for configuration of reconstruction
 * 
 */
public:
    Reconstruction() = default;
    Reconstruction(TString cfgFileName);
    ~Reconstruction(){delete fResiduals; delete fResolutionVsMultiplicity; delete fResolutionVsZTrue; delete pEffvsZ; delete pEff;}
 
    void FindTracklets();
    void MinDca();
    double MeanRunningWindow(vector<double> vertexes);
 
private:
    YAML::Node fConfigFile;
    string fOutFileName, fTreeFileName, fRecoTreeName, fRecoDet1Hits, fRecoDet2Hits, fGenTreeName, fGenConfig, fDetectorsName;
    double fMaxPhi, fSigmaZ;

    DetectorHandler fDetectors;
    
    std::vector<std::vector<MaterialBudget::fPoint>> fIntersections1;
    std::vector<std::vector<MaterialBudget::fPoint>> fIntersections2;
    std::vector<std::vector<Event::fVertMult>> fConfigs;
    std::vector<std::vector<MaterialBudget::fPoint>> fTracklets;
    //Vector containing 3D vertex reconstruction
    std::vector<std::vector<double>> fVertexes;
    //Vector containing Z coordinates of reconstructed vertexes
    std::vector<double> fVertexesZ;
    std::vector<double> fVertexesZResolutions;
    TH1D* fResiduals = new TH1D("Residuals", "Residuals", 500,-0.5,0.5);
    TH1D* fResolutionVsMultiplicity = new TH1D("ResolutionVsMultiplicity", "ResolutionVsMultiplicity", 20, 0., 100.);
    TH1D* fResolutionVsZTrue = new TH1D("ResolutionVsZTrue", "ResolutionVsZTrue", 20, -30., 30.);

    //Efficiency estimations
    double effvsMultBins[23]={0,1,2,3,4,5,6,7,8,9,10,15,20,25,30,35,40,50,60,70,80,90,100};
    TEfficiency* pEff = new TEfficiency("eff","Efficiency vs Multiplicity;Multiplicity;Efficiency", 22,effvsMultBins);
    TEfficiency* pEffvsZ = new TEfficiency("effvsZ","Efficiency vs ZTrue;ZTrue;Efficiency", 20, -30, 30);
    
    void FillHistoMinDca(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp);
    void FillHistoIntersection(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp);
    void FillHistoResiduals();
    void FillHistoEff();
    void FillHistoEfficiencyVsZTrue();
    void FillHistoResolutionVsMultiplicity();
    void FillHistoResolutionVsZTrue();
 
    ClassDef(Reconstruction, 1)
};
 
 
#endif
