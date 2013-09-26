// @(#)root/html:$Id: TLitCascade.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TLitCascade
#define LITR_TLitCascade
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitCascade     Generation of electromagnetic showers                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TParticle.h"
#include "TString.h"
#include "TVector3.h"

class TLitMedium;
class TGeoVolume;
class TGeoNode;
class TLitPhoton;

class TLitCascade : public TParticle {

protected:

  Int_t           fRun;          //Run number
  Bool_t          fRunStat;      //Ask for separate statistics and histograms for this run
  Bool_t          fForgetLast;   //If false [default] statistics keeped from run to run
  TString         fName;         //Name of this TLitCascade
  TString         fFullName;     //name + run number. Name of histos file created
  TString         fTitle;        //Title of this TLitCascade
  TString         fFullTitle;    //Title + run number
  TString         fSourcePath;   //Path name of the TGeoVolume containing the starting point of the cascade
  TGeoNode       *fStartGeoNode; //! Node to which fStartGeoVol belongs
  TGeoVolume     *fStartGeoVol;  //! Reference TGeoVolume for positionning the cascade
  TLitMedium     *fRefLitMedium; //! Only TLitMedium inside which cascade generates light
  TVector3        fStart;        //Starting point of the cascade, in LCS of fStartGeoVol
  TVector3        fWCSStart;     //! Starting point of the cascade, in WCS
  TVector3        fZaxis;        //Axis of the cascade, pointing from start to end in LCS of fStartGeoVol
  TVector3        fWCSXaxis;     //! Complement fWCSZaxis to full coordinnate system
  TVector3        fWCSYaxis;     //! Complement fWCSZaxis to full coordinnate system
  TVector3        fWCSZaxis;     //! Axis of the cascade, pointing from start to end in WCS
  Double_t        fE0;           //Energy of the cascade in Mev
  Double_t        fCj;           //Parameter Cj "Review of Particle Physics" (23.26) p 148
  Double_t        fA;            //Parameter  a "Review of Particle Physics" (23.25) p 148
  Double_t        fB;            //Parameter  b "Review of Particle Physics" (23.25) p 148
  Double_t        fFacdEdz;      //Factor in front of (23.25)
  Double_t        fFacdEdr;      //Factor in front of (23.27)
  Double_t        fK;            //Parameter  R "Review of Particle Physics" (23.27) p 148
  Double_t        fZmax;         //Maximal length of cascade in cm. Beyond no energy deposit
  Double_t        fRmax;         //Maximal radius of cascade in cm. Beyond no energy deposit
  Double_t        fX0;           //Radiation length of material fRefLitMedium
  Double_t        fRm;           //Moliere's radius of material fRefLitMedium
  Double_t        fEc;           //Critical energy of material fRefLitMedium
  Double_t        fPhotFac;      //Factor to diminish the number of produced photons
  // Positions
  Double_t        fXdV;          //! x coord. of center of volume element in WCS
  Double_t        fYdV;          //! y coord. of center of volume element in WCS
  Double_t        fZdV;          //! z coord. of center of volume element in WCS
  Double_t        fX;            //! x coord. of current point of emission of photons in volume element [WCS]
  Double_t        fY;            //! y coord. of current point of emission of photons in volume element [WCS]
  Double_t        fZ;            //! z coord. of current point of emission of photons in volume element [WCS]
  TGeoNode       *fNode;         //! Node in which (fXdV,fYdV,fZdV) or the current (x,y,z) is
  TGeoVolume     *fGeoVolume;    //! TGeoVolume in which (fXdV,fYdV,fZdV) is
  TGeoMedium     *fGeoMedium;    //! TGeoMedium of fGeoVolume
  TLitMedium     *fLitMedium;    //! != 0 if fGeoMedium is a TLitMedium
  Double_t        fRcasc;        //! r coord. of volume element in cascade coord. system
  Double_t        fFcasc;        //! phi coord. of volume element in cascade coord. system
  Double_t        fZcasc;        //! z coord. of volume element in cascade coord. system
  Double_t        fDStepL;       //! Longitudinal step increment
  Double_t        fDStepR;       //! Radial step increment
  Double_t        fDStepF;       //! Angular step increment
  Int_t           fNStepL;       //Number of longitudinal steps
  Int_t           fNStepR;       //Number of radial steps
  Int_t           fNStepF;       //Number of steps in phi
  Int_t           fIStepL;       //! Actual longitudinal step
  Int_t           fIStepR;       //! Actual radial step
  Int_t           fIStepF;       //! Actual step in phi
  TLitPhoton     *fPhot;         //! Pointer to generated photons
  Int_t           fNphot;        //! Photon number. At the end: total number of photons generated
  Int_t           fNphotOut;     //! Number of photons killed by SetGeo because not in fRefLitMedium
  Double_t        fWvlgth;       //! Value of generated wavelength
  Double_t        fT0;           //! Start time of photon in picoseconds

  void            CoordStep();
  Double_t        DeltaE(Double_t&) const;
  Bool_t          EmissionToWCS();
  void            EmitLight(Double_t&);
  void            FirstStep();
  void            GenE(const TVector3&,TVector3&) const;
  void            GenK(TVector3&) const;
  void            GenPositionIndV();
  void            InitP();
  void            InitPrint(Int_t,Double_t,Double_t,Double_t) const;
  void            NameFile();
  void            NameFromPath(TString&) const;
  Bool_t          NextCrossing();
  Bool_t          NextStep();
  Bool_t          SetGeo();


public:

  static Double_t fZmaxLR;  //maximal length of cascade in rad. length. Beyond no energy deposit
  static Double_t fRmaxMR;  //maximal radius of cascade in Moliere radius. Beyond no energy deposit

  TLitCascade() { InitP(); }
  TLitCascade(const char*,const char*,const char*,const char*,Int_t=22,Double_t=100.0,
    TVector3=1000000000.0,TVector3=0.0,Double_t=0.05);
  TLitCascade(const char*,const char*,const char*,TLitMedium*,Int_t=22,Double_t=100.0,
    TVector3=1000000000.0,TVector3=0.0,Double_t=0.05);
  TLitCascade(const char*,const char*,const char*,TVector3,TVector3=0.0,Int_t=22,
    Double_t=100.0,Double_t=0.05);
  TLitCascade(const char*,const char*,TLitMedium*,TVector3,TVector3=0.0,Int_t=22,
    Double_t=100.0,Double_t=0.05);
  virtual        ~TLitCascade();
  void            Gen(Int_t,Double_t = -2.0e+20,Bool_t = kTRUE,Bool_t = kFALSE);
  const char     *GetName() const         { return fName.Data();  }
  Double_t        GetPhotFac() const      { return fPhotFac;      }
  const char     *GetTitle() const        { return fTitle.Data(); }
  void            NewEnergy(Double_t);
  void            NewPosition(const char*,TVector3=1000000000.0,TVector3=0.0);
  void            NewPosition(TVector3,TVector3=0.0);
  void            SetName(const char *n)  { fName = n;            }
  void            SetPhotFac(Double_t f)  { fPhotFac=f;           }
  void            SetSteps(Int_t,Int_t=35,Int_t=120);
  void            SetTitle(const char *t) { fTitle = t;           }
  ClassDef(TLitCascade,1) //Generation of electromagnetic showers
};
#endif
