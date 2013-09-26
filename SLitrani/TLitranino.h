// @(#)root/html:$Id: TLitranino.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitranino
#define LIT_TLitranino
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitranino  Simplest possible particle to test SLitrani               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TVector3.h"

class TGeoNode;
class TGeoVolume;
class TVirtualGeoTrack;
class TLitDetector;
class TLitMedium;
class TLitVolume;

class TLitranino : public TNamed {

protected:

  TString           fFullName;       //fName + run number
  TString           fFullTitle;      //fTitle + run number
  Bool_t            fRunStat;        //Ask for separate statistics and histograms for this run
  Bool_t            fForgetLast;     //If false [default] statistics keeped from run to run
  TLitVolume       *fStartLitVol;    //TLitVolume from which litranino starts
  TString           fSourcePath;     //Path of the source of litranino
  Double_t          fMaxLength;      //Maximum length that litranino can travel before dying
  Int_t             fRun;            //Run number (>=1)
  Int_t             fNPoints;        //Number of interaction points on the neutralino trajectory
  Double_t          fWaveLength;     //Wave length of litranino is chosen randomly between 300 and 800 nm
  Double_t          fAbsLength;      //Absorption length valid for all TLitMedium for a litranino
  Double_t          fLengthInDet;    //! Length of path travelled in detector before stopping
  Double_t          fLengthMax;      //! Longest path length reached by litranino
  Double_t          fLength;         //! Current length of the path of the litranino
  Double_t          fT0;             //! Current time since birth of Litranino
  Bool_t            fIsOnBoundary;   //! True if on boundary and OnFace() has to be called
  TVector3          fCurrentPoint;   //! Current point
  TVector3          fCurrentDir;     //! Current direction
  TVector3          fStopPoint;      //! Last point of trajectory of litranino
  TVector3          fFN;             //! Normal vector to the next crossed surface at crossing point
  Double_t          fStep;           //! Step to be done in tracking
  Double_t          fSafety;         //! Safe distance
  Bool_t            fEntering;       //! True if the current track will enter inside some daughter of the current volume
  Bool_t            fPhtLive;        //! True while litranino living
  Bool_t            fAnomaly;        //! True if anomaly encountered, killing the neutralino is necessary
  TGeoNode         *fNode;           //! Current node
  TGeoVolume       *fGeoVolume;      //! Current volume
  TString           fPath;           //! Current path
  TLitVolume       *fLitVolume;      //! TLitVolume associated with fGeoVolume, if any
  TLitDetector     *fLitDet;         //! Current instance of run specific detector if fLitVolume is a detector
  TLitDetector     *fLitDetG;        //! Current instance of global detector if fLitVolume is a detector
  TGeoMedium       *fGeoMedium;      //! Current medium
  TLitMedium       *fLitMedium;      //! Current medium if it is a TLitMedium, not a TGeoMedium
  TGeoNode         *fNextNode;       //! Possible next node
  TGeoVolume       *fNextGeoVolume;  //! Next volume
  TString           fNextPath;       //! Next path
  TLitVolume       *fNextLitVolume;  //! TLitVolume associated with fNextGeoVolume, if any
  TLitDetector     *fNextLitDet;     //! Next instance of run specific detector if fLitVolume is a detector
  TLitDetector     *fNextLitDetG;    //! Next instance of global detector if fLitVolume is a detector
  TGeoMedium       *fNextGeoMedium;  //! Next medium
  TLitMedium       *fNextLitMedium;  //! Next medium if it is a TLitMedium, not a TGeoMedium
  //
  // Class variables for recording and drawing tracks
  //
  Int_t             fTrackNb;        //! Track Nb of track to be drawn. If -1, no track to draw. If >1000000, all tracks to be drawn
  Int_t             fRecordedTracks; //! Counter of tracks recorded to be drawn
  Bool_t            fToDraw;         //! True if current track is to be drawn
  Int_t             fTrackIndex;     //! Index of the newly created track to be recorded
  TVirtualGeoTrack *fCurrentTrack;   //! Track to be drawn
  Bool_t            fAnimateTracks;  //! If false, tracks only drawn. If true, tracks animated
  Int_t             fNframes;        //! Number of frames to be used for the animation


  void     InitP();
  void     Init();

public:

  static Int_t      fgNPointsMax; //Max nb. of interaction on photon trajectory

  TLitranino()                        { InitP();            }
  TLitranino(const char *,const char *,const char*,TLitVolume*,Double_t = 500.0);
  virtual      ~TLitranino();
  void          AgainstRev();
  void          AgainstShape();
  void          AllTracksToDraw();
  Bool_t        Detector();
  Bool_t        DetectorN();
  void          Gen(Int_t,Int_t=1,Double_t = -1.0e+20,Bool_t=kTRUE,Bool_t=kFALSE);
  Double_t      GetAbsLength() const       { return fAbsLength;  }
  void          InDetRecord();
  void          InMatRecord();
  void          InRevRecord();
  void          Jump();
  void          Move();
  void          OnFace();
  void          RestartPrinting(Int_t,Int_t=1);
  void          SetAbsLength(Double_t L)   { fAbsLength = L;     }
  void          SetAnimateTracks(Bool_t b) { fAnimateTracks = b; }
  void          SetNframes(Int_t k)        { fNframes = k;       }
  void          TrackToDraw(Int_t);
  ClassDef(TLitranino,1)  //Simplest possible particle to test SLitrani
};
#endif
