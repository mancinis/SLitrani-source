// @(#)root/html:$Id: TLitPhoton.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPhoton
#define LITR_TPhoton
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitPhoton    Full description of a photon inside the set-up       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TComplex.h"
#include "TVector3.h"
#include "TGeoVolume.h"
#include "TMat33.h"
#include "TVec3C.h"
#include "TMat33C.h"
#include "TLitPhys.h"

class TGeoNode;
class TVirtualGeoTrack;
class TLitDetector;
class TLitMedium;
class TLitVolume;
class TLitWave;

class TLitPhoton:public TObject {

protected:

  Int_t             fRun;            //run number
  Int_t             fKphot;          //photon number
  //source location
  TGeoVolume       *fStartGeoVol;    //! TLitVolume from which photon starts
  TString           fSourcePath;     //Path of the source of photon
  //current location
  TGeoNode         *fNode;           //! Current node
  TGeoVolume       *fGeoVolume;      //! TGeoVolume in which photon is
  TString           fPath;           //! Current path
  TLitVolume       *fLitVolume;      //! TLitVolume in which photon is
  TLitDetector     *fLitDet;         //! Current instance of run specific detector if fLitVolume is a detector
  TLitDetector     *fLitDetG;        //! Current instance of global detector if fLitVolume is a detector
  TGeoMedium       *fGeoMedium;      //! Current TGeoMedium, TGeoMedium of fGeoVolume
  TLitMedium       *fLitMedium;      //! If fGeoMedium is a TLitMedium
  //next location
  TGeoNode         *fNextNode;       //! Possible next node
  TGeoVolume       *fNextGeoVolume;  //! Next TGeoVolume
  TString           fNextPath;       //! Next path
  TLitVolume       *fNextLitVolume;  //! TLitVolume associated with fNextGeoVolume, if any
  TLitDetector     *fNextLitDet;     //! Next instance of run specific detector if fLitVolume is a detector
  TLitDetector     *fNextLitDetG;    //! Next instance of global detector if fLitVolume is a detector
  TGeoMedium       *fNextGeoMedium;  //! Next medium
  TLitMedium       *fNextLitMedium;  //! Next medium if it is a TLitMedium, not a TGeoMedium
 //
  TVector3          fX0;             //! Location of photon in World Coordinates = current point
  TVector3          fUpg;            //! Unit Poyinting vector in WCS = current direction
  TVector3          fNextPoint;      //! Possible next point of trajectory of photon
  Double_t          fNextDist;       //! Distance to possible next point fNextPoint
  Bool_t            fIsOnBoundary;   //! True if on boundary and OnFace() has to be called
  TVector3          fStopPoint;      //! Last point of trajectory of photon
  TVector3          fPtg;            //! Poyinting vector [MKSA] in WCS
  Double_t          fCospk;          //! cosine of angle between Poyinting and k vector
  TVector3          fUk0;            //! unit k vector in WCS
  Double_t          fLengthInDet;    //! Length of path travelled in detector before stopping
  TVector3          fVk0;            //! k vector in WCS
  Double_t          fAk0;            //! absolute value (module) of vector k in [cm-1]
  TComplex          fNc;             //! complex index of refraction, including absorption
  Double_t          fSl0;            //! space travelled by photon
  Double_t          fStep;           //! Step to be done in tracking
  //time
  Double_t          fT0;             //time elapsed since birth of photon or excitation in [ps]
  //status
  Int_t             fNPoints;        //Number of points on the trajectory of the photon
  Bool_t            fPhtLive;        //true if photon not yet absorbed or detected
  Bool_t            fPhtSeen;        //true if photon seen by detector
  Bool_t            fBetween;        //true if photon between 2 TGeoVolume, none of which being a wrapping
  Bool_t            fOutPoly;        //VERIFIER QUE PAS INUTILE !true if photon out of shape
  Bool_t            fPhtOut;         //true if photon is transmitted or absorbed, false if reflected.
  Bool_t            fPhtAbs;         //true if photon absorbed during transition process for instance on reflection on metal.
  Bool_t            fAnomaly;        //photon to be killed for abnormal reasons. Should never happen
  Bool_t            fFromani;        //true if incident wave in anisotropic medium
  Short_t           fChooseIndex;    //choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //physical values
  Double_t          fSkl0;           //Sum over all k*l
  Double_t          fWaveLength;     //wavelength of photon in nanometers
  Double_t          fW;              //frequency in ps-1
  Double_t          fEphot;          //energy of photon in joule [mksa]
  //The quotients of indices and magnetic permeability :
  TComplex          fBI;             // (ni*mut)/(nt*mui)
  TComplex          fBR1;            // (nr1*mut)/(nt*mui)
  TComplex          fBR2;            // (nr2*mut)/(nt*mui)
  TComplex          fBT1;            // (nt1*mui)/(ni*mut)
  TComplex          fBT2;            // (nt2*mui)/(ni*mut)
  //Absorption length and diffusion length
  Double_t          fAbsl0;          //abs. length for this photon.
  Bool_t            fIsDiffus;       //true if there is diffusion inside this material
  Double_t          fDiffusL;        //diffusion length
  Int_t             fNbOfDiffusion;  //number of diffusions that occured for this photon
  Int_t             fNbOfRayleigh;   //number of diffusions that occured for this photon
  Int_t             fNbOfWLS;        //number of wavelength shifts that occured for this photon
  //WLS and Rayleigh and absorption
  Bool_t            fIsRayleigh;     //true if there is Rayleigh scattering inside this material
  Bool_t            fIsWLS;          //true if the material is wavelength shifting
  Bool_t            fIsAbsorb;       //true if there is absorption inside this material
  Double_t          fRayleighL;      //Rayleigh Scattering length
  Double_t          fWLSL;           //Wavelength shifting length
  //Electric Field
  Double_t          fAef0;           //absolute value (module) of electric field E
  TVector3          fEf0;            //electric field [MKSA] in WCS
  Double_t          fBeta;           //phase of linearly polarized photon (irrelevant)
  //Magnetic Field
  TVector3          fBf0;            //magnetic field [MKSA] in WCS
  Double_t          fSinThetaMax;    //Max. value for angle theta in diffusion
  TVector3          fFN;             //normal to the face at the point fX0
  TVector3          fFNd;            //normal to the face, tilted by depolish
  //characteristics of material in which the photon is
  Short_t           fAnis0;          //anisotropic optical characteristics of material in which photon is
  Short_t           fAnia0;          //anisotropic absorption characteristics of material in which photon is
  Double_t          fPmu0;           //magnetic permeability of material in which photon is
  TComplex          fRfIndex0;       //refraction index, complex for convenience. Im always 0
  Double_t          fDiel0;          //diel. constant eps in case of isotropic material
  Bool_t            fIsDiag;         //the dielectric tensor in use is diagonal in ICS
  TMat33            fDzt0;           //dielectric tensor epsilon multiplied by mu in WCS
  TMat33C           fCDzt0;          //complex fDzt0, including absorption
  //characteristics of material associated with the incident wave
  Short_t           fAnisi;          //anisotropic characteristics of material associated with the incident wave
  Short_t           fAniai;          //anisotropic absorption characteristics of material associated with the incident wave
  Double_t          fPmui;           //magnetic permeability mu of material associated with the incident wave
  TComplex          fRfIndexi;       //refraction index of material associated with the incident wave
  Bool_t            fIsDiagi;        //the dielectric tensor of the shape containing the incident wave is diagonal in ICS
  TMat33            fDzti;           //dielectric tensor epsilon multiplied by mu in WCS, ass. with the incident wave
  //identical with fDzt0 when photon forward and with fDzt1 when photon backward
  TMat33            fDztii;          //idem fDzti in coordinate system of incidence (ICS)
  //characteristics of material into which the photon may go
  Short_t           fAnis1;          //anisotropic optical characteristics of destination material
  Short_t           fAnia1;          //anisotropic absorption characteristics of destination material
  Double_t          fPmu1;           //magnetic permeability of destination material
  TComplex          fRfIndex1;       //complex refraction index  of destination material
  Double_t          fDiel1;          //diel. constant eps in case of isotropic destination material
  TMat33            fDzt1;           //dielectric tensor epsilon multiplied by mu in WCS
  //characteristics of material associated with the transmitted wave
  Short_t           fAnist;          //anisotropic characteristics of material associated with the transmitted wave
  Short_t           fAniat;          //anisotropic absorption characteristics of material associated with the transmitted wave
  Double_t          fPmut;           //magnetic permeability of  of material associated with the transmitted wave
  TComplex          fRfIndext;       //refraction index of material associated with the transmitted wave
  Bool_t            fIsDiagt;        //the dielectric tensor of the shape containing the transmitted wave is diagonal in ICS
  TMat33            fDztt;           //dielectric tensor epsilon multiplied by mu in WCS, ass. with the transmitted wave
  //identical with fDzt1 when photon forward and with fDzt0 when photon backward
  TMat33            fDztti;          //idem fDztt in coordinate system of incidence (ICS)
  //Definition of the incident plane
  TVector3          fOx;             //Ox axis of the coordinate system of incidence in WCS
  TVector3          fOy;             //Oy axis of the coordinate system of incidence in WCS
  TVector3          fOz;             //Oz axis of the coordinate system of incidence in WCS
  TMat33            fMn;             //matrix transforms a vector vi in ICS into a vector v in WCS
  TMat33            fMt;             //matrix transforms a vector v in WCS into a vector vi in ICS. fMt is the transposed of fMn
  //5 possible waves at the intersection :
  TLitWave         *fI;              //incident wave
  TLitWave         *fR1;             //1st reflected wave
  TLitWave         *fR2;             //2nd reflected wave. Does not exist in isotropic material.
  TLitWave         *fT1;             //1st transmitted wave
  TLitWave         *fT2;             //2nd transmitted wave. Does not exist in isotropic material.
  //Weight of each possible choice for a wave
  Double_t          fWeigth[8];      //8 possible weigths
  OpticalProcess    fProcess;        // Optical process the photon will undergo next
  //
  // Class variables for recording and drawing tracks
  //
  Bool_t            fToDraw;         //! True if current track is to be drawn
  TVirtualGeoTrack *fCurrentTrack;   //! Track to be drawn

