// @(#)root/html:$Id: TMat33.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TMat33.h"

Int_t    TMat33::fgWidth     = 20;
Int_t    TMat33::fgPrec      = 12;
UInt_t   TMat33::fgSymTested = 0x10000;
UInt_t   TMat33::fgSym       = 0x20000;
UInt_t   TMat33::fgUniTested = 0x40000;
UInt_t   TMat33::fgUni       = 0x80000;
Double_t TMat33::fgLimPrec   = 1.0e-12;

ClassImp(TMat33)
//______________________________________________________________________________
//
// Implement 3X3 real matrices associated with the TVector3 of ROOT and also
//  with TVec3C
//
// TMat33 has over TMatrixD of ROOT the advantage of being of fixed
//  dimension, so more suited to TVector3.
// TMat33 has over SMatrix of ROOT the advantage of deriving from TObject
//  so that it can be stored into ROOT collections and into ROOT files.
// TMat33 has over TRotation of ROOT the advantage of being a general
//  matrix, not only a rotation.
//
// a TMat33 can be a general matrix, a unitarian (rotation) matrix, or
//  a symmetric matrix. This is indicated by the bits 17 and 18 of TMat33::fBits,
//  which have the following meaning:
//
//   Bit 17   Bit 18  Meaning
//
//      0        x    symmetric nature of matrix not tested
//      1        0    matrix is not symmetric  Mt != M
//      1        1    matrix si symmetric      Mt == M
//
//
//   Bit 19   Bit 20  Meaning
//
//      0        x    unitarian nature of matrix not tested
//      1        0    matrix is not unitarian Mt != M-1 (is not a rotation matrix)
//      1        1    matrix si unitarian     Mt == M-1 (is a rotation matrix)
//
// TMat33 has a special constructor to build a rotation matrix from the Euler
//  angles. See the 4th constructor. Have also a look at the class TEuler to go
//  easily from GEANT3 angles to Euler angles and vice-versa.
// The 5th and 6th constructor are for building symmetric matrices
// The 7th constructor builds a general matrix, neither symmetric, nor unitarian.
// The 8th constructor, the copy constructor, also allows to build the transposed
//  of the matrix to copy.
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
TMat33::TMat33(Double_t d) {
  // Constructor of a diagonal matrix where all diagonal elements are equal to d
  const Double_t un = 1.0;
  fV0(0) = d;
  fV1(1) = d;
  fV2(2) = d;
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested,kTRUE);
  if (TMath::Abs(d-un) < fgLimPrec) SetBit(fgUni);
  else                              SetBit(fgUni,kFALSE);
}
TMat33::TMat33(Double_t d00,Double_t d11,Double_t d22) {
  // Constructor of a diagonal matrix where diagonal elements are equal to d00,d11,d22
  const Double_t un = 1.0;
  Double_t eps;
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  SetBit(fgSymTested);
  SetBit(fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  SetBit(fgUniTested);
  if (eps < fgLimPrec) SetBit(fgUni);
  else                 SetBit(fgUni,kFALSE);
}
TMat33::TMat33(Double_t phi,Double_t theta,Double_t psi,Bool_t deg) {
// Constructor of the rotation matrix given by the 3 Euler angles (phi,theta,psi).
// If deg is true, the angles are given in degree, else in radians
//
//   The Euler representation uses 3 angles: (phi,theta,psi). We have
// (1) - rotation about the Oz axis with the angle phi
// (2) - a rotation of angle theta around the new Ox axis
// (3) - a rotation of angle psi around the new Oz axis
//   All rotations in trigonometric sense (counter-clockwise)
//   If we define the following abbreviations:
//      f  == phi,     t == theta,    p == psi,
//      sf == Sin(f)  st == Sin(t)   sp == Sin(p)
//      cf == Cos(f)  ct == Cos(t)   cp == Cos(p)
//   then the Euler matrix giving the new axis when applied to the old axis is:
//
//      | cp*cf - sp*ct*sf , -sp*cf - cp*ct*sf ,  st*sf |
//  M = | cp*sf + sp*ct*cf , -sp*sf + cp*ct*cf , -st*cf |
//      | sp*st            ,  cp*st            ,  ct    |
//
  const Double_t zero  = 0.0;
  const Double_t pideg = 180.0;
  const Double_t torad = TMath::Pi()/pideg;
  Double_t twopi;
  Double_t sphi,cphi,stheta,ctheta,spsi,cpsi;
  SetBit(fgSymTested);
  SetBit(fgSym,kFALSE);
  SetBit(fgUniTested);
  SetBit(fgUni);
  if (deg) twopi = 2*pideg;
  else     twopi = 2*TMath::Pi();
  if (phi < zero) {
    Error("TMat33","WARNING: phi must be >= 0. 2 pi repeatedly added");
    while (phi<zero)   phi  += twopi;
  }
  if (phi >= twopi) {
    Error("TMat33","WARNING: phi must be < 2 pi. 2 pi repeatedly subtracted");
    while (phi>=twopi) phi -= twopi;
  }
  if (theta < zero) {
    Error("TMat33","WARNING: theta must be >= 0. 2 pi repeatedly added");
    while (theta<zero)   theta  += twopi;
  }
  if (theta >= twopi) {
    Error("TMat33","WARNING: theta must be < 2 pi. 2 pi repeatedly subtracted");
    while (theta>=twopi) theta -= twopi;
  }
  if (psi < zero) {
    Error("TMat33","WARNING: psi must be >= 0. 2 pi repeatedly added");
    while (psi<zero)   psi  += twopi;
  }
  if (psi >= twopi) {
    Error("TMat33","WARNING: psi must be < 2 pi. 2 pi repeatedly subtracted");
    while (psi>=twopi) psi -= twopi;
  }
  if (deg) {
    phi    *= torad;
    theta  *= torad;
    psi    *= torad;
  }
  sphi    = TMath::Sin(phi);
  cphi    = TMath::Cos(phi);
  stheta  = TMath::Sin(theta);
  ctheta  = TMath::Cos(theta);
  spsi    = TMath::Sin(psi);
  cpsi    = TMath::Cos(psi);
  fV0(0)  =  cpsi*cphi - spsi*ctheta*sphi;
  fV1(0)  =  cpsi*sphi + spsi*ctheta*cphi;
  fV2(0)  =  stheta*spsi;
  fV0(1)  = -spsi*cphi - cpsi*ctheta*sphi;
  fV1(1)  = -spsi*sphi + cpsi*ctheta*cphi;
  fV2(1)  =  stheta*cpsi;
  fV0(2)  =  sphi*stheta;
  fV1(2)  = -cphi*stheta;
  fV2(2)  =  ctheta;
}
TMat33::TMat33(Double_t d00,Double_t d11,Double_t d22,
  Double_t d01,Double_t d02,Double_t d12) {
  // Constructor of a symmetric matrix
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  fV0(1) = d01;
  fV0(2) = d02;
  fV1(2) = d12;
  fV1(0) = d01;
  fV2(0) = d02;
  fV2(1) = d12;
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
TMat33::TMat33(TVector3 v0,TVector3 v1,Bool_t &parallel,Double_t &change) {
  // Given the vectors v0 = (x0,y0,z0) and v1 = (x1,y1,z1) builds the matrix with
  //1st line = v, 2nd line = v0, 3rd line = v1, such that
  //
  // (1) - v is a unit vector
  // (2) - v is orthogonal to v1
  // (3) - v is as close as possible to v0
  // (4) - v0 and v1 are renormalized to length 1
  //
  // If v0 is parallel to v1, the solution is not unique. In that case,
  //parallel is returned true. change = 1-v*v0 is returned. It was the quantity
  //to minimize!
  // This rather exotic constructor solves a vector problem, not a matrix problem.
  //Using a matrix to solve this problem is handy, since it gives the 3 vectors to
  //be found or modified as the 3 lines of the matrix, which you can access using
  //method GetLine().
  //
  const Double_t eps = 1.0e-12;
  Double_t l1,l2,x,y,z,x0,y0,z0,x1,y1,z1,a,b,c,d;
  TVector3 v;
  v0 = v0.Unit();
  v1 = v1.Unit();
  x0 = v0(0);
  y0 = v0(1);
  z0 = v0(2);
  x1 = v1(0);
  y1 = v1(1);
  z1 = v1(2);
  d = 1.0 - v0*v1;
  if (d<eps) {
    TVector3 v2;
    cout << "E TMat33::TMAt33 Error: v0 and v1 parallel" << endl;
    parallel = kTRUE;
    if (TMath::Abs(x1)<TMath::Abs(y1)) {
      if (TMath::Abs(x1)<TMath::Abs(z1)) v2.SetXYZ(1.0,0.0,0.0);
      else                               v2.SetXYZ(0.0,0.0,1.0);
    }
    else {
      if (TMath::Abs(y1)<TMath::Abs(z1)) v2.SetXYZ(0.0,1.0,0.0);
      else                               v2.SetXYZ(0.0,0.0,1.0);
    }
    v = v2.Cross(v1);
    v = v.Unit();
    x = v(0);
    y = v(1);
    z = v(2);
  }
  else {
    parallel = kFALSE;
    l2   = x0*x1+y0*y1+z0*z1;
    a    = x0-l2*x1;
    b    = y0-l2*y1;
    c    = z0-l2*z1;
    l1   = TMath::Sqrt(a*a+b*b+c*c);
    x    = a/l1;
    y    = b/l1;
    z    = c/l1;
    v(0) = x;
    v(1) = y;
    v(2) = z;
  }
  change = 1.0-x*x0-y*y0-z*z0;
  Set(v,v0,v1);
}
TMat33::TMat33(const TVector3 &v0,const TVector3 &v1) {
  // Constructor of a symmetric matrix, where the vector v0 contains the 3
  //diagonal elements of the symmetric matrix, and the vector v1 contains
  //the 3 non diagonal elements of the matrix
  fV0(0) = v0(0);
  fV1(1) = v0(1);
  fV2(2) = v0(2);
  fV0(1) = v1(0);
  fV0(2) = v1(1);
  fV1(2) = v1(2);
  fV1(0) = fV0(1);
  fV2(0) = fV0(2);
  fV2(1) = fV1(2);
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
TMat33::TMat33(const TVector3 &v0,const TVector3 &v1,const TVector3 &v2,Bool_t ascolumn) {
  // Constructor with 3 line vectors if ascolumn is false [default], or with 3 column
  //vectors if ascolumn is true.
  //
  // IMPORTANT NOTICE:
  //
  // The symmetric or unitarian state of the matrix constructed here is NOT
  //tested by this constructor.
  //If you happen to have entered a symmetric matrix, please either
  //  - immediatly call method VerifySymmetric() in order that the symmetric
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is symmetric]
  //      call   -- SetBit(fgSymTested);
  //             -- SetBit(fgSym);
  //If you happen to have entered a unitarian matrix, please either
  //  - immediatly call method VerifyUnitarity() in order that the unitarian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is unitarian]
  //      call   -- SetBit(fgUniTested);
  //             -- SetBit(fgUni);
  fV0 = v0;
  fV1 = v1;
  fV2 = v2;
  if (ascolumn) T();
  SetStateFalse();
}
TMat33::TMat33(const TMat33 &m,Bool_t TransposeIt) {
  // Copy constructor if TransposeIt is false [default]. Otherwise
  //construct the transposed of m.
  if (m.TestBit(fgUniTested)) SetBit(fgUniTested);
  else                        SetBit(fgUniTested,kFALSE);
  if (m.TestBit(fgUni))       SetBit(fgUni);
  else                        SetBit(fgUni,kFALSE);
  if (m.TestBit(fgSymTested)) SetBit(fgSymTested);
  else                        SetBit(fgSymTested,kFALSE);
  if (m.TestBit(fgSym))       SetBit(fgSym);
  else                        SetBit(fgSym,kFALSE);
  fV0 = m.fV0;
  fV1 = m.fV1;
  fV2 = m.fV2;
  if (TransposeIt) T();
}
void TMat33::ChangeSign() {
  //All elements of matrix do change sign
  Short_t k;
  for (k=0;k<3;k++) fV0(k) = -fV0(k);
  for (k=0;k<3;k++) fV1(k) = -fV1(k);
  for (k=0;k<3;k++) fV2(k) = -fV2(k);
}
void TMat33::Clean() {
// Replaces the values very close to 0 by 0
  const Double_t zero = 0.0;
  Short_t k;
  Double_t D,eps;
  D = MaxCoeff();
  eps = D*fgLimPrec;
  for (k=0;k<3;k++) if (TMath::Abs(fV0(k))<eps) fV0(k) = zero;
  for (k=0;k<3;k++) if (TMath::Abs(fV1(k))<eps) fV1(k) = zero;
  for (k=0;k<3;k++) if (TMath::Abs(fV2(k))<eps) fV2(k) = zero;
}
Double_t TMat33::CloseUnit() const {
  //Test how close from the unit matrix
  const Double_t un   = 1.0;
  Double_t d;
  d  = TMath::Abs(fV0(0)-un);
  d += TMath::Abs(fV0(1));
  d += TMath::Abs(fV0(2));
  d += TMath::Abs(fV1(0));
  d += TMath::Abs(fV1(1)-un);
  d += TMath::Abs(fV1(2));
  d += TMath::Abs(fV2(0));
  d += TMath::Abs(fV2(1));
  d += TMath::Abs(fV2(2)-un);
  return d;
}
Int_t TMat33::Compare(const TObject *obj) const {
// Compare according to absolute value of determinant. Allows to sort matrices in a collection
  if ( *this < *((TMat33*)obj) ) return -1;
  else {
    if ( *this > *((TMat33*)obj) ) return 1;
    else                              return 0;
  }
}
TMat33 TMat33::Cos() const {
// Calculates the matrix which is the cosinus of "this", on the condition that
//"this" is symmetric
  const Double_t zero = 0.0;
  TMat33 M(zero);
  if (IsSymmetric()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVector3  v0,v1,v2;
    TMat33 P0,P1,P2;
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
Short_t TMat33::DegMatrix(TVector3 &x,TVector3 &y,Bool_t test) const {
  //  Solve the equation A*x = 0 assuming that matrix A is degenerate. There are
  // 3 ways to get vector x. We use the 3, with a ponderate method. The weights
  // used are the sinus squared of the angle between 2 vectors. It is natural,
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
  //                 (3) the determinant of the matrix is returned in y[0]
  //                 (4) the value of |A*x| is returned in y[1]
  // If 1 returned, matrix M is once degenerate, normal case. There is
  //                only 1 vector as solution x. Vector y is 0.
  // If 2 returned, matrix M twice degenerate. All vectors in a certain plane
  //                are solutions. We arbitrary return 2 orthogonal vectors in
  //                this plane as solution : vector x and y. Remember in that
  //                case that any linear combination of these 2 vectors is also
  //                a solution.
  // If 3 returned, matrix M is the 0 matrix. Any vector is solution, so that
  //                it does not make sense to return something in x or y. We
  //                return x=0 and y=0.
  //
  // When not 0, the return vectors are always normalized to 1
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t small  = 1.0e-5;
  const Double_t vsmall = 1.0e-24;
  const Double_t ltest  = 5.0*small;
  Short_t deg = 1;
  Double_t a,b,c,d;
  Bool_t   b0,b1,b2;
  Bool_t   gen = kFALSE;
  Double_t w1,w2,w;
  Double_t w0 = zero;
  TVector3 x0,x1,x2,p;
  d = TMath::Abs(Determinant());
  x.SetXYZ(zero,zero,zero);
  y.SetXYZ(zero,zero,zero);
  if (d>small) {
    deg = 0;
    y[0] = d;
  }
  GetLine(0,a,b,c);
  TVector3 m0(a,b,c);
  GetLine(1,a,b,c);
  TVector3 m1(a,b,c);
  GetLine(2,a,b,c);
  TVector3 m2(a,b,c);
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
    w0 = zero;
  }
  else {
    b0 = kTRUE;
    m0 = m0.Unit();
  }
  if (b<vsmall) {
    b1 = kFALSE;
    w1 = zero;
  }
  else {
    b1 = kTRUE;
    m1 = m1.Unit();
  }
  if (c<vsmall) {
    b2 = kFALSE;
    w2 = zero;
  }
  else {
    b2 = kTRUE;
    m2 = m2.Unit();
  }
  if (b0) {
//m0 usable
    if (b1) {
//m0 && m1 usable
      if (b2) {
//m0 && m1 && m2 usable          : +0 +1 +2
        x0  = m0^m1;
        x1  = m0^m2;
        x2  = m1^m2;
        w0  = a*b*x0.Mag2();
        w1  = a*c*x1.Mag2();
        w2  = b*c*x2.Mag2();
        w   = w0 + w1 + w2;
        w0 /= w;
        w1 /= w;
        w2 /= w;
        x0  = x0.Unit();
        x1  = x1.Unit();
        x2  = x2.Unit();
        TMat33::RatherPositiv(x0);
        TMat33::RatherPositiv(x1);
        TMat33::RatherPositiv(x2);
        x  = w0*x0 + w1*x1 + w2*x2;
      }
      else {
//m0 && m1 usable, but not m2    : +0 +1 -2
        x0 = m0.Cross(m1);
        w0 = x0.Mag2();
        if (w0<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x0 = x0.Unit();
          TMat33::RatherPositiv(x0);
          x = x0;
        }
      }
    }
    else {
//m0 usable, but not m1
      if (b2) {
//m0 && m2 usable, but not m1    : +0 -1 +2
        x1 = m0.Cross(m2);
        w1 = x1.Mag2();
        if (w1<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x1 = x1.Unit();
          TMat33::RatherPositiv(x1);
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
        x2 = m1.Cross(m2);
        w2 = x2.Mag2();
        if (w2<vsmall) {
          deg = 2;
          p   = m1;
          gen = kTRUE;
        }
        else {
          x2 = x2.Unit();
          TMat33::RatherPositiv(x2);
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
    a  = TMath::Abs(p*x0);
    b  = TMath::Abs(p*x1);
    c  = TMath::Abs(p*x2);
    if (a<=b) {
      if (a<=c) {
        x = p.Cross(x0);
        x = x.Unit();
        y = p.Cross(x);
        y = y.Unit();
      }
      else {
        x = p.Cross(x2);
        x = x.Unit();
        y = p.Cross(x);
        y = y.Unit();
      }
    }
    else {
      if (b<=c) {
        x = p.Cross(x1);
        x = x.Unit();
        y = p.Cross(x);
        y = y.Unit();
      }
      else {
        x = p.Cross(x2);
        x = x.Unit();
        y = p.Cross(x);
        y = y.Unit();
      }
    }
  }
  if ((deg==0) || (test)) {
    switch (deg) {
    case 0:
    case 1:
      x0   = (*this)*x;
      w0   = x0.Mag();
      y[1] = w0;
      break;
    case 2:
      x0 = (*this)*x;
      x1 = (*this)*y;
      w0 = x0.Mag() + x1.Mag();
      break;
    default:
      break;
    }
    if ((test) && (w0>ltest)) cout << "W TMat33::DegMatrix Bad solution w0 = " << w0 << endl;
  }
  return deg;
}
Double_t TMat33::Determinant() const {
  //Returns the determinant of this matrix
  Double_t d;
  d = fV0*fV1.Cross(fV2);
  return d;
}
void TMat33::Diagonal(Double_t d00,Double_t d11,Double_t d22) {
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
  SetBit(fgSymTested);
  SetBit(fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  SetBit(fgUniTested);
  if (eps < fgLimPrec) SetBit(fgUni);
  else                 SetBit(fgUni,kFALSE);
}
void TMat33::Diagonalize() {
  //Clears the non diagonal elements
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t eps;
  fV1(0) = zero;
  fV2(0) = zero;
  fV2(1) = zero;
  fV0(1) = zero;
  fV0(2) = zero;
  fV1(2) = zero;
  SetBit(fgSymTested);
  SetBit(fgSym);
  eps  = TMath::Abs(fV0(0)-un);
  eps += TMath::Abs(fV1(1)-un);
  eps += TMath::Abs(fV2(2)-un);
  SetBit(fgUniTested);
  if (eps < fgLimPrec) SetBit(fgUni);
  else                 SetBit(fgUni,kFALSE);
}
Short_t TMat33::EigenValues(Double_t &L0,Double_t &L1,Double_t &L2) const {
  // Finds the eigenvalues, even in case matrix is not symmetric. The eigenvalues
  //are returned in increasing order.
  // Returns n = 10*n0 + n1, where
  //  - n0 : number of eigenvalues equal to 0
  //  - n1 : number of DIFFERENT eigenvalues
  // If there are complex eigenvalues, returns -n instead of n, and returns
  //real part of first complex eigenvalue in L1, and imaginary part in L2
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t epsc = 5.0e-7;
  Short_t n,n0,n1;
  Bool_t complex;
  Double_t D,eps;
  Double_t coef[4];
  coef[0] = fV0(2)*fV2(0)*fV1(1) + fV0(0)*fV1(2)*fV2(1) + fV0(1)*fV1(0)*fV2(2) -
    fV0(0)*fV1(1)*fV2(2) - fV0(1)*fV1(2)*fV2(0) - fV0(2)*fV1(0)*fV2(1);
  coef[1] = fV0(0)*fV1(1) + fV0(0)*fV2(2) + fV1(1)*fV2(2) - fV0(2)*fV2(0) -
    fV1(2)*fV2(1) - fV0(1)*fV1(0);
  coef[2] = - (fV0(0) + fV1(1) + fV2(2));
  coef[3] = un;
  n0 = 0;
  n1 = 3;
  complex = TMat33::MyRootsCubic(coef,L0,L1,L2);
  D       = TMath::Abs(L1);
  if (complex) {
    eps     = epsc*D;
    if (TMath::Abs(L2)<eps) {
      complex = kFALSE;
      L2      = L1;
    }
  }
  D      += TMath::Abs(L0);
  D      += TMath::Abs(L2);
  eps     = fgLimPrec*D;
  if (TMath::Abs(L0)<eps) { L0 = zero; n0++; }
  if (TMath::Abs(L1)<eps) L1 = zero;
  if (TMath::Abs(L2)<eps) L2 = zero;
  if (complex) n = -(10*n0 + n1);
  else {
    // ordering eigenvalues
    if (TMath::Abs(L1)<eps) n0++;
    if (TMath::Abs(L2)<eps) n0++;
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
  return n;
}
Short_t TMat33::EigenVectors(Double_t &L0,Double_t &L1,Double_t &L2,
  TVector3 &v0,TVector3 &v1,TVector3 &v2) const {
  // Finds the eigenvalues and the eigenvectors even in case matrix is not
  //symmetric, but in the case of 2 complex conjugate eigenvalues, only returns one
  //eigenvector.
  //The eigenvalues are returned in increasing order.
  // Returns n = 10*n0 + n1, where
  //  - n0 : number of eigenvalues equal to 0
  //  - n1 : number of DIFFERENT eigenvalues (n1<3 ==> degenerate eigenvalues)
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Short_t n;
  TVector3 u0,u1,u2;
  TVector3 w0,w1,w2;
  Double_t c0,c1,c2;
  n = EigenValues(L0,L1,L2);
  if (n>=0) {
// Finding v0
    TMat33 M(*this);
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
    v0 = v0.Unit();
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
    v1 = v1.Unit();
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
    v2 = v2.Unit();
  }
  else {
// Finding v0
    TMat33 M(*this);
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
    v0 = v0.Unit();
    v1.SetXYZ(zero,zero,zero);
    v2.SetXYZ(zero,zero,zero);
  }
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
    u0 = v0.Cross(w0);
    u1 = v0.Cross(w1);
    u2 = v0.Cross(w2);
    c0 = u0.Mag2();
    c1 = u1.Mag2();
    c2 = u2.Mag2();
    if (c0<c1) {
      if (c0<c2) {
        //avoid u0
        v1 = u1;
        v2 = v0.Cross(v1);
      }
      else {
        //avoid u2
        v1 = u1;
        v2 = v0.Cross(v1);
      }
    }
    else {
      if (c1<c2) {
        //avoid u1
        v1 = u0;
        v2 = v0.Cross(v1);
      }
      else {
        //avoid u2
        v1 = u1;
        v2 = v0.Cross(v1);
      }
    }
    v1 = v1.Unit();
    v2 = v2.Unit();
  }
  if (n==1) {
    v0.SetXYZ(un,zero,zero);
    v1.SetXYZ(zero,un,zero);
    v2.SetXYZ(zero,zero,un);
  }
  return n;
}
Short_t TMat33::EigenProjectors(Double_t &L0,Double_t &L1,Double_t &L2,
  TVector3 &v0,TVector3 &v1,TVector3 &v2,TMat33 &P0,TMat33 &P1,TMat33 &P2) const {
  // Finds the eigenvalues AND the eigenvectors AND the projectors on the eigenvectors
  //ONLY in the case matrix is symmetric. Please call VerifySymmetric() before
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
  P0.SetBit(fgSymTested);
  P0.SetBit(fgSym);
  P1.SetBit(fgSymTested);
  P1.SetBit(fgSym);
  P2.SetBit(fgSymTested);
  P2.SetBit(fgSym);
  if (IsSymmetric()) {
    n = EigenVectors(L0,L1,L2,v0,v1,v2);
    //
    P0(0,0) = v0(0)*v0(0);
    P0(0,1) = v0(0)*v0(1);
    P0(0,2) = v0(0)*v0(2);
    P0(1,0) = v0(1)*v0(0);
    P0(1,1) = v0(1)*v0(1);
    P0(1,2) = v0(1)*v0(2);
    P0(2,0) = v0(2)*v0(0);
    P0(2,1) = v0(2)*v0(1);
    P0(2,2) = v0(2)*v0(2);
    P0.Clean();
    //
    P1(0,0) = v1(0)*v1(0);
    P1(0,1) = v1(0)*v1(1);
    P1(0,2) = v1(0)*v1(2);
    P1(1,0) = v1(1)*v1(0);
    P1(1,1) = v1(1)*v1(1);
    P1(1,2) = v1(1)*v1(2);
    P1(2,0) = v1(2)*v1(0);
    P1(2,1) = v1(2)*v1(1);
    P1(2,2) = v1(2)*v1(2);
    P1.Clean();
    //
    P2(0,0) = v2(0)*v2(0);
    P2(0,1) = v2(0)*v2(1);
    P2(0,2) = v2(0)*v2(2);
    P2(1,0) = v2(1)*v2(0);
    P2(1,1) = v2(1)*v2(1);
    P2(1,2) = v2(1)*v2(2);
    P2(2,0) = v2(2)*v2(0);
    P2(2,1) = v2(2)*v2(1);
    P2(2,2) = v2(2)*v2(2);
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
    Error("EigenProjectors","This method cannot be called for non symmetric matrices");
  }
  return n;
}
TMat33 TMat33::Exp() const {
// Calculates the matrix which is the exponential of "this", on the condition
//that "this" is symmetric
  const Double_t zero = 0.0;
  TMat33 M(zero);
  if (IsSymmetric()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVector3  v0,v1,v2;
    TMat33 P0,P1,P2;
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
TVector3 TMat33::GetColumn(Int_t i) const {
  // Returns column i of matrix
  TVector3 v;
  i  = TMath::Abs(i);
  i %= 3;
  v(0) = fV0(i);
  v(1) = fV1(i);
  v(2) = fV2(i);
  return v;
}
void TMat33::GetColumn(Int_t i,Double_t &a,Double_t &b,Double_t &c) const {
  // Returns column i of matrix into (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  a = fV0(i);
  b = fV1(i);
  c = fV2(i);
}
TVector3 TMat33::GetLine(Int_t i) const {
  // Use rather operator () or [] !
  return (*this)(i);
}
void TMat33::GetLine(Int_t i,Double_t &a,Double_t &b,Double_t &c) const {
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
Bool_t TMat33::Invert(Double_t &det) {
// Invert the matrix and calculates its determinant "det".
// Inversion does not change the symmetric or unitarian character of the matrix.
// In case of failure, the original matrix is untouched. The boolean value returned
//only indicates if true that the calculation could be done without arithmetic
//overflow, division by 0, and so on. If you want an indication of the error
//when calculating the inverse, please use the next version of this method.
//
  const Double_t  un = 1.0;
  const Double_t eps = 1.0e-306;
  Bool_t ok = kFALSE;
  Double_t big,lim,adet;
  TMat33 M;
  det  = Determinant();
  adet = TMath::Abs(det);
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
Bool_t TMat33::Invert(Double_t &Error,Double_t &det) {
// Invert the matrix, calculates its determinant "det" and calculates the biggest
//error "Error" obtained when multiplying the inverse matrix with the original one.
// Inversion does not change the symmetric or unitarian character of the matrix.
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
  TMat33 W(*this);
  TMat33 M;
  Error = un/eps;
  det  = Determinant();
  adet = TMath::Abs(det);
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
TMat33 TMat33::Invert(Double_t &det,Bool_t &ok) const {
// Calculate the determinant of the matrix and returns the inverse of the matrix.
//Original matrix is preserved. ok is returned true if inversion succeeded.
  TMat33 M(*this);
  ok = M.Invert(det);
  return M;
}
TMat33 TMat33::Invert(Double_t &Error,Double_t &det,Bool_t &ok) const {
// Calculate the determinant of the matrix and returns the inverse of the matrix.
//Original matrix is preserved. ok is returned true if inversion succeeded.
//Error is an indication of the error when multiplying original matrix with
//inverse matrix, and not obtaining exactly matrix 1.
  TMat33 M(*this);
  ok = M.Invert(Error,det);
  return M;
}
Bool_t TMat33::IsSymmetric() const {
// Test whether "this" is a symmetric matrix, i.e. whether Mt == M.
  return TestBit(fgSymTested) && TestBit(fgSym);
}
Bool_t TMat33::IsSymmetric(Bool_t ForceTest) {
// Test whether this is a symmetric matrix, i.e. whether Mt == M.
// If ForceTest is false, result of a possible previous test is accepted
// If ForceTest is true, test is done in any case
  if ((ForceTest) || (!TestBit(fgSymTested))) {
    Double_t eps;
    eps = VerifySymmetric();
  }
  return TestBit(fgSym);
}
Bool_t TMat33::IsUnitarian() const {
// Test whether this is a rotation matrix, i.e. whether Mt == M-1.
  return TestBit(fgUniTested) && TestBit(fgUni);
}
Bool_t TMat33::IsUnitarian(Bool_t ForceTest) {
// Test whether this is a rotation matrix, i.e. whether Mt == M-1.
// If ForceTest is false, [default] result of a possible previous test is accepted
// If ForceTest is true, test is done in any case
  if ((ForceTest) || (!TestBit(fgUniTested))) {
    Double_t eps;
    eps = VerifyUnitarity();
  }
  return TestBit(fgUni);
}
Double_t TMat33::MaxCoeff() const  {
  // Finds the biggest among the absolute values of all matrix elements
  Short_t k;
  Double_t a;
  Double_t biggest = 0.0;
  for (k=0;k<3;k++) {
    a = TMath::Abs(fV0(k));
    if (a>biggest) biggest = a;
  }
  for (k=0;k<3;k++) {
    a = TMath::Abs(fV1(k));
    if (a>biggest) biggest = a;
  }
  for (k=0;k<3;k++) {
    a = TMath::Abs(fV2(k));
    if (a>biggest) biggest = a;
  }
  return biggest;
}
void TMat33::Print() const {
  Short_t k;
  cout << endl;
  for (k=0;k<3;k++) PrintWP(fV0(k));
  cout << endl;
  for (k=0;k<3;k++) PrintWP(fV1(k));
  cout << endl;
  for (k=0;k<3;k++) PrintWP(fV2(k));
  cout << endl;
  cout << endl;
}
void TMat33::PrintWP(Double_t a) {
  cout.width(fgWidth);
  cout.precision(fgPrec);
  cout << a;
}
void TMat33::RatherPositiv(TVector3 &v) {
//If the direction of a vector is free, chooses it rather positiv !
  Double_t d = v(0) + v(1) + v(2);
  if (d<0.0) v = -v;
}
void TMat33::Set(Double_t d) {
// Makes matrix diagonal with all diagonal elements are equal to d
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  SetLine(0,d,zero,zero);
  SetLine(1,zero,d,zero);
  SetLine(2,zero,zero,d);
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested,kTRUE);
  if (TMath::Abs(d-un) < fgLimPrec) SetBit(fgUni);
  else                              SetBit(fgUni,kFALSE);
}
void TMat33::Set(Double_t d00,Double_t d11,Double_t d22) {
// Makes matrix diagonal with diagonal elements equal to d00, d11, d22
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t eps;
  SetLine(0,d00,zero,zero);
  SetLine(1,zero,d11,zero);
  SetLine(2,zero,zero,d22);
  SetBit(fgSymTested);
  SetBit(fgSym);
  eps  = TMath::Abs(d00-un);
  eps += TMath::Abs(d11-un);
  eps += TMath::Abs(d22-un);
  SetBit(fgUniTested,kTRUE);
  if (eps < fgLimPrec) SetBit(fgUni);
  else                 SetBit(fgUni,kFALSE);
}
void TMat33::Set(Double_t phi,Double_t theta,Double_t psi,Bool_t deg) {
// Makes "this" to be the rotation matrix given by the 3 Euler angles (phi,theta,psi).
// If deg is true, the angles are given in degree, else in radians
//
//   The Euler representation uses 3 angles: (phi,theta,psi). We have
// (1) - rotation about the Oz axis with the angle phi
// (2) - a rotation of angle theta around the new Ox axis
// (3) - a rotation of angle psi around the new Oz axis
//   All rotations in trigonometric sense (counter-clockwise)
//   If we define the following abbreviations:
//      f  == phi,     t == theta,    p == psi,
//      sf == Sin(f)  st == Sin(t)   sp == Sin(p)
//      cf == Cos(f)  ct == Cos(t)   cp == Cos(p)
//   then the Euler matrix giving the new axis when applied to the old axis is:
//
//      | cp*cf - sp*ct*sf , -sp*cf - cp*ct*sf ,  st*sf |
//  M = | cp*sf + sp*ct*cf , -sp*sf + cp*ct*cf , -st*cf |
//      | sp*st            ,  cp*st            ,  ct    |
//
  const Double_t zero  = 0.0;
  const Double_t pideg = 180.0;
  const Double_t torad = TMath::Pi()/pideg;
  Double_t twopi;
  Double_t sphi,cphi,stheta,ctheta,spsi,cpsi;
  SetBit(fgSymTested);
  SetBit(fgSym,kFALSE);
  SetBit(fgUniTested);
  SetBit(fgUni);
  if (deg) twopi = 2*pideg;
  else     twopi = 2*TMath::Pi();
  if (phi < zero) {
    Error("Set","WARNING: phi must be >= 0. 2 pi repeatedly added");
    while (phi<zero)   phi  += twopi;
  }
  if (phi >= twopi) {
    Error("Set","WARNING: phi must be < 2 pi. 2 pi repeatedly subtracted");
    while (phi>=twopi) phi -= twopi;
  }
  if (theta < zero) {
    Error("Set","WARNING: theta must be >= 0. 2 pi repeatedly added");
    while (theta<zero)   theta  += twopi;
  }
  if (theta >= twopi) {
    Error("Set","WARNING: theta must be < 2 pi. 2 pi repeatedly subtracted");
    while (theta>=twopi) theta -= twopi;
  }
  if (psi < zero) {
    Error("Set","WARNING: psi must be >= 0. 2 pi repeatedly added");
    while (psi<zero)   psi  += twopi;
  }
  if (psi >= twopi) {
    Error("Set","WARNING: psi must be < 2 pi. 2 pi repeatedly subtracted");
    while (psi>=twopi) psi -= twopi;
  }
  if (deg) {
    phi    *= torad;
    theta  *= torad;
    psi    *= torad;
  }
  sphi    = TMath::Sin(phi);
  cphi    = TMath::Cos(phi);
  stheta  = TMath::Sin(theta);
  ctheta  = TMath::Cos(theta);
  spsi    = TMath::Sin(psi);
  cpsi    = TMath::Cos(psi);
  fV0(0)  =  cpsi*cphi - spsi*ctheta*sphi;
  fV1(0)  =  cpsi*sphi + spsi*ctheta*cphi;
  fV2(0)  =  stheta*spsi;
  fV0(1)  = -spsi*cphi - cpsi*ctheta*sphi;
  fV1(1)  = -spsi*sphi + cpsi*ctheta*cphi;
  fV2(1)  =  stheta*cpsi;
  fV0(2)  =  sphi*stheta;
  fV1(2)  = -cphi*stheta;
  fV2(2)  =  ctheta;
}
void TMat33::Set(Double_t d00,Double_t d11,Double_t d22,
  Double_t d01,Double_t d02,Double_t d12) {
// Makes "this" a symmetric matrix with the given arguments
  fV0(0) = d00;
  fV1(1) = d11;
  fV2(2) = d22;
  fV0(1) = d01;
  fV0(2) = d02;
  fV1(2) = d12;
  fV1(0) = d01;
  fV2(0) = d02;
  fV2(1) = d12;
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
void TMat33::Set(const TVector3 &v0,const TVector3 &v1) {
  // Makes "this" a symmetric matrix, where the vector v0 contains the 3
  //diagonal elements of the symmetric matrix, and the vector v1 contains
  //the 3 non diagonal elements of the matrix
  fV0(0) = v0(0);
  fV1(1) = v0(1);
  fV2(2) = v0(2);
  fV0(1) = v1(0);
  fV0(2) = v1(1);
  fV1(2) = v1(2);
  fV1(0) = fV0(1);
  fV2(0) = fV0(2);
  fV2(1) = fV1(2);
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
void TMat33::Set(const TVector3 &v0,const TVector3 &v1,const TVector3 &v2,Bool_t ascolumn) {
  // Makes "this" to be a matrix with 3 line vectors if ascolumn is false [default],
  //or with 3 column vectors if ascolumn is true.
  //
  // IMPORTANT NOTICE:
  //
  // The symmetric or unitarian state of the matrix constructed here is NOT
  //tested by this method.
  //If you happen to have entered a symmetric matrix, please either
  //  - immediatly call method VerifySymmetric() in order that the symmetric
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is symmetric]
  //      call   -- SetBit(fgSymTested);
  //             -- SetBit(fgSym);
  //If you happen to have entered a unitarian matrix, please either
  //  - immediatly call method VerifyUnitarity() in order that the unitarian
  //      state be recorded into the matrix
  //  - or [only if you are absolutly sure that the matrix is unitarian]
  //      call   -- SetBit(fgUniTested);
  //             -- SetBit(fgUni);
  fV0 = v0;
  fV1 = v1;
  fV2 = v2;
  if (ascolumn) T();
  SetStateFalse();
}
void TMat33::Set(const TMat33 &m,Bool_t TransposeIt) {
  // Makes "this" identical to m if TransposeIt is false [default]. Otherwise
  //construct the transposed of m.
  if (m.TestBit(fgUniTested)) SetBit(fgUniTested);
  else                        SetBit(fgUniTested,kFALSE);
  if (m.TestBit(fgUni))       SetBit(fgUni);
  else                        SetBit(fgUni,kFALSE);
  if (m.TestBit(fgSymTested)) SetBit(fgSymTested);
  else                        SetBit(fgSymTested,kFALSE);
  if (m.TestBit(fgSym))       SetBit(fgSym);
  else                        SetBit(fgSym,kFALSE);
  fV0 = m.fV0;
  fV1 = m.fV1;
  fV2 = m.fV2;
  if (TransposeIt) T();
}
void TMat33::SetAll(Double_t a) {
// Set all elements of matrix equal to a
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,a);
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
void TMat33::SetColumn(Int_t i,const TVector3 &v) {
  //Defines column i as being the vector v
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = v(0);
  fV1(i) = v(1);
  fV2(i) = v(2);
  SetStateFalse();
}
void TMat33::SetColumn(Int_t i,Double_t a) {
  //Defines column i as having all elements equal to a
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = a;
  fV1(i) = a;
  fV2(i) = a;
  SetStateFalse();
}
void TMat33::SetColumn(Int_t i,Double_t a,Double_t b,Double_t c) {
  //Defines column i as being the vector v = (a,b,c)
  i  = TMath::Abs(i);
  i %= 3;
  fV0(i) = a;
  fV1(i) = b;
  fV2(i) = c;
  SetStateFalse();
}
void TMat33::SetLine(Int_t i,const TVector3 &v) {
  //Defines line i as being the vector v
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
void TMat33::SetLine(Int_t i,Double_t a) {
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
void TMat33::SetLine(Int_t i,Double_t a,Double_t b,Double_t c) {
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
void TMat33::SetStateAs(const TMat33 &m) {
  // Set states of "this" as those of m
  if (m.TestBit(fgSymTested)) SetBit(fgSymTested);
  else                        SetBit(fgSymTested,kFALSE);
  if (m.TestBit(fgSym))        SetBit(fgSym);
  else                        SetBit(fgSym,kFALSE);
  if (m.TestBit(fgUniTested)) SetBit(fgUniTested);
  else                        SetBit(fgUniTested,kFALSE);
  if (m.TestBit(fgUni))       SetBit(fgUni);
  else                        SetBit(fgUni,kFALSE);
}
void TMat33::SetStateFalse() {
  // All states [unitarian and symmetric] set as undefined
  SetBit(fgSymTested,kFALSE);
  SetBit(fgSym,kFALSE);
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
}
TMat33 TMat33::Sin() const {
// Calculates the matrix which is the sinus of "this", on the condition that
//"this" is symmetric
  const Double_t zero = 0.0;
  TMat33 M(zero);
  if (IsSymmetric()) {
    Short_t n;
    Double_t  L0,L1,L2;
    TVector3  v0,v1,v2;
    TMat33 P0,P1,P2;
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
Double_t TMat33::SolveLin3(const TVector3 &b,TVector3 &x) const {
// Solve equation A*x = b, where A is equal to *this. this is not
//modified. Error in numerical calculation is returned
  Double_t error = 1.0e+307;
  Bool_t ok;
  Double_t det;
  TVector3 v;
  TMat33 A(*this);
  ok = A.Invert(det);
  if (ok) {
    x = A*b;
    v = (*this)*x - b;
    error = v.Mag();
  }
  return error;
}
Double_t TMat33::SolveLin3(const TMat33 &A,const TVector3 &b,TVector3 &x) {
// Solve equation A*x = b, A is not modified. Error in numerical calculation is returned
  Double_t error = 1.0e+307;
  Bool_t ok;
  Double_t det;
  TVector3 v;
  TMat33 B(A);
  ok = B.Invert(det);
  if (ok) {
    x = B*b;
    v = A*x - b;
    error = v.Mag();
  }
  return error;
}
Double_t TMat33::Symmetrize() {
  //Make the matrix symmetric
  const Double_t deux = 2.0;
  Double_t d;
  fV1(0) = (fV1(0)+fV0(1))/deux;
  fV0(1) = fV1(0);
  fV2(0) = (fV2(0)+fV0(2))/deux;
  fV0(2) = fV2(0);
  fV2(1) = (fV2(1)+fV1(2))/deux;
  fV1(2) = fV2(1);
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
  d = deux*(TMath::Abs(fV0[1]) + TMath::Abs(fV0[2]) + TMath::Abs(fV1[2]));
  return d;
}
void TMat33::T() {
  // Transposes the matrix. Transpositian does not change the symmetric or
  //unitarian character of the matrix
  Double_t y;
  y = fV1(0); fV1(0) = fV0(1); fV0(1) = y;
  y = fV2(0); fV2(0) = fV0(2); fV0(2) = y;
  y = fV1(2); fV1(2) = fV2(1); fV2(1) = y;
}
void TMat33::TransposedOf(const TMat33 &m) {
  //"this" becomes the transposed of m
  fV0(0) = m.fV0(0);  fV0(1) = m.fV1(0);  fV0(2) = m.fV2(0);
  fV1(0) = m.fV0(1);  fV1(1) = m.fV1(1);  fV1(2) = m.fV2(1);
  fV2(0) = m.fV0(2);  fV2(1) = m.fV1(2);  fV2(2) = m.fV2(2);
  SetStateAs(m);
}
void TMat33::Unit() {
  //Defines this matrix as being the unit matrix
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  fV0(0) = un;    fV0(1) = zero;  fV0(2) = zero;
  fV1(0) = zero;  fV1(1) = un;    fV1(2) = zero;
  fV2(0) = zero;  fV2(1) = zero;  fV2(2) = un;
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni);
}
Double_t TMat33::VerifyInverse(const TMat33 &W) const {
//  W is assumed to be the inverse of "this". Calculates the product
//(*this)*W and checks how far the result is from the Unit matrix.
//This number is returned and is a measure of how bad the calculation
//of the inverse matrix W was.
//  This method can also be used as a check of the unitarian character
//of the matrix by giving for W the transposed of "this". But for this
//it is simpler to call VerifyUnitarity().
  Double_t d;
  TMat33 U;
  U = W*(*this);
  d = U.CloseUnit();
  return d;
}
Double_t TMat33::VerifySymmetric() {
//  Returns a number which is an indication of how far "this" is from
//being a symmetric matrix. If it is found that the matrix is symmetric
//up to a value of fgLimPrec, the matrix is symmetrized.
  Double_t d;
  d  = TMath::Abs(fV1(0) - fV0(1));
  d += TMath::Abs(fV2(0) - fV0(2));
  d += TMath::Abs(fV2(1) - fV1(2));
  SetBit(fgSymTested);
  if (d<fgLimPrec) {
    Symmetrize();
    SetBit(fgSym);
  }
  else SetBit(fgSym,kFALSE);
  return d;
}
Double_t TMat33::VerifyUnitarity() {
//  Returns a number which is an indication of how far "this" is from
//being a unitarian matrix. If it is found that the matrix is unitarian
//up to a value of fgLimPrec, the matrix is considered as unitarian.
  Double_t d;
  TMat33 U;
  TMat33 W(*this,kTRUE);
  U = W*(*this);
  d = U.CloseUnit();
  SetBit(fgUniTested);
  if (d<fgLimPrec) SetBit(fgUni);
  else             SetBit(fgUni,kFALSE);
  return d;
}
void TMat33::Zero() {
  //Defines this matrix as being the null matrix
  TVector3 v;
  fV0 = v;
  fV1 = v;
  fV2 = v;
  SetBit(fgSymTested);
  SetBit(fgSym);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
}
TMat33 &TMat33::operator+=(Double_t d) {
  // Add d to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) += d;
  for (k=0;k<3;k++) fV1(k) += d;
  for (k=0;k<3;k++) fV2(k) += d;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  return *this;
}
TMat33 &TMat33::operator+=(const TMat33 &m) {
  // Add matrix m to this
  fV0 += m.fV0;
  fV1 += m.fV1;
  fV2 += m.fV2;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  if (!(TestBit(fgSymTested) && m.TestBit(fgSymTested) && TestBit(fgSym) && m.TestBit(fgSym))) {
    SetBit(fgSymTested,kFALSE);
    SetBit(fgSym,kFALSE);
  }
  return *this;
}
TMat33 &TMat33::operator-=(Double_t d) {
  // Subtract d to all elements of matrix
  Short_t k;
  for (k=0;k<3;k++) fV0(k) -= d;
  for (k=0;k<3;k++) fV1(k) -= d;
  for (k=0;k<3;k++) fV2(k) -= d;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  return *this;
}
TMat33 &TMat33::operator-=(const TMat33 &m) {
  // Subtract matrix m from this
  fV0 -= m.fV0;
  fV1 -= m.fV1;
  fV2 -= m.fV2;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  if (!(TestBit(fgSymTested) && m.TestBit(fgSymTested) && TestBit(fgSym) && m.TestBit(fgSym))) {
    SetBit(fgSymTested,kFALSE);
    SetBit(fgSym,kFALSE);
  }
  return *this;
}
TMat33 &TMat33::operator*=(Double_t d) {
  // Multiply by d all elements of matrix
  fV0 *= d;
  fV1 *= d;
  fV2 *= d;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  return *this;
}
TMat33 &TMat33::operator*=(const TMat33 &m) {
  // Multiply this by m. Result is this*m, not m*this!
  TMat33 p1(*this);
  TMat33 p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(fgSymTested,kFALSE);
  SetBit(fgSym,kFALSE);
  if (!(TestBit(fgUniTested) && m.TestBit(fgUniTested) && TestBit(fgUni) && m.TestBit(fgUni))) {
    SetBit(fgUniTested,kFALSE);
    SetBit(fgUni,kFALSE);
  }
  return *this;
}
TMat33 &TMat33::operator&=(Double_t d) {
  // Multiply by d all elements of the transposed matrix
  T();
  fV0 *= d;
  fV1 *= d;
  fV2 *= d;
  SetBit(fgUniTested,kFALSE);
  SetBit(fgUni,kFALSE);
  return *this;
}
TMat33 &TMat33::operator&=(const TMat33 &m) {
  // Transpose this and then multiply it by m. Result is thisT*m, not m*thisT!
  TMat33 p1(*this,kTRUE);
  TMat33 p2(m,kTRUE);
  fV0(0) = p1.fV0*p2.fV0;
  fV0(1) = p1.fV0*p2.fV1;
  fV0(2) = p1.fV0*p2.fV2;
  fV1(0) = p1.fV1*p2.fV0;
  fV1(1) = p1.fV1*p2.fV1;
  fV1(2) = p1.fV1*p2.fV2;
  fV2(0) = p1.fV2*p2.fV0;
  fV2(1) = p1.fV2*p2.fV1;
  fV2(2) = p1.fV2*p2.fV2;
  SetBit(fgSymTested,kFALSE);
  SetBit(fgSym,kFALSE);
  if (!(TestBit(fgUniTested) && m.TestBit(fgUniTested) && TestBit(fgUni) && m.TestBit(fgUni))) {
    SetBit(fgUniTested,kFALSE);
    SetBit(fgUni,kFALSE);
  }
  return *this;
}
TMat33 TMat33::operator+(Double_t d) const {
  //Returns a matrix equal to "*this" with d added to all elements
  TMat33 m4(*this);
  m4 += d;
  return m4;
}
TMat33 TMat33::operator+(const TMat33 &m) const {
  //Returns a matrix equal to "*this" added with matrix m
  TMat33 m4(*this);
  m4 += m;
  return m4;
}
TMat33 TMat33::operator-(Double_t d) const {
  //Returns a matrix equal to "*this" with d subtracted from all elements
  TMat33 m4(*this);
  m4 -= d;
  return m4;
}
TMat33 TMat33::operator-(const TMat33 &m) const {
  //Returns a matrix equal to "*this" minus matrix m
   TMat33 m4(*this);
  m4 -= m;
  return m4;
}
TMat33 TMat33::operator*(Double_t d) const {
  //Returns a matrix equal to "*this" with all elements multiplied by d
  TMat33 m4(*this);
  m4 *= d;
  return m4;
}
TMat33 TMat33::operator*(const TMat33 &m) const {
  //Returns a matrix equal to "*this" multiplied by matrix m
  TMat33 m4(*this);
  m4 *= m;
  return m4;
}
TMat33 TMat33::operator&(Double_t d) const {
  //this is transposed and then has all its elements multiplied by d
  TMat33 m4(*this,kTRUE);
  m4 *= d;
  return m4;
}
TMat33 TMat33::operator&(const TMat33 &m) const {
  //this is transposed and then multiplied by matrix m
  TMat33 m4(*this,kTRUE);
  m4 *= m;
  return m4;
}
TMat33 &TMat33::operator=(Double_t d) {
  //all elements of this equal to d
  Short_t k;
  for (k=0;k<3;k++) SetLine(k,d);
  SetBit(fgUniTested);
  SetBit(fgUni,kFALSE);
  SetBit(fgSymTested);
  SetBit(fgSym);
  return *this;
}
TMat33 &TMat33::operator=(const TMat33 &m) {
  //this becomes identical to m
  if (this != &m) {
    TObject::operator=(m);
    fV0 = m.fV0;
    fV1 = m.fV1;
    fV2 = m.fV2;
    SetStateAs(m);
  }
  return *this;
}
Bool_t TMat33::operator==(const TMat33& v) const {
// Check whether "this" is equal to v
  Bool_t isequal;
  isequal = (fV0 == v.fV0) && (fV1 == v.fV1) && (fV2 == v.fV2);
  return isequal;
}
Bool_t TMat33::operator>(const TMat33 &v) const {
// Check if the magnitude of "this" is greater than the magnitude of v
// magnitude == absolute value of determinant !
  return (TMath::Abs(Determinant()) > TMath::Abs(v.Determinant()));
}
Bool_t TMat33::operator>=(const TMat33 &v) const {
// Check if the magnitude of "this" is greater or equal to the magnitude of v
// magnitude == absolute value of determinant !
  return (TMath::Abs(Determinant()) >= TMath::Abs(v.Determinant()));
}
Bool_t TMat33::operator<(const TMat33 &v) const {
// Check if the magnitude of "this" is smaller than the magnitude of v
// magnitude == absolute value of determinant !
  return (TMath::Abs(Determinant()) < TMath::Abs(v.Determinant()));
}
Bool_t TMat33::operator<=(const TMat33 &v) const {
// Check if the magnitude of "this" is smaller or equal than the magnitude of v
// magnitude == absolute value of determinant !
  return (TMath::Abs(Determinant()) <= TMath::Abs(v.Determinant()));
}
Double_t TMat33::operator()(Int_t i,Int_t j) const {
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
  Double_t d = 0.0;
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
Double_t &TMat33::operator()(Int_t i,Int_t j,Bool_t change) {
  // Access to element (i,j) of matrix
  //
  // Important notice !!!
  //
  // If M is a TMat33, d a Double_t and i,j indeces [0-2], if you do
  //
  //  (1) -  d = M(i,j);    // There is no risk that the state of the
  //                        matrix (symmetric or unitarian) be changed,
  //        so this call is ok, the default value of change being false.
  //
  //  (2) -  M(i,j) = d;      // The state of the matrix will be changed
  //                        by the operation, so never do (2) ! Rather do:
  //  (3) -  M(i,j,kTRUE) = d;
  //
  Double_t *d = 0;
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
TVector3 TMat33::operator()(Int_t i) const {
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
  TVector3 v;
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
TVector3 &TMat33::operator()(Int_t i,Bool_t change)  {
  // Returns line i of matrix
  //
  // Important notice !!!
  //
  // If M is a TMat33, v a TVector3 and i an index [0-2], if you do
  //
  //  (1) -  v = M(i);      // There is no risk that the state of the
  //                        matrix (symmetric or unitarian) be changed,
  //        so this call is ok, the default value of change being false.
  //
  //  (2) -  M(i) = v;      // The state of the matrix will be changed
  //                        by the operation, so never do (2) ! Rather do:
  //  (3) -  M(i,kTRUE) = v;
  //
  TVector3 *v = 0;
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
TMat33 operator+(Double_t d, const TMat33 &m) {
  //Addition of d to all elements of matrix
  TMat33 p(m);
  p += d;
  return p;
}
TMat33 operator-(Double_t d, const TMat33 &m) {
  //All elements of matrix equal to d - element
  TMat33 p(m);
  p.ChangeSign();
  p += d;
  return p;
}
TMat33 operator*(Double_t d, const TMat33 &m) {
  //Multiplication of a matrix by a real number
  TMat33 p(m);
  p *= d;
  return p;
}
TVector3 operator*(const TMat33 &m, const TVector3 &v) {
  //Multiplication of a matrix by a [column] vector
  TVector3 w;
  w(0) = m.fV0*v;
  w(1) = m.fV1*v;
  w(2) = m.fV2*v;
  return w;
}
TVec3C operator*(const TMat33 &m, const TVec3C &v) {
  //Multiplication of a matrix by a [column] vector
  TVec3C w;
  w(0) = m.fV0*v;
  w(1) = m.fV1*v;
  w(2) = m.fV2*v;
  return w;
}
TVector3 operator*(const TVector3 &v,const TMat33 &m) {
  //Multiplication of a [line] vector by a matrix
  TVector3 w;
  TMat33 mt(m,kTRUE);
  w(0) = v*mt.fV0;
  w(1) = v*mt.fV1;
  w(2) = v*mt.fV2;
  return w;
}
TVec3C operator*(const TVec3C &v,const TMat33 &m) {
  //Multiplication of a [line] vector by a matrix
  TVec3C w;
  TMat33 mt(m,kTRUE);
  w(0) = v*mt.fV0;
  w(1) = v*mt.fV1;
  w(2) = v*mt.fV2;
  return w;
}
TMat33 operator&(Double_t d,const TMat33 &m) {
  //Result is the transposed of m, with all elements multiplied by d
  TMat33 p(m,kTRUE);
  p *= d;
  return p;
}
Bool_t TMat33::MyRootsCubic(const Double_t coef[4],Double_t &a, Double_t &b, Double_t &c) {
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
TVector3 Clean(TVector3 &a) {
// Clean vector from almost 0 values
  const Double_t zero = 0.0;
  TVector3 v(a);
  Double_t D,eps;
  D   = a.Mag();
  eps = D*TMat33::fgLimPrec;
  if (TMath::Abs(v.X())<eps) v.SetX(zero);
  if (TMath::Abs(v.Y())<eps) v.SetY(zero);
  if (TMath::Abs(v.Z())<eps) v.SetZ(zero);
  return v;
}
TVector3 operator^(const TVector3 &a,const TVector3 &b) {
  // Cross product using ^
  TVector3 v;
  v.SetX(a.Y()*b.Z() - a.Z()*b.Y());
  v.SetY(a.Z()*b.X() - a.X()*b.Z());
  v.SetZ(a.X()*b.Y() - a.Y()*b.X());
  return v;
}



















