// @(#)root/html:$Id: TLitranino.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TROOT.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TGeoVolume.h"
#include "TVirtualGeoTrack.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLitPhys.h"
#include "TLit.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLitDetector.h"
#include "TLitResults.h"
#include "TLitMarrow.h"
#include "TLitranino.h"

Int_t TLitranino::fgNPointsMax = 10000;

ClassImp(TLitranino)
//______________________________________________________________________________
//
//  Simplest possible particle to test SLitrani
//
//
//  There is only one static variable in this class that the user may be tempted
// to change: TLitranino::fgNPointsMax which is equal by default to 10000. It is
// the maximum number of interactions allowed for a litranino. If the user has a setup
// where the litranino can live very very long, may be the default of 10000 has to be
// increased. No problem, TLitPhoton::fgNPointsMax is a public static variable.
//
TLitranino::TLitranino(const char *name,const char *title,const char *path,
  TLitVolume *start,Double_t maxlength):TNamed(name,title) {
// Constructor giving source, direction and solid angle of emission.
//
// Arguments:
//
//  name      : name of this litranino generation
//  title     : title of this litranino generation
//  path      : path name of the TGeoVolume inside which litranino is generated
//              for instance "/TOP_1/REV_1/FIBT_1/FIB_1"
//  start     : TLitVolume from which litranino starts
//  maxlength : maximum length of path for the litraninos before dying
//
  const char *met = "TLitranino";
  Init();
  fSourcePath  = path;
  fStartLitVol = start;
  fMaxLength   = maxlength;
  if (!TLitResults::fgResults) TLitResults::fgResults = new TObjArray();
  if (!gLitGs) gLitGs = new TLitResults("GlobStat","Global statistics of all runs",0);
  gCleanOut->MMR(info,met,"maximum path length for litranino","fMaxLength",fMaxLength,ClassName());
}
TLitranino::~TLitranino() {
//destructor
}
void TLitranino::AgainstRev() {
  // Facing an other shape filled with a TLitMedium which is a wrapping,
  //a litranino is reflected back with an angle of reflection equal to
  //the angle of incidence. The litranino CAN be absorbed by the wrapping
  //if this wrapping has a supplementary absorption.
  const Short_t  ntry = 100;
  const Double_t eps  = 1.0e-5;
  const char *met = "AgainstRev";
  const Double_t *point;
  Bool_t HasCrossed;
  Short_t ktry;
  Double_t ak1,ak2;
  gCleanOut->MM(info,met,"Entering AgainstRev",ClassName());
  gCleanOut->MMR(info,met,"Direction nx","nx",fCurrentDir.X(),ClassName());
  gCleanOut->MMR(info,met,"Direction ny","ny",fCurrentDir.Y(),ClassName());
  gCleanOut->MMR(info,met,"Direction nz","nz",fCurrentDir.Z(),ClassName());
  // Check if absorbed by wrapping. For a litranino, only supplementary absorption
  //matters !
  ak1 = fLitMedium->GetMatCounterD();
  ak2 = fNextLitMedium->GetRevCounterD();
  if (gRandom3->Rndm()<fNextLitMedium->GetSupplAbs()) {
    // Here litranino is absorbed by wrapping
    fStopPoint = fCurrentPoint;
    gLitGs->fHMatRev->Fill(ak1,ak2);
    if (gLitCs) gLitCs->fHMatRev->Fill(ak1,ak2);
    InRevRecord();
  }
  else {
    // Here litranino is NOT absorbed by wrapping and returns into source shape
    gLitGs->fHRevMat->Fill(ak2,ak1);
    if (gLitCs) gLitCs->fHRevMat->Fill(ak2,ak1);
    Double_t ctheta = fCurrentDir*fFN;
    fCurrentDir    -= 2.0*ctheta*fFN;
    fCurrentDir = fCurrentDir.Unit();
    gCleanOut->MM(info,met,"After reflection :",ClassName());
    gCleanOut->MMR(info,met,"Direction nx","nx",fCurrentDir.X(),ClassName());
    gCleanOut->MMR(info,met,"Direction ny","ny",fCurrentDir.Y(),ClassName());
    gCleanOut->MMR(info,met,"Direction nz","nz",fCurrentDir.Z(),ClassName());
    gGeoManager->SetCurrentDirection(fCurrentDir.X(),fCurrentDir.Y(),fCurrentDir.Z());
    fNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    gCleanOut->MM(info,met,"Returning into original node",ClassName());
    fStep = gGeoManager->GetStep();
    gCleanOut->MMR(info,met,"Step to be done","fStep",fStep,ClassName());
    fLength += fStep;
    fT0     += fStep/TLitPhys::Get()->C();
    HasCrossed = (fNode != fNextNode);
    if (!HasCrossed) {
      gCleanOut->MM(error,met,"Step has not crossed boundary",ClassName());
      ktry  = 0;
      fStep = 0.0;
      while ((!HasCrossed) && (ktry<ntry)) {
        ktry++;
        fStep   += eps;
        gGeoManager->SetStep(fStep);
        fNode = gGeoManager->Step(kTRUE,kTRUE);
        HasCrossed = (fNode != fNextNode);
      }
      if (!HasCrossed) {
        gCleanOut->MM(killthis,met,"Impossible to cross boundary",ClassName());
        fAnomaly = kTRUE;
        fPhtLive = kFALSE;
      }
    }
    point = gGeoManager->GetCurrentPoint();
    fCurrentPoint.SetXYZ(point[0],point[1],point[2]);
    fNextNode      = 0;
    fNextGeoVolume = 0;
    fNextPath      = "";
    fNextLitVolume = 0;
    fNextLitDet    = 0;
    fNextLitDetG   = 0;
    fNextGeoMedium = 0;
    fNextLitMedium = 0;
  }
}
void TLitranino::AgainstShape() {
  // Facing an other shape filled with a TLitMedium which is not a wrapping,
  //a litranino simply continues in a straight line, which simply means
  //returning into method Move(), which will call Jump(). No chance to fill
  //histogram fHMatNot!
  const char *met = "AgainstShape";
  Double_t ak1,ak2;
  gCleanOut->MM(info,met,"We continue in a straight line",ClassName());
  if (fLitMedium->IsThinSlice(fNextLitMedium))
    gCleanOut->MM(info,met,"There is a thin slice between the 2 shapes",ClassName());
  else
    gCleanOut->MM(info,met,"No thin slice between the 2 shapes",ClassName());
  ak1 = fLitMedium->GetMatCounterD();
  ak2 = fNextLitMedium->GetMatCounterD();
  gLitGs->fHMatMat->Fill(ak1,ak2);
  if (gLitCs) gLitCs->fHMatMat->Fill(ak1,ak2);
  fNode          = fNextNode;
  fGeoVolume     = fNextGeoVolume;
  fPath          = fNextPath;
  fLitVolume     = fNextLitVolume;
  fLitDet        = fNextLitDet;
  fLitDetG       = fNextLitDetG;
  fGeoMedium     = fNextGeoMedium;
  fLitMedium     = fNextLitMedium;
  fNextNode      = 0;
  fNextGeoVolume = 0;
  fNextPath      = "";
  fNextLitVolume = 0;
  fNextLitDet    = 0;
  fNextLitDetG   = 0;
  fNextGeoMedium = 0;
  fNextLitMedium = 0;
}
void TLitranino::AllTracksToDraw() {
  // Call this method if you want all tracks to be drawn
  fTrackNb = 1000000;
}
Bool_t TLitranino::Detector() {
  // Finds the current detectors, global and per run, if they exist. If not
  //create them! By current, we mean those connected with fLitVolume and
  //fPath
  Bool_t ok1 = kFALSE;
  Bool_t ok2 = kFALSE;
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
Bool_t TLitranino::DetectorN() {
  // Finds the next detectors, global and per run, if they exist. If not
  //create them! By next, we mean those connected with fNextLitVolume and
  //fNextPath
  Bool_t ok1 = kFALSE;
  Bool_t ok2 = kFALSE;
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
void TLitranino::Gen(Int_t run,Int_t N,Double_t xparam,Bool_t runstat,Bool_t forgetlast) {
  // Generates run with N litraninos. The parameters are the following :
  // 
  // run        : run number. Arbitrary, but has to be greater than 0.
  // 
  // N          : number of litraninos to be generated in this run.
  // 
  // xparam     : this parameter is used as abscissa in the plotting of the
  //                results by the class TLitMarrow. For instance, if you
  //                have a setup with a crystal and optical fibre and you
  //                make 10 runs varying the incident angle of the fibre,
  //                you can choose the incident angle of the fibre as xparam.
  //                All plots provided by TLitMarrow will then have the
  //                incident angle of the fibre as x coordinate. You will get
  //                efficiency versus incident angle, and so on. If you do not
  //                give xparam, or give a value smaller than -1.0e+20, all
  //                plots of TLitMarrow will have the run number as x coord.
  // 
  //   runstat and forgetlast :
  // 
  //     if runstat == true [Default] AND forgetlast == false [Default] :
  // 
  //       in memory   : this run statistics keeped but this run histos deleted
  //                      when next run begins
  //       on the file : this run statistics and histos recorded
  // 
  //             It is the only configuration which allows the working of
  //           TLitMarrow, i.e. allowing to show histograms of quantities
  //           varying as a function of a run dependent parameter.
  //           Usage : normal case : try it first.
  // 
  //     if runstat == true AND forgetlast == true :
  // 
  //       in memory   : this run statistics and histograms deleted when next
  //                      run begins.
  //       on the file : this run statistics and histos recorded
  // 
  //           Disadvantage : TLitMarrow not working, gLitGp pointer unavailable
  //           Advantage    : no increase of memory with runs
  //                          per run histograms still available on file
  //           Usage : use this configuration when your number of runs is big
  //                   but you still want per run statistics and histograms
  //                   on file.
  // 
  //     if runstat == false ( forgetlast true or false ) :
  // 
  //       in memory   : no statistics and histograms for this run
  //       on the file : no statistics and histograms for this run
  // 
  //           Disadvantage : TLitMarrow not working, gLitGp pointer unavailable
  //                          per run statistics and histograms not available
  //           Advantage    : no increase of memory with runs
  //                          a bit faster, half less histograms to fill
  //           Usage : use this configuration when your number of runs is very
  //                   big and you are not interested in the per run statistics
  //                   and histograms.
  // 
  //   Notice that in any case, the global statistics and histograms for all
  // runs is always present in memory and on the file. This global statistics
  // is an object of class TLitResults, pointed to by the pointer gLitGs.
  // 
  const Int_t    pdg = 22;      //Nb given to gamma by the PDG (irrelevant)
  const char *met = "Gen";
  Bool_t ok;
  Color_t color = 0;
  Int_t kTrack;
  Bool_t IsgGpok;
  Bool_t IsLitMedium;
  Double_t x,y,z,nx,ny,nz;
  TLitPhys::Book();
  fRunStat    = runstat;
  fForgetLast = forgetlast;
  if (run <= 0) gCleanOut->MM(fatal,met,"Run number must be >= 1",ClassName());
  fRun = run;
  fFullName  = GetName();
  fFullTitle = GetTitle();
  TString srun = "";
  if (fRun<10000) srun.Append('0');
  if (fRun<1000) srun.Append('0');
  if (fRun<100) srun.Append('0');
  if (fRun<10) srun.Append('0');
  srun += fRun;
  fFullName.Append('_');
  fFullName.Append(srun);
  fFullTitle.Append(" run ");
  fFullTitle.Append(srun);
  //initializes the summary statistics of class TLitMarrow
  IsgGpok = (runstat && (!fForgetLast));
  if (!gLitGp && IsgGpok) gLitGp = new TLitMarrow("Summary","Runs",IsgGpok);
  if (gLitCs) {
    // If gLitCs is different from 0 here, it is the gLitCs of the previous run
    if (fForgetLast) {
      // If fForgetLast is true, the gLitCs of the previous run is removed from
      //the collection and then deleted. The content of TLitDetector::fgLitDetRun
      //is also deleted. No increase of memory used from run to run, but statistics
      //calculation at the end only possible by reading the ROOT file
      TLitResults::fgResults->Remove(gLitCs);
      delete gLitCs;
      gLitCs = 0;
    }
    else {
      // If fForgetLast is false, the variable containing the statistics, like
      //fNpGener,fNpSeen,fNpLossAny,...and so on are preserved, but all histograms
      //are deleted. It is also the case for the detectors in TLitDetector::fgLitDetRun:
      //statistics variable preserved, but histograms deleted. gLitCs of the previous
      //run remains without its histograms inside collection TLitResults::fgResults,
      //allowing statistics calculations at the end of all runs, without opening
      //the ROOT file
      gLitCs->DelHistos();
    }
  }
  gLitCs = 0;
  //Books a new gLitCs for this run. A new collection of detectors
  //TLitDetector::fgLitDetRun will also be booked
  if (fRunStat) gLitCs = new TLitResults(fFullName.Data(),fFullTitle.Data(),fRun,xparam);
  //Open the .root file for writing, if not yet done
  if (TLit::Get()->fFilesClosed) {
    TLit::Get()->OpenFilesW(runstat);
    gROOT->cd();
    if ((!runstat) && (gLitCs)) {
      delete gLitCs;
      gLitCs = 0;
    }
  }
  fRecordedTracks = 0;
  fLengthMax = 0.0;
  for (kTrack=0;kTrack<N;kTrack++) {
    gCleanOut->RunEv(fRun,kTrack);
    gLitGs->fNpGener++;
    if (gLitCs) gLitCs->fNpGener++;
    fLength = 0.0;
    fT0     = 0.0;
    fToDraw = (fTrackNb == kTrack) || (fTrackNb >= 1000000);
    gCleanOut->MM(info,met,"",ClassName());
    gCleanOut->MII(info,"run",fRun,"Litranino",kTrack);
    if (gCleanOut->Period2()) gCleanOut->MII(info,"run",fRun,"Litranino",kTrack);
    fNPoints = 0;
    fWaveLength = TLitPhys::Get()->MinWaveL() + (TLitPhys::Get()->MaxWaveL()-TLitPhys::Get()->MinWaveL())*gRandom3->Rndm();
    TLitMedium::NewWavelengthAll(fWaveLength);
    TLitVolume::NewWavelengthAll(fWaveLength);
    ok = fStartLitVol->Gen(fSourcePath.Data());
    fCurrentPoint = fStartLitVol->GetWCSSourcePoint();
    fCurrentDir   = fStartLitVol->GetWCSSourceDir();
    x = fCurrentPoint.X();
    y = fCurrentPoint.Y();
    z = fCurrentPoint.Z();
    gCleanOut->MMR(info,met,"Coordinate x","x",x,ClassName());
    gCleanOut->MMR(info,met,"Coordinate y","y",y,ClassName());
    gCleanOut->MMR(info,met,"Coordinate z","z",z,ClassName());
    if (fToDraw) {
      color++;
      if (color>9) color = 1;
      fRecordedTracks++;
      fTrackIndex   = gGeoManager->AddTrack(kTrack,pdg);
      fCurrentTrack = gGeoManager->GetTrack(fTrackIndex);
      fCurrentTrack->SetLineColor(color);
      gGeoManager->SetCurrentTrack(fCurrentTrack);
      fCurrentTrack->AddPoint(x,y,z,fLength/TLitPhys::Get()->Cmks());
    }
    nx         = fCurrentDir[0];
    ny         = fCurrentDir[1];
    nz         = fCurrentDir[2];
    gCleanOut->MMR(info,met,"X direction of emission","nx",nx,ClassName());
    gCleanOut->MMR(info,met,"Y direction of emission","ny",ny,ClassName());
    gCleanOut->MMR(info,met,"Z direction of emission","nz",nz,ClassName());
    fNode          = 0;
    fGeoVolume     = 0;
    fLitVolume     = 0;
    fLitDet        = 0;
    fLitDetG       = 0;
    fGeoMedium     = 0;
    fLitMedium     = 0;
    fNextNode      = 0;
    fNextGeoVolume = 0;
    fNextPath      = "";
    fNextLitVolume = 0;
    fNextLitDet    = 0;
    fNextLitDetG   = 0;
    fNextGeoMedium = 0;
    fNextLitMedium = 0;
    fNode      = gGeoManager->InitTrack(x,y,z,nx,ny,nz);
    if (!fNode) {
      fAnomaly = kTRUE;
      gCleanOut->MM(fatal,met,"Litranino created outside world",ClassName());
    }
    else {
      gCleanOut->MMT(info,met,"Litranino inside node","node",fNode->GetName(),ClassName());
      fGeoVolume    = fNode->GetVolume();
      gCleanOut->MMT(info,met,"Litranino inside volume","volume",fGeoVolume->GetName(),ClassName());
      const char *path = gGeoManager->GetPath();
      fPath = path;
      gCleanOut->MM(info,met,path,ClassName());
      if (fSourcePath.CompareTo(path)) {
        gCleanOut->MM(info,met,"User path  : ",ClassName());
        gCleanOut->MM(info,met,fSourcePath.Data(),ClassName());
        gCleanOut->MM(info,met,"Found path : ",ClassName());
        gCleanOut->MM(info,met,path,ClassName());
        gCleanOut->MM(error,met,"Litranino not in right volume",ClassName());
      }
      fLitVolume = (TLitVolume*)fGeoVolume->GetField();
      if (fLitVolume) {
        gCleanOut->MM(info,met,"There is an associated TLitVolume",ClassName());
        if (fLitVolume->GetDetKind() != none) {
          gCleanOut->MM(info,met,"TLitVolume is a detector",ClassName());
          Bool_t detfound = Detector();
          if (detfound) gCleanOut->MM(info,met,"Detector found",ClassName());
          else          gCleanOut->MM(fatal,met,"Detector not found",ClassName());
        }
      }
      else gCleanOut->MM(info,met,"No associated TLitVolume",ClassName());
      fGeoMedium    = fGeoVolume->GetMedium();
      IsLitMedium = fGeoMedium->InheritsFrom("TLitMedium");
      if (IsLitMedium) {
        fPhtLive = kTRUE;
        fAnomaly = kFALSE;
        fLitMedium = (TLitMedium*)fGeoMedium;
        gCleanOut->MMT(info,met,"Litranino inside TLitMedium","fLitMedium",fLitMedium->GetName(),ClassName());
        if (!fLitMedium->IsWrapping()) {
          gCleanOut->MM(info,met,"Ok: TLitMedium is not a wrapping",ClassName());
          fIsOnBoundary = kFALSE;
          Move();
        }
        else {
          gCleanOut->MM(fatal,met,"Litranino created inside a wrapping",ClassName());
        }
      }
      else {
        fPhtLive = kFALSE;
        fAnomaly = kTRUE;
        gCleanOut->MM(error,met,"Litranino not created in a TLitMedium",ClassName());
        gCleanOut->MMT(fatal,met,"Litranino inside TGeoMedium","fGeoMedium",fGeoMedium->GetName(),ClassName());
      }
    }
    if (fLength>fLengthMax) fLengthMax = fLength;
  }//end for (kTrack=0;kTrack<N;kTrack++)
  if (fRecordedTracks) {
    if (fAnimateTracks) gGeoManager->AnimateTracks(0.0,fLengthMax/TLitPhys::Get()->Cmks(),fNframes,"/D /G");
    else                gGeoManager->DrawTracks();
  }
//Put statistics of this run into the tree.
  if (gLitCs) {
		gLitCs->Conclusion();
    TLit::Get()->fLitFile->cd();
    TLit::Get()->fLitNb += TLit::Get()->fLitTree->Fill();
    gROOT->cd();
  }
}
void TLitranino::InDetRecord() {
  // Litranino has died inside a detector or has been detected. It has to be
  //registered. All calculations in local coordinate system of detector shape
  const Double_t eps   = 1.0e-12;
  const Double_t big   = 1.0e+12;
  const Double_t dixp7 = 1.0e+7;
  const char *met = "InDetRecord";
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
  gCleanOut->MM(info,met,"Litranino has been absorbed in detector",ClassName());
  gCleanOut->MMT(info,met,"TGeoVolume","Volume",fGeoVolume->GetName(),ClassName());
  gCleanOut->MMT(info,met,"TGeoNode","Node",fNode->GetName(),ClassName());
  gCleanOut->MM(info,met,fPath.Data(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate X","X",fStopPoint.X(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Y","Y",fStopPoint.Y(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Z","Z",fStopPoint.Z(),ClassName());
  // Jump in local coord system
//  for (k=0;k<3;k++) glob_pt[k] = fStopPoint[k];
  glob_pt = gGeoManager->GetCurrentPoint();
  glob_dir = gGeoManager->GetCurrentDirection();
  gGeoManager->MasterToLocal(glob_pt,loc_pt);
  for (k=0;k<3;k++) StopPoint[k] = loc_pt[k];
//  for (k=0;k<3;k++) glob_dir[k] = fCurrentDir[k];
  gGeoManager->MasterToLocalVect(glob_dir,loc_dir);
  for (k=0;k<3;k++) StopDir[k] = loc_dir[k];
  //
  gCleanOut->MMR(info,met,"Local coord. X","X",StopPoint.X(),ClassName());
  gCleanOut->MMR(info,met,"Local coord. Y","Y",StopPoint.Y(),ClassName());
  gCleanOut->MMR(info,met,"Local coord. Z","Z",StopPoint.Z(),ClassName());
  gCleanOut->MMR(info,met,"Local dir. nx","nx",StopDir.X(),ClassName());
  gCleanOut->MMR(info,met,"Local dir. ny","ny",StopDir.Y(),ClassName());
  gCleanOut->MMR(info,met,"Local dir. nz","nz",StopDir.Z(),ClassName());
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
  bin = gLitGs->fHPoints->Fill(fNPoints);
  bin = fLitDetG->FillDepth(DepthInDet);
  bin = fLitDetG->FillXYEntrance(xp,yp);
  if (gLitCs) {
    bin = gLitCs->fHPoints->Fill(fNPoints);
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
    bin = gLitGs->fHDistSeen->Fill(fLength);
    bin = gLitGs->fHWvlgthSeen->Fill(fWaveLength);
    fLitDetG->fNpSeen++;
    bin = fLitDetG->FillTimeSeen(fT0);
    bin = fLitDetG->FillWvlgthSeen(fWaveLength);
    if (gLitCs) {
      gLitCs->fNpSeen++;
      bin = gLitCs->fHTimeSeen->Fill(fT0);
      bin = gLitCs->fHDistSeen->Fill(fLength);
      bin = gLitCs->fHWvlgthSeen->Fill(fWaveLength);
      fLitDet->fNpSeen++;
      bin = fLitDet->FillTimeSeen(fT0);
      bin = fLitDet->FillWvlgthSeen(fWaveLength);
    }
  }
  else {
    gLitGs->fNpLossAny++;
    bin = gLitGs->fHAbsAny->Fill(fLength);
    fLitDetG->fNpLossAny++;
    bin = fLitDetG->FillWvlgthNot(fWaveLength);
    if (gLitCs) {
      gLitCs->fNpLossAny++;
      bin = gLitCs->fHAbsAny->Fill(fLength);
      fLitDet->fNpLossAny++;
      bin = fLitDet->FillWvlgthNot(fWaveLength);
    }
  }
}
void TLitranino::InMatRecord() {
  // Litranino has died inside a TLitMedium being not a wrapping. We store all infos
  //about this end
  const char *met = "InMatRecord";
  Int_t bin;
  fPhtLive = kFALSE;
  gCleanOut->MM(info,met,"Litranino has been absorbed by a volume",ClassName());
  gCleanOut->MMT(info,met,"TGeoVolume","Volume",fGeoVolume->GetName(),ClassName());
  gCleanOut->MMT(info,met,"TGeoNode","Node",fNode->GetName(),ClassName());
  gCleanOut->MM(info,met,fPath.Data(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate X","X",fStopPoint.X(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Y","Y",fStopPoint.Y(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Z","Z",fStopPoint.Z(),ClassName());
  gLitGs->fNpLossAny++;
  gLitGs->fNpAbsMat++;
  if (fLitMedium->GetAbsL()<=0.0) gLitGs->fNpAbsL0++;
  bin = gLitGs->fHPoints->Fill(fNPoints);
  bin = gLitGs->fHAbsAny->Fill(fLength);
  bin = gLitGs->fHAbsMat->Fill(fLength);
  if (gLitCs) {
    gLitCs->fNpLossAny++;
    gLitCs->fNpAbsMat++;
    if (fLitMedium->GetAbsL()<=0.0) gLitCs->fNpAbsL0++;
    bin = gLitCs->fHPoints->Fill(fNPoints);
    bin = gLitCs->fHAbsAny->Fill(fLength);
    bin = gLitCs->fHAbsMat->Fill(fLength);
  }
}
void TLitranino::InRevRecord() {
  // Litranino has died inside a TLitMedium being a wrapping. We store all infos
  //about this end. For a litranino, only supplementary absorption is activ, so
  //fNpAbsBef is incremented, but never fNpAbsRvt.
  const char *met = "InRevRecord";
  Int_t bin;
  fPhtLive   = kFALSE;
  gCleanOut->MM(info,met,"Litranino absorbed by a wrapping",ClassName());
  gCleanOut->MMT(info,met,"TGeoVolume","Volume",fNextGeoVolume->GetName(),ClassName());
  gCleanOut->MMT(info,met,"TGeoNode","Node",fNextNode->GetName(),ClassName());
  gCleanOut->MM(info,met,fNextPath.Data(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate X","X",fStopPoint.X(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Y","Y",fStopPoint.Y(),ClassName());
  gCleanOut->MMR(info,met,"Coordinate Z","Z",fStopPoint.Z(),ClassName());
  gLitGs->fNpLossAny++;
  gLitGs->fNpAbsBef++;
  bin = gLitGs->fHPoints->Fill(fNPoints);
  bin = gLitGs->fHAbsAny->Fill(fLength);
  bin = gLitGs->fHAbsBef->Fill(fLength);
  if (gLitCs) {
    gLitCs->fNpLossAny++;
    gLitCs->fNpAbsBef++;
    bin = gLitCs->fHPoints->Fill(fNPoints);
    bin = gLitCs->fHAbsAny->Fill(fLength);
    bin = gLitCs->fHAbsBef->Fill(fLength);
  }
}
void TLitranino::Init() {
  // Initialization of variables
  InitP();
  fAbsLength      = 100.0;
  fTrackNb        = -1;
  fToDraw         = kFALSE;
  fRecordedTracks = 0;
  fAnimateTracks  = kFALSE;
  fNframes        = 1000;
}
void TLitranino::InitP() {
//Pointers to 0
  fStartLitVol   = 0;
  fNode          = 0;
  fGeoVolume        = 0;
  fLitVolume     = 0;
  fLitDet        = 0;
  fLitDetG       = 0;
  fGeoMedium        = 0;
  fLitMedium     = 0;
  fNextNode      = 0;
  fNextGeoVolume    = 0;
  fNextLitVolume = 0;
  fNextLitDet    = 0;
  fNextLitDetG   = 0;
  fNextGeoMedium    = 0;
  fNextLitMedium = 0;
  fCurrentTrack  = 0;
}
void TLitranino::Jump() {
  // This routine moves the litranino from its current position to its next position
  const Short_t  ntry = 100;
  const Double_t eps  = 1.0e-5;
  const char *met = "Jump";
  Int_t bin;
  Bool_t IsOutside;
  Short_t ktry;
  Double_t xx,L;
  Double_t *normal;
  const Double_t *point;
  // If we are inside a detector, and if this detector has fStopAlways == true,
  //as is the case for example for a Phototube, then the litranino stops
  //here immediatly. We have to register it.
  if (fLitVolume) {
    if (!fLitVolume->GetWCSdone()) fLitVolume->SetWCS();
    gCleanOut->MM(info,met,"Coordinates in WCS in TLitVolume calculated",ClassName());
    if ((fLitDetG) && (fLitVolume->GetStopAlways())) {
      fStopPoint = fCurrentPoint;
      InDetRecord();
    }
  }
  // Finds next interaction face, and the distance to travel to the face and
  //do the step !
  if (fLitVolume && fLitVolume->IsStepping()) {
    Int_t k,nstep;
    Double_t step,endstep,LL;
    Bool_t lastst,absorbed;
    TGeoNode *node;
    node = gGeoManager->FindNextBoundary(TLitPhys::Get()->BiggestStep());
    LL = gGeoManager->GetStep();
    step    = fLitVolume->GetStep();
    nstep   = (Int_t)(LL/step);
    endstep = LL - nstep*step;
    nstep++;
    k = 0;
    while ((fPhtLive) && (k<nstep)) {
      //Provi
      // lpoint = gGeoManager->GetCurrentPoint();
      // cout << "point: " << lpoint[0] << "  " << lpoint[1] << "  " << lpoint[2] << endl;
      //End Provi
      lastst = (k==nstep-1);
      if (lastst) fStep = endstep;
      else        fStep = step;
      // checks if absorbed in material
      xx = gRandom3->Rndm();
      L = -fLitMedium->GetAbsL()*TMath::Log(1.0-xx);
      absorbed = (L<fStep);
      if (absorbed) fStep = L;
      gGeoManager->SetStep(fStep);
      if ((lastst) && (!absorbed)) {
        fNextNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
      }
      else node = gGeoManager->Step();
      //Provi
      // lpoint = gGeoManager->GetCurrentPoint();
      // cout << "point: " << lpoint[0] << "  " << lpoint[1] << "  " << lpoint[2] << endl;
      //End Provi
      if (absorbed) {
        fPhtLive = kFALSE;
        if (fLitDetG) {
          fLengthInDet = L;
          InDetRecord();
        }
        else InMatRecord();
      }//end if (absorbed)
      k++;
    }//end for (k=0;k<nstep;k++)
  }//end if (fLitVolume && fLitVolume->IsStepping())
  else {
    fNextNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    gCleanOut->MM(info,met,"FindNextBoundaryAndStep called",ClassName());
    fStep      = gGeoManager->GetStep();
    gCleanOut->MMR(info,met,"Step just done","fStep",fStep,ClassName());
    // Checks if absorbed in material
    xx = gRandom3->Rndm();
    if (fLitMedium) L = -fLitMedium->GetAbsL()*TMath::Log(1.0-xx);
    else            L = -fAbsLength*TMath::Log(1.0-xx);
    if (L<fStep) {
      fStopPoint = fCurrentPoint + L*fCurrentDir;
      // stepping backwards
      gGeoManager->SetCurrentDirection(-fCurrentDir.X(),-fCurrentDir.Y(),-fCurrentDir.Z());
      gGeoManager->SetStep(fStep-L);
      fNode = gGeoManager->Step();
      gGeoManager->SetCurrentDirection(fCurrentDir.X(),fCurrentDir.Y(),fCurrentDir.Z());
      fPhtLive = kFALSE;
      if (fLitVolume) {
        if (fLitDetG) {
          fLengthInDet = L;
          InDetRecord();
        }
        else InMatRecord();
      }//end if (fLitVolume)
    }//end if (L<fStep)
  }//end else if (fLitVolume && fLitVolume->IsStepping())
  if (fPhtLive) {
    if (!fNextNode) {
      gCleanOut->MM(killthis,met,"No next node found. Leaving the setup",ClassName());
      fAnomaly = kTRUE;
      fPhtLive = kFALSE;
    }
    else {
      fEntering  = gGeoManager->IsStepEntering();
      gCleanOut->MMB(info,met,"True if has enter inside some daughter","fEntering",fEntering,ClassName());
      normal = gGeoManager->FindNormalFast();
      fFN.SetXYZ(normal[0],normal[1],normal[2]);
      gCleanOut->MMR(info,met,"x component of normal to face","nx",fFN.X(),ClassName());
      gCleanOut->MMR(info,met,"y component of normal to face","ny",fFN.Y(),ClassName());
      gCleanOut->MMR(info,met,"z component of normal to face","nz",fFN.Z(),ClassName());
      fLength += fStep;
      fT0     += fStep/TLitPhys::Get()->C();
      gCleanOut->MMR(info,met,"Length of path","fLength",fLength,ClassName());
      gCleanOut->MMT(info,met,"Node found after step","fNextNode",fNextNode->GetName(),ClassName());
      IsOutside  = gGeoManager->IsOutside();
      if (IsOutside) {
        gLitGs->fNpOutSide++;
        gLitGs->fNpLossAny++;
        gLitGs->fNpAbnorm++;
        bin = gLitGs->fHOutSide->Fill(fLength);
        bin = gLitGs->fHAbnLost->Fill(fLength);
        bin = gLitGs->fHAbsAny->Fill(fLength);
        if (gLitCs) {
          gLitCs->fNpOutSide++;
          gLitCs->fNpLossAny++;
          gLitCs->fNpAbnorm++;
          bin = gLitCs->fHOutSide->Fill(fLength);
          bin = gLitCs->fHAbnLost->Fill(fLength);
          bin = gLitCs->fHAbsAny->Fill(fLength);
        }
        gCleanOut->MM(killthis,met,"Leaving the geometry",ClassName());
        fAnomaly = kTRUE;
        fPhtLive = kFALSE;
      }
      else {
        if (fNextNode == fNode) {
          gCleanOut->MM(error,met,"Not entering the next shape",ClassName());
          ktry = 0;
          fStep = 0.0;
          while ((fNextNode == fNode) && (ktry<ntry)) {
            ktry++;
            fStep   += eps;
            gGeoManager->SetStep(fStep);
            fNextNode = gGeoManager->Step(kTRUE,kTRUE);
          }
          if (fNextNode == fNode) {
            gCleanOut->MM(killthis,met,"Impossible to enter the next shape",ClassName());
            fAnomaly = kTRUE;
            fPhtLive = kFALSE;
          }
        }
        if (fPhtLive) {
          point = gGeoManager->GetCurrentPoint();
          fCurrentPoint.SetXYZ(point[0],point[1],point[2]);
          gCleanOut->MM(info,met,"New point reached",ClassName());
          gCleanOut->MMR(info,met,"Coordinate X","X",fCurrentPoint.X(),ClassName());
          gCleanOut->MMR(info,met,"Coordinate Y","Y",fCurrentPoint.Y(),ClassName());
          gCleanOut->MMR(info,met,"Coordinate Z","Z",fCurrentPoint.Z(),ClassName());
          if (fToDraw)
            fCurrentTrack->AddPoint(fCurrentPoint.X(),fCurrentPoint.Y(),fCurrentPoint.Z(),fLength/TLitPhys::Get()->Cmks());
          fNextGeoVolume = fNextNode->GetVolume();
          gCleanOut->MMT(info,met,"Next volume found","fNextGeoVolume",fNextGeoVolume->GetName(),ClassName());
          const char *path = gGeoManager->GetPath();
          fNextPath = path;
          gCleanOut->MM(info,met,path,ClassName());
          fNextLitVolume = (TLitVolume*)fNextGeoVolume->GetField();
          if (fNextLitVolume) {
            gCleanOut->MM(info,met,"There is an associated TLitVolume",ClassName());
            if (fNextLitVolume->GetDetKind() != none) {
              gCleanOut->MM(info,met,"TLitVolume is a detector",ClassName());
              Bool_t detfound = DetectorN();
              if (detfound) gCleanOut->MM(info,met,"Detector found",ClassName());
              else          gCleanOut->MM(fatal,met,"Detector not found",ClassName());
            }
          }
          else gCleanOut->MM(info,met,"No associated TLitVolume",ClassName());
          fNextGeoMedium = fNextGeoVolume->GetMedium();
          if (fNextGeoMedium->InheritsFrom("TLitMedium")) {
            fNextLitMedium = (TLitMedium*)fNextGeoMedium;
            gCleanOut->MMT(info,met,"Next TLitMedium found","fNextLitMedium",fNextLitMedium->GetName(),ClassName());
          }
          else {
            fNextLitMedium = 0;
            gCleanOut->MM(info,met,"Next medium is not a TLitMedium",ClassName());
            gCleanOut->MMT(info,met,"Next TGeoMedium found","fNextGeoMedium",fNextGeoMedium->GetName(),ClassName());
          }
        }//end if (fPhtLive)
      }//end else if (IsOutside)
    }//end else if (!fNextNode)
  }//end if (fPhtLive)
}
void TLitranino::Move() {
  // Stirring routine moving the litranino inside the setup, from birth to death
  const char *met = "Move";
  Int_t bin;
  while ((fPhtLive) && (!fAnomaly)) {
    fNPoints++;
    if (!fIsOnBoundary) {
      gCleanOut->MM(info,met,"Calling Jump",ClassName());
      Jump();
    }//end if (!fIsOnBoundary)
    if (fPhtLive) {
      if (fNPoints>fgNPointsMax) {
        fPhtLive = kFALSE;
        fAnomaly = kTRUE;
        gLitGs->fNpAbnorm++;
        gLitGs->fNpLossAny++;
        bin = gLitGs->fHAbsAny->Fill(fLength);
        bin = gLitGs->fHAbnLost->Fill(fLength);
        bin = gLitGs->fHPoints->Fill(fNPoints);
        if (gLitCs) {
          gLitCs->fNpAbnorm++;
          gLitCs->fNpLossAny++;
          bin = gLitCs->fHAbsAny->Fill(fLength);
          bin = gLitCs->fHAbnLost->Fill(fLength);
          bin = gLitCs->fHPoints->Fill(fNPoints);
        }
        gCleanOut->MM(killthis,met,"Too much interaction points on trajectory",ClassName());
      }
      if ((fPhtLive) && (fLength>fMaxLength)) {
        fPhtLive = kFALSE;
        fAnomaly = kTRUE;
        gLitGs->fNpAbnorm++;
        gLitGs->fNpLossAny++;
        bin = gLitGs->fHAbsAny->Fill(fLength);
        bin = gLitGs->fHAbnLost->Fill(fLength);
        bin = gLitGs->fHPoints->Fill(fNPoints);
        if (gLitCs) {
          gLitCs->fNpAbnorm++;
          gLitCs->fNpLossAny++;
          bin = gLitCs->fHAbsAny->Fill(fLength);
          bin = gLitCs->fHAbnLost->Fill(fLength);
          bin = gLitCs->fHPoints->Fill(fNPoints);
        }
        gCleanOut->MM(killthis,met,"Maximum path length reached. Litranino dead",ClassName());
      }
      if ((fPhtLive) && (!fAnomaly)) {
        gCleanOut->MM(info,met,"Calling OnFace",ClassName());
        fIsOnBoundary = kTRUE;
        OnFace();
      }//end if ((fPhtLive) && (!fAnomaly))
    }//end if (fPhtLive)
  }//end while ((fPhtLive) && (!fAnomaly))
  if (fAnomaly) {
    gCleanOut->MM(info,met,"Putting fPhtLive to false because of anomaly",ClassName());
    fPhtLive = kFALSE;
  }
}
void TLitranino::OnFace() {
  //OnFace is the steering routine to handle what has to be done when the
  //litranino reaches a face of a shape.
  const char *met = "OnFace";
  if (!fNextLitMedium) {
    gCleanOut->MM(error,met,"We face a TGeoMedium, not a TLitMedium",ClassName());
    gCleanOut->MM(error,"We stop the litranino here",ClassName());
    fPhtLive = kFALSE;
  }
  else {
    gCleanOut->MMT(info,met,"We face following TLitMedium","fNextLitMedium",fNextLitMedium->GetName(),ClassName());
    if (fNextLitMedium->IsWrapping()) {
      gCleanOut->MM(info,met,"We face a wrapping",ClassName());
      AgainstRev();
    }
    else {
      gCleanOut->MM(info,met,"We face a volume",ClassName());
      AgainstShape();
    }
  }//end else if (!fNextLitMedium)
  fIsOnBoundary = kFALSE;
}
void TLitranino::RestartPrinting(Int_t run,Int_t ev) {
  // Printing will restart at (run,ev)
  gCleanOut->SetRestart(run,ev);
}
void TLitranino::TrackToDraw(Int_t k) {
  //
  // (k <  0)                ==> no track to be drawn
  // (k >= 0) && (k<1000000) ==> track k is to be drawn
  // (k >= 1000000)          ==> all tracks to be drawn
  //
  fTrackNb = k;
}
