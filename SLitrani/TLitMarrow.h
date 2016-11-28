// @(#)root/html:$Id: TLitMarrow.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitMarrow
#define LITR_TLitMarrow
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitMarrow  Creates the final histograms presenting values           //
//            (efficiency, losses,etc) as a function of the parameter   //
//            varying from run to run                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH1.h"

class TLitResults;

class TLitMarrow : public TNamed {

protected:

  Bool_t       fInMem;                 //statistics is in memory.
  Int_t        fK;                     //histogram to be drawn
  Int_t        fNDet;                  //number of detectors
  Int_t        fNHperDet;              //number of summary histogram per detector
  Int_t        fNbins;                 //number of bins

  void         BookHistos();           //All booking of histograms
  void         FillHistos();           //Filling of all histograms
  TLitResults *GlobalP() const;        //Pointer to global statistics


public:

  TH1F        *fHEfficiency;           //Efficiency per run
  TH1F        *fHAbsMat;               //Photons lost inside non-sensitive material per run
  TH1F        *fHAbsBef;               //Photons absorbed before wrapping per run
  TH1F        *fHAbsRvt;               //Photons absorbed by wrapping per run
  TH1F        *fHOutSide;              //Photons lost because they leave the setup per run
	TH1F        *fHAbnorm;               //Photons lost for abnormal reasons per run
  TH1F        *fHTooLate;              //Photons lost because seen too late per run
  TH1F        *fHTooFew;               //Photons lost because of too few electrons in APD per run
  TH1F        *fHLossAng;              //Lost because of acceptance angle per run
  TH1F        *fHLossQE;               //Lost because of quantum efficiency per run

  TLitMarrow()              { InitP();   }
  TLitMarrow(const char*, const char*, Bool_t = kTRUE);
  virtual     ~TLitMarrow();
  void         ClearHistos();
  void         D(Int_t,Int_t) const;
  void         DrawAbnorm() const;     //Photons lost for abnormal reasons per run
  void         DrawAbsBef() const;     //Photons absorbed before wrapping per run
  void         DrawAbsMat() const;     //Photons lost inside non-sensitive material per run
  void         DrawAbsRvt() const;     //Photons absorbed by wrapping per run
  void         DrawAllLosses() const;  //Draw all losses, superimposed.
  void         DrawEfficiency() const; //Efficincy per run
  void         DrawLossAng() const;    //Lost because of acceptance angle per run
  void         DrawLossQE() const;     //Lost because of quantum efficiency per run
  void         DrawOutSide() const;    //Photons lost because they leave the setup per run
  void         DrawTooFew() const;     //Photons lost because of too few electrons in APD per run
  void         DrawTooLate() const;    //Photons lost because seen too late per run
  Int_t        GetfH() const   { return fK; }
  void         InitP();
  void         N();
  void         N1();
  void         ResetCounter()  { fK = 0;    }
  void         Summary();
  ClassDef(TLitMarrow,1)//Make the final analysis.
};

R__EXTERN TLitMarrow *gLitGp;

#endif
