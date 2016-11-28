// @(#)root/html:$Id: TLitAlbedo.h 2009-06-16
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*********************************************************************************
 * Copyright The same as the ones of ROOT                                        *
*********************************************************************************/
#ifndef LIT_TLitAlbedo
#define LIT_TLitAlbedo
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// TLitAlbedo  Complex indes of wrapping from albedo                            //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "TVector3.h"

class TGeoMaterial;
class TGeoMedium;
class TGeoTranslation;
class TGeoVolume;
class TSplineFit;
class TLitMedium;
class TLitVolume;

class TLitAlbedo : public TNamed {

protected:

  TSplineFit            *fAlbedo;       //Albedo as a function of wavelength
  TSplineFit            *fImPart;       //Arbitrary chosen imaginay part of index as a function of wavelength
  Double_t               fWrapDiffus;   //Proportion of diffusion versus reflection for the wrapping. Default 1.0 [100%]
  TGeoMaterial          *fSrcMat;       //! Material of SOURCE
  TLitMedium            *fSrcMed;       //! Medium of SOURCE
  TGeoMaterial          *fDetMat;       //! Material of DETECTOR
  TLitMedium            *fDetMed;       //! Medium of DETECTOR
  TGeoMaterial          *fWrapMat;      //! Material of WRAPPING
  TLitMedium            *fWrapMed;      //! Medium of WRAPPING
  TGeoMaterial          *fVacuumMat;    //! Material of TOP
  TGeoMedium            *fVacuum;       //! Medium of TOP
  //
  Double_t               fL;            //Half size of TGeoBBox WRAPPING. TGeoBBox TOP has half size fL+fD
  Double_t               fD;            //TGeoBBox DETECTOR has half sizes [fL-0.5*fD,fL,fL]
  TGeoTranslation       *fT1;           //! Translation of DETECTOR with respect to WRAPPING
  TGeoTranslation       *fT2;           //! Translation of SOURCE with respect to DETECTOR
  TGeoVolume            *fTop;          //! TGeoVolume TOP
  TGeoVolume            *fWrapping;     //! TGeoVolume WRAPPING
  TGeoVolume            *fDetector;     //! TGeoVolume DETECTOR
  TLitVolume            *fLitDet;       //! Associated TLitVolume
  TGeoVolume            *fSource;       //! TGeoVolume SOURCE
  TLitVolume            *fLitSource;    //! Associated TLitVolume
  Double_t               fDistPoint;    //Distance of point of emission of photon from center of SOURCE [negativ!]
  Double_t               fStartAngle;   //Starting angle of emission in the (Ox,Oy) plane [radian]
  Double_t               fLastAngle;    //Last angle of emission in the (Ox,Oy) plane [radian]
  Double_t               fStepAngle;    //Step for angle of emission in the (Ox,Oy) plane [radian]
  Int_t                  fNAngles;      //Number of steps to be done
  Double_t               fAperture;     //Aperture of the beam of photons in degree [default 5.0]
  Double_t               fEmitAngle;    //! Angle of emission in the (Ox,Oy) plane [radian]
  TVector3               fSourcePoint;  //Point emitting photons inside fSource
  TVector3               fSourceDir;    //! Axis around which photons are emitted inside fSource
  Int_t                  fNWaveLength;  //Number of wavelengths used
  Double_t              *fWaveLength;   //[fNWaveLength] List of wavelength used
  Double_t               fMaxReal;      //Maximum possible value for the real part of the index of studied wrapping
  Int_t                  fNbStepsReal;  //Number of steps in looking for the right value of real index
  //
  Short_t                fSize;         //Size of main canvas [ 1 <= fSize <= 8 ]
  Int_t                  fNbButtons;    //Number of buttons wanted in TwoPadDisplay. Bigger than 10 [default 24]
  Int_t                  fXminC;        //x coordinate, in pixel, of top left corner of main canvas [default 2]
  Int_t                  fYminC;        //y coordinate, in pixel, of top left corner of main canvas [default 2]
  Bool_t                 fWithDate;     //If true, date and time are appended to the name of the ROOT file
  TString                fNameFile;     //Name of the .root file that SLitrani produces
  TString                fListFile;     //Name of the .lis  file that SLitrani produces
  //
  TString                fUpcom;        //Comment at top right of main canvas
  TString                fDowncom;      //Comment at bottom right of main canvas

  void                   ClearP();
  void                   Init();
  void                   InitP();

public:

  TLitAlbedo()                                    { InitP(); }
  TLitAlbedo(const char*,const char*,const char* = "Aluminium");
  virtual    ~TLitAlbedo();
  void        BuildGeometry();
  void        Gen();
  Double_t    GetAperture() const        { return fAperture;        }
  Double_t    GetD() const               { return fD;               }
  Double_t    GetDistPoint() const       { return fDistPoint;       }
  const char *GetDowncom() const         { return fDowncom.Data();  }
  Double_t    GetL() const               { return fL;               }
  const char *GetListFile() const        { return fListFile.Data(); }
  Double_t    GetMaxReal() const         { return fMaxReal;         }
  const char *GetNameFile() const        { return fNameFile.Data(); }
  Int_t       GetNAngles() const         { return fNAngles;         }
  Short_t     GetNbButtons() const       { return fNbButtons;       }
  Int_t       GetNbStepsReal() const     { return fNbStepsReal;     }
  Int_t       GetNWaveLength() const     { return fNWaveLength;     }
  Short_t     GetSize() const            { return fSize;            }
  Double_t    GetStartAngle() const;
  Double_t    GetStepAngle() const       { return fStepAngle;       }
  const char *GetUpcom() const           { return fUpcom.Data();    }
  Double_t    GetWavelength(Int_t k) const;
  Bool_t      GetWithDate() const        { return fWithDate;        }
  Double_t    GetWrapDiffus() const      { return fWrapDiffus;      }
  Int_t       GetXminC() const           { return fXminC;           }
  Int_t       GetYminC() const           { return fYminC;           }
  void        Go();
  void        PrintAngles() const;
  void        SetAperture(Double_t a)    { fAperture=a;             }
  void        SetD(Double_t d)           { fD = d;                  }
  void        SetDefWaveL();
  void        SetDistPoint(Double_t d);
  void        SetDowncom(const char *s)  { fDowncom = s;            }
  void        SetL(Double_t d)           { fL = d;                  }
  void        SetListFile(const char *n) { fListFile = n;           }
  void        SetMaxReal(Double_t d)     { fMaxReal = d;            }
  void        SetNameFile(const char *n) { fNameFile = n;           }
  void        SetNAngles(Int_t);
  void        SetNbButtons(Int_t N)      { fNbButtons = N;          }
  void        SetNbStepsReal(Int_t n)    { fNbStepsReal = n;        }
  void        SetSize(Short_t s)         { fSize = s;               }
  void        SetStartAngle(Double_t);
  void        SetUpcom(const char *s)    { fUpcom = s;              }
  void        SetWaveL(Int_t,Double_t*);
  void        SetWithDate(Bool_t d)      { fWithDate = d;           }
  void        SetWrapDiffus(Double_t d)  { fWrapDiffus = d;         }
  void        SetXminC(Int_t x)          { fXminC = x;              }
  void        SetYminC(Int_t y)          { fYminC = y;              }
  ClassDef(TLitAlbedo,1) //Complex index of wrapping from albedo
};
#endif
