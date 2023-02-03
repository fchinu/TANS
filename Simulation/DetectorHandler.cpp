#include "DetectorHandler.h"

DetectorHandler::DetectorHandler(YAML::Node ConfigFile):
fConfigFile(ConfigFile),
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThicknesses(fConfigFile["Thickness"].as<std::vector<double> >()),
fLengths(fConfigFile["Length"].as<std::vector<double> >()),
fMaterials(fConfigFile["Material"].as<std::vector<string> >()),
fDoMultScattering(fConfigFile["DoMultScattering"].as<bool>()),
fDoSmearing(fConfigFile["DoSmearing"].as<bool>()),
fDoNoise(fConfigFile["DoNoise"].as<bool>()),
fMeanNoise(fConfigFile["MeanNoise"].as<int>())
{
    GenerateDetectors();
}

void DetectorHandler::GenerateDetectors()     //TODO: add default cases
{
/*
 *  Function that generates the array of detectors based on the ConfigFile info
 */
    for (vector<bool>::size_type i=0; i<fIsDetector.size(); ++i)
    {
        if (fIsDetector[i])
        {
            if (fVerbose)
                cout<<"Creating Detector"<<endl;
                fDetectors.push_back(new Detector(fThicknesses[i],fRadii[i],fLengths[i],fMaterials[i],fDoMultScattering,fDoSmearing,fDoNoise,fMeanNoise));
        }
        else
        {
            if (fVerbose)
                cout<<"Creating MaterialBudget"<<endl;
            fDetectors.push_back(new MaterialBudget(fThicknesses[i],fRadii[i],fLengths[i],fMaterials[i],fDoMultScattering));
        }
    }

    //Sorts detectors/materialbudgets based on the radii
    std::sort(fDetectors.begin(),fDetectors.end(),[] (MaterialBudget const *const a, MaterialBudget const *const b) { return a->GetRadius() < b->GetRadius(); });
}