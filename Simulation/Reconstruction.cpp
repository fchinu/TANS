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

    TTreeReaderValue<std::vector<MaterialBudget::fPoint>> intersect1(myReader, "RecHits_1");
    TTreeReaderValue<std::vector<MaterialBudget::fPoint>> intersect2(myReader, "RecHits_2");
 
    // Loop over all entries of the TTree or TChain.
    while (myReader.Next()) {
       // Just access the data as if intersect1 and intersect2 were iterators (note the '*'
       // in front of them):
       fIntersections1.push_back(*intersect1);
       fIntersections2.push_back(*intersect2);
    }

    cout << "Intersezioni1: " << fIntersections1.size() << endl;    
    for (auto i: fIntersections1){
        cout << i.size() << endl;
        for (auto j: i){
            j.print();
        }
        //cout << endl;
    }
    cout << endl;
    cout << "Intersezioni2: " << fIntersections2.size() << endl;
    for (auto x: fIntersections2){
        cout << x.size() << endl;
        for (auto y: x){
            y.print();
        }
        //cout << endl;
    }
    cout << endl;
    cout << "Fine stampa" << endl;
    

    FindTracklets();
    cout << "Fine FindTracklets ended" << endl;
    /*VertexReco();
    cout << "VertexReco ended" << endl;
    */
    file->Close();
}
 
void Reconstruction::FindTracklets()
{
    cout << "Entering FindTracklets" << endl;
    double phimax = 0.010; // maximum angle difference for 2 intersections to be a tracklet
    std::vector<MaterialBudget::fPoint> tracklet;
    for (auto x: fIntersections1){
        for (auto y: x){
            for (auto i: fIntersections2){
                for (auto j: i){
                    if(abs(y.phi-j.phi)<phimax){
                        tracklet.push_back(y);
                        tracklet.push_back(j);
                        fTracklets.push_back(tracklet);
                        tracklet.clear();
                    }
                }
            }
        }
    }

    for(auto z: fTracklets){
        for(auto w: z){
            w.print();
        }
        cout << endl;
    }
    cout << endl;
    cout << "Number of tracklets: " << fTracklets.size() << endl;
    /*int nEntries1 = fData[0].size();
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
    }*/
}
 
void Reconstruction::VertexReco()
{
    cout << "Entering VertexReco" << endl;
    std::vector<double> vertex;
    for(auto i: fTracklets){
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        a = i[0].x - i[1].x;
        b = i[0].y - i[1].y;
        c = i[0].z - i[1].z;
 
        // value of parameter t for intersection between line and orthogonal plane passing for O
        double t = -(a*i[1].x + b*i[1].y + c*i[1].z)/(a*a + b*b + c*c);
 
        // vertex coordinates
        double x, y, z;
        x = i[1].x + a*t;
        y = i[1].y + b*t;
        z = i[1].z + c*t;
 
        vertex.push_back(x);
        vertex.push_back(y);
        vertex.push_back(z);
 
        fVertexes.push_back(vertex);
        vertex.clear();
    }
    for(auto i: fVertexes){
        cout << "Vertex coordinates: ";
        for(auto j: i){
            cout << j << " ";
        }
        cout << endl;
    }
}
