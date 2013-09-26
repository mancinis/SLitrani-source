// @(#)root/html:$Id: TLitVolume.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitVolume
#define LIT_TLitVolume
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitVolume    Addition to TGeoVolume needed by SLitrani              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TVector3.h"
#include "TH2.h"
#include "TMat33.h"
#include "TMat33C.h"
#include "TLitPhys.h"

enum ChoiceOfn  {israndom=0,biggest,smallest};

class TGeoVolume;
class TLitMedium;

class TLitVolume : public TNamed {

protected:

  TGeoVolume *fGeoVolume;       //! TGeoVolume associated with this TLitVolume at creation
  TLitMedium *fLitMedium;       //! TLitMedium assiciated with fGeoVolume
  Bool_t      fWCSdone;         //! True if calculation of points and vectors in WCS done
// Stepping characteristics
  Bool_t      fStepping;        //If true, stepping required in this medium, else jump to next volume
  Double_t    fStep;            //Step required in this medium. If <=0 jump to next volume
// Anisotropic characteristics
  Short_t     fAnisotropic;     //0 isotrop 1 neg bir 2 pos bir 3 birefringent
                                //-1 ==> optical property not yet defined !
                                // 0 ==> medium is optically isotropic
			                          // 1 ==> medium is "Uniaxial negative birefringent"
			                          //        ( epsa < epsb = epsc )
			                          // 2 ==> medium is "Uniaxial positiv birefringent"
			                          //        ( epsa = epsb < epsc )
			                          // 3 ==> medium is "biaxial birefringent" or "totally anisotropic"
			                          //        ( epsa < epsb < epsc )
  Short_t     fAniAbsL;         //0 ==> abs. L. isotropic 1 ==> La != Lb = Lc  2 ==> La = Lb != Lc  3 ==> la != Lb != Lc
  Bool_t      fDielChange;      //true if dielectric tensor depends upon wavelength.
  TVector3    fAniOx;           //Fixed x axis of dielectric tensor in LOCAL coordinates of associated TGeoVolume
  TVector3    fAniOy;           //Fixed y axis of dielectric tensor in LOCAL coordinates of associated TGeoVolume
  TVector3    fAniOz;           //Fixed z axis of dielectric tensor in LOCAL coordinates of associated TGeoVolume
  TVector3    fWCSAniOx;        //! Transient value in WCS of fAniOx, valid for the instance of this TGeoVolume where the current track is.
  TVector3    fWCSAniOy;        //! Transient value in WCS of fAniOy, valid for the instance of this TGeoVolume where the current track is.
  TVector3    fWCSAniOz;        //! Transient value in WCS of fAniOz, valid for the instance of this TGeoVolume where the current track is.
// Detector characteristics
  KindOfDet   fDetKind;         //{none,gendet,phototube,apd}
  Short_t     fEntrance;        //If (0,1,2,3,4,5) entrance face and fXYZEntrance is a point on this entrance face. If -1, no entrance face
  Double_t    fEx;              //1st semi-dimension of entrance window, if any
  Double_t    fEy;              //2nd semi-dimension of entrance window, if any
  Double_t    fEz;              //Semidepth behind entrance window, if any
  Bool_t      fStopAlways;      //false: photon only stops inside the TGeoVolume because of its finite absorption length.  true: photon reaching this TGeoVolume always stops in the TGeoVolume
  Bool_t      fIsQE;            //true if there is a quantum efficiency
  TString     fNameQE;          //name of the fit (TSplineFit) giving quantum efficiency as a function of wavelength
  Double_t    fQE;              //Value of fFitQE for a given wavelength
  TSplineFit *fFitQE;           //! pointer to this fit for volume detectors and APD only
  Bool_t      fIsAccept;        //true if there is a condition on entrance angle
  Double_t    fCosAccept;       //cosine of max acceptance angle
  TVector3    fDetAxis;         //axis with respect to which entrance angle of photon has to be calculated
  Double_t    fAPDThreshold;    //If nb. of e- emitted by APD > fAPDThreshold, photon considered as detected
  Double_t    fVDrifte;         //drift velocity of electrons inside APD in cm/s
  TString     fNameGP;          //name of the fit giving gain profile of the APD
  TSplineFit *fGP;              //! pointer to this fit
  TVector3    fXYZEntrance;     //Point on the entrance face if there is one. For an APD, the cathode
  TVector3    fWCSEntrance;     //! Transient value in WCS of fXYZEntrance
  Double_t    fDistCatAn;       //distance from cathode to anode in cm.
// Damages due to radiation. Variables for both methods
  Bool_t      fDamage;          //true if depth dependent damage
  TVector3    fDamAxis;         //axis of damage
  TVector3    fDamStart;        //starting point of damage
  TVector3    fWCSDamAxis;      //! Transient value in WCS of axis of damage
  TVector3    fWCSDamStart;     //! Transient value in WCS of starting point of damage
  Double_t    fDamKimax;        //Kimax 1/Li(z,r,l) at the point where Li(z,r,l) is minimal, i.e. where damages are maximal
  TSplineFit *fDamFitKimax;     //! Fit giving Kimax as a function of wavelength
  Double_t    fDamFmax;         //fmax: value of damage distribution f(z,r) where it is maximal
  Double_t    fDamStep;         //steps [cm] for calculating absorption
  Double_t    fDamSupFac;       //supplementary constant factor by which Ki(lambda) can be multiplied [default 1]
  TH2D       *fHDamage;         //histogram recording 1 - damage factor.
// Damage method of "RadiationDamage"
  Double_t    fDamZm;           //mean of gaussian  in z representing damages f(z,r)
  Double_t    fDamZs;           //sigma of gaussian in z representing damages f(z,r)
  Double_t    fDamRs;           //sigma of gaussian in r representing damages f(z,r)
  Double_t    fDamGz;           //value at a given depth of g(z): the z distribution provided by the user.
  Double_t    fDamGzMax;        //maximum value taken by g(z)
  TSplineFit *fDamFitGz;        //! fit giving g(z)
// Damage method of "ShowerDamage"
  Bool_t      fDamByShower;     //damages have been caused by showers. ShowerDamage() has been called, not RadiationDamage()
  Double_t    fDamE0;           //energy in Mev of the showers having caused damage
  Bool_t      fDamGamma;        //true if damages caused by gammas. If false, damages caused by electrons
  Double_t    fDamA;            //a of ShowerDamage
  Double_t    fRk2;             //K^2 of the r distribution
  Double_t    fDamFac;          //constant factor in front of dependent factor in f(z,r)
// Photon emission characteristics
  Bool_t      fEmitPhoton;      //True if photons can be emitted from the TGeoVolume associated with this TLitVolume
  KindOfDist  fKindOfEmission;  //Kind of distribution for the direction of emission of photon
  TString     fPath;            //! Transient path for the instance [node] of the TLitVolume actally in use
  Bool_t      fSourceFixed;     //If true, all photons are emitted from the fixed point source inside shape. Default false
  TVector3    fSourcePoint;     //Actual point of emission of photon in local coordinate of shape
  TVector3    fWCSSourcePoint;  //! fSourcePoint in WCS
  TVector3    fAxisDir;         //Axis around which photons are generated in local coordinates of the shape
  TVector3    fSourceDir;       //Actual direction of emission of photon in local coordinate of shape
  TVector3    fWCSSourceDir;    //! fSourceDir in WCS
  Double_t    fThetaMax;        //Maximum theta angle around fSourceDir in radian
  TSplineFit *fFitDist;         //TSplineFit to be used in case fKindOfEmission == provided
  Bool_t      fFromFace;        //If true, emission from face of TGeoVolume
  TVector3    fDirFace;         //Direction to take to reach face, in LCS
  TVector3    fWCSDirFace;      //! Idem fDirFace in WCS
// E field of photon at emission
  ChoiceOfn   fChooseIndex;     //Chooses between the 2 possible values of index in case of anisotropy
  TVector3    fE;               //! Generated direction for the E field in LCS of the TGeoVolume associated with this TLitVolume
  TVector3    fWCSE;            //! fE  in World Coordinate System
  Bool_t      fPolarized;       //True if photon linearly polarized at emission
  TVector3    fPolar;           //Direction of polarization for the E field in LCS of the TGeoVolume associated with this TLitVolume
  
