#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include"Run.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include "TEfficiency.h"
#include "yaml-cpp/yaml.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <string>
 
class Reconstruction: public TObject
{
public:
    Reconstruction() = default;
    Reconstruction(TString cfgFileName);
    ~Reconstruction(){delete fResiduals; delete pEff;}
 
    void VertexReco();
    void FindTracklets();
    void MinDca();
 
private:
    YAML::Node fConfigFile;
    string fOutFileName,fTreeFileName, fRecoTreeName, fRecoDet1Hits, fRecoDet2Hits, fGenTreeName, fGenConfig;
    double fMaxPhi,fSigmaZ;
    
    std::vector<std::vector<MaterialBudget::fPoint>> fIntersections1;
    std::vector<std::vector<MaterialBudget::fPoint>> fIntersections2;
    std::vector<std::vector<Event::fVertMult>> fConfigs;
    std::vector<std::vector<MaterialBudget::fPoint>> fTracklets;
    std::vector<std::vector<double>> fVertexes;
    std::vector<double> fVertexesZ;
    std::vector<double> fVertexesZResolutions;
    TH1D* fResiduals = new TH1D("Residuals", "Residuals", 500,-0.5,0.5);
    TH1D* fResolutionVsMultiplicity = new TH1D("ResolutionVsMultiplicity", "ResolutionVsMultiplicity", 20, 0., 100.);
    TH1D* fResolutionVsZTrue = new TH1D("ResolutionVsZTrue", "ResolutionVsZTrue", 18, -27., 27.);
    TH1D* fEfficiencyVsZTrue = new TH1D("EfficiencyVsZTrue", "EfficiencyVsZTrue", 14, -27., 27.);
    TEfficiency* pEff = new TEfficiency("eff","Efficiency vs Multiplicity;Multiplicity;Efficiency", 100, 0, 100);
    
    void FillHistoMinDca(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp);
    void FillHistoIntersection(TH1D* histo, vector<MaterialBudget::fPoint>& tracklets, vector<double>& vertextemp);
    void FillHistoResiduals();
    void FillHistoEff();
    void FillHistoResolutionVsMultiplicity();
    void FillHistoResolutionVsZTrue();
    void FillHistoEfficiencyVsZTrue();

 
    ClassDef(Reconstruction, 1)
};
 
 
#endif
