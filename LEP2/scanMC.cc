#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include "TLorentzVector.h"

#define PI 3.1415926
enum SIMPLEPWFLAG {CHARGED_TRACK, CHARGED_LEPTONS1, CHARGED_LEPTONS2, V0, PHOTON, NEUTRAL_HADRON};
/** 
The process: 
 
 STEP 1
 Go into loopMC.sh and edit the file so that the top is as follows:
 folder="LEP2_MC/GGUD"
 filelist="LEP2MCGGUDY1997E183_mctrue_aftercut-001.list"
 output="cleaned_LEP2MCGGUDY1997E183_mctrue_aftercut-001.aleph"
 suffix="LEP2MCGGUDY1997E183_mctrue_aftercut-001"   

This means that the aleph monte-carlo file has to be in the specified folder and that there should exist a filelist in LEP2 with the name of aleph file in it. Then it outputs the cleaned file. 
 
 STEP 2
 Replace the infile into scanMC with the correct file from LEP2 and run scanMC. This will output a root file into ROOTFiles
 
 */
TString filename = "/Users/anthony/desktop/LEP2MCGGUDY1997E183_mctrue_beforecut-001.aleph.root";

using namespace std;

class particleData
{
public:
    int nParticle;
    int EventNo;
    int RunNo;
    float Energy;
    
    Float_t px[100000];
    Float_t py[100000];
    Float_t pz[100000];
    Float_t pt[100000];
    Float_t eta[100000];
    Float_t phi[100000];
    Float_t mass[100000];
    Float_t charge[10000];
    Float_t pwflag[10000];
    Float_t pid[10000];
};

void scanMC (TString infile="/Users/anthony/desktop/LEP2MCGGUDY1997E183_mctrue_beforecut-001.aleph"){
    
    FILE *fp=fopen(Form("%s",infile.Data()),"r");
    
    float _px,_py,_pz,_m,_charge,_pwflag,_pid;
    
    TFile *hf = new TFile(Form("%s.root",infile.Data()), "RECREATE" );
    TTree *tout = new TTree("t","");
    TLorentzVector v;
    cout<<"step1"<<endl;
    particleData pData;
    tout->Branch("nParticle", &pData.nParticle,"nParticle/I");
    tout->Branch("EventNo", &pData.EventNo,"EventNo/I");
    tout->Branch("RunNo", &pData.RunNo,"RunNo/I");
    tout->Branch("Energy", &pData.Energy,"Energy/F");
    tout->Branch("px", pData.px,"px[nParticle]/F");
    tout->Branch("py", pData.py,"py[nParticle]/F");
    tout->Branch("pz", pData.pz,"pz[nParticle]/F");
    tout->Branch("pt", pData.pt,"pt[nParticle]/F");
    tout->Branch("mass", pData.mass,"mass[nParticle]/F");
    tout->Branch("eta", pData.eta,"eta[nParticle]/F");
    tout->Branch("phi", pData.phi,"phi[nParticle]/F");
    tout->Branch("charge", pData.charge,"charge[nParticle]/F");
    tout->Branch("pwflag", pData.pwflag,"pwflag[nParticle]/F");
    tout->Branch("pid", pData.pid,"pid[nParticle]/F");
    
    int counterEntries=0;
    int counterParticles=0;
    
    cout<<"step2"<<endl;
    while(fscanf(fp,"%f %f %f %f %f %f %f",&_px,&_py,&_pz,&_m,&_charge,&_pwflag,&_pid)!=EOF) {
        if (_px==-999.&&_py==-999.&&_pz==-999.) {
            cout<<"counterEntries="<<counterEntries<<endl;
            pData.nParticle=counterParticles;
            if(counterEntries>0) tout->Fill();
            pData.RunNo=_charge;
            pData.EventNo=_pwflag;
            pData.Energy=_pid;
            counterParticles=0;
            cout<<"------------------------------------------------------------------------"<<endl;
            continue;
        }
        cout<<_px<<"    "<<_py<<"    "<<_pz<<"    "<<_charge<<"   "<<_pwflag<<"   "<<_pid<<endl;
        pData.px[counterParticles]=_px;
        pData.py[counterParticles]=_py;
        pData.pz[counterParticles]=_pz;
        pData.mass[counterParticles]=_m;
        v.SetXYZM(_px,_py,_pz,_m);
        pData.pt[counterParticles]=v.Pt();
        pData.eta[counterParticles]=v.PseudoRapidity();
        pData.phi[counterParticles]=v.Phi();
        pData.charge[counterParticles]=_charge;
        pData.pwflag[counterParticles]=_pwflag;
        pData.pid[counterParticles]=_pid;
        counterParticles++;
        counterEntries++;
    }
    hf->Write();
}


