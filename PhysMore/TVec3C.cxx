// @(#)root/html:$Id: TVec3C.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TVec3C.h"

ClassImp(TVec3C)

//______________________________________________________________________________
//
// TVec3C Implement complex 3-vectors in the same way as the ROOT TVector3
// TVec3C are associated with the complex matrices TMat33C, see also
//  this class.
//
// An effort has been made in order that TVec3C looks as close as possible
//  like TVector3. In principle, everything you can do with TVector3 can also
//  be done with TVec3C.
// Notice however the presence of 2 types of dot product [methods DotH(), Dot()
//  operator * or operator &]. If a, b are TVec3C, then:
//     a*b  is equal to  Conj(a(0))*b(0) + Conj(a(1))*b(1) + Conj(a(2))*b(2)
//                [dot product in hermitian space]
//     a&b  is equal to  a(0)*b(0) + a(1)*b(1) + a(2)*b(2)
//                [dot product in "euclidian complex" space]
//
// Operator ^ is associated with the cross product. If a, b, c are
//  TVec3C, you can write for the cross product:
//     c = a.Cross(b);     or simpler   c = a^b;
// Operator ^ has also been added for the real vectors TVector3. See inside
//  class TMat33
//
// A huge number of operators make the handling of TVec3C very easy.
//  writing C++ code with it is almost identical with writing mathematical
//  expressions.
//
// TVec3C has the 3 methods [Compare, IsEqual, IsSortable] allowing to sort
//  vectors inside collection. The classification parameter is the magnitude
//  of the vectors.
//
TVec3C::TVec3C(Double_t x, Double_t y, Double_t z) {
// Default constructor
  fX = x;  fY = y;  fZ = z;
}
TVec3C::TVec3C(TComplex x, TComplex y, TComplex z) {
  fX = x;  fY = y;  fZ = z;
}
TVec3C::TVec3C(const Float_t *x) {
  fX = x[0];  fY = x[1];  fZ = x[2];
}
TVec3C::TVec3C(const Double_t *x) {
  fX = x[0];  fY = x[1];  fZ = x[2];
}
TVec3C::TVec3C(const TComplex *z) {
  fX = z[0];  fY = z[1];  fZ = z[2];
}
TVec3C::TVec3C(const TVector3 &v) {
// Constructor from a real vector
  fX = v.X();
  fY = v.Y();
  fZ = v.Z();
}
TVec3C::TVec3C(const TVec3C &p,Bool_t conjugate):TObject(p) {
// Copy constructor in case conjugate false [default]. Otherwise
//construct the complex conjugate vector.
  if (conjugate) {
    fX = TComplex::Conjugate(p.fX);
    fY = TComplex::Conjugate(p.fY);
    fZ = TComplex::Conjugate(p.fZ);
  }
  else {
    fX = p.X();
    fY = p.Y();
    fZ = p.Z();
  }
}
TVec3C::~TVec3C() {}
//
void TVec3C::Clean() {
  const Double_t zero = 0.0;
  const TComplex I(0.0,1.0);
  const Double_t epsD = 1.0e-12;
  Bool_t adjust;
  Double_t eps,D,zR,zI;
  D   = MaxCoeff();
  eps = D*epsD;
  //
  zR  = fX.Re();
  zI  = fX.Im();
  adjust = kFALSE;
  if (TMath::Abs(zR)<eps) {
    adjust = kTRUE;
    zR     = zero;
  }
  if (TMath::Abs(zI)<eps) {
    adjust = kTRUE;
    zI     = zero;
  }
  if (adjust) fX = zR + zI*I;
  //
  zR  = fY.Re();
  zI  = fY.Im();
  adjust = kFALSE;
  if (TMath::Abs(zR)<eps) {
    adjust = kTRUE;
    zR     = zero;
  }
  if (TMath::Abs(zI)<eps) {
    adjust = kTRUE;
    zI     = zero;
  }
  if (adjust) fY = zR + zI*I;
  //
  zR  = fZ.Re();
  zI  = fZ.Im();
  adjust = kFALSE;
  if (TMath::Abs(zR)<eps) {
    adjust = kTRUE;
    zR     = zero;
  }
  if (TMath::Abs(zI)<eps) {
    adjust = kTRUE;
    zI     = zero;
  }
  if (adjust) fZ = zR + zI*I;
}
Int_t TVec3C::Compare(const TObject *obj) const {
// Compare according to magnitude. Allows to sort vectors in a collection
  if ( *this < *((TVec3C*)obj) ) return -1;
  else {
    if ( *this > *((TVec3C*)obj) ) return 1;
    else                              return 0;
  }
}
Double_t TVec3C::MaxCoeff() {
// Returns the maximum of the absolute values of the 3 components
  Double_t d;
  Double_t cp;
  d       = fX.Rho();
  cp      = fY.Rho();
  if (cp>d) d = cp;
  cp      = fZ.Rho();
  if (cp>d) d = cp;
  return d;
}
void TVec3C::Print() const {
// Print vector
  cout << *this << endl;
}
void TVec3C::SetMag(Double_t mag) {
  // Multiply vector in such a way that the new magnitude be |mag|
  const Double_t eps = 1.0e-308;
  const Double_t big = 1.0e+308;
  if (TMath::Abs(mag)<big) {
    Double_t oldmag = Mag();
    if (oldmag < eps) Warning("SetMag","zero vector can't be stretched");
    else {
      SetX((fX/oldmag)*mag);
      SetY((fY/oldmag)*mag);
      SetZ((fZ/oldmag)*mag);
    }
  }
  else Warning("SetMag","Required magnitude too big");
}
void TVec3C::Unit() {
  // "this" becomes a unit vector
  const Double_t un   = 1.0;
  const Double_t eps = 1.0e-308;
  Double_t  tot = Mag();
  if (tot > eps) {
    tot = un/tot;
    fX *= tot;
    fY *= tot;
    fZ *= tot;
  }
  else Warning("Unit","zero vector can't be stretched");
}
TVec3C TVec3C::Unit() const {
  // Returns a unit vector parallel to "this"
  TVec3C v(fX,fY,fZ);
  v.Unit();
  return v;
}
TVec3C &TVec3C::operator*=(Double_t a) {
  // "this" is multiplied by real a
  fX *= a;
  fY *= a;
  fZ *= a;
  return *this;
}
TVec3C &TVec3C::operator*=(TComplex z) {
  // "this" is multiplied by complex a
  fX *= z;
  fY *= z;
  fZ *= z;
  return *this;
}
TVec3C &TVec3C::operator^=(const TVector3 &p) {
  // "this" becomes the cross product of "this" with real vector p
  TVec3C q(*this);
  fX = q.fY*p.Z()-p.Y()*q.fZ;
  fY = q.fZ*p.X()-p.Z()*q.fX;
  fZ = q.fX*p.Y()-p.X()*q.fY;
  return *this;
}
TVec3C &TVec3C::operator^=(const TVec3C &p) {
  // "this" becomes the cross product of "this" with complex vector p
  TVec3C q(*this);
  fX = q.fY*p.Z()-p.Y()*q.fZ;
  fY = q.fZ*p.X()-p.Z()*q.fX;
  fZ = q.fX*p.Y()-p.X()*q.fY;
  return *this;
}
TVec3C TVec3C::operator+(const TVec3C &w) const {
  // Return sum of "this" and w, w complex
  TVec3C v;
  v.fX = fX + w.fX;
  v.fY = fY + w.fY;
  v.fZ = fZ + w.fZ;
  return v;
}
TVec3C TVec3C::operator+(const TVector3 &w) const {
  // Return sum of "this" and w, w real
  TVec3C v;
  v.fX = fX + w.X();
  v.fY = fY + w.Y();
  v.fZ = fZ + w.Z();
  return v;
}
TVec3C TVec3C::operator-(const TVec3C &w) const {
  // Returns "this" - w, w complex
  TVec3C v;
  v.fX = fX - w.fX;
  v.fY = fY - w.fY;
  v.fZ = fZ - w.fZ;
  return v;
}
TVec3C TVec3C::operator-(const TVector3 &w) const {
  // Returns "this" - w, w real
  TVec3C v;
  v.fX = fX - w.X();
  v.fY = fY - w.Y();
  v.fZ = fZ - w.Z();
  return v;
}
TVec3C TVec3C::operator-() const {
  // Allows to put a '-' sign before a vector
  const Double_t zero = 0.0;
  return TVec3C(zero - fX, zero - fY, zero - fZ);
}
TVec3C TVec3C::operator*(Double_t a) const {
  // Returns a vector equal to "this" multiplied by real a
  TVec3C b(*this);
  b *= a;
  return b;
}
TVec3C TVec3C::operator*(const TComplex &z) const {
  // Returns a vector equal to "this" multiplied by complex z
  TVec3C b(*this);
  b *= z;
  return b;
}
TComplex TVec3C::operator*(const TVector3 &v) const {
// Hermitian scalar product Conjugate(v1)*v2  with v2 real
  return TComplex::Conjugate(fX)*v.X() + TComplex::Conjugate(fY)*v.Y() + TComplex::Conjugate(fZ)*v.Z();
}
TComplex TVec3C::operator*(const TVec3C &v) const {
// Hermitian scalar product Conjugate(v1)*v2  with v2 complex
  return TComplex::Conjugate(fX)*v.fX + TComplex::Conjugate(fY)*v.fY + TComplex::Conjugate(fZ)*v.fZ;
}
TComplex TVec3C::operator&(const TVector3 &v) const {
// Usual scalar product v1*v2   with v2 real
  return fX*v.X() + fY*v.Y() + fZ*v.Z();
}
TComplex TVec3C::operator&(const TVec3C &v) const {
// Usual scalar product v1*v2   with v2 complex
  return fX*v.fX + fY*v.fY + fZ*v.fZ;
}
TVec3C TVec3C::operator^(const TVector3 &p) const {
// Returns vector which is cross product of "this" with p,  p real
  TVec3C v(*this);
  v ^= p;
  return v;
}
TVec3C TVec3C::operator^(const TVec3C &p) const {
// Returns vector which is cross product of "this" with p,  p complex
  TVec3C v(*this);
  v ^= p;
  return v;
}
Bool_t TVec3C::operator==(const TVector3& v) const {
// Check whether "this" is equal to v
  const Double_t eps = 1.0e-307;
  Bool_t isequal;
  Double_t d;
  TComplex z;
  z  = v.X() - fX;
  d  = z.Rho2();
  z  = v.Y() - fY;
  d += z.Rho2();
  z  = v.Z() - fZ;
  d += z.Rho2();
  isequal = (d<eps);
  return isequal;
}
Bool_t TVec3C::operator==(const TVec3C& v) const {
// Check whether "this" is equal to v
  const Double_t eps = 1.0e-307;
  Bool_t isequal;
  Double_t d;
  TComplex z;
  z  = v.fX - fX;
  d  = z.Rho2();
  z  = v.fY - fY;
  d += z.Rho2();
  z  = v.fZ - fZ;
  d += z.Rho2();
  isequal = (d<eps);
  return isequal;
}
Bool_t TVec3C::operator!=(const TVector3& v) const {
// Check whether "this" is NOT equal to v
  const Double_t eps = 1.0e-307;
  Bool_t isnotequal;
  Double_t d;
  TComplex z;
  z  = v.X() - fX;
  d  = z.Rho2();
  z  = v.Y() - fY;
  d += z.Rho2();
  z  = v.Z() - fZ;
  d += z.Rho2();
  isnotequal = !(d<eps);
  return isnotequal;
}
Bool_t TVec3C::operator!=(const TVec3C& v) const {
// Check whether "this" is NOT equal to v
  const Double_t eps = 1.0e-307;
  Bool_t isnotequal;
  Double_t d;
  TComplex z;
  z  = v.fX - fX;
  d  = z.Rho2();
  z  = v.fY - fY;
  d += z.Rho2();
  z  = v.fZ - fZ;
  d += z.Rho2();
  isnotequal = !(d<eps);
  return isnotequal;
}
Bool_t TVec3C::operator>(const TVector3 &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Mag2() > v.Mag2());
}
Bool_t TVec3C::operator>(const TVec3C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Mag2() > v.Mag2());
}
Bool_t TVec3C::operator>=(const TVector3 &v) const {
// Check if the magnitude of "this" is greater or equal the magnitude of v
  return (Mag2() >= v.Mag2());
}
Bool_t TVec3C::operator>=(const TVec3C &v) const {
// Check if the magnitude of "this" is greater or equal the magnitude of v
  return (Mag2() >= v.Mag2());
}
Bool_t TVec3C::operator<(const TVector3 &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Mag2() < v.Mag2());
}
Bool_t TVec3C::operator<(const TVec3C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Mag2() < v.Mag2());
}
Bool_t TVec3C::operator<=(const TVector3 &v) const {
// Check if the magnitude of "this" is greater or equal the magnitude of v
  return (Mag2() <= v.Mag2());
}
Bool_t TVec3C::operator<=(const TVec3C &v) const {
// Check if the magnitude of "this" is greater or equal the magnitude of v
  return (Mag2() <= v.Mag2());
}
TComplex TVec3C::operator()(Int_t i) const {
  // Allows accessing vectot components
  i  = TMath::Abs(i);
  i %= 3;
  switch(i) {
  case 0:
    return fX;
  case 1:
    return fY;
  case 2:
    return fZ;
  }
  return fX;
}
TComplex &TVec3C::operator()(Int_t i) {
  // Allows accessing vectot components
  i  = TMath::Abs(i);
  i %= 3;
  switch(i) {
  case 0:
    return fX;
  case 1:
    return fY;
  case 2:
    return fZ;
  }
  return fX;
}
//
//  friend methods
//
TVec3C operator+(const TVector3 &a, const TVec3C &b) {
  // Addition of a real and a complex vector
  return TVec3C(a.X() + b.X(), a.Y() + b.Y(), a.Z() + b.Z());
}
TVec3C operator-(const TVector3 &a, const TVec3C &b) {
  // Subtraction of a complex vector from a real vector
  return TVec3C(a.X() - b.X(), a.Y() - b.Y(), a.Z() - b.Z());
}
TVec3C operator*(Double_t a, const TVec3C &p) {
  // Returns vector which is the multiplication of all components of vector p by real a
  return TVec3C(a*p.X(), a*p.Y(), a*p.Z());
}
TVec3C operator*(const TComplex &z, const TVec3C &p) {
  // Returns vector which is the multiplication of all components of vector p by complex z
  return TVec3C(z*p.X(), z*p.Y(), z*p.Z());
}
TComplex operator*(const TVector3 &a, const TVec3C &b) {
  // Hermitian scalar product Conjugate(a)*b, a real and b complex == usual scalar product
  return a.X()*b.X() + a.Y()*b.Y() + a.Z()*b.Z();
}
TComplex operator&(const TVector3 &a, const TVec3C &b) {
  // Usual scalar product a*b with a real and b complex
  return a.X()*b.X() + a.Y()*b.Y() + a.Z()*b.Z();
}
TVec3C operator^(const TVector3 &a, const TVec3C &b) {
  // Returns cross product of real vector a with complex vector b
  TVec3C v;
  v.fX = a.Y()*b.Z() - a.Z()*b.Y();
  v.fY = a.Z()*b.X() - a.X()*b.Z();
  v.fZ = a.X()*b.Y() - a.Y()*b.X();
  return v;
}
ostream& operator<<(ostream& out, const TVec3C &v) {
  // Allows to do cout << TVector3
  Double_t xr,xi,yr,yi,zr,zi;
  xr = v.X().Re();
  xi = v.X().Im();
  yr = v.Y().Re();
  yi = v.Y().Im();
  zr = v.Z().Re();
  zi = v.Z().Im();
  out << " [";
  //
  out << '(';
  out.width(14);
  out.precision(6);
  out << xr << ", ";
  out.width(14);
  out.precision(6);
  out << xi << "i) ";
  //
  out << '(';
  out.width(14);
  out.precision(6);
  out << yr << ", ";
  out.width(14);
  out.precision(6);
  out << yi << "i) ";
  //
  out << '(';
  out.width(14);
  out.precision(6);
  out << zr << ", ";
  out.width(14);
  out.precision(6);
  out << zi << "i)]";
  return out;
}
istream& operator>>(istream& in, TVec3C &v) {
  // Allows to do cin << TVector3. [6 <cr> are necessary !]
      in >> v.fX >> v.fY >> v.fZ;
      return in;
}
