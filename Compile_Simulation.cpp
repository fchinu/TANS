void Compile_Simulation(TString myopt="fast")
{
    TString opt;
    if (myopt.Contains("force"))
        opt="kfg";          //k mantiene il .so, f forza la compilazione (come mettere il ++), g serve se vogliamo usare un debugger
    else
        opt="kg";
    gSystem->CompileMacro("Particle.cpp",opt.Data());
}