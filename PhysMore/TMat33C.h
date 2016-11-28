// @(#)root/html:$Id: TMat33C.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TMat33C
#define LIT_TMat33C
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMat33C   Implement 3X3 complex matrices associated with the real    //
//              TVector3 and with the complex TVec3C                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TVector3.h"
#include "TVec3C.h"

class TMat33C : public TObject  {

protected:

  TVec3C          fV0;  //1st line of matrix
  TVec3C          fV1;  //2nd line of matrix
  TVec3C          fV2;  //3rd line of matrix

public:

  friend TMat33C  operator+(Double_t,const TMat33C&);
  friend TMat33C  operator+(TComplex,const TMat33C&);
  friend TMat33C  operator-(Double_t,const TMat33C&);
  friend TMat33C  operator-(TComplex,const TMat33C&);
  friend TMat33C  operator*(Double_t,const TMat33C&);
  friend TMat33C  operator*(TComplex,const TMat33C&);
  friend TMat33C  operator*(const TMat33&,const TMat33C&);
  friend TVec3C   operator*(const TMat33C&,const TVector3&);
  friend TVec3C   operator*(const TMat33C&,const TVec3C&);
  friend TVec3C   operator*(const TVector3&,const TMat33C&);
  friend TVec3C   operator*(const TVec3C&,const TMat33C&);
  friend TMat33C  operator&(Double_t,const TMat33C&);
  friend TMat33C  operator&(TComplex,const TMat33C&);
  friend TMat33C  operator&(const TMat33&,const TMat33C&);

