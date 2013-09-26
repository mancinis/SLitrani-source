// @(#)root/html:$Id: TLitPol3.h 2008-4-7
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitPol3
#define LIT_TLitPol3
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// TLitPol3  Handling of 3rd degree polynomials                                 //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TComplex.h"

class TLitPol3 : public TObject {

protected:

  Short_t      fNd;        //Degree of polynomial [3,2,1,0]
  Double_t     fAn;        //Coefficient of x^N, N degree of polynomial
  Double_t     fXt;        //Translation x' = x - fXt, such that term in x'^2 disappear
  Double_t     fP;         //p of equation f(x') = x'^3 + px' + q
  Double_t     fQ;         //q of equation f(x') = x'^3 + px' + q
  Double_t     fMag;       //! Magnitude of coefficients = |a|+|b|+|c|+|d|, then magnitude of roots
  Short_t      fNr;        //! Number of real roots.
  TComplex     fRoots[3];  //! Array of real or complex roots of polynomial
  Double_t     fERoots[3]; //! Absolute value of the normalized polynomial at fRoots[k]

  void         InitP();
  void         Mag1(Double_t,Double_t,Double_t,Double_t);
  void         Mag2();
  void         NotSmallIm(Double_t);
  void         Order(Double_t *) const;
  Double_t     Yn(Double_t) const;
  TComplex     Yn(TComplex) const;
  Double_t     Yn(Int_t,Double_t) const;
  TComplex     Yn(Int_t,TComplex) const;

public:

  TLitPol3()                   { InitP(); }
  TLitPol3(Double_t,Double_t,Double_t,Double_t);
  TLitPol3(TLitPol3&);
  virtual       ~TLitPol3();
  virtual Int_t  Compare(const TObject*) const;
  Double_t       FindRoots(Double_t = -1.0);
  void           Getabcd(Double_t&,Double_t&,Double_t&,Double_t&) const;
  Double_t       GetAn() const      { return fAn;           }
  Double_t       GetERoot(Short_t k) const;
  Double_t       GetMag() const     { return fMag;          }
  Short_t        GetNc() const;
  Short_t        GetNd() const      { return fNd;           }
  Short_t        GetNr() const      { return fNr;           }
  Double_t       GetP() const       { return fP;            }
  Double_t       GetQ() const       { return fQ;            }
  TComplex       GetRoot(Short_t k) const;
  Double_t       GetXt() const      { return fXt;           }
  Bool_t         IsComplex(Short_t) const;
  Bool_t         IsComplex(Short_t,Double_t lim) const;
  virtual Bool_t IsEqual(const TObject*) const;
  virtual Bool_t IsSortable() const { return kTRUE;         }
  void           Print() const;
  void           PrintRoots() const;
  void           Set(Double_t,Double_t,Double_t,Double_t);
  Double_t       Y(Double_t) const;
  TComplex       Y(TComplex) const;
  Double_t       Y(Int_t,Double_t) const;
  TComplex       Y(Int_t,TComplex) const;
  Double_t       operator()(Double_t) const;
  TComplex       operator()(TComplex) const;
  Double_t       operator()(Int_t,Double_t) const;
  TComplex       operator()(Int_t,TComplex) const;
  TLitPol3      &operator=(const TLitPol3&);
  Bool_t         operator==(const TLitPol3&);
  ClassDef(TLitPol3,1) //Handling of 3rd degree polynomials
};
#endif