  void        AddMe();
  void        Init();
  void        InitP();

public:

  TMat33      fM;               //! Transient value in WCS of Rot. matrix which transforms Ox(WCS) to Ox'(DTS). DTS = Dielectric tensor system
  TMat33      fMt;              //! Transient value in WCS of transposed of Rot. matrix which transforms  E(WCS) to  E'(DTS). DTS = Dielectric tensor system. fMt is the transpose matrix of fM.
  TMat33      fDTmu;            //! Transient value in WCS of dielectric tensor MULTIPLIED BY MAGNETIC PERMEABILITY
  TMat33C     fCDTmu;           //! Transient value in WCS of complex fDTmu to calculate complex index of wave

  static TObjArray *fgLitVolume;//List of all TLitVolume

  TLitVolume()                                { InitP();                }
  TLitVolume(TGeoVolume*);
  TLitVolume(TGeoVolume*,Double_t, Double_t, Double_t, Double_t);
  virtual       ~TLitVolume();
  void           ClearEmission();
  void           ClearWCSdone()               { fWCSdone  = kFALSE;     }
  Bool_t         EmissionToWCS();
  Bool_t         Gen(const char*);
  void           GenEField();
  Bool_t         GenSourceDir();
  Bool_t         GenSourcePoint();
  Short_t        GetAniAbsL() const           { return fAniAbsL;        }
  TVector3       GetAniOx() const             { return fAniOx;          }
  TVector3       GetAniOy() const             { return fAniOy;          }
  TVector3       GetAniOz() const             { return fAniOz;          }
  Short_t        GetAnisotropic() const       { return fAnisotropic;    }
  ChoiceOfn      GetChooseIndex() const       { return fChooseIndex;    }
  Double_t       GetCosAccept() const         { return fCosAccept;      }
  Double_t       GetDamage(const TVector3*,Double_t L0);
  Double_t       GetDamA() const              { return fDamA;           }
  TVector3       GetDamAxis() const           { return fDamAxis;        }
  Bool_t         GetDamByShower() const       { return fDamByShower;    }
  Double_t       GetDamE0() const             { return fDamE0;          }
  Double_t       GetDamFac() const            { return fDamFac;         }
  TSplineFit    *GetDamFitGz() const          { return fDamFitGz;       }
  TSplineFit    *GetDamFitKimax() const       { return fDamFitKimax;    }
  Double_t       GetDamFmax() const           { return fDamFmax;        }
  Bool_t         GetDamGamma() const          { return fDamGamma;       }
  Double_t       GetDamGz() const             { return fDamGz;          }
  Double_t       GetDamGzMax() const          { return fDamGzMax;       }
  Double_t       GetDamKimax() const          { return fDamKimax;       }
  TVector3       GetDamStart() const          { return fDamStart;       }
  Double_t       GetDamStep() const           { return fDamStep;        }
  Double_t       GetDamSupFac() const         { return fDamSupFac;      }
  Double_t       GetDamZm() const             { return fDamZm;          }
  Double_t       GetDamZs() const             { return fDamZs;          }
  TVector3       GetDetAxis() const           { return fDetAxis;        }
  KindOfDet      GetDetKind() const           { return fDetKind;        }
  Bool_t         GetDielChange() const        { return fDielChange;     }
  Double_t       GetDistCatAn() const         { return fDistCatAn;      }
  void           GetEmission(TVector3&,TVector3&,TVector3&) const;
  Bool_t         GetEmitPhoton() const        { return fEmitPhoton;     }
  Short_t        GetEntrance() const          { return fEntrance;       }
  Double_t       GetEx() const                { return fEx;             }
  Double_t       GetEy() const                { return fEy;             }
  Double_t       GetEz() const                { return fEz;             }
  TSplineFit    *GetFitQE() const             { return fFitQE;          }
  Bool_t         GetFromFace() const          { return fFromFace;       }
  TSplineFit    *GetGainProfile() const       { return fGP;             }
  TGeoVolume    *GetGeoVolume() const         { return fGeoVolume;      }
  Bool_t         GetIsQE() const              { return fIsQE;           }
  KindOfDist     GetKindOfEmission() const    { return fKindOfEmission; }
  TLitMedium    *GetLitMedium() const         { return fLitMedium;      }
  const TString &GetNameGP() const            { return fNameGP;         }
  const TString &GetNameQE() const            { return fNameQE;         }
  const TString &GetPath() const              { return fPath;           }
  Double_t       GetQE() const                { return fQE;             }
  Double_t       GetRk2() const               { return fRk2;            }
  TVector3       GetSourceDir() const         { return fSourceDir;      }
  TVector3       GetSourcePoint() const       { return fSourcePoint;    }
  Double_t       GetStep() const              { return fStep;           }
  Bool_t         GetStopAlways() const        { return fStopAlways;     }
  Double_t       GetThetaMax() const          { return fThetaMax;       }
  Double_t       GetVDrifte() const           { return fVDrifte;        }
  Bool_t         GetWCSdone() const           { return fWCSdone;        }
  TVector3       GetWCSE() const              { return fWCSE;           }
  TVector3       GetWCSEntrance() const       { return fWCSEntrance;    }
  TVector3       GetWCSSourceDir() const      { return fWCSSourceDir;   }
  TVector3       GetWCSSourcePoint() const    { return fWCSSourcePoint; }
  TVector3       GetXYZEntrance() const       { return fXYZEntrance;    }
  Bool_t         IsAccept() const             { return fIsAccept;       }
  Bool_t         IsDamage() const             { return fDamage;         }
  Bool_t         IsStepping() const           { return fStepping;       }
  void           NewWavelength(Double_t);
  static void    NewWavelengthAll(Double_t,Bool_t = kFALSE);
  Bool_t         RadiationDamage(const TVector3*,const TVector3*,Double_t,Double_t,Double_t,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t         RadiationDamage(const TVector3*,const TVector3*,const char*,Double_t,Double_t,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t         RadiationDamage(const TVector3*,const TVector3*,Double_t,const char *,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t         RadiationDamage(const TVector3*,const TVector3*,const char*,const char *,Double_t,Double_t = 0.4,Double_t = 1.0);
  void           RecordDamages(Int_t,Double_t,Int_t,Double_t);
  void           SetAPD(const char*,Short_t,Double_t=-1.0,Double_t=1.0e+8);
  void           SetAxisDir(Double_t=0.0,Double_t=0.0,Double_t=1.0);
  void           SetDetector(Bool_t, const char*, Double_t, Double_t = 0.0, Double_t = 0.0, Double_t = 1.0);
  void           SetDielTensor();
  Bool_t         SetEField(Bool_t=kFALSE,Double_t=90.0,Double_t=0.0,ChoiceOfn = israndom);
  Bool_t         SetEmission(KindOfDist=on4pi,Double_t=180.0,TVector3=0.0,
                   const char* = "",Bool_t=kFALSE,TVector3=0.0,Bool_t=kFALSE,TVector3=0.0);
  void           SetGeoVolume(TGeoVolume *v)     { fGeoVolume = v;      }
  void           SetKindOfEmission(KindOfDist k) { fKindOfEmission = k; }
  void           SetM();
  void           SetPath(const char *t)          { fPath = t;           }
  void           SetNoDamage(Bool_t = kFALSE);
  void           SetOpticalAxis(Double_t, Double_t, Double_t, Double_t);
  void           SetPhotoTube(const char*,Double_t dir,Double_t = -1.0);
  Bool_t         SetSourcePoint(Bool_t,TVector3 = 0.0);
  Double_t       SetStep(Double_t);
  void           SetWCS();
  void           ShowDamage() const;
  Bool_t         ShowerDamage(const TVector3*,const TVector3*,Double_t,Double_t,Bool_t=kTRUE,Double_t=0.4,Double_t = 1.0);
  Bool_t         ShowerDamage(const TVector3*,const TVector3*,const char*,Double_t,Bool_t=kTRUE,Double_t=0.4,Double_t = 1.0);
  ClassDef(TLitVolume,1)  //Addition to TGeoVolume needed by SLitrani
};
#endif
