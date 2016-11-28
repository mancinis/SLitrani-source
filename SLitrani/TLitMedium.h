// @(#)root/html:$Id: TLitMedium.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitMedium
#define LIT_TLitMedium

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitMedium  TGeoMedium for SLitrani                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TComplex.h"
#include "TGeoMedium.h"
#include "TLitPhys.h"

class TSplineFit;
class TLitSpectrum;
class TLitSellmeier;

enum KindOfMedium {OpticMedium,OpticMediumWithTS,OpticWrapping};
enum MethodUsed {undefined,constant,byfit,sellmeier};
enum dEdxMethod {nodEdx,WithoutLandau,WithLandau};

class TLitMedium : public TGeoMedium {

protected:

  static Double_t fgMinimalGap;     //minimal difference between epsa,epsb,epsc

  KindOfMedium    fKind;            //medium is for normal tracking medium, or for wrapping or for thin slice.
  Int_t           fMatCounter;      //counter of materials
  Int_t           fRevCounter;      //counter of wrappings
  Bool_t          fChecked;         //medium is correctly defined (method CheckIt has been called)
  Bool_t          fSensible;        //true if the medium is able to detect photons

  //Absorption lengths

  Short_t         fAniAbsL;         //0 ==> abs. L. isotropic   1 ==> La != Lb  = Lc   2 ==> La  = Lb != Lc   3 ==> la != Lb != Lc
  Double_t        fFacAbsL;         //Factor by which all absorption length are multiplied after having been obtained from fit (default=1.0)
  Double_t        fAbsLa;           //value of absorption length associated with fEpsa or isotropic absorption length when fAniAbsL = 0
  Double_t        fPhimua;          //imaginary part of fEpsa multiplied by fMu
  TString         fNameAbsLa;       //name of the fit giving fAbsLa as a function of wavelength
  TSplineFit     *fFitAbsLa;        //pointer to the fit for fAbsLa. Not used in case OpticWrapping
  Double_t        fAbsLb;           //value of absorption length associated with fEpsb
  Double_t        fPhimub;          //imaginary part of fEpsb multiplied by fMu
  TString         fNameAbsLb;       //name of the fit giving fAbsLb as a function of wavelength
  TSplineFit     *fFitAbsLb;        //pointer to the fit for fAbsLb
  Double_t        fAbsLc;           //value of absorption length associated with fEpsc
  Double_t        fPhimuc;          //imaginary part of fEpsc multiplied by fMu
  TString         fNameAbsLc;       //name of the fit giving fAbsLc as a function of wavelength
  TSplineFit     *fFitAbsLc;        //pointer to the fit for fAbsLc
  Bool_t          fHasAbsorption;   //True if material has absorption

  //Diffusion lengths

  Bool_t          fHasDiffusion;    //True if material has istropic scattering defined
  Bool_t          fHasRayleigh;     //True if material has Rayleigh scattering defined
  Double_t        fFacDiffus;       //if >0, factor which gives Diffusion length when multiplied by absorption length
  Double_t        fDiffusLength;    //Actual value of diffusion length
  TString         fNameDiffusL;     //name of the fit giving fDiffusLength as a function of wavelength
  TSplineFit     *fFitDiffusL;      //pointer to the fit giving fDiffusLength as a function of wavelength
  Double_t        fRayleighLength;  //Actual value of Rayleigh scattering length
  TString         fNameRayleighL;   //name of the fit giving fRayleighLength as a function of wavelength
  TSplineFit     *fFitRayleighL;    //pointer to the fit giving fRayleighLength as a function of wavelength

  //Index of refraction

