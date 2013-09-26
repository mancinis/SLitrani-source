// @(#)root/html:$Id: TEuler.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TEuler.h"

ClassImp(TEuler)
//______________________________________________________________________________
//
//  TEuler  is an help for going from the Euler representation of a rotation
//            to the GEANT3 representation or vice-versa.
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
//   The GEANT3 representation give the 6 angles (thetax,phix,thetay,phiy,thetaz,
//phiz) of the (Ox',Oy',Oz') axis of the rotated system in the original (Ox,Oy,Oz)
//coordinate system. It means:
//
//        | Sin(thetax)*Cos(phix) |
//  Ox' = | Sin(thetax)*Sin(phix) |
//        | Cos(thetax)           |
//
//        | Sin(thetay)*Cos(phiy) |
//  Oy' = | Sin(thetay)*Sin(phiy) |
//        | Cos(thetay)           |
//
//        | Sin(thetaz)*Cos(phiz) |
//  Oz' = | Sin(thetaz)*Sin(phiz) |
//        | Cos(thetaz)           |
//
//   The matrix M in this GEANT3 representation is:
//
//  M = | Ox' , Oy' , Oz' |
//
//  The Euler representation has the advantage of being compact, only the needed
//3 parameters. It has the disadvantage that the values of the angles is more
//difficult to guess than the ones of the GEANT3 representation.
//  The GEANT3 representation has the drawback of giving more parameters than
//needed. In TEuler, we simply not ask to the user to give the (thetaz,phiz)
//angles, since they can be calculated from the other ones [look at the 3rd constructor].
//Even so, we have a parameter too much and the user can provide (Ox',Oy') axis
//which are not orthogonal. If this happens, TEuler corrects the values of
//(thetay,phiy) in such a way to change them minimally, but such that Ox' and Oy'
//are orthogonal. A warning message is issued.
//
//  Use the 2nd constructor if you want to enter the Euler angles and get back
//the GEANT3 angles.
//  Use the 3rd constructor if you want to enter the GEANT3 angles and get back
//the Euler angles
//  If an object of class TEuler is created and put into a ROOT file, only
//the 3 Euler angles are written in the file. When you read the object back
//from the file, call method Set(), without arguments, in order to get back
//the other class variables values.
//  Use the Get... methods to access at the class variables and the Print...
//methods to print them.
//
TEuler::TEuler(Bool_t identity) {
// Default constructor [identity=false] or constructor of identity
//transformation [identity=true]
  if (identity) Identical();
}
TEuler::TEuler(Double_t phi,Double_t theta,Double_t psi,Bool_t deg) {
// Initialization from Euler angles given in degree [deg=kTRUE, default] or in
//radian [deg=kFALSE]. The GEANT3 angles and the rotation matrix fM are then
//calculated.
// The conventions are:
//   0 <= phi   <  360
//   0 <= theta <  360
//   0 <= psi   <  360
//
  Set(phi,theta,psi,deg);
}
TEuler::TEuler(Double_t thetax,Double_t phix,Double_t thetay,Double_t phiy,Bool_t deg) {
// Initialization from the GEANT 3 angles given in degree [deg==kTRUE, default] or in
//radian [deg==kFALSE]. It is not necessary to give the angles (thetaz,phiz) of the Oz'
//axis since it can be determined from the first 2.
// The conventions are:
//  0 <= thetax/y/z <=180
//  0 <= phix/y/z   < 360
//
  Set(thetax,phix,thetay,phiy,deg);
}
TEuler::~TEuler() {
//destructor
}
void TEuler::GetEulerAngles(Double_t &phi,Double_t &theta,Double_t &psi,Bool_t deg) const {
// Get the Euler angles in radian [deg==false] or in degree [deg==true, default]
  if (deg) {
    phi   = GetDPhi();
    theta = GetDTheta();
    psi   = GetDPsi();
  }
  else {
    phi   = fPhi;
    theta = fTheta;
    psi   = fPsi;
  }
}
void TEuler::GetG3Angles(Double_t &thetax,Double_t &phix,Double_t &thetay,Double_t &phiy,
  Double_t &thetaz,Double_t &phiz,Bool_t deg) const {
// Get the GEANT3 angles in radian [deg==false] or in degree [deg==true, default]
  if (deg) {
    thetax = GetDThetax();
    phix   = GetDPhix();
    thetay = GetDThetay();
    phiy   = GetDPhiy();
    thetaz = GetDThetaz();
    phiz   = GetDPhiz();
  }
  else {
    thetax = fThetax;
    phix   = fPhix;
    thetay = fThetay;
    phiy   = fPhiy;
    thetaz = fThetaz;
    phiz   = fPhiz;
  }
}
void TEuler::GetOx(TVector3 &Ox) const {
// Get the new Ox' axis in the old (Ox,Oy,Oz) coordinate system
  Ox = fM.GetColumn(0);
}
void TEuler::GetOy(TVector3 &Oy) const {
// Get the new Oy' axis in the old (Ox,Oy,Oz) coordinate system
  Oy = fM.GetColumn(1);
}
void TEuler::GetOz(TVector3 &Oz) const {
// Get the new Oz' axis in the old (Ox,Oy,Oz) coordinate system
  Oz = fM.GetColumn(2);
}
void TEuler::GetOxyz(TVector3 &Ox,TVector3 &Oy,TVector3 &Oz) const {
// Get the 3 new axis Ox', Oy', Oz'  in the old (Ox,Oy,Oz) coordinate system
  GetOx(Ox);
  GetOy(Oy);
  GetOz(Oz);
}
void TEuler::Identical() {
//Identical transformation
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  const Double_t pis2 = TMath::Pi()/deux;
  fPhi    = zero;
  fTheta  = zero;
  fPsi    = zero;
  fThetax = pis2;
  fPhix   = zero;
  fThetay = pis2;
  fPhiy   = pis2;
  fThetaz = zero;
  fPhiz   = zero;
  fM.Unit();
}
void TEuler::PrintEulerAngles(Bool_t deg) const {
// Print the Euler angles in radian [deg==false] or in degree [deg==true, default]
  cout << endl;
  cout << "The 3 Euler angles in ";
  if (deg) {
    Double_t phi,theta,psi;
    cout << "degree: " << endl;
    phi   = GetDPhi();
    theta = GetDTheta();
    psi   = GetDPsi();
    PrintWP(phi);
    PrintWP(theta);
    PrintWP(psi);
  }
  else {
    cout << "radian: " << endl;
    PrintWP(fPhi);
    PrintWP(fTheta);
    PrintWP(fPsi);
  }
  cout << endl;
}
void TEuler::PrintG3Angles(Bool_t deg) const {
// Print the GEANT3 angles in radian [deg==false] or in degree [deg==true, default]
  cout << endl;
  cout << "The 6 GEANT3 angles in ";
  if (deg) {
    Double_t thetax,phix,thetay,phiy,thetaz,phiz;
    cout << "degree: " << endl;
    thetax = GetDThetax();
    phix   = GetDPhix();
    thetay = GetDThetay();
    phiy   = GetDPhiy();
    thetaz = GetDThetaz();
    phiz   = GetDPhiz();
    PrintWP(thetax);
    PrintWP(phix);
    PrintWP(thetay);
    PrintWP(phiy);
    PrintWP(thetaz);
    PrintWP(phiz);
  }
  else {
    cout << "radian: " << endl;
    PrintWP(fThetax);
    PrintWP(fPhix);
    PrintWP(fThetay);
    PrintWP(fPhiy);
    PrintWP(fThetaz);
    PrintWP(fPhiz);
  }
  cout << endl;
}
void TEuler::PrintMatrix() const {
//Prints the rotation matrix
  cout << endl;
  cout << "Rotation matrix of TEuler" << endl;
  cout << endl;
  fM.Print();
}
void TEuler::PrintWP(Double_t x) const {
// Print x in width 16 and precision 10
  cout.width(16);
  cout.precision(10);
  cout << x;
}
void TEuler::Set() {
// Setting Geant3 angles and rotation matrix once Euler angles are defined.
//In case you just read this instance of TEuler from a ROOT file, please
//immediatly call this method Set() in order to define all the class variables
//not put on file (the ones with //!). Only the Euler angles are put inside a
//ROOT file.
  const Double_t zero  = 0.0;
  const Double_t small = 1.0e-20;
  Double_t ax,ay,az,stx,x,y;
  Double_t sphi,cphi,stheta,ctheta,spsi,cpsi;
  sphi    = TMath::Sin(fPhi);
  cphi    = TMath::Cos(fPhi);
  stheta  = TMath::Sin(fTheta);
  ctheta  = TMath::Cos(fTheta);
  spsi    = TMath::Sin(fPsi);
  cpsi    = TMath::Cos(fPsi);
//Finding fThetax and fPhix
  ax      =  cpsi*cphi - spsi*ctheta*sphi;
  ay      =  cpsi*sphi + spsi*ctheta*cphi;
  az      =  stheta*spsi;
  fM(0,0) = ax;
  fM(1,0) = ay;
  fM(2,0) = az;
  fThetax = TMath::ACos(az);
  stx     = TMath::Sin(fThetax);
  if (stx<small) {
    x     = 1.0;
    y     = zero;
  }
  else {
    x = ax/stx;
    y = ay/stx;
  }
  fPhix = TMath::ATan2(y,x);
  if (fPhix<zero) fPhix += 2*TMath::Pi();
//Finding fThetay and fPhiy
  ax      = -spsi*cphi - cpsi*ctheta*sphi;
  ay      = -spsi*sphi + cpsi*ctheta*cphi;
  az      =  stheta*cpsi;
  fM(0,1) = ax;
  fM(1,1) = ay;
  fM(2,1) = az;
  fThetay = TMath::ACos(az);
  stx     = TMath::Sin(fThetay);
  if (stx<small) {
    x     = 1.0;
    y     = zero;
  }
  else {
    x = ax/stx;
    y = ay/stx;
  }
  fPhiy = TMath::ATan2(y,x);
  if (fPhiy<zero) fPhiy += 2*TMath::Pi();
//Finding fThetaz and fPhiz
  ax      =  sphi*stheta;
  ay      = -cphi*stheta;
  az      =  ctheta;
  fM(0,2) = ax;
  fM(1,2) = ay;
  fM(2,2) = az;
  fThetaz = TMath::ACos(az);
  stx     = TMath::Sin(fThetaz);
  if (stx<small) {
    x     = 1.0;
    y     = zero;
  }
  else {
    x = ax/stx;
    y = ay/stx;
  }
  fPhiz = TMath::ATan2(y,x);
  if (fPhiz<zero) fPhiz += 2*TMath::Pi();
}
void TEuler::Set(Double_t phi,Double_t theta,Double_t psi,Bool_t deg) {
// Initialization from Euler angles given in degree [deg=kTRUE, default] or in
//radian [deg=kFALSE]. The GEANT3 angles and the rotation matrix fM are then
//calculated.
// The conventions are:
//   0 <= phi   <  360
//   0 <= theta <  360
//   0 <= psi   <  360
//
  const Double_t zero  = 0.0;
  const Double_t pideg = 180.0;
  const Double_t torad = TMath::Pi()/pideg;
  Double_t twopi;
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
    fPhi    = phi*torad;
    fTheta  = theta*torad;
    fPsi    = psi*torad;
  }
  Set();
}
void TEuler::Set(Double_t thetax,Double_t phix,Double_t thetay,Double_t phiy,Bool_t deg) {
// Initialization from the GEANT 3 angles given in degree [deg==kTRUE, default] or in
//radian [deg==kFALSE]. It is not necessary to give the angles (thetaz,phiz) of the Oz'
//axis since it can be determined from the first 2.
// The conventions are:
//  0 <= thetax/y/z <=180
//  0 <= phix/y/z   < 360
//
  const Double_t zero  = 0.0;
  const Double_t pideg = 180;
  const Double_t torad = TMath::Pi()/pideg;
  const Double_t small = 1.0e-8;
  const Double_t eps   = 1.0e-14;
  Short_t i,j;
  TVector3 vx,vy,vz;
  Double_t sthetax,cthetax,sphix,cphix;
  Double_t sthetay,cthetay,sphiy,cphiy;
  Double_t sthetaz,sphiz,cphiz;
  Double_t tt,det,aa;
  Double_t pi;
  Double_t twopi = 360.0;
  if (deg) {
    pi    = pideg;
    twopi = 2*pideg;
  }
  else {
    pi    = TMath::Pi();
    twopi = 2*pi;
  }
  //x
  if (thetax<zero) {
    thetax = TMath::Abs(thetax);
    phix  += pi;
    Error("Set","WARNING: thetax must be >=0. Abs taken and pi added to phix");
    if (phix>=twopi) while (phix>=twopi) phix -= twopi;
  }
  if (thetax>pi) {
    Error("Set","WARNING: thetax must be <= pi. pi repeatedly subtracted and phix +-pi");
    while (thetax>pi) {
      thetax -= pi;
      if (phix>pi) phix -= pi;
      else         phix += pi;
    }
  }
  if (phix < zero) {
    Error("Set","WARNING: phix must be >= 0. 2 pi repeatedly added");
    while (phix<zero)   phix  += twopi;
  }
  if (phix >= twopi) {
    Error("Set","WARNING: phix must be < 2 pi. 2 pi repeatedly subtracted");
    while (phix>=twopi) phix -= twopi;
  }
  //y
  if (thetay<zero) {
    thetay = TMath::Abs(thetay);
    phiy += pi;
    Error("Set","WARNING: thetay must be >=0. Abs taken and pi added to phiy");
    if (phiy>=twopi) while (phiy>=twopi) phiy -= twopi;
  }
  if (thetay>pi) {
    Error("Set","WARNING: thetay must be <= pi. pi repeatedly subtracted and phiy +-pi");
    while (thetay>pi) {
      thetay -= pi;
      if (phiy>pi) phiy -= pi;
      else         phiy += pi;
    }
  }
  if (phiy < zero) {
    Error("Set","WARNING: phiy must be >= 0. 2 pi repeatedly added");
    while (phiy<zero)   phiy  += twopi;
  }
  if (phiy >= twopi) {
    Error("Set","WARNING: phiy must be < 2 pi. 2 pi repeatedly subtracted");
    while (phiy>=twopi) phiy -= twopi;
  }
  if (deg) {
    fThetax = thetax*torad;
    fPhix   = phix*torad;
    fThetay = thetay*torad;
    fPhiy   = phiy*torad;
  }
  else {
    fThetax = thetax;
    fPhix   = phix;
    fThetay = thetay;
    fPhiy   = phiy;
  }
  //x
  sthetax = TMath::Sin(fThetax);
  cthetax = TMath::Cos(fThetax);
  sphix   = TMath::Sin(fPhix);
  cphix   = TMath::Cos(fPhix);
  vx(0)   = sthetax*cphix;
  vx(1)   = sthetax*sphix;
  vx(2)   = cthetax;
  //y
  sthetay = TMath::Sin(fThetay);
  cthetay = TMath::Cos(fThetay);
  sphiy   = TMath::Sin(fPhiy);
  cphiy   = TMath::Cos(fPhiy);
  vy(0)   = sthetay*cphiy;
  vy(1)   = sthetay*sphiy;
  vy(2)   = cthetay;
  //test of orthogonality and correction if not
  vz = vx.Cross(vy);
  vz = vz.Unit();
  tt      = TMath::Abs(vx*vy);
  if (tt>small) {
    Error("Set","WARNING: Oy not orthogonal to Ox. Oy corrected");
    vy = vz.Cross(vx);
    vy = vy.Unit();
    vz = vx.Cross(vy);
    vz = vz.Unit();
    fThetay = TMath::ACos(vy(2));
    sthetay = TMath::Sin(fThetay);
    if (sthetay>small) {
      cphiy = vy(0)/sthetay;
      sphiy = vy(1)/sthetay;
    }
    else {
      sphiy = zero;
      cphiy = 1.0;
    }
    fPhiy = TMath::ATan2(sphiy,cphiy);
    if (TMath::Abs(fPhiy)<eps) fPhiy = zero;
    if (fPhiy<zero) fPhiy += 2*TMath::Pi();
    tt = TMath::Abs(vx*vy);
    if (tt>small) Error("Set","Oy not orthogonal to Ox even after correction");
  }
  fThetaz = TMath::ACos(vz(2));
  sthetaz = TMath::Sin(fThetaz);
  if (sthetaz>small) {
    cphiz = vz(0)/sthetaz;
    sphiz = vz(1)/sthetaz;
  }
  else {
    sphiz = zero;
    cphiz = 1.0;
  }
  fPhiz = TMath::ATan2(sphiz,cphiz);
  if (TMath::Abs(fPhiz)<eps) fPhiz = zero;
  if (fPhiz<zero) fPhiz += 2*TMath::Pi();
  //x
  fM(0,0) = vx(0);
  fM(1,0) = vx(1);
  fM(2,0) = vx(2);
  //y
  fM(0,1) = vy(0);
  fM(1,1) = vy(1);
  fM(2,1) = vy(2);
  //z
  fM(0,2) = vz(0);
  fM(1,2) = vz(1);
  fM(2,2) = vz(2);
  for (i=0;i<3;i++)
    for (j=0;j<3;j++) if (TMath::Abs(fM(i,j))<eps) fM(i,j) = zero;
  //test determinant is +1
  det = fM.Determinant();
  if (TMath::Abs(det-1.0) > small) {
    Error("Set","Error: determinant of rotation matrix not equal to 1");
    cout << "  det = " << det << "  bad GEANT3 angles" << endl;
  }
// Getting the Euler angles
  Double_t stheta,ctheta,sphi,cphi,spsi,cpsi;
  ctheta = fM(2,2);
  fTheta = TMath::ACos(ctheta);
  stheta = TMath::Sin(fTheta);
  if (fTheta<small) {
// In that case, only fPsi+fPhi is defined. We choose arbitrarily fPsi==0
    fPsi = zero;
    spsi = zero;
    cpsi = 1.0;
    cphi = fM(0,0);
    sphi = fM(1,0);
    fPhi = TMath::ATan2(sphi,cphi);
    if (fPhi<zero) fPhi += 2*TMath::Pi();
  }
  else {
    sphi =  fM(0,2)/stheta;
    cphi = -fM(1,2)/stheta;
    spsi =  fM(2,0)/stheta;
    cpsi =  fM(2,1)/stheta;
    fPhi = TMath::ATan2(sphi,cphi);
    if (fPhi<zero) fPhi += 2*TMath::Pi();
    fPsi = TMath::ATan2(spsi,cpsi);
    if (fPsi<zero) fPsi += 2*TMath::Pi();
  }
//Checks
  aa  = cpsi*cphi - spsi*ctheta*sphi;
  tt  = TMath::Abs(fM(0,0) - aa);
  aa  = -spsi*cphi - cpsi*ctheta*sphi;
  tt += TMath::Abs(fM(0,1) - aa);
  aa  = cpsi*sphi + spsi*ctheta*cphi;
  tt += TMath::Abs(fM(1,0) - aa);
  aa  = -spsi*sphi + cpsi*ctheta*cphi;
  tt += TMath::Abs(fM(1,1) - aa);
  if (tt>small) Error("Set","ERROR: Bad Euler matrix");
}
