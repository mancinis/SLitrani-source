// @(#)root/html:$Id: TArrayZ.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TArrayZ.h"

ClassImp(TArrayZ)
//______________________________________________________________________________
//
//     TArrayZ  One dimensional array of complex
//
//   Methods "ReSet" redefines the array completely, all old values are lost
//   Methods "Set" allows to change sizes fN. Preserves the maximum possible
// from the old values.
//   Method Print() to print the array. Operator << can also be used for
// printing the array with cout. It also works within CINT.
//   TArrayZ derives from TObject. They can be put into collections or written
// to a ROOT file. TArrayZ are sortable according to their norm, calculated
// as if they were 1-dimensional.
//   TArrayZ can be added or subtracted. They can be multiplied or divided
// by a number, etc. A scalar product is defined. Look at all the available
// operators.
//
TArrayZ::TArrayZ(const TArrayZ &orig) {
  //copy constructor
  fN = orig.fN;
  fA  = new TComplex [fN];
  for (Int_t i=0;i<fN;i++) fA[i] = orig.fA[i];
}
TArrayZ::TArrayZ(Int_t n1) {
  //constructor for a 1 dimensional array of size n1. Array is put to 0
  const TComplex zero = 0;
  fN   = n1;
  fA    = new TComplex [fN];
  for (Int_t i=0;i<fN;i++) fA[i] = zero;
}
TArrayZ::~TArrayZ() {
  //destructor
  Clean();
}
Double_t TArrayZ::Biggest() const {
  // returns the biggest element in absolute value in form of a double
  Int_t i;
  Double_t d;
  Double_t dmax = 0.0;
  for (i=0;i<fN;i++) {
    d = TComplex::Abs(fA[i]);
    if (d>dmax) dmax = d;
  }
  return dmax;
}
void TArrayZ::Clean() {
  //
  if (fA) delete [] fA;
  Init();
}
Int_t TArrayZ::Compare(const TObject* obj) const {
  // Compare according to norm
  Int_t k;
  Double_t num1, num2;
  TArrayZ *p;
  p = (TArrayZ *)obj;
  num1 = Norm();
  num2 = p->Norm();
  if (num1<num2) k=-1;
  else {
    if (num1>num2) k=1;
    else k=0;
  }
  return k;
}
void TArrayZ::Copy(const TArrayZ &orig) {
  //copy orig into this
  Clean();
  fN = orig.fN;
  fA  = new TComplex [fN];
  for (Int_t i=0;i<fN;i++) fA[i] = orig.fA[i];
}
Bool_t TArrayZ::IsEqual(const TObject *obj) const {
  // 2 arrays are equal if they are compatible and if all elements are
  //equal up to 1.0e-10 of the biggest norm
  const Double_t eps = 1.0e-10;
  Bool_t equal = kFALSE;
  Double_t norm,norm1,norm2;
  TArrayZ *p;
  p = (TArrayZ *)obj;
  if (IsCompatibleWith(*p)) {
    Int_t k;
    Double_t d;
    TComplex z;
    norm1 = Norm();
    norm2 = p->Norm();
    norm  = TMath::Max(norm1,norm2);
    equal = kTRUE;
    k     = 0;
    while ((equal) && (k<fN)) {
      z  = fA[k] - p->fA[k];
      d  = TComplex::Abs(z)/norm;
      if (d>eps) equal = kFALSE;
      k++;
    }
  }
  return equal;
}
void TArrayZ::Init() {
  //Initialization
  fN  = 0;
  fA   = 0;
}
Double_t TArrayZ::Norm() const {
  // Calculates the norm [scalar product with itself] of the array
  Int_t k;
  TComplex z,z1,z2;
  Double_t norm = 0.0;
  for (k=0;k<fN;k++) {
    z1    = fA[k];
    z2    = TComplex::Conjugate(z1);
    z     = z1*z2;
    norm += z.Re();
  }
  return norm;
}
void TArrayZ::Print() const {
  cout << *this << endl;
}
ostream& TArrayZ::Print(ostream& os) const {
  // Print the whole n multidimensionnal array
  Int_t i1;
  os << endl;
  os << "Nb. of elements : " << fN << endl;
  os << endl;
  for (i1=0;i1<fN;i1++) {
    os << "A(";
    os.width(4);
    os << i1 << ") =  ";
    os << fA[i1] << endl;
  }
  return os;
}
void TArrayZ::ReSet(Int_t n1) {
  //Reset this to be 1 dimensional of dimension n1
  const TComplex zero = 0;
  Clean();
  fN   = n1;
  fA    = new TComplex [fN];
  for (Int_t i=0;i<fN;i++) fA[i] = zero;
}
void TArrayZ::Set(Int_t n1) {
  // Gives the new size n1. The element of the old array are copied to the new array
  // up to element n1. Other elements are lost.
  Int_t i1,N1;
  if (fA) {
    TArrayZ old(*this);
    ReSet(n1);
    N1 = TMath::Min(old.fN,fN);
    for (i1=0;i1<N1;i1++) (*this)[i1] = old[i1];
  }
  else ReSet(n1);
}
TArrayZ &TArrayZ::operator=(const TArrayZ &orig) {
  // TArrayZ assignment operator.
  if (this != &orig) {
    Clean();
    Copy(orig);
  }
  return *this;
}
TArrayZ &TArrayZ::operator+=(TComplex val) {
  // Add val to every element of the array
  Int_t i;
  for (i=0;i<fN;i++) fA[i] += val;
  return *this;
}
TArrayZ &TArrayZ::operator+=(const TArrayZ &B) {
  // Add array B if B compatible with *this. Else do nothing
  if (IsCompatibleWith(B)) {
    Int_t i;
    for (i=0;i<fN;i++) fA[i] += B.fA[i];
  }
  return *this;
}
TArrayZ &TArrayZ::operator-=(TComplex val) {
  // Subtract val to every element of the array
  Int_t i;
  for (i=0;i<fN;i++) fA[i] -= val;
  return *this;
}
TArrayZ &TArrayZ::operator-=(const TArrayZ &B) {
  // Subtract array B if B compatible with *this. Else do nothing
  if (IsCompatibleWith(B)) {
    Int_t i;
    for (i=0;i<fN;i++) fA[i] -= B.fA[i];
  }
  return *this;
}
TArrayZ &TArrayZ::operator*=(TComplex val) {
  // Multiply every element of the array with val
  Int_t i;
  for (i=0;i<fN;i++) fA[i] *= val;
  return *this;
}
TArrayZ &TArrayZ::operator/=(TComplex val) {
  // Divide every element of the array with val
  Int_t i;
  for (i=0;i<fN;i++) fA[i] /= val;
  return *this;
}
Bool_t TArrayZ::operator<(const TArrayZ &B) const {
  // Is the norm of *this smaller than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1<num2);
}
Bool_t TArrayZ::operator<=(const TArrayZ &B) const {
  // Is the norm of *this smaller or equal than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1<=num2);
}
Bool_t TArrayZ::operator>(const TArrayZ &B) const {
  // Is the norm of *this greater than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1>num2);
}
Bool_t TArrayZ::operator>=(const TArrayZ &B) const {
  // Is the norm of *this greater or equal than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1>=num2);
}
TComplex &TArrayZ::operator[](Int_t i) {
   if (i>=fN) i = 0;
   return fA[i];
}
TComplex TArrayZ::operator[](Int_t i) const {
   if (i>=fN) return 0;
   return fA[i];
}
TArrayZ operator+(const TArrayZ &A,const TArrayZ &B) {
  // Return A+B if both are compatible. If not compatible, returns A
  TArrayZ C(A);
  C += B;
  return C;
}
TArrayZ operator-(const TArrayZ &A,const TArrayZ &B) {
  // Return A-B if both are compatible. If not compatible, returns A
  TArrayZ C(A);
  C -= B;
  return C;
}
TComplex operator*(const TArrayZ &A,const TArrayZ &B) {
  // Calculates scalar product with complex conjugate if same size, else returns 0.
  TComplex z = 0;
  if (A.IsCompatibleWith(B)) {
    Int_t k;
    TComplex zz,z1,z2;
    for (k=0;k<A.fN;k++) {
      z1 = A.GetA(k);
      z2 = B.GetA(k);
      zz = z1*TComplex::Conjugate(z2);
      z += zz;
    }
  }
  return z;
}
ostream &operator<<(ostream &os,const TArrayZ &c) {
  //Allows to output the array with cout
  return c.Print(os);
}
