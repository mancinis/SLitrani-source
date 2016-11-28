// @(#)root/html:$Id: TLitPhoton.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TVirtualGeoTrack.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitDetector.h"
#include "TLitResults.h"
#include "TLitEqIndex.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLitWave.h"
#include "TLitPhoton.h"

Int_t TLitPhoton::fgNPointsMax = 10000;
Long64_t TLitPhoton::fgNphot   = 0;

ClassImp(TLitPhoton)
//______________________________________________________________________________
//
//
//        --------------------------------------------------------------
//        |                                                             |
//        |  This class is an internal class of SLitrani. It is not ne-  |
//        | cessary for a user of SLitrani to know anything about it.    |
//        | The documentation about it is of interest only for those    |
//        | interested in the inner working of SLitrani.                 |
//        |                                                             |
//         -------------------------------------------------------------
//
//  TLitPhoton contains all data and methods describing a photon inside the set-up.
//
//  There is only one static variable in this class that the user may be tempted
// to change: TLitPhoton::fgNPointsMax which is equal by default to 10000. It is
// the maximum number of interactions allowed for a photon. If the user has a setup
// where the photon can live very very long, may be the default of 10000 has to be
// increased. No problem, TLitPhoton::fgNPointsMax is a public static variable.
//
//______________________________________________________________________________
//
TLitPhoton::TLitPhoton(Int_t run,Int_t kphot,const TVector3 &x0,TGeoVolume *GeoVolume,
  const char *path,Double_t wvlgth,Double_t t0,TVector3 &K,TVector3 &E,Short_t ks) {
  //Constructor of the photon. Wavelength, time of emission, point of emission
  //and k vector have already been generated. Direction of E vector has also
  //been generated, except in the case of generation inside anisotropic
  //material.
  //  run        : run number
  //  kphot      : photon number
  //  x0         : starting point of photon in World Coordinate System
  //  GeoVolume  : TGeoVolume in which photon is generated. As there can be many
  //               instances of the TGeoVolume in the geometry, it is necessary
  //               to specify also "path"
  //  path       : path name of the TGeoVolume inside which photon is generated
  //               for instance "/TOP_1/REV_1/FIB_1"
  //  wvlgth     : wavelength of photon in nanometers
  //  t0         : start time of photon
  //  K          : k vector of photon in World Coordinate System
  //  E          : direction of E field in WCS, in case of isotropic material
  //               0 otherwise.
  //  ks         : choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //                in anisotropic material. In anisotropic material, there
  //                are 2 possible values for the index of refraction once
  //                the direction of the k vector is fixed.
  //
  const Double_t zero = 0.0;
  InitP();
  Init();
  fRun           = run;
  fKphot         = kphot;
  fgNphot++;
  gCleanOut->RunEv(fRun,fKphot);
  if (fKphot==1) gCleanOut->MMI(info,"TLitPhoton","First photon","run",fRun,ClassName());
  if (gCleanOut->Period2()) gCleanOut->MII(info,"Run",fRun,"Photon",fKphot);
  fChooseIndex   = ks;
  fX0            = x0;
  fStartGeoVol   = GeoVolume;
  fSourcePath    = path;
  fWaveLength    = wvlgth;
  fT0            = t0;
  fUk0           = K.Unit();
  fEf0           = E.Unit();
  fSl0           = zero;
  fSkl0          = zero;
  fNbOfDiffusion = 0;
  fNbOfRayleigh  = 0;
  fNbOfWLS       = 0;
  fAnomaly       = kFALSE;
  InitTracking();
  fI  = new TLitWave("incident","incident wave");
  fI->fShare  = Taken; //Incident wave always participates to process
  fI->fPhases = 1;     //By construction, one arbitrary, irrelevant, global phase
  fR1     = new TLitWave("reflected_1","1st reflected wave");
  fR2     = new TLitWave("reflected_2","2nd reflected wave");
  fT1     = new TLitWave("tranmitted_1","1st transmitted wave");
  fT2     = new TLitWave("tranmitted_2","2nd transmitted wave");
  StartPhoton();
}
TLitPhoton::TLitPhoton(Int_t run,Int_t kphot,const TVector3 &x0,TLitVolume *LitVolume,
  const char *path,Double_t wvlgth,Double_t t0,TVector3 &K,TVector3 &E,Short_t ks) {
  //Constructor of the photon. Wavelength, time of emission, point of emission
  //and k vector have already been generated. Direction of E vector has also
  //been generated, except in the case of generation inside anisotropic
  //material.
  //  run        : run number
  //  kphot      : photon number
  //  x0         : starting point of photon in World Coordinate System
  //  LitVolume  : TLitVolume in which photon is generated. As there can be many
  //               instances of the TGeoVolume associated with TLitVolume in the
  //               geometry, it is necessary to specify also "path"
  //  path       : path name of the TGeoVolume inside which photon is generated
  //               for instance "/TOP_1/REV_1/FIB_1"
  //  wvlgth     : wavelength of photon in nanometers
  //  t0         : start time of photon
  //  K          : k vector of photon in World Coordinate System
  //  E          : direction of E field in WCS, in case of isotropic material
  //               0 otherwise.
  //  ks         : choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //                in anisotropic material. In anisotropic material, there
  //                are 2 possible values for the index of refraction once
  //                the direction of the k vector is fixed.
  //
  const Double_t zero = 0.0;
  TGeoVolume *GeoVolume;
  InitP();
  Init();
  fRun           = run;
  fKphot         = kphot;
  fgNphot++;
  gCleanOut->RunEv(fRun,fKphot);
  if (fKphot==1) gCleanOut->MMI(info,"TLitPhoton","First photon","run",fRun,ClassName());
  if (gCleanOut->Period2()) gCleanOut->MII(info,"Run",fRun,"Photon",fKphot);
  fChooseIndex   = ks;
  fX0            = x0;
  GeoVolume      = LitVolume->GetGeoVolume();
  fStartGeoVol   = GeoVolume;
  fSourcePath    = path;
  fWaveLength    = wvlgth;
  fT0            = t0;
  fUk0           = K.Unit();
  fEf0           = E.Unit();
  fSl0           = zero;
  fSkl0          = zero;
  fNbOfDiffusion = 0;
  fNbOfRayleigh  = 0;
  fNbOfWLS       = 0;
  fAnomaly       = kFALSE;
  InitTracking();
  fI  = new TLitWave("incident","incident wave");
  fI->fShare  = Taken; //Incident wave always participates to process
  fI->fPhases = 1;     //By construction, one arbitrary, irrelevant, global phase
  fR1     = new TLitWave("reflected_1","1st reflected wave");
  fR2     = new TLitWave("reflected_2","2nd reflected wave");
  fT1     = new TLitWave("tranmitted_1","1st transmitted wave");
  fT2     = new TLitWave("tranmitted_2","2nd transmitted wave");
  StartPhoton();
}
TLitPhoton::~TLitPhoton() {
  // Destructor : TLitPhoton is only owner of TLitWave
  if (fI)  delete fI;
  if (fR1) delete fR1;
  if (fR2) delete fR2;
  if (fT1) delete fT1;
  if (fT2) delete fT2;
}
void TLitPhoton::AgainstRev() {
  //  Handles the case of a photon having just leave a TGeoVolume and entered
  // a new TGeoVolume which is a wrapping. A TGeoVolume is a wrapping if its
  // TGeoMedium is in fact a TLitMedium for which method SetAsWrapping() has
  // been called. The photon is just inside the new TGeoVolume.
  //
  const char *met = "AgainstRev";
  Double_t amat0,arev1;
  Int_t bin;
  Double_t y;
  Bool_t depolished;
  fBetween = kFALSE;
  amat0 = fLitMedium->GetMatCounterD();
  arev1 = fNextLitMedium->GetRevCounterD();
  if (fNextLitMedium->IsSlice()) {
    //There is a slice of material between the face and the wrapping
    fAnist    = 0;
    fAniat    = 0;
    // fPmut is the magnetic permeability mu of the slice, not of the wrapping
    // fRfIndext is the index of refraction of slice, not of wrapping
    fPmut     = fNextLitMedium->GetMu();
    fRfIndext = fNextLitMedium->GetIndex();
    //Depolishing of surface is forbidden if material anisotropic (too
    //complicate !)
    depolished = fLitMedium->IsDepolish(fNextLitMedium);
    if ((depolished) && (!fAnis0)) {
      Depolish();
      fOx = fFNd;
    }
    else fOx = fFN;
    if (!fAnomaly) {
      Transition();
      if (!fAnomaly) {
        if (fPhtOut) {
          fOutPoly = kTRUE;
          ExitFace();
        }// end if (fPhtOut)
        fOutPoly = kFALSE;
      }// end if (!fAnomaly)
    }// end if (!fAnomaly)
  }// end if (fNextLitMedium->IsSlice())
  else {
    //  There is no slice of material between the face and the wrapping
    //  In that case, it is forbidden that the material in front of the wrapping
    // be anisotropic
    if (fAnis0 && (fNextLitMedium->IsSupplAbs()!=2)) {
      gCleanOut->MM(warning,met,"Anisotropy in front of wrapping without slice",ClassName());
    }//end if (fAnis0)
    //  First we must check whether there is a supplementary
    // absorption (in addition to the normal absorption given by the Fresnel
    // equations)
    switch (fNextLitMedium->IsSupplAbs()) {
      case 1:
        y = gRandom3->Rndm();
        if (y<fNextLitMedium->GetSupplAbs()) fPhtAbs = kTRUE;
        break;
      case 2:
        fPhtAbs = kTRUE;
        break;
      default:
        break;
    }//end switch (fNextLitMedium->IsSupplAbs())
    if (fPhtAbs) {
      //Photon has been absorbed before the wrapping
      fPhtLive = kFALSE;
      gLitGs->fNpLossAny += 1;
      gLitGs->fNpAbsBef  += 1;
      bin = gLitGs->fHAbsAny->Fill(fSl0);
      bin = gLitGs->fHAbsBef->Fill(arev1);
      if (gLitCs) {
        gLitCs->fNpLossAny += 1;
        gLitCs->fNpAbsBef  += 1;
        bin = gLitCs->fHAbsAny->Fill(fSl0);
        bin = gLitCs->fHAbsBef->Fill(arev1);
      }
    }//end if (fPhtAbs)
    else {
      //Photon has NOT been absorbed before the wrapping. Photon is alive, standing
      //before the wrapping. A wrapping is never anisotropic ==> fAnist = 0;
      //We must decide whether the photon is reflected or diffused
      fAnist    = 0;
      fAniat    = 0;
      fPmut     = fNextLitMedium->GetWrapMu();
      fRfIndext = fNextLitMedium->GetWrapIndex();
      y = gRandom3->Rndm();
      if (y>fNextLitMedium->Diffus()) {
        //Here the photon is reflected
        depolished = fLitMedium->IsDepolish(fNextLitMedium);
        if (depolished) {
          PrintWhich();
          gCleanOut->MM(warning,met,"Depolish not allowed if no slice",ClassName());
          gCleanOut->MM(warning,met,"We continue without depolish",ClassName());
        }
        fOx = fFN;
        if (!fAnomaly) {
          Transition();
          if (!fAnomaly) {
            if (fPhtAbs) {
              //Photon has been absorbed by the wrapping
              fPhtLive = kFALSE;
              gLitGs->fNpLossAny += 1;
              gLitGs->fNpAbsRvt  += 1;
              bin = gLitGs->fHAbsAny->Fill(fSl0);
              bin = gLitGs->fHAbsRvt->Fill(arev1);
              if (gLitCs) {
                gLitCs->fNpLossAny += 1;
                gLitCs->fNpAbsRvt  += 1;
                bin = gLitCs->fHAbsAny->Fill(fSl0);
                bin = gLitCs->fHAbsRvt->Fill(arev1);
              }
            }// end if (fPhtAbs)
          }// end if (!fAnomaly)
        }// end if (!fAnomaly)
      }// end if (y>fNextLitMedium->Diffus())
      else {
        //Here the photon is diffused. We do not depolish in that case.
        fOx = fFN;
        Diffused();
      }// end else if (y>fNextLitMedium->Diffus())
    }// end else if (fPhtAbs)
  }// end else if (fNextLitMedium->IsSlice())
  if (fPhtLive) {
    //  if photon still alive, it means that it returns from the wrapping into
    // the original TGeoVolume
    TGeoNode *node;
    const Double_t *dir;
    bin = gLitGs->fHRevMat->Fill(arev1,amat0);
    if (gLitCs) bin = gLitCs->fHRevMat->Fill(arev1,amat0);
    fBetween = kFALSE;
    fOutPoly = kFALSE;
    // Unfortunately, we are already in the new TGeoVolume. A step backwards
    // is required!
    dir = gGeoManager->GetCurrentDirection();
    gGeoManager->SetCurrentDirection(-dir[0],-dir[1],-dir[2]);
    node = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    if (TLit::Get()->GetTestCalc()) {
      // step backward must be small. Else : error
      Double_t step = gGeoManager->GetStep();
      if (step>0.01) {
        fAnomaly = kTRUE;
        fPhtLive = kTRUE;
        gCleanOut->MMR(killthis,met,"Step should be infinitesimal","step",step,ClassName());
      }
    }//end if (TLit::Get()->GetTestCalc())
    if (node != fNode) {
      fAnomaly = kTRUE;
      fPhtLive = kTRUE;
      gCleanOut->MM(killthis,met,"Not going back into old TGeoVolume",ClassName());
    }
    else gGeoManager->SetCurrentDirection(fUpg.X(),fUpg.Y(),fUpg.Z());
    ClearNewVolume(kTRUE);
  }//end if (fPhtLive)
  else {
    bin = gLitGs->fHMatRev->Fill(amat0,arev1);
    if (gLitCs) bin = gLitCs->fHMatRev->Fill(amat0,arev1);
  }//end else if (fPhtLive)
}
void TLitPhoton::AgainstShape() {
  //  Handles the case of a photon having just leave a TGeoVolume and entered
  // a new TGeoVolume. The photon is just inside the new TGeoVolume. The new
  // TGeoVolume is NOT a wrapping [ in which case, AgainstRev() would have been
  // called ]. There may or may not be a thin slice between the 2 TGeoVolume.
  //  If the TLitMedium of the new TGeoVolume IS THE SAME as the TLitMedium of
  // the old TGeoVolume, then the photon simply continues with the same direction
  // fUpg in the new TGeoVolume.
  //  If the TLitMedium of the new TGeoVolume IS NOT THE SAME as the TLitMedium
  // of the old TGeoVolume, then either
  //  (1) - the Fresnel equation have to be applied if the 2 TLitMedium are
  // isotropic and there is no thin slice.
  //  (2) - if there is a thin slice, more complicated equations then the Fresnel
  // ones have to be applied. See:
  // http://gentit.web.cern.ch/gentit/litrani/birefringence/Physics_behind.pdf
  //  (3) - if one of the two TLitMedium is anisotropic, then the equations
  // described at
  // http://gentit.web.cern.ch/gentit/litrani/birefringence/Physics_behind.pdf
  // have to be applied
  //  (4) - it is forbidden [ not implemented ] that the two TLitMedium be both
  // anisotropic
  //
  const char *met = "AgainstShape";
  Int_t bin;
  Double_t amat0,amat1;
  if (fNextLitMedium == fLitMedium) {
    //Photon penetrates into the other TGeoVolume
    NewVolume();
  }
  else {
    //Here media are different, so that calculations of transition needed
    fBetween  = kTRUE;
    fPmu1     = fNextLitMedium->GetMu();
    fAnis1    = fNextLitMedium->GetAnisotropy();
    fAnia1    = fNextLitMedium->GetAniAbsL();
    if (fAnis1) fDzt1     = fNextLitVolume->fDTmu;
    else        fRfIndex1 = fNextLitMedium->GetIndex();
    fAnist    = fAnis1;
    fAniat    = fAnia1;
    fPmut     = fPmu1;
    if (fAnist) fDztt     = fDzt1;
    else        fRfIndext = fRfIndex1;
    //We do not accept depolish when in contact with an other TGeoVolume
    fOx = fFN;
    Transition();
    if (!fAnomaly) {
      amat0 = fLitMedium->GetMatCounterD();
      amat1 = fNextLitMedium->GetMatCounterD();
      if (fPhtOut) {
        //Photon penetrates into the new TGeoVolume
        bin = gLitGs->fHMatMat->Fill(amat0,amat1);
        if (gLitCs) bin = gLitCs->fHMatMat->Fill(amat0,amat1);
        NewVolume();
        fPmu0       = fLitMedium->GetMu();
        fAnis0      = fLitMedium->GetAnisotropy();
        fAnia0      = fLitMedium->GetAniAbsL();
        if (fAnis0) fDzt0 = fLitVolume->fDTmu;
        else fRfIndex0 = fLitMedium->GetIndex();
        fIsAbsorb   = fLitMedium->GetHasAbsorption();
        if (fIsAbsorb) {
          if (fAnia0) {
            fCDzt0 = fLitVolume->fCDTmu;
            FindAbsLength();
          }
          else fAbsl0 = fLitMedium->GetAbsL();
        }
        fIsRayleigh = fLitMedium->GetHasRayleigh();  
        fRayleighL  = fLitMedium->GetRayleighLength();
        fIsWLS      = fLitMedium->GetHasWLS();
        fWLSL       = fLitMedium->GetWLSL();
        fIsDiffus   = fLitMedium->GetHasDiffusion();
        fDiffusL    = fLitMedium->GetDiffLength();
        // We are already in the new TGeoVolume, but the direction of photon has changed
        gGeoManager->SetCurrentDirection(fUpg.X(),fUpg.Y(),fUpg.Z());
      }
      else {
        // Photon turns back and remain in the same TGeoVolume
        TGeoNode *node;
        const Double_t *dir;
        bin = gLitGs->fHMatNot->Fill(amat0,amat1);
        if (gLitCs) bin = gLitCs->fHMatNot->Fill(amat0,amat1);
        fBetween = kFALSE;
        fOutPoly = kFALSE;
        // Unfortunately, we are already in the new TGeoVolume. A step backwards
        // is required!
        dir = gGeoManager->GetCurrentDirection();
        gGeoManager->SetCurrentDirection(-dir[0],-dir[1],-dir[2]);
        node = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
        if (TLit::Get()->GetTestCalc()) {
          // step backward must be small. Else : error
          Double_t step = gGeoManager->GetStep();
          if (step>0.01) {
            fAnomaly = kTRUE;
            fPhtLive = kTRUE;
            gCleanOut->MMR(killthis,met,"Step should be infinitesimal","step",step,ClassName());
          }
        }//end if (TLit::Get()->GetTestCalc())
        if (node != fNode) {
          fAnomaly = kTRUE;
          fPhtLive = kTRUE;
          gCleanOut->MM(killthis,met,"Not going back into old TGeoVolume",ClassName());
        }
        else gGeoManager->SetCurrentDirection(fUpg.X(),fUpg.Y(),fUpg.Z());
        ClearNewVolume(kTRUE);
      }//end if (fPhtOut)
    }//end if (!fAnomaly)
  }//end if (fNextLitMedium == fLitMedium)
}
Bool_t TLitPhoton::AlfaDelta(TVector3 v,Double_t stheta, Double_t ctheta,
       Double_t &salfa, Double_t &calfa, Double_t &sdelta, Double_t &cdelta) {
// From the unit TVector3 v calculates alfa and delta for the decomposition :
//
//       x[0] = sin(delta)*cos(alfa)
//       x[1] = cos(delta)*cos(alfa)
//       x[2] = sin(alfa)
//
// AlfaDelta return true, except in the case where cos(alfa) is 0. In that
// case, delta is undefined, AlfaDelta returns false and arbitrarily puts
// sin(delta) = sin(theta), cos(delta) = cos(theta).
//
// The problem is undefined in the sense that there are 2 possible solutions :
// we can change the signs of calfa,sdelta and cdelta : we still have a solution.
// So we choose the solution which is closest stheta and ctheta.
//
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  Double_t calf2,sdelt2,cdelt2;
  Double_t dd1,dd2;
  Bool_t b;
  Double_t r;
  salfa = v(2);
  calfa = TMath::Sqrt(un - salfa*salfa);
  r     = TMath::Abs(calfa);
  if (r>wsmall) {
    b      = kTRUE;
    sdelta = v(0)/calfa;
    cdelta = v(1)/calfa;
    calf2  = - calfa;
    sdelt2 = - sdelta;
    cdelt2 = - cdelta;
    dd1    = TMath::Abs(sdelta - stheta) + TMath::Abs(cdelta - ctheta);
    dd2    = TMath::Abs(sdelt2 - stheta) + TMath::Abs(cdelt2 - ctheta);
    if (dd2<dd1) {
      calfa  = calf2;
      sdelta = sdelt2;
      cdelta = cdelt2;
    }
  }
  else {
    b      = kFALSE;
    salfa  = un;
    calfa  = zero;
    sdelta = stheta;
    cdelta = ctheta;
  }
  return b;
}
Bool_t TLitPhoton::AlfaDelta(const TVec3C &v,const TComplex &stheta, const TComplex &ctheta,
  TComplex &salfa, TComplex &calfa,TComplex &sdelta, TComplex &cdelta) {
//  From the unit T3CVector "this" calculates alfa and delta for the decomposition :
//
//       x[0] = sin(delta)*cos(alfa)
//       x[1] = cos(delta)*cos(alfa)
//       x[2] = sin(alfa)
//
//  AlfaDelta return true, except in the case where cos(alfa) is 0. In that
// case, delta is undefined, AlfaDelta returns false and arbitrarily puts
// sin(delta) = stheta, cos(delta) = ctheta.
//
//  The problem is undefined in the sense that there are 2 possible solutions :
// we can change the signs of calfa,sdelta and cdelta : we still have a solution.
// So we choose the solution which is closest stheta and ctheta.
//
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  TComplex calf2,sdelt2,cdelt2;
  Double_t dd1,dd2;
  Bool_t b;
  Double_t r;
  salfa = v(2);
  calfa = TComplex::Sqrt(un - salfa*salfa);
  r     = TComplex::Abs(calfa);
  if (r>wsmall) {
    b      = kTRUE;
    sdelta = v(0)/calfa;
    cdelta = v(1)/calfa;
    calf2  = zero - calfa;
    sdelt2 = zero - sdelta;
    cdelt2 = zero - cdelta;
    dd1    = TComplex::Abs(sdelta - stheta) + TComplex::Abs(cdelta - ctheta);
    dd2    = TComplex::Abs(sdelt2 - stheta) + TComplex::Abs(cdelt2 - ctheta);
    if (dd2<dd1) {
      calfa  = calf2;
      sdelta = sdelt2;
      cdelta = cdelt2;
    }
  }
  else {
    b      = kFALSE;
    salfa  = un;
    calfa  = zero;
    sdelta = stheta;
    cdelta = ctheta;
  }
  return b;
}
void TLitPhoton::AniToAni() const {
  //Calculates the transition from an anisotropic medium towards an anisotropic
  //medium
  PrintWhich();
  gCleanOut->MM(fatal,"AniToAni","Transition from anisotropic to anisotropic not implemented",ClassName());
}
void TLitPhoton::AniToIso() {
  //Calculates the transition from an anisotropic medium towards an isotropic
  //medium
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-5;
  const Double_t dixm2  = 1.0e-2;
  const Double_t wsmall = 1.0e-11;
  const Double_t DbLim  = 1.0e-150;
  const char *met = "AniToIso";
  TComplex zeps;
  Double_t eps;
  Double_t TestIm;
  Bool_t Good1;              //The 1st solution selected among the 4
  //proposed by TEqIndex is good or not
  Bool_t Good2;              //The 2nd solution selected among the 4
  //proposed by TEqIndex is good or not
  Short_t kIncoming;         //points at the one among the 4 solution which is
  //closest to the incoming wave. The incoming wave
  //appears among the 4 solutions and must be avoided !
  Short_t is;
  Short_t imin1,imin2;       //2 solutions among the 4 proposed by TEqIndex
  Bool_t  eva1,eva2;         //true if wave fR1(fR2) evanescent
  Bool_t isol[4];
  Double_t d00,d01,d02,d11,d12,d22;
  Double_t c01,c02,c12;
  Double_t a1,a3,a4,a5,gd;
  Double_t quality;          //quality of solution provided by TEqIndex
  Double_t ns;               //(index of refraction)*(sin(thetai) of incident wave
  Double_t a,b,c,d,g;        //coefficients for the equation giving the 2
  //indices for the 2 reflected waves
  Double_t dh1,dh2;          //intermediate calculations
  TComplex rac;              //square root of equation (48)
  TComplex z2;               //for test of equation (48)
  TComplex zh1;              //intermediate calculations
  TVector3 vh1;
  //
  Bool_t invers = kFALSE;    //true if the 2 solutions have been interchanged
  fR1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fT1->fRfI   = fRfIndext;   //assigns to T1 wave its index of refraction
  //Fresnel or Descartes formula, between incident and transmitted
  ns          = fI->fRfI.Re()*fI->fStht.Re();
  fT1->fStht  = ns/fT1->fRfI;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht = TLitPhoton::CosFromSin(fT1->fStht);
  //If fT1->fCtht is "almost" pure imaginary, the very small real part is due
  //to numerical imprecision. We clean it.
  if ((TMath::Abs(fT1->fCtht.Im())>wsmall) &&
    (TMath::Abs(fT1->fCtht.Re())<wsmall)) fT1->fCtht(0.0,fT1->fCtht.Im());
  //Later on, we will have to divide by fT1->fCtht. Hence we cannot accept it
  //to be 0. If it is the case, we signal an error, but continue giving a
  //small value to fT1->fCtht. fT1->fCtht = 0 corresponds to the case of a
  //grazing incident wave, which theoretically has a 0 probability to happen,
  //but can happen practically due to numerical imprecision.
  if (TComplex::Abs(fT1->fCtht)<DbLim) {
    PrintWhich();
    gCleanOut->MMC(error,met,"fT1->fCtht too small","fT1->fCtht",fT1->fCtht,ClassName());
    fT1->fCtht = DbLim;
  }
  // Here comes the calculation of the equation (48) (see theoretical paper)
  //giving the 2 possible indices for the 2 reflected waves
  g   = ns*ns;
  d00 = fDztii(0,0);
  d01 = fDztii(0,1);
  d02 = fDztii(0,2);
  d11 = fDztii(1,1);
  d12 = fDztii(1,2);
  d22 = fDztii(2,2);
  c01 = d01*d01;
  c02 = d02*d02;
  c12 = d12*d12;
  a1  = (d00-d11)*g + d00*(d11+d22) - c02 - c01;
  a3  = - deux*d01*ns;
  a4  = - deux*ns*(d01*d22-d02*d12);
  a5  = g*(c02 - c12 - d22*(d00-d11));
  a5 += - d00*d11*d22 - deux*d01*d02*d12 + d11*c02 + d00*c12 + d22*c01;
  // We have to divide by d00, so we cannot accept it to be 0
  if (TMath::Abs(d00) < DbLim) {
    PrintWhich();
    gCleanOut->MMR(killthis,met,"Abs(fDztii(0,0)) is too small","d00",d00,ClassName());
    fAnomaly = kTRUE;
    return;
  }
  fFromani = kTRUE;
  a = -a1/d00;
  b =  a3/d00;
  c = -a4/d00;
  d = -a5/d00;
  if (fIsDiagi) {
    gd  = d22 - d11;
    gd += (g*(d11 - d00))/d00;
    gd *= gd;
  }
  else {
    gd  = a*a - quatre*d;
  }
  gLitIdx->Set(a,b,c,d,g,gd);
  quality = gLitIdx->Solve(fAnisi);
  if (quality>small) {
    PrintWhich();
    gCleanOut->MMR(killthis,met,"Bad solution for index equation","quality",quality,ClassName());
    gCleanOut->MRR(killthis,"a",a,"b",b);
    gCleanOut->MRR(killthis,"c",c,"d",d);
    gCleanOut->MRR(killthis,"g",g,"gd",gd);
    fAnomaly = kTRUE;
    return;
  }
  Bool_t bii = (fI->fCtht.Re()>=zero);
  kIncoming = gLitIdx->ClosestFrom(fI->fRfI.Re(),bii);
  for (is=0;is<4;is++) isol[is] = kTRUE;
  Short_t NumberOfTry = 0;
bigloop: 
  do {
    Good1 = kFALSE;
    Good2 = kFALSE;
    NumberOfTry += 1;
    fR1->fPhases = 0;
    fR2->fPhases = 0;
    fT1->fPhases = 0;
    //We look for 2 possible solutions. Notice that we refuse that both reflective waves
    //be evanescent.
    Bool_t found = kFALSE;
    while (!found) {
      GetImin12(NumberOfTry,imin1,imin2,kIncoming);
      found = (isol[imin1]) && (isol[imin2]);
      if (!found) {
        NumberOfTry += 1;
        if (NumberOfTry>6) {
          PrintWhich();
          gCleanOut->MM(killthis,met,"All solutions tried without success",ClassName());
          fAnomaly = kTRUE;
          return;
        }// end if (NumberOfTry>6)
      }// end if (!found)
    }// end while (!found)
    eva1 = gLitIdx->GetfR(imin1);
    eva2 = gLitIdx->GetfR(imin2);
    //We accept that one of the 2 reflected wave be evanescent, but it must be the
    //second one !
    if ((eva1) && (!eva2)) {
      is    = imin1;
      imin1 = imin2;
      imin2 = is;
      eva1  = kFALSE;
      eva2  = kTRUE;
    }
    //We assign found value of index to the 1st reflected wave :
    fR1->fRfI = TComplex::Sqrt(gLitIdx->GetfZ(imin1));
    fR1->fEva = eva1;
    if (!eva1) fR1->fRfI(fR1->fRfI.Re(),0.0);
    //We assign found value of index to the 2nd reflected wave :
    fR2->fRfI = TComplex::Sqrt(gLitIdx->GetfZ(imin2));
    fR2->fEva = eva2;
    if (!eva2) fR2->fRfI(fR2->fRfI.Re(),0.0);
    //Fresnel or Descartes equation for the sinus of the 1st reflected wave
    fR1->fStht = ns/fR1->fRfI;
    fR1->fCtht = gLitIdx->GetfD(imin1);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fR1->fCtht.Im())>wsmall) &&
      (TMath::Abs(fR1->fCtht.Re())<wsmall)) fR1->fCtht(0.0,fR1->fCtht.Im());
    if ((TMath::Abs(fR1->fCtht.Re())>wsmall) &&
      (TMath::Abs(fR1->fCtht.Im())<wsmall)) fR1->fCtht(fR1->fCtht.Re(),0.0);
    else fR1->fEva = kTRUE;
    if (TLit::Get()->GetTestCalc()) {
    // Check sin^2 + cos^2 = 1
      zeps = 1.0 - fR1->fStht*fR1->fStht - fR1->fCtht*fR1->fCtht;
      eps  = zeps.Rho();
      if (eps>small)
        gCleanOut->MMR(error,met,"Bad sin/cos for fR1","test",eps,ClassName());
    // Check that equation (48) is satisfied with solution (1)
      z2  = fR1->fRfI*fR1->fRfI;
      rac = fR1->fRfI*fR1->fCtht;
      zeps = z2*z2 + a*z2 + d + (b*z2 + c)*rac;
      if (TComplex::Abs(zeps)>small) {
        gCleanOut->MM(error,met,"Bad solution of equation (48)",ClassName());
      }
    }
    //Fresnel or Descartes equation for the sinus of the 2nd reflected wave
    fR2->fStht = ns/fR2->fRfI;
    fR2->fCtht = gLitIdx->GetfD(imin2);
    //We clean very very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical, we
    //at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fR2->fCtht.Im())>wsmall) &&
      (TMath::Abs(fR2->fCtht.Re())<wsmall)) fR2->fCtht(0.0,fR2->fCtht.Im());
    if ((TMath::Abs(fR2->fCtht.Re())>wsmall) &&
      (TMath::Abs(fR2->fCtht.Im())<wsmall)) fR2->fCtht(fR2->fCtht.Re(),0.0);
    else fR2->fEva = kTRUE;
    if (TLit::Get()->GetTestCalc()) {
    // Check sin^2 + cos^2 = 1
      zeps = 1.0 - fR2->fStht*fR2->fStht - fR2->fCtht*fR2->fCtht;
      eps  = zeps.Rho();
      if (eps>small)
        gCleanOut->MMR(error,met,"Bad sin/cos for fR2","test",eps,ClassName());
    //Check that equation (48) is satisfied with solution (2)
      z2  = fR2->fRfI*fR2->fRfI;
      rac = fR2->fRfI*fR2->fCtht;
      zeps = z2*z2 + a*z2 + d + (b*z2 + c)*rac;
      if (TComplex::Abs(zeps)>small) {
        gCleanOut->MM(error,met,"Bad solution of equation (48)",ClassName());
      }
    }
    //Again, we must check that both waves are not evanescent.
    if ((fR1->fEva) && (fR2->fEva)) continue;
    //We must check that if only one wave evanescent,then it is the second
    if ((fR1->fEva) && (!fR2->fEva)) {
      TComplex cc1,cc2,cc3;
      invers     = kTRUE;
      cc1        = fR2->fRfI;
      cc2        = fR2->fStht;
      cc3        = fR2->fCtht;
      fR2->fRfI  = fR1->fRfI;
      fR2->fStht = fR1->fStht;
      fR2->fCtht = fR1->fCtht;
      fR1->fRfI  = cc1;
      fR1->fStht = cc2;
      fR1->fCtht = cc3;
      fR1->fEva  = kFALSE;
      fR2->fEva  = kTRUE;
    }
    //
    //Determination of alfa (Salf,Calf) and delta (Sdel,Cdel) by solution of the
    //degenerate system (4), (5), (6) [see theoretical paper]. We separate the
    //real and the complex case.
    //
    //1st reflected wave :
    //
    if (fR1->fEva) {
      TComplex za,z00,z01,z02,z11,z12,z22;
      za  = fR1->fRfI*fR1->fCtht;
      z00 = g - d00;
      z01 = ns*za - d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = d12;
      za  = fR1->fRfI;
      z22 = za*za - d22;
      TMat33C M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      TVec3C v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st refl wave : matrix M (complex) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st refl wave : matrix M (complex) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"1st refl wave : matrix M (complex) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = TLitPhoton::AlfaDelta(v1,fR1->fStht,fR1->fCtht,
        fR1->fSalf,fR1->fCalf,fR1->fSdel,fR1->fCdel);
