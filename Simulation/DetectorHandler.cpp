#include "DetectorHandler.h"

ClassImp(DetectorHandler)

DetectorHandler::DetectorHandler(YAML::Node ConfigFile):
fConfigFile(ConfigFile),
fIsDetector(fConfigFile["IsDetector"].as<std::vector<bool> >()),
fRadii(fConfigFile["Radii"].as<std::vector<double> >()),
fThickness(fConfigFile["Thickness"].as<std::vector<double> >()),
fLength(fConfigFile["Lenght"].as<std::vector<double> >()),
fMaterial(fConfigFile["Material"].as<std::vector<string> >()),
fDoMultScattering(fConfigFile["DoMultScattering"].as<bool>()),
fDoSmearing(fConfigFile["DoSmearing"].as<bool>()),
fDoNoise(fConfigFile["DoNoise"].as<bool>()),
fMeanNoise(fConfigFile["MeanNoise"].as<int>()),
fVerbose(fConfigFile["Verbose"].as<bool>())
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
                fDetectors.push_back(new Detector(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering,fDoSmearing,fDoNoise,fMeanNoise));
        }
        else
        {
            if (fVerbose)
                cout<<"Creating MaterialBudget"<<endl;
            fDetectors.push_back(new MaterialBudget(fThickness[i],fRadii[i],fLength[i],fMaterial[i],fDoMultScattering));
        }
    }

    //Sorts detectors/materialbudgets based on the radii
    std::sort(fDetectors.begin(),fDetectors.end(),[] (MaterialBudget const *const a, MaterialBudget const *const b) { return a->GetRadius() < b->GetRadius(); });
}