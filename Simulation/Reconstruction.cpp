#include "Reconstruction.h"
#include"TDirectory.h"
#include "TH1D.h"
#include "TCanvas.h"
 
ClassImp(Reconstruction)
 
Reconstruction::Reconstruction()
{
    // Open the file containing the tree.
    TFile* file = new TFile("Simulation/Tree.root","update");
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

    //cout << "Intersezioni1: " << fIntersections1.size() << endl;    
    //for (auto i: fIntersections1){
    //    //cout << i.size() << endl;
    //    for (auto j: i){
    //        //j.print();
    //    }
    //    //cout << endl;
    //}
    //cout << endl;
    //cout << "Intersezioni2: " << fIntersections2.size() << endl;
    //for (auto x: fIntersections2){
    //    cout << x.size() << endl;
    //    for (auto y: x){
    //        //y.print();
    //    }
    //    //cout << endl;
    //}
    //cout << endl;
    //cout << "Fine stampa" << endl;
    

    FindTracklets();
    //cout << "Fine FindTracklets ended" << endl;
    MinDca();
    //cout << "VertexReco ended" << endl;
    file->Close();
}
 
void Reconstruction::FindTracklets()
{
    //cout << "Entering FindTracklets" << endl;
    double phimax = 0.010; // maximum angle difference for 2 intersections to be a tracklet
    std::vector<MaterialBudget::fPoint> tracklet;
    for (auto x: fIntersections1)
        for (auto i: fIntersections2)
        {
            for (auto y: x)
                for (auto j: i)
                    if(abs(y.phi-j.phi)<phimax)
                    {
                        tracklet.push_back(y);
                        tracklet.push_back(j);
                    }
            fTracklets.push_back(tracklet);
        }
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

void Reconstruction::MinDca()
{
    TH1D* histo = new TH1D("vertex", "vertex", 50, -15,15);
    vector<double> vertex;
    for(auto& i: fTracklets){
        for (unsigned j=0;j<i.size();j+=2)
        {
        double a,b,c; // parameters for line connecting two intersections of the same tracklet from detector 2 to detector 1
        a = i[j+1].x - i[j].x;
        b = i[j+1].y - i[j].y;
        c = i[j+1].z - i[j].z;

        double t;
        t = - (a*i[j].x + b*i[j].y) / (a*a+b*b);    //t of closest approach

        vertex.push_back(i[j].z + c*t);
        histo->Fill(vertex.back());
        }
    }
    TCanvas* c = new TCanvas("canvas","canvas",1000,1000);
    histo->DrawClone("histo");
    int histomax(histo->GetMaximumBin());
    double xmin(histo->GetBinLowEdge(histomax)), xmax=xmin+histo->GetBinWidth(histomax);
    double mean=0;
    int count=0;
    for (auto& i : vertex)
    {
        if (i<xmax && i>xmin)
        {
            mean+=i;
            ++count;
        }
    }
    mean/=count;
    cout<<"Vertex is:"<<mean;
    c->SaveAs("Canvas.png");
    //delete histo;
}