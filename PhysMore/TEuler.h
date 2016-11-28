// @(#)root/html:$Id: TEuler.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TEuler
#define LIT_TEuler
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEuler                                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TVector3.h"
#include "TMat33.h"

class TEuler : public TObject {

protected:

  Double_t fPhi;    //Euler angle phi in radians
  Double_t fTheta;  //Euler angle theta in radian
  Double_t fPsi;    //Euler angle psi in radian
  Double_t fThetax; //! Geant3 theta angle of the rotated x axis in radian
  Double_t fPhix;   //! Geant3 phi   angle of the rotated x axis in radian
  Double_t fThetay; //! Geant3 theta angle of the rotated y axis in radian
  Double_t fPhiy;   //! Geant3 phi   angle of the rotated y axis in radian
  Double_t fThetaz; //! Geant3 theta angle of the rotated z axis in radian
  Double_t fPhiz;   //! Geant3 phi   angle of the rotated z axis in radian

  void     PrintWP(Double_t) const;

public:

  TMat33 fM;         //! Matrix giving new axis when applied to old axis

  TEuler(Bool_t = kFALSE);
  TEuler(Double_t,Double_t,Double_t,Bool_t=kTRUE);
  TEuler(Double_t,Double_t,Double_t,Double_t,Bool_t=kTRUE);
  virtual ~TEuler();
  void     GetEulerAngles(Double_t&,Double_t&,Double_t&,Bool_t=kTRUE) const;
  void     GetG3Angles(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Bool_t=kTRUE) const;
  Double_t GetDPhi() const    { return (180*fPhi)/TMath::Pi();    }
  Double_t GetDTheta() const  { return (180*fTheta)/TMath::Pi();  }
  Double_t GetDPsi() const    { return (180*fPsi)/TMath::Pi();    }
  Double_t GetDPhix() const   { return (180*fPhix)/TMath::Pi();   }
  Double_t GetDPhiy() const   { return (180*fPhiy)/TMath::Pi();   }
  Double_t GetDPhiz() const   { return (180*fPhiz)/TMath::Pi();   }
  Double_t GetDThetax() const { return (180*fThetax)/TMath::Pi(); }
  Double_t GetDThetay() const { return (180*fThetay)/TMath::Pi(); }
  Double_t GetDThetaz() const { return (180*fThetaz)/TMath::Pi(); }
  void     GetOx(TVector3&) const;
  void     GetOy(TVector3&) const;
  void     GetOz(TVector3&) const;
  void     GetOxyz(TVector3&,TVector3&,TVector3&) const;
  Double_t GetRPhi() const    { return fPhi;                      }
  Double_t GetRTheta() const  { return fTheta;                    }
  Double_t GetRPsi() const    { return fPsi;                      }
  Double_t GetRPhix() const   { return fPhix;                     }
  Double_t GetRPhiy() const   { return fPhiy;                     }
  Double_t GetRPhiz() const   { return fPhiz;                     }
  Double_t GetRThetax() const { return fThetax;                   }
  Double_t GetRThetay() const { return fThetay;                   }
  Double_t GetRThetaz() const { return fThetaz;                   }
  void     Identical();
  void     PrintEulerAngles(Bool_t=kTRUE) const;
  void     PrintG3Angles(Bool_t=kTRUE) const;
  void     PrintMatrix() const;
  void     Set();
  void     Set(Double_t,Double_t,Double_t,Bool_t=kTRUE);
  void     Set(Double_t,Double_t,Double_t,Double_t,Bool_t=kTRUE);
  ClassDef(TEuler,1)  //Euler <==> GEANT3 rotation angles
};
#endif
