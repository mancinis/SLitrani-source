// @(#)root/html:$Id: TLitPol4.cxx 2008-7-7
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*********************************************************************************
 * Copyright The same as the ones of ROOT                                        *
*********************************************************************************/
#include <complex>
#include <vector>
#include "TMath.h"
#include "Math/Polynomial.h"
#include "TCleanOut.h"
#include "TLitPol3.h"
#include "TLitPol4.h"

ClassImp(TLitPol4)
//________________________________________________________________________
//
//  Handling  of 4th degree polynomials
//
//  if (|a| >= 1.0e-12*fMag), the given polynomial
//
//   (1) - f(x) = ax^4 + bx^3 + cx^2 + dx + e
//
// is stored after translation fXt=-b/4a and division by a as:
//
//   (2) - f(x') = x'^4 + px'^2 + qx + r          |  x' = x - fXt
//
//  fMag = |a|+|b|+|c|+|d|+|e|
//
//  Condition  |a| >= 1.0e-12*fMag is imperative. If |a| is strictly 0
// program is stopped with a message. If |a| is not strictly 0, but
// |a| < 1.0e-12*fMag an error message is issued, but program continues
//
//  If P is of type TLitPol4, the value of P at x can be obtained either by:
//
//   y = P.Y(x);     or simpler by    y = P(x);
//
//  The value of the normalized polynomial (2) at x' is obtained internally
// [Yn() is private] by:
//
//   y' = P.Yn(x');   // If x' = x - fXt, we have then y = fAn*y'
//
//  The value of the nth derivative of P at x is obtained by:
//
//   y = P.Y(n,x);   or simpler by   y = P(n,x);
//
//  You can retrieve the original coefficients (a,b,c,d,e) with method
// Getabcde().
//  You can print the polynomial by method Print().
//  To find the roots of the polynomial, call method FindRoots() or FindRootsRMP()
// FindRoots finds the roots without the help of Root::Math::Polynomial, but using
// various Newton methods for finding the first root. Then TLitPol3 is called to
// find the 3 other roots analytically.
//  The interest of FindRootRMP() is that it uses Root::Math::Polynomial to find
// the 4 roots analytically. But as the precision is not very good by this method
// FindRootsRMP() is often obliged to better it using Newton!
//  Method GetERoot(k) returns fERoots[k] which is the absolute value of the
// polynomial at fRoots[k], and can be taken as the error on fRoots[k] since
// it should be 0.
//  You can print the 4 roots of the polynomial using method PrintRoots().
//  TLitPol4 inherits from TObject. So you can store it into root collections or
// into a ROOT file. Notice in the later case that the variables defining
// the polynomial are stored into the ROOT file, but not the variables concerning
// the found roots. If you want those variables defined after the reading of
// the polynomial from the ROOT file, you have to call FindRootsRMP() or
// FindRoots().
//
TLitPol4::TLitPol4(Double_t a,Double_t b,Double_t c,Double_t d,Double_t e) {
  // Constructor of polynomial: ax^4 + bx^3 + cx^2 + dx + e
  InitP();
  Set(a,b,c,d,e);
}
TLitPol4::TLitPol4(TLitPol4 &P) {
  // Copy constructor
  Short_t k;
  InitP();
  fAn   = P.fAn;
  fXt   = P.fXt;
  fP    = P.fP;
  fQ    = P.fQ;
  fR    = P.fR;
  fMag  = P.fMag;
  fNr   = P.fNr;
  fNe   = P.fNe;
  for (k=0;k<4;k++) {
    if (k<3) fYextr[k] = P.fYextr[k];
    fRoots[k]  = P.fRoots[k];
    fERoots[k] = P.fERoots[k];
  }
}
TLitPol4::~TLitPol4() {
  // Destructor
  ClearP();
}
void TLitPol4::ClearP() {
  // Delete all pointers
  if (fPol3) {
    delete fPol3;
    fPol3 = 0;
  }
}
Int_t TLitPol4::Compare(const TObject *obj) const {
  // Ordering according to fAn [why not ?]
  TLitPol4 *P;
  P = (TLitPol4*)obj;
  if (fAn < P->fAn) return -1;
  else if (fAn > P->fAn) return 1;
  else return 0;
}
void TLitPol4::Derivative() {
  //  Defines fPol3 as the derivative of this normalized polynomial
  const Double_t big  = 1.0e+100;
  const Double_t eps  = 1.0e-8;
  const Double_t eps2 = 5.0e-13;
  Double_t residue,ymin;
  Short_t k,kmin;
  if (fPol3) fPol3->Set(4.0,0.0,2.0*fP,fQ);
  else       fPol3 = new TLitPol3(4.0,0.0,2.0*fP,fQ);
  residue = fPol3->FindRoots();
  if (residue>eps)
    gCleanOut->MMR(warning,"Derivative","Roots of fPol3 bad","residue",residue,ClassName());
  fNe = fPol3->GetNr();
  kmin = -1;
  ymin = big;
  for (k=0;k<fNe;k++) {
    fYextr[k] = Yn(fPol3->GetRoot(k));
    if (fYextr[k]>0.0) {
      if ((fYextr[k]<eps2) && (fYextr[k]<ymin)) {
        kmin = k;
        ymin = fYextr[k];
      }
    }
  }
  if (kmin>=0) fYextr[kmin] = 0.0;
  for (k=fNe;k<3;k++) fYextr[k] = big;
}
Double_t TLitPol4::FindRoots() {
  //  Finds roots of polynomial using Newton method for the first root, with a
  // starting z value given by method Starting(). The analytical method of
  // ROOT::Math::Polynomial is not used.
  const Double_t big = 1.0e+100;
  Double_t res = big;          //Sum of residues of all roots
  Bool_t hasconverged = kFALSE;
  Short_t k,k1,nr3;
  Bool_t iscomplex;
  Double_t x,y,x2,r3,a,b,rn;
  Double_t A,B,C;
  TLitPol3 P3;
  TComplex z;
  Derivative();
  iscomplex = Starting(z);
  if (iscomplex) {
    hasconverged = Newton(z,rn);
    if (hasconverged) {
      fNr = 0;
      x = z.Re();
      y = TMath::Abs(z.Im());
      fRoots[0](x,-y);
      fRoots[1](x,y);
      z = fRoots[0];
      a = z.Re();
      b = z.Im();
      z = Yn(z);
      fERoots[0] = z.Rho();
      z = fRoots[1];
      z = Yn(z);
      fERoots[1] = z.Rho();
      B = 2*a;
      C = fP+3*a*a-b*b;
      A = B*B-4*C;
      if (A>=0.0) {
        A = TMath::Sqrt(A);
        x = 0.5*(-B-A);
        fRoots[2]  = x;
        fERoots[2] = TMath::Abs(Yn(x));
        x = 0.5*(-B+A);
        fRoots[3]  = x;
        fERoots[3] = TMath::Abs(Yn(x));
        fNr += 2;
      }//end if (A>=0.0)
      else {
        A = TMath::Sqrt(-A);
        a = -0.5*B;
        b = -0.5*A;
        z(a,b);
        fRoots[2] = z;
        z = Yn(z);
        fERoots[2] = z.Rho();
        z(a,-b);
        fRoots[3] = z;
        z = Yn(z);
        fERoots[3] = z.Rho();
      }//end else if (A>=0.0)
    }//end if (hasconverged)
  }//end if (iscomplex)
  else {
    fNr = 1;
    x   = z.Re();
    hasconverged = Newton(x,rn);
    if (hasconverged) {
      fRoots[0] = x;
      fERoots[0] = rn;
      x2  = x*x;
      A   = x;
      B   = fP+x2;
      C   = fQ+x*(fP+x2);
      P3.Set(1.0,A,B,C);
      r3  = P3.FindRoots();
      nr3 = P3.GetNr();
      fNr += nr3;
      for (k=0;k<nr3;k++) {
        k1 = k+1;
        fRoots[k1] = P3.GetRoot(k);
        x  = fRoots[k1].Re();
        fERoots[k1] = TMath::Abs(Yn(x));
      }
      for (k=nr3;k<3;k++) {
        k1 = k+1;
        z  = P3.GetRoot(k);
        fRoots[k1] = z;
        z  = Yn(z);
        fERoots[k1] = z.Rho();
      }
    }//end if (hasconverged)
  }//end else if (iscomplex)
  if (hasconverged) {
    res = NotSmallIm();
    for (k=0;k<4;k++) fRoots[k] += fXt;
    Order();
  }
  return res;
}
Double_t TLitPol4::FindRootsRMP() {
  //  Finds the roots of the polynomial first using the analytical method
  // of Root::Math::Polynomial [RMP]. If the found roots are not good enough
  // [sum of fERoots[k] > resmax], tries to better the result by using Newton()
  // methods.
  const Double_t big    = 1.0e+100;
  const Double_t eps    = 1.0e-100;
  const Double_t resmax = 5.0e-12;
  const Double_t eps2   = 1.0e-5;
  const UInt_t QNAN       = 0x7ff80000;
  Bool_t hasconverged = kFALSE;
  Bool_t haschanged   = kFALSE;
  Double_t res,res2;
  Short_t k,k1,nr3,kk;
  TComplex z,yz;
  Double_t x,y,x2,r3,a,b,rn,yi;
  Double_t A,B,C;
  TLitPol3 P3;
  Bool_t iscomplex;
  deux u;
  Double_t p[5];
  vector<complex<double> > v(4);
  ROOT::Math::Polynomial BB(4);
  p[4] = 1.0;
  p[3] = 0.0;
  p[2] = fP;
  p[1] = fQ;
  p[0] = fR;
  BB.SetParameters(p);
  v = BB.FindRoots();
  z(v[0].real(),v[0].imag());
  u.DD = z.Re();
  if ((u.II[1]==QNAN) && (u.II[0]==0)) {
    return big;
  }
  fNr  = 0;
  for (k=0;k<4;k++) {
    z(v[k].real(),v[k].imag());
    x = z.Re();
    yi = TMath::Abs(z.Im());
    if ((yi>eps) && (yi<eps2)) {
      haschanged = kTRUE;
      yi         = 0.0;
      fRoots[k]  = x;
    }
    else fRoots[k] = z;
    if (yi<eps) {
      fNr++;
      y = Yn(x);
      fERoots[k] = TMath::Abs(y);
    }
    else {
      yz = Yn(z);
      fERoots[k] = yz.Rho();
    }
  }
  ImAtEnd();
  res = NotSmallIm();
  if (haschanged || (res>resmax)) {
    k = MostIsolated(iscomplex);
    kk = 0;
    while (!hasconverged && kk<2) {
      kk++;
      if (iscomplex) {
        z = fRoots[k];
        hasconverged = Newton(z,rn);
        if (hasconverged) {
          fNr = 0;
          x = z.Re();
          y = TMath::Abs(z.Im());
          fRoots[0](x,-y);
          fRoots[1](x,y);
          z = fRoots[0];
          a = z.Re();
          b = z.Im();
          z = Yn(z);
          fERoots[0] = z.Rho();
          z = fRoots[1];
          z = Yn(z);
          fERoots[1] = z.Rho();
          B = 2*a;
          C = fP+3*a*a-b*b;
          A = B*B-4*C;
          if (A>=0.0) {
            fNr += 2;
            A = TMath::Sqrt(A);
            x = 0.5*(-B-A);
            fRoots[2]  = x;
            fERoots[2] = TMath::Abs(Yn(x));
            x = 0.5*(-B+A);
            fRoots[3]  = x;
            fERoots[3] = TMath::Abs(Yn(x));

          }//end if (A>=0.0)
          else {
            A = TMath::Sqrt(-A);
            a = -0.5*B;
            b = -0.5*A;
            z(a,b);
            fRoots[2] = z;
            z = Yn(z);
            fERoots[2] = z.Rho();
            z(a,-b);
            fRoots[3] = z;
            z = Yn(z);
            fERoots[3] = z.Rho();
          }//end else if (A>=0.0)
        }//end if (hasconverged)
        else {
          iscomplex = kFALSE;
          fRoots[k](fRoots[k].Re(),0.0);
        }
      }//end if (iscomplex)
      else {
        fNr = 1;
        x = fRoots[k].Re();
        hasconverged = Newton(x,rn);
        if (hasconverged) {
          fRoots[0] = x;
          fERoots[0] = rn;
          x2  = x*x;
          A   = x;
          B   = fP+x2;
          C   = fQ+x*(fP+x2);
          P3.Set(1.0,A,B,C);
          r3  = P3.FindRoots();
          nr3 = P3.GetNr();
          fNr += nr3;
          for (k=0;k<nr3;k++) {
            k1 = k+1;
            fRoots[k1] = P3.GetRoot(k);
            x  = fRoots[k1].Re();
            fERoots[k1] = TMath::Abs(Yn(x));
          }
          for (k=nr3;k<3;k++) {
            k1 = k+1;
            z  = P3.GetRoot(k);
            fRoots[k1] = z;
            z  = Yn(z);
            fERoots[k1] = z.Rho();
          }
        }//end if (hasconverged)
        else {
          Double_t zim,gm,ga;
          iscomplex = kTRUE;
          gm        = TMath::Abs(fR);
          gm        = TMath::Exp(0.25*TMath::Log(gm));
          ga        = TMath::Abs(fP);
          ga        = TMath::Sqrt(0.5*ga);
          zim       = 0.25*(gm+ga);
          fRoots[k](fRoots[k].Re(),zim);
        }
      }//end else if (iscomplex)
    }//end while (!hasconverged && kk<2)
    if (hasconverged) res = NotSmallIm();
  }//end if (res>resmax)
  res2 = GetOrig();
  for (k=0;k<4;k++) fRoots[k] += fXt;
  Order();
  res += res2;
  return res;
}
void TLitPol4::Getabcde(Double_t &a,Double_t &b,Double_t &c,Double_t &d,Double_t &e) const {
  // Retrieves coefficients of polynomial from the coefficients of the
  // normalized polynomial
  Double_t xt2;
  xt2 = fXt*fXt;
  a = fAn;
  b = -4*fAn*fXt;
  c = fAn*(6*xt2+fP);
  d = fAn*(fQ-2*fP*fXt-4*xt2*fXt);
  e = fAn*(xt2*xt2+fP*xt2-fQ*fXt+fR);
}
Double_t TLitPol4::GetERoot(Short_t k) const {
  const Double_t big = 1.0e+100;
  Double_t y =big;
  if ((k>=0) && (k<4)) y = fERoots[k];
  return y;
}
Double_t TLitPol4::GetOrig() const {
  //  Retrieves the coefficients of the normalized polynomial starting from
  // the roots. Useful in case a root has been forgotten, and an other root
  // has been taken twice! Returns the sum of tha absolute values of the
  // differences between new and old coefficients. Must be called before
  // the roots have been shifted by fXt.
  Double_t df;
  TComplex b,p,q,r;
  TComplex z0,z1,z2,z3;
  z0 = fRoots[0];
  z1 = fRoots[1];
  z2 = fRoots[2];
  z3 = fRoots[3];
  b  = -(z0+z1+z2+z3);
  p  = z0*z1+z0*z2+z0*z3+z1*z2+z1*z3+z2*z3;
  q  = -(z0*z1*z2 + z0*z1*z3 + z0*z2*z3 + z1*z2*z3);
  r  = z0*z1*z2*z3;
  p -= fP;
  q -= fQ;
  r -= fR;
  df = b.Rho()+p.Rho()+q.Rho()+r.Rho();
  return df;
}
Double_t TLitPol4::GetXextr(Short_t k) const {
  // Get x [in fact xp, translated x] value of extrema k
  const Double_t big = 1.0e+100;
  Double_t xp = big;
  if (fPol3) {
    if ((k>=0) && (k<fNe)) xp = fPol3->GetRoot(k);
  }
  else gCleanOut->MM(warning,"GetXextr","Call Derivative() first",ClassName());
  return xp;
}
Double_t TLitPol4::GetYextr(Short_t k) const {
  // Get y value of normalized polynomial at extrema k
  const Double_t big = 1.0e+100;
  Double_t y = big;
  if ((k>=0) && (k<fNe)) y = fYextr[k];
  return y;
}
Short_t TLitPol4::GetNc() const {
  //  Returns the number of complex roots, always even: complex conjugate
  // pairs are counted as 2. Returns -1 if roots have not yet been found
  Short_t nc = -1;
  const char *met = "GetNc";
  if (fNr>=0) {
    switch (fNr) {
      case 0:
        nc = 4;
        break;
      case 1:
        gCleanOut->MMI(error,met,"Abnormal nb. of real roots","fNr",fNr,ClassName());
        break;
      case 2:
        nc = 2;
        break;
      case 3:
        gCleanOut->MMI(error,met,"Abnormal nb. of real roots","fNr",fNr,ClassName());
        break;
      case 4:
        nc = 0;
        break;
    }
  }
  return nc;
}
TComplex TLitPol4::GetRoot(Short_t k) const {
  const Double_t big = 1.0e+100;
  TComplex z(big,big);
  if ((k>=0) && (k<4)) z = fRoots[k];
  return z;
}
void TLitPol4::ImAtEnd() {
  // Put real roots first
  const Double_t eps = 1.0e-100;
  TComplex z1,z2;
  Double_t r1,r2;
  switch (fNr) {
    case 0:
      break;
    case 2:
      if ((TMath::Abs(fRoots[0].Im())>eps) && (TMath::Abs(fRoots[1].Im())>eps)) {
        z1 = fRoots[0];
        r1 = fERoots[0];
        z2 = fRoots[1];
        r2 = fERoots[1];
        fRoots[0]  = fRoots[2];
        fERoots[0] = fERoots[2];
        fRoots[1]  = fRoots[3];
        fERoots[1] = fERoots[3];
        fRoots[2]  = z1;
        fERoots[2] = r1;
        fRoots[3]  = z2;
        fERoots[3] = r2;
      }
      if ((TMath::Abs(fRoots[1].Im())>eps) && (TMath::Abs(fRoots[2].Im())>eps)) {
        z1 = fRoots[1];
        r1 = fERoots[1];
        fRoots[1]  = fRoots[3];
        fERoots[1] = fERoots[3];
        fRoots[3]  = z1;
        fERoots[3] = r1;
        if (fRoots[3].Im()<fRoots[2].Im()) {
          z1 = fRoots[2];
          r1 = fERoots[2];
          fRoots[2]  = fRoots[3];
          fERoots[2] = fERoots[3];
          fRoots[3]  = z1;
          fERoots[3] = r1;
        }
      }
      break;
    case 4:
      break;
    default:
      gCleanOut->MMI(error,"ImAtEnd","Bad number of real roots","fNr",fNr,ClassName());
      break;
  }
}
void TLitPol4::InitP() {
  // All pointers to 0 and not stored values initialized
  const Double_t zero = 0.0;
  Short_t k;
  fPol3 = 0;
  fMag  = zero;
  fNr   = -1;
  fNe   = -1;
  for (k=0;k<3;k++) fYextr[k] = zero;
  for (k=0;k<4;k++) {
    fRoots[k]  = zero;
    fERoots[k] = zero;
  }
}
Bool_t TLitPol4::IsComplex(Int_t k) const {
  // Returns true if root is complex
  const Double_t eps = 1.0e-100;
  Bool_t cplx = kFALSE;
  if ((k>=0) && (k<4)) {
    cplx = (TMath::Abs(fRoots[k].Im())>eps);
  }
  else gCleanOut->MMI(error,"IsComplex","Only 4 roots 0<=k<4","k",k,ClassName());
  return cplx;
}
Bool_t TLitPol4::IsComplex(Short_t k,Double_t lim) const {
  //  Returns true if the absolute value of the imaginary part of root k is bigger
  // then lim. Does not touch at the roots.
  Bool_t cplx = kFALSE;
  if ((k>=0) && (k<4)) {
    cplx = (TMath::Abs(fRoots[k].Im())>lim);
  }
  else gCleanOut->MMI(error,"IsComplex","Only 4 roots 0<=k<4","k",k,ClassName());
  return cplx;
}
Bool_t TLitPol4::IsEqual(const TObject *obj) const {
  // 2 TLitPol4 are equal if all there coefficients are equal
  const Double_t eps = 1.0e-20;
  Bool_t equal = kFALSE;
  Double_t mag,d;
  TLitPol4 *P;
  P = (TLitPol4*)obj;
  d   = 0.0;
  mag = TMath::Abs(fAn)+TMath::Abs(fXt)+TMath::Abs(fP)+TMath::Abs(fQ)+TMath::Abs(fR);
  d  += TMath::Abs(fAn - P->fAn);
  d  += TMath::Abs(fXt - P->fXt);
  d  += TMath::Abs(fP  - P->fP);
  d  += TMath::Abs(fQ  - P->fQ);
  d  += TMath::Abs(fR  - P->fR);
  d  /= mag;
  equal = d<eps;
  return equal;
}
Short_t TLitPol4::MostIsolated(Bool_t &iscomplex) const {
  // Returns the root most suitable for Newton
  const Double_t big = 1.0e+100;
  Short_t k    = 0;
  Short_t kmax = 0;
  Short_t k1,k2,kmx;
  iscomplex = kFALSE;
  Double_t d,dmin,dmax;
  Double_t lim[4];
  TComplex z;
  switch (fNr) {
    case 0:
      iscomplex = kTRUE;
      break;
    case 2:
      kmax = 2;
      break;
    case 4:
      kmax = 4;
      break;
    default:
      gCleanOut->MMI(error,"MostIsolated","Bad nb. of real roots","fNr",fNr,ClassName());
      break;
  }
  if (kmax>0) {
    for (k1=0;k1<kmax;k1++) {
      dmin = big;
      for (k2=0;k2<4;k2++) {
        if (k1!=k2) {
          z = fRoots[k1]-fRoots[k2];
          d = z.Rho();
          if (d<dmin) dmin = d;
        }
      }
      lim[k1] = dmin;
    }
    dmax = lim[0];
    kmx = 0;
    for (k1=1;k1<kmax;k1++) {
      if (lim[k1]>dmax) {
        dmax = lim[k1];
        kmx = k1;
      }
    }
    k = kmx;
  }
  return k;
}
Bool_t TLitPol4::Newton(Double_t &x,Double_t &res) const {
  //  Tries to find the 1st real root of the normalized polynomial using the
  // usual Newton method, as long as the first derivative is not small. If
  // the first derivative becomes small, switch to Newton method of third order.
  //  The iteration start at x. The result found is returned into x. res is the
  // absolute value of the normalized polynomial at the returned x. Methods returns
  // true if it has converged, false otherwise.
  //
  const Double_t eps   = 1.0e-10; //convergence criterium
  const Double_t eps2  = 0.01;    //lim on abs(yp0) to switch to Newton3
  const Double_t eps3  = 1.0e-5;  //minimum for imaginary part of roots of TLitPol3
  const Int_t    nimax = 1000;    //maximum number of iterations
  const Int_t    niwr  = 3;       //maximum number of iterations getting worse
  const Int_t    suppl = 2;       //nb. of supplementary iterations AFTER convergence
  Bool_t ok = kFALSE;
  Bool_t third;
  Short_t nr3;
  Int_t k,kbad,ksup;
  Double_t x1   = 0.0;
  Double_t y1   = 0.0;
  Double_t res1 = 0.0;
  Double_t x0,x2,x3,y0,y2,y3,yp0,ys0,yt0,res0,res2,res3,r3,x02;
  Double_t B,C,D;
  TLitPol3 P3;
  x0   = x;
  y0   = Yn(x0);
  res0 = TMath::Abs(y0);
  yp0  = Yn(1,x0);
  third = (TMath::Abs(yp0)<eps2);
  if (third) {
    x02 = x0*x0;
    ys0 = Yn(2,x0);
    yt0 = Yn(3,x0);
    B   = 3*(ys0-x0*yt0);
    C   = 6*yp0-6*x0*ys0+3*x02*yt0;
    D   = 6*y0-6*x0*yp0+3*x02*ys0-x02*x0*yt0;
    P3.Set(yt0,B,C,D);
  }
  k    = 0;
  kbad = 0;
  ksup = 0;
  while ((ksup<suppl) && (kbad<niwr) && (k<nimax)) {
    k++;
    if (third) {
      r3  = P3.FindRoots(eps3);
      nr3 = P3.GetNr();
      switch (nr3) {
        case 0:
          // We should never come here
          gCleanOut->MM(error,"Newton","0 Root for Newton3",ClassName());
          x = x0;
          return (res0<eps);;
          break;
        case 1:
          x1   = P3.GetRoot(0);
          y1   = Yn(x1);
          res1 = TMath::Abs(y1);
          break;
        case 2:
          x1   = P3.GetRoot(0);
          y1   = Yn(x1);
          res1 = TMath::Abs(y1);
          x2   = P3.GetRoot(1);
          y2   = Yn(x2);
          res2 = TMath::Abs(y2);
          if (res2<res1) {
            x1   = x2;
            y1   = y2;
            res1 = res2;
          }
          break;
        case 3:
          x1   = P3.GetRoot(0);
          y1   = Yn(x1);
          res1 = TMath::Abs(y1);
          x2   = P3.GetRoot(1);
          y2   = Yn(x2);
          res2 = TMath::Abs(y2);
          x3   = P3.GetRoot(2);
          y3   = Yn(x3);
          res3 = TMath::Abs(y3);
          if (res2<res1) {
            x1   = x2;
            y1   = y2;
            res1 = res2;
          }
          if (res3<res1) {
            x1   = x3;
            y1   = y3;
            res1 = res3;
          }
          break;
      }// end switch (nr3)
    }// end if (third)
    else {
      x1   = x0 - y0/yp0;
      y1   = Yn(x1);
      res1 = TMath::Abs(y1);
    }
    if (res1<eps) ksup++;
    if (res1<res0) {
      x0   = x1;
      y0   = y1;
      res0 = res1;
      yp0  = Yn(1,x0);
    }
    else {
      kbad++;
      x0   = 0.5*(x0+x1);
      y0   = Yn(x0);
      res0 = TMath::Abs(y0);
      yp0  = Yn(1,x0);
    }
    third = (TMath::Abs(yp0)<eps2);
    if (third) {
      x02 = x0*x0;
      ys0 = Yn(2,x0);
      yt0 = Yn(3,x0);
      B   = 3*(ys0-x0*yt0);
      C   = 6*yp0-6*x0*ys0+3*x02*yt0;
      D   = 6*y0-6*x0*yp0+3*x02*ys0-x02*x0*yt0;
      P3.Set(yt0,B,C,D);
    }
  }
  x = x0;
  res = res0;
  if (res0<eps) ok = kTRUE;
  return ok;
}
Bool_t TLitPol4::Newton(TComplex &z,Double_t &res) const {
  //  Tries to find the 1st complex root of the normalized polynomial using
  // a Newton method of 2nd order in the complex plane. Since there are 2
  // values to determine [real and imaginary part], it is clear that 1st order
  // is not suited, so that we use the equation:
  //
  //  f(z1) = f(z0) + (z1-z0)f'(z0) + 0.5*(z1-z0)^2f''(z0) = 0
  //
  // to find the new z1 value. The iteration start at z. The result found is
  // returned into z. res is the absolute value of the normalized polynomial at
  // the returned z. Methods returns true if it has converged, false otherwise.
  //
  const Double_t eps   = 1.0e-10; //convergence criterium
  const Int_t    nimax = 1000;   //maximum number of iterations
  const Int_t    niwr  = 3;      //maximum number of iterations getting worse
  const Int_t    suppl = 2;      //nb. of supplementary iterations AFTER convergence
  Bool_t ok = kFALSE;
  Int_t k,kbad,ksup;
  Double_t res0,res1,res2;
  TComplex z0,z1,z2,y0,y1,y2,yp0,ys0,A,B,C,D;
  z0   = z;
  y0   = Yn(z0);
  yp0  = Yn(1,z0);
  ys0  = Yn(2,z0);
  res0 = y0.Rho();
  k    = 0;
  kbad = 0;
  ksup = 0;
  while ((ksup<suppl) && (kbad<niwr) && (k<nimax)) {
    k++;
    A    = ys0;
    B    = yp0-z0*ys0;
    C    = z0*z0*ys0-2.0*(z0*yp0-y0);
    D    = B*B - A*C;
    D    = TComplex::Sqrt(D);
    z1   = (-B+D)/A;
    z2   = -(B+D)/A;
    y1   = Yn(z1);
    y2   = Yn(z2);
    res1 = y1.Rho();
    res2 = y2.Rho();
    if (res2<res1) {
      z1 = z2;
      y1 = y2;
      res1 = res2;
    }
    if (res1<eps) ksup++;
    if (res1<res0) {
      z0   = z1;
      y0   = y1;
      yp0  = Yn(1,z0);
      ys0  = Yn(2,z0);
      res0 = res1;
    }
    else {
      kbad++;
      z0   = 0.5*(z0+z1);
      y0   = Yn(z0);
      yp0  = Yn(1,z0);
      ys0  = Yn(2,z0);
      res0 = y0.Rho();
    }
  }
  z = z0;
  res = res0;
  if (res0<eps) ok = kTRUE;
  return ok;
}
Double_t TLitPol4::NotSmallIm() {
  //  First change the value of fMag to be the sum of the absolute values of the
  // roots of the normalized polynomial. Then kill imaginary parts whose absolute
  // value is smaller than fMag*eps. To be called while fRoots still contains the
  // roots of the normalized polynomial, not yet the roots of the polynomial!
  const Double_t eps    = 1.3e-8; //eps*fMag = max abs value of imaginary part
  const Double_t resmax = 1.0e-12;
  const char *met = "NotSmallIm";
  Bool_t nochange = kTRUE;
  Short_t k,nrs;
  Double_t x,y,small,res0,res1;
  TComplex z;
  TComplex zs[4];
  Double_t rs[4];
  fMag = 0.0;
  res0 = 0.0;
  for (k=0;k<4;k++) {
    zs[k] = fRoots[k];
    rs[k] = fERoots[k];
    fMag += zs[k].Rho();
    res0 += rs[k];
  }
  if (fMag>=1.0) small = fMag*eps;
  else           small = eps;
  nrs = fNr;
  switch (nrs) {
    case 0:
      if (TMath::Abs(zs[0].Im())<small) {
        nochange = kFALSE;
        z = zs[0];
        x = z.Re();
        zs[0](x,0.0);
        y = Yn(x);
        rs[0] = TMath::Abs(y);
        zs[1](x,0.0);
        rs[1] = TMath::Abs(y);
        nrs += 2;
      }
      if (TMath::Abs(zs[2].Im())<small) {
        nochange = kFALSE;
        z = zs[2];
        x = z.Re();
        zs[2](x,0.0);
        y = Yn(x);
        rs[2] = TMath::Abs(y);
        zs[3](x,0.0);
        rs[3] = TMath::Abs(y);
        nrs += 2;
      }
      break;
    case 1:
      gCleanOut->MM(error,met,"Only 1 real root in TLitPol4",ClassName());
      break;
    case 2:
      if (TMath::Abs(zs[2].Im())<small) {
        nochange = kFALSE;
        z = zs[2];
        x = z.Re();
        zs[2](x,0.0);
        y = Yn(x);
        rs[2] = TMath::Abs(y);
        zs[3](x,0.0);
        rs[3] = TMath::Abs(y);
        nrs += 2;
      }
      break;
    case 3:
      gCleanOut->MM(error,met,"3 real root in TLitPol4",ClassName());
      break;
    case 4:
      break;
  }
  if (!nochange) {
    res1 = 0.0;
    for (k=0;k<4;k++) res1 += rs[k];
    if ((res1<res0) ||((res1<resmax) && (res1<10*res0))) {
      fNr  = nrs;
      res0 = res1;
      for (k=0;k<4;k++) {
        fRoots[k]  = zs[k];
        fERoots[k] = rs[k];
      }
    }
  }
  return res0;
}
void TLitPol4::Order() {
  //  Put roots in increasing order of real part, but real roots still before
  // complex roots
  Bool_t ok = kFALSE;
  Short_t k,k1;
  TComplex z1,z2;
  Double_t r1,r2;
  switch (fNr) {
    case 0:
      if (fRoots[0].Re()>fRoots[2].Re()) {
        z1 = fRoots[0];
        r1 = fERoots[0];
        z2 = fRoots[1];
        r2 = fERoots[1];
        fRoots[0]  = fRoots[2];
        fERoots[0] = fERoots[2];
        fRoots[1]  = fRoots[3];
        fERoots[1] = fERoots[3];
        fRoots[2]  = z1;
        fERoots[2] = r1;
        fRoots[3]  = z2;
        fERoots[3] = r2;
      }
      if (fRoots[0].Im()>fRoots[1].Im()) {
        z1 = fRoots[0];
        r1 = fERoots[0];
        fRoots[0]  = fRoots[1];
        fERoots[0] = fERoots[1];
        fRoots[1]  = z1;
        fERoots[1] = r1;
      }
      if (fRoots[2].Im()>fRoots[3].Im()) {
        z1 = fRoots[2];
        r1 = fERoots[2];
        fRoots[2]  = fRoots[3];
        fERoots[2] = fERoots[3];
        fRoots[3]  = z1;
        fERoots[3] = r1;
      }
      break;
    case 2:
      if (fRoots[0].Re()>fRoots[1].Re()) {
        z1 = fRoots[0];
        r1 = fERoots[0];
        fRoots[0]  = fRoots[1];
        fERoots[0] = fERoots[1];
        fRoots[1]  = z1;
        fERoots[1] = r1;
      }
      break;
    case 4:
      while (!ok) {
        ok = kTRUE;
        for (k=0;k<3;k++) {
          k1 = k+1;
          if (fRoots[k].Re()>fRoots[k1].Re()) {
            ok = kFALSE;
            z1 = fRoots[k];
            r1 = fERoots[k];
            fRoots[k]   = fRoots[k1];
            fERoots[k]  = fERoots[k1];
            fRoots[k1]  = z1;
            fERoots[k1] = r1;
          }
        }
      }
      break;
    default:
      gCleanOut->MMI(error,"Order","Bad number of real roots","fNr",fNr,ClassName());
      break;
  }
}
void TLitPol4::Print() const {
  // Prints normalized polynomial
  Double_t a,b,c,d,e;
  Getabcde(a,b,c,d,e);
  cout << endl;
  cout << "  p(z) == az^4 + bz^3 + cz^2 + dz + e" << endl;
  cout << "  a  = ";
  cout.width(20);
  cout.precision(12);
  cout << a << "     b  = ";
  cout.width(20);
  cout.precision(12);
  cout << b << "  c  = ";
  cout.width(20);
  cout.precision(12);
  cout << c << endl;
  cout << "  d  = ";
  cout.width(20);
  cout.precision(12);
  cout << d << "     e  = ";
  cout.width(20);
  cout.precision(12);
  cout << e << endl;
  cout << endl;
  cout << "  pn(z') == z'^4 + p*z'^2 + q*z' + r" << endl;
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
  cout << fQ << "     r  = ";
  cout.width(20);
  cout.precision(12);
  cout << fR << endl;
}
void TLitPol4::PrintRoots() const {
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
    for (k=0;k<4;k++)
      gCleanOut->MIR(info,"for root",k,"residue",fERoots[k]);
  }
}
void TLitPol4::Set(Double_t a,Double_t b,Double_t c,Double_t d,Double_t e) {
  // Defines polynomial: ax^4 + bx^3 + cx^2 + dx + e
  // Set roots as undefined
  const Double_t eps1 = 1.0e-300;
  const Double_t eps2 = 1.0e-12;
  const Double_t big  = 1.0e+100;
  const char *met = "Set";
  Short_t k;
  Double_t aa,ab,ac,ad,ae,aan,xt2,xt3;
  aa = TMath::Abs(a);
  ab = TMath::Abs(b);
  ac = TMath::Abs(c);
  ad = TMath::Abs(d);
  ae = TMath::Abs(e);
  fMag = aa+ab+ac+ad+ae;
  if (fMag<eps1) {
    gCleanOut->MM(fatal,met,"All coefficients are 0",ClassName());
  }
  else {
    aan  = aa/fMag;
    if (aan<eps1) {
      gCleanOut->MMR(fatal,met,"Coefficient a is 0","a",aa,ClassName());
    }
    else {
      if (aan<eps2) {
        gCleanOut->MMR(error,met,"Coefficient a is too small","a",a,ClassName());
      }
      else {
        //polynomial of 4th degree
        b  /= a;
        c  /= a;
        d  /= a;
        e  /= a;
        fAn = a;
        fXt = -b/4.0;
        xt2 = fXt*fXt;
        xt3 = fXt*xt2;
        fP  = 6*xt2+3*b*fXt+c;
        fQ  = 4*xt3+3*b*xt2+2*c*fXt+d;
        fR  = fXt*xt3+b*xt3+c*xt2+d*fXt+e;
      }
    }
  }
  fNr = -1;
  for (k=0;k<4;k++) {
    if (k<3) fYextr[k] = big;
    fRoots[k](big,big);
    fERoots[k] = big;
  }
}
Bool_t TLitPol4::Starting(TComplex &z) const {
  //  Tries to find a suitable z value for starting a Newton iteration
  // for finding the first root of the normalized polynomial. The found
  // value is returned in z. Methods returns true if z is complex, false
  // if z is real. If z is complex, method Newton(TComplex&) has to be used,
  // else method Newton(Double_t&).
  //  Let us write x1,x2,x3 the x values of the extrema in case there are
  // 3 extrema. Only x1 if only one extremum. And let us define y1,y2,y3
  // the y values of the extrema, and R1,R2,R3 the radius of curvature
  // at the extrema. F is some factor of order of magnitude 1 which has
  // to be adjusted by experimenting.
  //
  //  We distinguishes 7 cases:
  //
  //  (1) - 3 extrema  y1<=0 y3<=0 y1<=y3
  //          We propose a starting z which is real, with
  //             z.Re() = x1 - F*R1
  //  (2) - 3 extrema  y1<=0 y3>0
  //          We propose a starting z which is real, with
  //             z.Re() = x1 - F*R1
  //  (3) - 1 extremum y1<=0
  //          We propose a starting z which is real, with
  //             z.Re() = x1 - F*R1
  //  (4) - 3 extrema  y1<=0 y3<=0 y1>y3
  //          We propose a starting z which is real, with
  //             z.Re() = x3 + F*R3
  //  (5) - 3 extrema  y1>0 y3<=0
  //          We propose a starting z which is real, with
  //             z.Re() = x3 + F*R3
  //  (6) - 3 extrema  y1>0 y3>0
  //          In that case, there is no real roots! The starting z is complex,
  //             z.Re() = x1 -F*R1    z.Im() = 0.25*(gm+ga)
  //  (7) - 1 extremum y1>0
  //          In that case, there is no real roots! The starting z is complex,
  //             z.Re() = x1 -F*R1    z.Im() = 0.25*(gm+ga)
  //  Suppose that the 4 roots are pure imaginary. Then the geometric mean
  // of the imaginary parts of the 4 roots is gm = |fR|^1/4. And the aritmetic
  // mean of the imaginary parts is ga = Sqrt(0.5*|fP|). It explains the
  // Ansatz done for the starting value of the imaginary part.
  //
  const Double_t zero = 0.0;
  const Double_t eps  = 1.0e-12;
  const Double_t F    = 1.5;
  Bool_t iscomplex = kFALSE; //The returned value
  Short_t cases=0;  //The 7 cases described above
  Double_t x;       //Chose x [x1,x2 or x3]
  Double_t R;       //Radius of curvature at chosen x
  Double_t ypp;     //y'' at chosen x
  Double_t zim,gm,ga;
  z(zero,zero);
  if (fYextr[0]<=eps) {
    // y1<=0
    if (fNe==3) {
      // y1<=0 3 extrema
      if (fYextr[2]<=eps) {
        // y1<=0 3 extrema y3<=0
        if (fYextr[0]<=fYextr[2]) {
          // y1<=0 3 extrema y3<=0 y1<=y3
          cases = 1;
        }
        else {
          // y1<=0 3 extrema y3<=0 y1 > y3
          cases = 4;
        }
      }
      else {
        // y1<=0 3 extrema y3>0
        cases = 2;
      }
    }
    else {
      // y1<=0 1 extremum
      cases = 3;
    }
  }
  else {
    // y1>0
    if (fNe==3) {
      // y1>0 3 extrema
      if (fYextr[2]<=eps) {
        // y1>0 3 extrema y3<=0
        cases = 5;
      }
      else {
        // y1>0 3 extrema y3>0
        cases = 6;
      }
    }
    else {
      // y1>0 1 extremum
      cases = 7;
    }
  }
  switch (cases) {
    case 1:
    case 2:
    case 3:
      iscomplex = kFALSE;
      x         = GetXextr(0);
      ypp       = Yn(2,x);
      R         = 1.0/TMath::Abs(ypp);
      z(x-F*R,zero);
      break;
    case 4:
    case 5:
      iscomplex = kFALSE;
      x         = GetXextr(2);
      ypp       = Yn(2,x);
      R         = 1.0/TMath::Abs(ypp);
      z(x+F*R,zero);
      break;
    case 6:
    case 7:
      iscomplex = kTRUE;
      x         = GetXextr(0);
      ypp       = Yn(2,x);
      R         = 1.0/TMath::Abs(ypp);
      gm        = TMath::Abs(fR);
      gm        = TMath::Exp(0.25*TMath::Log(gm));
      ga        = TMath::Abs(fP);
      ga        = TMath::Sqrt(0.5*ga);
      zim       = 0.25*(gm+ga);
      z(x-F*R,zim);
      break;
    default:
      gCleanOut->MMI(error,"Starting","Impossible case","cases",cases,ClassName());
      break;
  }
  return iscomplex;
}
Double_t TLitPol4::Y(Double_t x) const {
  //  Calculation of polynomial at real x
  Double_t y,xp;
  xp = x - fXt;
  y  = fAn*Yn(xp);
  return y;
}
TComplex TLitPol4::Y(TComplex z) const {
  //  Calculation of polynomial at complex z
  TComplex y,zp;
  zp = z - fXt;
  y  = fAn*Yn(zp);
  return y;
}
Double_t TLitPol4::Y(Int_t n,Double_t x) const {
  //  Calculation of nth derivative at real x
  Double_t y,xp;
  xp = x - fXt;
  y  = fAn*Yn(n,xp);
  return y;
}
TComplex TLitPol4::Y(Int_t n,TComplex z) const {
  //  Calculation of nth derivative at complex z
  TComplex y,zp;
  zp = z - fXt;
  y  = fAn*Yn(n,zp);
  return y;
}
Double_t TLitPol4::Yn(Double_t xp) const {
  //  Calculation of normalized polynomial at real xp
  Double_t y,xp2;
  xp2 = xp*xp;
  y = xp2*xp2+fP*xp2+fQ*xp+fR;
  return y;
}
TComplex TLitPol4::Yn(TComplex zp) const {
  //  Calculation of normalized polynomial at complex zp
  TComplex y,zp2;
  zp2 = zp*zp;
  y = zp2*zp2+fP*zp2+fQ*zp +fR;
  return y;
}
Double_t TLitPol4::Yn(Int_t n,Double_t xp) const {
  //  Calculation of nth derivative of normalized polynomial at real x
  Double_t y;
  n  = TMath::Abs(n);
  switch (n) {
    case 0:
      y = Yn(xp);
      break;
    case 1:
      y = 4*xp*xp*xp+2*fP*xp + fQ;
      break;
    case 2:
      y = 12*xp*xp + 2*fP;
      break;
    case 3:
      y = 24*xp;
      break;
    case 4:
      y = 24;
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
TComplex TLitPol4::Yn(Int_t n,TComplex zp) const {
  //  Calculation of nth derivative of normalized polynomial at complex zp
  TComplex y;
  n  = TMath::Abs(n);
  switch (n) {
    case 0:
      y = Yn(zp);
      break;
    case 1:
      y = 4.0*zp*zp*zp+2*fP*zp + fQ;
      break;
    case 2:
      y = 12.0*zp*zp + 2*fP;
      break;
    case 3:
      y = 24.0*zp;
      break;
    case 4:
      y = 24.0;
      break;
    default:
      y = 0.0;
      break;
  }
  return y;
}
Double_t TLitPol4::operator()(Double_t x) const {
  return Y(x);
}
TComplex TLitPol4::operator()(TComplex x) const {
  return Y(x);
}
Double_t TLitPol4::operator()(Int_t k,Double_t x) const {
  return Y(k,x);
}
TComplex TLitPol4::operator()(Int_t k,TComplex x) const {
  return Y(k,x);
}
TLitPol4 &TLitPol4::operator=(const TLitPol4 &P) {
  // TLitPol4 assignment operator.
  if (this != &P) {
    Short_t k;
    fAn   = P.fAn;
    fXt   = P.fXt;
    fP    = P.fP;
    fQ    = P.fQ;
    fR    = P.fR;
    fMag  = P.fMag;
    fNr   = P.fNr;
    fNe   = P.fNe;
    fPol3 = 0;
    if (P.fPol3) {
      fPol3 = new TLitPol3();
      fPol3 = P.fPol3;
    }
    for (k=0;k<4;k++) {
      if (k<3) fYextr[k] = P.fYextr[k];
      fRoots[k]  = P.fRoots[k];
      fERoots[k] = P.fERoots[k];
    }
  }
  return *this;
}
Bool_t TLitPol4::operator==(const TLitPol4 &P) {
  return IsEqual(&P);
}
