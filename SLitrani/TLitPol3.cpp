// @(#)root/html:$Id: TLitPol3.cxx 2008-4-7
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TCleanOut.h"
#include "TLitPol3.h"

ClassImp(TLitPol3)
//________________________________________________________________________
//
//  Handling of 3rd degree polynomials
//
//  The given polynomial
//
//   (1) - f(x) = ax^3 + bx^2 + cx + d
//
// is stored after translation fXt=-b/3a and division by a as:
//
//   (2) - f(x') = x'^3 + px' + q          |  x' = x - fXt
//
//  If ((|a|==0) && (|b|!=0)), the given polynomial
//
//   (3) - f(x) = bx^2 + cx + d
//
// is stored after translation fXt=-c/2b and division by b as:
//
//   (4) - f(x') = x'^2 + q                |  x' = x - fXt
//
//  If ((|a|==0) && (|b|==0) && (|c|!=0)), the given polynomial
//
//   (5) - f(x) = cx + d
//
// is stored after translation fXt=-d/c and division by c as:
//
//   (6) - f(x') = x'                      |  x' = x - fXt
//
//  If ((|a|==0) && (|b|==0) && (|c|==0)), the given polynomial
//
//   (7) - f(x) = d
//
// is stored without translation fXt=0 and with division by d as:
//
//   (8) - f(x') = 1                       |  x' = x
//
//
//  If P is of type TLitPol3, the value of P at x can be obtained either by:
//
//   y = P.Y(x);     or simpler by    y = P(X);
//
//  The value of the normalized polynomial at x' is obtained internally
// [Yn() is private] by:
//
//   y' = P.Yn(x');   // If x' = x - fXt, we have then y = fAn*y'
//
//  The value of the nth derivative of P at x is obtained by:
//
//   y = P.Y(n,x);   or simpler by   y = P(n,x);
//
//  You can retrieve the original coefficients (a,b,c,d) with method
// Getabcd().
//  You can print the polynomial by method Print().
//  To find the roots of the polynomial, call method FindRoots(). This method
// uses the analytical method, no Newton iteration is used.
//  Method GetERoot(k) returns fERoots[k] which is the absolute value of the
// polynomial at fRoots[k], and can be taken as the error on fRoots[k] since
// it should be 0.
//  You can print the 4 roots of the polynomial using method PrintRoots().
//  TLitPol3 inherits from TObject. So you can store it into root collections or
// into a ROOT file. Notice in the later case that the variables defining
// the polynomial are stored into the ROOT file, but not the variables concerning
// the found roots. If you want those variables defined after the reading of
// the polynomial from the ROOT file, you have to call FindRoots().
//
TLitPol3::TLitPol3(Double_t a,Double_t b,Double_t c,Double_t d) {
  // Constructor of polynomial: ax^3 + bx^2 + cx + d
  InitP();
  Set(a,b,c,d);
}
TLitPol3::TLitPol3(TLitPol3 &P) {
  // Copy constructor
  Short_t k;
  fNd  = P.fNd;
  fAn  = P.fAn;
  fXt  = P.fXt;
  fP   = P.fP;
  fQ   = P.fQ;
  fMag = P.fMag;
  fNr  = P.fNr;
  for (k=0;k<3;k++) {
    fRoots[k]  = P.fRoots[k];
    fERoots[k] = P.fERoots[k];
  }
}
TLitPol3::~TLitPol3() {
  // Destructor
}
Int_t TLitPol3::Compare(const TObject *obj) const {
  // Ordering according to fAn [why not ?]
  TLitPol3 *P;
  P = (TLitPol3*)obj;
  if (fAn < P->fAn) return -1;
  else if (fAn > P->fAn) return 1;
  else return 0;
}
Double_t TLitPol3::FindRoots(Double_t small) {
  // Finds roots using tha analytical method. Returns sum of errors on all roots
  // if small != 0.0, cut imaginary parts below small
  Double_t residue = 0.0;
  Short_t k,nc;
  Double_t qs2, ps3,d1,d2,d3;
  switch (fNd) {
    case 0:
      fNr = 0;
      break;
    case 1:
      fNr = 1;
      fRoots[0]  = 0.0;
      fERoots[0] = TMath::Abs(Yn(0.0));
      break;
    case 2:
      if (fQ>=0.0) {
        fNr = 0;
        d1  = TMath::Sqrt(fQ);
        fRoots[0](0.0,-d1);
        fERoots[0] = TComplex::Abs(Yn(fRoots[0]));
        fRoots[1](0.0,d1);
        fERoots[1] = TComplex::Abs(Yn(fRoots[1]));
      }
      else {
        fNr = 2;
        d1  = TMath::Sqrt(-fQ);
        fRoots[0] = -d1;
        fERoots[0] = TMath::Abs(Yn(-d1));
        fRoots[1] =  d1;
        fERoots[1] = TMath::Abs(Yn(d1));
      }
      break;
    case 3:
      qs2 = fQ/2.0;
      ps3 = fP/3.0;
      d1  = qs2*qs2+ps3*ps3*ps3;
      if (d1>=0.0) {
        const Double_t uns3 = 1.0/3.0;
        Double_t u,v;
        d1  = TMath::Sqrt(d1);
        d2  = -qs2 + d1;
        if (d2>=0.0) {
          d2  = TMath::Log(d2);
          u   = TMath::Exp(uns3*d2);
        }
        else {
          d2  = TMath::Log(-d2);
          u   = -TMath::Exp(uns3*d2);
        }
        d3  = -qs2 - d1;
        if (d3>=0.0) {
          d3  = TMath::Log(d3);
          v   = TMath::Exp(uns3*d3);
        }
        else {
          d3  = TMath::Log(-d3);
          v   = -TMath::Exp(uns3*d3);
        }
        d1  = u+v;
        d2  = -d1/2.0;
        d3  = TMath::Abs(((u-v)*TMath::Sqrt(3.0))/2.0);
        fNr = 1;
        fRoots[0]  = d1;
        fERoots[0] = TMath::Abs(Yn(d1));
        fRoots[1](d2,-d3);
        fERoots[1] = TComplex::Abs(Yn(fRoots[1]));
        fRoots[2](d2,d3);
        fERoots[2] = TComplex::Abs(Yn(fRoots[2]));
      }
      else {
        Double_t p,cphi,phi,y[3];
        p    = -fP;
        ps3  = p/3.0;
        cphi = -qs2/TMath::Sqrt(ps3*ps3*ps3);
        phi  = TMath::ACos(cphi);
        d1   = phi/3.0;
        d2   = TMath::Pi()/3.0;
        d3   = d2+d1;
        d2  -= d1;
        ps3  = TMath::Sqrt(ps3);
        y[0] =  2.0*ps3*TMath::Cos(d1);
        y[1] = -2.0*ps3*TMath::Cos(d3);
        y[2] = -2.0*ps3*TMath::Cos(d2);
        Order(y);
        fNr  = 3;
        for (k=0;k<3;k++) {
          fRoots[k] = y[k];
          fERoots[k] = TMath::Abs(Yn(y[k]));
        }
      }
      break;
  }
  Mag2();
  if ((small>0.0) && (fNr<3)) NotSmallIm(small);
  for (k=0;k<fNr;k++) {
    residue += fERoots[k];
    fRoots[k] += fXt;
  }
  nc = GetNc();
  for (k=fNr;k<fNr+nc;k++) {
    residue += fERoots[k];
    fRoots[k] += fXt;
  }
  return residue;
}
void TLitPol3::Getabcd(Double_t &a,Double_t &b,Double_t &c,Double_t &d) const {
  // Retrieves coefficients of polynomial from the coefficients of the
  // normalized polynomial
  a = fAn;
  b = -3*fXt*fAn;
  c = fAn*(3*fXt*fXt+fP);
  d = fAn*(fQ-fP*fXt-fXt*fXt*fXt);
}
Double_t TLitPol3::GetERoot(Short_t k) const {
  const Double_t big = 1.0e+100;
  Double_t y =big;
  if ((k>=0) && (k<3)) y = fERoots[k];
  return y;
}
Short_t TLitPol3::GetNc() const {
  //  Returns the number of complex roots, always even: complex conjugate
  // pairs are counted as 2. Returns -1 if roots have not yet been found
  Short_t nc = -1;
  if (fNr>=0) {
    switch (fNd) {
      case 0:
      case 1:
        nc = 0;
        break;
      case 2:
        if (fNr) nc = 0;
        else     nc = 2;
        break;
      case 3:
        if (fNr==3) nc = 0;
        else        nc = 2;
        break;
    }
  }
  return nc;
}
TComplex TLitPol3::GetRoot(Short_t k) const {
  const Double_t big = 1.0e+100;
  TComplex z(big,big);
  if ((k>=0) && (k<3)) z = fRoots[k];
  return z;
}
void TLitPol3::InitP() {
  // Not stored values initialized
  const Double_t zero = 0.0;
  Short_t k;
  fMag  = zero;
  fNr   = -1;
  for (k=0;k<3;k++) {
    fRoots[k]  = zero;
    fERoots[k] = zero;
  }
}
Bool_t TLitPol3::IsComplex(Short_t k) const {
  // Returns true if root is complex
  const Double_t eps = 1.0e-100;
  Bool_t cplx = kFALSE;
  if ((k>=0) && (k<3)) {
    cplx = (TMath::Abs(fRoots[k].Im())>eps);
  }
  else gCleanOut->MMI(error,"IsComplex","Only 3 roots 0<=k<3","k",k,ClassName());
  return cplx;
}
Bool_t TLitPol3::IsComplex(Short_t k,Double_t lim) const {
  // Returns true if the absolute value of the imaginary part of root k is bigger
  //then lim. Does not touch at the roots.
  Bool_t cplx = kFALSE;
  if ((k>=0) && (k<3)) {
    cplx = (TMath::Abs(fRoots[k].Im())>lim);
  }
  else gCleanOut->MMI(error,"IsComplex","Only 3 roots 0<=k<3","k",k,ClassName());
  return cplx;
}
Bool_t TLitPol3::IsEqual(const TObject *obj) const {
  // 2 TLitPol3 are equal if all there coefficients are equal
  const Double_t eps = 1.0e-20;
  Bool_t equal = kFALSE;
  Double_t mag,d;
  TLitPol3 *P;
  P = (TLitPol3*)obj;
  if (fNd==P->fNd) {
    d   = 0.0;
    mag = TMath::Abs(fAn)+TMath::Abs(fXt)+TMath::Abs(fP)+TMath::Abs(fQ);
    d  += TMath::Abs(fAn - P->fAn);
    d  += TMath::Abs(fXt - P->fXt);
    d  += TMath::Abs(fP  - P->fP);
    d  += TMath::Abs(fQ  - P->fQ);
    d  /= mag;
    equal = d<eps;
  }
  return equal;
}
void TLitPol3::Mag1(Double_t a,Double_t b,Double_t c,Double_t d) {
  // Set fMag as magnitude of coefficients = |a|+|b|+|c|+|d|
  Double_t aa,ab,ac,ad;
  aa = TMath::Abs(a);
  ab = TMath::Abs(b);
  ac = TMath::Abs(c);
  ad = TMath::Abs(d);
  fMag = aa+ab+ac+ad;
}
void TLitPol3::Mag2() {
  // Set fMag as sum of abs value of roots of normalized polynomial
  Short_t k;
  fMag = 0.0;
  for (k=0;k<3;k++) fMag += fRoots[k].Rho();
}
void TLitPol3::NotSmallIm(Double_t small) {
  // Put very small imaginary parts to 0
  Short_t k;
  for (k=0;k<3;k++) {
    if (TMath::Abs(fRoots[k].Im())<small) {
      fRoots[k](fRoots[k].Re(),0.0);
    }
  }
  fNr = 3;
  for (k=0;k<3;k++) {
    if (TMath::Abs(fRoots[k].Im())>=small) fNr--;
  }
}
void TLitPol3::Order(Double_t *y) const {
  // Put in increasing order
  Bool_t ok = kFALSE;
  Short_t k,k1;
  Double_t d;
  while (!ok) {
    ok = kTRUE;
    for (k=0;k<2;k++) {
      k1 = k+1;
      if (y[k]>y[k1]) {
        ok = kFALSE;
        d = y[k1];
        y[k1] = y[k];
        y[k]  = d;
      }
    }
  }
}
void TLitPol3::Print() const {
  // Prints normalized polynomial
  Double_t a,b,c,d;
  Getabcd(a,b,c,d);
  cout << endl;
  cout << "  p(z) == az^3 + bz^2 + cz + d" << endl;
  cout << "  a  = ";
  cout.width(20);
  cout.precision(12);
  cout << a << "     b  = ";
  cout.width(20);
  cout.precision(12);
  cout << b << endl;
  cout << "  c  = ";
  cout.width(20);
  cout.precision(12);
  cout << c << "     d  = ";
  cout.width(20);
  cout.precision(12);
  cout << d << endl;
  cout << endl;
  cout << "  pn(z') == z'^3 + p*z' + q" << endl;
  cout << "  z' = z - xt          p(z) = an*pn(z')" << endl;
  cout << "  an = ";
  cout.width(20);
  cout.precision(12);
  cout << fAn << "     xt = ";
  cout.width(20);
  cout.precision(12);
  cout << fXt << endl;
  cout << "  p  = ";
  cout.width(20);
  cout.precision(12);
  cout << fP << "     q  = ";
  cout.width(20);
  cout.precision(12);
  cout << fQ << endl;
}
void TLitPol3::PrintRoots() const {
  // Print the found roots
  const char *met = "PrintRoots";
  if (fNr<0) {
    gCleanOut->MM(info,met,"Roots not yet found",ClassName());
  }
  else {
    Short_t k,Nc;
    Nc = GetNc();
    cout << endl;
    gCleanOut->MM(info,met,"Roots of polynomial",ClassName());
    cout << endl;
    gCleanOut->MMI(info,met,"Number of real    roots","Nr",fNr,ClassName());
    gCleanOut->MMI(info,met,"Number of complex roots","Nc",Nc,ClassName());
    for (k=0;k<fNr;k++)
      gCleanOut->MIR(info,"root",k,"value",fRoots[k].Re());
    for (k=fNr;k<fNr+Nc;k++)
      gCleanOut->MIC(info,"root",k,"value",fRoots[k]);
    for (k=0;k<3;k++)
      gCleanOut->MIR(info,"for root",k,"residue",fERoots[k]);
  }
}
void TLitPol3::Set(Double_t a,Double_t b,Double_t c,Double_t d) {
  // Defines polynomial: ax^3 + bx^2 + cx + d
  // Set roots as undefined
  const Double_t eps1 = 1.0e-300;
  const Double_t eps2 = 1.0e-12;
  const Double_t big  = 1.0e+100;
  Short_t k;
  Double_t xt2;
  Double_t aa,ab,ac,ad;
  aa = TMath::Abs(a);
  ab = TMath::Abs(b);
  ac = TMath::Abs(c);
  ad = TMath::Abs(d);
  fMag = aa+ab+ac+ad;
  if (fMag<eps1) {
    fNd = 0; fAn=0.0; fXt=0.0; fP=0.0; fQ=0.0; fNr = 0;
  }
  else {
    if ((aa/fMag)>=eps2) {
      //polynomial of 3rd degree
      fNd = 3;
      b  /= a;
      c  /= a;
      d  /= a;
      fAn = a;
      fXt = -b/3.0;
      xt2 = fXt*fXt;
      fP  = 3*xt2+2*b*fXt + c;
      fQ  = fXt*xt2+b*xt2+c*fXt+d;
    }
    else {
      //polynomial less than 3rd degree
      if ((ab/fMag)>=eps2) {
        //polynomial of 2nd degree
        fNd = 2;
        c  /= b;
        d  /= b;
        fAn = b;
        fXt = -c/2.0;
        fP  = 0.0;
        fQ  = fXt*fXt+c*fXt+d;
      }
      else {
        //polynomial less than 2nd degree
        if ((ac/fMag)>=eps2) {
          //polynomial of 1st degree
          fNd = 1;
          d  /= c;
          fAn = c;
          fXt = -d;
          fP  = 0.0;
          fQ  = 0.0;
        }
        else {
          //polynomial of 0 degree
          fNd = 0;
          fAn = d;
          fXt = 0.0;
          fP  = 0.0;
          fQ  = 0.0;
        }
      }
    }
  }
  fNr = -1;
  for (k=0;k<3;k++) {
    fRoots[k](big,big);
    fERoots[k] = big;
  }
}
Double_t TLitPol3::Y(Double_t x) const {
  //  Calculation of polynomial at real x
  Double_t y = 0.0;
  Double_t xp;
  xp = x - fXt;
  switch (fNd) {
    case 0:
      y = fAn;
      break;
    case 1:
      y = fAn*xp;
      break;
    case 2:
      y = fAn*(xp*xp+fQ);
      break;
    case 3:
      y = fAn*(xp*xp*xp+fP*xp+fQ);
      break;
  }
  return y;
}
TComplex TLitPol3::Y(TComplex z) const {
  //  Calculation of polynomial at complex z
  TComplex y,xp;
  xp = z - fXt;
  switch (fNd) {
    case 0:
      y = fAn;
      break;
    case 1:
      y = fAn*xp;
      break;
    case 2:
      y = fAn*(xp*xp+fQ);
      break;
    case 3:
      y = fAn*(xp*xp*xp+fP*xp+fQ);
      break;
  }
  return y;
}
Double_t TLitPol3::Y(Int_t n,Double_t x) const {
  //  Calculation of nth derivative at real x
  Double_t y = 0.0;
  Double_t xp;
  n  = TMath::Abs(n);
  xp = x - fXt;
  switch (n) {
    case 0:
      y = Y(x);
      break;
    case 1:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = fAn;
          break;
        case 2:
          y = 2*fAn*xp;
          break;
        case 3:
          y = fAn*(3*xp*xp+fP);
          break;
      }
      break;
    case 2:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 2*fAn;
          break;
        case 3:
          y = 6*fAn*xp;
          break;
      }
      break;
    case 3:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 0.0;
          break;
        case 3:
          y = 6*fAn;
          break;
      }
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
TComplex TLitPol3::Y(Int_t n,TComplex z) const {
  //  Calculation of nth derivative at complex z
  TComplex y,xp;
  n  = TMath::Abs(n);
  xp = z - fXt;
  switch (n) {
    case 0:
      y = Y(z);
      break;
    case 1:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = fAn;
          break;
        case 2:
          y = 2*fAn*xp;
          break;
        case 3:
          y = fAn*(3.0*xp*xp+fP);
          break;
      }
      break;
    case 2:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 2*fAn;
          break;
        case 3:
          y = 6.0*fAn*xp;
          break;
      }
      break;
    case 3:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 0.0;
          break;
        case 3:
          y = 6*fAn;
          break;
      }
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
Double_t TLitPol3::Yn(Double_t xp) const {
  //  Calculation of normalized polynomial at real xp
  Double_t y = 0.0;
  switch (fNd) {
    case 0:
      y = 1.0;
      break;
    case 1:
      y = xp;
      break;
    case 2:
      y = xp*xp+fQ;
      break;
    case 3:
      y = xp*xp*xp+fP*xp+fQ;
      break;
  }
  return y;
}
TComplex TLitPol3::Yn(TComplex zp) const {
  //  Calculation of normalized polynomial at complex zp
  TComplex y;
  switch (fNd) {
    case 0:
      y = 1.0;
      break;
    case 1:
      y = zp;
      break;
    case 2:
      y = zp*zp+fQ;
      break;
    case 3:
      y = zp*zp*zp+fP*zp+fQ;
      break;
  }
  return y;
}
Double_t TLitPol3::Yn(Int_t n,Double_t xp) const {
  //  Calculation of nth derivative of normalized polynomial at real x
  Double_t y = 0.0;
  n  = TMath::Abs(n);
  switch (n) {
    case 0:
      y = Yn(xp);
      break;
    case 1:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 1.0;
          break;
        case 2:
          y = 2*xp;
          break;
        case 3:
          y = 3*xp*xp+fP;
          break;
      }
      break;
    case 2:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 2.0;
          break;
        case 3:
          y = 6*xp;
          break;
      }
      break;
    case 3:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 0.0;
          break;
        case 3:
          y = 6.0;
          break;
      }
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
TComplex TLitPol3::Yn(Int_t n,TComplex zp) const {
  //  Calculation of nth derivative of normalized polynomial at complex zp
  TComplex y;
  n  = TMath::Abs(n);
  switch (n) {
    case 0:
      y = Yn(zp);
      break;
    case 1:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 1.0;
          break;
        case 2:
          y = 2.0*zp;
          break;
        case 3:
          y = 3.0*zp*zp+fP;
          break;
      }
      break;
    case 2:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 2.0;
          break;
        case 3:
          y = 6.0*zp;
          break;
      }
      break;
    case 3:
      switch (fNd) {
        case 0:
          y = 0.0;
          break;
        case 1:
          y = 0.0;
          break;
        case 2:
          y = 0.0;
          break;
        case 3:
          y = 6.0;
          break;
      }
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
Double_t TLitPol3::operator()(Double_t x) const {
  return Y(x);
}
TComplex TLitPol3::operator()(TComplex x) const {
  return Y(x);
}
Double_t TLitPol3::operator()(Int_t k,Double_t x) const {
  return Y(k,x);
}
TComplex TLitPol3::operator()(Int_t k,TComplex x) const {
  return Y(k,x);
}
TLitPol3 &TLitPol3::operator=(const TLitPol3 &P) {
  // TLitPol3 assignment operator.
  if (this != &P) {
    Short_t k;
    fNd  = P.fNd;
    fAn  = P.fAn;
    fXt  = P.fXt;
    fP   = P.fP;
    fQ   = P.fQ;
    fMag = P.fMag;
    fNr  = P.fNr;
    for (k=0;k<3;k++) {
      fRoots[k]  = P.fRoots[k];
      fERoots[k] = P.fERoots[k];
    }
  }
  return *this;
}
Bool_t TLitPol3::operator==(const TLitPol3 &P) {
  return IsEqual(&P);
}