//    bdeg = v1.AlfaDelta(fR1->fStht,fR1->fCtht,
//      fR1->fSalf,fR1->fCalf,fR1->fSdel,fR1->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fR1->fRfI.Re()*fR1->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa - d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = d12;
      qa  = fR1->fRfI.Re();
      q22 = qa*qa - d22;
      TMat33 M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      TVector3 v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st refl wave : matrix M (real) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st refl wave : matrix M (real) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"1st refl wave : matrix M (real) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = TLitPhoton::AlfaDelta(v1,fR1->fStht.Re(),fR1->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fR1->fSalf = qsa;
      fR1->fCalf = qca;
      fR1->fSdel = qsd;
      fR1->fCdel = qcd;
    }
    //
    //2nd reflected wave :
    //
    if (fR2->fEva) {
      TComplex za,z00,z01,z02,z11,z12,z22;
      za  = fR2->fRfI*fR2->fCtht;
      z00 = g - d00;
      z01 = ns*za - d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = d12;
      za  = fR2->fRfI;
      z22 = za*za - d22;
      TMat33C M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      TVec3C v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd refl wave : matrix M (complex) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd refl wave : matrix M (complex) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"2nd refl wave : matrix M (complex) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = TLitPhoton::AlfaDelta(v1,fR2->fStht,fR2->fCtht,
        fR2->fSalf,fR2->fCalf,fR2->fSdel,fR2->fCdel);
//      bdeg = v1.AlfaDelta(fR2->fStht,fR2->fCtht,
//        fR2->fSalf,fR2->fCalf,fR2->fSdel,fR2->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fR2->fRfI.Re()*fR2->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa - d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = d12;
      qa  = fR2->fRfI.Re();
      q22 = qa*qa - d22;
      TMat33 M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      TVector3 v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd refl wave : matrix M (real) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
