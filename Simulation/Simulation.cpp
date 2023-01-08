#include "Run.h"

void Simulation(TString ConfigFileName = "Config_Run.yaml")
{
    gRandom->SetSeed(42);
    Run a(ConfigFileName);
}