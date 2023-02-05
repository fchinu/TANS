#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"

void Compile_Reconstruction(TString myopt="run")
{
    gSystem->AddIncludePath((string("-I ")+gSystem->GetWorkingDirectory()+"/Simulation").c_str());
    gSystem->AddIncludePath((string("-I ")+gSystem->GetWorkingDirectory()+"/Reconstruction").c_str());
    gSystem->AddIncludePath((string("-I ")+gSystem->GetWorkingDirectory()+"/Build").c_str());
    TString opt;
    if (myopt.Contains("force"))
        opt="kfg-";          //k mantiene il .so, f forza la compilazione (come mettere il ++), g serve se vogliamo usare un debugger
    else
        opt="kg-";
    if (myopt.Contains("clean"))
        gSystem->Exec("./clean.sh");
    gSystem->CompileMacro("Simulation/MaterialBudget.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/Event.cpp",opt.Data(),"","Build");
        gSystem->CompileMacro("Simulation/DetectorHandler.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Reconstruction/Reconstruction.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Reconstruction/RunAnalysis.cpp",opt.Data(),"","Build");

    gROOT->ProcessLine("RunAnalysis()");
}