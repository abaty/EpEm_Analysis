#ifndef SETTINGS
#define SETTINGS

#include <string>
#include <iostream>
#include "TH1F.h"

class Settings{
  public:

    std::string inputFile = "/data/abaty/ALEPHTrees/cleaned_ALEPH_Data2-v3_Aug11_2017.root";

    //cuts
    bool doUseLeptons = true;

    //kinematics
    float etaCut = 1.8;
    float dEtaBins = 36;//keep even
    float dPhiBins = 36;//keep factor of 4

    float dEtaRangeToIntegrate[2] = {2.0,3.6};//try to make this correspond with bin edges based on above parameters

    //mixing
    int nMixedEvents = 5;
    int maxSkipSize  = 3;


    //plots
    static const int nMultBins = 5;
    int multBinsLow[nMultBins]  = {0,   0 , 15, 25, 35};
    int multBinsHigh[nMultBins] = {999, 15, 25, 35, 999};

    //other
    bool doAllData = true;
    int nEvts = 5000;

    Settings();
    bool isInMultBin(int n, int bin);

  private:

};

Settings::Settings()
{
  std::cout << "Getting settings.." << std::endl;
  return;
}

bool Settings::isInMultBin(int n, int bin){
  if(bin >= nMultBins) std::cout << "Error in isInMultBin(): bin out of bounds!" << std::endl;
  if(n >= multBinsLow[n] && n < multBinsHigh[bin]) return true;
  return false;
}

#endif
