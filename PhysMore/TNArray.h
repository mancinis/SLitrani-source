// @(#)root/html:$Id: TNArray.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TNArray
#define LIT_TNArray
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TNArray  class for multidimensional arrays of any base type          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TComplex.h"

template <typename TT> class TNArray : public TObject {

protected:

  Int_t     fNd;  //dimension of the array
  UInt_t   *fN;   //[fNd] size for each dimension
  Int_t     fNL;  //length of the array = fN[0]*fN[1]*fN[2]*fN[3]*fN[4]*fN[5] + 1
  TT       *fA;   //[fNL] Array of any base type of dimension fNd

  void         Init();
  inline Int_t OneDim(Int_t) const;
  inline Int_t OneDim(Int_t,Int_t) const;
  inline Int_t OneDim(Int_t,Int_t,Int_t) const;
  inline Int_t OneDim(Int_t,Int_t,Int_t,Int_t) const;
  inline Int_t OneDim(Int_t,Int_t,Int_t,Int_t,Int_t) const;
  inline Int_t OneDim(Int_t,Int_t,Int_t,Int_t,Int_t,Int_t) const;

public:


  TNArray() { Init(); }
  TNArray(const TNArray&);
  TNArray(Int_t,UInt_t*);
  TNArray(Int_t);
  TNArray(Int_t,Int_t,Bool_t = kFALSE);
  TNArray(Int_t,Int_t,Int_t);
  TNArray(Int_t,Int_t,Int_t,Int_t);
  TNArray(Int_t,Int_t,Int_t,Int_t,Int_t);
  TNArray(Int_t,Int_t,Int_t,Int_t,Int_t,Int_t);
  virtual ~TNArray();
  Double_t Biggest() const;
  void     Clean();
  Int_t    Compare(const TObject* obj) const;
  void     Copy(const TNArray&);
  TT       GetA(Int_t k) const  { return fA[k];     }
  Int_t    GetDimension() const { return fNd;       }
  Int_t    GetN(Int_t k) const  { return fN[k];     }
  Int_t    GetN1() const        { return fN[0];     }
  Int_t    GetN2() const        { return fN[1];     }
  Int_t    GetN3() const        { return fN[2];     }
  Int_t    GetN4() const        { return fN[3];     }
  Int_t    GetN5() const        { return fN[4];     }
  Int_t    GetN6() const        { return fN[5];     }
  Int_t    GetSize() const      { return fNL - 1;   }
  TT       GetOverFlow() const  { return fA[fNL-1]; }
  Bool_t   IsCompatibleWith(const TNArray&) const;
  Bool_t   IsEqual(const TObject*) const;
  Bool_t   IsSortable() const   { return kTRUE;     }
  Double_t Norm() const;
  void     Print() const;
  ostream& Print(ostream& os) const;
  void     ReSet(Int_t);
  void     ReSet(Int_t,Int_t);
  void     ReSet(Int_t,Int_t,Int_t);
  void     ReSet(Int_t,Int_t,Int_t,Int_t);
  void     ReSet(Int_t,Int_t,Int_t,Int_t,Int_t);
  void     ReSet(Int_t,Int_t,Int_t,Int_t,Int_t,Int_t);
  void     Set(Int_t);
  void     Set(Int_t,Int_t);
  void     Set(Int_t,Int_t,Int_t);
  void     Set(Int_t,Int_t,Int_t,Int_t);
  void     Set(Int_t,Int_t,Int_t,Int_t,Int_t);
  void     Set(Int_t,Int_t,Int_t,Int_t,Int_t,Int_t);
  TNArray  &operator=(const TNArray&);
  TNArray  &operator+=(TT val);
  TNArray  &operator+=(const TNArray&);
  TNArray  &operator-=(TT val);
  TNArray  &operator-=(const TNArray&);
  TNArray  &operator*=(TT val);
  TNArray  &operator/=(TT val);
  Bool_t    operator< (const TNArray&) const;
  Bool_t    operator<=(const TNArray&) const;
  Bool_t    operator> (const TNArray&) const;
  Bool_t    operator>=(const TNArray&) const;
  const TT &operator()(Int_t i1) const;
  const TT &operator()(Int_t i1,Int_t i2) const;
  const TT &operator()(Int_t i1,Int_t i2,Int_t i3) const;
  const TT &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4) const;
  const TT &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5) const;
  const TT &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5,Int_t i6) const;
  TT       &operator()(Int_t i1);
  TT       &operator()(Int_t i1,Int_t i2);
  TT       &operator()(Int_t i1,Int_t i2,Int_t i3);
  TT       &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4);
  TT       &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5);
  TT       &operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5,Int_t i6);
  ClassDef(TNArray,1) //Multidimensional arrays of any base type
};
template <typename TT> inline Bool_t TNArray<TT>::IsCompatibleWith(const TNArray<TT> &A) const {
  // All dimensions have to be equal in order to declare 2 arrays as compatible
  Bool_t compatible = kFALSE;
  if (fNd == A.fNd) {
    if (fNL == A.fNL) {
      if (fN[0] == A.fN[0]) {
        if (fNd>1) {
          if (fN[1] == A.fN[1]) {
            if (fNd>2) {
              if (fN[2] == A.fN[2]) {
                if (fNd>3) {
                  if (fN[3] == A.fN[3]) {
                    if (fNd>4) {
                      if (fN[4] == A.fN[4]) {
                        if (fNd>5) {
                          if (fN[5] == A.fN[5]) compatible = kTRUE;
                        }
                        else compatible = kTRUE;
                      }
                    }
                    else compatible = kTRUE;
                  }
                }
                else compatible = kTRUE;
              }
            }
            else compatible = kTRUE;
          }
        }
        else compatible = kTRUE;
      }
    }
  }
  return compatible;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1) const {
//Index from 1 dimension to 1 dimension
  if ((i1>=fNL - 1) || (i1<0)) {
    i1 = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i1;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1,Int_t i2) const {
//Index from 2 dimension to 1 dimension
  Int_t i;
  i = i1 + fN[0]*i2;
  if ((i>=fNL - 1) || (i<0)) {
    i = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1,Int_t i2,Int_t i3) const {
//Index from 3 dimension to 1 dimension
  Int_t i;
  i = i1 + fN[0]*(i2 + fN[1]*i3);
  if ((i>=fNL - 1) || (i<0)) {
    i = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1,Int_t i2,Int_t i3,Int_t i4) const {
//Index from 4 dimension to 1 dimension
  Int_t i;
  i = i1 + fN[0]*(i2 + fN[1]*(i3 + fN[2]*i4));
  if ((i>=fNL - 1) || (i<0)) {
    i = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5) const {
//Index from 5 dimension to 1 dimension
  Int_t i;
  i = i1 + fN[0]*(i2 + fN[1]*(i3 + fN[2]*(i4 + fN[3]*i5)));
  if ((i>=fNL - 1) || (i<0)) {
    i = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i;
}
template <typename TT> inline Int_t TNArray<TT>::OneDim(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5,Int_t i6) const {
//Index from 6 dimension to 1 dimension
  Int_t i;
  i = i1 + fN[0]*(i2 + fN[1]*(i3 + fN[2]*(i4 + fN[3]*(i5 + fN[4]*i6))));
  if ((i>=fNL - 1) || (i<0)) {
    i = fNL - 1;
    Error("OneDim","Index outside bounds");
  }
  return i;
}
template<typename TT> class TNArray;
template<typename TT> TNArray<TT> operator+ (const TNArray<TT>&,const TNArray<TT>&);
template<typename TT> TNArray<TT> operator- (const TNArray<TT>&,const TNArray<TT>&);
template<typename TT> TComplex operator* (const TNArray<TT>&,const TNArray<TT>&);
template<typename TT> ostream &operator<<(ostream&,const TNArray<TT>&);
typedef TNArray<Char_t>     TNArrayC;
typedef TNArray<Short_t>    TNArrayS;
typedef TNArray<Int_t>      TNArrayI;
typedef TNArray<Long_t>     TNArrayL;
typedef TNArray<Long64_t>   TNArrayL64;
typedef TNArray<Float_t>    TNArrayF;
typedef TNArray<Double_t>   TNArrayD;
typedef TNArray<TComplex>   TNArrayZ;

#endif