  Short_t         fAnisotropic;     //0 isotrop 1 neg bir 2 pos bir 3 birefringent
                                    //-1 ==> optical property not yet defined !
                                    // 0 ==> medium is optically isotropic
			                              // 1 ==> medium is "Uniaxial negative birefringent"
			                              //        ( epsa < epsb = epsc )
			                              // 2 ==> medium is "Uniaxial positiv birefringent"
			                              //        ( epsa = epsb < epsc )
			                              // 3 ==> medium is "biaxial birefringent" or "totally anisotropic"
			                              //        ( epsa < epsb < epsc )
  Double_t        fIndex;           //OpticMedium ==> Index of refraction of medium.  OpticWrapping ==> Index of refraction of slice of material between wrapping and wrapped,  OpticThinSlice ==> index of refraction of thin slice
  MethodUsed      fIndexM;          //method used for calculating the index of refraction
  TLitSellmeier  *fIndexS;          //pointer to Sellmeier formula for index
  TString         fNameIndex;       //name of the fit giving the index of refraction as a function of wavelength
  TSplineFit     *fFitIndex;        //pointer to the fit for index of refraction as a function of wavelength

  //Magnetic permeability

  Double_t        fMu;              //Magnetic permeability mu of medium if not wrapping, of slice if wrapping
  TString         fNameMu;          //name of the fit giving the magnetic permeability mu as a function of wavelength
  TSplineFit     *fFitMu;           //pointer to the fit for magnetic permeability mu as a function of wavelength
  Bool_t          fMuFound;         //Fit for Mu or value for mu found;

  //Epsa : 1st diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsa;            //Epsa : 1st diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpsaM;           //method used for calculating fEpsa
  TLitSellmeier  *fEpsaS;           //pointer to Sellmeier formula for Epsa
  TString         fNameEpsa;        //name of the fit giving the 1st diagonal element Epsa as a function of wavelength
  TSplineFit     *fFitEpsa;         //Fit for fEpsa as a function of wavelength

  //Epsb : 2nd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsb;            //Epsb : 2nd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpsbM;           //method used for calculating fEpsb
  TLitSellmeier  *fEpsbS;           //pointer to Sellmeier formula for Epsb
  TString         fNameEpsb;        //name of the fit giving the 2nd diagonal element Epsb as a function of wavelength
  TSplineFit     *fFitEpsb;         //Fit for fEpsb as a function of wavelength

  //Epsc : 3rd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsc;            //Epsc : 3rd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpscM;           //method used for calculating fEpsc
  TLitSellmeier  *fEpscS;           //pointer to Sellmeier formula for Epsc
  TString         fNameEpsc;        //name of the fit giving the 3rd diagonal element Epsc as a function of wavelength
  TSplineFit     *fFitEpsc;         //Fit for fEpsc as a function of wavelength

//dE/dx

  dEdxMethod      fdEdxUsed;        //this medium is optically activ when crossed by particle
  TString         fNamedEdx;        //name of the family of fits containing all distributions
  Int_t           fNbdEdx;          //Nb. of elements in the family fDeposi, if fDeposit is a family
  TSplineFit     *fDeposit;         //Fit giving deposited energy in 1 cm versus energy of particle when no Landau fluctuations
  Double_t        fPhotMev;         //Number of photons emitted per deposited Mev of energy
  Double_t        fResolutionScale; //Scaling of resolution on fPhotMev to allow for effects due to the Fano factor and intrinsic broadening of resolution due to non-homogeneity.

//Fluorescence

  TLitSpectrum   *fLitSpectrum;     //Emission spectrum [wavelength and times] of this material
	Bool_t          fHistFluo;        //histogrammation required for fluorescence of this medium
 
//Electromagnetic cascades

  Double_t        fRm;              //Moliere's radius in cm
  Double_t        fEc;              //Critical energy  in Mev

//Non-proportionality of light yield

  TSplineFit     *fFitNPLY;         //SplineFit for the data on the non-proportionality of the light yield. Nb. of photons as a function of deposited energy
  Bool_t          fHasNPLY;         //Is the material defined as being non-proportional
  TString         fNameNPLY;        //Name of the fit

//Photo Electric effect

  TSplineFit     *fFitPE;           //Fit giving photo-electric cross section for a photon of energy MeV
  Bool_t          fPEOk;            //Has the PE been initialised
  TString         fNamePE;          //Name of the PE fit

//Wavelength shifting