void checkMC(){
    
    // FILE LOCATION NEEDS TO BE FIXED TO svmithi02 location
    // NOT ALREADY FIXED BECAUSE CURRENT FILE NOT IN SPECIFIED LOCATION
    TFile *f = new TFile("/data/flowex/Datasamples/LEP2_MAIN/ROOTfiles/cleaned_LEP2MCGGBBY2000E207_mctrue_beforecut-001.aleph.root");
    TTree *t1 = (TTree*)f->Get("tgen");
    Int_t nParticle,EventNo,RunNo;
    Float_t Energy;
    Float_t px[100000];
    Float_t py[100000];
    Float_t pz[100000];
    Float_t pt[100000];
    Float_t eta[100000];
    Float_t pid[100000];
    Float_t phi[100000];
    Float_t mass[100000];
    
    t1->SetBranchAddress("nParticle",&nParticle);
    t1->SetBranchAddress("EventNo",&EventNo);
    t1->SetBranchAddress("RunNo",&RunNo);
    t1->SetBranchAddress("Energy",&Energy);
    t1->SetBranchAddress("px",px);
    t1->SetBranchAddress("py",py);
    t1->SetBranchAddress("pz",pz);
    t1->SetBranchAddress("pt",pt);
    t1->SetBranchAddress("eta",eta);
    t1->SetBranchAddress("pid",pid);
    t1->SetBranchAddress("phi",phi);
    t1->SetBranchAddress("mass",mass);
    
    Int_t nevent = (Int_t)t1->GetEntries();
    
    for (Int_t i=0;i<nevent;i++) {
        t1->GetEntry(i);
        
        int nparticles = nParticle;
        cout<<"------------------------------------------------------------------------"<<endl;
        cout<<"nparticles="<<nparticles<<endl;
        
        for ( int j=0;j<nparticles;j++ ) {
            
            int pid1 = pid[j];
            // if (pid1!=PION&&pid1!=PROTON&&pid1!=KAON) continue;
            float eta1 = eta[j];
            float phi1 = phi[j];
            float px1 = px[j];
            float py1 = py[j];
            float pz1 = pz[j];
            float pt1 = pt[j];
            float mass1 = mass[j];
            cout<<"EventNo="<<EventNo<<" px="<<px1<<"py="<<py1<<"pz="<<pz1<<"pt="<<pt1<<"mass="<<mass1<<endl;
        }
    }
}


double dphi(double phi1,double phi2)
{
    double a=phi1-phi2;
    while (a<-PI) a+=2*PI;
    while (a>PI) a-=2*PI;
    
    if (a<-PI/2) a=2*PI+a;
    return a;
}



void thrust(int isBelle, float x[], float y[]){
    
    //TString filename;
   // if(isBelle) filename="/data/flowex/Datasamples/Belle/output_2_withtheta.root";
    //else filename="/data/flowex/Datasamples/LEP2_MAIN/ROOTfiles/cleaned_ALEPH_Data-all.aleph.root";
    
    
    //filename ="/mnt/c/Users/Bibek Kumar Pandit/Desktop/Root_Directory/StudyMult/DataFiles/ROOTfiles/cleaned_ALEPH_Data-all";
    
    TFile *f = new TFile(filename.Data());
    TTree *t1 = (TTree*)f->Get("t");
    Int_t nParticle;
    Float_t pt[50000];
    Float_t px[50000];
    Float_t py[50000];
    Float_t pz[50000];
    Float_t eta[50000];
    Float_t theta[50000];
    Float_t pid[50000];
    Float_t phi[50000];
    Float_t mass[50000];
    Float_t pwflag[50000];
    
    t1->SetBranchAddress("nParticle",&nParticle);
    t1->SetBranchAddress("pt",pt);
    t1->SetBranchAddress("px",px);
    t1->SetBranchAddress("py",py);
    t1->SetBranchAddress("pz",pz);
    t1->SetBranchAddress("eta",eta);
    t1->SetBranchAddress("theta",theta);
    t1->SetBranchAddress("pid",pid);
    t1->SetBranchAddress("phi",phi);
    t1->SetBranchAddress("mass",mass);
    t1->SetBranchAddress("pwflag",pwflag);
    
    /*
     x=sin(theta)*cos(phi)
     y=sin(theta)*sin(phi)
     z=cos(theta)
     */
    
    int nstepstheta=100;
    double mintheta=0.;
    double maxtheta=3.14;
    
    int nstepsphi=100;
    double minphi=-3.14;
    double maxphi=3.14;
    
    double sizetheta=(maxtheta-mintheta)/(double)nstepstheta;
    double sizephi=(maxphi-minphi)/(double)nstepsphi;
    
    double mytheta,myphi;
    double x_versor,y_versor,z_versor;
    TVector3 unity_versor;
    TVector3 particle_vector;
    
    double maxscalar,theta_max,phi_max;
    
    // all entries and fill the histograms
    Int_t nevent = (Int_t)t1->GetEntries();
    
    int nevent_process = nevent; //59874
    
    for (Int_t i=0;i<nevent_process;i++)
    {
        if (i%10==0) cout <<i<<"/"<<nevent_process<<endl;
        t1->GetEntry(i);
        
        int nparticles = nParticle;
        
        maxscalar=-1;
        
        for ( int itheta=0; itheta<nstepstheta; itheta++ )
        {
            mytheta=sizetheta*(double)itheta;
            myphi = -3.14;
            for ( int iphi=0; iphi<nstepsphi; iphi++ )
            {
                
                x_versor=sin(mytheta)*cos(myphi);
                y_versor=sin(mytheta)*sin(myphi);
                z_versor=cos(mytheta);
                
                unity_versor.SetXYZ(x_versor,y_versor,z_versor);
                double scalar_prod;
                
                scalar_prod=0.;
                
                for ( int j=0;j<nparticles;j++ )
                {
                    float px1=px[j];
                    float py1=py[j];
                    float pz1=pz[j];
                    particle_vector.SetXYZ(px1,py1,pz1);
                    scalar_prod = scalar_prod + fabs(particle_vector.Dot(unity_versor));
                }//end of loop over particles
                
                myphi+=sizephi;
                if (scalar_prod>maxscalar) { maxscalar=scalar_prod; theta_max=mytheta; phi_max=myphi;}
            }//loop over phi
        }//loop over theta
        //int binmax = hprojection[i]->GetMaximumBin();
        x[i] = theta_max;
        y[i] = phi_max;
    }// end of loop over events
}




