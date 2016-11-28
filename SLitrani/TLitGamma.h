// @(#)root/html:$Id: TPhotoElecCompton.h 2008-06-11
// Author: D. Wahl <mailto:david.n.wahl@gmail.com>,F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TLitGamma
#define LITR_TLitGamma
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitGamma   Generation of gamma rays of energy approx 0.1-1 Mev      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TVector3.h"
#include "TSplineFit.h"
#include "TLitParticle.h"

class TVirtualGeoTrack;
class TLitMedium;
class TLitVolume;
class TLitPhoton;

class TLitGamma : public TLitParticle {

protected:

  Int_t             fRun;            //Run number
  TString           fName;           //Name of this TLitCascade
  TString           fFullName;       //name + run number. Name of histos file created
  TString           fTitle;          //Title of this TLitCascade
  TString           fFullTitle;      //Title + run number
  Bool_t            fRunStat;        //Ask for separate statistics and histograms for this run
  Bool_t            fForgetLast;     //If false [default] statistics keeped from run to run
  // Beam cradle
  Bool_t            fMoving;         //True if beam cradle moving from run to run
  Bool_t            fCheckMove;      //True if chek for extrusion after move
  TString           fSourcePath;     //Path of the beam cradle of particles
  TGeoPhysicalNode *fGeoPhysNode;    //! TGeoPhysicalNode used when beam cradle has to be moved
  TGeoVolume       *fStartGeoVol;    //! TGeoVolume being the beam cradle
  TLitVolume       *fStartLitVol;    //! TLitVolume perhaps associated with fStartGeoVol [not required]
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
  // Characteristics of gamma and current location
  TVector3          fX;              //! Current position of Gamma in WCS
  TVector3          fDir;            //! Current direction of Gamma in WCS
  TVector3          fDirElec;        //! Direction of the scattered electron. Of use later, if tracking of scattered e-
  Double_t          fRoad;           //! Length of path travelled by particle
  Double_t          fTime;           //! Time for gamma to reach fX
  TGeoNode         *fNode;           //! Node to which fStartGeoVol belongs
  TGeoVolume       *fGeoVolume;      //! TGeoVolume in which particle is
  Int_t             fKGeoVolume;     //! Index of fGeoVolume in collection TGeoManager::fVolumes
  TString           fPath;           //! Current path
  TLitVolume       *fLitVolume;      //! TLitVolume in which particle is
  TGeoMedium       *fGeoMedium;      //! Current TGeoMedium, TGeoMedium of fGeoVolume
  TLitMedium       *fLitMedium;      //! != 0 if fGeoMedium is a TLitMedium
  Double_t          fZ;              //! Z of fGeoMedium
  Double_t          fA;              //! A of fGeoMedium
  Double_t          fDensity;        //! Density of fGeoMedium
  TGeoNode         *fNextNode;       //! Possible next node
  Double_t          fStepMax;        //! Maximum possible step to stay inside fGeoVolume
  //  Energy deposition
  Bool_t            fComptonOk;      //! True if Compton effect possible in fGeoMedium: A,Z and rho defined
  Bool_t            fPhotoElecOk;    //! True if PhotoElectric effect possible in fLitMedium
  Bool_t            fComptonCut;     //! True if Compton cross section cut at 0.1 Mev
  Bool_t            fGammaDead;      //! Did gamma gave all its energy, for instance by photo-electric cross-section
  EDepProcess       fProcess;        //! Current process {kPhotoElectric,kComptonScattering,kCascade,kBeam}
  Double_t          fEIncident;      //! Energy [Mev] of incidental gamma
  Double_t          fEScattered;     //! Energy [Mev] of scattered gamma in Compton Scattering
  Double_t          fEDeposit;       //! Energy [Mev] deposited in step. By PE or by scattered e- in CS
  Double_t          fTotEdeposit;    //! Total energy deposited in fGeoVolume
  Int_t             fNCompton;       //! Compton interaction counter
  Double_t          fXsecCompton;    //! Compton scattering cross section * 10E+24 cm2 in fGeoVolume at energy fEIncident
  Double_t          fXsecPhotoEl;    //! PhotoElectric cross section      * 10E+24 cm2 in fGeoVolume at energy fEIncident
   // Characteristics of photon generated
  Int_t             fNphot;          //! Number of photons generated by this gamma
  TLitPhoton       *fPhot;           //! Pointer to generated photon
  Double_t          fWvlgth;         //! Wavelength of this photon in [nm]
  TVector3          fX0;             //! Starting point of photon;
  Double_t          fT0;             //! Time of emission of this photon in [ps]
  // Fill Death members
  Bool_t            fFillDeath;      //! true if FillDeath has to be called
  TH1F             *fHX0;            //! hist of x coord of death point of seen photons
  TH1F             *fHY0;            //! hist of y coord of death point of seen photons
  TH1F             *fHZ0;            //! hist of z coord of death point of seen photons
  // Class variables for recording and drawing gammas
  Bool_t            fToDraw;         //! Gamma is to be drawn
  Int_t             fDrawCode;       //! Gamma Nb of gamma to be drawn. If -1, no gamma to draw. If >1000000, all gammas to be drawn
  Color_t           fTrackColor;     //! Color of gamma to be drawn [1-9]
  Int_t             fRecordedTracks; //! Counter of gammas recorded to be drawn
  Int_t             fTrackIndex;     //! Index of the newly created gamma to be recorded
  TVirtualGeoTrack *fCurrentTrack;   //! Track to be drawn