  TMat33C() { ; }
  TMat33C(Double_t);
  TMat33C(TComplex);
  TMat33C(Double_t,Double_t,Double_t);
  TMat33C(TComplex,TComplex,TComplex);
  TMat33C(Double_t,Double_t,Double_t,TComplex,TComplex,TComplex);
  TMat33C(const TVector3&,const TVec3C&);
  TMat33C(const TVec3C&,const TVec3C&,const TVec3C&,Bool_t=kFALSE);
  TMat33C(const TMat33C &m,Bool_t = kFALSE);
  TMat33C(const TMat33 &m,Bool_t = kFALSE);
  virtual           ~TMat33C() {;}
  void               ChangeSign();
  void               Clean();
  Double_t           CloseUnit() const;
  Int_t              Compare(const TObject*) const;
  void               Conjugate();
  TMat33C            Cos() const;
  void               CT();
  Short_t            DegMatrix(TVec3C&,TVec3C&,Bool_t=kFALSE) const;
  TComplex           Determinant() const;
  void               Diagonal(Double_t,Double_t,Double_t);
  void               Diagonal(TComplex,TComplex,TComplex);
  void               Diagonalize();
  Short_t            EigenValues(Double_t&,Double_t&,Double_t&) const;
  Short_t            EigenVectors(Double_t&,Double_t&,Double_t&,TVec3C&,TVec3C&,TVec3C&) const;
  Short_t            EigenProjectors(Double_t&,Double_t&,Double_t&,TVec3C&,TVec3C&,TVec3C&,
                               TMat33C&,TMat33C&,TMat33C&) const;
  TMat33C            Exp() const;
  TVec3C             GetColumn(Int_t i) const;
  void               GetColumn(Int_t i,TComplex&,TComplex&,TComplex&) const;
  TVec3C             GetLine(Int_t) const;
  void               GetLine(Int_t i,TComplex&,TComplex&,TComplex&) const;
  void               HConjugateOf(const TMat33C&);
  void               Hermitianize();
  Bool_t             Invert(TComplex&);
  Bool_t             Invert(Double_t&,TComplex&);
  TMat33C            Invert(TComplex&,Bool_t&) const;
  TMat33C            Invert(Double_t&,TComplex&,Bool_t&) const;
  Bool_t             IsEqual(const TObject *obj) const { return *this == *((TMat33C *)obj); }
  Bool_t             IsHermitian() const;
  Bool_t             IsHermitian(Bool_t);
  Bool_t             IsSortable() const                { return kTRUE;                        }
  Bool_t             IsUnitarian() const;
  Bool_t             IsUnitarian(Bool_t);
  Double_t           MaxCoeff() const;
  void               Print() const;
  static void        RatherPositiv(TVec3C&);
  void               Set(Double_t);
  void               Set(TComplex);
  void               Set(Double_t,Double_t,Double_t);
  void               Set(TComplex,TComplex,TComplex);
  void               Set(Double_t,Double_t,Double_t,TComplex,TComplex,TComplex);
  void               Set(const TVector3&,const TVec3C&);
  void               Set(const TVec3C&,const TVec3C&,const TVec3C&,Bool_t=kFALSE);
  void               Set(const TMat33C &m,Bool_t = kFALSE);
  void               Set(const TMat33 &m,Bool_t = kFALSE);
  void               SetAll(Double_t);
  void               SetAll(TComplex);
  void               SetColumn(Int_t i,const TVec3C&);
  void               SetColumn(Int_t i,Double_t a);
  void               SetColumn(Int_t i,TComplex a);
  void               SetColumn(Int_t i,TComplex a,TComplex b,TComplex c);
  void               SetLine(Int_t i,const TVec3C&);
  void               SetLine(Int_t i,Double_t a);
  void               SetLine(Int_t i,TComplex a);
  void               SetLine(Int_t i,TComplex a,TComplex b,TComplex c);
  void               SetStateAs(const TMat33C&);
  void               SetStateFalse();
  TMat33C            Sin() const;
  Double_t           SolveLin3(const TVec3C&,TVec3C&) const;
  static Double_t    SolveLin3(const TMat33C&,const TVec3C&,TVec3C&);
  void               T();
  void               Unit();
  Double_t           VerifyHermitian() const;
  Double_t           VerifyHermitian(Bool_t);
  Double_t           VerifyInverse(const TMat33C&) const;
  Double_t           VerifyUnitarity();
  void               Zero();
  static Bool_t      MyRootsCubic(const Double_t coef[4],Double_t&,Double_t&,Double_t&);
  TMat33C           &operator+=(Double_t);
  TMat33C           &operator+=(TComplex);
  TMat33C           &operator+=(const TMat33C&);
  TMat33C           &operator-=(Double_t);
  TMat33C           &operator-=(TComplex);
  TMat33C           &operator-=(const TMat33C&);
  TMat33C           &operator*=(Double_t);
  TMat33C           &operator*=(TComplex);
  TMat33C           &operator*=(const TMat33&);
  TMat33C           &operator*=(const TMat33C&);
  TMat33C           &operator&=(Double_t);
  TMat33C           &operator&=(TComplex);
  TMat33C           &operator&=(const TMat33&);
  TMat33C           &operator&=(const TMat33C&);
  TMat33C            operator+(Double_t) const;
  TMat33C            operator+(TComplex) const;
  TMat33C            operator+(const TMat33C&) const;
  TMat33C            operator-(Double_t) const;
  TMat33C            operator-(TComplex) const;
  TMat33C            operator-(const TMat33C&) const;
  TMat33C            operator*(Double_t) const;
  TMat33C            operator*(TComplex) const;
  TMat33C            operator*(const TMat33&) const;
  TMat33C            operator*(const TMat33C&) const;
  TMat33C            operator&(Double_t) const;
  TMat33C            operator&(TComplex) const;
  TMat33C            operator&(const TMat33&) const;
  TMat33C            operator&(const TMat33C&) const;
  TMat33C           &operator=(Double_t);
  TMat33C           &operator=(TComplex);
  TMat33C           &operator=(const TMat33C&);
  Bool_t             operator==(const TMat33&) const;
  Bool_t             operator==(const TMat33C&) const;
  Bool_t             operator>(const TMat33C&) const;
  Bool_t             operator>=(const TMat33C&) const;
  Bool_t             operator<(const TMat33C&) const;
  Bool_t             operator<=(const TMat33C&) const;
  TComplex           operator()(Int_t i,Int_t j) const;
  TComplex          &operator()(Int_t i,Int_t j,Bool_t=kFALSE);
  TVec3C             operator()(Int_t i) const;
  TVec3C            &operator()(Int_t i,Bool_t=kFALSE);
  ClassDef(TMat33C,1) //Implements complex 3X3 matrices associated with TVector3 and TVec3C
};
#endif