//        TMat33::fgWidth = 20;
//        TMat33::fgPrec  = 12;
        M.Print();
        gCleanOut->MM(warning,met,"2nd refl wave : matrix M (real) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"2nd refl wave : matrix M (real) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = TLitPhoton::AlfaDelta(v1,fR2->fStht.Re(),fR2->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fR2->fSalf = qsa;
      fR2->fCalf = qca;
      fR2->fSdel = qsd;
      fR2->fCdel = qcd;
    }
    //
    //Here are the variables that we have determined until now :
    //
    //    fR1->fRfI
    //    fR1->fStht and fCtht
    //    fR1->fSalf and fCalf
    //    fR1->fSdel and fCdel
    //    fR2->fRfI
    //    fR2->fStht and fCtht
    //    fR2->fSalf and fCalf
    //    fR2->fSdel and fCdel
    //    fT1->fRfI
    //    fT1->fStht and fCtht
    //
    //Here are the variables that remain to be found :
    //
    //    fR1->fE
    //    fR2->fE
    //    fT1->fE
    //    fT1->fSalf and fCalf
    //
    //These last variables are found thanks to equations (10) (11) (12) (13)
    // (See theoretical paper)
    //
    //
    //Solution for fR1->fE and fR2->fE
    //
    zh1  = fPmut/(fT1->fRfI*fPmui);
    fBI  = fI->fRfI.Re()*zh1;
    fBR1 = fR1->fRfI*zh1;
    fBR2 = fR2->fRfI*zh1;
    fI->fAlm1  = fI->fStht*fI->fSdel + fI->fCtht*fI->fCdel;
    fR1->fAlm1 = fR1->fStht*fR1->fSdel + fR1->fCtht*fR1->fCdel;
    fR2->fAlm1 = fR2->fStht*fR2->fSdel + fR2->fCtht*fR2->fCdel;
    TComplex F11,F12,F13,F21,F22,F23,F31,F32,F33,F41,F42,F43,disc;
    F11  = fBI*fI->fCalf*fI->fAlm1;
    F12  = fBR1*fR1->fCalf*fR1->fAlm1;
    F13  = fBR2*fR2->fCalf*fR2->fAlm1;
    F21  = (fI->fCdel*fI->fCalf)/fT1->fCtht;
    F22  = zero - (fR1->fCdel*fR1->fCalf)/fT1->fCtht;
    F23  = zero - (fR2->fCdel*fR2->fCalf)/fT1->fCtht;
    F31  = fI->fSalf;
    F32  = fR1->fSalf;
    F33  = fR2->fSalf;
    F41  = (fBI*fI->fSalf*fI->fCtht)/fT1->fCtht;
    F42  = zero - (fBR1*fR1->fSalf*fR1->fCtht)/fT1->fCtht;
    F43  = zero - (fBR2*fR2->fSalf*fR2->fCtht)/fT1->fCtht;
    disc = (F12-F22)*(F33-F43) - (F13-F23)*(F32-F42);
    //
    //We have to divide by disc, so it cannot be extremely small. If we find it
    //extremely small, we take it as a sign that we have made a wrong choice of
    //solution from TLitEqIndex. For instance, if by error TLitEqIndex has given twice
    //the same solution, disc will be 0. So we try an other pair of solution.
    //
    if (TComplex::Abs(disc)<DbLim) continue;
    zh1 = fI->fE/disc;
    fR1->fE = zh1*((F21-F11)*(F33-F43) - (F41-F31)*(F13-F23));
    fR2->fE = zh1*((F12-F22)*(F41-F31) - (F32-F42)*(F21-F11));
    //We do not accept that the real part of the module of an electric field be <0
    if (fR1->fE.Re()<zero) {
      fR1->fE    = zero - fR1->fE;
      fR1->fSalf = zero - fR1->fSalf;
      fR1->fCalf = zero - fR1->fCalf;
      F12        = zero - F12;
      F22        = zero - F22;
      F32        = zero - F32;
      F42        = zero - F42;
    }
    if (fR2->fE.Re()<zero) {
      fR2->fE    = zero - fR2->fE;
      fR2->fSalf = zero - fR2->fSalf;
      fR2->fCalf = zero - fR2->fCalf;
      F13        = zero - F13;
      F23        = zero - F23;
      F33        = zero - F33;
      F43        = zero - F43;
    }
    //
    //Only 2 variables remain to be found :
    //
    //    fT1->fE
    //    fT1->fSalf and fCalf
    //
    TComplex tca,tsa;
    tca = F11*fI->fE + F12*fR1->fE + F13*fR2->fE;
    tsa = F31*fI->fE + F32*fR1->fE + F33*fR2->fE;
    fT1->fE    = TComplex::Sqrt(tsa*tsa + tca*tca);
    if (fT1->fE.Re()<zero) fT1->fE = zero - fT1->fE;
    fT1->fCalf = tca/fT1->fE;
    fT1->fSalf = tsa/fT1->fE;
    //
    //All the variables of system of equations (E1) have no been found ! We calculate
    //now the other physical values depending upon these variables
    //
    //
    //  Last calculations for wave fR1
    //
    // - module and phase of electric field :
    TLitPhoton::RPhi(fR1->fE,fR1->fAE,fR1->fPE);
    if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
    else fR1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fR1->fCalf,fR1->fWcaf,fR1->fPcaf);
    TLitPhoton::RPhi(fR1->fSalf,fR1->fWsaf,fR1->fPsaf);
    if (TMath::Abs(fR1->fPcaf - fR1->fPsaf)>wsmall)  fR1->fPhases += 1;
    // - angle of refraction complex for the 1st reflective wave ? No !
    if (TMath::Abs(fR1->fCtht.Im()) + TMath::Abs(fR1->fStht.Im())>wsmall)
      fR1->fPhases = 3;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //phase of electric field
      fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR1->fSdel.Im())>small) ||
        (TMath::Abs(fR1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fR1->fSdel(fR1->fSdel.Re(),0.0);
      fR1->fCdel(fR1->fCdel.Re(),0.0);
      fR1->fEf.SetXYZ(- fR1->fAE*fR1->fSdel.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCdel.Re()*fR1->fWcaf,
        fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR1 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR1->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUp(0) = dh1;
      }
      if (dh1>zero) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different reflected
      //waves ! To be absolutly sure about that, we still leave this possibility
      //open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"Electric field fR1->fEf has 2 phases",ClassName());
      gCleanOut->MM(error,met,"This should not happen in anisotropic media",ClassName());
      //
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //the angle delta of the decomposition of the electric fiel cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR1->fSdel.Im())>small) ||
        (TMath::Abs(fR1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fR1->fSdel(fR1->fSdel.Re(),0.0);
      fR1->fCdel(fR1->fCdel.Re(),0.0);
      //electric field
      fR1->fEfp.SetXYZ(- fR1->fAE*fR1->fSdel.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCdel.Re()*fR1->fWcaf,
        zero);
      fR1->fEfn.SetXYZ(zero,zero,fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmui);
      BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR1 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR1->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUpp(0) = dh1;
      }
      dh2 = fR1->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR1->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    default:
      //
      //The 1st reflected wave cannot be absorbed
      //
      if (invers) isol[imin2] = kFALSE;
      else        isol[imin1] = kFALSE;
      continue;
    }// end switch (fR1->fPhases)
    //
    //  Last calculations for wave fR2
    //
    // - module and phase of electric field :
    TLitPhoton::RPhi(fR2->fE,fR2->fAE,fR2->fPE);
    if (TMath::Abs(fR2->fPE)>wsmall) fR2->fPhases += 1;
    else fR2->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fR2->fCalf,fR2->fWcaf,fR2->fPcaf);
    TLitPhoton::RPhi(fR2->fSalf,fR2->fWsaf,fR2->fPsaf);
    if (TMath::Abs(fR2->fPcaf - fR2->fPsaf)>wsmall)  fR2->fPhases += 1;
    // - angle of refraction complex for the 2nd reflective wave ?
    if (TMath::Abs(fR2->fCtht.Im()) + TMath::Abs(fR2->fStht.Im())>wsmall)
      fR2->fPhases = 3;
    switch (fR2->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      //Notice the - sign in front of fR2->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      fR2->fUk.SetXYZ(-fR2->fCtht.Re(),fR2->fStht.Re(),zero);
      //The index of refraction of the 2nd reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      //Module of k vector
      fR2->fAk = (fW*fR2->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR2->fVk = fR2->fAk*fR2->fUk;
      //phase of electric field
      fR2->fPE += (fR2->fPcaf + fR2->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR2->fSdel.Im())>small) ||
        (TMath::Abs(fR2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fR2->fSdel(fR2->fSdel.Re(),0.0);
      fR2->fCdel(fR2->fCdel.Re(),0.0);
      fR2->fEf.SetXYZ(- fR2->fAE*fR2->fSdel.Re()*fR2->fWcaf,
        - fR2->fAE*fR2->fCdel.Re()*fR2->fWcaf,
        fR2->fAE*fR2->fWsaf);
      BuildBPUP(fR2->fVk,fR2->fEf,fR2->fBf,fR2->fPg,fR2->fUp,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR2 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR2->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR2->fUp(0) = dh1;
      }
      if (dh1>zero) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different reflected
      //waves ! To be absolutly sure about that, we still leave this possibility
      //open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"Electric field fR2->fEf has 2 phases",ClassName());
      gCleanOut->MM(error,met,"This should not happen in anisotropic media",ClassName());
      //
      //Notice the - sign in front of fR2->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR2->fUk.SetXYZ(-fR2->fCtht.Re(),fR2->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      //Module of k vector
      fR2->fAk = (fW*fR2->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR2->fVk = fR2->fAk*fR2->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR2->fSdel.Im())>small) ||
        (TMath::Abs(fR2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fR2->fSdel(fR2->fSdel.Re(),0.0);
      fR2->fCdel(fR2->fCdel.Re(),0.0);
      //electric field
      fR2->fEfp.SetXYZ(- fR2->fAE*fR2->fSdel.Re()*fR2->fWcaf,
        - fR2->fAE*fR2->fCdel.Re()*fR2->fWcaf,
        zero);
      fR2->fEfn.SetXYZ(zero,zero,fR2->fAE*fR2->fWsaf);
      BuildBPUP(fR2->fVk,fR2->fEfp,fR2->fBfp,fR2->fPgp,fR2->fUpp,fPmui);
      BuildBPUP(fR2->fVk,fR2->fEfn,fR2->fBfn,fR2->fPgn,fR2->fUpn,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR2 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR2->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR2->fUpp(0) = dh1;
      }
      dh2 = fR2->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR2->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    default:
      //
      //The 2nd reflected CAN be absorbed, but if it is the case, we have nothing to
      //compute !
      //
      Good2 = kTRUE;
      break;
    }// end switch (fR2->fPhases)
    //
    //  Last calculations for wave fT1
    //
    //Transmitted wave is in isotropic medium, so that angle delta = theta
    fT1->fSdel = fT1->fStht;
    fT1->fCdel = fT1->fCtht;
    // - module and phase of electric field :
    TLitPhoton::RPhi(fT1->fE,fT1->fAE,fT1->fPE);
    if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
    else fT1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fT1->fCalf,fT1->fWcaf,fT1->fPcaf);
    TLitPhoton::RPhi(fT1->fSalf,fT1->fWsaf,fT1->fPsaf);
    if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
    // - angle of refraction complex for the 1st transmitted wave ?
    TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
      TMath::Abs(fT1->fRfI.Im());
    if (TestIm>wsmall) fT1->fPhases = 3;
    switch (fT1->fPhases) {
    case 0:
    case 1:
      //Here either no phase, or one global phase. Parallel and normal components
      //of the wave have the same phase. No reason to distinguish between the 2.
      //The transmitted wave is not elliptically polarized.
      fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //If the phases are 0,1 or 2 it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","fT1->fRfI",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
      fT1->fEf.SetXYZ(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
      //Verify that the transmitted wave goes forward, up to a very small numerical
      //tolerance
      dh1 = fT1->fUp(0);
      if ((dh1<zero) && (dh1>-wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if (dh1<zero) {
        PrintWhich();
        gCleanOut->MMR(killthis,met,"transmitted wave going backward","dh1",dh1,ClassName());
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (TLit::Get()->GetTestCalc()) {
        vh1 = fT1->fEf;
        vh1 = vh1.Unit();
        dh1 = TMath::Abs(vh1*fT1->fUk);
        if (dh1>small) {
          PrintWhich();
          gCleanOut->MMR(killthis,met,"k and E not orthogonal","dh1",dh1,ClassName());          
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    case 2:
      //  Here, there are 2 phases. Parallel and normal component have different phases,
      //so that they have to be calculated separatly. Later on, we will have to choose
      //between these 2 waves.
      fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //If the phases are 0,1 or 2 it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","fT1->fRfI",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      //electric field
      fT1->fEfp.SetXYZ(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
        zero);
      fT1->fEfn.SetXYZ(zero,zero,fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
      BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
      //Verify that the transmitted waves go forward, up to a very small numerical
      //tolerance
      dh1 = fT1->fUpp(0);
      dh2 = fT1->fUpn(0);
      if ((dh1<zero) && (dh1>-wsmall)) {
        dh1 = - dh1;
        fT1->fUpp(0) = dh1;
      }
      if ((dh2<zero) && (dh2>-wsmall)) {
        dh2 = - dh2;
        fT1->fUpn(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        PrintWhich();
        gCleanOut->MM(killthis,met,"transmitted wave going backward",ClassName());
        gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (TLit::Get()->GetTestCalc()) {
        vh1 = fT1->fEfp;
        vh1 = vh1.Unit();
        dh1 = TMath::Abs(vh1*fT1->fUk);
        vh1 = fT1->fEfn;
        vh1 = vh1.Unit();
        dh2 = TMath::Abs(vh1*fT1->fUk);
        if ((dh1>small) || (dh2>small)) {
          PrintWhich();
          gCleanOut->MM(killthis,met,"k and E not orthogonal",ClassName());
          gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    default:
      //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
      //anything
      break;
    }//end switch (fT1->fPhases)
  }// end do while
  while (((!Good1) || (!Good2)) && (NumberOfTry<6));
  //Verify that the system of 13 equations has been solved properly
  if (TLit::Get()->GetTestCalc()) VerifyAtoI();
  if (fAnomaly) return;
  //
  //  After all these complicate calculations, we have at hand a powerful check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved! Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  //  We HAVE to do this test in any case, since a non-conservation of flux
  //may be the sign that we have taken a bad pair of solutions and that we
  //have to try an other one. ( We leave the "if ( 1 || TLit::Get()->GetTestCalc())"
  //in the case that it appears experimentally that it never happens. In
  //that case, we could render the test optional. )
  //
  if ( 1 /* TLit::Get()->GetTestCalc() */ ) {
    Double_t influx,outflux,dflux;
    influx = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 1  fT1 : 1
      outflux = fT1->fPg(0) - fR1->fPg(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 1  fT1 : 2
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 1  fR2 : 1  fT1 : 3
      //Transmitted wave is absorbed.
      //Transmitted wave may be absorbed :
      //  (1) - because of total reflection. In that case the refraction index
      //        of the outer material is real and the energy is conserved.
      //  (2) - because of reflection on metallic surface. In that case the
      //        refraction index of the outer material is complex and the
      //        energy is not conserved ( the absorbed wave takes energy )
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else  outflux = - fR1->fPg(0) - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 1  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 3  fT1 : 1
      outflux = fT1->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 3  fT1 : 2
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1  fR2 : 3  fT1 : 3
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    case 2:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 1  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 1  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 2  fR2 : 1  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      outflux =   fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 2  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 3  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 3  fT1 : 2   !!! theoretically impossible  !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2  fR2 : 3  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    default:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 1  fT1 : 1   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [1]",ClassName());
      outflux = fT1->fPg(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 1  fT1 : 2   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [2]",ClassName());
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 3  fR2 : 1  fT1 : 3   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [3]",ClassName());
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [4]",ClassName());
      gCleanOut->MM(error,met,"Solution theoretically impossible",ClassName());
      outflux = fT1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [5]",ClassName());
      gCleanOut->MM(error,met,"Solution theoretically impossible",ClassName());
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 3  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fR1 and fR2 out of order [6]",ClassName());
      gCleanOut->MM(error,met,"Solution theoretically impossible",ClassName());
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 3  fT1 : 1
      PrintWhich();
      gCleanOut->MM(error,met,"All reflected waves absorbed [1]",ClassName());
      outflux = fT1->fPg(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 3  fT1 : 2
      PrintWhich();
      gCleanOut->MM(error,met,"All reflected waves absorbed [2]",ClassName());
      outflux = fT1->fPgn(0) + fT1->fPgp(0);
      break;
    default:
      // fR1 : 3  fR2 : 3  fT1 : 3
      PrintWhich();
      gCleanOut->MM(error,met,"All reflected waves absorbed [3]",ClassName());
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = zero;
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    }// end switch (fR1->fPhases)
    dflux = influx - outflux;
    if (TMath::Abs(dflux)>dixm2) {
      PrintWhich();
      gCleanOut->MMR(warning,met,"Flux not conserved","dflux",dflux,ClassName());
      gCleanOut->MM(warning,met,"This may be due to a bad choice of solutions",ClassName());
      gCleanOut->MM(warning,met," from TLitEqIndex. We try an other choice",ClassName());
      // Correction of Alessandro Thea. Instead of:
      //   goto bigloop;
      if (NumberOfTry<7) goto bigloop;
      else {
        PrintWhich();
        gCleanOut->MM(killthis,met,"All solutions tried without success",ClassName());
        fAnomaly = kTRUE;
        return;
      }
      // End Correction of Alessandro Thea.
    }//end if (TMath::Abs(dflux)>dixm2)
  }//end if (1)
}
void TLitPhoton::BuildBPUP(const TVector3 &k,const TVector3 &E,TVector3 &B,TVector3 &P,
  TVector3 &uP,Double_t mu) const {
  //  When the k vector and the E field vector are defined, calculates B field,
  // the Poyinting vector and unit Poyinting vector. The calculations are
  // the same in the isotropic and anisotropic cases.
  // The conversion factor conv is due to the fact that the k vector is in [cm-1]
  // instead of [m-1] and that the frequency fW is in [ps-1] instead of [s-1]
  const Double_t conv   = 1.0e-10;
  const Double_t wsmall = 1.0e-300;
  Double_t y;
  B  = k.Cross(E);
  y  = conv/fW;
  B  = y*B;
  y  = (TLitPhys::Get()->Eps0()*TLitPhys::Get()->Cmks()*TLitPhys::Get()->Cmks())/mu;
  P  = E.Cross(B);
  P  = y*P;
  uP = P;
  y  = uP.Mag2();
  if (y>wsmall) uP = uP.Unit();
}
void TLitPhoton::ch1111() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1111","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1112() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1112","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1121() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1121","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1113() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1113","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1113","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1114() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1114","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1122() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1122","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1123() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1123","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1123","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1124() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1124","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1131() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++) 
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1131","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1131","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1132() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) 
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1132","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1132","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1133() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  for (k=0;k<2;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1133","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1133","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1141() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1141","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1142() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPgn(0));
  fWeigth[3] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1142","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1144() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  kwhich = Toss(2);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1144","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1211() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1211","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1212() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1212","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1213() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1213","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1213","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1214() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1214","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1221() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1221","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1222() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1222","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1223() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1223","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR1->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR1->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1223","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1224() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1224","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1231() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1231","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1231","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1232() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1232","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1232","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1233() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1233","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1233","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1241() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1241","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1242() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1242","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1244() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1244","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1311() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1311","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1311","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1312() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1312","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1312","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1313() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<3;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1313","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1313","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1321() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1321","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1321","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1322() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1322","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1322","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1323() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1323","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1323","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1331() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<3;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1331","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1331","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1332() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPgn(0));
  fWeigth[3] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1332","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1332","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch1333() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  outflux = fWeigth[0];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch1333","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(2);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch1333","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2111() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2111","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2112() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2112","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2113() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2113","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2113","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2114() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2114","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2121() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2121","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2122() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2122","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2123() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2123","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2123","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2124() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2124","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2131() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2131","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2131","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2132() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2132","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2132","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2133() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2133","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2133","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2141() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2141","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2142() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2142","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2144() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2144","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2211() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2211","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2212() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2212","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2213() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2213","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2213","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2214() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2214","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2221() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  fWeigth[6] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2221","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2222() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  fWeigth[6] = TMath::Abs(fT2->fPgn(0));
  fWeigth[7] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(8);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 7:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2222","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2223() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<6;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2223","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[6] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2223","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2224() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2224","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2231() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<6;k++) 
    if (k!=4) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2231","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2231","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2232() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<7;k++) 
    if (k!=4) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2232","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2232","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2233() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2233","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2233","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2241() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2241","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2242() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2242","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2244() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2244","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2311() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2311","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2311","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2312() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2312","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2312","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2313() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2313","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2313","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2321() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<6;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2321","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2321","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2322() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<7;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2322","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2322","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2323() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2323","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2323","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2331() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2331","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2331","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2332() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2332","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2332","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::ch2333() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  for (k=0;k<2;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gCleanOut->MM(error,"ch2333","Energy creation ?",ClassName());
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gCleanOut->MM(error,"ch2333","Impossible choice",ClassName());
    break;
  }
}
void TLitPhoton::Choose() {
  //  Among all possible waves, chooses the one to take according to weights
  //given by the fluxes of energy across the x=0 plane. This is given by the
  //x component of the Poyinting vector of each wave. The maximum possible
  //number of waves is 8 ! ( yes 8 ! ). An absorbed wave may be chosen (for
  //instance transmitted wave against a wrapping with complex index). In that
  //case the photon dies.
  //
  //  In fact, this is not true that there are 8 possible waves. It would be
  //true in the case where the possibility would exist, in anisotropic media,
  //that the parallel and the normal components of a wave has to have to be
  //splitted, a case that we have decided to leave open in the program ( in
  //order to really check that it does not happen !). But in fact, it does not
  //happen, so that the maximum number of waves is 4.
  //
  const char *met = "Choose";
  if (fR1->fShare == NotInvolved) {
    gCleanOut->MM(error,met,"No reflected wave in process",ClassName());
  }
  if (fT1->fShare == NotInvolved) {
    gCleanOut->MM(error,met,"No transmitted wave in process",ClassName());
  }
  if (fR2->fShare == NotInvolved) fR2->fPhases = 4;
  if (fT2->fShare == NotInvolved) fT2->fPhases = 4;
  switch (fR1->fPhases) {
  case 0:
  case 1:
    // fR1 : 1
    switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 1
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 1 | fT1 : 1 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 1
          ch1111();
          break;
        case 2:
          // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1112]",ClassName());
          ch1112();
          break;
        case 3:
          // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 3
          ch1113();
          break;
        default:
          // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : N
          ch1114();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 1 | fT1 : 1 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1121]",ClassName());
          ch1121();
          break;
        case 2:
          // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1122]",ClassName());
          ch1122();
          break;
        case 3:
          // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1123]",ClassName());
          ch1123();
          break;
        default:
          // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1124]",ClassName());
          ch1124();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 1 | fT1 : 1 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 1
          ch1131();
          break;
        case 2:
          // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1132]",ClassName());
          ch1132();
          break;
        case 3:
          // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 3
          ch1133();
          break;
        default:
          // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : N
          ch1133();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 1 | fT1 : 1 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 1
          ch1141();
          break;
        case 2:
          // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1142]",ClassName());
          ch1142();
          break;
        case 3:
          // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 3
          ch1133();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : N
          ch1144();
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    case 2:
      // fR1 : 1 | fT1 : 2
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 1 | fT1 : 2 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1211]",ClassName());
          ch1211();
          break;
        case 2:
          // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1212]",ClassName());
          ch1212();
          break;
        case 3:
          // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 3
          ch1213();
          break;
        default:
          // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : N
          ch1214();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 1 | fT1 : 2 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1221]",ClassName());
          ch1221();
          break;
        case 2:
          // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1222]",ClassName());
          ch1222();
          break;
        case 3:
          // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1223]",ClassName());
          ch1223();
          break;
        default:
          // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1224]",ClassName());
          ch1224();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 1 | fT1 : 2 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1231]",ClassName());
          ch1231();
          break;
        case 2:
          // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1232]",ClassName());
          ch1232();
          break;
        case 3:
          // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 3
          ch1233();
          break;
        default:
          // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : N
          ch1233();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 1 | fT1 : 2 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1241]",ClassName());
           ch1241();
          break;
        case 2:
          // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1242]",ClassName());
          ch1242();
          break;
        case 3:
          // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 3
          ch1233();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : N
          ch1244();
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    default:
      // fR1 : 1 | fT1 : 3
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 1 | fT1 : 3 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 1
          ch1311();
          break;
        case 2:
          // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 2
          ch1312();
          break;
        case 3:
          // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 3
          ch1313();
          break;
        default:
          // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : N
          ch1313();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 1 | fT1 : 3 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1321]",ClassName());
          ch1321();
          break;
        case 2:
          // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1322]",ClassName());
          ch1322();
          break;
        case 3:
          // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1323]",ClassName());
           ch1323();
          break;
        default:
          // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [1323]",ClassName());
          ch1323();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 1 | fT1 : 3 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 1
          ch1331();
          break;
        case 2:
          // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 2
          ch1332();
          break;
        case 3:
          // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 3
          ch1333();
          break;
          default:
          // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : N
          ch1333();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 1 | fT1 : 3 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 1
          ch1331();
          fR2->fShare = NotInvolved;
          break;
        case 2:
          // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 2
          ch1332();
          fR2->fShare = NotInvolved;
          break;
        case 3:
          // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 3
          ch1333();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : N
          ch1333();
          fR2->fShare = NotInvolved;
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    }//end switch (fT1->fPhases)
    break;
  case 2:
    // fR1 : 2
    switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 1
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 2 | fT1 : 1 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2111]",ClassName());
          ch2111();
          break;
        case 2:
          // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2112]",ClassName());
          ch2112();
          break;
        case 3:
          // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2113]",ClassName());
          ch2113();
          break;
        default:
          // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2114]",ClassName());
          ch2114();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 2 | fT1 : 1 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2121]",ClassName());
          ch2121();
          break;
        case 2:
          // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2122]",ClassName());
          ch2122();
          break;
        case 3:
          // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2123]",ClassName());
          ch2123();
          break;
        default:
          // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2124]",ClassName());
          ch2124();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 2 | fT1 : 1 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 1
          ch2131();
          break;
        case 2:
          // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2132]",ClassName());
          ch2132();
          break;
        case 3:
          // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 3
          ch2133();
          break;
        default:
          // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : N
          ch2133();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 2 | fT1 : 1 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 1
          ch2141();
          break;
        case 2:
          // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2142]",ClassName());
          ch2142();
          break;
        case 3:
          // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 3
          ch2133();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : N
          ch2144();
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    case 2:
      // fR1 : 2 | fT1 : 2
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 2 | fT1 : 2 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2211]",ClassName());
          ch2211();
          break;
        case 2:
          // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2212]",ClassName());
          ch2212();
          break;
        case 3:
          // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2213]",ClassName());
          ch2213();
          break;
        default:
          // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2214]",ClassName());
          ch2214();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 2 | fT1 : 2 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2221]",ClassName());
          ch2221();
          break;
        case 2:
          // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2222]",ClassName());
          ch2222();
          break;
        case 3:
          // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2223]",ClassName());
          ch2223();
          break;
        default:
          // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2224]",ClassName());
          ch2224();
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 2 | fT1 : 2 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2231]",ClassName());
          ch2231();
          break;
        case 2:
          // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2232]",ClassName());
          ch2232();
          break;
        case 3:
          // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 3
          ch2233();
          break;
        default:
          // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : N
          ch2233();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 2 | fT1 : 2 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2241]",ClassName());
          ch2241();
          break;
        case 2:
          // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2242]",ClassName());
          ch2242();
          break;
        case 3:
          // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 3
          ch2233();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : N
          ch2244();
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    default:
      // fR1 : 2 | fT1 : 3
      switch (fR2->fPhases) {
      case 0:
      case 1:
        // fR1 : 2 | fT1 : 3 | fR2 : 1
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2311]",ClassName());
          ch2311();
          break;
        case 2:
          // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2312]",ClassName());
          ch2312();
          break;
        case 3:
          // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 3
        default:
          // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2313]",ClassName());
          ch2313();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      case 2:
        // fR1 : 2 | fT1 : 3 | fR2 : 2
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 1
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2321]",ClassName());
          ch2321();
          break;
        case 2:
          // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 2
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2322]",ClassName());
          ch2322();
          break;
        case 3:
          // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 3
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2323]",ClassName());
          ch2323();
          break;
        default:
          // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : N
          PrintWhich();
          gCleanOut->MM(error,met,"Should not happen [2324]",ClassName());
          ch2323();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      case 3:
        // fR1 : 2 | fT1 : 3 | fR2 : 3
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 1
          ch2331();
          break;
        case 2:
          // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 2
          ch2332();
          break;
        case 3:
          // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 3
          ch2333();
          break;
        default:
          // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : N
          ch2333();
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      default:
        // fR1 : 2 | fT1 : 3 | fR2 : N
        switch (fT2->fPhases) {
        case 0:
        case 1:
          // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 1
          ch2331();
          fR2->fShare = NotInvolved;
          break;
        case 2:
          // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 2
          ch2332();
          fR2->fShare = NotInvolved;
          break;
        case 3:
          // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 3
          ch2333();
          fR2->fShare = NotInvolved;
          break;
        default:
          // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : N
          ch2333();
          fR2->fShare = NotInvolved;
          fT2->fShare = NotInvolved;
          break;
        }//end switch (fT2->fPhases)
        break;
      }//end switch (fR2->fPhases)
      break;
    }//end switch (fT1->fPhases)
    break;
  default:
    // fR1 : 3
    gCleanOut->MM(error,met,"1st reflected wave absorbed",ClassName());
    break;
  }//end switch (fR1->fPhases)
}//end Choose
void TLitPhoton::ClearNewVolume(Bool_t wcs) {
  //  The possible new TgeoVolume has not been entered by the photon. We clear it
  // If wcs is true, call TLitVolume::ClearWCSdone().
  fNextNode      = 0;
  fNextGeoVolume = 0;
  fNextPath      = "";
  if (fNextLitVolume && wcs) fNextLitVolume->ClearWCSdone();
  fNextLitVolume = 0;
  fNextLitDet    = 0;
  fNextLitDetG   = 0;
  fNextGeoMedium = 0;
  fNextLitMedium = 0;
}
void TLitPhoton::ClearVolume() {
  // The possible new TgeoVolume has not been entered by the photon. We clear it
  fNode      = 0;
  fGeoVolume = 0;
  fPath      = "";
  if (fLitVolume) fLitVolume->ClearWCSdone();
  fLitVolume = 0;
  fLitDet    = 0;
  fLitDetG   = 0;
  fGeoMedium = 0;
  fLitMedium = 0;
}
TComplex TLitPhoton::CosFromSin(TComplex s) {
//  Given a complex number representing the sinus of a complex angle, returns
// the complex value of the cosinus of the same complex angle. The value chosen
// is the one with a positive real part, if the imaginary part is 0.
//  If the imaginary part is non-zero, the value chosen is the one with a
// negative imaginary part. THE IMAGINARY PART OF THE RESULT IS ALWAYS CHOSEN
// NEGATIVE IF IT IS NON-ZERO. This is an arbitrary choice motivated by the
// fact that in SLitrani, the phases of waves have to be negative when a wave
// is absorbed, never positive which would correspond to an unphysical explosion
// of the wave.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t vsmall = 1.0e-12;
  TComplex z;
  z = TComplex::Sqrt(un - s*s);
  if (z.Im()>vsmall) z = zero - z;
  if (TMath::Abs(z.Im())<=vsmall) z(z.Re(),zero);
  if (TMath::Abs(z.Re())<=vsmall) z(zero,z.Im());
  return z;
}
void TLitPhoton::Depolish() {
  //  Tilt the normal to the surface to simulate a depolish. Implement grinding
  // of surface by tilting arbitrarily the normal to the surface. The tilt is
  // isotropic in space (sin(t) dt df) with t between 0 and fLitMedium->GetDPLTheta().
  // We need to build a coordinate system for which the Oz axis is the normal
  // to the face. The Ox and Oy axis are irrelevant, provided Ox,Oy,Oz is a right
  // orthonormal system. We generate the Ox axis at random and then make it orthogonal
  // to Oz.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t small  = 1.0e-6;
  const Double_t vsmall = 1.0e-8;
  const Int_t nmax = 10000;
  const char *met = "Depolish";
  Double_t y,sti,cti,stt,ctt,eps;
  Double_t tgr,stgr,ctgr,fgr,sfgr,cfgr,ctgrM;
  TVector3 an1,an2,an3;
  TVector3 e2;
  if (TMath::Abs(fRfIndext.Im())>vsmall) {
    PrintWhich();
    gCleanOut->MM(fatal,met,"No grinding in front of a complex index of refraction",ClassName());
    fAnomaly = kTRUE;
    return;
  }
  else fRfIndext(fRfIndext.Re(),0.0);
  an3  = fFN;
  do {
    y    = gRandom3->Rndm();
    ctgr = un - y;
    stgr = TMath::Sqrt(un - ctgr*ctgr);
    y    = gRandom3->Rndm();
    fgr  = deux*TMath::Pi()*y;
    sfgr = TMath::Sin(fgr);
    cfgr = TMath::Cos(fgr);
    an1.SetXYZ(stgr*cfgr,stgr*sfgr,ctgr);
    an1 = an3.Cross(an1);
    eps = an1*an1;
  } while (eps<vsmall);
  an1 = an1.Unit();
  an2 = an3^an1;
  an2 = an2.Unit();
  ctgrM = fLitMedium->GetDPLCosTheta();
  Bool_t ok = kFALSE;
  Int_t n   = 0;
  while ((!ok) && (n<nmax)) {
    n   += 1;
    y    = gRandom3->Rndm();
    tgr  = TLitPhys::ACos(un - y*(un-ctgrM));
    stgr = TMath::Sin(tgr);
    ctgr = TMath::Cos(tgr);
    y    = gRandom3->Rndm();
    fgr  = deux*TMath::Pi()*y;
    sfgr = TMath::Sin(fgr);
    cfgr = TMath::Cos(fgr);
    fFNd = stgr*cfgr*an1 + stgr*sfgr*an2 + ctgr*an3;
    //Check that Poyinting vector of photon and new normal not opposite !
    cti = fUpg*fFNd;
    ok = (cti>zero);
    if (ok) {
      //In case of total reflection, photon must go inside polyhedron !
      e2 = fUpg - deux*cti*fFNd;
      y  = e2*an3;
      ok = (y<-small);
      if (ok) {
        //Now check that refractive wave will not be backward
        sti = TMath::Sqrt(un-cti*cti);
        stt = (fRfIndexi.Re()*sti)/fRfIndext.Re();
        if (stt<un) {
          ctt = TMath::Sqrt(un-stt*stt);
          e2  = stt*fUpg + (sti*ctt-cti*stt)*fFNd;
          e2  = e2.Unit();
          y   = e2*an3;
          ok  = (y>zero);
        }// end if (stt<un)
      }// end if (ok) 
    }// end if (ok) 
  }// end while ((!ok) && (n<nmax))
  if (n>=nmax) {
    PrintWhich();
    gCleanOut->MM(killthis,met,"Anomaly in generation of grinding",ClassName());
    fAnomaly = kTRUE;
  }
}
Bool_t TLitPhoton::Detector() {
  //  Finds the current detectors, global and per run, if they exist. If not
  // create them! By current, we mean those connected with fLitVolume and
  // fPath
  Bool_t ok1 = kFALSE;
  Bool_t ok2 = kFALSE;
  fLitDet    = 0;
  fLitDetG   = 0;
  if (fLitVolume) {
    // Per run detector
    if (gLitCs) {
      if (TLitDetector::fgLastUsedDet) {
        if (TLitDetector::fgLastUsedDet->IsMe(fPath)) {
          fLitDet = TLitDetector::fgLastUsedDet;
          ok1     = kTRUE;
        }
      }
      if (!ok1) {
        fLitDet = TLitDetector::FindDetector(fPath,kFALSE);
        if (!fLitDet) {
          fLitDet = new TLitDetector(fPath.Data(),fLitVolume->GetDetKind(),kFALSE,
            fLitVolume->GetIsQE(),fLitVolume->GetStopAlways(),(fLitVolume->GetEntrance()>=0),
            fLitVolume->GetEx(),fLitVolume->GetEy(),fLitVolume->GetEz());
        }
        TLitDetector::fgLastUsedDet = fLitDet;
        ok1          = kTRUE;
      }//end if (!ok1)
    }//end if (gLitCs)
    else ok1 = kTRUE;
    // Global detector
    TString s = 'G';
    s.Append(fPath);
    if (TLitDetector::fgLastUsedDetG) {
      if (TLitDetector::fgLastUsedDetG->IsMe(s)) {
        fLitDetG = TLitDetector::fgLastUsedDetG;
        ok2      = kTRUE;
      }
    }
    if (!ok2) {
      fLitDetG = TLitDetector::FindDetector(s,kTRUE);
      if (!fLitDetG) {
        fLitDetG = new TLitDetector(s.Data(),fLitVolume->GetDetKind(),kTRUE,
          fLitVolume->GetIsQE(),fLitVolume->GetStopAlways(),(fLitVolume->GetEntrance()>=0),
          fLitVolume->GetEx(),fLitVolume->GetEy(),fLitVolume->GetEz());
      }
      TLitDetector::fgLastUsedDetG = fLitDetG;
      ok2           = kTRUE;
    }//end if (!ok2)
  }//end if (fLitVolume)
  return ok1 && ok2;
}
Bool_t TLitPhoton::DetectorN() {
  //  Finds the next detectors, global and per run, if they exist. If not
  // create them! By next, we mean those connected with fNextLitVolume and
  // fNextPath
  Bool_t ok1   = kFALSE;
  Bool_t ok2   = kFALSE;
  fNextLitDet  = 0;
  fNextLitDetG = 0;
  if (fNextLitVolume) {
    // Per run detector
    if (gLitCs) {
      if (TLitDetector::fgLastUsedDet) {
        if (TLitDetector::fgLastUsedDet->IsMe(fNextPath)) {
          fNextLitDet = TLitDetector::fgLastUsedDet;
          ok1     = kTRUE;
        }
      }
      if (!ok1) {
        fNextLitDet = TLitDetector::FindDetector(fNextPath,kFALSE);
        if (!fNextLitDet) {
          fNextLitDet = new TLitDetector(fNextPath.Data(),fNextLitVolume->GetDetKind(),kFALSE,
            fNextLitVolume->GetIsQE(),fNextLitVolume->GetStopAlways(),(fNextLitVolume->GetEntrance()>=0),
            fNextLitVolume->GetEx(),fNextLitVolume->GetEy(),fNextLitVolume->GetEz());
        }
        TLitDetector::fgLastUsedDet = fNextLitDet;
        ok1          = kTRUE;
      }//end if (!ok1)
    }//end if (gLitCs)
    else ok1 = kTRUE;
    // Global detector
    TString s = 'G';
    s.Append(fNextPath);
    if (TLitDetector::fgLastUsedDetG) {
      if (TLitDetector::fgLastUsedDetG->IsMe(s)) {
        fNextLitDetG = TLitDetector::fgLastUsedDetG;
        ok2          = kTRUE;
      }
    }
    if (!ok2) {
      fNextLitDetG = TLitDetector::FindDetector(s,kTRUE);
      if (!fNextLitDetG) {
        fNextLitDetG = new TLitDetector(s.Data(),fNextLitVolume->GetDetKind(),kTRUE,
          fNextLitVolume->GetIsQE(),fNextLitVolume->GetStopAlways(),(fNextLitVolume->GetEntrance()>=0),
          fNextLitVolume->GetEx(),fNextLitVolume->GetEy(),fNextLitVolume->GetEz());
      }
      TLitDetector::fgLastUsedDetG = fNextLitDetG;
      ok2           = kTRUE;
    }//end if (!ok2)
  }//end if (fNextLitVolume)
  return ok1 && ok2;
}
void TLitPhoton::Diffused() {
  //  Handles the case of a photon in front of a wrapping which diffuses the
  // photon. Look at the class description of TLitMedium to find a full
  // description of characteristics of wrappings.
  //
  //    HOW DIFFUSION IS GENERATED
  //
  //  In SLitrani, diffusion is treated exactly in the same way as reflection,
  // except that the reflection is done on a plane of "any" orientation. More
  // precisely, the orientation of the reflecting plane is chosen in such a way
  // that the reflected photon has a distribution of reemission which is :
  //
  //       cos(t)sin(t) dt df     [ 0 <= t <= ThetaMax ]
  //
  //  There is a supplementary factor cos(t) with respect to a totally isotropic
  // distribution sin(t) dt df in order to slightly favour the direction normal
  // to the diffusing surface. By default ThetaMax is 90 degree. ThetaMax is
  // given in the constructor of TLitMedium when defining a wrapping. You cannot
  // choose it bigger than 90 degree, it would be meaningless. You can choose
  // it smaller if you want to avoid that the diffused photon be almost tangent
  // to the plane of the diffusing wrapping. There is a good reason to choose
  // it smaller than 90 degree in case of a diffusing wrapping covering, wi-
  // thout any slice of intermediate material, a face of a TGeoVolume made of an
  // anisotropic material : see tricky point c, paragraph "Tricky points about
  // wrappings" in the documentation of TLitMedium.
  //  Once one knows what the direction of the diffused photon will be, one
  // calculates the orientation of a pseudo plane of reflection in such a way
  // that the photon REFLECTED on this pseudo plane of reflection has the
  // wanted direction for the DIFFUSED photon. One does that assuming that the
  // angle of reflection is equal to the incident angle, which is true only for
  // isotropic material. This is the origin of tricky point c) quoted above.
  //  We first construct a local coordinate system into which we intend
  // to gener the direction of the diffused photon.
  // "In" and "Out" characteristics of medium must be defined before calling
  // Diffused()
  const Double_t zero  =  0.0;
  const Double_t un    =  1.0;
  const Double_t minus = -1.0;
  const Double_t deux  =  2.0;
  const Double_t dixm6 =  1.0e-6;
  Int_t bin;
  Double_t y,a,b,c,thtdf,sthtdf,cthtdf,phidf,sphidf,cphidf;
  TVector3 an1,an2,an3;
  // We build an orthonormal system with Oz axis opposite to normal
  an3 = minus*fFN;
  a = an3(0);  b = an3(1);
  c = a*a+b*b;
  if (c>dixm6) {
    an1(0) =  an3(1);
    an1(1) = -an3(0);
    an1(2) =  zero;
    an1    = an1.Unit();
    an2    = an3.Cross(an1);
  }
  else {
    an1(0) =  an3(2);
    an1(1) =  zero;
    an1(2) = -an3(0);
    an1    = an1.Unit();
    an2    = an3.Cross(an1);
  }
  //  Now we generate a vector which is anywhere on the hemisphere around
  // the direction of an3. It makes an angle theta=thtdf < PI/2 with an3
  // and its projection onto the plane (an1,an2) makes an angle phi = phidf
  // with an1. This vector will be taken as the direction of the diffused
  // photon.
  y      = gRandom3->Rndm();
  phidf  = deux*TMath::Pi()*y;
  sphidf = TMath::Sin(phidf);
  cphidf = TMath::Cos(phidf);
  y      = gRandom3->Rndm();
  //
  //  This WOULD BE the code for an isotropic distribution of diffusion,i.e. a
  // distribution of sin(thtdf)dthtdfdphidf :
  //
  //    thtdf  = TLitPhys::ACos(1-y)
  //
  //  But in reality the distribution for diffusion is better described if we
  // favour the direction normal to the plane of the diffusing medium by a
  // factor cos(thtdf), i.e. if we take the distribution
  //
  //    cos(thtdf)sin(thtdf)dthtdfdphidf   [0<=thtdf<=ThetaMax]
  //
  //  Look at the documentation about TLitMedium to have a complete description
  // of diffusion.
  //
  thtdf  = TLitPhys::ASin(fSinThetaMax*TMath::Sqrt(y));
  sthtdf = TMath::Sin(thtdf);
  cthtdf = TMath::Cos(thtdf);
  a      = sthtdf*cphidf;
  b      = sthtdf*sphidf;
  c      = cthtdf;
  TVector3 tupg;
  //tupg is the direction of the diffused photon in WCS.
  tupg   = a*an1 + b*an2 + c*an3;
  //we find the normal to the pseudo reflecting plane, which will give
  //the reflected photon at tupg !
  a      = fUpg*tupg;
  b      = un/TMath::Sqrt(deux*(un-a));
  fOx    = b*(fUpg - tupg);
  Transition();
  if (!fAnomaly && fPhtAbs) {
    //Photon has been absorbed by the wrapping
    fPhtLive = kFALSE;
    gLitGs->fNpLossAny += 1;
    gLitGs->fNpAbsRvt  += 1;
    Double_t arev1 = fNextLitMedium->GetRevCounterD();
    bin = gLitGs->fHAbsAny->Fill(fSl0);
    bin = gLitGs->fHAbsRvt->Fill(arev1);
    if (gLitCs) {
      gLitCs->fNpLossAny += 1;
      gLitCs->fNpAbsRvt  += 1;
      bin = gLitCs->fHAbsAny->Fill(fSl0);
      bin = gLitCs->fHAbsRvt->Fill(arev1);
    }
  }// end if (!fAnomaly && fPhtAbs)
}
void TLitPhoton::ExitFace() {
  //  Handles photon between face and wrapping
  const Int_t nmaxloop = 10000; //doing following loop more than nmaxloop times
                                // is an anomaly
  Int_t nloop = 0;
  while (!fAnomaly && fOutPoly && fPhtLive) {
    SliceToRev();
    if (!fAnomaly && fPhtLive) {
      SliceToShape();
    }
    nloop++;
    if (nloop>nmaxloop) {
      fAnomaly = kTRUE;
      gCleanOut->MMI(killthis,"ExitFace","nloop too big","nloop",nloop,ClassName());
    }
  }
}
void TLitPhoton::FindAbsLength() {
  //  Finds the complex index of refraction, including absorption, in the case
  //of anisotropic absorption.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  Double_t f1,f2,f3,f1s,f2s,f3s,f1m,f2m,f3m,f12,f13,f23;
  TComplex g11,g12,g13,g22,g23,g33;
  TComplex g1,g2,g3,g4,g5,g6;
  TComplex a0,a1,a2,dd;
  f1  = fUk0(0);
  f1s = f1*f1;
  f2  = fUk0(1);
  f2s = f2*f2;
  f3  = fUk0(2);
  f3s = f3*f3;
  f1m = un - f1s;
  f2m = un - f2s;
  f3m = un - f3s;
  f12 = f1*f2;
  f13 = f1*f3;
  f23 = f2*f3;
  g11 = fCDzt0(0,0);
  g12 = fCDzt0(0,1);
  g13 = fCDzt0(0,2);
  g22 = fCDzt0(1,1);
  g23 = fCDzt0(1,2);
  g33 = fCDzt0(2,2);
  g1  = g22*g33 - g23*g23;
  g2  = g11*g33 - g13*g13;
  g3  = g11*g22 - g12*g12;
  g4  = g12*g33 - g23*g13;
  g5  = g13*g22 - g12*g23;
  g6  = g23*g11 - g12*g13;
  a0  = g22*g13*g13-deux*g12*g13*g23+g11*g23*g23+g33*g12*g12-g11*g22*g33;
  a1  = f1m*g1+f2m*g2+f3m*g3+deux*(f12*g4+f13*g5+f23*g6);
  a2  = -f1s*g11 -f2s*g22 -f3s*g33 -deux*(f12*g12 + f13*g13 + f23*g23);
  dd  = TComplex::Sqrt(a1*a1-quatre*a2*a0);
  g3  = deux*a2;
  g1  = TComplex::Sqrt((zero -a1 - dd)/g3);
  g2  = TComplex::Sqrt((zero -a1 + dd)/g3);
  f1  = g1.Re();
  f1s = g1.Im();
  if (f1<zero) {
    f1  = - f1;
    f1s = - f1s;
  }
  f2  = g2.Re();
  f2s = g2.Im();
  if (f2<zero) {
    f2  = - f2;
    f2s = - f2s;
  }
  f1m = TMath::Abs(fRfIndex0.Re() - f1);
  f2m = TMath::Abs(fRfIndex0.Re() - f2);
  if (f2m<f1m) {
    f1  = f2;
    f1s = f2s;
  }
  //f1s must be negativ
  if (f1s>zero) {
    f1s = - f1s;
    gCleanOut->MM(killthis,"FindAbsLength","absorption length is negativ",ClassName());
    fAnomaly = kTRUE;
  }
  f1s = - f1s;
  fAbsl0 = TLitPhys::Get()->C()/(deux*fW*f1s);
}
void TLitPhoton::FindDielAni() {
  //  Find dielectric properties and E field in case of anisotropic material
  // uz0 is the unit vector to which the E field has to be orthogonal
  // In anisotropic material, uz0 is NOT parallel to k vector.
  // fUx0, fUy0, uz0 build a right orthonormal system
  // fUx0 is taken horizontal, OBDA (ohne Beschränkung der Allgemeinheit !)
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t z05    = 0.5;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-7;
  const Double_t vsmall = 1.0e-300;
  const char *met = "FindDielAni";
  Double_t f1,f2,f3,f1s,f2s,f3s;
  Double_t a0,a1,a2,dd;
  Double_t g1,g2,g3,g4,g5,g6;
  Double_t g11,g12,g13,g22,g23,g33;
  Short_t NbSol = 0;
  f1  = fUk0(0);
  f1s = f1*f1;
  f2  = fUk0(1);
  f2s = f2*f2;
  f3  = fUk0(2);
  f3s = f3*f3;
  g11 = fDzt0(0,0);
  g12 = fDzt0(0,1);
  g13 = fDzt0(0,2);
  g22 = fDzt0(1,1);
  g23 = fDzt0(1,2);
  g33 = fDzt0(2,2);
  g1  = g22*g33 - g23*g23;
  g2  = g11*g33 - g13*g13;
  g3  = g11*g22 - g12*g12;
  g4  = g12*g33 - g23*g13;
  g5  = g13*g22 - g12*g23;
  g6  = g23*g11 - g12*g13;
  a0  = g22*g13*g13-deux*g12*g13*g23+g11*g23*g23+g33*g12*g12-g11*g22*g33;
  a1  = (un-f1s)*g1+(un-f2s)*g2+(un-f3s)*g3+deux*(f1*f2*g4+f1*f3*g5+f2*f3*g6);
  a2 = -f1s*g11 -f2s*g22 -f3s*g33 -deux*(f1*f2*g12 + f1*f3*g13 + f2*f3*g23);
  dd = a1*a1-quatre*a2*a0;
  if ((dd < zero) && (dd > -small)) dd = zero;
  if (dd<zero) {
    PrintWhich();
    gCleanOut->MM(killthis,met,"Complex indices",ClassName());
    fAnomaly = kTRUE;
    return;
  }
  dd = TMath::Sqrt(dd);
  //g1 and g2 : the 2 acceptable index of refraction ^2
  g3 = deux*a2;
  if (TMath::Abs(g3)<vsmall) {
    PrintWhich();
    gCleanOut->MM(killthis,met,"Indices are infinite",ClassName());
    fAnomaly = kTRUE;
    return;
  }
  g1 = (-a1 - dd)/g3;
  g2 = (-a1 + dd)/g3;
  if ((g1<=zero) && (g2<=zero)) {
    PrintWhich();
    gCleanOut->MM(killthis,met,"Both index imaginary",ClassName());
    fAnomaly = kTRUE;
    return;
  }
  if (g1<=zero) {
    PrintWhich();
    gCleanOut->MM(warning,met,"1st index imaginary",ClassName());
    g1 = g2;
  }
  else {
    NbSol += 1;
    g1 = TMath::Sqrt(g1);
  }
  if (g2<=zero) {
    PrintWhich();
    gCleanOut->MM(warning,met,"2nd index imaginary",ClassName());
  }
  else {
    NbSol += 1;
    g2 = TMath::Sqrt(g2);
  }
  if (NbSol>=2) {
    switch (fChooseIndex) {
    case 1:
      if (g2>g1) g1=g2;
      break;
    case 2:
      if (g2<g1) g1=g2;
      break;
    default:
      //We toss a coin to choose between the 2 indices
      g3 = gRandom3->Rndm();
      if (g3<z05) g1=g2;
      break;
    }
  }
  fRfIndex0 = g1;
  fRfIndexi = g1;
  fI->fRfI  = g1;
  fDiel0    = (g1*g1)/fPmu0;
  fI->fDiel = fDiel0;
  //We can calculate fAk0 ( module of wave vector ) in [ cm-1 ]
  fAk0 = (fW/TLitPhys::Get()->C())*g1;
  //and the k vector itself :
  fVk0 = fAk0*fUk0;
  g3    = g1*g1;
  //We verify that the determinant is really 0 with g1
  TMat33 aam;
  aam(0,0) = g3*(f2s+f3s) - g11;
  aam(0,1) = - ( g3*f1*f2 + g12 );
  aam(0,2) = - ( g3*f1*f3 + g13 );
  aam(1,0) = aam(0,1);
  aam(1,1) = g3*(f1s+f3s) - g22;
  aam(1,2) = - ( g3*f2*f3 + g23 );
  aam(2,0) = aam(0,2);
  aam(2,1) = aam(1,2);
  aam(2,2) = g3*(f1s+f2s) - g33;
  //Now we find the direction of the electric field.
  Short_t deg;
  TVector3 Ef2;
  deg = aam.DegMatrix(fEf0,Ef2,TLit::Get()->GetTestCalc());
//  deg = fEf0.DegMatrix(aam,Ef2,TLit::Get()->GetTestCalc());
  switch (deg) {
  case 0:
    PrintWhich();
    gCleanOut->MM(killthis,met,"Determinant non zero",ClassName());
    fAnomaly = kTRUE;
    return;
  case 2:
    PrintWhich();
    gCleanOut->MM(killthis,met,"Matrix twice degenerate",ClassName());
    fAnomaly = kTRUE;
    break;
  case 3:
    PrintWhich();
    gCleanOut->MM(killthis,met,"Matrix is 0",ClassName());
    fAnomaly = kTRUE;
    return;
  default:
    break;
  }
  //We toss a coin to decide of the direction of the E field
  g3 = gRandom3->Rndm();
  if (g3<z05) fEf0 = -fEf0;
  //Verify that uz0 is orthogonal to E field
  if (TLit::Get()->GetTestCalc()) {
    TVector3 uz0;
    uz0 = fUk0*fDzt0;
    uz0 = uz0.Unit();
    g1  = TMath::Abs(fEf0*uz0);
    if (g1>small) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Bad direction for E field","Instead of 0",g1,ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
  //Calculates energy density
  Double_t u;
  u = fEf0*(fDzt0*fEf0);
  u = (u*TLitPhys::Get()->Eps0())/fPmu0;
  fAef0 = TMath::Sqrt(fEphot/(u*TLitPhys::Get()->Vphot()));
  //E field
  fEf0  = fAef0*fEf0;
  if (fAnia0) FindAbsLength();
  else        fAbsl0 = fLitMedium->GetAbsL();
}
void TLitPhoton::FindDielIso() {
  // Find dielectric properties and E field in case of isotropic material
  Double_t x;
  fRfIndex0 = fLitMedium->GetIndex();
  x         = fRfIndex0.Re();
  //Dielectric constant
  fDiel0    = (x*x)/fPmu0;
  fI->fDiel = fDiel0;
  //Module of electric field. TLitPhys::Get()->fVphot is an arbitrary volume (we have taken
  //1 mm3) in which the energy of the total energy of the photon is hw, i.e. we
  //consider arbitrarily that the momentan extension of the photon is 1mm3.
  fAef0     = TMath::Sqrt((fEphot/TLitPhys::Get()->Eps0())/(TLitPhys::Get()->Vphot()*fDiel0));
  //fAk0 ( module of wave vector ) in [ cm-1 ]
  fAk0      = (fW/TLitPhys::Get()->C())*x;
  //k vector
  fVk0      = fAk0*fUk0;
  //E field
  fEf0      = fAef0*fEf0;
}
void TLitPhoton::FindProcessAndLength() {
  // Finds which process is to occur for this step. Is called by Jump()
  //returns the distance to travel before the chosen process fProcess occurs
  const Double_t verysmall = 1.0e-50;
  const Bool_t   newmethod = kTRUE;
  TVector3 x10 = fNextDist*fUpg;
  Double_t yDistTest = 0.0; // Test distance to see if process occurs before photon reaches boundary
  Double_t yAbs,pstopAbs;
  if (fIsAbsorb) {
    if (fAbsl0<verysmall) {
      fProcess = kAbsorption;
      return;
    }
    if (!IsDamage()) {
      yAbs      = gRandom3->Rndm();
      yDistTest = -fAbsl0*TMath::Log(1.0 - yAbs);
    }
    else {
      //
      // Position-dependent absorption length
      //
      TVector3 damxstart; //start of step idam
      TVector3 damxstop;  //stop  of step idam
      TVector3 damx;      //point where absorption length calculated.
      Int_t ndamstep;     //number of steps
      Int_t idam;         //which step
      Int_t idamm1;       //idam - 1
      Double_t damstep;   //length of one step in cm
      Double_t damabsli;  //absorption length for step idam;
      Double_t damstart;  //u coord. of start of step idam
      Double_t damstop;   //u coord. of stop  of step idam
      Double_t daml;      //length of step idam
      Double_t damylabs;  //last path before absorption
      Bool_t absorbed = kFALSE;
      damstep = fLitVolume->GetDamStep();
      ndamstep = Int_t(fNextDist/damstep + 1);
      x10 = x10.Unit();
      damstart = 0.0;
      idam = 1;
      while ((!absorbed) && (idam<=ndamstep)) {
        idamm1    = idam - 1;
        damstart  = idamm1*damstep;
        damxstart = fX0 + damstart*x10;
        if (idam<ndamstep) {
          damstop  = idam*damstep;
          damxstop = fX0 + idam*damstep*x10;
        }
        else {
          damstop  = fNextDist;
          damxstop = fNextPoint;
        }
        damx     = 0.5*(damxstart + damxstop);
        damabsli = fAbsl0*fLitVolume->GetDamage(&damx,fAbsl0);
        daml     = damstop - damstart;
        if (newmethod) {
          damylabs = gRandom3->Rndm();
          damylabs = -damabsli*TMath::Log(1.0 - damylabs);
          absorbed = damylabs<daml;
          if (absorbed) yDistTest += damylabs;
          else          yDistTest += daml;
        }//end if (newmethod)
        else {
          pstopAbs = 1 - TMath::Exp(-daml/damabsli);
          yAbs     = gRandom3->Rndm();
          absorbed = yAbs<pstopAbs;
          if (absorbed) {
            damylabs = gRandom3->Rndm();
            damylabs = -fAbsl0*TMath::Log(1.0-damylabs*pstopAbs);
            yDistTest   += damylabs;
          }
          else yDistTest   += daml;
        }//end else if (newmethod)
        idam++;
      } //end while ((!absorbed) && (idam<=ndamstep))
    } //end else (!fLitVolume->IsDamage())
    if (yDistTest<fNextDist){
      fNextDist     = yDistTest;
      fIsOnBoundary = kFALSE;
      fProcess      = kAbsorption;
    } 
  } // end if (fIsAbsorb)
  if (fIsDiffus) {
    yDistTest = -fDiffusL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<fNextDist) {
      fNextDist     = yDistTest;
      fIsOnBoundary = kFALSE;
      fProcess      = kIsotropicScattering; // Jump() will be recalled
    } 
  }
  if (fIsRayleigh) {
    yDistTest = -fRayleighL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<fNextDist) {
      fNextDist     = yDistTest;
      fIsOnBoundary = kFALSE;
      fProcess      = kRayleighScattering; // Jump() will be recalled
    }
  } 
  if (fIsWLS) {
    yDistTest = -fWLSL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<fNextDist) {
      fNextDist     = yDistTest;
      fIsOnBoundary = kFALSE;
      fProcess      = kWLS; // Jump() will be recalled
    }
  }
}
void TLitPhoton::GetImin12(Short_t k,Short_t &i1,Short_t &i2,Short_t ki) {
  //  Finds the k th pair of solution. ki is the solution of the incoming
  // wave (if call from anitoiso) which should be tried last.
  Short_t k0,k1,k2,k3;
  k -= 1;
  k %= 6;
  if (ki<0) {
    //no incoming wave to avoid
    k0 = 0; k1 = 1; k2 = 2; k3 = 3;
  }
  else {
    //incoming wave to avoid
    k0 = -1; k1 = -1; k2 = -1; k3 = -1;
    for (Short_t i = 0; i<4; i++) {
      if (i != ki ) {
        if  (k0 == -1) k0 = i;
        if ((k1 == -1) && (k0 != i)) k1 = i;
        if ((k2 == -1) && (k0 != i) && (k1 != i)) k2 = i;
      }
    }
    k3 = ki;
  }
  switch (k) {
  case 0:
    i1 = k0; i2 = k1;
    break;
  case 1:
    i1 = k0; i2 = k2;
    break;
  case 2:
    i1 = k1; i2 = k2;
    break;
  case 3:
    i1 = k0; i2 = k3;
    break;
  case 4:
    i1 = k1; i2 = k3;
    break;
  case 5:
    i1 = k2; i2 = k3;
    break;
  }
}
void TLitPhoton::ICStoWCS() {
  //  Returns from the coordinate system of the incident plane ICS into the
  // world coordinate system WCS and defines there completly the photon
  // Update the direction of the track in TGeo
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-6;
  const char *met = "ICStoWCS";
  Short_t anisnew;
  Double_t d,pmunew;
  fUk0 = fMn*fI->fUk;
  fVk0 = fMn*fI->fVk;
  fEf0 = fMn*fI->fEf;
  fUk0 = fUk0.Unit();
  if (fPhtOut) {
    anisnew = fAnist;
    pmunew  = fPmut;
  }
  else {
    anisnew = fAnisi;
    pmunew  = fPmui;
  }
  //Refraction index for a photon of wave vector UK0.
  fI->fRfI  = (fAk0*TLitPhys::Get()->C())/fW;
  fRfIndexi = fI->fRfI;
  fPmui     = pmunew;
  //Dielectric constant for a photon of wave vector UK0
  d         = fI->fRfI.Re();
  fI->fDiel = (d*d)/pmunew;
  Double_t s,eu;
  TVector3 ee(fEf0);
  ee = ee.Unit();
  s  = fUk0*ee;
  //In isotropic case, check the orthogonality between k vector and E field,
  //and, if not perfect due to finite machine precision make the smallest
  //possible correction to ee so that it is perfectly orthogonal to UK0 and
  //remains of length 1.
  if ((!anisnew) && (TMath::Abs(s)>small)) {
    gCleanOut->MMR(warning,met,"k and E not orthogonal","scalar P",s,ClassName());
    d  = un/TMath::Sqrt(un - s*s);
    ee = d*(ee - s*fUk0);
  }
  eu = un - s;
  //fAef0 : module of renormalized E field ( we increase the module of the
  //electric field to this value in order that the energy of the photon after
  //the process is the same as the energy of the photon before the process ).
  //The following calculation is valid in both isotropic and anisotropic case.
  //In isotropic case : eu = 1.
  fAef0 = (fEphot/TLitPhys::Get()->Eps0())/(TLitPhys::Get()->Vphot()*fI->fDiel*eu);
  fEf0  = fAef0*ee;
  BuildBPUP(fVk0,fEf0,fBf0,fPtg,fUpg,pmunew);
  fCospk = fUk0*fUpg;
  if (fAnia0) FindAbsLength();
}
void TLitPhoton::InDetRecord() {
  // Photon has died inside a detector or has been detected. It has to be
  //registered. All calculations in local coordinate system of detector TGeoVolume
  const Double_t eps   = 1.0e-12;
  const Double_t big   = 1.0e+12;
  const Double_t dixp7 = 1.0e+7;
  Int_t bin;
  Double_t dx,dy,dz;
  Double_t t,x0,y0,z0,nx,ny,nz,xp,yp;
  Double_t DepthInDet; //Depth of penetration inside detector with respect to TLitVolume::fDetAxis
  Double_t NbOfPairs;  //Nb of e+e- pairs produced in APD
  Short_t k;
  Double_t loc_pt[3], loc_dir[3];
  const Double_t *glob_pt;
  const Double_t *glob_dir;
  TVector3 StopPoint,StopDir;
  fPhtLive = kFALSE;
  Bool_t seen = kTRUE;
  // Jump in local coord system
  glob_pt = gGeoManager->GetCurrentPoint();
  glob_dir = gGeoManager->GetCurrentDirection();
  gGeoManager->MasterToLocal(glob_pt,loc_pt);
  for (k=0;k<3;k++) StopPoint[k] = loc_pt[k];
  gGeoManager->MasterToLocalVect(glob_dir,loc_dir);
  for (k=0;k<3;k++) StopDir[k] = loc_dir[k];
  //
  DepthInDet   = 0.0;
  xp = big; yp = big;
  if ((fLitVolume->GetStopAlways()) && (fLitVolume->GetEntrance()>=0)) {
    // Here we are on the entrance face
    x0 = StopPoint.X();
    y0 = StopPoint.Y();
    z0 = StopPoint.Z();
    switch (fLitVolume->GetEntrance()) {
      case 0:
        // Face at -dy. (xp,yp) are (x,z)
        xp = x0;  yp = z0;
        break;
      case 1:
        // Face at +dx. (xp,yp) are (y,z)
        xp = y0;  yp = z0;
        break;
      case 2:
        // Face at +dy.  (xp,yp) are (x,z)
        xp = x0;  yp = z0;
        break;
      case 3:
        // Face at -dx. (xp,yp) are (y,z)
        xp = y0;  yp = z0;
        break;
      case 4:
        // Face at -dz.  (xp,yp) are (x,y)
        xp = x0;  yp = y0;
        break;
      case 5:
        // Face at +dz.  (xp,yp) are (x,y)
        xp = x0;  yp = y0;
        break;
      default:
        break;
    }
  }
  if ((!fLitVolume->GetStopAlways()) && (fLitVolume->GetEntrance()>=0)) {
    //Here we are inside the volume
    //Calculates depth with respect to fDetAxis
    DepthInDet = fLengthInDet*(StopDir*fLitVolume->GetDetAxis());
    //Records projection with respect to TLitVolume::fDetAxis on entrance face
    dx = ((TGeoBBox*)fGeoVolume->GetShape())->GetDX();
    dy = ((TGeoBBox*)fGeoVolume->GetShape())->GetDY();
    dz = ((TGeoBBox*)fGeoVolume->GetShape())->GetDZ();
    x0 = StopPoint.X();
    y0 = StopPoint.Y();
    z0 = StopPoint.Z();
    nx = fLitVolume->GetDetAxis().X();
    ny = fLitVolume->GetDetAxis().Y();
    nz = fLitVolume->GetDetAxis().Z();
    switch (fLitVolume->GetEntrance()) {
      case 0:
        // Face at -dy. (xp,yp) are (x,z)
        if (TMath::Abs(ny)>eps) {
          t  = (-dy - y0)/ny;
          xp = x0 + t*nx;
          yp = z0 + t*nz;
        }
        break;
      case 1:
        // Face at +dx. (xp,yp) are (y,z)
        if (TMath::Abs(nx)>eps) {
          t  = (dx -x0)/nx;
          xp = y0 + t*ny;
          yp = z0 + t*nz;
        }
        break;
      case 2:
        // Face at +dy.  (xp,yp) are (x,z)
        if (TMath::Abs(ny)>eps) {
          t  = (dy - y0)/ny;
          xp = x0 + t*nx;
          yp = z0 + t*nz;
        }
        break;
      case 3:
        // Face at -dx. (xp,yp) are (y,z)
        if (TMath::Abs(nx)>eps) {
          t  = (-dx -x0)/nx;
          xp = y0 + t*ny;
          yp = z0 + t*nz;
        }
        break;
      case 4:
        // Face at -dz.  (xp,yp) are (x,y)
        if (TMath::Abs(nz)>eps) {
          t  = (-dz - z0)/nz;
          xp = x0 + t*nx;
          yp = y0 + t*ny;
        }
        break;
      case 5:
        // Face at +dz.  (xp,yp) are (x,y)
        if (TMath::Abs(nz)>eps) {
          t  = (dz - z0)/nz;
          xp = x0 + t*nx;
          yp = y0 + t*ny;
        }
        break;
      default:
        break;
    }
  }//end if ((!fLitVolume->GetStopAlways()) && (fLitVolume->GetEntrance()>=0))
  bin = fLitDetG->FillDepth(DepthInDet);
  bin = fLitDetG->FillXYEntrance(xp,yp);
  if (gLitCs) {
    bin = fLitDet->FillDepth(DepthInDet);
    bin = fLitDet->FillXYEntrance(xp,yp);
  }
  //
  // (1) - First we look at acceptance angle
  //
  if (fLitVolume->IsAccept()) {
    Double_t cosang;
    cosang = StopDir*fLitVolume->GetDetAxis();
    seen   = (cosang>fLitVolume->GetCosAccept());
    if (!seen) {
      gLitGs->fNpLossAng++;
      fLitDetG->fNpLossAng++;
      if (gLitCs) {
        gLitCs->fNpLossAng++;
        fLitDet->fNpLossAng++;
      }
    }
  }
  //
  // (2) - Then we look at quantum efficiency
  //
  if ((seen) && (fLitVolume->GetIsQE())) {
    Double_t y1,y2;
    bin = fLitDetG->FillInside(fWaveLength);
    if (gLitCs) bin = fLitDet->FillInside(fWaveLength);
    y1 = gRandom3->Rndm();
    y2 = fLitVolume->GetQE();
    gLitGs->fPhweigth += y2;
    if (y1<=y2) {
      //Here, the photon is seen
      bin = fLitDetG->FillQEff(fWaveLength);
      if (gLitCs) {
        bin = fLitDet->FillQEff(fWaveLength);
      }
    }
    else {
      //Here, the photon is not seen, because of quantum efficiency
      seen = kFALSE;
      gLitGs->fNpLossQE++;
      fLitDetG->fNpLossQE++;
      if (gLitCs) {
        gLitCs->fNpLossQE++;
        fLitDet->fNpLossQE++;
      }
    }
  }
  //
  // (3) - if APD, test number of emitted electrons
  //
  if ((seen) && (fLitVolume->GetDetKind()==apd)) {
    if (DepthInDet>TLitDetector::fgMaxCatAn) NbOfPairs = 0.0;
    else NbOfPairs = TMath::Abs(fLitVolume->GetGainProfile()->V(DepthInDet*dixp7));
    seen = (NbOfPairs>=TLitPhys::Get()->Threshe());
    if (!seen) {
      gLitGs->fNpTooFew++;
      if (gLitCs) {
        gLitCs->fNpTooFew++;
      }
    }
  }
  //
  // (4) - Look if too late
  //
  if (seen) {
    //
    // if apd, add drift time towards the anode
    if (fLitVolume->GetDetKind()==apd) {
      Double_t drifttime,catan,speed;
      catan = fLitVolume->GetDistCatAn();
      speed = fLitVolume->GetVDrifte();
      drifttime = (catan-DepthInDet)/speed;
      drifttime *= 1.0e+12;     //go in ps
      fT0 += drifttime;
    }
    seen = (fT0<TLitPhys::Get()->TooLate());
    if (!seen) {
      gLitGs->fNpTooLate++;
      bin = gLitGs->fHTooLate->Fill(fWaveLength);
      if (gLitCs) {
        gLitCs->fNpTooFew++;
        bin = gLitCs->fHTooLate->Fill(fWaveLength);
      }
    }
  }
  //
  // Final stat
  //
  if (seen) {
    gLitGs->fNpSeen++;
    bin = gLitGs->fHTimeSeen->Fill(fT0);
    bin = gLitGs->fHDistSeen->Fill(fSl0);
    bin = gLitGs->fHWvlgthSeen->Fill(fWaveLength);
    fLitDetG->fNpSeen++;
    bin = fLitDetG->FillTimeSeen(fT0);
    bin = fLitDetG->FillWvlgthSeen(fWaveLength);
    if (gLitCs) {
      gLitCs->fNpSeen++;
      bin = gLitCs->fHTimeSeen->Fill(fT0);
      bin = gLitCs->fHDistSeen->Fill(fSl0);
      bin = gLitCs->fHWvlgthSeen->Fill(fWaveLength);
      fLitDet->fNpSeen++;
      bin = fLitDet->FillTimeSeen(fT0);
      bin = fLitDet->FillWvlgthSeen(fWaveLength);
    }
  }
  else {
    gLitGs->fNpLossAny++;
    bin = gLitGs->fHAbsAny->Fill(fSl0);
    fLitDetG->fNpLossAny++;
    bin = fLitDetG->FillWvlgthNot(fWaveLength);
    if (gLitCs) {
      gLitCs->fNpLossAny++;
      bin = gLitCs->fHAbsAny->Fill(fSl0);
      fLitDet->fNpLossAny++;
      bin = fLitDet->FillWvlgthNot(fWaveLength);
    }
  }
}
void TLitPhoton::Init() {
  // Initialization of class variables, except pointers
  const Double_t zerod = 0.0;
  fSourcePath    = "";
  fPath          = "";
  fNextPath      = "";
  fRun           = 0;       //run number
  fKphot         = 0;       //photon number
  fSl0           = zerod;   //space travelled by photon
  fT0            = zerod;   //time elapsed since birth of photon or excitation in [ps]
  //status
  fNPoints       = 0;       //Number of points on the trajectory of the photon
  fIsOnBoundary  = kFALSE;
  fPhtLive       = kFALSE;  //true if photon not yet absorbed or detected
  fPhtSeen       = kFALSE;  //true if photon seen by detector
  fBetween       = kFALSE;  //true if photon between 2 TGeoVolume, none of which being a wrapping
  fOutPoly       = kFALSE;  //VERIFIER QUE PAS INUTILE !true if photon out of TGeoVolume
  fPhtOut        = kFALSE;  //true if photon is transmitted or absorbed, false if reflected.
  fPhtAbs        = kFALSE;  //true if photon absorbed during transition process for instance on reflection on metal.
  fAnomaly       = kFALSE;  //photon to be killed for abnormal reasons. Should never happen
  fFromani       = kFALSE;  //true if incident wave in anisotropic medium
  fChooseIndex   = 0;       //choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //physical values
  fSkl0          = zerod;   //Sum over all k*l
  fWaveLength    = zerod;   //wavelength of photon in nanometers
  fW             = zerod;   //frequency in ps-1
  fEphot         = zerod;   //energy of photon in joule [mksa]
  //k vector
  fAk0           = zerod;   //absolute value (module) of vector k in [cm-1]
  //Absorption length and diffusion length
  fIsAbsorb      = kFALSE;  //true if there is absorption inside this material
  fAbsl0         = zerod;   //abs. length for this photon.
  fIsDiffus      = kFALSE;  //true if there is diffusion inside this material
  fDiffusL       = zerod;   //diffusion length
  fNbOfDiffusion = 0;       //number of diffusions that occured for this photon
  fIsRayleigh    = kFALSE;  //true if there is Rayleigh scattering inside this material
  fRayleighL     = zerod;   //Rayleigh scattering length
  fNbOfRayleigh  = 0;       //number of Rayleigh scatterings that occured for this photon 
  fIsWLS         = kFALSE;  //true if there is this material is wavelength shifting
  fWLSL          = zerod;   //Wavelength shifting length
  fNbOfWLS       = 0;       //number of wavelength shifts that occured for this photon 
  //Electric Field
  fAef0          = zerod;   //absolute value (module) of electric field E
  fBeta          = zerod;   //phase of linearly polarized photon (irrelevant)
  fCospk         = zerod;   //cosine of angle between Poyinting and k vector
  //characteristics of the face where the photon is
  fSinThetaMax   = zerod;   //Max. value for angle theta in diffusion
  //characteristics of material in which the photon is
  fAnis0         = 0;       //anisotropic optical characteristics of material in which photon is
  fAnia0         = 0;       //anisotropic absorption characteristics of material in which photon is
  fPmu0          = 1.0;     //magnetic permeability of material in which photon is
  fDiel0         = zerod;   //diel. constant eps in case of isotropic material
  fIsDiag        = kFALSE;  //the dielectric tensor in use is diagonal in ICS
  //characteristics of material associated with the incident wave
  fAnisi         = 0;       //anisotropic characteristics of material associated with the incident wave
  fAniai         = 0;       //anisotropic absorption characteristics of material associated with the incident wave
  fPmui          = zerod;   //magnetic permeability mu of material associated with the incident wave
  fIsDiagi       = kFALSE;  //the dielectric tensor of the TGeoVolume containing the incident wave is diagonal in ICS
  //characteristics of material into which the photon may go
  fAnis1         = 0;       //anisotropic optical characteristics of destination material
  fAnia1         = 0;       //anisotropic absorption characteristics of destination material
  fPmu1          = 1.0;     //magnetic permeability of destination material
  fDiel1         = zerod;   //diel. constant eps in case of isotropic destination material
  //characteristics of material associated with the transmitted wave
  fAnist         = 0;       //anisotropic characteristics of material associated with the transmitted wave
  fAniat         = 0;       //anisotropic absorption characteristics of material associated with the transmitted wave
  fPmut          = zerod;   //magnetic permeability of  of material associated with the transmitted wave
  fIsDiagt       = kFALSE;  //the dielectric tensor of the TGeoVolume containing the transmitted wave is diagonal in ICS
  //Weight of each possible choice for a wave
  for (Short_t j=0;j<8;j++) fWeigth[j] = zerod;
  fToDraw        = kFALSE;
}
void TLitPhoton::InitP() {
  //Pointers to 0
  fStartGeoVol   = 0;
  fNode          = 0;
  fGeoVolume     = 0;
  fLitVolume     = 0;
  fLitDet        = 0;
  fLitDetG       = 0;
  fGeoMedium     = 0;
  fLitMedium     = 0;
  //
  fNextNode      = 0;
  fNextGeoVolume = 0;
  fNextLitVolume = 0;
  fNextLitDet    = 0;
  fNextLitDetG   = 0;
  fNextGeoMedium = 0;
  fNextLitMedium = 0;
  fI             = 0;
  fR1            = 0;
  fR2            = 0;
  fT1            = 0;
  fT2            = 0;
  fCurrentTrack  = 0;
}
void TLitPhoton::InitTracking() {
  //  Initialize tracking. Finds pointer to TGeoVolume and pointer to optical material.
  // Verifies that starting volume is ok. Calculates vectors in WCS of TLitVolume
  // found.
  //  Do not bother about the fact that the track is initialized with the wrong
  // direction fUk0, instead of the not yet defined fUpg [unit Poyinting vector ]
  // This will be corrected in method StartPhoton(), before any motion of the track.
  //
  const char *met = "InitTracking";
  Bool_t IsLitMedium,detfound;
  fNode = gGeoManager->InitTrack(fX0[0],fX0[1],fX0[2],fUk0[0],fUk0[1],fUk0[2]);
  if (!fNode) {
    fAnomaly = kTRUE;
    gCleanOut->MM(fatal,met,"Photon created outside world",ClassName());
  }//end if (!fNode)
  else {
    fGeoVolume  = fNode->GetVolume();
    if (fGeoVolume != fStartGeoVol) {
      gCleanOut->MM(fatal,met,"Photon not in right TGeoVolume",ClassName());
      fAnomaly = kTRUE;
    }
    const char *path = gGeoManager->GetPath();
    fPath = path;
    if (fSourcePath.CompareTo(fPath)) {
      gCleanOut->MM(error,met,"User path different from found path",ClassName());
      gCleanOut->MMT(error,met,fSourcePath.Data(),"given by","user",ClassName());
      gCleanOut->MMT(fatal,met,path,"found by","TGeo",ClassName());
    }
    fLitVolume = (TLitVolume*)fGeoVolume->GetField();
    if (fLitVolume) {
      fLitVolume->ClearWCSdone();
      if (fLitVolume->GetDetKind() != none) detfound = Detector();
      fLitVolume->SetWCS();
    }//end if (fLitVolume)
    fGeoMedium = fGeoVolume->GetMedium();
    IsLitMedium = fGeoMedium->InheritsFrom("TLitMedium");
    if (!IsLitMedium) {
      gCleanOut->MM(fatal,met,"Photon not created inside a TLitMedium",ClassName());
      fAnomaly = kTRUE;
    }//end if (!IsLitMedium)         
    else {
      fLitMedium = (TLitMedium*)fGeoMedium;
      if (!fLitVolume) {
        if (fLitMedium->GetAnisotropy())
          gCleanOut->MM(fatal,met,"If medium anisotropic, TLitVolume is required",ClassName());
      }//end if (!fLitVolume)
      if (fLitMedium->IsWrapping()) {
        gCleanOut->MM(fatal,met,"Photon created inside a wrapping",ClassName());
        fAnomaly = kTRUE;
      }//end if (fLitMedium->IsWrapping())
      else {
        fIsOnBoundary = kFALSE;
        fPmu0         = fLitMedium->GetMu();
        fAnis0        = fLitMedium->GetAnisotropy();
        fAnia0        = fLitMedium->GetAniAbsL();
        if (fAnis0) fDzt0     = fLitVolume->fDTmu;
        else        fRfIndex0 = fLitMedium->GetIndex();
        fIsAbsorb     = fLitMedium->GetHasAbsorption();
        if (fIsAbsorb) {
          if (fAnia0) {
            fCDzt0 = fLitVolume->fCDTmu;
            FindAbsLength();
          }//end if (fAnia0)
          else fAbsl0 = fLitMedium->GetAbsL();
        }//end if (fIsAbsorb)
        fIsRayleigh   = fLitMedium->GetHasRayleigh();  
        fRayleighL    = fLitMedium->GetRayleighLength();
        fIsWLS        = fLitMedium->GetHasWLS();
        fWLSL         = fLitMedium->GetWLSL();
        fIsDiffus     = fLitMedium->GetHasDiffusion();
        fDiffusL      = fLitMedium->GetDiffLength();
      }//end else if (fLitMedium->IsWrapping())
    }//end else if (!IsLitMedium)
  }//end else if (!fNode)
}//end InitTracking()
Bool_t TLitPhoton::IsDamage() const {
  //  Returns true if TGeoVolume fGeoVolume is associated with TLitVolume fLitVolume
  // and fLitVolume has radiation damage
  Bool_t dam = kFALSE;
  if (fLitVolume) {
    dam = fLitVolume->IsDamage();
  }
  return dam;
}
void TLitPhoton::IsoToAni() {
  //Calculates the transition from an isotropic medium towards an anisotropic
  //medium
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-5;
  const Double_t dixm2  = 1.0e-2;
  const Double_t wsmall = 1.0e-11;
  const Double_t DbLim  = 1.0e-150;
  const char *met = "IsoToAni";
  //
  TComplex zeps;
  Double_t eps;
  Double_t ns;               //(index of refraction)*(sin(thetai) of incident wave
  Double_t a1,a3,a4,a5,gd;
  Double_t a,b,c,d,g;        //coefficients for the equation (E1) handled by
  //TEqIndex giving the 2 indices for the 2
  //transmitted waves
  Double_t quality;          //quality of solution provided by TEqIndex
  Double_t d00,d01,d02,d11,d12,d22;
  Double_t c01,c02,c12;
  Double_t dh1,dh2;
  TComplex zh1;
  TVector3 vh1;
  Short_t is;
  Bool_t isol[4];
  Bool_t  eva1,eva2;         //true if wave fR1(fR2) evanescent
  Bool_t Good1;              //The 1st solution selected among the 4
  //proposed by TEqIndex is good or not
  Bool_t Good2;              //The 2nd solution selected among the 4
  //proposed by TEqIndex is good or not
  Short_t imin1,imin2;       //2 solutions among the 4 proposed by TEqIndex
  //
  Bool_t invers = kFALSE;    //true if the 2 solutions have been interchanged
  fR1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  //Same index for reflected and incident wave since medium isotropic
  fI->fRfI    = fRfIndexi;
  fR1->fRfI   = fI->fRfI;
  //Angle of reflection == angle of incidence and real.
  fR1->fCtht  = fI->fCtht;
  fR1->fStht  = fI->fStht;
  // Here comes the calculation of the equation (E1) (see theoretical paper)
  //giving the 2 possible indices for the 2 transmitted waves
  ns  = fI->fRfI.Re()*fI->fStht.Re();
  g   = ns*ns;
  d00 = fDztti(0,0);
  d01 = fDztti(0,1);
  d02 = fDztti(0,2);
  d11 = fDztti(1,1);
  d12 = fDztti(1,2);
  d22 = fDztti(2,2);
  c01 = d01*d01;
  c02 = d02*d02;
  c12 = d12*d12;
  a1  = (d00-d11)*g + d00*(d11+d22) - c02 - c01;
  a3  = deux*d01*ns;               //+sign, contrary to same equation in AniToIso
  a4  = deux*ns*(d01*d22-d02*d12); //+sign, contrary to same equation in AniToIso
  a5  = g*(c02 - c12 - d22*(d00-d11));
  a5 += - d00*d11*d22 - deux*d01*d02*d12 + d11*c02 + d00*c12 + d22*c01;
  // We have to divide by d00, so we cannot accept it to be 0
  if (TMath::Abs(d00) < DbLim) {
    PrintWhich();
    gCleanOut->MMR(killthis,met,"fDztti(0,0) is 0","d00",d00,ClassName());
    fAnomaly = kTRUE;
    return;
  }
  fFromani = kFALSE;
  a = -a1/d00;
  b =  a3/d00;
  c = -a4/d00;
  d = -a5/d00;
  if (fIsDiagt) {
    gd  = d22 - d11;
    gd += (g*(d11 - d00))/d00;
    gd *= gd;
  }
  else {
    gd  = a*a - quatre*d;
  }
  gLitIdx->Set(a,b,c,d,g,gd);
  quality = gLitIdx->Solve(fAnist);
  if (quality>small) {
    PrintWhich();
    gCleanOut->MMR(killthis,met,"Bad solution for index equation","quality",quality,ClassName());
    gCleanOut->MRR(killthis,"a",a,"b",b);
    gCleanOut->MRR(killthis,"c",c,"d",d);
    gCleanOut->MRR(killthis,"g",g,"gd",gd);
    fAnomaly = kTRUE;
    return;
  }
  for (is=0;is<4;is++) isol[is] = kTRUE;
  Short_t NumberOfTry = 0;
bigloop:
  do {
    Good1 = kFALSE;
    Good2 = kFALSE;
    NumberOfTry += 1;
    fR1->fPhases = 0;
    fT1->fPhases = 0;
    fT2->fPhases = 0;
    //We look for 2 possible solutions.
    Bool_t found = kFALSE;
    while (!found) {
      GetImin12(NumberOfTry,imin1,imin2);
      found = (isol[imin1]) && (isol[imin2]);
      if (!found) {
        NumberOfTry += 1;
        if (NumberOfTry>6) {
          PrintWhich();
          gCleanOut->MM(killthis,met,"All solutions tried without success",ClassName());
          fAnomaly = kTRUE;
          return;
        }// end if (NumberOfTry>6)
      }// end if (!found)
    }// end while (!found)
    eva1 = gLitIdx->GetfR(imin1);
    eva2 = gLitIdx->GetfR(imin2);
    //We assign found value of index to the 1st transmitted wave :
    fT1->fRfI = TComplex::Sqrt(gLitIdx->GetfZ(imin1));
    fT1->fEva = eva1;
    if (!eva1) fT1->fRfI(fT1->fRfI.Re(),0.0);
    //We assign found value of index to the 2nd transmitted wave :
    fT2->fRfI = TComplex::Sqrt(gLitIdx->GetfZ(imin2));
    fT2->fEva = eva2;
    if (!eva2) fT2->fRfI(fT2->fRfI.Re(),0.0);
    //Fresnel or Descartes equation for the sinus of the 1st transmitted wave
    fT1->fStht = ns/fT1->fRfI;
    fT1->fCtht = gLitIdx->GetfD(imin1);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fT1->fCtht.Im())>wsmall) &&
      (TMath::Abs(fT1->fCtht.Re())<wsmall)) fT1->fCtht(0.0,fT1->fCtht.Im());
    if ((TMath::Abs(fT1->fCtht.Re())>wsmall) &&
      (TMath::Abs(fT1->fCtht.Im())<wsmall)) fT1->fCtht(fT1->fCtht.Re(),0.0);
    else fT1->fEva = kTRUE;
    if (TLit::Get()->GetTestCalc()) {
      zeps = 1.0 - fT1->fStht*fT1->fStht - fT1->fCtht*fT1->fCtht;
      eps  = zeps.Rho();
      if (eps>small)
        gCleanOut->MMR(error,met,"Bad sin/cos for fT1","test",eps,ClassName());
    }
    //Fresnel or Descartes equation for the sinus of the 2nd transmitted wave
    fT2->fStht = ns/fT2->fRfI;
    fT2->fCtht = gLitIdx->GetfD(imin2);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fT2->fCtht.Im())>wsmall) &&
      (TMath::Abs(fT2->fCtht.Re())<wsmall)) fT2->fCtht(0.0,fT2->fCtht.Im());
    if ((TMath::Abs(fT2->fCtht.Re())>wsmall) &&
      (TMath::Abs(fT2->fCtht.Im())<wsmall)) fT2->fCtht(fT2->fCtht.Re(),0.0);
    else fT2->fEva = kTRUE;
    if (TLit::Get()->GetTestCalc()) {
      zeps = 1.0 - fT2->fStht*fT2->fStht - fT2->fCtht*fT2->fCtht;
      eps  = zeps.Rho();
      if (eps>small)
        gCleanOut->MMR(error,met,"Bad sin/cos for fT2","test",eps,ClassName());
    }
    //Contrary to AniToIso, here both waves may very well be evanescent.
    //We must check that if only one wave evanescent,then it is the second
    if ((fT1->fEva) && (!fT2->fEva)) {
      TComplex cc1,cc2,cc3;
      invers     = kTRUE;
      cc1        = fT2->fRfI;
      cc2        = fT2->fStht;
      cc3        = fT2->fCtht;
      fT2->fRfI  = fT1->fRfI;
      fT2->fStht = fT1->fStht;
      fT2->fCtht = fT1->fCtht;
      fT1->fRfI  = cc1;
      fT1->fStht = cc2;
      fT1->fCtht = cc3;
      fT1->fEva  = kFALSE;
      fT2->fEva  = kTRUE;
    }
    //
    //Determination of alfa (Salf,Calf) and delta (Sdel,Cdel) by solution of the
    //degenerate system (4), (5), (6) [see theoretical paper]. We separate the
    //real and the complex case.
    //
    //1st transmitted wave :
    //
    if (fT1->fEva) {
      TComplex za,z00,z01,z02,z11,z12,z22;
      za  = fT1->fRfI*fT1->fCtht;
      z00 = g - d00;
      z01 = ns*za + d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = - d12;
      za  = fT1->fRfI;
      z22 = za*za - d22;
      TMat33C M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      TVec3C v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st transm wave : matrix M (complex) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st transm wave : matrix M (complex) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"1st transm wave : matrix M (complex) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = TLitPhoton::AlfaDelta(v1,fT1->fStht,fT1->fCtht,
        fT1->fSalf,fT1->fCalf,fT1->fSdel,fT1->fCdel);
