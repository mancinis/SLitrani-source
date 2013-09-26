// @(#)root/html:$Id: TLitBeam.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TLitBeam
#define LITR_TLitBeam
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitBeam    Generation of a beam of particles                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "TVector3.h"


class TGeoVolume;
class TGeoPhysicalNode;
class TSpileFit;
class TLitMedium;
class TLitPhoton;
class TLitParticle;

class TLitBeam : public TNamed {

protected:

  // Run characteristics
  Int_t             fRun;            //Run number
  TString           fFullName;       //Name + run number
  TString           fFullTitle;      //Title + run number
  Bool_t            fRunStat;        //Ask for separate statistics and histograms for this run
  Bool_t            fForgetLast;     //If false [default] statistics keeped from run to run
  // Beam cradle
  TLitParticle     *fParticle;       //Particle of the beam, with energy distribution
  Bool_t            fMoving;         //True if beam cradle moving from run to run
  Bool_t            fCheckMove;      //True if chek for extrusion after move
  TString           fSourcePath;     //Path of the beam cradle of particles
  TGeoPhysicalNode *fGeoPhysNode;    //! TGeoPhysicalNode used when beam cradle has to be moved
  TGeoVolume       *fStartGeoVol;    //! TGeoVolume being the beam cradle
  TLitVolume       *fStartLitVol;    //! TLitVolume perhaps associated with fStartGeoVol [not required]
  // Current location
  TVector3          fX;              //! Current coordinates of particle in WCS
  TVector3          fDir;            //! Current direction of particle [in fact fixed!]
  Double_t          fRoad;           //! Length of path travelled by particle
  Double_t          fTime;           //! Time for particle to reach fX
  Double_t          fStep;           //! current step along particle trajectory
  TGeoNode         *fNode;           //! Current node
  TGeoVolume       *fGeoVolume;      //! TGeoVolume in which particle is
  TString           fPath;           //! Current path
  TLitVolume       *fLitVolume;      //! TLitVolume in which particle is
  TGeoMedium       *fGeoMedium;      //! Current TGeoMedium, TGeoMedium of fGeoVolume
  TLitMedium       *fLitMedium;      //! != 0 if fGeoMedium is a TLitMedium
  Bool_t            fIsdEdx;         //! True if dEdx in this TLitMedium
  TGeoNode         *fNextNode;       //! Possible next node
  // Particle emission characteristics
  KindOfDist        fKindOfEmission; //Kind of distribution for the direction of emission of particle
  Double_t          fThetaMax;       //Maximum theta angle around fSourceDir in radian
  Double_t          fApercos;        //Cos of fThetaMax
  Double_t          fApersin;        //Sin of fThetaMax
  TVector3          fAxisDirx;       //Complements fAxisDirz to have full coord. system
  TVector3          fAxisDiry;       //Complements fAxisDirz to have full coord. system
  TVector3          fAxisDirz;       //Axis around which particles are generated in local coordinates of the shape
  TString           fDistName;       //Name of SplineFit to be used in case fKindOfEmission == provided
  TSplineFit       *fFitDist;        //! TSplineFit to be used in case fKindOfEmission == provided
  Bool_t            fSourceFixed;    //If true, all particles are emitted from the fixed point source inside shape.
  Bool_t            fFromFace;       //If true, emission from face of TGeoVolume
  TVector3          fDirFace;        //Direction to take to reach face, in LCS
  TVector3          fWCSDirFace;     //! Idem fDirFace in WCS
  TVector3          fSourcePoint;    //Chosen or generated point of emission of particle in local coordinate of shape
  TVector3          fWCSSourcePoint; //! fSourcePoint in WCS
  TVector3          fSourceDir;      //! Generated direction of emission around fAxisDirz of particle in local coordinate of shape
  TVector3          fAxisb1;         //! Complement fWCSSourceDir to have full coordinate system
  TVector3          fAxisb2;         //! Complement fWCSSourceDir to have full coordinate system
  TVector3          fWCSSourceDir;   //! fSourceDir in WCS
  Double_t          fTheta;          //! Generated theta for the emission of the particle or of the particle
  Double_t          fPhi;            //! Generated phi for the emission of the particle or of the particle
  Double_t          fnCerenkov;      //! Index of refraction to be used in Cerenkov formulae for fLitMedium
  //Characteristics of photon generated
  TLitPhoton       *fPhot;           //! Pointer to generated photon
  Double_t          fWvlgth;         //! Wavelength of this photon in [nm]
  TVector3          fX0;             //! Starting point of photon;
  Double_t          fT0;             //! Time of emission of this photon in [ps]

  Bool_t            EmissionToWCS();
  void              EmitLight(Double_t);
  void              Gen4pi();
  void              GenE(const TVector3&,TVector3&) const;
  void              GenFlat();
  void              GenK(TVector3&);
  void              GenKECer(TVector3&,TVector3&) const;
  Bool_t            GenSourceDir();
  Bool_t            GenProvided();
  void              GenSinCos();
  Bool_t            GenSourcePoint();
  void              Init();
  void              InitP();
  void              InitTracking();
  void              NameFile();
  Bool_t            NextCrossing();
  Bool_t            StartOneParticle();
  void              WandLCerenkov(Double_t&,Double_t&) const;

public:

  TLitBeam()                                  { InitP();                   }
  TLitBeam(const char*,const char*,const char*,TLitParticle* = 0,Bool_t=kFALSE,Bool_t=kFALSE);
  virtual          ~TLitBeam();
  void              Gen(Int_t, Int_t = 1, Double_t = -2.0e+20, Bool_t = kTRUE,Bool_t = kFALSE, Double_t = 0.5);
  const char       *FullName() const          { return fFullName.Data();   }
  const char       *FullTitle() const         { return fFullTitle.Data();  }
  TVector3          GetAxisDirx() const       { return fAxisDirx;          }
  TVector3          GetAxisDiry() const       { return fAxisDiry;          }
  TVector3          GetAxisDirz() const       { return fAxisDirz;          }
  TVector3          GetDirFace() const        { return fDirFace;           }
  const char       *GetDistName() const       { return fDistName.Data();   }
  Bool_t            GetFromFace() const       { return fFromFace;          }
  KindOfDist        GetKindOfEmission() const { return fKindOfEmission;    }
  TLitParticle     *GetParticle()             { return fParticle;          }
  Bool_t            GetSourceFixed() const    { return fSourceFixed;       }
  const char       *GetSourcePath() const     { return fSourcePath.Data(); }
  TVector3          GetSourcePoint() const    { return fSourcePoint;       }
  Double_t          GetThetaMax() const       { return fThetaMax;          }
  void              MoveCradle(TGeoMatrix*,Bool_t=kTRUE);
  void              NameFromPath(TString&) const;
  void              SetCerenkov(Bool_t);
  Bool_t            SetEmission(KindOfDist=flat,Double_t=1.0,TVector3=0.0,const char* ="",
                      Bool_t=kFALSE,TVector3=0.0,Bool_t=kTRUE,TVector3=0.0);
  void              SetParticle(TLitParticle*,Bool_t=kFALSE);
  ClassDef(TLitBeam,1) //Defines a beam of particles
};
#endif
