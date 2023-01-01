#include "Reconstruction.h"
#include"TDirectory.h"
 
ClassImp(Reconstruction)
 
Reconstruction::Reconstruction()
{
    // reading tree data into fData
    // TTreeReader reader1("Intersections", file);
    // TTreeReader reader2("Intersections", file);
    //
    // TTreeReaderValue<MaterialBudget::fPoint> intersections1(reader1, "RecHits detector 1");
    // TTreeReaderValue<MaterialBudget::fPoint> intersections2(reader2, "RecHits detector 1");
   
    // Open the file containing the tree.
    TFile* file = TFile::Open("Simulation/Tree.root");
    //if (*file)
    //    cout<<"file letto";
    // Create a TTreeReader for the tree, for instance by passing the
    // TTree's name and the TDirectory / TFile it is in.
    TTreeReader myReader("fTreeRec", file);
   
    // The branch "RecHits detector 1" contains MaterialBudget::fPoint.
    cout << "Ciao" << endl;
    TTreeReaderValue<vector<MaterialBudget::fPoint>> intersect1(myReader, "RecHits_1");
    cout << "Ciao" << endl;
    TTreeReaderValue<vector<MaterialBudget::fPoint>> intersect2(myReader, "RecHits_2");
    cout << "Ciao" << endl;
 
    std::vector<vector<MaterialBudget::fPoint>> intersections1;
    std::vector<vector<MaterialBudget::fPoint>> intersections2;
    // Loop over all entries of the TTree or TChain.
    while (myReader.Next()) {
       // Just access the data as if myPx and myPy were iterators (note the '*'
       // in front of them):
       intersections1.push_back(*intersect1);
       intersections2.push_back(*intersect2);
    }
 
    cout << "Intersezioni1: " << intersections1.size() << endl;
    cout << "Intersezioni2: " << intersections2.size() << endl;
 
    //FindTracklets();
    VertexReco();
    //file->Close();
}
 
void Reconstruction::FindTracklets()
{
    double phimax = 0.010; // maximum angle difference for 2 intersections to be a tracklet
    int nEntries1 = fData[0].size();
    int nEntries2 = fData[1].size();
    std::vector<MaterialBudget::fPoint> tracklet;
    for(unsigned int i=0; i<nEntries1; i++){
        for(unsigned int j=0; j<nEntries2; j++){
            double phi1 = fData[0][i].phi;
            double phi2 = fData[1][j].phi;
            if(abs(phi1-phi2)<phimax){
                tracklet.push_back(fData[0][i]);
                tracklet.push_back(fData[1][j]);
                fTracklets.push_back(tracklet);
                tracklet.clear();
            }
        }
    }
}
 
void Reconstruction::VertexReco()
{
    std::vector<double> vertex;
    for(int i=0; i<fTracklets.size(); i++){
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        a = fTracklets[i][0].x - fTracklets[i][1].x;
        b = fTracklets[i][0].y - fTracklets[i][1].y;
        c = fTracklets[i][0].z - fTracklets[i][1].z;
 
        // value of parameter t for intersection between line and orthogonal plane passing for O
        double t = -(a*fTracklets[i][1].x + b*fTracklets[i][1].y + c*fTracklets[i][1].z)/(a*a + b*b + c*c);
 
        // vertex coordinates
        double x, y, z;
        x = fTracklets[i][1].x + a*t;
        y = fTracklets[i][1].y + b*t;
        z = fTracklets[i][1].z + c*t;
 
        vertex.push_back(x);
        vertex.push_back(y);
        vertex.push_back(z);
 
        fVertexes.push_back(vertex);
        vertex.clear();
    }
}
