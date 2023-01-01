#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include"Run.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <vector>
#include <string>
 
class Reconstruction: public TObject
{
public:
    Reconstruction();
 
    void VertexReco();
    void FindTracklets();
 
private:
    std::vector<std::vector<MaterialBudget::fPoint>> fData;
    std::vector<std::vector<MaterialBudget::fPoint>> fTracklets;
    std::vector<std::vector<double>> fVertexes;
 
    ClassDef(Reconstruction, 1)
};
 
 
#endif