  Bool_t          fHasWLS;          //True if material is wavelength shifting
  TLitSpectrum   *fWLSSpectrum;     //Wavelength shifted spectrum
  Double_t        fWLSL;            //WLS Absorption length
  TString         fNameWLSL;        //name of the fit fFitWLSL
  TSplineFit     *fFitWLSL;         //Fit for absorption length for wavelength shifting as a function of wavelength
  Bool_t          fCanStopElectron; //Compton electron properties
  Double_t        fEP[6];           //Material properties necessary to calculate electron ionisation dE/dX

//Variables if wrapping

  Bool_t          fWrapIsSlice;     //True if there is a slice of material between the face of the shape and the wrapping
  Double_t        fWrapDiffus;      //Proportion of diffusion when fixed (not varying with wavelength)
  TString         fWrapNameDiffus;  //Name of the fit giving the proportion of light diffused as a function of wavelength
  TSplineFit     *fWrapFitDiffus;   //Fit giving the proportion of light diffused for wrapping as a function of wavelength
  Double_t        fWrapRindex;      //Real part of index of refraction when fixed (not varying with wavelength)
  TString         fWrapNameRindex;  //Name of the fit giving the real part of the index of refraction of the wrapping as a function of wavelength
  TSplineFit     *fWrapFitRindex;   //Fit giving the real part of index of refraction of wrapping as a function of wavelength
  Double_t        fWrapIindex;      //Imaginary part of index of refraction when fixed (not varying with wavelength)
  TString         fWrapNameIindex;  //Name of the fit giving the imaginary part of the index of refraction of the wrapping as a function of wavelength
  TSplineFit     *fWrapFitIindex;   //Fit giving the imaginary part of index of refraction of wrapping as a function of wavelength
  Double_t        fWrapMu;          //Magnetic permeability mu when fixed (not varying with wavelength)
  TString         fWrapNameMu;      //Name of the fit giving the magnetic permeability mu of the wrapping as a function of wavelength
  TSplineFit     *fWrapFitMu;       //Fit giving the magnetic permeability mu of wrapping as a function of wavelength
  Short_t         fWrapAbsorbing;   // o : no supplementary absorption.  1 : some suppl. abs.   2 : wrapping 100% absorbing
  Double_t        fWrapSupplAbs;    //[0..1] supplementary absorption, (f.e. : dust). Default 0
  Double_t        fWrapSinThetaMax; //sinus of the 9th argument of the constructors.

//Variables for thin slice

  Bool_t          fTSThinSlice;     //True if possible thin slice
  TLitMedium     *fTSNextMed;       //TLitMedium after the thin slice. If an other one, no thin slice!
  Double_t        fTSWidth;         //True width of thin slice in cm
  Double_t        fTSIndex;         //Fixed index of refraction of thin slice if fTSFitIndex == 0
  TString         fTSNameIndex;     //Name of fit of type TSplineFit giving index of refraction of thin slice as a function of wavelength
  TSplineFit     *fTSFitIndex;      //Fit giving index of refraction of thin slice as a function of wavelength
  Double_t        fTSMu;            //Fixed magnetic permeability of thin slice if fTSFitMu == 0
  TString         fTSNameMu;        //Name of fit of type TSplineFit giving magnetic permeability mu of thin slice as a function of wavelength
  TSplineFit     *fTSFitMu;         //Fit giving magnetic permeability mu of thin slice as a function of wavelength

//Variables for depolish

  Bool_t          fDPLIsDepol;      //True if possible depolish with fDPLNextMed
  TLitMedium     *fDPLNextMed;      //In contact with fDPLNextMed, this generates depolish
  Double_t        fDPLTheta;        //Angle of depolish [0-90](roughness of surface)
  Double_t        fDPLCosTheta;     //Cos(fDPLTheta)


  void  AddMe();                        //Add this medium to the list of media
  void  FindIindex(const TString&);     //Find imaginary part of index of refraction of wrapping
  void  FindRindex(const TString&);     //Find real part of index of refraction of wrapping
  void  FindWrapDiffus(const TString&); //Find % of diffusion for wrapping.
  void  FindWrapMu(const TString&);     //Find fit for magnetic permeability mu of wrapping.
  void  Init();                         //basic initialization
  void  InitP();                        //Pointers to 0
  void  InitWrapping();                 //initializes variables for wrappings
  void  MoreAbsorption(Double_t);       //Defines supplementary absorption, if any.
  void  NewPhiMu(Double_t);             //calculates the 3 fPhiMua/b/c
  void  PrintProblem() const;           //Header for signaling a problem
  void  PrintName() const;              //prints the name of the medium
  void  ProblemHeader() const;          //header before reporting a problem

public:

