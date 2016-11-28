// @(#)root/html:$Id: TLitResults.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TLitResults
#define LITR_TLitResults
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitResults    Summary of all statistics and histograms for one    //
//                  run or for all runs                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TPaveLabel.h"
#include "TPaveText.h"

class TLitResults : public TNamed {

protected:

  Int_t       fRun;            //run number. If 0 : global statistics
  Bool_t      fConclusion;     //true only when Conclusion() has been called
  Double_t    fXparam;         //parameter to take as abscissa for TLitMarrow
  Short_t     fNfluo;          //Nb of components of fluorescence for material histogrammed
  Short_t     fNbHistos;       //Number of histograms or graphs (not belonging to detectors) to be plotted
  TObjArray  *fListOfDet;      //List of physical detectors with their histograms
  TPaveLabel *fPavel;          //! Pave Label for the summary plot
  TPaveText  *fPavet;          //! Pave Text for the summary plot
  Int_t       fKN1;            //! histo to be plotted by N1()
  Int_t       fKD1;            //! histo to be plotted by D1()
  Short_t     fKDet;           //! Kth detector to be plotted
  Int_t       fNdet;           //! Number of detectors
  Int_t       fNh;             //! Number of booked histos in detector

  Short_t     AtLeastOne();    //Chooses at least one fluorescent material to histogram
  void        Init();          //Initialization
  void        InitP();         //Put pointers to 0

public:

  static Double_t fgEnergyOfGamma; //Maximal energy of gammas defined in TLitGamma;

  //General
  Int_t       fNpGener;        //Nb. of photons generated
  TH1D       *fHPoints;        //( 0) - Nb. of points on photon trajectory
  //Detection of photons
  Int_t       fNpSeen;         //Nb. of photons seen
  Double_t    fPhweigth;       //(Photons arrived at detector with QE)*(Quantum Efficiency)
  TH1D       *fHTimeSeen;      //( 1) - time of arrival of detected photons on any detector
  TH1D       *fHDistSeen;      //( 2) - distance travelled by photons seen in any detector
  TH1D       *fHWvlgthSeen;    //( 3) - Wavelength of photons seen
  //Loss of photons
  Int_t       fNpLossAny;      //Nb. of photons lost for ANY reason
  Int_t       fNpAbsMat;       //Nb. of photons lost inside non-sensitive material
  Int_t       fNpAbsBef;       //Nb. of photons absorbed before wrapping (supplementary absorption)
  Int_t       fNpAbsRvt;       //Nb. of photons absorbed by wrapping
  Int_t       fNpOutSide;      //Nb. of photons lost because they leave the setup
  Int_t       fNpAbnorm;       //Nb. of photons lost for abnormal reasons
  Int_t       fNpTooLate;      //Nb. of photons lost because seen too late
  Int_t       fNpTooFew;       //photons lost because of too few electrons in APD
  Int_t       fNpLossAng;      //Lost because of acceptance angle
  Int_t       fNpLossQE;       //Lost because of quantum efficiency
  Int_t       fNpAbsL0;        //Lost because user put a value of 0 for absorption length
  TH1D       *fHAbsAny;        //( 4) - distance travelled by photons lost anywhere
  TH1D       *fHAbsMat;        //( 5) - Nb. of photons absorbed in various material
  TH1D       *fHAbsBef;        //( 6) - Nb. of photons absorbed before various wrappings
  TH1D       *fHAbsRvt;        //( 7) - Nb. of photons absorbed by various wrappings
  TH1D       *fHOutSide;       //( 8) - distance travelled by photons leaving the setup
  TH1D       *fHAbnLost;       //( 9) - distance travelled by photons abnormally lost
  TH1D       *fHTooLate;       //(10) - Wavelength of photons seen, but too late
  //Diffusion of photons
  TH1D       *fHDiffusion;     //(11) - Number of diffusions done by photon
  //Time and Wavelength
  TH1D       *fHTimeAll;       //(12) - generated time of emission, all components of fluorescence
  TH1D       *fHWvlgthAll;     //(13) - generated wavelength all components of fluorescence
  TH1D       *fHTimeCer;       //(14) - generated time of emission for Cerenkov photons
  TH1D       *fHWvlgthCer;     //(15) - generated wavelength for Cerenkov photons
  TH2F       *fHTimeEach;      //(16) - generated time of emission, each component of fluorescence
  TH2F       *fHWvlgthEach;    //(17) - generated wavelength each component of fluorescence
  //Particle generation
  Int_t       fNbPart;         //number of particles generated
  TH1D       *fHPMomentum;     //(18) - momentum of generated particles
  TH1D       *fHPTheta;        //(19) - theta distribution of generated particles
  TH2F       *fHEnergyInShape; //(20) - Energy deposited in shape k by gamma
  // Transition between media
  TH2F       *fHMatMat;        //(21) - Transition from one material towards another material
  TH2F       *fHMatNot;        //(22) - Reflection from one material by another material
  TH2F       *fHMatRev;        //(23) - Absorption by wrapping when coming from mat
  TH2F       *fHRevMat;        //(24) - Reflection by wrapping when coming from mat

  static TObjArray *fgResults; //List of all statistics of run

  TLitResults() { Init();}
  TLitResults(const char*, const char*, Int_t, Double_t = -2.0e+20);
  virtual    ~TLitResults();
  Bool_t      AddToList();
  void        CInit();
  Int_t       Compare(const TObject*) const;
  void        Conclusion();
  void        D();
  Int_t       D1();
  void        DelHistos();
  void        DelPaveT();
  void        DoStat();
  TObjArray  *GetListOfDet() const { return fListOfDet; }
  Int_t       GetRun() const       { return fRun;       }
  Double_t    GetXparam() const    { return fXparam;    }
  void        GInit();
  Bool_t      HasFluo() const      { return fNfluo>0;   }
  Bool_t      IsEqual(const TObject*) const;
  Bool_t      IsSortable() const   { return kTRUE;      }
  void        N();
  Int_t       N1();
  void          ResetCounters();
  ClassDef(TLitResults,1) //Summary of all statistics and histograms for one run
};
R__EXTERN TLitResults *gLitGs;
R__EXTERN TLitResults *gLitCs;
#endif