//      bdeg = v1.AlfaDelta(fT1->fStht,fT1->fCtht,
//        fT1->fSalf,fT1->fCalf,fT1->fSdel,fT1->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fT1->fRfI.Re()*fT1->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa + d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = - d12;
      qa  = fT1->fRfI.Re();
      q22 = qa*qa - d22;
      TMat33 M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      TVector3 v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st transm wave : matrix M (real) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"1st transm wave : matrix M (real) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"1st transm wave : matrix M (real) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = TLitPhoton::AlfaDelta(v1,fT1->fStht.Re(),fT1->fCtht.Re(),qsa,qca,qsd,qcd);
//      bdeg = v1.AlfaDelta(fT1->fStht.Re(),fT1->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fT1->fSalf = qsa;
      fT1->fCalf = qca;
      fT1->fSdel = qsd;
      fT1->fCdel = qcd;
    }
    //
    //2nd transmitted wave :
    //
    if (fT2->fEva) {
      TComplex za,z00,z01,z02,z11,z12,z22;
      za  = fT2->fRfI*fT2->fCtht;
      z00 = g - d00;
      z01 = ns*za + d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = - d12;
      za  = fT2->fRfI;
      z22 = za*za - d22;
      TMat33C M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      TVec3C v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd transm wave : matrix M (complex) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd transm wave : matrix M (complex) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"2nd transm wave : matrix M (complex) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = TLitPhoton::AlfaDelta(v1,fT2->fStht,fT2->fCtht,
        fT2->fSalf,fT2->fCalf,fT2->fSdel,fT2->fCdel);
