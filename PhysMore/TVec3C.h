// @(#)root/html:$Id: TVec3C.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TVec3C
#define LIT_TVec3C
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVec3C  Implement complex 3-vectors in the same way as the ROOT      //
//            class TVector3                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TMath.h"
#include "TVector3.h"
#include "TComplex.h"

class TVec3C : public TObject {

protected:

  TComplex         fX;   //1st component of complex vector
  TComplex         fY;   //2nd component of complex vector
  TComplex         fZ;   //3rd component of complex vector

public:

  friend TVec3C    operator+(const TVector3&, const TVec3C&);
  friend TVec3C    operator-(const TVector3&, const TVec3C&);
  friend TVec3C    operator*(Double_t, const TVec3C&);
  friend TVec3C    operator*(const TComplex&, const TVec3C&);
  friend TComplex  operator*(const TVector3&, const TVec3C&);
  friend TComplex  operator&(const TVector3&, const TVec3C&);
  friend TVec3C    operator^(const TVector3&, const TVec3C&);
  friend ostream&  operator<<(ostream&, const TVec3C&);
  friend istream&  operator>>(istream&, TVec3C&);

  TVec3C(Double_t x = 0.0, Double_t y = 0.0, Double_t z = 0.0);
  TVec3C(TComplex x, TComplex y = 0.0, TComplex z = 0.0);
  TVec3C(const Float_t*);
  TVec3C(const Double_t*);
  TVec3C(const TComplex*);
  TVec3C(const TVector3&);
  TVec3C(const TVec3C&,Bool_t=kFALSE);
  virtual ~TVec3C();
  void              Clean();
  Int_t             Compare(const TObject*) const;
  inline TVec3C     Cross(const TVector3&) const;
  inline TVec3C     Cross(const TVec3C&) const;
  inline TComplex   Dot(const TVector3&) const;
  inline TComplex   Dot(const TVec3C&) const;
  inline TComplex   DotH(const TVector3&) const;
  inline TComplex   DotH(const TVec3C&) const;
  inline TVector3   Re() const;
  inline void       GetXYZ(TComplex&,TComplex&,TComplex&) const;
  inline void       GetXYZ(TComplex *carray) const;
  inline TVector3   Im() const;
  Bool_t            IsEqual(const TObject *obj) const { return *this == *((TVec3C *)obj); }
  Bool_t            IsSortable() const                { return kTRUE;                        }
  inline Double_t   Mag() const;
  inline Double_t   Mag2() const;
  Double_t          MaxCoeff();
  void              Print() const;
  inline TComplex   Px() const;
  inline TComplex   Py() const;
  inline TComplex   Pz() const;
  void              SetMag(Double_t);
  inline void       SetX(Double_t);
  inline void       SetX(TComplex);
  inline void       SetY(Double_t);
  inline void       SetY(TComplex);
  inline void       SetZ(Double_t);
  inline void       SetZ(TComplex);
  inline void       SetXYZ(Double_t x, Double_t y, Double_t z);
  inline void       SetXYZ(TComplex x, TComplex y, TComplex z);
  void              Unit();
  TVec3C            Unit() const;
  inline TComplex   x()  const;
  inline TComplex   X()  const;
  inline TComplex   y()  const;
  inline TComplex   Y()  const;
  inline TComplex   z()  const;
  inline TComplex   Z()  const;
  inline TVec3C    &operator+=(const TVector3&);
  inline TVec3C    &operator+=(const TVec3C&);
  inline TVec3C    &operator-=(const TVector3&);
  inline TVec3C    &operator-=(const TVec3C&);
  TVec3C           &operator*=(Double_t);
  TVec3C           &operator*=(TComplex);
  TVec3C           &operator^=(const TVector3&);
  TVec3C           &operator^=(const TVec3C&);
  TVec3C            operator+(const TVec3C&) const;
  TVec3C            operator+(const TVector3&) const;
  TVec3C            operator-(const TVec3C&) const;
  TVec3C            operator-(const TVector3&) const;
  TVec3C            operator-() const;
  TVec3C            operator*(Double_t) const;
  TVec3C            operator*(const TComplex&) const;
  TComplex          operator*(const TVector3&) const;
  TComplex          operator*(const TVec3C&) const;
  TComplex          operator&(const TVector3&) const;
  TComplex          operator&(const TVec3C&) const;
  TVec3C            operator^(const TVector3&) const;
  TVec3C            operator^(const TVec3C&) const;
  inline TVec3C    &operator=(const TVector3&);
  inline TVec3C    &operator=(const TVec3C&);
  Bool_t            operator==(const TVector3&) const;
  Bool_t            operator==(const TVec3C&) const;
  Bool_t            operator!=(const TVector3&) const;
  Bool_t            operator!=(const TVec3C&) const;
  Bool_t            operator>(const TVector3&) const;
  Bool_t            operator>(const TVec3C&) const;
  Bool_t            operator>=(const TVector3&) const;
  Bool_t            operator>=(const TVec3C&) const;
  Bool_t            operator<(const TVector3&) const;
  Bool_t            operator<(const TVec3C&) const;
  Bool_t            operator<=(const TVector3&) const;
  Bool_t            operator<=(const TVec3C&) const;
  TComplex          operator()(Int_t) const;
  TComplex         &operator()(Int_t);
  inline TComplex   operator[](Int_t) const;
  inline TComplex  &operator[](Int_t);
  ClassDef(TVec3C,1) //Implement complex 3-vectors in the same way as real TVector3
};
inline TVec3C TVec3C::Cross(const TVector3 &p) const {
// Cross product with a real vector. See also operator ^= and ^
  return TVec3C(fY*p.Z()-p.Y()*fZ, fZ*p.X()-p.Z()*fX, fX*p.Y()-p.X()*fY);
}
inline TVec3C TVec3C::Cross(const TVec3C &p) const {
// Cross product with a complex vector. See also operator ^= and ^
  return TVec3C(fY*p.fZ-p.fY*fZ, fZ*p.fX-p.fZ*fX, fX*p.fY-p.fX*fY);
}
inline TComplex TVec3C::Dot(const TVector3 &p) const {
// Usual scalar product v1*v2, v1 complex and v2 real
  return fX*p.X() + fY*p.Y() + fZ*p.Z();
}
inline TComplex TVec3C::Dot(const TVec3C &p) const {
// Usual scalar product v1*v2, v1 complex and v2 complex
  return fX*p.fX + fY*p.fY + fZ*p.fZ;
}
inline TComplex TVec3C::DotH(const TVector3 &p) const {
// Hermitian scalar product Conjugate(v1)*v2
  return TComplex::Conjugate(fX)*p.X() + TComplex::Conjugate(fY)*p.Y() + TComplex::Conjugate(fZ)*p.Z();
}
inline TComplex TVec3C::DotH(const TVec3C &p) const {
// Hermitian scalar product Conjugate(v1)*v2
  return TComplex::Conjugate(fX)*p.fX + TComplex::Conjugate(fY)*p.fY + TComplex::Conjugate(fZ)*p.fZ;
}
inline TVector3 TVec3C::Re() const {
// Returns real part of vector
  return TVector3(fX.Re(),fY.Re(),fZ.Re());
}
inline void TVec3C::GetXYZ(TComplex &z0,TComplex &z1,TComplex &z2) const {
  // Get the 3 components into z0, z1, z2
  z0 = fX;
  z1 = fY;
  z2 = fZ;
}
inline void TVec3C::GetXYZ(TComplex *carray) const {
  // Get the 3 components into carray
  carray[0] = fX;
  carray[1] = fY;
  carray[2] = fZ;
}
inline TVector3 TVec3C::Im() const {
// Returns imaginary part of vector
  return TVector3(fX.Im(),fY.Im(),fZ.Im());
}
inline Double_t TVec3C::Mag() const  { return TMath::Sqrt(Mag2()); }
inline Double_t TVec3C::Mag2() const { return fX.Rho2() + fY.Rho2() + fZ.Rho2(); }
inline TComplex TVec3C::Px() const   { return fX; }
inline TComplex TVec3C::Py() const   { return fY; }
inline TComplex TVec3C::Pz() const   { return fZ; }
inline void TVec3C::SetX(Double_t x) { fX = x; }
inline void TVec3C::SetX(TComplex x) { fX = x; }
inline void TVec3C::SetY(Double_t y) { fY = y; }
inline void TVec3C::SetY(TComplex y) { fY = y; }
inline void TVec3C::SetZ(Double_t z) { fZ = z; }
inline void TVec3C::SetZ(TComplex z) { fZ = z; }
inline void TVec3C::SetXYZ(Double_t x, Double_t y, Double_t z) {
   fX = x;  fY = y;  fZ = z;
}
inline void TVec3C::SetXYZ(TComplex x, TComplex y, TComplex z) {
   fX = x;  fY = y;  fZ = z;
}
inline TComplex TVec3C::x()  const { return fX; }
inline TComplex TVec3C::X()  const { return fX; }
inline TComplex TVec3C::y()  const { return fY; }
inline TComplex TVec3C::Y()  const { return fY; }
inline TComplex TVec3C::z()  const { return fZ; }
inline TComplex TVec3C::Z()  const { return fZ; }
inline TVec3C& TVec3C::operator+=(const TVector3 &p) {
  fX += p.X();
  fY += p.Y();
  fZ += p.Z();
  return *this;
}
inline TVec3C& TVec3C::operator+=(const TVec3C &p) {
  fX += p.fX;
  fY += p.fY;
  fZ += p.fZ;
  return *this;
}
inline TVec3C& TVec3C::operator-=(const TVector3 &p) {
  fX -= p.X();
  fY -= p.Y();
  fZ -= p.Z();
  return *this;
}
inline TVec3C& TVec3C::operator-=(const TVec3C &p) {
  fX -= p.fX;
  fY -= p.fY;
  fZ -= p.fZ;
  return *this;
}
inline TVec3C &TVec3C::operator=(const TVector3 &p) {
  fX = p.X();
  fY = p.Y();
  fZ = p.Z();
  return *this;
}
inline TVec3C &TVec3C::operator=(const TVec3C &p) {
  fX = p.fX;
  fY = p.fY;
  fZ = p.fZ;
  return *this;
}
TComplex  TVec3C::operator[] (Int_t i) const { return operator()(i); }
TComplex &TVec3C::operator[] (Int_t i)       { return operator()(i); }
#endif