void scan_thrust(){
    //TString filename;
    //filename="/mnt/c/Users/Bibek Kumar Pandit/Desktop/Root_Directory/StudyMult/DataFiles/ROOTfiles/cleaned_ALEPH_Data-all.aleph.root";
    //filename = "/Users/anthony/desktop/LEP2MCGGUDY1997E183_mctrue_beforecut-001.aleph.root";
    
    
    
    TFile *f = new TFile(filename.Data());
    TTree *t1 = (TTree*)f->Get("t");
    
    
    
    int nParticle;
    int EventNo;
    int RunNo;
    float Energy;
    float TTheta; //to be added
    float TPhi; //to be added
    
    Float_t px[100000];
    Float_t py[100000];
    Float_t pz[100000];
    Float_t pt[100000];
    Float_t eta[100000];
    Float_t theta[100000];
    Float_t phi[100000];
    Float_t mass[100000];
    Float_t charge[10000];
    Float_t pwflag[10000];
    Float_t pid[10000];
    
    t1->SetBranchAddress("nParticle",&nParticle);
    t1->SetBranchAddress("EventNo",&EventNo);
    t1->SetBranchAddress("RunNo",&RunNo);
    t1->SetBranchAddress("Energy",&Energy);
    t1->SetBranchAddress("px",px);
    t1->SetBranchAddress("py",py);
    t1->SetBranchAddress("pz",pz);
    t1->SetBranchAddress("pt",pt);
    t1->SetBranchAddress("mass",mass);
    t1->SetBranchAddress("eta",eta);
    t1->SetBranchAddress("theta",theta);
    t1->SetBranchAddress("phi",phi);
    t1->SetBranchAddress("charge",charge);
    t1->SetBranchAddress("pwflag", pwflag);
    t1->SetBranchAddress("pid",pid);
    
    
    
    
    
    
    //TFile *g = new TFile("/mnt/c/Users/Bibek Kumar Pandit/Desktop/Root_Directory/StudyMult/DataFiles/ROOTfiles/cleaned_ALEPH_Data2-all.aleph.root", "RECREATE");
    
    TFile *g = new TFile("/Users/anthony/desktop/LEP2MCGGUDY1997E183_mctrue_beforecut-001_thrust.aleph.root","RECREATE");
    
    
    
    
    TTree *newtree = t1->CloneTree(0); // Do no copy the data yet
    // DO NOT delete the old tree and close the old file
    //add the branch to the new tree and try to fill it
    newtree->Branch("TTheta", &TTheta, "TTheta/F");
    newtree->Branch("TPhi", &TPhi, "TPhi/F");
    
    
    //int nentries=newtree->GetEntries();
    Int_t nevent = (Int_t)t1->GetEntries();
    
    int nevent_process = nevent;
    
    //cout<<nentries<<endl;
    //int nentries = 100;
    float x[nevent_process];
    memset(x, 0, nevent*sizeof(float));
    
    float y[nevent_process];
    memset(y, 0, nevent*sizeof(float)); 
    
    thrust(0,x,y);
    
    
    for( int i=0; i < nevent_process; i++){
        t1->GetEntry(i);
        TTheta = x[i];
        TPhi = y[i];
        newtree->Fill();
    }
    g->Write();
}

void clean()
{
    scanMC();
    scan_thrust();
}