  void              AdaptHistMom(TH1D*);
  void              Clear();
  Bool_t            DepositEnergy();
  Bool_t            EmissionToWCS();
  void              EmitLight();
  void              FillDeath() const;
  void              Gen4pi();
  void              GenE(const TVector3&,TVector3&) const;
  void              GenFlat();
  void              GenK(TVector3&) const;
  Bool_t            GenProvided();
  void              GenSinCos();
  Bool_t            GenSourceDir();
  Bool_t            GenSourcePoint();
  Double_t          Get_dSigma_dOmega(Double_t) const;
  Double_t          GetPathLength();
  void              GetXSectnKN();   // Get Klein Nishina  X-Section
  void              GetXSectnPE();   // Get Photo-electric X-Section
  void              IndexGeoVol();
  void              Init();
  void              InitP();
  void              InitTracking();
  void              InitTV();
  void              NameFile();
  Bool_t            NextCrossing();
  Bool_t            StartOneGamma();

public:

  TLitGamma()                              { InitP();                   }
  TLitGamma(const char*,const char*,const char*,Double_t=1.0,Bool_t=kFALSE,Bool_t=kFALSE,
    PDistribution=singlevalued,Double_t=1.0,Double_t=1.0,Double_t=0.1,Double_t=1000.0);
  virtual          ~TLitGamma();
  Bool_t            CheckDead() const      { return fGammaDead;         }
  Bool_t            ComptonCut() const     { return fComptonCut;        }
  void              ComptonCut(Bool_t b)   { fComptonCut = b;           }
  void              ComptonOk();
  void              Gen(Int_t,Int_t = 1,Double_t = -2.0e+20,Bool_t = kTRUE,Bool_t = kFALSE);  
  TVector3          GetAxisDirx() const    { return fAxisDirx;          }
  TVector3          GetAxisDiry() const    { return fAxisDiry;          }
  TVector3          GetAxisDirz() const    { return fAxisDirz;          }
  Int_t             GetCompton() const     { return fNCompton;          }
  TVector3          GetDirFace() const     { return fDirFace;           }
  Bool_t            GetForgetLast() const  { return fForgetLast;        }
  Bool_t            GetFromFace() const    { return fFromFace;          }
  Bool_t            GetMoving() const      { return fMoving;            }
  const char       *GetName() const        { return fName.Data();       }
  Int_t             GetRun() const         { return fRun;               }
  Bool_t            GetRunStat() const     { return fRunStat;           }
  Bool_t            GetSourceFixed() const { return fSourceFixed;       }
  const char       *GetSourcePath() const  { return fSourcePath.Data(); }
  TVector3          GetSourcePoint() const { return fSourcePoint;       }
  Double_t          GetThetaMax() const    { return fThetaMax;          }
  const char       *GetTitle() const       { return fTitle.Data();      }
  void              IsToDraw(Int_t);
  KindOfDist        KindOfEmission() const { return fKindOfEmission;    }
  void              MoveCradle(TGeoMatrix*,Bool_t=kTRUE);
  void              NameFromPath(TString&) const;
  Bool_t            SetEmission(KindOfDist=flat,Double_t=1.0,TVector3=0.0,const char* ="",
                      Bool_t=kFALSE,TVector3=0.0,Bool_t=kTRUE,TVector3=0.0);
  void              SetFillDeath(TH1F*, TH1F*, TH1F*);
  void              WhichToDraw(Int_t=1);
  ClassDef(TLitGamma,1)//Generation of gamma rays of energy less than ~1 Mev
};
#endif