//      bdeg = v1.AlfaDelta(fT2->fStht,fT2->fCtht,
//        fT2->fSalf,fT2->fCalf,fT2->fSdel,fT2->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fT2->fRfI.Re()*fT2->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa + d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = - d12;
      qa  = fT2->fRfI.Re();
      q22 = qa*qa - d22;
      TMat33 M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      TVector3 v1,v2;
      Short_t tdeg;
      tdeg = M.DegMatrix(v1,v2,TLit::Get()->GetTestCalc());
//      tdeg = v1.DegMatrix(M,v2,TLit::Get()->GetTestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd transm wave : matrix M (real) not degenerate",ClassName());
        break;
      case 2:
        PrintWhich();
        gCleanOut->MM(warning,met,"2nd transm wave : matrix M (real) twice degenerate",ClassName());
        continue;
      case 3:
        PrintWhich();
        gCleanOut->MM(killthis,met,"2nd transm wave : matrix M (real) is 0",ClassName());
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = TLitPhoton::AlfaDelta(v1,fT2->fStht.Re(),fT2->fCtht.Re(),qsa,qca,qsd,qcd);
//      bdeg = v1.AlfaDelta(fT2->fStht.Re(),fT2->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fT2->fSalf = qsa;
      fT2->fCalf = qca;
      fT2->fSdel = qsd;
      fT2->fCdel = qcd;
    }
    //
    //Here are the variables that we have determined until now :
    //
    //    fT1->fRfI
    //    fT1->fStht and fCtht
    //    fT1->fSalf and fCalf
    //    fT1->fSdel and fCdel
    //    fT2->fRfI
    //    fT2->fStht and fCtht
    //    fT2->fSalf and fCalf
    //    fT2->fSdel and fCdel
    //    fR1->fRfI
    //    fR1->fStht and fCtht
    //
    //Here are the variables that remain to be found :
    //
    //    fT1->fE
    //    fT2->fE
    //    fR1->fE
    //    fR1->fSalf and fCalf
    //
    //These last variables are found thanks to equations (9) (10) (11) (12)
    // (See theoretical paper)
    //
    //
    //Solution for fT1->fE and fT2->fE
    //
    zh1  = fPmui/(fI->fRfI.Re()*fPmut);
    dh1  = fI->fCtht.Re();
    fBT1 = fT1->fRfI*zh1;
    fBT2 = fT2->fRfI*zh1;
    fT1->fAlm1 =fT1->fStht*fT1->fSdel + fT1->fCtht*fT1->fCdel;
    fT2->fAlm1 =fT2->fStht*fT2->fSdel + fT2->fCtht*fT2->fCdel;
    TComplex F12,F13,F22,F23,F32,F33,F42,F43,disc;
    F12  = fBT1*fT1->fCalf*fT1->fAlm1;
    F13  = fBT2*fT2->fCalf*fT2->fAlm1;
    F22  = zero - (fT1->fCdel*fT1->fCalf)/dh1;
    F23  = zero - (fT2->fCdel*fT2->fCalf)/dh1;
    F32  = fT1->fSalf;
    F33  = fT2->fSalf;
    F42  = zero - (fBT1*fT1->fCtht*fT1->fSalf)/dh1;
    F43  = zero - (fBT2*fT2->fCtht*fT2->fSalf)/dh1;
    disc = (F12-F22)*(F33-F43) - (F13-F23)*(F32-F42);
    //
    //We have to divide by disc, so it cannot be extremely small. If we find it
    //extremely small, we take it as a sign that we have made a wrong choice of
    //solution from TEqIndex. For instance, if by error TEqIndex has given twice
    //the same solution, disc will be 0. So we try an other pair of solution.
    //
    if (TComplex::Abs(disc)<DbLim) continue;
    zh1 = (deux*fI->fE)/disc;
    fT1->fE = zh1*((F33-F43)*fI->fCalf.Re() - (F13-F23)*fI->fSalf.Re());
    fT2->fE = zh1*((F12-F22)*fI->fSalf.Re() - (F32-F42)*fI->fCalf.Re());
    //We do not accept that the real part of the module of an electric field be <0
    if (fT1->fE.Re()<zero) {
      fT1->fE    = zero - fT1->fE;
      fT1->fSalf = zero - fT1->fSalf;
      fT1->fCalf = zero - fT1->fCalf;
      F12        = zero - F12;
      F22        = zero - F22;
      F32        = zero - F32;
      F42        = zero - F42;
    }
    if (fT2->fE.Re()<zero) {
      fT2->fE    = zero - fT2->fE;
      fT2->fSalf = zero - fT2->fSalf;
      fT2->fCalf = zero - fT2->fCalf;
      F13        = zero - F13;
      F23        = zero - F23;
      F33        = zero - F33;
      F43        = zero - F43;
    }
    //
    //Only 2 variables remain to be found :
    //
    //    fR1->fE
    //    fR1->fSalf and fCalf
    //
    TComplex rca,rsa;
    rca =  F12*fT1->fE + F13*fT2->fE - fI->fE*fI->fCalf.Re();
    rsa =  F32*fT1->fE + F33*fT2->fE - fI->fE*fI->fSalf.Re();
    fR1->fE = TComplex::Sqrt(rsa*rsa+rca*rca);
    if (fR1->fE.Re()<zero) fR1->fE = zero - fR1->fE;
    fR1->fCalf = rca/fR1->fE;
    fR1->fSalf = rsa/fR1->fE;
    if (TLit::Get()->GetTestCalc()) {
      zeps = 1.0 - fR1->fSalf*fR1->fSalf - fR1->fCalf*fR1->fCalf;
      eps  = zeps.Rho();
      if (eps>small)
        gCleanOut->MMR(error,met,"Bad sin/cos for fR1","test",eps,ClassName());
    }
    //
    //All the variables of system of equations (B1-B16) have now been found ! We
    //calculate the other physical values depending upon these variables
    //
    //  Last calculations for wave fT1
    //
    // - module and phase of electric field :
    TLitPhoton::RPhi(fT1->fE,fT1->fAE,fT1->fPE);
    if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
    else fT1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fT1->fCalf,fT1->fWcaf,fT1->fPcaf);
    TLitPhoton::RPhi(fT1->fSalf,fT1->fWsaf,fT1->fPsaf);
    if (TMath::Abs(fT1->fPcaf - fT1->fPsaf)>wsmall)  fT1->fPhases += 1;
    // - angle of refraction complex for the 1st transmitted wave ?
    if (TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im())>wsmall)
      fT1->fPhases = 3;
    switch (fT1->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT1->fSdel.Im())>small) ||
        (TMath::Abs(fT1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fT1->fSdel(fT1->fSdel.Re(),0.0);
      fT1->fCdel(fT1->fCdel.Re(),0.0);
      fT1->fEf.SetXYZ( - fT1->fAE*fT1->fSdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT1 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT1->fUp(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if (dh1<zero) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different
      //transmitted waves ! To be absolutly sure about that, we still leave this
      //possibility open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"Electric field fT1->fEf has 2 phases",ClassName());
      gCleanOut->MM(error,met,"This should not happen in anisotropic media",ClassName());
      fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT1->fSdel.Im())>small) ||
        (TMath::Abs(fT1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fT1->fSdel(fT1->fSdel.Re(),0.0);
      fT1->fCdel(fT1->fCdel.Re(),0.0);
      fT1->fEfp.SetXYZ( -fT1->fAE*fT1->fSdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCdel.Re()*fT1->fWcaf,
        zero);
      fT1->fEfn.SetXYZ(zero,zero,fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
      BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT1 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT1->fUpp(0);
      dh2 = fT1->fUpn(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if ((dh2<zero) && (dh2> -wsmall)) {
        dh2 = - dh2;
        fT1->fUp(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    default:
      //
      //Here there are 3 phases. The 1st transmitted wave is absorbed. This is
      //perfectly allowed : this is the case in total reflection, or in the 
      //reflection on a wrapping with complex index. If the wave is absorbed,
      //we have nothing to calculate any more for it !
      //
      Good1 = kTRUE;
      break;
    }//end switch
    //
    //  Last calculations for wave fT2
    //
    // - module and phase of electric field :
    TLitPhoton::RPhi(fT2->fE,fT2->fAE,fT2->fPE);
    if (TMath::Abs(fT2->fPE)>wsmall) fT2->fPhases += 1;
    else fT2->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fT2->fCalf,fT2->fWcaf,fT2->fPcaf);
    TLitPhoton::RPhi(fT2->fSalf,fT2->fWsaf,fT2->fPsaf);
    if (TMath::Abs(fT2->fPcaf - fT2->fPsaf)>wsmall)  fT2->fPhases += 1;
    // - angle of refraction complex for the 2nd transmitted wave ?
    if (TMath::Abs(fT2->fCtht.Im()) + TMath::Abs(fT2->fStht.Im())>wsmall)
      fT2->fPhases = 3;
    switch (fT2->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      fT2->fUk.SetXYZ(fT2->fCtht.Re(),fT2->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fT2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      fT2->fAk  = (fW*fT2->fRfI.Re())/TLitPhys::Get()->C();
      fT2->fVk  = fT2->fAk*fT2->fUk;
      fT2->fPE += (fT2->fPcaf + fT2->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be
      //complex in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT2->fSdel.Im())>small) ||
        (TMath::Abs(fT2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fT2->fSdel(fT2->fSdel.Re(),0.0);
      fT2->fCdel(fT2->fCdel.Re(),0.0);
      fT2->fEf.SetXYZ( - fT2->fAE*fT2->fSdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fCdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fWsaf);
      BuildBPUP(fT2->fVk,fT2->fEf,fT2->fBf,fT2->fPg,fT2->fUp,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT2 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT2->fUp(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT2->fUp(0) = dh1;
      }
      if (dh1<zero) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different
      //transmitted waves ! To be absolutly sure about that, we still leave this
      //possibility open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"Electric field fT2->fEf has 2 phases",ClassName());
      gCleanOut->MM(error,met,"This should not happen in anisotropic media",ClassName());
      fT2->fUk.SetXYZ(fT2->fCtht.Re(),fT2->fStht.Re(),zero);
      //The index of refraction of the 2nd transmitted wave cannot be complex in
      //the case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fT2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      fT2->fAk  = (fW*fT2->fRfI.Re())/TLitPhys::Get()->C();
      fT2->fVk  = fT2->fAk*fT2->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT2->fSdel.Im())>small) ||
        (TMath::Abs(fT2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fT2->fSdel(fT2->fSdel.Re(),0.0);
      fT2->fCdel(fT2->fCdel.Re(),0.0);
      fT2->fEfp.SetXYZ( -fT2->fAE*fT2->fSdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fCdel.Re()*fT2->fWcaf,
        zero);
      fT2->fEfn.SetXYZ(zero,zero,fT2->fAE*fT2->fWsaf);
      BuildBPUP(fT2->fVk,fT2->fEfp,fT2->fBfp,fT2->fPgp,fT2->fUpp,fPmut);
      BuildBPUP(fT2->fVk,fT2->fEfn,fT2->fBfn,fT2->fPgn,fT2->fUpn,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT2 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT2->fUpp(0);
      dh2 = fT2->fUpn(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT2->fUp(0) = dh1;
      }
      if ((dh2<zero) && (dh2> -wsmall)) {
        dh2 = - dh2;
        fT2->fUp(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    default:
      //
      //Here there are 3 phases. The 2nd transmitted wave is absorbed. This is
      //perfectly allowed : this is the case in total reflection, or in the 
      //reflection on a wrapping with complex index. If the wave is absorbed,
      //we have nothing to calculate any more for it !
      //
      Good2 = kTRUE;
      break;
    }

    //
    //  Last calculations for wave fR1
    //
    //As reflected wave in isotropic medium, angle delta = angle theta
    fR1->fSdel = fR1->fStht;
    fR1->fCdel = fR1->fCtht;
    // - module and phase of electric field :
    TLitPhoton::RPhi(fR1->fE,fR1->fAE,fR1->fPE);
    if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
    else fR1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    TLitPhoton::RPhi(fR1->fCalf,fR1->fWcaf,fR1->fPcaf);
    TLitPhoton::RPhi(fR1->fSalf,fR1->fWsaf,fR1->fPsaf);
    if (TMath::Abs(fR1->fPcaf - fR1->fPsaf)>wsmall)  fR1->fPhases += 1;
    // - angle of refraction complex for the 1st reflective wave ? No !
    if (TMath::Abs(fR1->fCtht.Im()) + TMath::Abs(fR1->fStht.Im())>wsmall)
      fR1->fPhases = 3;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //Here either no phase, or one global phase. Parallel and normal components
      //of the wave have the same phase. No reason to distinguish between the 2.
      //The reflected wave is not elliptically polarized.
      //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
      //reflection angle is defined contrary to the trigonometric sense. A reflection
      //angle EQUAL to the incidence angle implies a reflected wave having an
      //OPPOSITE x direction as the incident wave.
      fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phase
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) continue;
      //Module of k vector
      fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk  = fR1->fAk*fR1->fUk;
      //phase of electric field
      fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
      //3 components of electric field
      fR1->fEf.SetXYZ(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
        fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
      //
      //reflected wave must go backward
      //
      dh1 = fR1->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUp(0) = dh1;
      }
      if (dh1>zero) {
        PrintWhich();
        gCleanOut->MMR(killthis,met,"Reflected wave going forward","dh1",dh1,ClassName());
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (TLit::Get()->GetTestCalc()) {
        vh1 = fR1->fEf;
        vh1 = vh1.Unit();
        dh1 = TMath::Abs(vh1*fR1->fUk);
        if (dh1>small) {
          PrintWhich();
          gCleanOut->MMR(killthis,met,"k and E not orthogonal","dh1",dh1,ClassName());
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","fR1->fRfI",fR1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //electric field
      fR1->fEfp.SetXYZ(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
        zero);
      fR1->fEfn.SetXYZ(zero,zero,fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmui);
      BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmui);
      //
      //reflected wave must go backward
      //
      dh1 = fR1->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUpp(0) = dh1;
      }
      dh2 = fR1->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR1->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        PrintWhich();
        gCleanOut->MM(killthis,met,"Reflected wave going forward",ClassName());
        gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (TLit::Get()->GetTestCalc()) {
        vh1 = fR1->fEfp;
        vh1 = vh1.Unit();
        dh1 = TMath::Abs(vh1*fR1->fUk);
        vh1 = fR1->fEfn;
        vh1 = vh1.Unit();
        dh2 = TMath::Abs(vh1*fR1->fUk);
        if ((dh1>small) || (dh2>small)) {
          PrintWhich();
          gCleanOut->MM(killthis,met,"k and E not orthogonal",ClassName());
          gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    default:
      //Here there are 3 phases so that there would be no reflected wave. This
      //is impossible.
      PrintWhich();
      gCleanOut->MM(killthis,met,"Reflected wave absorbed ?",ClassName());
      fAnomaly = kTRUE;
      return;
      break;
    }//end switch (fR1->fPhases)
  }// end do while
  while (((!Good1) || (!Good2)) && (NumberOfTry<6));
  //Verify that the system of 12 equations has been solved properly
  if (TLit::Get()->GetTestCalc()) VerifyItoA();
  if (fAnomaly) return;
  //
  //  After all these complicate calculations, we have at hand a powerful check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved! Notice that if a transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if a transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  //  We HAVE to do this test in any case, since a non-conservation of flux
  //may be the sign that we have taken a bad pair of solutions and that we
  //have to try an other one. ( We leave the "if ( 1 || TLit::Get()->GetTestCalc())"
  //in the case that it appears experimentally that it never happens. In
  //that case, we could render the test optional. )
  //
  if ( 1 /* TLit::Get()->GetTestCalc() */ ) {
    Double_t outflux = 0.0;
    Double_t influx,dflux;
    influx = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 1 | fT2 : 1
      outflux = fT1->fPg(0) + fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 1 | fT2 : 2   !!! theoretically impossible         !!!
      outflux = fT1->fPg(0) + fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 1 | fT2 : 3
      //
      //  2nd transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = fT1->fPg(0) - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    case 2:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 2 | fT2 : 1   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 2 | fT2 : 2   !!! fT1/2:2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + 
        fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 2 | fT2 : 3   !!! fT1 : 2 theoretically impossible !!!
      //
      //  2nd transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    default:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 3 | fT2 : 1   !!! fT1 and fT2 out of order         !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"fT1 and fT2 out of order [1]",ClassName());
      outflux = fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 3 | fT2 : 2   !!! fT1 and fT2 out of order         !!!
      //                               !!! fT2 : 2 theoretically impossible !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"fT1 and fT2 out of order [2]",ClassName());
      outflux = fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 3 | fT2 : 3
      //
      //  Both transmitted wave are absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
      }//end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 1 | fT2 : 1
      outflux = fT1->fPg(0) + fT2->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 1 | fT2 : 2
      outflux = fT1->fPg(0) + fT2->fPgn(0) + fT2->fPgp(0) -
        fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 1 | fT2 : 3
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    case 2:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 2 | fT2 : 1   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPg(0)
        - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 2 | fT2 : 2   !!! fT1/2:2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPgn(0) + fT2->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 2 | fT2 : 3   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    default:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 3 | fT2 : 1   !!! fT1 and fT2 out of order         !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gCleanOut->MM(error,met,"fT1 and fT2 out of order [3]",ClassName());
      outflux = fT2->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 3 | fT2 : 2   !!! fT1 and fT2 out of order         !!!
      //                               !!! fT2 : 2 theoretically impossible !!!
      PrintWhich();
      gCleanOut->MM(error,met,"fT1 and fT2 out of order [4]",ClassName());
      outflux = fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 3 | fT2 : 3
      //
      //  Both transmitted wave are absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a wrapping whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
      }//end switch (fT1->fPhases)
      break;
    default:
      // fR1 : 3  an error report for this case has already been given
      break;
    }//end switch (fR1->fPhases)
    dflux = influx - outflux;
    if (TMath::Abs(dflux)>dixm2) {
      PrintWhich();
      gCleanOut->MMR(warning,met,"Flux not conserved","dflux",dflux,ClassName());
      gCleanOut->MM(warning,met,"This may be due to a bad choice of solutions",ClassName());
      gCleanOut->MM(warning,met," from TLitEqIndex. We try an other choice",ClassName());
      if (NumberOfTry<7) goto bigloop;
      else {
        PrintWhich();
        gCleanOut->MM(killthis,met,"All solutions tried without success",ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }
  }//end if (TLit::Get()->GetTestCalc())
}
void TLitPhoton::IsoToIso() {
  //Calculates the transition from an isotropic medium towards an isotropic
  //medium. There is no thin slice in-between.
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t msmall = 1.0e-6;
  const Double_t small  = 1.0e-8;
  const Double_t vsmall = 1.0e-10;
  const Double_t wsmall = 1.0e-11;
  const char *met = "IsoToIso";
  Double_t aa,bb,cc,TestIm;
  Double_t dh1,dh2;
  TComplex a,b;
  TComplex delta,cnum1,cnum2,cden1,cden2;
  TComplex tsa,tca,rsa,rca;   //T*sin(alf) T*cos(alf) R*sin(alf) R*cos(alf)
  fR1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare  = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare  = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fR1->fPhases = 0;           //initializes at 0 the number of phases
  fT1->fPhases = 0;           //initializes at 0 the number of phases
  fI->fRfI     = fRfIndexi;   //assigns to each wave its index of refraction
  fR1->fRfI    = fRfIndexi;
  fT1->fRfI    = fRfIndext;
  fR1->fCtht   = fI->fCtht;
  fR1->fStht   = fI->fStht;   //angle of reflection == angle of incidence : REAL
  //Fresnel or Descartes formula, between incident and transmitted
  fT1->fStht = (fI->fRfI.Re()*fI->fStht.Re())/fT1->fRfI;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht = TLitPhoton::CosFromSin(fT1->fStht);
  //Here come the calculations involving electric fields of the waves
  //fT1->fRfI, fT1->fCtht may be complex.
  delta = (fPmui*fT1->fRfI)/(fPmut*fI->fRfI.Re());
  a     = delta*fI->fCtht.Re();
  b     = delta*fT1->fCtht;
  cnum1 = a - fT1->fCtht;
  cden1 = a + fT1->fCtht;
  cnum2 = fI->fCtht.Re() - b;
  cden2 = fI->fCtht.Re() + b;
  aa    = fI->fAE*fI->fCalf.Re();
  bb    = fI->fAE*fI->fSalf.Re();
  cc    = deux*fI->fCtht.Re();
  tca   = (aa*cc)/cden1;
  tsa   = (bb*cc)/cden2;
  rca   = (aa*cnum1)/cden1;
  rsa   = (bb*cnum2)/cden2;
  //
  //We calculate everything on the transmitted wave
  //
  fT1->fE  = TComplex::Sqrt(tsa*tsa+tca*tca);
//  fT1->fE.RPhi(fT1->fAE,fT1->fPE);
  TLitPhoton::RPhi(fT1->fE,fT1->fAE,fT1->fPE);
  if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
  else fT1->fPE = zero;
  fT1->fCalf = tca/fT1->fE;
  fT1->fSalf = tsa/fT1->fE;
//  fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
  TLitPhoton::RPhi(fT1->fCalf,fT1->fWcaf,fT1->fPcaf);
//  fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
  TLitPhoton::RPhi(fT1->fSalf,fT1->fWsaf,fT1->fPsaf);
  if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
  TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
    TMath::Abs(fT1->fRfI.Im());
  if (TestIm>wsmall) fT1->fPhases = 3;
  switch (fT1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The transmitted wave is not elliptically polarized.
    fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (TLit::Get()->GetTestCalc()) {
      //If the phases are 0,1 or 2, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","Index",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }//end if (TLit::Get()->GetTestCalc())
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
    fT1->fEf.SetXYZ(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUp(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUp(0) = dh1;
    }
    if (dh1<zero) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Transmitted wave going backward","dh1",dh1,ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (TLit::Get()->GetTestCalc()) {
      //If there are 2 phases, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","fT1->fRfI",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fEfp.SetXYZ(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      zero);
    fT1->fEfn.SetXYZ(zero,zero,fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
    BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUpp(0);
    dh2 = fT1->fUpn(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUpp(0) = dh1;
    }
    if ((dh2<zero) && (dh2> -wsmall)) {
      dh2 = - dh2;
      fT1->fUpn(0) = dh2;
    }
    if ((dh1<zero) || (dh2<zero)) {
      PrintWhich();
      gCleanOut->MM(error,met,"Transmitted wave going backward",ClassName());
      gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 3:
    //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
    //anything
    break;
  default:
    PrintWhich();
    gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","nphases",fT1->fPhases,ClassName());
    fAnomaly = kTRUE;
    return;
  }//end switch (fT1->fPhases)
  //
  //We calculate everything on the reflected wave
  //
  fR1->fE  = TComplex::Sqrt(rsa*rsa+rca*rca);
//  fR1->fE.RPhi(fR1->fAE,fR1->fPE);
  TLitPhoton::RPhi(fR1->fE,fR1->fAE,fR1->fPE);
  if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
  else fR1->fPE = zero;
  fR1->fCalf = rca/fR1->fE;
  fR1->fSalf = rsa/fR1->fE;
//  fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
  TLitPhoton::RPhi(fR1->fCalf,fR1->fWcaf,fR1->fPcaf);
//  fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
  TLitPhoton::RPhi(fR1->fSalf,fR1->fWsaf,fR1->fPsaf);
  if (TMath::Abs(fR1->fPcaf-fR1->fPsaf)>wsmall) fR1->fPhases +=  1;
  //A reflected wave is never absorbed
  switch (fR1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The reflected wave is not elliptically polarized.
    //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
    //reflection angle is defined contrary to the trigonometric sense. A reflection
    //angle EQUAL to the incidence angle implies a reflected wave having an
    //OPPOSITE x direction as the incident wave.
    fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
    fR1->fEf.SetXYZ(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUp(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUp(0) = dh1;
    }
    if (dh1>zero) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Reflected wave going forward","dh1",dh1,ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fEfp.SetXYZ(-fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      -fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      zero);
    fR1->fEfn.SetXYZ(zero,zero,fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmut);
    BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmut);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUpp(0);
    dh2 = fR1->fUpn(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUpp(0) = dh1;
    }
    if ((dh2>zero) && (dh2<wsmall)) {
      dh2 = - dh2;
      fR1->fUpn(0) = dh2;
    }
    if ((dh1>zero) || (dh2>zero)) {
      PrintWhich();
      gCleanOut->MRR(error,"dh1",dh1,"dh2",dh2);
      gCleanOut->MM(killthis,met,"Reflected wave going forward",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  default:
    PrintWhich();
    gCleanOut->MMI(killthis,met,"Abnormal nb. of phases reflected wave","fR1->fPhases",fR1->fPhases,ClassName());
    fAnomaly = kTRUE;
    return;
  }//end switch (fR1->fPhases)
  //
  //  After all these complicate calculations, we have at hand an important check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved. Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  if (TLit::Get()->GetTestCalc()) {
    Bool_t ToCheck = kTRUE;
    Double_t flux;
    Double_t dflux = 0;
    flux = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //reflected wave not elliptically polarized
      switch (fT1->fPhases) {
      case 0:
      case 1:
        //reflected only absolut phase  --- transmitted wave only absolut phase
        dflux = flux - (fT1->fPg(0)  - fR1->fPg(0));
        break;
      case 2:
        //reflected only absolut phase  --- transmitted wave with 2 phases
        dflux = flux - (fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0));
        break;
      case 3:
        //reflected only absolut phase  --- transmitted wave is absorbed
        if (TMath::Abs(fT1->fRfI.Im())<small) {
          dflux = flux + fR1->fPg(0);
        }
        else ToCheck = kFALSE;
        break;
      default:
        //impossible value of fT1->fPhases
        PrintWhich();
        gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","fT1->fPhases",fT1->fPhases,ClassName());
        fAnomaly = kTRUE;
        return;
      }//end switch (fT1->fPhases)
      break;
    case 2:
      //reflected wave elliptically polarized
      switch (fT1->fPhases) {
      case 0:
      case 1:
        //reflected wave with 2 phases  --- transmitted wave only absolut phase
        dflux = flux - (fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0));
        break;
      case 2:
        //reflected wave with 2 phases  --- transmitted wave with 2 phases
        dflux = flux -
          (fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0));
        break;
      case 3:
        //reflected wave with 2 phases  --- transmitted wave is absorbed
        if (TMath::Abs(fT1->fRfI.Im())<small) {
          dflux = flux + fR1->fPgn(0) + fR1->fPgp(0);
        }
        else ToCheck = kFALSE;
        break;
      default:
        //impossible value of fT1->fPhases
        PrintWhich();
        gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","fT1->fPhases",fT1->fPhases,ClassName());
        fAnomaly = kTRUE;
        return;
      }//end switch (fT1->fPhases)
      break;
    default:
      //reflected wave absorbed ? Impossible !
      PrintWhich();
      gCleanOut->MM(killthis,met,"Reflected wave absorbed",ClassName());
      fAnomaly = kTRUE;
      return;
    }//end switch (fR1->fPhases)
    if (ToCheck) {
      if (TMath::Abs(dflux)>msmall) {
        PrintWhich();
        gCleanOut->MMR(killthis,met,"Energy flux not conserved","dflux",dflux,ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }
  }//end if (TLit::Get()->GetTestCalc())
}//end IsoToIso
void TLitPhoton::IsoTsIso() {
  //Calculates the transition from an isotropic medium towards an isotropic
  //medium. There IS a thin slice in-between
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t msmall = 1.0e-6;
  const Double_t small  = 1.0e-8;
  const Double_t vsmall = 1.0e-10;
  const Double_t wsmall = 1.0e-11;
  const char *met = "IsoTsIso";
  TComplex I(zero,un);
  Double_t aa,dh1,dh2,TestIm;      //for intermadiate calculations
  TComplex ca,cb,cc,cd,ce,cf,cg; //for intermediate calculations
  TComplex csthtv;  //sin of angle of refraction inside thin slice. Is always real, but may be > 1.
  TComplex ccthtv;  //cos of angle of refraction inside thin slice. May be pure imaginary.
  Double_t thsd;    //width of thin slice in [cm]
  Double_t thsri;   //refraction index of thin slice, always real
  Double_t thsmu;   //magnetic permability of thin slice
  TComplex betat;   //= (nt/mut) / (nc/muc). Complex if nt complex.
  Double_t betac;   //= (nc/muc) / (ni/mui). Always real.
  TComplex phi1;    //phase inside thin slice due to width of thin slice. Imaginary if ccthtv imaginary.
  TComplex sphi1;   //sin of phi1
  TComplex cphi1;   //cos of phi1
  TComplex phi2;    //phase exit of thin slice due to width of thin slice. Complex in case of fT1->fRfI or fT1->fCtht complex.
  TComplex ephi2;   // = exp(+i*phi2)
  TComplex cnum1,cnum2; //the 2 complex numerators
  TComplex cden1;   //complex denominator
  TComplex tsa,tca,rsa,rca;   //T*sin(alf) T*cos(alf) R*sin(alf) R*cos(alf)

  fR1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare  = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare  = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fR1->fPhases = 0;           //initializes at 0 the number of phases
  fT1->fPhases = 0;           //initializes at 0 the number of phases
  fI->fRfI     = fRfIndexi;   //assigns to each wave its index of refraction
  fR1->fRfI    = fRfIndexi;
  fT1->fRfI    = fRfIndext;
  fR1->fCtht   = fI->fCtht;
  fR1->fStht   = fI->fStht;   //angle of reflection == angle of incidence : REAL
  //Fresnel or Descartes formula, between incident and transmitted
  fT1->fStht = (fI->fRfI.Re()*fI->fStht.Re())/fT1->fRfI;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht = TLitPhoton::CosFromSin(fT1->fStht);
  //Fresnel or Descartes formula, between incident and thin slice. Everything on
  //the right side of = is real, so csthtv will be real, but may be >1!
  thsri  = fNextLitMedium->GetTSIndex();
  thsmu  = fNextLitMedium->GetTSMu();
  thsd   = fNextLitMedium->GetTSWidth();
  csthtv = (fI->fRfI.Re()*fI->fStht.Re())/thsri;
  //either ccthtv is real (if csthtv<=1) or pure imaginary (if csthtv>1)
  ccthtv = TLitPhoton::CosFromSin(csthtv);
  //the 2 quotients of indices and magnetic permeability.
  betac  = (thsri*fPmui)/(fI->fRfI.Re()*thsmu);
  betat  = (fT1->fRfI*thsmu)/(thsri*fPmut);
  //  [fW] = ps-1   [thsd] = cm   [TLitPhys::Get()->C()] = cm/ps   ==> [aa] = 0
  aa     = (fW*thsd)/TLitPhys::Get()->C();
  phi1   = aa*thsri*ccthtv;
  phi2   = aa*fT1->fRfI*fT1->fCtht;
  sphi1  = TComplex::Sin(phi1);
  cphi1  = TComplex::Cos(phi1);
  ephi2  = TComplex::Exp(I*phi2);
  //Wave inside incident plane
  ce     = betac*betat*fI->fCtht.Re();
  cf     = betac*fI->fCtht.Re()*fT1->fCtht;
  cg     = betat*ccthtv*ccthtv;
  ca     = ce - fT1->fCtht;
  cb     = cf - cg;
  cc     = ce + fT1->fCtht;
  cd     = cf + cg;
  ce     = cphi1*ccthtv;
  cf     = I*sphi1;
  cnum1  = ce*ca+cf*cb;
  cden1  = ce*cc+cf*cd;
  cnum2  = deux*fI->fCtht.Re()*ccthtv*ephi2;
  rca    = (fI->fAE*fI->fWcaf*cnum1)/cden1;
  tca    = (fI->fAE*fI->fWcaf*cnum2)/cden1;
  //Wave normal to incident plane
  ce    = betac*betat*fT1->fCtht;
  cf    = betac*ccthtv*ccthtv;
  cg    = betat*fT1->fCtht*fI->fCtht.Re();
  ca    = fI->fCtht.Re() - ce;
  cb    = cg - cf;
  cc    = fI->fCtht.Re() + ce;
  cd    = cg + cf;
  ce    = cphi1*ccthtv;
  cf    = I*sphi1;
  cnum1 = ce*ca+cf*cb;
  cden1 = ce*cc+cf*cd;
  cnum2 = deux*fI->fCtht.Re()*ccthtv*ephi2;
  rsa   = (fI->fAE*fI->fWsaf*cnum1)/cden1;
  tsa   = (fI->fAE*fI->fWsaf*cnum2)/cden1;
  //
  //We calculate everything on the transmitted wave
  //
  fT1->fE  = TComplex::Sqrt(tsa*tsa+tca*tca);
//  fT1->fE.RPhi(fT1->fAE,fT1->fPE);
  TLitPhoton::RPhi(fT1->fE,fT1->fAE,fT1->fPE);
  if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
  else fT1->fPE = zero;
  fT1->fCalf = tca/fT1->fE;
  fT1->fSalf = tsa/fT1->fE;
//  fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
  TLitPhoton::RPhi(fT1->fCalf,fT1->fWcaf,fT1->fPcaf);
//  fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
  TLitPhoton::RPhi(fT1->fSalf,fT1->fWsaf,fT1->fPsaf);
  if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
  TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
    TMath::Abs(fT1->fRfI.Im());
  if (TestIm>wsmall) fT1->fPhases = 3;
  switch (fT1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the SAME phase. No reason to distinguish between the 2.
    //The transmitted wave is not elliptically polarized.
    fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (TLit::Get()->GetTestCalc()) {
      //If the phases are 0 or 1, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","Index",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
    fT1->fEf.SetXYZ(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUp(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUp(0) = dh1;
    }
    if (dh1<zero) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Transmitted wave going backward","dh1",dh1,ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fT1->fUk.SetXYZ(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (TLit::Get()->GetTestCalc()) {
      //If there are 2 phases, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gCleanOut->MMC(killthis,met,"Index unduly complex","fT1->fRfI",fT1->fRfI,ClassName());
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    //electric field
    fT1->fEfp.SetXYZ(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      zero);
    fT1->fEfn.SetXYZ(zero,zero,fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
    BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUpp(0);
    dh2 = fT1->fUpn(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUpp(0) = dh1;
    }
    if ((dh2<zero) && (dh2> -wsmall)) {
      dh2 = - dh2;
      fT1->fUpn(0) = dh2;
    }
    if ((dh1<zero) || (dh2<zero)) {
      PrintWhich();
      gCleanOut->MM(error,met,"Transmitted wave going backward",ClassName());
      gCleanOut->MRR(killthis,"dh1",dh1,"dh2",dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 3:
    //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
    //anything
    break;
  default:
    PrintWhich();
    gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","nphases",fT1->fPhases,ClassName());
    fAnomaly = kTRUE;
    return;
  }//end switch (fT1->fPhases)
  //
  //We calculate everything on the reflected wave
  //
  fR1->fE    = TComplex::Sqrt(rsa*rsa+rca*rca);
//  fR1->fE.RPhi(fR1->fAE,fR1->fPE);
  TLitPhoton::RPhi(fR1->fE,fR1->fAE,fR1->fPE);
  if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
  else fR1->fPE = zero;
  fR1->fCalf = rca/fR1->fE;
  fR1->fSalf = rsa/fR1->fE;
//  fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
  TLitPhoton::RPhi(fR1->fCalf,fR1->fWcaf,fR1->fPcaf);
//  fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
  TLitPhoton::RPhi(fR1->fSalf,fR1->fWsaf,fR1->fPsaf);
  if (TMath::Abs(fR1->fPcaf-fR1->fPsaf)>wsmall) fR1->fPhases +=  1;
  //A reflected wave is never absorbed !
  switch (fR1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The reflected wave is not elliptically polarized.
    //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
    //reflection angle is defined CONTRARY to the trigonometric sense. A reflection
    //angle EQUAL to the incidence angle implies a reflected wave having an
    //OPPOSITE x direction as the incident wave.
    fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
    fR1->fEf.SetXYZ(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUp(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUp(0) = dh1;
    }
    if (dh1>zero) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Reflected wave going forward","dh1",dh1,ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fR1->fUk.SetXYZ(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    //electric field
    fR1->fEfp.SetXYZ(-fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      -fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      zero);
    fR1->fEfn.SetXYZ(zero,zero,fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmut);
    BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmut);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUpp(0);
    dh2 = fR1->fUpn(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUpp(0) = dh1;
    }
    if ((dh2>zero) && (dh2<wsmall)) {
      dh2 = - dh2;
      fR1->fUpn(0) = dh2;
    }
    if ((dh1>zero) || (dh2>zero)) {
      PrintWhich();
      gCleanOut->MRR(error,"dh1",dh1,"dh2",dh2);
      gCleanOut->MM(killthis,met,"Reflected wave going forward",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    break;
  default:
    PrintWhich();
    gCleanOut->MMI(killthis,met,"Abnormal nb. of phases reflected wave","fR1->fPhases",fR1->fPhases,ClassName());
    fAnomaly = kTRUE;
    return;
  }//end switch (fR1->fPhases)
  //
  //  After all these complicate calculations, we have at hand an important check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved. Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  if (TLit::Get()->GetTestCalc()) {
    Bool_t ToCheck = kTRUE;
    Double_t flux,dflux;
    flux  = fI->fPg(0);
    dflux = flux;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //reflected wave not elliptically polarized
      switch (fT1->fPhases) {
      case 0:
      case 1:
        //reflected only absolut phase  --- transmitted wave only absolut phase
        dflux -= fT1->fPg(0)  - fR1->fPg(0);
        break;
      case 2:
        //reflected only absolut phase  --- transmitted wave with 2 phases
        dflux -= fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
        break;
      case 3:
        //reflected only absolut phase  --- transmitted wave is absorbed
        if (TMath::Abs(fT1->fRfI.Im())<small) {
          dflux += fR1->fPg(0);
        }
        else ToCheck = kFALSE;
        break;
      default:
        //impossible value of fT1->fPhases
        PrintWhich();
        gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","fT1->fPhases",fT1->fPhases,ClassName());
        fAnomaly = kTRUE;
        return;
      }//end switch (fT1->fPhases)
      break;
    case 2:
      //reflected wave elliptically polarized
      switch (fT1->fPhases) {
      case 0:
      case 1:
        //reflected wave with 2 phases  --- transmitted wave only absolut phase
        dflux -= fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0);
        break;
      case 2:
        //reflected wave with 2 phases  --- transmitted wave with 2 phases
        dflux -= fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
        break;
      case 3:
        //reflected wave with 2 phases  --- transmitted wave is absorbed
        if (TMath::Abs(fT1->fRfI.Im())<small) {
          dflux += fR1->fPgn(0) + fR1->fPgp(0);
        }
        else ToCheck = kFALSE;
        break;
      default:
        //impossible value of fT1->fPhases
        PrintWhich();
        gCleanOut->MMI(killthis,met,"Abnormal nb. of phases transmitted wave","fT1->fPhases",fT1->fPhases,ClassName());
        fAnomaly = kTRUE;
        return;
      }//end switch (fT1->fPhases)
      break;
    default:
      //reflected wave absorbed ? Impossible !
      PrintWhich();
      gCleanOut->MM(killthis,met,"Reflected wave absorbed",ClassName());
      fAnomaly = kTRUE;
      return;
    }//end switch (fR1->fPhases)
    if (ToCheck) {
      if (TMath::Abs(dflux)>msmall) {
        PrintWhich();
        gCleanOut->MMR(killthis,met,"Energy flux not conserved","dflux",dflux,ClassName());
        fAnomaly = kTRUE;
        return;
      }//end if (TMath::Abs(dflux)>msmall)
    }//end if (ToCheck)
  }//end if (TLit::Get()->GetTestCalc())
}
void TLitPhoton::Jump() {
  //  This routine moves the photon from its current position to its next position
  // depending on which OpticalProcess {kNoOpt=0, kAbsorption, kIsotropicScattering,
  // kRayleighScattering, kWLS, kSurfaceProcess} occurs. The routine first checks 
  // that the photon has not exceeded TLitPhys::Get()->TooLate(). If it has the photon
  // is killed.
  //
  //  Then, depending upon the OpticalProcess, either
  //
  //  (1) - Jump() finds the next place the photon will exit from the TGeoVolume and
  //        and face an other TGeoVolume, which can be a normal TGeoVolume or a
  //        wrapping. In that case, at the exit of Jump(), OnFace() will be called
  //        by Move(). The step done will just penetrate the new TGeoVolume.
  //  (2) - Jump() makes a finite step inside the TGeoVolume and executes the required
  //        OpticalProcess. The step done leaves the photon inside the same TGeoVolume.
  //        In that case, at the exit of Jump(), Jump() will be called again by Move(),
  //        again and again, until we face case (1).
  //  For each possible OpticalProcess, an interaction length is generated. The shortest
  // length generated out of all the processes is taken as the length of the next step
  // and defines the chosen OpticalProcess.
  //  If the TGeoVolume the photon is in is a detector, and if this detector is of kind
  // fStopAlways == true [ i.e. photon reaching this TGeoVolume immediatly stops in the
  // TGeoVolume ] then Jump() immediatly stops the photon and record it by a call to
  // InDetRecord().
  //  If the TGeoVolume the photon is in is a detector, and if this detector is of kind
  // fStopAlways == false [ i.e. photon only stops inside the detector because of its
  // finite absorption length ] then Jump() records the photon by a call to InDetRecord()
  // in case the photon stops because of finite absorption length by the OpticalProcess
  // kAbsorption.
  const Double_t verysmall = 1.0e-50;
  const Double_t *point;
  const char *met = "Jump";
  Int_t bin;
  Bool_t detfound = kFALSE;
  fProcess = kNoOpt;
  //  First check if photon is too late. If it is too late, do not perform Jump
  // This check is important as we have now left the possibility for materials to
  // have no absorption opening a potential infinite loop
  if (fT0>TLitPhys::Get()->TooLate()) {
    //The photon is too late
    gLitGs->fNpLossAny += 1;
    gLitGs->fNpTooLate += 1;
    bin = gLitGs->fHTooLate->Fill(fWaveLength);
    bin = gLitGs->fHAbsAny->Fill(fSl0);
    if (gLitCs) {
      gLitCs->fNpLossAny += 1;
      gLitCs->fNpTooLate += 1;
      bin = gLitCs->fHTooLate->Fill(fWaveLength);
      bin = gLitCs->fHAbsAny->Fill(fSl0);
    }
    fPhtLive = kFALSE;
    return;
  }//end if (fT0>TLitPhys::Get()->TooLate())
  //  Then checks that photon is not inside a medium with absorption length 0,
  // like is the case for an optical fibre. In that case, kills the photon and
  // register it into fNpAbsL0
  if (fAbsl0<verysmall) {
    gLitGs->fNpAbsL0 +=1;
    if (gLitCs) gLitCs->fNpAbsL0 +=1;
    fPhtLive = kFALSE;
    return;
  }
  //  If we are inside a detector, and if this detector has fStopAlways == true,
  // as is the case for example for a Phototube, then the photon stops here immediatly.
  // We have to register it.
  if (fLitVolume) {
    if (!fLitVolume->GetWCSdone()) fLitVolume->SetWCS();
    if (fLitVolume->GetDetKind() != none) detfound = Detector();
//    gCleanOut->MM(info,met,"Coordinates in WCS in TLitVolume calculated",ClassName());
    if ((fLitDetG) && (fLitVolume->GetStopAlways())) {
      fStopPoint = fX0;
      InDetRecord();
      if (!fPhtLive) return;
    }
  }
  //Find next interaction face, and the distance to travel to the face
  TGeoNode *node = 0;
  node   = gGeoManager->FindNextBoundary(TLitPhys::Get()->BiggestStep());
  if (!node) {
    PrintWhich();
    gCleanOut->MM(killthis,met,"FindNextBoundary failed",ClassName());
    fAnomaly = kTRUE;
    fPhtLive = kFALSE;
    return;
  }
  fNextDist     = gGeoManager->GetStep();
  fNextPoint    = fX0 + fNextDist*fUpg;
  fIsOnBoundary = kTRUE;
  fProcess      = kSurfaceProcess;
  //we calculates the length to travel [cm]
  FindProcessAndLength();
  if (fIsOnBoundary) {
    fNextNode  = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    fNextDist  = gGeoManager->GetStep();
    fNextPoint = fX0 + fNextDist*fUpg;
  }
  else {
    gGeoManager->SetStep(fNextDist);
    node = gGeoManager->Step(kFALSE,kFALSE);
    fNextDist = gGeoManager->GetStep();
    point = gGeoManager->GetCurrentPoint();
    fNextPoint.SetXYZ(point[0],point[1],point[2]);
  }
  fT0   += (fAk0*fNextDist*fCospk)/fW;  //Update elapsed time
  fSl0  += fNextDist;                   //Update space travelled by adding last step
  if (fProcess != kAbsorption) fSkl0 += fAk0*fNextDist;
  if (fProcess == kSurfaceProcess) {
    fX0 = fNextPoint;
    //Checks whether the photon is too late : not necessary to continue with photons
    //which in any case are too late to be detected !
    if (fT0>TLitPhys::Get()->TooLate()) {
      gLitGs->fNpLossAny += 1;
      gLitGs->fNpTooLate += 1;
      bin = gLitGs->fHTooLate->Fill(fWaveLength);
      bin = gLitGs->fHAbsAny->Fill(fSl0);
      if (gLitCs) {
        gLitCs->fNpLossAny += 1;
        gLitCs->fNpTooLate += 1;
        bin = gLitCs->fHTooLate->Fill(fWaveLength);
        bin = gLitCs->fHAbsAny->Fill(fSl0);
      }
      fPhtLive = kFALSE;
    }// end if (fT0>TLitPhys::Get()->TooLate())
  }
  else fX0 = fNextPoint;
  if (fProcess == kIsotropicScattering) {
    ++fNbOfDiffusion;
    Regenerate(kIsotropicScattering);
  }
  if (fProcess == kRayleighScattering) {
    ++fNbOfRayleigh;
    Regenerate(kRayleighScattering);
  }
  if (fProcess == kWLS) {
    ++fNbOfWLS;
    Regenerate(kWLS);
  }  
  if (fProcess == kAbsorption) {
    fPhtLive = kFALSE;
    if (fAbsl0<verysmall) {
      gLitGs->fNpAbsL0 +=1;
      if (gLitCs) gLitCs->fNpAbsL0 +=1;
    }
    else {
      //Check whether photon stopped in a sensitive material
      fNPoints++;
      if (fLitMedium->IsSensible()) {
        //Check whether photon is too late !
        if (fT0>TLitPhys::Get()->TooLate()) {
          //The photon is too late
          gLitGs->fNpLossAny += 1;
          gLitGs->fNpTooLate += 1;
          bin = gLitGs->fHTooLate->Fill(fWaveLength);
          bin = gLitGs->fHAbsAny->Fill(fSl0);
          if (gLitCs) {
            gLitCs->fNpLossAny += 1;
            gLitCs->fNpTooLate += 1;
            bin = gLitCs->fHTooLate->Fill(fWaveLength);
            bin = gLitCs->fHAbsAny->Fill(fSl0);
          }
        }
        else {  //if (fT0>TLitPhys::Get()->TooLate())
          //The photon is in time
          if (fLitDetG) {
            fLengthInDet = fNextDist;
            InDetRecord();
          }//end if (fLitDetG)
        } //if (fT0>TLitPhys::Get()->TooLate())
      } //end if (fLitMedium->IsSensible())
      else {
        //Here the photon is lost absorbed by non-sensitive material of a TGeoVolume
        Double_t amat0;
        amat0 = fLitMedium->GetMatCounterD();
        gLitGs->fNpLossAny += 1;
        gLitGs->fNpAbsMat  += 1;
        bin = gLitGs->fHAbsAny->Fill(fSl0);
        bin = gLitGs->fHAbsMat->Fill(amat0);
        if (gLitCs) {
          gLitCs->fNpLossAny += 1;
          gLitCs->fNpAbsMat  += 1;
          bin = gLitCs->fHAbsAny->Fill(fSl0);
          bin = gLitCs->fHAbsMat->Fill(amat0);
        }// end if (gLitCs)
      }// end else of if (fLitMedium->IsSensible())
    } // end if (fAbsl0<verysmall)
  }// end if (fProcess == kAbsorbed)
  if (fProcess == kNoOpt) gCleanOut->MM(error,met,"Bad value of fProcess",ClassName());
}
void TLitPhoton::MinOrthoN(TVector3 &x,const TVector3 &v,Double_t &s,Double_t &c) {
//  Change the vector x in the MINIMAL WAY, in order that it gets
//orthogonal to the vector v and that it has the length 1. On return,
//s and c are the sin and cos of the angle between vectors x (before it
//moves) and v. v is any vector, not necessary unitary.
//  It is to be noticed that this problem has not a unique solution when x
//and v are parallel ! It is up to you to have verified that x and v are
//not parallel. No warning message will be issued in case of the 2 vectors
//parallel. On return, you will still get a x vector which is orthogonal
//to v and unitary, BUT its direction in the plane orthogonal to v will
//be unpredictable (but always the same for the 2 same vectors). It will
//be in the plane formed by v and the axis corresponding to the smallest
//component of v. For instance if v.fV2 is the smallext component of the
//vector v, x will be in the plane (v,Oz).
//
  const Double_t zero      = 0.0;
  const Double_t un        = 1.0;
  const Double_t deux      = 2.0;
  const Double_t small     = 1.0e-5;
  const Double_t VerySmall = 1.0e-12;
  Short_t k;
  Double_t f,y,s2,c2;
  TVector3 w;
  TVector3 a(v);
  TVector3 b(x);
  a = a.Unit();
  b = b.Unit();
  w = a.Cross(b);
  s = w.Mag();
  if (s>VerySmall) {
    f = un/s;
    c = a*b;
    if (s>small) {
      x = f*(b - c*a);
    }
    else {
//if s is small, use a better way of handling cosine
      y  = (s*s)/deux;
      y -= y*y;
      TVector3 z;
      z  = b - a;
      x = f*(z + y*a);
      x = x.Unit();
//we make it a second time !
      b  = x;
      w  = a.Cross(b);
      s2 = w.Mag();
      f  = un/s2;
      c2 = a*b;
      x = f*(b - c2*a);
    }
  }
  else {
//    k = a.Smallest();
    k = TLitPhoton::Smallest(a);
    switch (k) {
    case 0:
      b.SetXYZ(un,zero,zero);
      break;
    case 1:
      b.SetXYZ(zero,un,zero);
      break;
    case 2:
      b.SetXYZ(zero,zero,un);
      break;
    }
    w = a.Cross(b);
    c = un;
    x = w;
  }
}
void TLitPhoton::Move() {
  //Stirring routine moving the photon inside the setup, from birth to death
  const Axis_t small = 1.0e-6;
  const char *met = "Move";
  Int_t bin;
  Axis_t a;
  while ((fPhtLive) && (!fAnomaly)) {
    if (!fIsOnBoundary) {
      do {
        fNPoints++;
        if (fToDraw) fCurrentTrack->AddPoint(fX0[0],fX0[1],fX0[2],fT0);
        Jump();
      }
      while (fProcess == kIsotropicScattering || fProcess == kRayleighScattering || fProcess == kWLS);
    }//end if (!fIsOnBoundary)
    if (fPhtLive) {
      if (fNPoints>fgNPointsMax) {
        fPhtLive = kFALSE;
        fAnomaly = kTRUE;
        gCleanOut->MM(killthis,met,"Too much interaction points on trajectory",ClassName());
      }
      if ((fPhtLive) && (!fAnomaly)) {
        // Here fIsOnBoundary must be true, and fNextNode must be defined
        if (!fIsOnBoundary)
          gCleanOut->MM(fatal,met,"fIsOnBoundary false at exit of Jump()",ClassName());
        if (!fNextNode)
          gCleanOut->MM(fatal,met,"fNextNode undefined at exit of Jump()",ClassName());
        OnFace();
      }//end if ((fPhtLive) && (!fAnomaly))
    }//end if (fPhtLive)
  }//end while ((fPhtLive) && (!fAnomaly))
  a = fNbOfDiffusion + small;
  bin = gLitGs->fHPoints->Fill(fNPoints);
  bin = gLitGs->fHDiffusion->Fill(a);
  if (gLitCs) {
    bin = gLitCs->fHPoints->Fill(fNPoints);
    bin = gLitCs->fHDiffusion->Fill(a);
  }
  if (fPhtSeen) {
    TLit::Get()->fSeen = kTRUE;
    TLit::Get()->fX0   = fX0(0);
    TLit::Get()->fY0   = fX0(1);
    TLit::Get()->fZ0   = fX0(2);
  }
  else TLit::Get()->fSeen = kFALSE;
  if (fAnomaly) {
    PrintWhich();
    fPhtLive = kFALSE;
    gLitGs->fNpLossAny++;
    gLitGs->fNpAbnorm++;
    bin = gLitGs->fHAbsAny->Fill(fSl0);
    bin = gLitGs->fHAbnLost->Fill(fSl0);
    if (gLitCs) {
      gLitCs->fNpLossAny++;
      gLitCs->fNpAbnorm++;
      bin = gLitCs->fHAbsAny->Fill(fSl0);
      bin = gLitCs->fHAbnLost->Fill(fSl0);
    }
  }//end if (fAnomaly)
  if (fToDraw) fCurrentTrack->AddPoint(fX0[0],fX0[1],fX0[2],fT0);
  ClearNewVolume(kTRUE);
  ClearVolume();
}
void TLitPhoton::OnFace() {
  //  OnFace is the steering routine to handle what has to be done when the
  // photon reaches a face of a TGeoVolume.
  //  Calling OnFace(), the track must be just in the next TGeoVolume, fNextGeoVolume,
  // not yet defined. But fNextNode must be defined.
  //  Finds the normal fFN to the face at point fX0, pointing outside the TGeoVolume
  // The dot product of the normal and the current direction is positive defined.
  const Double_t un   = 1.0;
  const char *met = "OnFace";
  const char *path;
  Double_t *normal;
  // Finds characteristics of encountered volume
  fNextGeoVolume = fNextNode->GetVolume();
  path = gGeoManager->GetPath();
  fNextPath = path;
  fNextLitVolume = (TLitVolume*)fNextGeoVolume->GetField();
  if (fNextLitVolume) {
    //  May be the fNextLitVolume will not be entered. In that case, it is not
    // necessary to enquire whether the new volume is a detector. We differ this
    // enquiry. But it is important to call fNextLitVolume->SetWCS(). In case of
    // anisotropy, the dielectric tensor must be ready to decide whether the photon
    // enters or not the new volume
    fNextLitVolume->SetWCS();
  }
  fNextGeoMedium = fNextGeoVolume->GetMedium();
  if (fNextGeoMedium->InheritsFrom("TLitMedium")) {
    fNextLitMedium = (TLitMedium*)fNextGeoMedium;
  }
  else fNextLitMedium = 0;
  if (fNextLitMedium) {
    //Is the next volume a wrapping?
    if (fNextLitMedium->IsWrapping()) fSinThetaMax = fNextLitMedium->GetSinThetaMax();
    else                              fSinThetaMax = un;
    // Finds the normal to the face at point fX0, pointing in such a way that
    // the dot product of the normal and the current direction is positive defined
    normal = gGeoManager->FindNormalFast();
    fFN.SetXYZ(normal[0],normal[1],normal[2]);
    fAnisi   = fAnis0;
    fAniai   = fAnia0;
    fPmui    = fPmu0;
    if (fAnisi) fDzti     = fDzt0;
    else        fRfIndexi = fRfIndex0;
    if (fNextLitMedium->IsWrapping()) {
      fBetween = kFALSE;
      AgainstRev();
    }
    else {
      fBetween = kTRUE;
      AgainstShape();
    }
    fIsOnBoundary = kFALSE;
  }//end if (fNextLitMedium)
  else {
    fAnomaly = kTRUE;
    fPhtLive = kFALSE;
    gCleanOut->MM(error,met,"Photon in front of a TGeoMedium which is not a TLitMedium",ClassName());
    gCleanOut->MM(error,met,"This should never happen",ClassName());
    gCleanOut->MM(killthis,met,"Photon killed",ClassName());
  }
}
void TLitPhoton::NewVolume() {
  // The photon definitely enters into new volume
  ClearVolume();
  fNode          = fNextNode;
  fGeoVolume     = fNextGeoVolume;
  fPath          = fNextPath;
  fLitVolume     = fNextLitVolume;
  fLitDet        = fNextLitDet;
  fLitDetG       = fNextLitDetG;
  fGeoMedium     = fNextGeoMedium;
  fLitMedium     = fNextLitMedium;
  ClearNewVolume(kFALSE);
  fOutPoly       = kFALSE;
  fBetween       = kFALSE;
}
void TLitPhoton::PrintWhich() const {
  //Print run number and photon number
  gCleanOut->MII(info,"photon",fKphot,"global photon nb",fgNphot);
}
void TLitPhoton::Regenerate(OpticalProcess process) {
  //  Regenerate a photon in the same position with the possibility of changing 
  // the wavelength and direction. Regenerate() should only be called for 
  // isotropic or Rayleigh scattering or wavelength shifting (WLS). 
  // WLS is the same as isotropic scattering but the photon changes wavelength. 
  // Rayleigh scattering has a preference for regeneration of photons in the 
  // forward and backward directions.
  // In isotropic scattering and wavelength shifting, the polarisation of the
  // photon is regenerated randomly.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t small = 1.0e-6;
  const char *met = "Regenerate";
  Double_t x = 0.0;
  Double_t y = 0.0;
  Double_t z = 0.0;
  Double_t theta,phi,st,ct,sf,cf,s,c,test;
  TVector3 Ox,Oy,Os;
  Double_t os1,os2,os3;
  Bool_t parallel;
  Double_t change;
  const TVector3 fAxisb1(un,zero,zero);
  const TVector3 fAxisb2(zero,un,zero);
  TMat33 M;
  switch (process) {
  case kWLS:
    Int_t kfluo;
    fLitMedium->DoWLS(fWaveLength,fT0,kfluo);
    fWLSL = fLitMedium->GetWLSL();
    //No break
  case kIsotropicScattering:
    x = gRandom3->Rndm();
    y = un - deux*x;
    theta = TLitPhys::ACos(y);
    phi   = deux*TMath::Pi()*gRandom3->Rndm();
    st = TMath::Sin(theta);
    ct = TMath::Cos(theta);
    sf = TMath::Sin(phi);
    cf = TMath::Cos(phi);
    x  = st*cf;
    y  = st*sf;
    z  = ct;
    fUk0.SetXYZ(x,y,z);
    if (!fAnis0) {
      s = TMath::Abs(fUk0*fAxisb1);
      c = TMath::Abs(fUk0*fAxisb2);
      if (s<=c) Ox = fAxisb1;
      else      Ox = fAxisb2;
      //      Ox.MinOrthoN(fUk0,s,c);
      TLitPhoton::MinOrthoN(Ox,fUk0,s,c);
      Oy = fUk0.Cross(Ox);
      Oy = Oy.Unit();
      if (TLit::Get()->GetTestCalc()) {
        test = TMath::Abs(un - Ox.Mag());
        if (test>small) gCleanOut->MM(error,met,"Ox not of length 1",ClassName());
        test = TMath::Abs(Ox*Oy);
        if (test>small) gCleanOut->MM(error,met,"Ox not orthogonal to Oy",ClassName());
        test = TMath::Abs(Ox*fUk0);
        if (test>small) gCleanOut->MM(error,met,"Ox not orthogonal to fUk0",ClassName());
        test = TMath::Abs(Oy*fUk0);
        if (test>small) gCleanOut->MM(error,met,"Oy not orthogonal to fUk0",ClassName());
      }
      // Est-ce vrai que la dispersion isotrope implique que la polarisation 
      // est nouvellement aleatoire?
      phi = deux*TMath::Pi()*gRandom3->Rndm();
      s   = TMath::Sin(phi);
      c   = TMath::Cos(phi);
      fEf0   = c*Ox + s*Oy;
      if (TLit::Get()->GetTestCalc()) {
        test = TMath::Abs(un - fEf0.Mag());
        if (test>small) gCleanOut->MM(error,met,"fEf0 not of length 1",ClassName());
        test = TMath::Abs(fEf0*fUk0);
        if (test>small) gCleanOut->MM(error,met,"fEf0 not orthogonal to fUk0",ClassName());
      }
    }
    else fEf0.SetXYZ(zero,zero,zero);
    break;
  case kRayleighScattering:
    if (!fAnis0) {
      //Find normal to direction of photon
      s = TMath::Abs(fUk0*fAxisb1);
      c = TMath::Abs(fUk0*fAxisb2);
      if (s<=c) Ox = fAxisb1;
      else      Ox = fAxisb2;
      TMat33 M(Ox,fUk0,parallel,change);
      Ox = M.GetLine(0);
      // Any normal to fUk0
      // We can do this with no loss of generality since we generate a random 
      // angle phi below.
      Double_t p1, p2, p3;
      p1 = fUk0(0);
      p2 = fUk0(1);
      p3 = fUk0(2);
      Double_t ox1, ox2, ox3;
      ox1 = Ox(0);
      ox2 = Ox(1);
      ox3 = Ox(2);
      // Find plane of scattering
      phi = deux*TMath::Pi()*gRandom3->Rndm();
      cf = TMath::Cos(phi);
      sf = TMath::Sin(phi);
      // Normal to fUk0 in plane of scattering
      os1 = (cf+(1.-cf)*p1*p1)*ox1  
        + ((-p3*sf)+(1.-cf)*p1*p2)*ox2 
        + ((p2*sf)+(1.-cf)*p1*p3)*ox3;
      os2 = ((p3*sf)+(1.-cf)*p1*p2)*ox1  
        + (cf+(1.-cf)*p2*p2)*ox2  
        + ((-p1*sf)+(1.-cf)*p2*p3)*ox3;
      os3 = ((-p2*sf)+(1.-cf)*p1*p3)*ox1 
        + ((p1*sf)+(1.-cf)*p2*p3)*ox2  
        + (cf+(1.-cf)*p3*p3)*ox3;
      Os.SetXYZ(os1,os2,os3); 
      Os = Os.Unit();
      if (TLit::Get()->GetTestCalc()) {
        test = TMath::Abs(Os*fUk0);
        if (test>small) gCleanOut->MM(error,met,"Os not orthogonal to fUk0",ClassName());
      }//end if (TLit::Get()->GetTestCalc())
      // Find polarisation wrt plane of scattering
      Double_t module = fEf0.Mag();
      fEf0 = fEf0.Unit();
      //      Double_t para = fEf0.VCos(Os);
      Double_t para = Os*fEf0;
      //      Double_t perp = fEf0.VSin(Os);
      TVector3 ccc;
      ccc = Os.Cross(fEf0);
      Double_t perp = ccc.Mag();
      //
      // Find scattering angle by acceptance rejection
      // (perp^2 + (para*ct)^2) comes from the x-section for Rayleigh Scattering
      // the factor of st comes from the infinitessimal solid angle
      theta = gRandom3->Rndm()*TMath::Pi();
      ct = TMath::Cos(theta);
      st = TMath::Sqrt(1-ct*ct);
      while (gRandom3->Rndm()> (perp*perp + para*para*ct*ct)*st) {
        theta = gRandom3->Rndm()*TMath::Pi();
        ct = TMath::Cos(theta);
        st = TMath::Sqrt(1-ct*ct);
      }//end while 
      TVector3 oldfUk0 = fUk0;
      // Rotate the k vector
      fUk0 = fUk0*ct + Os*st;
      fUk0 = fUk0.Unit();
      // Rotate and give amplitude to the polarisation vector
      fEf0 = (Os*ct - oldfUk0*st);
      if (TLit::Get()->GetTestCalc()) {
        test = TMath::Abs(fEf0*fUk0);
        if (test>small) gCleanOut->MM(error,met,"fEf0 not orthogonal to fUk0",ClassName());
      }//end if (TLit::Get()->GetTestCalc())
      fEf0 = module*fEf0;
    }//end if (!fAnis0)
    else {
      gCleanOut->MM(error,met,"Rayleigh Scattering not implemented in anistropic media",ClassName());
    }//end else if (!fAnis0)
    break;
  case kSurfaceProcess:
  case kNoOpt:
  case kAbsorption:
  default:
    gCleanOut->MM(fatal,met,"Regenerate called for wrong Optical Process",ClassName());
  }//end switch (process)
  StartPhoton();
}
void TLitPhoton::RPhi(TComplex z,Double_t &r,Double_t &phi) {
  //  Calculates module (with sign !) and phase of a complex number.
  // If the real part of the complex number is negativ, returns a negative module.
  // Advantage : in case of a negative real number, the module stays negative and
  // the phase 0. A negative real number is not considered as a non-real, complex
  // number with a phase of pi
  const Double_t zero   = 0.0;
  const Double_t wsmall = 1.0e-300;
  Double_t axr,axi;
  axr = TMath::Abs(z.Re());
  axi = TMath::Abs(z.Im());
  if ((axr<wsmall) && (axi<wsmall)) {
    r   = zero;
    phi = zero;
  }
  else {
    r   = TMath::Sqrt(z.Re()*z.Re()+z.Im()*z.Im());
    if (z.Re()<zero) r = -r;
    if (axi<wsmall) {
      phi = zero;
    }
    else {
      phi = atan2(z.Im(),z.Re());
      if (z.Re()<zero) {
        if (z.Im()>=zero) phi = phi - TMath::Pi();
        else phi = phi + TMath::Pi();
      }
    }
  }
}
void TLitPhoton::SetCurrentTrack(Int_t track_index) {
  // Get current track into fCurrentTrack in case track to be drawn
  gGeoManager->SetCurrentTrack(track_index);
  fCurrentTrack = gGeoManager->GetCurrentTrack();
}
void TLitPhoton::SliceToRev() {
  //  After the photon has been refracted out of the polyhedron into the slice
  // of material between face and wrapping, SliceToRev looks at what happens to
  // it when it hits the wrapping. Is it absorbed, diffused, or reflected ?
  //  First look whether there is a "pre-absorption" before hitting wrapping
  // ( dust ?).
  Int_t bin;
  Double_t y;
  switch (fNextLitMedium->IsSupplAbs()) {
  case 0:
    break;
  case 1:
    y = gRandom3->Rndm();
    if (y<fNextLitMedium->GetSupplAbs()) fPhtAbs = kTRUE;
    break;
  case 2:
    fPhtAbs = kTRUE;
    break;
  default:
    break;
  }
  if (fPhtAbs) {
    //Photon has been absorbed before the wrapping
    fPhtLive = kFALSE;
    Double_t arev1;
    arev1 = fNextLitMedium->GetRevCounterD();
    gLitGs->fNpLossAny += 1;
    gLitGs->fNpAbsBef  += 1;
    bin = gLitGs->fHAbsAny->Fill(fSl0);
    bin = gLitGs->fHAbsBef->Fill(arev1);
    if (gLitCs) {
      gLitCs->fNpLossAny += 1;
      gLitCs->fNpAbsBef  += 1;
      bin = gLitCs->fHAbsAny->Fill(fSl0);
      bin = gLitCs->fHAbsBef->Fill(arev1);
    }
  }// end if (fPhtAbs)
  else {
    //"In" characteristics"
    fAnisi    = 0;
    fAniai    = 0;
    fPmui     = fNextLitMedium->GetMu();
    fRfIndexi = fNextLitMedium->GetIndex();
    //"Out" characteristics"
    fAnist    = 0;
    fPmut     = fNextLitMedium->GetWrapMu();
    fRfIndext = fNextLitMedium->GetWrapIndex();
    fOx       = fFN;
    //decides whether the photon is reflected or diffused
    y = gRandom3->Rndm();
    if (y>fNextLitMedium->Diffus()) {
      //The photon is reflected
      Transition();
      if (!fAnomaly && fPhtAbs) {
        //Photon has been absorbed by the wrapping
        fPhtLive = kFALSE;
        gLitGs->fNpLossAny += 1;
        gLitGs->fNpAbsRvt  += 1;
        Double_t arev1 = fNextLitMedium->GetRevCounterD();
        bin = gLitGs->fHAbsAny->Fill(fSl0);
        bin = gLitGs->fHAbsRvt->Fill(arev1);
        if (gLitCs) {
          gLitCs->fNpLossAny += 1;
          gLitCs->fNpAbsRvt  += 1;
          bin = gLitCs->fHAbsAny->Fill(fSl0);
          bin = gLitCs->fHAbsRvt->Fill(arev1);
        }
      }// end if (!fAnomaly && fPhtAbs)
    }// end if (y>fRevetment->Diffus())
    else {
      //The photon is diffused
      Diffused();
    }// end if (y>fRevetment->Diffus())
  }// end else if (fPhtAbs)
}
void TLitPhoton::SliceToShape() {
  //  On return from the wrapping towards the face, decides whether the photon
  // is reflected back again towards the wrapping or reenters the TGeoVolume.
  //  "In" Characteristics :
  //  The "IN" medium is the slice between TGeoVolume not being a wrapping and
  // TGeoVolume being a wrapping. This slice is always isotropic.
  //  "Out" characteristics
  //  The OUT characteristics are those of the TGeoVolume the photon has
  // just leaved to hit the wrapping.
  const Double_t minus = -1.0;
  fAnisi    = 0;
  fAniai    = 0;
  fPmui     = fNextLitMedium->GetMu();
  fRfIndexi = fNextLitMedium->GetIndex();
  fAnist    = fAnis0;
  fAniat    = fAnia0;
  fPmut     = fPmu0;
  if (fAnist) fDztt     = fDzt0;
  else        fRfIndext = fRfIndex0;
  //Because coming back, normal has to be reversed.
  fOx = minus*fFN;
  Transition();
  if (!fAnomaly) fOutPoly = !fPhtOut;
}
Short_t TLitPhoton::Smallest(TVector3 v) {
//Finds which of the 3 components of vector v is the smallest in absolute value
  Short_t k;
  Double_t a0,a1,a2;
  a0 = TMath::Abs(v(0));
  a1 = TMath::Abs(v(1));
  a2 = TMath::Abs(v(2));
  if (a0<a1) {
    if (a0<a2) {
      k = 0;
    }
    else {
      k = 2;
    }
  }
  else {
    if (a1<a2) {
      k = 1;
    }
    else {
      k = 2;
    }
  }
  return k;
}
void TLitPhoton::StartPhoton() {
  // Finish to generate the photon
  const Double_t deux  = 2.0;
  const Double_t dixm7 = 1.0e-7;
  const Double_t dix12 = 1.0e+12;
  //calculates frequency [ps-1] and energy [Joules] of photon
  fW = (deux*TMath::Pi()*TLitPhys::Get()->C())/(dixm7*fWaveLength);
  fEphot = TLitPhys::Get()->Hbar()*fW*dix12;
  //material in which photon is
  if (fAnis0) FindDielAni();
  else        FindDielIso();
  if (!fAnomaly) {
    //We generate the phase of the linearly polarized photon
    fBeta = deux*TMath::Pi()*gRandom3->Rndm();
    //Magnetic Field and Poyinting vector in WCS
    BuildBPUP(fVk0,fEf0,fBf0,fPtg,fUpg,fPmu0);
    gGeoManager->SetCurrentDirection(fUpg[0],fUpg[1],fUpg[2]);
    fCospk   = fUk0*fUpg;
    fPhtLive = kTRUE;
    fOutPoly = kFALSE;
    fPhtSeen = kFALSE;
    fPhtAbs  = kFALSE;
    fPhtOut  = kFALSE;
    fOutPoly = kFALSE;
    if (fLitVolume) {
      if (fLitVolume->GetEmitPhoton()) {
        if (fLitVolume->GetFromFace()) {
          fIsOnBoundary = kTRUE;
          fNextNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
          fNextDist = gGeoManager->GetStep();
          fX0      += fNextDist*fUpg;
        }
      }
    }
  }//end if (!fAnomaly)
}
void TLitPhoton::ToICS() {
  //  ToICS finds the axis of the coordinate system ICS of the incident plane. It
  // also finds the transformation matrices fMn and fMt towards ICS.
  // The ICS coordinate system is defined like this :
  //
  // fOx is the normal to the hit face, pointing outside. fOx has to be defined
  //     before calling ToICS.
  // fOy is normal to fOx and is IN the plane containing fOx and the wave vector k
  //     of the photon. fOx,fOy build the incident plane. In addition, fOy is chosen
  //     in such a way that the scalar product of fOy with the k vector of the
  //     photon is positiv.
  // fOz is normal to fOx and fOy. fOx, fOy, fOz build a right orthonormal system
  //
  // fMn transforms an axis vector of WCS into an axis vector of ICS, so
  // fMn transforms a vector of ICS into a vector of WCS
  // fMt transforms an axis vector of ICS into an axis vector of WCS, so
  // fMt transforms a vector of WCS into a vector of ICS
  //
  const char *met       = "ToICS";
  const Double_t vsmall = 1.0e-10;
  TVector3 w;
  Double_t y,stht_i,ctht_i;
  ctht_i = fOx*fUk0;
  w      = fOx.Cross(fUk0);
  stht_i = w.Mag();
  fOz    = w.Unit();
  fOy    = fOz.Cross(fOx);
  y      = fOy*fUk0;
  if (y<0.0) {
    fOy = -fOy;
    fOz = -fOz;
  }
  fI->fStht = stht_i;
  fI->fCtht = ctht_i;
  if (TLit::Get()->GetTestCalc()) {
    Double_t y1,y2,y3;
    // Verify sin and cos
    y1 = TMath::Abs(stht_i*stht_i + ctht_i*ctht_i - 1.0);
    if (y1>vsmall) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad definition of sin and cos",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify fOx
    y1 = TMath::Abs(fOx*fOx - 1.0);
    y2 = TMath::Abs(fOx*fOy);
    y3 = TMath::Abs(fOx*fOz);
    if ((y1>vsmall) || (y2>vsmall) || (y3>vsmall)) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad Ox axis",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify fOy
    y1 = TMath::Abs(fOy*fOy - 1.0);
    y3 = TMath::Abs(fOy*fOz);
    if ((y1>vsmall) || (y3>vsmall)) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad Oy axis",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify fOz
    y1 = TMath::Abs(fOz*fOz - 1.0);
    if (y1>vsmall) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad Oz axis",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify orthonormality
    y1 = TMath::Abs(fOx*(fOy.Cross(fOz)) - 1.0);
    if (y1>vsmall) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"System not orthonormal",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify that Oy is indeed in the incident plane
    y1 = TMath::Abs(fUk0*fOz);
    if (y1>vsmall) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad definition of Oy",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    // Verify that scalar product of k with Oy is positiv
    y1 = fUk0*fOy;
    if (y1<0.0) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad angle of Oy with k",ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
  fMn.SetColumn(0,fOx(0),fOx(1),fOx(2));
  fMn.SetColumn(1,fOy(0),fOy(1),fOy(2));
  fMn.SetColumn(2,fOz(0),fOz(1),fOz(2));
  fMt.TransposedOf(fMn);
  if (TLit::Get()->GetTestCalc()) {
    TMat33 A;
    A = fMn*fMt;
    y = A.CloseUnit();
    if (y>vsmall) {
      PrintWhich();
      gCleanOut->MMR(killthis,met,"Transformation matrix fMn bad","Error",y,ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
}
Short_t TLitPhoton::Toss(Short_t nchoice) {
  // Chooses randomly according to weights fWeigth among nchoice possibility
  const Short_t  huit  = 8;
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-6;
  Short_t ichoice = -1;
  Double_t y;
  nchoice -= 1;
  nchoice %= huit;
  nchoice += 1;
  if (nchoice == 1) ichoice = 0;
  else {
    Double_t dd;
    Double_t zwhich[huit];
    Short_t k;
    Bool_t notabs = kFALSE;
    //We normalize the weigths
    dd = zero;
    for (k=0;k<nchoice;k++) dd += fWeigth[k];
    for (k=0;k<nchoice;k++) fWeigth[k] /= dd;
    //
    //  Following code is for taking into account the small error due to
    //finite precision of the machine in the case where one choice
    //corresponds to absorption and absorption is not allowed
    //( for instance in case of total reflection ). In that case,
    //fWeigth should be 0 but is not exactly 0 due to numerical imprecision,
    //and we put it exactly to 0.
    //
    for (k=0;k<nchoice;k++)
      if (fWeigth[k]<small) {
        notabs = kTRUE;
        fWeigth[k] = zero;
      }
      if (notabs) {
        dd = zero;
        for (k=0;k<nchoice;k++) dd += fWeigth[k];
        for (k=0;k<nchoice;k++) fWeigth[k] /= dd;
      }
      //Now comes the choice
      for (k=0;k<nchoice-1;k++) {
        if (k==0) zwhich[k] = fWeigth[k];
        else zwhich[k] = zwhich[k-1] + fWeigth[k];
      }
      for (k=nchoice-1;k<huit;k++) zwhich[k] = un;
      y = gRandom3->Rndm();
      if (y<=zwhich[3]) {
        if (y<=zwhich[1]) {
          if (y<=zwhich[0]) {
            ichoice = 0;
          }// if (y<=zwhich[0])
          else {
            ichoice = 1;
          }// end else if (y<=zwhich[0])
        }// if (y<=zwhich[1])
        else {
          if (y<=zwhich[2]) {
            ichoice = 2;
          }// if (y<=zwhich[2])
          else {
            ichoice = 3;
          }// end else if (y<=zwhich[2])
        }// end else if (y<=zwhich[1])
      }// if (y<=zwhich[3])
      else {
        if (y<=zwhich[5]) {
          if (y<=zwhich[4]) {
            ichoice = 4;
          }// if (y<=zwhich[4])
          else {
            ichoice = 5;
          }//end else if (y<=zwhich[4])
        }// if (y<=zwhich[5])
        else {
          if (y<=zwhich[6]) {
            ichoice = 6;
          }// if (y<=zwhich[6])
          else {
            ichoice = 7;
          }// end else if (y<=zwhich[6])
        }// end else if (y<=zwhich[5])
      }//end else if (y<=zwhich[3])
  }
  return ichoice;
}
void TLitPhoton::Transition() {
  // Stirring method handling transition between 2 media.
  // The fOx axis must already have been defined before calling Transition()
  //
  const Double_t zero   = 0.0;
  const Double_t vsmall = 1.0e-7;
  const Double_t wsmall = 1.0e-10;
  const char *met = "Transition";
  Double_t y;
  if (TLit::Get()->GetTestCalc()) {
    //tests that the photon is not backward
    y = fOx*fUpg;
    if (y<=zero) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Bad direction of photon",ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
  //Creates the coordinate system of the incident plane ICS
  ToICS();
  if (fAnomaly) return;
  fIsDiag  = kFALSE;
  fIsDiagi = kFALSE;
  fIsDiagt = kFALSE;
  //Transports the dielectric tensor associated with the incident wave into ICS
  //Make it absolutly symmetric and checks whether diagonal.
  if (fAnisi) {
    fDztii = fMt*fDzti*fMn;
    y = fDztii.Symmetrize();
    if (y<vsmall) {
      fIsDiag  = kTRUE;
      fIsDiagi = kTRUE;
      fDztii.Diagonalize();
    }
  }
  //Transports the dielectric tensor associated with the transmitted wave into ICS
  //Make it absolutly symmetric and checks whether diagonal.
  if (fAnist) {
    fDztti = fMt*fDztt*fMn;
    y = fDztti.Symmetrize();
    if (y<vsmall) {
      fIsDiag  = kTRUE;
      fIsDiagt = kTRUE;
      fDztti.Diagonalize();
    }
  }
  //We transport the photon into the ICS.
  fI->fUk = fMt*fUk0;
  fI->fEf = fMt*fEf0;
  //Verify that the k vector is inside the incident plane
  if (TLit::Get()->GetTestCalc()) {
    if (TMath::Abs(fI->fUk(2))>wsmall) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"k vector not in incident plane",ClassName());
        fAnomaly = kTRUE;
      return;
    }
  }//end if (TLit::Get()->GetTestCalc())
  fI->fUk(2) = zero;
  fI->fUk = fI->fUk.Unit();
  fI->fVk = fAk0*fI->fUk;
  //We make the angular decomposition of the electric field of the incident wave
  if (fAnisi) fI->Split(kTRUE);
  else        fI->Split(kFALSE);
  //Defines B field, Poyinting vector and unit Poyinting vector
  BuildBPUP(fI->fVk,fI->fEf,fI->fBf,fI->fPg,fI->fUp,fPmui);
  //Verifies that the incident wave goes forward !
  if (fI->fUp(0)<zero) {
    PrintWhich();
    if (fI->fUp(0)<-wsmall) {
      gCleanOut->MM(killthis,met,"Incident wave going back",ClassName());
      fAnomaly = kTRUE;
      return;
    }
    else {
      fI->fUp(0) = -fI->fUp(0);
      fI->fPg(0) = -fI->fPg(0);
      gCleanOut->MMR(warning,met,"Incident wave slightly backward","x coord",fI->fUp(0),ClassName());
    }
  }//end if (fI->fUp(0)<zero)
  //List of verifications
  if (TLit::Get()->GetTestCalc()) {
    //Verify that the angle decomposition of the E field is ok
    fAnomaly = fI->VerifySplit();
    if (fAnomaly) {
      gCleanOut->MM(killthis,met,"VerifySplit returns bad",ClassName());
      return;
    }
    //Verify, if anisotropy, that initial conditions ok
    if (fAnisi) {
      fAnomaly = fI->VerifyStart(fI->fRfI.Re(),fDztii);
      if (fAnomaly) {
        gCleanOut->MM(killthis,met,"VerifyStart returns bad",ClassName());
        return;
      }
    }
  }//end if (TLit::Get()->GetTestCalc())
  //Here come all calculations about the transition !
  switch (fAnisi) {
  case 0:
    switch (fAnist) {
    case 0:
      if (fLitMedium->IsThinSlice(fNextLitMedium)) IsoTsIso();
      else                                         IsoToIso();
      break;
    default:
      IsoToAni();
      break;
    }//end switch (fAnist)
    break;
  default:
    switch (fAnist) {
    case 0:
      AniToIso();
      break;
    default:
      AniToAni();
      break;
    }//end switch (fAnist)
    break;
  }//end switch (fAnisi)
  //Now we have to choose the wave which will be taken by the photon
  Choose();
  if (fAnomaly) return;
  fPhtLive = !fPhtAbs;
  //Keep the phase between 0 and 2pi.
  if (fBeta>TMath::TwoPi()) fBeta -= TMath::TwoPi();
  else if (fBeta<zero)      fBeta += TMath::TwoPi();
  //Return from the coord. system of the incident plane (ICS) to WCS
  if (fPhtLive) ICStoWCS();
}
void TLitPhoton::VerifyAtoI() {
  //  Verify that system of 13 equations has been correctly solved for the case
  // anisotropic to isotropic
  const Double_t small = 2.0e-5;
  const char *met = "VerifyAtoI";
  Short_t k;
  Double_t dd;
  TComplex zh1;
  TComplex ceps[13];
  TComplex nr2,nv2;
  TComplex str2,ctr2,sctr,stv2,ctv2,sctv;
  Double_t d00,d01,d02,d11,d12,d22;
  d00 = fDztii(0,0);
  d01 = fDztii(0,1);
  d02 = fDztii(0,2);
  d11 = fDztii(1,1);
  d12 = fDztii(1,2);
  d22 = fDztii(2,2);
  nr2 = fR1->fRfI*fR1->fRfI;
  nv2 = fR2->fRfI*fR2->fRfI;
  str2 = fR1->fStht*fR1->fStht;
  ctr2 = fR1->fCtht*fR1->fCtht;
  sctr = fR1->fStht*fR1->fCtht;
  stv2 = fR2->fStht*fR2->fStht;
  ctv2 = fR2->fCtht*fR2->fCtht;
  sctv = fR2->fStht*fR2->fCtht;
  //Here are the 13 equations of system (E1) :
  zh1      = fI->fRfI*fI->fStht;
  ceps[0]  = fR1->fRfI*fR1->fStht - zh1;
  ceps[1]  = fR2->fRfI*fR2->fStht - zh1;
  ceps[2]  = fT1->fRfI*fT1->fStht - zh1;
  //
  ceps[3]  = (nr2*str2-d00)*fR1->fSdel*fR1->fCalf +
    (nr2*sctr-d01)*fR1->fCdel*fR1->fCalf +
    d02*fR1->fSalf;
  ceps[4]  = (nr2*sctr-d01)*fR1->fSdel*fR1->fCalf +
    (nr2*ctr2-d11)*fR1->fCdel*fR1->fCalf +
    d12*fR1->fSalf;
  ceps[5]  = d02*fR1->fSdel*fR1->fCalf + d12*fR1->fCdel*fR1->fCalf +
    (nr2-d22)*fR1->fSalf;
  //
  ceps[6]  = (nv2*stv2-d00)*fR2->fSdel*fR2->fCalf +
    (nv2*sctv-d01)*fR2->fCdel*fR2->fCalf +
    d02*fR2->fSalf;
  ceps[7]  = (nv2*sctv-d01)*fR2->fSdel*fR2->fCalf +
    (nv2*ctv2-d11)*fR2->fCdel*fR2->fCalf +
    d12*fR2->fSalf;
  ceps[8]  = d02*fR2->fSdel*fR2->fCalf + d12*fR2->fCdel*fR2->fCalf +
    (nv2-d22)*fR2->fSalf;
  //
  ceps[9]  = fBI*fI->fE*fI->fCalf*fI->fAlm1 +
    fBR1*fR1->fE*fR1->fCalf*fR1->fAlm1 +
    fBR2*fR2->fE*fR2->fCalf*fR2->fAlm1 - fT1->fE*fT1->fCalf;
  ceps[10] = fI->fE*fI->fSalf + fR1->fE*fR1->fSalf + fR2->fE*fR2->fSalf -
    fT1->fE*fT1->fSalf;
  ceps[11] = fI->fE*fI->fCdel*fI->fCalf    - fR1->fE*fR1->fCdel*fR1->fCalf -
    fR2->fE*fR2->fCdel*fR2->fCalf - fT1->fE*fT1->fCtht*fT1->fCalf;
  ceps[12] = fBI*fI->fE*fI->fSalf*fI->fCtht -
    fBR1*fR1->fE*fR1->fSalf*fR1->fCtht -
    fBR2*fR2->fE*fR2->fSalf*fR2->fCtht -
    fT1->fE*fT1->fSalf*fT1->fCtht;
  for (k=0;k<13;k++) {
    dd = TComplex::Abs(ceps[k]);
    if (dd>small) {
      PrintWhich();
      gCleanOut->MM(error,met,"This equation has a bad solution",ClassName());
      gCleanOut->MIR(error,"k",k,"dd",dd);
    }
    if (dd>10*small) {
      gCleanOut->MM(killthis,met,"Photon will be killed",ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
}
void TLitPhoton::VerifyItoA() {
  //Verify that system of 13 equations has been correctly solved for the case
  //isotropic to anisotropic
  const Double_t small = 2.0e-5;
  const char *met = "VerifyItoA";
  Short_t k;
  Double_t dd;
  TComplex ceps[12];
  TComplex zh1,nt2,nv2,stt2,ctt2,sctt,stv2,ctv2,sctv;
  Double_t d00,d01,d02,d11,d12,d22;
  d00     = fDztti(0,0);
  d01     = fDztti(0,1);
  d02     = fDztti(0,2);
  d11     = fDztti(1,1);
  d12     = fDztti(1,2);
  d22     = fDztti(2,2);
  zh1     = fI->fRfI*fI->fStht;
  nt2     = fT1->fRfI*fT1->fRfI;
  nv2     = fT2->fRfI*fT2->fRfI;
  stt2    = fT1->fStht*fT1->fStht;
  ctt2    = fT1->fCtht*fT1->fCtht;
  sctt    = fT1->fStht*fT1->fCtht;
  stv2    = fT2->fStht*fT2->fStht;
  ctv2    = fT2->fCtht*fT2->fCtht;
  sctv    = fT2->fStht*fT2->fCtht;
  ceps[0] = fT1->fRfI*fT1->fStht - zh1;
  ceps[1] = fT2->fRfI*fT2->fStht - zh1;
  //
  ceps[2] = (nt2*stt2-d00)*fT1->fSdel*fT1->fCalf +
    (nt2*sctt+d01)*fT1->fCdel*fT1->fCalf +
    d02*fT1->fSalf;
  ceps[3] = (nt2*sctt+d01)*fT1->fSdel*fT1->fCalf +
    (nt2*ctt2-d11)*fT1->fCdel*fT1->fCalf -
    d12*fT1->fSalf;
  ceps[4] = d02*fT1->fSdel*fT1->fCalf - d12*fT1->fCdel*fT1->fCalf +
    (nt2-d22)*fT1->fSalf;
  //
  ceps[5] = (nv2*stv2-d00)*fT2->fSdel*fT2->fCalf +
    (nv2*sctv+d01)*fT2->fCdel*fT2->fCalf +
    d02*fT2->fSalf;
  ceps[6] = (nv2*sctv+d01)*fT2->fSdel*fT2->fCalf +
    (nv2*ctv2-d11)*fT2->fCdel*fT2->fCalf -
    d12*fT2->fSalf;
  ceps[7] = d02*fT2->fSdel*fT2->fCalf - d12*fT2->fCdel*fT2->fCalf +
    (nv2-d22)*fT2->fSalf;
  //
  ceps[8] = fI->fE*fI->fCalf + fR1->fE*fR1->fCalf -
    fBT1*fT1->fE*fT1->fCalf*fT1->fAlm1 -
    fBT2*fT2->fE*fT2->fCalf*fT2->fAlm1;
  ceps[9] = fI->fE*fI->fSalf   + fR1->fE*fR1->fSalf -
    fT1->fE*fT1->fSalf - fT2->fE*fT2->fSalf;
  ceps[10]= fI->fCtht*(fI->fE*fI->fCalf - fR1->fE*fR1->fCalf) -
    fT1->fE*fT1->fCdel*fT1->fCalf -
    fT2->fE*fT2->fCdel*fT2->fCalf;
  ceps[11]= fI->fCtht*(fI->fE*fI->fSalf - fR1->fE*fR1->fSalf) -
    fBT1*fT1->fE*fT1->fCtht*fT1->fSalf -
    fBT2*fT2->fE*fT2->fCtht*fT2->fSalf;
  for (k=0;k<12;k++) {
    dd = TComplex::Abs(ceps[k]);
    if (dd>small) {
      PrintWhich();
      gCleanOut->MM(error,met,"This equation has a bad solution",ClassName());
      gCleanOut->MIR(error,"k",k,"dd",dd);
    }
    if (dd>10*small) {
      PrintWhich();
      gCleanOut->MM(killthis,met,"Photon will be killed",ClassName());
      fAnomaly = kTRUE;
      return;
    }
  }
}
