#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"

void Compile_Simulation(TString myopt="run")
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
    gSystem->CompileMacro("Simulation/Particle.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/MaterialBudget.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/Detector.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/AngularHandler.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/DetectorHandler.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/Event.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/MultHandler.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/VertexHandler.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/Run.cpp",opt.Data(),"","Build");
    gSystem->CompileMacro("Simulation/Simulation.cpp",opt.Data(),"","Build");

    gROOT->ProcessLine("Simulation()");
}