  void              AgainstRev();
  void              AgainstShape();
  static Bool_t     AlfaDelta(TVector3,Double_t,Double_t,Double_t&,Double_t&,Double_t&,Double_t&);
  static Bool_t     AlfaDelta(const TVec3C&,const TComplex&,const TComplex&,TComplex&,TComplex&,TComplex&,TComplex&);
  void              AniToAni() const;
  void              AniToIso();
  void              BuildBPUP(const TVector3&,const TVector3&,TVector3&,TVector3&,TVector3&,Double_t) const;
  //
  void              ch1111();
  void              ch1112();
  void              ch1121();
  void              ch1113();
  void              ch1114();
  void              ch1122();
  void              ch1123();
  void              ch1124();
  void              ch1131();
  void              ch1132();
  void              ch1133();
  void              ch1141();
  void              ch1142();
  void              ch1144();
  void              ch1211();
  void              ch1212();
  void              ch1213();
  void              ch1214();
  void              ch1221();
  void              ch1222();
  void              ch1223();
  void              ch1224();
  void              ch1231();
  void              ch1232();
  void              ch1233();
  void              ch1241();
  void              ch1242();
  void              ch1244();
  void              ch1311();
  void              ch1312();
  void              ch1313();
  void              ch1321();
  void              ch1322();
  void              ch1323();
  void              ch1331();
  void              ch1332();
  void              ch1333();
  //
  void              ch2111();
  void              ch2112();
  void              ch2121();
  void              ch2113();
  void              ch2114();
  void              ch2122();
  void              ch2123();
  void              ch2124();
  void              ch2131();
  void              ch2132();
  void              ch2133();
  void              ch2141();
  void              ch2142();
  void              ch2144();
  void              ch2211();
  void              ch2212();
  void              ch2213();
  void              ch2214();
  void              ch2221();
  void              ch2222();
  void              ch2223();
  void              ch2224();
  void              ch2231();
  void              ch2232();
  void              ch2233();
  void              ch2241();
  void              ch2242();
  void              ch2244();
  void              ch2311();
  void              ch2312();
  void              ch2313();
  void              ch2321();
  void              ch2322();
  void              ch2323();
  void              ch2331();
  void              ch2332();
  void              ch2333();
  //
  void              Choose();
  void              ClearNewVolume(Bool_t);
  void              ClearVolume();
  static TComplex   CosFromSin(TComplex);
  void              Depolish();
  void              Diffused();
  void              FindAbsLength();
  void              FindDielAni();
  void              FindDielIso();
  void              FindProcessAndLength();
  void              GetImin12(Short_t,Short_t&,Short_t&,Short_t = -1);
  void              ICStoWCS();
  void              InDetRecord();
  void              Init();
  void              InitP();
  void              InitTracking();
  Bool_t            IsDamage() const;
  void              IsoToAni();
  void              IsoToIso();
  void              IsoTsIso();
  Short_t           K(Short_t i, Short_t j) const { return 3*j + i; }
  static void       MinOrthoN(TVector3 &x,const TVector3 &v,Double_t &s,Double_t &c);
  void              NewVolume();
  void              PrintWhich() const;
  void              Regenerate(OpticalProcess);
  static void       RPhi(TComplex,Double_t&,Double_t&);
  void              SliceToRev();
  void              SliceToShape();
  static Short_t    Smallest(TVector3);
  void              StartPhoton();
  void              ToICS();
  Short_t           Toss(Short_t);
  void              VerifyAtoI();
  void              VerifyItoA();

public:

  static Int_t      fgNPointsMax; //Max nb. of interaction on photon trajectory
  static Long64_t   fgNphot;      //Total number of photons for all runs

  TLitPhoton()                                 { InitP();              }
  TLitPhoton(Int_t,Int_t,const TVector3&,TGeoVolume*,const char*,Double_t,Double_t,
    TVector3&,TVector3&,Short_t=0);
  TLitPhoton(Int_t,Int_t,const TVector3&,TLitVolume*,const char*,Double_t,Double_t,
    TVector3&,TVector3&,Short_t=0);
  virtual          ~TLitPhoton();
  Bool_t            Detector();
  Bool_t            DetectorN();
  void              ExitFace();
  Bool_t            GetAnomaly() const         { return fAnomaly;      }
  TVirtualGeoTrack *GetCurrentTrack() const    { return fCurrentTrack; }
  Bool_t            GetToDraw() const          { return fToDraw;       }
  void              Jump();
  void              Move();
  void              OnFace();
  void              SetCurrentTrack(Int_t);
  void              SetToDraw(Bool_t b)        { fToDraw = b;          }
  void              Transition();
  ClassDef(TLitPhoton,0) //Full description of a photon inside the set-up
};
#endif
