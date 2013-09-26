// @(#)root/html:$Id: TLitPol4.h 2008-7-7
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*********************************************************************************
 * Copyright The same as the ones of ROOT                                        *
*********************************************************************************/
#ifndef LIT_TLitPol4
#define LIT_TLitPol4
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// TLitPol4  Handle polynomials of 4th degree                                   //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TComplex.h"


union deux
{
  Double_t DD;
  UInt_t II[2];
};

class TLitPol3;

class TLitPol4 : public TObject {

protected:

  Double_t     fAn;        //Coefficient of x^4
  Double_t     fXt;        //Translation x' = x - fXt, such that term in x'^2 disappear
  Double_t     fP;         //p of equation f(x') = x'^4 + px'^2 + qx + r
  Double_t     fQ;         //q of equation f(x') = x'^4 + px'^2 + qx + r
  Double_t     fR;         //r of equation f(x') = x'^4 + px'^2 + qx + r
  Double_t     fMag;       //! Magnitude of coefficients = |a|+|b|+|c|+|d|, then magnitude of roots
  Short_t      fNr;        //! Number of real roots
  Short_t      fNe;        //! Number of extrema
  Double_t     fYextr[3];  //! Y value at the extrema
  TLitPol3    *fPol3;      //! Derivative of normalized polynomial of 4th degree
  TComplex     fRoots[4];  //! Array of real or complex roots of polynomial
  Double_t     fERoots[4]; //! Absolute value of the normalized polynomial at fRoots[k]

  void         ClearP();
  void         Derivative();
  void         ImAtEnd();
  void         InitP();
  Short_t      MostIsolated(Bool_t&) const;
  Bool_t       Newton(Double_t&,Double_t &) const;
  Bool_t       Newton(TComplex&,Double_t &) const;
  Double_t     NotSmallIm();
  void         Order();
  Bool_t       Starting(TComplex&) const;
  Double_t     Yn(Double_t) const;
  TComplex     Yn(TComplex) const;
  Double_t     Yn(Int_t,Double_t) const;
  TComplex     Yn(Int_t,TComplex) const;

public:

  TLitPol4()                        { InitP();              }
  TLitPol4(Double_t,Double_t,Double_t,Double_t,Double_t);
  TLitPol4(TLitPol4&);
  virtual       ~TLitPol4();
  virtual Int_t  Compare(const TObject*) const;
  Double_t       FindRoots();
  Double_t       FindRootsRMP();
  void           Getabcde(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&) const;
  Double_t       GetAn() const      { return fAn;           }
  Double_t       GetERoot(Short_t k) const;
  Double_t       GetMag() const     { return fMag;          }
  Short_t        GetNc() const;
  Short_t        GetNe() const      { return fNe;           }
  Double_t       GetOrig() const;
  Short_t        GetNr() const      { return fNr;           }
  Double_t       GetP() const       { return fP;            }
  Double_t       GetQ() const       { return fQ;            }
  Double_t       GetR() const       { return fR;            }
  TComplex       GetRoot(Short_t k) const;
  Double_t       GetXextr(Short_t k) const;
  Double_t       GetXt() const      { return fXt;           }
  Double_t       GetYextr(Short_t k) const;
  Bool_t         IsComplex(Int_t) const;
  Bool_t         IsComplex(Short_t,Double_t lim) const;
  virtual Bool_t IsEqual(const TObject*) const;
  virtual Bool_t IsSortable() const { return kTRUE;         }
  void           Print() const;
  void           PrintRoots() const;
  void           Set(Double_t,Double_t,Double_t,Double_t,Double_t);
  Double_t       Y(Double_t) const;
  TComplex       Y(TComplex) const;
  Double_t       Y(Int_t,Double_t) const;
  TComplex       Y(Int_t,TComplex) const;
  Double_t       operator()(Double_t) const;
  TComplex       operator()(TComplex) const;
  Double_t       operator()(Int_t,Double_t) const;
  TComplex       operator()(Int_t,TComplex) const;
  TLitPol4      &operator=(const TLitPol4&);
  Bool_t         operator==(const TLitPol4&);
  ClassDef(TLitPol4,1) //Handle polynomials of 4th degree
};
#endif
