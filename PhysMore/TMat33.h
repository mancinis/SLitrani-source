// @(#)root/html:$Id: TMat33.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TMat33
#define LIT_TMat33
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMat33  Implement 3X3 real matrices associated with the TVector3     //
//            of ROOT and TVec3C                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TVector3.h"
#include "TVec3C.h"

class TMat33 : public TObject  {

protected:

  TVector3          fV0;  //1st line of matrix
  TVector3          fV1;  //2nd line of matrix
  TVector3          fV2;  //3rd line of matrix

  static void PrintWP(Double_t);

public:

  static Int_t      fgWidth;
  static Int_t      fgPrec;
  static UInt_t     fgSymTested;
  static UInt_t     fgSym;
  static UInt_t     fgUniTested;
  static UInt_t     fgUni;
  static Double_t   fgLimPrec;

  friend TMat33     operator+(Double_t,const TMat33&);
  friend TMat33     operator-(Double_t,const TMat33&);
  friend TMat33     operator*(Double_t,const TMat33&);
  friend TVector3   operator*(const TMat33&,const TVector3&);
  friend TVec3C     operator*(const TMat33&,const TVec3C&);
  friend TVector3   operator*(const TVector3&,const TMat33&);
  friend TVec3C     operator*(const TVec3C&,const TMat33&);
  friend TMat33     operator&(Double_t,const TMat33&);

  TMat33() { ; }
  TMat33(Double_t);
  TMat33(Double_t,Double_t,Double_t);
  TMat33(Double_t,Double_t,Double_t,Bool_t);
  TMat33(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  TMat33(const TVector3&,const TVector3&);
  TMat33(TVector3,TVector3,Bool_t&,Double_t&);
  TMat33(const TVector3&,const TVector3&,const TVector3&,Bool_t=kFALSE);
  TMat33(const TMat33 &m,Bool_t = kFALSE);
  virtual          ~TMat33() {;}
  void              ChangeSign();
  void              Clean();
  Double_t          CloseUnit() const;
  Int_t             Compare(const TObject*) const;
  TMat33            Cos() const;
  Short_t           DegMatrix(TVector3&,TVector3&,Bool_t=kFALSE) const;
  Double_t          Determinant() const;
  void              Diagonal(Double_t,Double_t,Double_t);
  void              Diagonalize();
  Short_t           EigenValues(Double_t&,Double_t&,Double_t&) const;
  Short_t           EigenVectors(Double_t&,Double_t&,Double_t&,TVector3&,TVector3&,TVector3&) const;
  Short_t           EigenProjectors(Double_t&,Double_t&,Double_t&,TVector3&,TVector3&,TVector3&,
                               TMat33&,TMat33&,TMat33&) const;
  TMat33            Exp() const;
  TVector3          GetColumn(Int_t i) const;
  void              GetColumn(Int_t i,Double_t&,Double_t&,Double_t&) const;
  TVector3          GetLine(Int_t) const;
  void              GetLine(Int_t i,Double_t&,Double_t&,Double_t&) const;
  Bool_t            Invert(Double_t&);
  Bool_t            Invert(Double_t&,Double_t&);
  TMat33            Invert(Double_t&,Bool_t&) const;
  TMat33            Invert(Double_t&,Double_t&,Bool_t&) const;
  Bool_t            IsEqual(const TObject *obj) const { return *this == *((TMat33 *)obj); }
  Bool_t            IsSortable() const                { return kTRUE;                        }
  Bool_t            IsSymmetric() const;
  Bool_t            IsSymmetric(Bool_t);
  Bool_t            IsUnitarian() const;
  Bool_t            IsUnitarian(Bool_t);
  Double_t          MaxCoeff() const;
  void              Print() const;
  static void       RatherPositiv(TVector3&);
  void              Set(Double_t);
  void              Set(Double_t,Double_t,Double_t);
  void              Set(Double_t,Double_t,Double_t,Bool_t);
  void              Set(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void              Set(const TVector3&,const TVector3&);
  void              Set(const TVector3&,const TVector3&,const TVector3&,Bool_t=kFALSE);
  void              Set(const TMat33 &m,Bool_t = kFALSE);
  void              SetAll(Double_t);
  void              SetColumn(Int_t i,const TVector3&);
  void              SetColumn(Int_t i,Double_t a);
  void              SetColumn(Int_t i,Double_t a,Double_t b,Double_t c);
  void              SetLine(Int_t i,const TVector3&);
  void              SetLine(Int_t i,Double_t a);
  void              SetLine(Int_t i,Double_t a,Double_t b,Double_t c);
  void              SetStateAs(const TMat33&);
  void              SetStateFalse();
  TMat33            Sin() const;
  Double_t          SolveLin3(const TVector3&,TVector3&) const;
  static Double_t   SolveLin3(const TMat33&,const TVector3&,TVector3&);
  Double_t          Symmetrize();
  void              T();
  void              TransposedOf(const TMat33&);
  void              Unit();
  Double_t          VerifyInverse(const TMat33&) const;
  Double_t          VerifySymmetric();
  Double_t          VerifyUnitarity();
  void              Zero();
  static Bool_t     MyRootsCubic(const Double_t coef[4],Double_t&,Double_t&,Double_t&);
  TMat33           &operator+=(Double_t);
  TMat33           &operator+=(const TMat33&);
  TMat33           &operator-=(Double_t);
  TMat33           &operator-=(const TMat33&);
  TMat33           &operator*=(Double_t);
  TMat33           &operator*=(const TMat33&);
  TMat33           &operator&=(Double_t);
  TMat33           &operator&=(const TMat33&);
  TMat33            operator+(Double_t) const;
  TMat33            operator+(const TMat33&) const;
  TMat33            operator-(Double_t) const;
  TMat33            operator-(const TMat33&) const;
  TMat33            operator*(Double_t) const;
  TMat33            operator*(const TMat33&) const;
  TMat33            operator&(Double_t) const;
  TMat33            operator&(const TMat33&) const;
  TMat33           &operator=(Double_t);
  TMat33           &operator=(const TMat33&);
  Bool_t            operator==(const TMat33&) const;
  Bool_t            operator>(const TMat33&) const;
  Bool_t            operator>=(const TMat33&) const;
  Bool_t            operator<(const TMat33&) const;
  Bool_t            operator<=(const TMat33&) const;
  Double_t          operator()(Int_t i,Int_t j) const;
  Double_t         &operator()(Int_t i,Int_t j,Bool_t=kFALSE);
  TVector3          operator()(Int_t i) const;
  TVector3         &operator()(Int_t i,Bool_t=kFALSE);
  ClassDef(TMat33,1) //implements 3X3 matrices associated with TVector3 and TVec3C
};
TVector3 Clean(const TVector3&);
TVector3 operator^(const TVector3&,const TVector3&);
#endif
