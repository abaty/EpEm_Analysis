#include <TTree.h>


// DataFormat
class TPCNtupleData{
    public:
    Int_t nParticle;
    Float_t pt[50000];
    Float_t eta[50000];
    Float_t theta[50000];
    Float_t pid[50000];
    Float_t phi[50000];
    Float_t mass[50000];
    Float_t pwflag[50000];
    
    Float_t px[100000];
    Float_t py[100000];
    Float_t pz[100000];
    Float_t TTheta;
    Float_t TPhi;
};


// Set the branch addresses
void setupTPCTree(TTree *t1, TPCNtupleData &data)
{
    t1->SetBranchAddress("nParticle",&data.nParticle);
    t1->SetBranchAddress("pt",data.pt);
    t1->SetBranchAddress("eta",data.eta);
    t1->SetBranchAddress("theta",data.theta);
    t1->SetBranchAddress("pid",data.pid);
    t1->SetBranchAddress("phi",data.phi);
    t1->SetBranchAddress("mass",data.mass);
    t1->SetBranchAddress("pwflag",data.pwflag);
    
    t1->SetBranchAddress("px",data.px);
    t1->SetBranchAddress("py",data.py);
    t1->SetBranchAddress("pz",data.pz);
    t1->SetBranchAddress("TTheta", &data.TTheta);
    t1->SetBranchAddress("TPhi", &data.TPhi);
}
