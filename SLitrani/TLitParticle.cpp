// @(#)root/html:$Id: TLitParticle.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TMath.h"
#include "TCleanOut.h"
#include "TLitPhys.h"
#include "TLitParticle.h"

ClassImp(TLitParticle)
//______________________________________________________________________________
//
//  IMPORTANT NOTICE
//
//  See definition and the format of the default particle list in
//
//  $ROOTSYS/etc/pdg_table.txt
//
//  In particular, pdg code. For mu- pdg code = 13. For mu+ pdg code = -13
//
//  TLitParticle  Defines particles with their momentum distribution. These
// particles will generate photons when crossing fluorescent materials.
// The momentum distribution can be :
//
// - singlevalued : particles have always the same momentum
// - straigth     : flat between fPmin and fPmax
// - gaussian     : gaussian distributed, with mean fPmean ans sigma fSig.
// - exponential  : exp(-b*p), with b = fSlope.
//
//  Notice that by default particles also emit Cerenkov light. If you do not
// want Cerenkov light, look at the method SetEmitCerenkov().
//
TLitParticle::TLitParticle(Int_t pdg,PDistribution dist,Double_t Pmean,Double_t Pmin,
  Double_t Pmax,Double_t sig,Double_t b) {
//Constructor, with the following arguments :
//
//  pdg     : pdg code of the particle, which can be found in $ROOTSYS/etc/pdg_table.txt
//            knowing pdg code, all static characteristics of particle, for
//            instance mass, can be obtained.
//  dist    : distribution in momentum : {singlevalued,straigth,gaussian,exponential}
//             default : singlevalued  
//  Pmean   : mean momentum in Gev/c, valid for {singlevalued,gaussian}
//             default : 100.0
//  Pmin    : minimum momentum in Gev/c, valid for {straigth,gaussian,exponential}
//             default : 1.0
//  Pmax    : maximum momentum in Gev/c, valid for {straigth,gaussian,exponential}
//             default : 10.0
//  sig     : standard deviation of distribution in Gev/c, valid for {gaussian}
//             default : 0.5
//  b       : slope of exponential distribution in [Gev/c]-1
//             default : 1.0
//
  const char *met = "TLitParticle";
  SetPdgCode(pdg);
  fPdist = dist;
  fPmean = Pmean;
  if (fPdist==gaussian) {
    if (Pmin>fPmean) {
      gCleanOut->MMR(error,met,"Pmin>Pmean","Pmin",Pmin,ClassName());
      Pmin = 0.9*fPmean;
      gCleanOut->MMR(error,met,"Pmin set to 0.9*Pmean","Pmin",Pmin,ClassName());
    }
    if (Pmax<fPmean) {
      gCleanOut->MMR(error,met,"Pmax<Pmean","Pmax",Pmax,ClassName());
      Pmax = 1.1*fPmean;
      gCleanOut->MMR(error,met,"Pmax set to 1.1*Pmean","Pmax",Pmax,ClassName());
    }
  }
  fPmin  = Pmin;
  fPmax  = Pmax;
  fSig   = sig;
  fSlope = b;
  Exp12();
  fEmitCerenkov = kTRUE;
  fCerNphot     = 0;
}
TLitParticle::~TLitParticle() {
}
void TLitParticle::Cerenkov(Double_t n,Double_t thickness) {
//  Providing index of refraction n and thickness of material in [cm] in
// the call to Cerenkov(), you will fix all parameters affecting
// the generation of Cerenkov light ( theta Cerenkov and number of Cerenkov
// photons emitted ). As soon as you have called Cerenkov once, the
// particle is able to emit Cerenkov light.
//
  const Double_t un     = 1.0;
  const Double_t nmtocm = 1.0e-7;
  Double_t WaveLmin,WaveLmax; //min and max wavelengths used in [cm]
  Double_t fac;
  fCerCos   = un/(fBeta*n);
  if (fCerCos<un) {
    fCerSin   = TMath::Sqrt(un - fCerCos*fCerCos);
    WaveLmin  = TLitPhys::Get()->MinWaveL()*nmtocm;
    WaveLmax  = TLitPhys::Get()->MaxWaveL()*nmtocm;
    fac       = thickness*(un/WaveLmin - un/WaveLmax);
    fCerNphot = Int_t(TMath::TwoPi()*TLitPhys::Get()->Alpha()*fac*fCerSin*fCerSin);
  }
  else fCerNphot = 0;
}
void TLitParticle::Exp12() {
  // Calculates fExp1 and fD
  Double_t Exp2;    //2nd constant in case {exponential}
  fExp1  = TMath::Exp(-fSlope*fPmin);
  Exp2   = TMath::Exp(-fSlope*fPmax);
  fD     = fExp1 - Exp2;
}
void TLitParticle::GenP() {
// Generates the momentum fP [Gev/c] of the particle and calculates
//  - energy fE     [Gev  ]
//  - beta   fBeta  [ -   ]
//  - speed  fSpeed [cm/ps]
  Double_t y;
  Double_t mass;
  Bool_t ok = kFALSE;
  switch (fPdist) {
  case singlevalued:
    fP = fPmean;
    break;
  case straigth:
    y = gRandom3->Rndm();
    fP = fPmin + (fPmax-fPmin)*y;
    break;
  case gaussian:
    while (!ok) {
      fP = gRandom3->Gaus(fPmean,fSig);
      ok = ((fP>=fPmin) && (fP<=fPmax));
    }
    break;
  case exponential:
    y = gRandom3->Rndm();
    fP = - TMath::Log(fExp1 - y*fD)/fSlope;
    break;
  }
  mass = Mass();
  fE = TMath::Sqrt(fP*fP + mass*mass);
  fBeta  = fP/fE;
  fSpeed = fBeta*TLitPhys::Get()->C();
}
void TLitParticle::Print() const {
  // Prints everything about particle
  const char *met = "Print";
  gCleanOut->M(info,"");
  gCleanOut->MMI(info,met,"Particle pdg code","pdg",fPdgCode,ClassName());
  gCleanOut->MMR(info,met,"Mass of particle in Gev","mass",Mass(),ClassName());
  switch (fPdist) {
    case singlevalued:
      gCleanOut->MMT(info,met,"Type of momentum distribution","type","singlevalued",ClassName());
       break;
    case straigth:
      gCleanOut->MMT(info,met,"Type of momentum distribution","type","straigth",ClassName());
      break;
    case gaussian:
      gCleanOut->MMT(info,met,"Type of momentum distribution","type","gaussian",ClassName());
      break;
    case exponential:
      gCleanOut->MMT(info,met,"Type of momentum distribution","type","exponential",ClassName());
      break;
  }
  gCleanOut->MMR(info,met,"mean momentum [Gev/c]","mean",fPmean,ClassName());
  gCleanOut->MMR(info,met,"min. momentum [Gev/c]","min",fPmin,ClassName());
  gCleanOut->MMR(info,met,"max. momentum [Gev/c]","max",fPmax,ClassName());
  gCleanOut->MMR(info,met,"sigma of gaussian [Gev/c]","sig",fSig,ClassName());
  gCleanOut->MMR(info,met,"slope of exponential [Gev/c]-1","slope",fSlope,ClassName());
}
void TLitParticle::SetLorentzE(Double_t dirx,Double_t diry,Double_t dirz,Double_t P) {
  // Given the direction of P and the absolute value of P, sets fPx,fPy,fPz and fE
  // (dirx,diry,dirz) MUST be a unit vector
  Double_t E,M;
  M = GetMass();
  E = TMath::Sqrt(P*P + M*M);
  SetMomentum(P*dirx,P*diry,P*dirz,E);
}
