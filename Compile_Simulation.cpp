#include "TString.h"
#include "TSystem.h"

void Compile_Simulation(TString myopt="fast")
{
    TString opt;
    if (myopt.Contains("force"))
        opt="kfg";          //k mantiene il .so, f forza la compilazione (come mettere il ++), g serve se vogliamo usare un debugger
    else
        opt="kg";
    if (myopt.Contains("clean"))
        gSystem->Exec("./clean.sh");
    gSystem->CompileMacro("Particle.cpp",opt.Data());
    gSystem->CompileMacro("MaterialBudget.cpp",opt.Data());
    gSystem->CompileMacro("Detector.cpp",opt.Data());
    gSystem->CompileMacro("Event.cpp",opt.Data());
}