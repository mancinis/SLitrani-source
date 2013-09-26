// @(#)root/html:$Id: TArrayZ.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TArrayZ
#define LIT_TArrayZ
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TArrayZ  class for 1dim array of complex                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TComplex.h"

class TArrayZ : public TObject {

protected:


  void         Init();

public:

  Int_t     fN;  //length of the array
  TComplex *fA;   //[fN] Array of any base type of dimension fNd

  TArrayZ() { Init(); }
  TArrayZ(const TArrayZ&);
  TArrayZ(Int_t);
  virtual ~TArrayZ();
  Double_t Biggest() const;
  void     Clean();
  Int_t    Compare(const TObject* obj) const;
  void     Copy(const TArrayZ&);
  TComplex GetA(Int_t k) const  { return fA[k];     }
  Bool_t   IsCompatibleWith(const TArrayZ&) const;
  Bool_t   IsEqual(const TObject*) const;
  Bool_t   IsSortable() const   { return kTRUE;     }
  Double_t Norm() const;
  void     Print() const;
  ostream& Print(ostream& os) const;
  void     ReSet(Int_t);
  void     Set(Int_t);
  TArrayZ  &operator=(const TArrayZ&);
  TArrayZ  &operator+=(TComplex val);
  TArrayZ  &operator+=(const TArrayZ&);
  TArrayZ  &operator-=(TComplex val);
  TArrayZ  &operator-=(const TArrayZ&);
  TArrayZ  &operator*=(TComplex val);
  TArrayZ  &operator/=(TComplex val);
  Bool_t    operator< (const TArrayZ&) const;
  Bool_t    operator<=(const TArrayZ&) const;
  Bool_t    operator> (const TArrayZ&) const;
  Bool_t    operator>=(const TArrayZ&) const;
  TComplex &operator[](Int_t i);
  TComplex  operator[](Int_t i) const;
  ClassDef(TArrayZ,1) //1dim array of complex
};
inline Bool_t TArrayZ::IsCompatibleWith(const TArrayZ &A) const {
  // All dimensions have to be equal in order to declare 2 arrays as compatible
  return fN == A.fN;
}
TArrayZ operator+ (const TArrayZ&,const TArrayZ&);
TArrayZ operator- (const TArrayZ&,const TArrayZ&);
TComplex operator* (const TArrayZ&,const TArrayZ&);
ostream &operator<<(ostream&,const TArrayZ&);

#endif
