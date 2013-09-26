// @(#)root/html:$Id: TLitSpontan.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TSpontan
#define LITR_TSpontan
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitSpontan    spontaneous source of photon inside a TGeoVolume    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"

class TGeoPhysicalNode;
class TLitMedium;
class TLitVolume;
class TLitPhoton;
class TLitResults;
class TLitMarrow;

class TLitSpontan:public TNamed {

protected:

  TString           fFullName;       //Name + run number
  TString           fFullTitle;      //Title + run number
  Bool_t            fMoving;         //True if photon cradle moving from run to run
  Bool_t            fCheckMove;      //True if chek for extrusion after move
  TGeoPhysicalNode *fGeoPhysNode;    //! TGeoPhysicalNode used when photon cradle has to be moved
  TGeoVolume       *fStartGeoVol;    //! TGeoVolume acting as photon cradle
  TLitVolume       *fStartLitVol;    //! TLitVolume associated with fStartGeoVol
  TLitMedium       *fLitMedium;      //! Medium of TGeoVolume associated with fStartLitVol
  TString           fSourcePath;     //Path of the beam cradle of photons
  Bool_t            fWvlgthFixed;    //If true, wavelength fixed, not generated by material
  Double_t          fWvlgth;         //Value of wavelength
  Int_t             fRun;            //Run number
  Bool_t            fRunStat;        //Ask for separate statistics and histograms for this run
  Bool_t            fForgetLast;     //If false [default] statistics keeped from run to run
  TLitPhoton       *fPhot;           //! pointer to generated photons
  Double_t          fT0;             //Start time of photon in picoseconds
  Bool_t            fFillDeath;      //True if FillDeath has to be called
  TH1F             *fHX0;            //! histo of x coord of death point of seen photons, defined in CINT
  TH1F             *fHY0;            //! histo of y coord of death point of seen photons, defined in CINT
  TH1F             *fHZ0;            //! histo of z coord of death point of seen photons, defined in CINT
  //
  // Class variables for recording and drawing tracks
  //
  Int_t             fDrawCode;       //! Track Nb of track to be drawn. If -1, no track to draw. If >1000000, all tracks to be drawn
  Color_t           fTrackColor;     //! Color of track to be drawn [1-9]
  Int_t             fRecordedTracks; //! Counter of tracks recorded to be drawn
  Int_t             fTrackIndex;     //! Index of the newly created track to be recorded

  void              FillDeath() const;
  void              Init();
  void              InitP();

public:

  TLitSpontan() { InitP(); }
  TLitSpontan(const char*,const char*,const char*,Bool_t=kFALSE,Bool_t=kFALSE);
  TLitSpontan(const char*,const char*,const char*,Double_t,Bool_t=kFALSE,Bool_t=kFALSE);
  virtual          ~TLitSpontan();
  void              AllTracksToDraw();
  void              Gen(Int_t,Int_t = 1,Double_t = -2.0e+20,Bool_t=kTRUE,Bool_t=kFALSE);
  Int_t             GetRecordedTracks() const  { return fRecordedTracks; }
  void              MoveCradle(TGeoMatrix*,Bool_t=kTRUE);
  void              NameFromPath(TString&) const;
  void              Periodicity(Long64_t=5000) const;
  void              Print() const;
  Bool_t            SetEmission(KindOfDist=on4pi,Double_t=180.0,TVector3=0.0,
                      const char* = "",Bool_t=kFALSE,TVector3=0.0,Bool_t=kFALSE,TVector3=0.0);
  void              SetFillDeath(TH1F*, TH1F*, TH1F*);
  void              SetRecordedTracks(Int_t k) { fRecordedTracks = k;    }
  void              SetWvlgth(Double_t);
  void              TrackToDraw(Int_t);
  ClassDef(TLitSpontan,0) //Spontaneous source of photon inside a TGeoVolume
};
#endif