  static Double_t   fgOldWavelength;  //old value of wavelength
  static Double_t   fgMinWaveL;       //minimal value for a wavelength
  static Double_t   fgMaxWaveL;       //maximal value for a wavelength
  static Double_t   fgCerWaveL;       //wavelength to use for n used in Cerenkov formulae
  static TObjArray *fgLitMedium;      //list of all TLitMedium

//Default constructor
  TLitMedium()                            { InitP(); AddMe(); }
//Constructors for tracking media, with pointer towards material or mixture
  TLitMedium(const char*, Int_t, const TGeoMaterial*, Bool_t, const char*, const char*,
             const char* = "",const char* = "",Double_t* =0);
  TLitMedium(const char*, Int_t, const TGeoMaterial*, Bool_t, const char*, Double_t,
             Double_t = -1,Double_t = -1,Double_t* =0);
  TLitMedium(const char*, Int_t, const TGeoMaterial*, Bool_t, Double_t, const char*,
             const char* = "",const char* = "",Double_t* =0);
  TLitMedium(const char*, Int_t, const TGeoMaterial*, Bool_t, Double_t, Double_t,
             Double_t = -1,Double_t = -1,Double_t* =0);
//Constructors for tracking media, with material or mixture number
  TLitMedium(const char*, Int_t, Int_t, Bool_t, const char*, const char*,
             const char*,const char*, Int_t, Int_t, Double_t, Double_t, Double_t, Double_t,
             Double_t, Double_t);
  TLitMedium(const char*, Int_t, Int_t, Bool_t, const char*, Double_t,
             Double_t,Double_t, Int_t, Int_t, Double_t, Double_t, Double_t, Double_t,
             Double_t, Double_t);
  TLitMedium(const char*, Int_t, Int_t, Bool_t, Double_t, const char*,
             const char*,const char*, Int_t, Int_t, Double_t, Double_t, Double_t, Double_t,
             Double_t, Double_t);
  TLitMedium(const char*, Int_t, Int_t, Bool_t, Double_t, Double_t,
             Double_t,Double_t, Int_t, Int_t, Double_t, Double_t, Double_t, Double_t,
             Double_t, Double_t);
//Constructors for wrappings
  TLitMedium(const char*, Int_t, const TGeoMaterial*, const char* = "", Double_t* =0);
  TLitMedium(const char*, Int_t, const TGeoMaterial*, Double_t, Double_t* =0);
//Destructor
  ~TLitMedium();
  Bool_t        CheckIt();
  void          dEdx(const char*,Double_t,Bool_t = kTRUE);
  dEdxMethod    dEdxActiv() const            { return fdEdxUsed;        }
  Double_t      Diffus() const               { return fWrapDiffus;      }
  void          DoWLS(Double_t &,Double_t &,Int_t &);
  static TLitMedium *FindLitMedium(const char *);
  void          FindSpectrum(const char*);
  void          FluoComponent(Double_t,Double_t,Double_t,Double_t);
  Double_t      GetAbsL() const              { return fAbsLa;           }
  Double_t      GetAbsLb() const             { return fAbsLb;           }
  Double_t      GetAbsLc() const             { return fAbsLc;           }
  Short_t       GetAniAbsL() const           { return fAniAbsL;         }
  Short_t       GetAnisotropy() const        { return fAnisotropic;     }
  Bool_t        GetCanStopElectron() const   { return fCanStopElectron; }
  Double_t      GetdEdx(Double_t) const;
  Double_t      GetDiffLength() const        { return fDiffusLength;    }
  Double_t      GetDPLCosTheta() const       { return fDPLCosTheta;     }
  Bool_t        GetDPLIsDepol() const        { return fDPLIsDepol;      }
  TLitMedium   *GetDPLNextMed() const        { return fDPLNextMed;      }
  Double_t      GetDPLTheta() const          { return fDPLTheta;        }
  Double_t      GetEc() const                { return fEc;              }
  Double_t     *GetElectronProperties()      { return &(fEP[0]);        }
  Double_t      GetEpsa() const              { return fEpsa;            }
  Double_t      GetEpsb() const              { return fEpsb;            }
  Double_t      GetEpsc() const              { return fEpsc;            }
  Short_t       GetFluorescent() const;
  Bool_t        GetHasAbsorption() const     { return fHasAbsorption;   }
  Bool_t        GetHasDiffusion() const      { return fHasDiffusion;    }
  Bool_t        GetHasRayleigh() const       { return fHasRayleigh;     }
  Bool_t        GetHasWLS() const            { return fHasWLS;          }
  Bool_t        GetHistFluo() const          { return fHistFluo;        }
  Bool_t        GetIChangeWithWvl() const;
  Double_t      GetIndex() const             { return fIndex;           }
  KindOfMedium  GetKind() const              { return fKind;            }
  TLitSpectrum *GetLitSpectrum() const       { return fLitSpectrum;     }
  Int_t         GetMatCounter() const        { return fMatCounter;      }
  Double_t      GetMatCounterD() const       { return fMatCounter-0.5;  }
  Double_t      GetMu() const                { return fMu;              }
  Double_t      GetnCerenkov() const;
  Int_t         GetNPhot(Double_t Edep) const;
  Bool_t        GetPEOk() const              { return fPEOk;            }
  Double_t      GetPhimua() const            { return fPhimua;          }
  Double_t      GetPhimub() const            { return fPhimub;          }
  Double_t      GetPhimuc() const            { return fPhimuc;          }
  Double_t      GetPhotMev() const;
  Double_t      GetRayleighLength() const    { return fRayleighLength;  }
  Int_t         GetRevCounter() const        { return fRevCounter;      }
  Double_t      GetRevCounterD() const       { return fRevCounter-0.5;  }
  Double_t      GetRm() const                { return fRm;              }
	Double_t      GetSinThetaMax() const       { return fWrapSinThetaMax; }
  Double_t      GetSupplAbs() const          { return fWrapSupplAbs;    }
  TSplineFit   *GetTSFitIndex() const        { return fTSFitIndex;      }
  TSplineFit   *GetTSFitMu() const           { return fTSFitMu;         }
  Double_t      GetTSIndex() const           { return fTSIndex;         }
  Double_t      GetTSMu() const              { return fTSMu;            }
  TString&      GetTSNameIndex()             { return fTSNameIndex;     }
  TString&      GetTSNameMu()                { return fTSNameMu;        }
  TLitMedium   *GetTSNextMed() const         { return fTSNextMed;       }
  Bool_t        GetTSThinSlice() const       { return fTSThinSlice;     }
  Double_t      GetTSWidth() const           { return fTSWidth;         }
  Double_t      GetWLSL() const              { return fWLSL;            }
  TComplex      GetWrapIndex() const         { TComplex n(fWrapRindex,fWrapIindex); return n; }
  Double_t      GetWrapMu() const            { return fWrapMu;          }
  Double_t      GetX0() const                { return GetMaterial()->GetRadLen(); }
  Double_t      GetXSectnPE(Double_t MeV) const;
  Bool_t        GoodForCPE() const;
  void          IsBirefr(const char*,const char*,const char*);
  void          IsBirefr(Double_t,Double_t,Double_t);
  void          IsBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void          IsBirefr(TLitSellmeier* const,TLitSellmeier* const,TLitSellmeier* const);
  Bool_t        IsDepolish(const TLitMedium *) const;
  Bool_t        IsFluorescent() const        { return (GetFluorescent()>0); }
  void          IsIsotropic(const char*);
  void          IsIsotropic(Double_t);
  void          IsIsotropic(Int_t,Double_t,Double_t,Double_t,Double_t=0.0,Double_t=0.0,Double_t=0.0,Double_t=0.0);
  void          IsIsotropic(TLitSellmeier* const);
  Bool_t        IsNonProportional() const    { return fHasNPLY;         }
  Bool_t        IsSensible() const           { return fSensible;        }
  Bool_t        IsSlice() const              { return fWrapIsSlice;     }
  Short_t       IsSupplAbs() const           { return fWrapAbsorbing;   }
  Bool_t        IsThinSlice(const TLitMedium *) const;
  void          IsUniAxialNegBirefr(const char*,const char*);
  void          IsUniAxialNegBirefr(Double_t,Double_t);
  void          IsUniAxialNegBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void          IsUniAxialNegBirefr(TLitSellmeier* const,TLitSellmeier* const);
  void          IsUniAxialPosBirefr(const char*,const char*);
  void          IsUniAxialPosBirefr(Double_t,Double_t);
  void          IsUniAxialPosBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void          IsUniAxialPosBirefr(TLitSellmeier* const,TLitSellmeier* const);
  Bool_t        IsThinSlice(TLitMedium*);
  Bool_t        IsWrapping() const           { return fKind==OpticWrapping; }
  void          NewWavelength(Double_t);
  static void   NewWavelengthAll(Double_t,Bool_t = kFALSE);
  void          Print();
  void          SetAbsLa(Double_t,Bool_t = kFALSE);
  void          SetAbsLa(const char *,Bool_t = kFALSE);
  void          SetAbsLb(Double_t,Bool_t = kFALSE);
  void          SetAbsLb(const char *,Bool_t = kFALSE);
  void          SetAbsLc(Double_t,Bool_t = kFALSE);
  void          SetAbsLc(const char *,Bool_t = kFALSE);
  void          SetAsWrapping(const char*, const char*, const char*, const char*,Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(const char*, const char*, const char*, Double_t=1.0,Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(const char*, Double_t,Double_t,const char*,     Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(const char*, Double_t,Double_t,Double_t=1.0,Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(Double_t,const char*, const char*, const char*,     Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(Double_t,const char*, const char*, Double_t=1.0,Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(Double_t,Double_t,Double_t,const char*,     Double_t=0.0,Double_t=90.0);
  void          SetAsWrapping(Double_t,Double_t,Double_t,Double_t=1.0,Double_t=0.0,Double_t=90.0);
  void          SetCanStopElectron(Double_t, Double_t, Double_t,Double_t, Double_t, Double_t);
  void          SetCascades(Double_t, Double_t = -1.0);
  void          SetCascades(Double_t, Double_t, Double_t);
  void          SetDepolished(TLitMedium*,Double_t ThetaDepls = 60.0);
  void          SetDiffusionLength(Double_t,Bool_t=kTRUE);
  void          SetDiffusionLength(const char* ="");
  void          SetEc(Double_t ec)           { fEc = ec;           }
  void          SetFactorOnAbsL(Double_t);
  void          SetHistFluo(Bool_t);
  void          SetLitSpectrum(TLitSpectrum *s) { fLitSpectrum = s;}
  void          SetKind(KindOfMedium kind)      { fKind = kind;    }
  void          SetNonPropLightYield(const char* ="");
  void          SetPhotMev(Double_t photmev, Double_t resScale = 1.0);
  void          SetRayleighLength(Double_t,Bool_t = kFALSE);
  void          SetRayleighLength(const char*);
  void          SetRm(Double_t rm)           { fRm = rm;           }
  void          SetWLS(TLitSpectrum*, Double_t wlsl);
  void          SetWLS(TLitSpectrum*, const char* wlslFile);
  void          SetWrapIndex(Double_t,Double_t);
  void          SetX0(Double_t x0)           { GetMaterial()->SetRadLen(x0); }
  void          SetXSectnPE(const char* name);
  void          SetXSectnPE(TSplineFit*);
  void          WaveAndLife(Double_t&,Double_t&,Int_t&);
  void          WithThinSlice(TLitMedium*,Double_t,const char*,const char*);
  void          WithThinSlice(TLitMedium*,Double_t,const char*,Double_t = 1.0);
  void          WithThinSlice(TLitMedium*,Double_t,Double_t,const char*);
  void          WithThinSlice(TLitMedium*,Double_t,Double_t,Double_t = 1.0);
  ClassDef(TLitMedium,1)  //Add optical properties to a TGeoMedium
};
#endif
