// @(#)root/html:$Id: TMat33C.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TMat33.h"
#include "TMat33C.h"

ClassImp(TMat33C)
//______________________________________________________________________________
//
// TMat33C Implement 3X3 complex matrices associated with the real TVector3 and
//  with the complex TVec3C.
// A TMat33C can be a general matrix, a unitarian matrix, or an hermitian matrix.
//  This is indicated by the bits 17 and 18 of TMat33::fBits, which have the
//  following meaning:
//
//   Bit 17   Bit 18  Meaning
//
//      0        x    hermitian nature of matrix not tested
//      1        0    matrix is not hermitian  Conj(Mt) != M
//      1        1    matrix si hermitian      Conj(Mt) == M
//
//
//   Bit 19   Bit 20  Meaning
//
//      0        x    unitarian nature of matrix not tested
//      1        0    matrix is not unitarian Conj(Mt) != M-1
//      1        1    matrix si unitarian     Conj(Mt) == M-1
//
// The 6th and 7th constructor are for building hermitian matrices
// The 8th constructor builds a general matrix, neither hermitian, nor unitarian.
// The 9th constructor, the copy constructor, also allows to build the hermitian
//  conjugate of the matrix to copy.
//
// The usual things associated with matrices are there:
//  (1) - calculation of determinant [method Determinant()]
//  (2) - inversion of matrix [method Invert()]
//  (3) - calculation of eigenvalues, eigenvectors and projectors
//         [methods EigenValues(), EigenVectors(), EigenProjectors]
//  (4) - solution of linear system [method SolveLin3]
//
// A huge number of operators make the handling of TMat33 very easy.
//  writing C++ code with it is almost identical with writing mathematical
//  expressions.
// TMat33 has the 3 methods [Compare, IsEqual, IsSortable] allowing to sort
//  matrices inside collection. The classification parameter is the absolute value
//  of the determinant.
//
TMat33C::TMat33C(Double_t d) {
  // Constructor of a diagonal matrix where all diagonal elements are equal to d
  const Double_t un = 1.0;
  fV0(0) = d;
  fV1(1) = d;
  fV2(2) = d;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested,kTRUE);
  if (TMath::Abs(d-un) < TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else                              SetBit(TMat33::fgUni,kFALSE);
}
TMat33C::TMat33C(TComplex z) {
  // Constructor of a diagonal matrix where all diagonal elements are equal to complex z
  const Double_t un  = 1.0;
  const Double_t eps = 1.0e-307;
  fV0(0) = z;
  fV1(1) = z;
  fV2(2) = z;
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUni,kFALSE);
  if (TMath::Abs(z.Im()<eps)) {
    SetBit(TMat33::fgSym,kTRUE);
    if (TMath::Abs(z.Re() - un)<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
TMat33C::TMat33C(Double_t d00,Double_t d11,Double_t d22) {
  // Constructor of a diagonal matrix where diagonal elements are equal to real d00,d11,d22
  const Double_t un = 1.0;
  Double_t eps;
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  if (eps < TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else                            SetBit(TMat33::fgUni,kFALSE);
}
TMat33C::TMat33C(TComplex z00,TComplex z11,TComplex z22) {
  // Constructor of a diagonal matrix where diagonal elements are equal to complex z00,z11,z22
  const Double_t un = 1.0;
  const Double_t eps = 1.0e-307;
  Double_t cc;
  fV0(0) = z00;
  fV1(1) = z11;
  fV2(2) = z22;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  cc     = TMath::Abs(z00.Im());
  cc    += TMath::Abs(z11.Im());
  cc    += TMath::Abs(z22.Im());
  if (cc<eps) {
    SetBit(TMat33::fgSym,kTRUE);
    cc  = TMath::Abs(z00.Re() - un);
    cc += TMath::Abs(z11.Re() - un);
    cc += TMath::Abs(z22.Re() - un);
    if (cc<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
TMat33C::TMat33C(Double_t d00,Double_t d11,Double_t d22,
  TComplex z01,TComplex z02,TComplex z12) {
  // Constructor of an hermitian matrix with real diagonal elements d00, d11, d22 and
  //non diagonal elements z01, z02, z12
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  fV0(1) = z01;
  fV0(2) = z02;
  fV1(2) = z12;
  fV1(0) = TComplex::Conjugate(z01);
  fV2(0) = TComplex::Conjugate(z02);
  fV2(1) = TComplex::Conjugate(z12);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
TMat33C::TMat33C(const TVector3 &v0,const TVec3C &v1) {
  // Constructor of a hermitian matrix, where the vector v0 contains the 3 real diagonal
  //elements of the hermitian matrix, and the vector v1 contains the 3 complex non diagonal
  //elements of the matrix
  fV0(0) = v0(0);
  fV1(1) = v0(1);
  fV2(2) = v0(2);
  fV0(1) = v1(0);
  fV0(2) = v1(1);
  fV1(2) = v1(2);
  fV1(0) = TComplex::Conjugate(fV0(1));
  fV2(0) = TComplex::Conjugate(fV0(2));
  fV2(1) = TComplex::Conjugate(fV1(2));
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
TMat33C::TMat33C(const TVec3C &v0,const TVec3C &v1,const TVec3C &v2,Bool_t ascolumn) {
  // Constructor with 3 line vectors if ascolumn is false [default], or with 3 column
  //vectors if ascolumn is true.
  //
  // IMPORTANT NOTICE:
  //
  // The hermitian or unitarian state of the matrix constructed here is NOT
  //tested by this constructor.
  //If you happen to have entered an hermitian matrix, please either
  //  - immediatly call method VerifyHermitian() in order that the hermitian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is hermitian]
  //      call   -- SetBit(TMat33::fgSymTested);
  //             -- SetBit(TMat33::fgSym);
  //If you happen to have entered a unitarian matrix, please either
  //  - immediatly call method VerifyUnitarity() in order that the unitarian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is unitarian]
  //      call   -- SetBit(TMat33::fgUniTested);
  //             -- SetBit(TMat33::fgUni);
  fV0 = v0;
  fV1 = v1;
  fV2 = v2;
  if (ascolumn) CT();
  SetStateFalse();
}
TMat33C::TMat33C(const TMat33C &m,Bool_t CcTransp) {
  // Copy constructor if CcTransp is false [default]. Otherwise
  //construct the complex conjugate transposed of m.
  if (m.TestBit(TMat33::fgUniTested)) SetBit(TMat33::fgUniTested);
  else                                   SetBit(TMat33::fgUniTested,kFALSE);
  if (m.TestBit(TMat33::fgUni))       SetBit(TMat33::fgUni);
  else                                   SetBit(TMat33::fgUni,kFALSE);
  if (m.TestBit(TMat33::fgSymTested)) SetBit(TMat33::fgSymTested);
  else                                   SetBit(TMat33::fgSymTested,kFALSE);
  if (m.TestBit(TMat33::fgSym))       SetBit(TMat33::fgSym);
  else                                   SetBit(TMat33::fgSym,kFALSE);
  fV0 = m.fV0;
  fV1 = m.fV1;
  fV2 = m.fV2;
  if (CcTransp) CT();
}
TMat33C::TMat33C(const TMat33 &m,Bool_t CcTransp) {
  // Copy constructor if CcTransp is false [default]. Otherwise construct the transposed of m.
  if (m.TestBit(TMat33::fgUniTested)) SetBit(TMat33::fgUniTested);
  else                                   SetBit(TMat33::fgUniTested,kFALSE);
  if (m.TestBit(TMat33::fgUni))       SetBit(TMat33::fgUni);
  else                                   SetBit(TMat33::fgUni,kFALSE);
  if (m.TestBit(TMat33::fgSymTested)) SetBit(TMat33::fgSymTested);
  else                                   SetBit(TMat33::fgSymTested,kFALSE);
  if (m.TestBit(TMat33::fgSym))       SetBit(TMat33::fgSym);
  else                                   SetBit(TMat33::fgSym,kFALSE);
  fV0 = m.GetLine(0);
  fV1 = m.GetLine(1);
  fV2 = m.GetLine(2);
  if (CcTransp) T();
}
void TMat33C::ChangeSign() {
  //All elements of matrix do change sign
  Short_t k;
  for (k=0;k<3;k++) fV0(k) = -fV0(k);
  for (k=0;k<3;k++) fV1(k) = -fV1(k);
  for (k=0;k<3;k++) fV2(k) = -fV2(k);
}
void TMat33C::Clean() {
// Replaces the values very close to 0 by 0
  const Double_t zero = 0.0;
  const TComplex I(0.0,1.0);
  Bool_t ismod;
  Short_t k;
  Double_t D,eps,zr,zi;
  D = MaxCoeff();
  eps  = D*TMat33::fgLimPrec;
  for (k=0;k<3;k++) {
    zr = fV0(k).Re();
    zi = fV0(k).Im();
    ismod = kFALSE;
    if (TMath::Abs(zr)<eps) {
      ismod = kTRUE;
      zr = zero;
    }
    if (TMath::Abs(zi)<eps) {
      ismod = kTRUE;
      zi = 0;
    }
    if (ismod) fV0(k) = zr + zi*I;
  }
  for (k=0;k<3;k++) {
    zr = fV1(k).Re();
    zi = fV1(k).Im();
    ismod = kFALSE;
    if (TMath::Abs(zr)<eps) {
      ismod = kTRUE;
      zr = zero;
    }
    if (TMath::Abs(zi)<eps) {
      ismod = kTRUE;
      zi = 0;
    }
    if (ismod) fV1(k) = zr + zi*I;
  }
  for (k=0;k<3;k++) {
    zr = fV2(k).Re();
    zi = fV2(k).Im();
    ismod = kFALSE;
    if (TMath::Abs(zr)<eps) {
      ismod = kTRUE;
      zr = zero;
    }
    if (TMath::Abs(zi)<eps) {
      ismod = kTRUE;
      zi = 0;
    }
    if (ismod) fV2(k) = zr + zi*I;
  }
}
Double_t TMat33C::CloseUnit() const {
  //Test how close from the unit matrix
  const Double_t un   = 1.0;
  Double_t d;
  TComplex z;
  z  = fV0(0)-un;
  d  = z.Rho2();;
  d += fV0(1).Rho2();
  d += fV0(2).Rho2();
  d += fV1(0).Rho2();
  z  = fV1(1)-un;
  d += z.Rho2();
  d += fV1(2).Rho2();
  d += fV2(0).Rho2();
  d += fV2(1).Rho2();
  z  = fV2(2)-un;
  d += z.Rho2();
  d  = TMath::Sqrt(d);
  return d;
}
Int_t TMat33C::Compare(const TObject *obj) const {
// Compare according to absolute value of determinant. Allows to sort matrices in a collection
  if ( *this < *((TMat33C*)obj) ) return -1;
  else {
    if ( *this > *((TMat33C*)obj) ) return 1;
    else                               return 0;
  }
}
void TMat33C::Conjugate() {
  // Complex conjugate *this, but DO NOT transpose it
  fV0(0) = TComplex::Conjugate(fV0(0));
  fV0(1) = TComplex::Conjugate(fV0(1));
  fV0(2) = TComplex::Conjugate(fV0(2));
  fV1(0) = TComplex::Conjugate(fV1(0));
  fV1(1) = TComplex::Conjugate(fV1(1));
  fV1(2) = TComplex::Conjugate(fV1(2));
  fV2(0) = TComplex::Conjugate(fV2(0));
  fV2(1) = TComplex::Conjugate(fV2(1));
  fV2(2) = TComplex::Conjugate(fV2(2));

}
TMat33C TMat33C::Cos() const {
// Calculates the matrix which is the cosinus of "this", on the condition that
//"this" is hermitian
  const Double_t zero = 0.0;
  TMat33C M(zero);
  if (IsHermitian()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVec3C  v0,v1,v2;
    TMat33C P0,P1,P2;
    n = EigenProjectors(L0,L1,L2,v0,v1,v2,P0,P1,P2);
    L0 = TMath::Cos(L0);
    L1 = TMath::Cos(L1);
    L2 = TMath::Cos(L2);
    M  = L0*P0 + L1*P1 + L2*P2;
    M.SetBit(TMat33::fgSymTested);
    M.SetBit(TMat33::fgSym);
  }
  return M;
}
void TMat33C::CT() {
  // Transposes and complex conjugate the matrix. This does not change the hermitian or
  //unitarian character of the matrix
  TComplex z;
  fV0(0) = TComplex::Conjugate(fV0(0));
  fV1(1) = TComplex::Conjugate(fV1(1));
  fV2(2) = TComplex::Conjugate(fV2(2));
  z = fV1(0); fV1(0) = TComplex::Conjugate(fV0(1)); fV0(1) = TComplex::Conjugate(z);
  z = fV2(0); fV2(0) = TComplex::Conjugate(fV0(2)); fV0(2) = TComplex::Conjugate(z);
  z = fV1(2); fV1(2) = TComplex::Conjugate(fV2(1)); fV2(1) = TComplex::Conjugate(z);
}
Short_t TMat33C::DegMatrix(TVec3C &x,TVec3C &y,Bool_t test) const {
  //  Solve the equation A*x = 0 assuming that matrix A is degenerate. There are
  // 3 ways to get vector x. We use the 3, with a ponderate method. The weights
  // used are the absolute sin^2 of the angle between 2 vectors. It is natural,
  // since when this sinus is 0, the solution is undefined.
  //
  //  Put test to kTRUE if you want result to be checked.
  //
  // If 0 returned, it means that matrix M is not degenerate, so that in principle
  //                the problem A*x = 0 has no solution other than x = 0. But
  //                in that case, we do the calculation "as if" matrix
  //                A were degenerate. However,
  //                 (1) we return 0
  //                 (2) the "bad" solution is returned in x
  //                 (3) the determinant of the matrix is returned in y[0].Re()
  //                 (4) the value of |A*x| is returned in y[1].Re()
  // If 1 returned, matrix M is once degenerate, normal case. There is
  //                only 1 vector as solution x. Vector y is 0.
  // If 2 returned, matrix M twice degenerate. All vectors in a certain plane are
  //                solutions. We arbitrary return 2 orthogonal vectors in this
  //                plane as solution : vector x and y. Remember in that case that
  //                any linear combination of these 2 vectors is also a solution.
  // If 3 returned, matrix M is the 0 matrix. Any vector is solution, so that it
  //                does not make sense to return something in x or y. We return
  //                x=0 and y=0.
  //
  // When not 0, the return vectors are always normalized to 1
  //
  const TComplex zero(0.0,0.0);
  const TComplex un(1.0,0.0);
  const Double_t small  = 1.0e-5;
  const Double_t vsmall = 1.0e-15;
  const Double_t ltest  = 5.0*small;
  Short_t deg = 1;
  Double_t a,b,c,d;
  TComplex ca,cb,cc;
  Bool_t   b0,b1,b2;
  Bool_t   gen = kFALSE;
  Double_t w1,w2,w;
  Double_t w0 = 0.0;
  TVec3C x0,x1,x2,p;
  d = TComplex::Abs(Determinant());
  x.SetXYZ(zero,zero,zero);
  y.SetXYZ(zero,zero,zero);
  if (d>small) {
    deg = 0;
    y[0] = d;
  }
  GetLine(0,ca,cb,cc);
  TVec3C m0(ca,cb,cc);
  GetLine(1,ca,cb,cc);
  TVec3C m1(ca,cb,cc);
  GetLine(2,ca,cb,cc);
  TVec3C m2(ca,cb,cc);
  a = m0.Mag2();
  b = m1.Mag2();
  c = m2.Mag2();
  TVector3 vz(a,b,c);
  vz = vz.Unit();
  a = vz(0);
  b = vz(1);
  c = vz(2);
  if (a<vsmall) {
    b0 = kFALSE;
    w0 = 0.0;
  }
  else {
    b0 = kTRUE;
    m0.Unit();
  }
  if (b<vsmall) {
    b1 = kFALSE;
    w1 = 0.0;
  }
  else {
    b1 = kTRUE;
    m1.Unit();
  }
  if (c<vsmall) {
    b2 = kFALSE;
    w2 = 0.0;
  }
  else {
    b2 = kTRUE;
    m2.Unit();
  }
  if (b0) {
    //m0 usable
    if (b1) {
      //m0 && m1 usable
      if (b2) {
        //m0 && m1 && m2 usable          : +0 +1 +2
        x0 = m0^m1;
        x1 = m0^m2;
        x2 = m1^m2;
        w0 = a*b*x0.Mag2();
        w1 = a*c*x1.Mag2();
        w2 = b*c*x2.Mag2();
        w  = w0 + w1 + w2;
        w0 /= w;
        w1 /= w;
        w2 /= w;
        x0.Unit();
        x1.Unit();
        x2.Unit();
        TMat33C::RatherPositiv(x0);
        TMat33C::RatherPositiv(x1);
        TMat33C::RatherPositiv(x2);
        x  = w0*x0 + w1*x1 + w2*x2;
      }
      else {
        //m0 && m1 usable, but not m2    : +0 +1 -2
        x0 = m0^m1;
        w0 = x0.Mag2();
        if (w0<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x0.Unit();
          TMat33C::RatherPositiv(x0);
          x = x0;
        }
      }
    }
    else {
      //m0 usable, but not m1
      if (b2) {
        //m0 && m2 usable, but not m1    : +0 -1 +2
        x1 = m0^m2;
        w1 = x1.Mag2();
        if (w1<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x1.Unit();
          TMat33C::RatherPositiv(x1);
          x = x1;
        }
      }
      else {
        //m0 usable but not m1, not m2   : +0 -1 -2
        deg = 2;
        p   = m0;
        gen = kTRUE;
      }
    }
  }
  else {
    //m0 unusable
    if (b1) {
      //m1 usable but not m0
      if (b2) {
        //m1 && m2 usable, not m0        : -0 +1 +2
        x2 = m1^m2;
        w2 = x2.Mag2();
        if (w2<vsmall) {
          deg = 2;
          p   = m1;
          gen = kTRUE;
        }
        else {
          x2.Unit();
          TMat33C::RatherPositiv(x2);
          x = x2;
        }
      }
      else {
        //m1 usable, not m0, m2          : -0 +1 -2
        deg = 2;
        p = m1;
        gen = kTRUE;
      }
    }
    else {
      //m0, m1 unusable
      if (b2) {
        //m2 usable, not m0,m1           : -0 -1 +2
        deg = 2;
        p = m2;
        gen = kTRUE;
      }
      else {
        //m0, m1, m2 unusable            : -0 -1 -2
        deg = 3;
      }
    }
  }
  if (gen) {
    x0.SetXYZ(un,zero,zero);
    x1.SetXYZ(zero,un,zero);
    x2.SetXYZ(zero,zero,un);
    a  = TComplex::Abs(p*x0);
    b  = TComplex::Abs(p*x1);
    c  = TComplex::Abs(p*x2);
    if (a<=b) {
      if (a<=c) {
        x = p^x0;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
    else {
      if (b<=c) {
        x = p^x1;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
  }
  if (((deg==0) || (test))) {
    switch (deg) {
    case 0:
    case 1:
      x0 = (*this)*x;
      w0 = x0.Mag();
      y[1] = w0;
      break;
    case 2:
      x0 = (*this)*x;
      x1 = (*this)*y;
      w0 = x0.Mag() + x1.Mag();
      break;
    default:
      w0 = 0.0;
      break;
    }
    if ((test) && (w0>ltest)) cout << "W TMat33C::DegMatrix Bad solution w0 = " << w0 << endl;
  }
  return deg;
}
TComplex TMat33C::Determinant() const {
  //Returns the determinant of this matrix
  TComplex z;
  z = fV0&(fV1^fV2);
  return z;
}
void TMat33C::Diagonal(Double_t d00,Double_t d11,Double_t d22) {
  //Defines this matrix as being a diagonal with elements d00, d11, d22
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t eps;
  fV0(0) = d00;
  fV0(1) = zero;
  fV0(2) = zero;
  fV1(0) = zero;
  fV1(1) = d11;
  fV1(2) = zero;
  fV2(0) = zero;
  fV2(1) = zero;
  fV2(2) = d22;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  if (eps < TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else                            SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::Diagonal(TComplex d00,TComplex d11,TComplex d22) {
  //Defines this matrix as being a diagonal with elements d00, d11, d22
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t eps  = 1.0e-307;
  Double_t cc;
  fV0(0) = d00;
  fV0(1) = zero;
  fV0(2) = zero;
  fV1(0) = zero;
  fV1(1) = d11;
  fV1(2) = zero;
  fV2(0) = zero;
  fV2(1) = zero;
  fV2(2) = d22;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  cc     = TMath::Abs(d00.Im());
  cc    += TMath::Abs(d11.Im());
  cc    += TMath::Abs(d22.Im());
  if (cc<eps) {
    SetBit(TMat33::fgSym,kTRUE);
    cc  = TMath::Abs(d00.Re() - un);
    cc += TMath::Abs(d11.Re() - un);
    cc += TMath::Abs(d22.Re() - un);
    if (cc<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
void TMat33C::Diagonalize() {
  //Clears the non diagonal elements
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t eps  = 1.0e-307;
  Double_t cc;
  fV1(0) = zero;
  fV2(0) = zero;
  fV2(1) = zero;
  fV0(1) = zero;
  fV0(2) = zero;
  fV1(2) = zero;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  cc     = TMath::Abs(fV0(0).Im());
  cc    += TMath::Abs(fV1(1).Im());
  cc    += TMath::Abs(fV2(2).Im());
  if (cc<eps) {
    SetBit(TMat33::fgSym,kTRUE);
    cc  = TMath::Abs(fV0(0).Re() - un);
    cc += TMath::Abs(fV1(1).Re() - un);
    cc += TMath::Abs(fV2(2).Re() - un);
    if (cc<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
Short_t TMat33C::EigenValues(Double_t &L0,Double_t &L1,Double_t &L2) const {
  // Finds the eigenvalues, ONLY in case matrix is hermitian. The eigenvalues
  //are returned in increasing order.
  // Returns n = 10*n0 + n1, where
  //  - n0 : number of eigenvalues equal to 0
  //  - n1 : number of DIFFERENT eigenvalues
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t epsc = 5.0e-7;
  Short_t n,n0,n1;
  Bool_t complex,ishermit;
  Double_t D,eps;
  TComplex z0,z1,z2;
  Double_t coef[4];
  n   = 0;
  L0  = zero;
  L1  = zero;
  L2  = zero;
  D   = TMat33::fgLimPrec*MaxCoeff();
  eps = VerifyHermitian();
  ishermit = eps<D;
  if (ishermit) {
    z0   = fV0(2)*fV2(0)*fV1(1) + fV0(0)*fV1(2)*fV2(1) + fV0(1)*fV1(0)*fV2(2) -
      fV0(0)*fV1(1)*fV2(2) - fV0(1)*fV1(2)*fV2(0) - fV0(2)*fV1(0)*fV2(1);
    z1   = fV0(0)*fV1(1) + fV0(0)*fV2(2) + fV1(1)*fV2(2) - fV0(2)*fV2(0) -
      fV1(2)*fV2(1) - fV0(1)*fV1(0);
    z2   = -(fV0(0) + fV1(1) + fV2(2));
    eps  = TMath::Abs(z0.Im());
    eps += TMath::Abs(z1.Im());
    eps += TMath::Abs(z2.Im());
    if (eps>epsc) {
      Error("Eigenvalues","Coefficients of 3rd order polynomial not real");
      return n;
    }
    coef[0] = z0.Re();
    coef[1] = z1.Re();
    coef[2] = z2.Re();
    coef[3] = un;
    n0      = 0;
    n1      = 3;
    complex = TMat33C::MyRootsCubic(coef,L0,L1,L2);
    D       = TMath::Abs(L1);
    if (complex) {
      eps     = epsc*D;
      if (TMath::Abs(L2)<eps) {
        complex = kFALSE;
        L2      = L1;
      }
      else {
        Error("EigenValues","Complex eigenvalues for hermitian matrix");
        return n;
      }
    }
    D      += TMath::Abs(L0);
    D      += TMath::Abs(L2);
    eps     = TMat33::fgLimPrec*D;
    if (TMath::Abs(L0)<eps) { L0 = zero; n0++; }
    if (TMath::Abs(L1)<eps) { L1 = zero; n0++; }
    if (TMath::Abs(L2)<eps) { L2 = zero; n0++; }
    // ordering eigenvalues
    if (L1<L0) { D = L1; L1 = L0; L0 = D; }
    if (L2<L1) { D = L2; L2 = L1; L1 = D; }
    if (L1<L0) { D = L1; L1 = L0; L0 = D; }
    // counting different eigenvalues
    if ((L1-L0)<eps) {
      D  = 0.5*(L1 + L0);
      L0 = D;
      L1 = D;
      n1--;
    }
    if ((L2-L0)<eps) {
      D  = 0.5*(L2 + L0);
      L0 = D;
      L2 = D;
      n1--;
    }
    if ((n1>1) && (L2-L1)<eps) {
      D  = 0.5*(L2 + L1);
      L1 = D;
      L2 = D;
      n1--;
    }
    n = 10*n0 + n1;
  }
  else Error("EigenValues","Matrix is not hermitian");
  return n;
}
Short_t TMat33C::EigenVectors(Double_t &L0,Double_t &L1,Double_t &L2,TVec3C &v0,TVec3C &v1,TVec3C &v2) const {
  // Finds the eigenvalues and the eigenvectors ONLY in case matrix is hermitian.
  //The eigenvalues are returned in increasing order.
  // Returns n = 10*n0 + n1, where
  //  - n0 : number of eigenvalues equal to 0
  //  - n1 : number of DIFFERENT eigenvalues (n1<3 ==> degenerate eigenvalues)
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Short_t n;
  TVec3C u0,u1,u2;
  TVec3C w0,w1,w2;
  Double_t c0,c1,c2;
  n = EigenValues(L0,L1,L2);
  // Finding v0
  TMat33C M(*this);
  M(0,0) -= L0;
  M(1,1) -= L0;
  M(2,2) -= L0;
  M.Clean();
  w0      = M(0);
  w1      = M(1);
  w2      = M(2);
  u0      = w0.Cross(w1);
  u1      = w0.Cross(w2);
  u2      = w1.Cross(w2);
  c0      = u0.Mag2();
  c1      = u1.Mag2();
  c2      = u2.Mag2();
  if (c0>=c1) {
    if (c0>=c2) v0 = u0;
    else        v0 = u2;
  }
  else {
    if (c1>=c2) v0 = u1;
    else        v0 = u2;
  }
  v0.Unit();
  v0.Clean();
  // Finding v1
  M = *this;
  M(0,0) -= L1;
  M(1,1) -= L1;
  M(2,2) -= L1;
  M.Clean();
  w0      = M(0);
  w1      = M(1);
  w2      = M(2);
  u0      = w0.Cross(w1);
  u1      = w0.Cross(w2);
  u2      = w1.Cross(w2);
  c0      = u0.Mag2();
  c1      = u1.Mag2();
  c2      = u2.Mag2();
  if (c0>=c1) {
    if (c0>=c2) v1 = u0;
    else        v1 = u2;
  }
  else {
    if (c1>=c2) v1 = u1;
    else        v1 = u2;
  }
  v1.Unit();
  v1.Clean();
  // Finding v2
  M = *this;
  M(0,0) -= L2;
  M(1,1) -= L2;
  M(2,2) -= L2;
  M.Clean();
  w0      = M(0);
  w1      = M(1);
  w2      = M(2);
  u0      = w0.Cross(w1);
  u1      = w0.Cross(w2);
  u2      = w1.Cross(w2);
  c0      = u0.Mag2();
  c1      = u1.Mag2();
  c2      = u2.Mag2();
  if (c0>=c1) {
    if (c0>=c2) v2 = u0;
    else        v2 = u2;
  }
  else {
    if (c1>=c2) v2 = u1;
    else        v2 = u2;
  }
  v2.Unit();
  v2.Clean();
  //Case of 2 degenerate eigenvalue
  if (n==2) {
    c0 = v0.Mag2();
    c1 = v1.Mag2();
    c2 = v1.Mag2();
    w0.SetXYZ(un,zero,zero);
    w1.SetXYZ(zero,un,zero);
    w2.SetXYZ(zero,zero,un);
    if (c0>=c1) {
      if (c0<c2) v0 = v2;
    }
    else {
      if (c1>=c2) v0 = v1;
      else        v0 = v2;
    }
    u0 = v0^w0;
    u1 = v0^w1;
    u2 = v0^w2;
    c0 = u0.Mag2();
    c1 = u1.Mag2();
    c2 = u2.Mag2();
    if (c0<c1) {
      if (c0<c2) {
        //avoid u0
        v1 = u1;
        v2 = v0^v1;
      }
      else {
        //avoid u2
        v1 = u1;
        v2 = v0^v1;
      }
    }
    else {
      if (c1<c2) {
        //avoid u1
        v1 = u0;
        v2 = v0^v1;
      }
      else {
        //avoid u2
        v1 = u1;
        v2 = v0^v1;
      }
    }
    v1.Unit();
    v2.Unit();
  }
  if (n==1) {
    v0.SetXYZ(un,zero,zero);
    v1.SetXYZ(zero,un,zero);
    v2.SetXYZ(zero,zero,un);
  }
  return n;
}
Short_t TMat33C::EigenProjectors(Double_t &L0,Double_t &L1,Double_t &L2,
  TVec3C &v0,TVec3C &v1,TVec3C &v2,TMat33C &P0,TMat33C &P1,TMat33C &P2) const {
  // Finds the eigenvalues AND the eigenvectors AND the projectors on the eigenvectors
  //ONLY in the case matrix is hermitian. Please call VerifyHermitian() before
  //calling this method.
  // The eigenvalues are returned in increasing order.
  // Returns n1: number of DIFFERENT eigenvalues (n1<3 ==> degenerate eigenvalues)
  // Eigenvectors v0, v1, v2 are orthonormal
  // Projectors P0, P1, P2 are such that
  //   P0*v0 = v0   P0*v1 =  0   P0*v2 =  0
  //   P1*v0 =  0   P1*v1 = v1   P1*v2 =  0
  //   P2*v0 =  0   P2*v1 =  0   P2*v2 = v2
  //   P0*P0 = P0   P1*P1 = P1   P2*P2 = P2
  //   P0*P1 =  0   P0*P2 =  0   P1*P2 =  0
  //
  const Double_t zero = 0.0;
  Short_t n = 0;
  P0.SetBit(TMat33::fgSymTested);
  P0.SetBit(TMat33::fgSym);
  P1.SetBit(TMat33::fgSymTested);
  P1.SetBit(TMat33::fgSym);
  P2.SetBit(TMat33::fgSymTested);
  P2.SetBit(TMat33::fgSym);
  n = EigenVectors(L0,L1,L2,v0,v1,v2);
  if (n) {
    //
    P0(0,0) = TComplex::Conjugate(v0(0))*v0(0);
    P0(0,1) = TComplex::Conjugate(v0(1))*v0(0);
    P0(0,2) = TComplex::Conjugate(v0(2))*v0(0);
    P0(1,0) = TComplex::Conjugate(v0(0))*v0(1);
    P0(1,1) = TComplex::Conjugate(v0(1))*v0(1);
    P0(1,2) = TComplex::Conjugate(v0(2))*v0(1);
    P0(2,0) = TComplex::Conjugate(v0(0))*v0(2);
    P0(2,1) = TComplex::Conjugate(v0(1))*v0(2);
    P0(2,2) = TComplex::Conjugate(v0(2))*v0(2);
    P0.Clean();
    //
    P1(0,0) = TComplex::Conjugate(v1(0))*v1(0);
    P1(0,1) = TComplex::Conjugate(v1(1))*v1(0);
    P1(0,2) = TComplex::Conjugate(v1(2))*v1(0);
    P1(1,0) = TComplex::Conjugate(v1(0))*v1(1);
    P1(1,1) = TComplex::Conjugate(v1(1))*v1(1);
    P1(1,2) = TComplex::Conjugate(v1(2))*v1(1);
    P1(2,0) = TComplex::Conjugate(v1(0))*v1(2);
    P1(2,1) = TComplex::Conjugate(v1(1))*v1(2);
    P1(2,2) = TComplex::Conjugate(v1(2))*v1(2);
    P1.Clean();
    //
    P2(0,0) = TComplex::Conjugate(v2(0))*v2(0);
    P2(0,1) = TComplex::Conjugate(v2(1))*v2(0);
    P2(0,2) = TComplex::Conjugate(v2(2))*v2(0);
    P2(1,0) = TComplex::Conjugate(v2(0))*v2(1);
    P2(1,1) = TComplex::Conjugate(v2(1))*v2(1);
    P2(1,2) = TComplex::Conjugate(v2(2))*v2(1);
    P2(2,0) = TComplex::Conjugate(v2(0))*v2(2);
    P2(2,1) = TComplex::Conjugate(v2(1))*v2(2);
    P2(2,2) = TComplex::Conjugate(v2(2))*v2(2);
    P2.Clean();
    //
  }
  else {
    L0 = zero;
    L1 = zero;
    L2 = zero;
    v0.SetXYZ(zero,zero,zero);
    v1.SetXYZ(zero,zero,zero);
    v2.SetXYZ(zero,zero,zero);
    P0.SetAll(zero);
    P1.SetAll(zero);
    P2.SetAll(zero);
    Error("EigenProjectors","This method cannot be called for non hermitian matrices");
  }
  return n;
}
TMat33C TMat33C::Exp() const {
// Calculates the matrix which is the exponential of "this", on the condition
//that "this" is hermitian
  const Double_t zero = 0.0;
  TMat33C M(zero);
  if (IsHermitian()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVec3C  v0,v1,v2;
    TMat33C P0,P1,P2;
    n = EigenProjectors(L0,L1,L2,v0,v1,v2,P0,P1,P2);
    L0 = TMath::Exp(L0);
    L1 = TMath::Exp(L1);
    L2 = TMath::Exp(L2);
    M  = L0*P0 + L1*P1 + L2*P2;
    M.SetBit(TMat33::fgSymTested);
    M.SetBit(TMat33::fgSym);
  }
  return M;
}
TVec3C TMat33C::GetColumn(Int_t i) const {
  // Returns column i of matrix
  TVec3C v;
  i  = TMath::Abs(i);
  i %= 3;
  v(0) = fV0(i);
  v(1) = fV1(i);
  v(2) = fV2(i);
  return v;
}
void TMat33C::GetColumn(Int_t i,TComplex &a,TComplex &b,TComplex &c) const {
  // Returns column i of matrix into (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  a = fV0(i);
  b = fV1(i);
  c = fV2(i);
}
TVec3C TMat33C::GetLine(Int_t i) const {
  // Use rather operator () or [] !
  return (*this)(i);
}
void TMat33C::GetLine(Int_t i,TComplex &a,TComplex &b,TComplex &c) const {
  // Returns line i of matrix into (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
  case 0:
    a = fV0(0);  b = fV0(1);  c = fV0(2);
    break;
  case 1:
    a = fV1(0);  b = fV1(1);  c = fV1(2);
    break;
  case 2:
    a = fV2(0);  b = fV2(1);  c = fV2(2);
    break;
  }
}
void TMat33C::HConjugateOf(const TMat33C &m) {
  //"this" becomes the hermitian conjugate of m
  fV0(0) = TComplex::Conjugate(m.fV0(0));
  fV0(1) = TComplex::Conjugate(m.fV1(0));
  fV0(2) = TComplex::Conjugate(m.fV2(0));
  fV1(0) = TComplex::Conjugate(m.fV0(1));
  fV1(1) = TComplex::Conjugate(m.fV1(1));
  fV1(2) = TComplex::Conjugate(m.fV2(1));
  fV2(0) = TComplex::Conjugate(m.fV0(2));
  fV2(1) = TComplex::Conjugate(m.fV1(2));
  fV2(2) = TComplex::Conjugate(m.fV2(2));
  SetStateAs(m);
}
void TMat33C::Hermitianize() {
  //Minimal change to the matrix in order that it be hermitian
  const Double_t deux = 2.0;
  TComplex z01,z02,z12;
  fV0(0) = fV0(0).Re();
  fV1(1) = fV1(1).Re();
  fV2(2) = fV2(2).Re();
  z01    = (fV0(1) + TComplex::Conjugate(fV1(0)))/deux;
  z02    = (fV0(2) + TComplex::Conjugate(fV2(0)))/deux;
  z12    = (fV1(2) + TComplex::Conjugate(fV2(1)))/deux;
  fV0(1) = z01;
  fV0(2) = z02;
  fV1(2) = z12;
  fV1(0) = TComplex::Conjugate(z01);
  fV2(0) = TComplex::Conjugate(z02);
  fV2(1) = TComplex::Conjugate(z12);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
Bool_t TMat33C::Invert(TComplex &det) {
// Invert the matrix and calculates its determinant "det".
// Inversion does not change the hermitian or unitarian character of the matrix.
// In case of failure, the original matrix is untouched. The boolean value returned
//only indicates if true that the calculation could be done without arithmetic
//overflow, division by 0, and so on. If you want an indication of the error
//when calculating the inverse, please use the next version of this method.
//
  const Double_t  un = 1.0;
  const Double_t eps = 1.0e-306;
  Bool_t ok = kFALSE;
  Double_t big,lim,adet;
  TMat33C M;
  det  = Determinant();
  adet = det.Rho();
  if (adet<eps) return ok;
  M.fV0(0) =   fV1(1)*fV2(2) - fV1(2)*fV2(1);
  M.fV1(0) = -(fV1(0)*fV2(2) - fV1(2)*fV2(0));
  M.fV2(0) =   fV1(0)*fV2(1) - fV1(1)*fV2(0);
  M.fV0(1) = -(fV0(1)*fV2(2) - fV0(2)*fV2(1));
  M.fV1(1) =   fV0(0)*fV2(2) - fV0(2)*fV2(0);
  M.fV2(1) = -(fV0(0)*fV2(1) - fV0(1)*fV2(0));
  M.fV0(2) =   fV0(1)*fV1(2) - fV0(2)*fV1(1);
  M.fV1(2) = -(fV0(0)*fV1(2) - fV0(2)*fV1(0));
  M.fV2(2) =   fV0(0)*fV1(1) - fV0(1)*fV1(0);
  big = M.MaxCoeff();
  if (adet>=un) lim = eps*(big/adet);
  else          lim = big*(eps/adet);
  if (lim<un) {
    ok = kTRUE;
    fV0(0) = M.fV0(0)/det;
    fV0(1) = M.fV0(1)/det;
    fV0(2) = M.fV0(2)/det;
    fV1(0) = M.fV1(0)/det;
    fV1(1) = M.fV1(1)/det;
    fV1(2) = M.fV1(2)/det;
    fV2(0) = M.fV2(0)/det;
    fV2(1) = M.fV2(1)/det;
    fV2(2) = M.fV2(2)/det;
    Clean();
  }
  return ok;
}
Bool_t TMat33C::Invert(Double_t &Error,TComplex &det) {
// Invert the matrix, calculates its determinant "det" and calculates the biggest
//error "Error" obtained when multiplying the inverse matrix with the original one.
// Inversion does not change the hermitian or unitarian character of the matrix.
// In case of failure, the original matrix is untouched. The boolean value returned
//only indicates if true that the calculation could be done without arithmetic
//overflow, division by 0, and so on. It is not an indication of the quality of the
//inverse matrix, for which the value of "Error" is the good one.
// As the calculation of "Error" is costly, an other version of this method
//exists, without it!
//
  const Double_t  un = 1.0;
  const Double_t eps = 1.0e-306;
  Bool_t ok = kFALSE;
  Double_t big,lim,adet;
  TMat33C W(*this);
  TMat33C M;
  Error = un/eps;
  det  = Determinant();
  adet = det.Rho();
  if (adet<eps) return ok;
  M.fV0(0) =   fV1(1)*fV2(2) - fV1(2)*fV2(1);
  M.fV1(0) = -(fV1(0)*fV2(2) - fV1(2)*fV2(0));
  M.fV2(0) =   fV1(0)*fV2(1) - fV1(1)*fV2(0);
  M.fV0(1) = -(fV0(1)*fV2(2) - fV0(2)*fV2(1));
  M.fV1(1) =   fV0(0)*fV2(2) - fV0(2)*fV2(0);
  M.fV2(1) = -(fV0(0)*fV2(1) - fV0(1)*fV2(0));
  M.fV0(2) =   fV0(1)*fV1(2) - fV0(2)*fV1(1);
  M.fV1(2) = -(fV0(0)*fV1(2) - fV0(2)*fV1(0));
  M.fV2(2) =   fV0(0)*fV1(1) - fV0(1)*fV1(0);
  big = M.MaxCoeff();
  if (adet>=un) lim = eps*(big/adet);
  else          lim = big*(eps/adet);
  if (lim<un) {
    ok = kTRUE;
    fV0(0) = M.fV0(0)/det;
    fV0(1) = M.fV0(1)/det;
    fV0(2) = M.fV0(2)/det;
    fV1(0) = M.fV1(0)/det;
    fV1(1) = M.fV1(1)/det;
    fV1(2) = M.fV1(2)/det;
    fV2(0) = M.fV2(0)/det;
    fV2(1) = M.fV2(1)/det;
    fV2(2) = M.fV2(2)/det;
    Clean();
    Error = VerifyInverse(W);
  }
  return ok;
}
TMat33C TMat33C::Invert(TComplex &det,Bool_t &ok) const {
// Calculate the determinant of the matrix and returns the inverse of the matrix.
//Original matrix is preserved. ok is returned true if inversion succeeded.
  TMat33C M(*this);
  ok = M.Invert(det);
  return M;
}
TMat33C TMat33C::Invert(Double_t &Error,TComplex &det,Bool_t &ok) const {
// Calculate the determinant of the matrix and returns the inverse of the matrix.
//Original matrix is preserved. ok is returned true if inversion succeeded.
//Error is an indication of the error when multiplying original matrix with
//inverse matrix, and not obtaining exactly matrix 1.
  TMat33C M(*this);
  ok = M.Invert(Error,det);
  return M;
}
Bool_t TMat33C::IsHermitian() const {
// Test whether "this" is a hermitian matrix, i.e. whether Mt == M.
  return TestBit(TMat33::fgSymTested) && TestBit(TMat33::fgSym);
}
Bool_t TMat33C::IsHermitian(Bool_t ForceTest) {
// Test whether this is a hermitian matrix, i.e. whether Conj(Mt) == M.
// If ForceTest is false, result of a possible previous test is accepted
// If ForceTest is true, test is done in any case
  if ((ForceTest) || (!TestBit(TMat33::fgSymTested))) {
    Double_t eps;
    eps = VerifyHermitian();
  }
  return TestBit(TMat33::fgSym);
}
Bool_t TMat33C::IsUnitarian() const {
// Test whether this is a unitarian matrix, i.e. whether Conj(Mt) == M-1.
  return TestBit(TMat33::fgUniTested) && TestBit(TMat33::fgUni);
}
Bool_t TMat33C::IsUnitarian(Bool_t ForceTest) {
// Test whether this is a unitarian matrix, i.e. whether Conj(Mt) == M-1.
// If ForceTest is false, [default] result of a possible previous test is accepted
// If ForceTest is true, test is done in any case
  if ((ForceTest) || (!TestBit(TMat33::fgUniTested))) {
    Double_t eps;
    eps = VerifyUnitarity();
  }
  return TestBit(TMat33::fgUni);
}
Double_t TMat33C::MaxCoeff() const  {
  // Finds the biggest among the absolute values of all matrix elements
  Short_t k;
  Double_t a;
  Double_t biggest = 0.0;
  for (k=0;k<3;k++) {
    a = fV0(k).Rho();
    if (a>biggest) biggest = a;
  }
  for (k=0;k<3;k++) {
    a = fV1(k).Rho();
    if (a>biggest) biggest = a;
  }
  for (k=0;k<3;k++) {
    a = fV2(k).Rho();
    if (a>biggest) biggest = a;
  }
  return biggest;
}
void TMat33C::Print() const {
  cout << endl;
  fV0.Print();
  fV1.Print();
  fV2.Print();
  cout << endl;
}
void TMat33C::RatherPositiv(TVec3C &v) {
//If the direction of a vector is free, chooses it rather positiv and real  xxx
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t d,d2;
  d  = TMath::Abs(v(0).Re() + v(1).Re() + v(2).Re());
  d2 = TMath::Abs(v(0).Im() + v(1).Im() + v(2).Im());
  if (d2>d) {
    TComplex I(zero,un);
    v(0) = I*v(0);
    v(1) = I*v(1);
    v(2) = I*v(2);
  }
  d = v(0).Re() + v(0).Im() + v(1).Re() + v(1).Im() + v(2).Re() + v(2).Im();
  if (d<zero) {
    v(0) = zero - v(0);
    v(1) = zero - v(1);
    v(2) = zero - v(2);
  }
}
void TMat33C::Set(Double_t d) {
  // Makes "this" to be a diagonal matrix where all diagonal elements are equal to real d
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  SetLine(0,d,zero,zero);
  SetLine(1,zero,d,zero);
  SetLine(2,zero,zero,d);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested,kTRUE);
  if (TMath::Abs(d-un) < TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else                              SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::Set(TComplex z) {
  // Makes "this" a diagonal matrix where all diagonal elements are equal to complex z
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t eps  = 1.0e-307;
  SetLine(0,z,zero,zero);
  SetLine(1,zero,z,zero);
  SetLine(2,zero,zero,z);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUni,kFALSE);
  if (TMath::Abs(z.Im()<eps)) {
    SetBit(TMat33::fgSym,kTRUE);
    if (TMath::Abs(z.Re() - un)<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
void TMat33C::Set(Double_t d00,Double_t d11,Double_t d22) {
  // Makes "this" a diagonal matrix where diagonal elements are equal to real d00,d11,d22
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t eps;
  SetLine(0,d00,zero,zero);
  SetLine(1,zero,d11,zero);
  SetLine(2,zero,zero,d22);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  if (eps < TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else                            SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::Set(TComplex z00,TComplex z11,TComplex z22) {
  // Makes "this" a diagonal matrix where diagonal elements are equal to complex z00,z11,z22
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t eps  = 1.0e-307;
  Double_t cc;
  SetLine(0,z00,zero,zero);
  SetLine(1,zero,z11,zero);
  SetLine(2,zero,zero,z22);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  cc     = TMath::Abs(z00.Im());
  cc    += TMath::Abs(z11.Im());
  cc    += TMath::Abs(z22.Im());
  if (cc<eps) {
    SetBit(TMat33::fgSym,kTRUE);
    cc  = TMath::Abs(z00.Re() - un);
    cc += TMath::Abs(z11.Re() - un);
    cc += TMath::Abs(z22.Re() - un);
    if (cc<eps) SetBit(TMat33::fgUni);
  }
  else SetBit(TMat33::fgSym,kFALSE);
}
void TMat33C::Set(Double_t d00,Double_t d11,Double_t d22,
  TComplex z01,TComplex z02,TComplex z12) {
  // Makes "this" an hermitian matrix with real diagonal elements d00, d11, d22 and
  //non diagonal elements z01, z02, z12
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  fV0(1) = z01;
  fV0(2) = z02;
  fV1(2) = z12;
  fV1(0) = TComplex::Conjugate(z01);
  fV2(0) = TComplex::Conjugate(z02);
  fV2(1) = TComplex::Conjugate(z12);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::Set(const TVector3 &v0,const TVec3C &v1) {
  // Makes "this" a hermitian matrix, where the vector v0 contains the 3 real diagonal
  //elements of the hermitian matrix, and the vector v1 contains the 3 complex non diagonal
  //elements of the matrix
  fV0(0) = v0(0);
  fV1(1) = v0(1);
  fV2(2) = v0(2);
  fV0(1) = v1(0);
  fV0(2) = v1(1);
  fV1(2) = v1(2);
  fV1(0) = TComplex::Conjugate(fV0(1));
  fV2(0) = TComplex::Conjugate(fV0(2));
  fV2(1) = TComplex::Conjugate(fV1(2));
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::Set(const TVec3C &v0,const TVec3C &v1,const TVec3C &v2,Bool_t ascolumn) {
  // Makes "this" having the 3 line vectors v0, v1, v2 if ascolumn is false [default],
  //or the 3 column vectors if ascolumn is true.
  //
  // IMPORTANT NOTICE:
  //
  // The hermitian or unitarian state of the matrix constructed here is NOT
  //tested by this method.
  //If you happen to have entered an hermitian matrix, please either
  //  - immediatly call method VerifyHermitian() in order that the hermitian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is hermitian]
  //      call   -- SetBit(TMat33::fgSymTested);
  //             -- SetBit(TMat33::fgSym);
  //If you happen to have entered a unitarian matrix, please either
  //  - immediatly call method VerifyUnitarity() in order that the unitarian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is unitarian]
  //      call   -- SetBit(TMat33::fgUniTested);
  //             -- SetBit(TMat33::fgUni);
  fV0 = v0;
  fV1 = v1;
  fV2 = v2;
  if (ascolumn) CT();
  SetStateFalse();
}
void TMat33C::Set(const TMat33C &m,Bool_t CcTransp) {
  // Makes "this" identical to m if CcTransp is false [default]. Otherwise
  //construct the complex conjugate transposed of m.
  if (m.TestBit(TMat33::fgUniTested)) SetBit(TMat33::fgUniTested);
  else                                   SetBit(TMat33::fgUniTested,kFALSE);
  if (m.TestBit(TMat33::fgUni))       SetBit(TMat33::fgUni);
  else                                   SetBit(TMat33::fgUni,kFALSE);
  if (m.TestBit(TMat33::fgSymTested)) SetBit(TMat33::fgSymTested);
  else                                   SetBit(TMat33::fgSymTested,kFALSE);
  if (m.TestBit(TMat33::fgSym))       SetBit(TMat33::fgSym);
  else                                   SetBit(TMat33::fgSym,kFALSE);
  fV0 = m.fV0;
  fV1 = m.fV1;
  fV2 = m.fV2;
  if (CcTransp) CT();
}
void TMat33C::Set(const TMat33 &m,Bool_t CcTransp) {
  // Makes "this" identical to m if CcTransp is false [default]. Otherwise construct the
  //transposed of m.
  if (m.TestBit(TMat33::fgUniTested)) SetBit(TMat33::fgUniTested);
  else                                   SetBit(TMat33::fgUniTested,kFALSE);
  if (m.TestBit(TMat33::fgUni))       SetBit(TMat33::fgUni);
  else                                   SetBit(TMat33::fgUni,kFALSE);
  if (m.TestBit(TMat33::fgSymTested)) SetBit(TMat33::fgSymTested);
  else                                   SetBit(TMat33::fgSymTested,kFALSE);
  if (m.TestBit(TMat33::fgSym))       SetBit(TMat33::fgSym);
  else                                   SetBit(TMat33::fgSym,kFALSE);
  fV0 = m.GetLine(0);
  fV1 = m.GetLine(1);
  fV2 = m.GetLine(2);
  if (CcTransp) T();
}
void TMat33C::SetAll(Double_t a) {
// Set all elements of matrix equal to real a
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,a);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::SetAll(TComplex z) {
// Set all elements of matrix equal to complex z
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,z);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym,kFALSE);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::SetColumn(Int_t i,const TVec3C &v) {
  //Defines column i as being the vector v
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = v(0);
  fV1(i) = v(1);
  fV2(i) = v(2);
  SetStateFalse();
}
void TMat33C::SetColumn(Int_t i,Double_t a) {
  //Defines column i as having all elements equal to a
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = a;
  fV1(i) = a;
  fV2(i) = a;
  SetStateFalse();
}
void TMat33C::SetColumn(Int_t i,TComplex a) {
  //Defines column i as having all elements equal to a
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = a;
  fV1(i) = a;
  fV2(i) = a;
  SetStateFalse();
}
void TMat33C::SetColumn(Int_t i,TComplex a,TComplex b,TComplex c) {
  //Defines column i as being the vector v = (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = a;
  fV1(i) = b;
  fV2(i) = c;
  SetStateFalse();
}
void TMat33C::SetLine(Int_t i,const TVec3C &v) {
  //Defines line i as being the complex vector v
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
  case 0:
    fV0(0) = v(0);  fV0(1) = v(1);  fV0(2) = v(2);
    break;
  case 1:
    fV1(0) = v(0);  fV1(1) = v(1);  fV1(2) = v(2);
    break;
  case 2:
    fV2(0) = v(0);  fV2(1) = v(1);  fV2(2) = v(2);
    break;
  }
  SetStateFalse();
}
void TMat33C::SetLine(Int_t i,Double_t a) {
  //Defines line i as having all elements equal to a
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
  case 0:
    fV0(0) = a;  fV0(1) = a;  fV0(2) = a;
    break;
  case 1:
    fV1(0) = a;  fV1(1) = a;  fV1(2) = a;
    break;
  case 2:
    fV2(0) = a;  fV2(1) = a;  fV2(2) = a;
    break;
  }
  SetStateFalse();
}
void TMat33C::SetLine(Int_t i,TComplex a) {
  //Defines line i as having all elements equal to a
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
  case 0:
    fV0(0) = a;  fV0(1) = a;  fV0(2) = a;
    break;
  case 1:
    fV1(0) = a;  fV1(1) = a;  fV1(2) = a;
    break;
  case 2:
    fV2(0) = a;  fV2(1) = a;  fV2(2) = a;
    break;
  }
  SetStateFalse();
}
void TMat33C::SetLine(Int_t i,TComplex a,TComplex b,TComplex c) {
  //Defines line i as being the vector v = (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
  case 0:
    fV0(0) = a;  fV0(1) = b;  fV0(2) = c;
    break;
  case 1:
    fV1(0) = a;  fV1(1) = b;  fV1(2) = c;
    break;
  case 2:
    fV2(0) = a;  fV2(1) = b;  fV2(2) = c;
    break;
  }
  SetStateFalse();
}
void TMat33C::SetStateAs(const TMat33C &m) {
  // Set states of "this" as those of m
  if (m.TestBit(TMat33::fgSymTested)) SetBit(TMat33::fgSymTested);
  else                        SetBit(TMat33::fgSymTested,kFALSE);
  if (m.TestBit(TMat33::fgSym))        SetBit(TMat33::fgSym);
  else                        SetBit(TMat33::fgSym,kFALSE);
  if (m.TestBit(TMat33::fgUniTested)) SetBit(TMat33::fgUniTested);
  else                        SetBit(TMat33::fgUniTested,kFALSE);
  if (m.TestBit(TMat33::fgUni))       SetBit(TMat33::fgUni);
  else                        SetBit(TMat33::fgUni,kFALSE);
}
void TMat33C::SetStateFalse() {
  // All states [unitarian and hermitian] set as undefined
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
}
TMat33C TMat33C::Sin() const {
// Calculates the matrix which is the sinus of "this", on the condition that
//"this" is hermitian
  const Double_t zero = 0.0;
  TMat33C M(zero);
  if (IsHermitian()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVec3C  v0,v1,v2;
    TMat33C P0,P1,P2;
    n = EigenProjectors(L0,L1,L2,v0,v1,v2,P0,P1,P2);
    L0 = TMath::Sin(L0);
    L1 = TMath::Sin(L1);
    L2 = TMath::Sin(L2);
    M  = L0*P0 + L1*P1 + L2*P2;
    M.SetBit(TMat33::fgSymTested);
    M.SetBit(TMat33::fgSym);
  }
  return M;
}
Double_t TMat33C::SolveLin3(const TVec3C &b,TVec3C &x) const {
// Solve equation A*x = b, where A is equal to *this. this is not
//modified. Error in numerical calculation is returned
  Double_t error = 1.0e+307;
  Bool_t ok;
  TComplex det;
  TVec3C v;
  TMat33C A(*this);
  ok = A.Invert(det);
  if (ok) {
    x = A*b;
    v = (*this)*x - b;
    error = v.Mag();
  }
  return error;
}
Double_t TMat33C::SolveLin3(const TMat33C &A,const TVec3C &b,TVec3C &x) {
// Solve equation A*x = b, A is not modified. Error in numerical calculation is returned
  Double_t error = 1.0e+307;
  Bool_t ok;
  TComplex det;
  TVec3C v;
  TMat33C B(A);
  ok = B.Invert(det);
  if (ok) {
    x = B*b;
    v = A*x - b;
    error = v.Mag();
  }
  return error;
}
void TMat33C::T() {
  // Transpose (but DO NOT conjugate) the matrix
  TComplex z;
  z = fV1(0); fV1(0) = fV0(1); fV0(1) = z;
  z = fV2(0); fV2(0) = fV0(2); fV0(2) = z;
  z = fV1(2); fV1(2) = fV2(1); fV2(1) = z;
}
void TMat33C::Unit() {
  //Defines this matrix as being the unit matrix
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  fV0(0) = un;    fV0(1) = zero;  fV0(2) = zero;
  fV1(0) = zero;  fV1(1) = un;    fV1(2) = zero;
  fV2(0) = zero;  fV2(1) = zero;  fV2(2) = un;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni);
}
Double_t TMat33C::VerifyHermitian() const {
//  Returns a number which is an indication of how far "this" is from
//being a hermitian matrix.
  Double_t d;
  TComplex z;
  d  = TMath::Abs(fV0(0).Im());
  d += TMath::Abs(fV1(1).Im());
  d += TMath::Abs(fV2(2).Im());
  z  = TComplex::Conjugate(fV1(0)) - fV0(1);
  d += z.Rho();
  z  = TComplex::Conjugate(fV2(0)) - fV0(2);
  d += z.Rho();
  z  = TComplex::Conjugate(fV2(1)) - fV1(2);
  d += z.Rho();
  return d;
}
Double_t TMat33C::VerifyHermitian(Bool_t force) {
  //  Returns a number which is an indication of how far "this" is from
  //being a hermitian matrix. If force is true and it is found that the matrix
  //is hermitian up to a value of TMat33::fgLimPrec, the matrix is made
  //hermitian.
  Double_t d;
  TComplex z;
  d  = TMath::Abs(fV0(0).Im());
  d += TMath::Abs(fV1(1).Im());
  d += TMath::Abs(fV2(2).Im());
  z  = TComplex::Conjugate(fV1(0)) - fV0(1);
  d += z.Rho();
  z  = TComplex::Conjugate(fV2(0)) - fV0(2);
  d += z.Rho();
  z  = TComplex::Conjugate(fV2(1)) - fV1(2);
  d += z.Rho();
  if (force) {
    SetBit(TMat33::fgSymTested);
    if (d<TMat33::fgLimPrec) {
      Hermitianize();
      SetBit(TMat33::fgSym);
    }
    else SetBit(TMat33::fgSym,kFALSE);
  }
  return d;
}
Double_t TMat33C::VerifyInverse(const TMat33C &W) const {
//  W is assumed to be the inverse of "this". Calculates the product
//(*this)*W and checks how far the result is from the Unit matrix.
//This number is returned and is a measure of how bad the calculation
//of the inverse matrix W was.
//  This method can also be used as a check of the unitarian character
//of the matrix by giving for W the transposed of "this". But for this
//it is simpler to call VerifyUnitarity().
  Double_t d;
  TMat33C U;
  U = W*(*this);
  d = U.CloseUnit();
  return d;
}
Double_t TMat33C::VerifyUnitarity() {
//  Returns a number which is an indication of how far "this" is from
//being a unitarian matrix. If it is found that the matrix is unitarian
//up to a value of TMat33::fgLimPrec, the matrix is considered as unitarian.
//
  Double_t d;
  TMat33C U;
  TMat33C W(*this,kTRUE);
  U = W*(*this);
  d = U.CloseUnit();
  SetBit(TMat33::fgUniTested);
  if (d<TMat33::fgLimPrec) SetBit(TMat33::fgUni);
  else             SetBit(TMat33::fgUni,kFALSE);
  return d;
}
void TMat33C::Zero() {
  //Defines this matrix as being the null matrix
  TVector3 v;
  fV0 = v;
  fV1 = v;
  fV2 = v;
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
}
TMat33C &TMat33C::operator+=(Double_t d) {
  // Add d to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) += d;
  for (k=0;k<3;k++) fV1(k) += d;
  for (k=0;k<3;k++) fV2(k) += d;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator+=(TComplex z) {
  // Add z to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) += z;
  for (k=0;k<3;k++) fV1(k) += z;
  for (k=0;k<3;k++) fV2(k) += z;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator+=(const TMat33C &m) {
  // Add matrix m to this
  fV0 += m.fV0;
  fV1 += m.fV1;
  fV2 += m.fV2;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  if (!(TestBit(TMat33::fgSymTested) && m.TestBit(TMat33::fgSymTested) && TestBit(TMat33::fgSym) && m.TestBit(TMat33::fgSym))) {
    SetBit(TMat33::fgSymTested,kFALSE);
    SetBit(TMat33::fgSym,kFALSE);
  }
  return *this;
}
TMat33C &TMat33C::operator-=(Double_t d) {
  // Subtract d to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) -= d;
  for (k=0;k<3;k++) fV1(k) -= d;
  for (k=0;k<3;k++) fV2(k) -= d;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator-=(TComplex z) {
  // Subtract d to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) -= z;
  for (k=0;k<3;k++) fV1(k) -= z;
  for (k=0;k<3;k++) fV2(k) -= z;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator-=(const TMat33C &m) {
  // Subtract matrix m from this
  fV0 -= m.fV0;
  fV1 -= m.fV1;
  fV2 -= m.fV2;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  if (!(TestBit(TMat33::fgSymTested) && m.TestBit(TMat33::fgSymTested) && TestBit(TMat33::fgSym) && m.TestBit(TMat33::fgSym))) {
    SetBit(TMat33::fgSymTested,kFALSE);
    SetBit(TMat33::fgSym,kFALSE);
  }
  return *this;
}
TMat33C &TMat33C::operator*=(Double_t d) {
  // Multiply by d all elements of matrix
  fV0 *= d;
  fV1 *= d;
  fV2 *= d;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator*=(TComplex z) {
  // Multiply by d all elements of matrix
  fV0 *= z;
  fV1 *= z;
  fV2 *= z;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator*=(const TMat33 &m) {
  // Multiply this by m. Result is this*m, not m*this!
  TMat33C p1(*this);
  TMat33C p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  if (!(TestBit(TMat33::fgUniTested) && m.TestBit(TMat33::fgUniTested) && TestBit(TMat33::fgUni) && m.TestBit(TMat33::fgUni))) {
    SetBit(TMat33::fgUniTested,kFALSE);
    SetBit(TMat33::fgUni,kFALSE);
  }
  return *this;
}
TMat33C &TMat33C::operator*=(const TMat33C &m) {
  // Multiply this by m. Result is this*m, not m*this!
  TMat33C p1(*this);
  TMat33C p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  if (!(TestBit(TMat33::fgUniTested) && m.TestBit(TMat33::fgUniTested) && TestBit(TMat33::fgUni) && m.TestBit(TMat33::fgUni))) {
    SetBit(TMat33::fgUniTested,kFALSE);
    SetBit(TMat33::fgUni,kFALSE);
  }
  return *this;
}
TMat33C &TMat33C::operator&=(Double_t d) {
  // Multiply by d all elements of the hermitian conjugate matrix
  CT();
  fV0 *= d;
  fV1 *= d;
  fV2 *= d;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator&=(TComplex z) {
  // Multiply by z all elements of the hermitian conjugate matrix
  CT();
  fV0 *= z;
  fV1 *= z;
  fV2 *= z;
  SetBit(TMat33::fgUniTested,kFALSE);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator&=(const TMat33 &m) {
  // Conjugate + Transpose this and then multiply it by m. Result is Conj(thisT)*m,
  //not m*Conj(thisT)!
  TMat33C p1(*this,kTRUE);
  TMat33C p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  if (!(TestBit(TMat33::fgUniTested) && m.TestBit(TMat33::fgUniTested) && TestBit(TMat33::fgUni) && m.TestBit(TMat33::fgUni))) {
    SetBit(TMat33::fgUniTested,kFALSE);
    SetBit(TMat33::fgUni,kFALSE);
  }
  return *this;
}
TMat33C &TMat33C::operator&=(const TMat33C &m) {
  // Conjugate + Transpose this and then multiply it by m. Result is Conj(thisT)*m,
  //not m*Conj(thisT)!
  TMat33C p1(*this,kTRUE);
  TMat33C p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(TMat33::fgSymTested,kFALSE);
  SetBit(TMat33::fgSym,kFALSE);
  if (!(TestBit(TMat33::fgUniTested) && m.TestBit(TMat33::fgUniTested) && TestBit(TMat33::fgUni) && m.TestBit(TMat33::fgUni))) {
    SetBit(TMat33::fgUniTested,kFALSE);
    SetBit(TMat33::fgUni,kFALSE);
  }
  return *this;
}
TMat33C TMat33C::operator+(Double_t d) const {
  //Returns a matrix equal to "*this" with d added to all elements
  TMat33C m4(*this);
  m4 += d;
  return m4;
}
TMat33C TMat33C::operator+(TComplex z) const {
  //Returns a matrix equal to "*this" with z added to all elements
  TMat33C m4(*this);
  m4 += z;
  return m4;
}
TMat33C TMat33C::operator+(const TMat33C &m) const {
  //Returns a matrix equal to "*this" added with matrix m
  TMat33C m4(*this);
  m4 += m;
  return m4;
}
TMat33C TMat33C::operator-(Double_t d) const {
  //Returns a matrix equal to "*this" with d subtracted from all elements
  TMat33C m4(*this);
  m4 -= d;
  return m4;
}
TMat33C TMat33C::operator-(TComplex z) const {
  //Returns a matrix equal to "*this" with z subtracted from all elements
  TMat33C m4(*this);
  m4 -= z;
  return m4;
}
TMat33C TMat33C::operator-(const TMat33C &m) const {
  //Returns a matrix equal to "*this" minus matrix m
  TMat33C m4(*this);
  m4 -= m;
  return m4;
}
TMat33C TMat33C::operator*(Double_t d) const {
  //Returns a matrix equal to "*this" with all elements multiplied by d
  TMat33C m4(*this);
  m4 *= d;
  return m4;
}
TMat33C TMat33C::operator*(TComplex z) const {
  //Returns a matrix equal to "*this" with all elements multiplied by z
  TMat33C m4(*this);
  m4 *= z;
  return m4;
}
TMat33C TMat33C::operator*(const TMat33 &m) const {
  //Returns a matrix equal to "*this" multiplied by matrix m
  TMat33C m4(*this);
  m4 *= m;
  return m4;
}
TMat33C TMat33C::operator*(const TMat33C &m) const {
  //Returns a matrix equal to "*this" multiplied by matrix m
  TMat33C m4(*this);
  m4 *= m;
  return m4;
}
TMat33C TMat33C::operator&(Double_t d) const {
  // Returns a matrix equal to hermitian conjugate of "*this" with all elements
  //multiplied by d
  TMat33C m4(*this,kTRUE);
  m4 *= d;
  return m4;
}
TMat33C TMat33C::operator&(TComplex z) const {
  // Returns a matrix equal to hermitian conjugate of "*this" with all elements
  //multiplied by z
  TMat33C m4(*this,kTRUE);
  m4 *= z;
  return m4;
}
TMat33C TMat33C::operator&(const TMat33 &m) const {
  // Returns a matrix equal to hermitian conjugate of "*this" multiplied with matrix m
  TMat33C m4(*this,kTRUE);
  m4 *= m;
  return m4;
}
TMat33C TMat33C::operator&(const TMat33C &m) const {
  // Returns a matrix equal to hermitian conjugate of "*this" multiplied with matrix m
  TMat33C m4(*this,kTRUE);
  m4 *= m;
  return m4;
}
TMat33C &TMat33C::operator=(Double_t d) {
  //all elements of *this set equal to d
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,d);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym);
  return *this;
}
TMat33C &TMat33C::operator=(TComplex z) {
  //all elements of *this set equal to z
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,z);
  SetBit(TMat33::fgUniTested);
  SetBit(TMat33::fgUni,kFALSE);
  SetBit(TMat33::fgSymTested);
  SetBit(TMat33::fgSym,kFALSE);
  return *this;
}
TMat33C &TMat33C::operator=(const TMat33C &m) {
  //*this becomes identical to m
  if (this != &m) {
    TObject::operator=(m);
    fV0 = m.fV0;
    fV1 = m.fV1;
    fV2 = m.fV2;
    SetStateAs(m);
  }
  return *this;
}
Bool_t TMat33C::operator==(const TMat33& v) const {
// Check whether *this is equal to v, in which case *this is real
  Bool_t isequal;
  isequal = (fV0 == v(0)) && (fV1 == v(1)) && (fV2 == v(2));
  return isequal;
}
Bool_t TMat33C::operator==(const TMat33C& v) const {
// Check whether "this" is equal to v
  Bool_t isequal;
  isequal = (fV0 == v.fV0) && (fV1 == v.fV1) && (fV2 == v.fV2);
  return isequal;
}
Bool_t TMat33C::operator>(const TMat33C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Determinant().Rho() > v.Determinant().Rho());
}
Bool_t TMat33C::operator>=(const TMat33C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Determinant().Rho() >= v.Determinant().Rho());
}
Bool_t TMat33C::operator<(const TMat33C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Determinant().Rho() < v.Determinant().Rho());
}
Bool_t TMat33C::operator<=(const TMat33C &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
  return (Determinant().Rho() <= v.Determinant().Rho());
}
TComplex TMat33C::operator()(Int_t i,Int_t j) const {
  // Access to element (i,j) of matrix
  //
  // STRANGE!
  //
  // This method is never called, neither in case
  //  (1) - d = M(i,j);         //nor in case
  //  (2) - M(i,j) = d;
  // It is always the next method which is called! I would expect that this method
  //be called in case (1) ???
  //
  TComplex d;
  i  = TMath::Abs(i);
  i %= 3;
  j  = TMath::Abs(j);
  j %= 3;
  switch (i) {
    case 0:
      d = fV0(j);
      break;
    case 1:
      d = fV1(j);
      break;
    case 2:
      d = fV2(j);
      break;
  }
  return d;
}
TComplex &TMat33C::operator()(Int_t i,Int_t j,Bool_t change) {
  // Access to element (i,j) of matrix
  //
  // Important notice !!!
  //
  // If M is a TMat33C, d a Double_t and i,j indeces [0-2], if you do
  //
  //  (1) -  d = M(i,j);    // There is no risk that the state of the
  //                        matrix (hermitian or unitarian) be changed,
  //        so this call is ok, the default value of change being false.
  //
  //  (2) -  M(i,j) = d;      // The state of the matrix will be changed
  //                        by the operation, so never do (2) ! Rather do:
  //  (3) -  M(i,j,kTRUE) = d;
  //
  TComplex *d = 0;
  i  = TMath::Abs(i);
  i %= 3;
  j  = TMath::Abs(j);
  j %= 3;
  switch (i) {
    case 0:
      d  = &fV0(j);
      break;
    case 1:
      d  = &fV1(j);
      break;
    case 2:
      d  = &fV2(j);
      break;
  }
  if (change) SetStateFalse();
  return *d;
}
TVec3C TMat33C::operator()(Int_t i) const {
  // Returns line i of matrix
  //
  // STRANGE!
  //
  // This method is never called, neither in case
  //  (1) - v = M(i);         //nor in case
  //  (2) - M(i) = v;
  // It is always the next method which is called! I would expect that this method
  //be called in case (1) ???
  //
  TVec3C v;
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
    case 0:
      v = fV0;
      break;
    case 1:
      v = fV1;
      break;
    case 2:
      v = fV2;
      break;
  }
  return v;
}
TVec3C &TMat33C::operator()(Int_t i,Bool_t change)  {
  // Returns line i of matrix
  //
  // Important notice !!!
  //
  // If M is a TMat33C, v a TVector3 and i an index [0-2], if you do
  //
  //  (1) -  v = M(i);      // There is no risk that the state of the
  //                        matrix (hermitian or unitarian) be changed,
  //        so this call is ok, the default value of change being false.
  //
  //  (2) -  M(i) = v;      // The state of the matrix will be changed
  //                        by the operation, so never do (2) ! Rather do:
  //  (3) -  M(i,kTRUE) = v;
  //
  TVec3C *v = 0;
  i  = TMath::Abs(i);
  i %= 3;
  switch (i) {
    case 0:
      v = &fV0;
      break;
    case 1:
      v = &fV1;
      break;
    case 2:
      v = &fV2;
      break;
  }
  if (change) SetStateFalse();
  return *v;
}
TMat33C operator+(Double_t d, const TMat33C &m) {
  //Addition of d to all elements of matrix
  TMat33C p(m);
  p += d;
  return p;
}
TMat33C operator+(TComplex z, const TMat33C &m) {
  //Addition of z to all elements of matrix
  TMat33C p(m);
  p += z;
  return p;
}
TMat33C operator-(Double_t d, const TMat33C &m) {
  //All elements of matrix equal to d - element
  TMat33C p(m);
  p.ChangeSign();
  p += d;
  return p;
}
TMat33C operator-(TComplex z, const TMat33C &m) {
  //All elements of matrix equal to z - element
  TMat33C p(m);
  p.ChangeSign();
  p += z;
  return p;
}
TMat33C operator*(Double_t d, const TMat33C &m) {
  //Multiplication of a matrix by a real number
  TMat33C p(m);
  p *= d;
  return p;
}
TMat33C operator*(TComplex z, const TMat33C &m) {
  //Multiplication of a matrix by a real number
  TMat33C p(m);
  p *= z;
  return p;
}
TMat33C operator*(const TMat33 &M, const TMat33C &Mc) {
  //Multiplication of a matrix by a [column] vector
  TMat33C W(M);
  W *= Mc;
  return W;
}
TVec3C operator*(const TMat33C &m, const TVector3 &v) {
  //Multiplication of a matrix by a [column] vector
  TVec3C w;
  w(0) = m.fV0&v;
  w(1) = m.fV1&v;
  w(2) = m.fV2&v;
  return w;
}
TVec3C operator*(const TMat33C &m, const TVec3C &v) {
  //Multiplication of a matrix by a [column] vector
  TVec3C w;
  w(0) = m.fV0&v;
  w(1) = m.fV1&v;
  w(2) = m.fV2&v;
  return w;
}
TVec3C operator*(const TVector3 &v,const TMat33C &m) {
  // Multiplication of a [line] vector by a matrix. We call line vector the
  //complex conjugate transposed vector
  TVec3C w;
  TMat33C mt(m);
  mt.T();
  w(0) = v*mt.fV0;
  w(1) = v*mt.fV1;
  w(2) = v*mt.fV2;
  return w;
}
TVec3C operator*(const TVec3C &v,const TMat33C &m) {
  //Multiplication of a [line] vector by a matrix. We call line vector the
  //complex conjugate transposed vector
  TVec3C w;
  TMat33C mt(m);
  mt.T();
  w(0) = v*mt.fV0;
  w(1) = v*mt.fV1;
  w(2) = v*mt.fV2;
  return w;
}
TMat33C operator&(Double_t d,const TMat33C &m) {
  // Returns a matrix equal to hermitian conjugate of m with all elements
  //multiplied by d
  TMat33C p(m,kTRUE);
  p *= d;
  return p;
}
TMat33C operator&(TComplex z,const TMat33C &m) {
  // Returns a matrix equal to hermitian conjugate of m with all elements
  //multiplied by d
  TMat33C p(m,kTRUE);
  p *= z;
  return p;
}
TMat33C operator&(const TMat33 &M,const TMat33C &Mc) {
  // Returns a matrix equal to conjugate of M multiplied by Mc
  TMat33C P(M,kTRUE);
  P *= Mc;
  return P;
}
Bool_t TMat33C::MyRootsCubic(const Double_t coef[4],Double_t &a, Double_t &b, Double_t &c) {
  // Calculates roots of polynomial of 3rd order a*x^3 + b*x^2 + c*x + d, where
  // a == coef[3], b == coef[2], c == coef[1], d == coef[0]
  //coef[3] must be different from 0
  // If the boolean returned by the method is false:
  //    ==> there are 3 real roots a,b,c
  // If the boolean returned by the method is true:
  //    ==> there is one real root a and 2 complex conjugates roots (b+i*c,b-i*c)
  //
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t deux  = 2.0;
  const Double_t trois = 3.0;
  Bool_t complex = kFALSE;
  Double_t r,s,t,p,q,d,ps3,ps33,qs2,u,v,tmp,lnu,lnv,su,sv,y1,y2,y3;
  a    = zero;
  b    = zero;
  c    = zero;
  if (coef[3] == 0) return complex;
  r    = coef[2]/coef[3];
  s    = coef[1]/coef[3];
  t    = coef[0]/coef[3];
  p    = s - (r*r)/trois;
  ps3  = p/trois;
  q    = (deux*r*r*r)/27.0 - (r*s)/trois + t;
  qs2  = q/deux;
  ps33 = ps3*ps3*ps3;
  d    = ps33 + qs2*qs2;
  if (d>=zero) {
    complex = kTRUE;
    d   = TMath::Sqrt(d);
    u   = -qs2 + d;
    v   = -qs2 - d;
    tmp = un/trois;
    lnu = TMath::Log(TMath::Abs(u));
    lnv = TMath::Log(TMath::Abs(v));
    su  = TMath::Sign(un,u);
    sv  = TMath::Sign(un,v);
    u   = su*TMath::Exp(tmp*lnu);
    v   = sv*TMath::Exp(tmp*lnv);
    y1  = u + v;
    y2  = -y1/deux;
    y3  = ((u-v)*TMath::Sqrt(trois))/deux;
    tmp = r/trois;
    a   = y1 - tmp;
    b   = y2 - tmp;
    c   = y3;
  }
  else {
    Double_t phi,cphi,phis3,c1,c2,c3,pis3;
    ps3   = -ps3;
    ps33  = -ps33;
    cphi  = -qs2/TMath::Sqrt(ps33);
    phi   = TMath::ACos(cphi);
    phis3 = phi/trois;
    pis3  = TMath::Pi()/trois;
    c1    = TMath::Cos(phis3);
    c2    = TMath::Cos(pis3 + phis3);
    c3    = TMath::Cos(pis3 - phis3);
    tmp   = TMath::Sqrt(ps3);
    y1    = deux*tmp*c1;
    y2    = -deux*tmp*c2;
    y3    = -deux*tmp*c3;
    tmp = r/trois;
    a   = y1 - tmp;
    b   = y2 - tmp;
    c   = y3 - tmp;
  }
  return complex;
}


















