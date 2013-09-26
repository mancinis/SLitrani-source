// @(#)root/html:$Id: TLitDetector.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TString.h"
#include "TCleanOut.h"
#include "TLitHistParams.h"
#include "TLitDetector.h"

ClassImp(TLitDetector)
//______________________________________________________________________________
//
//   Contains all statistics and histograms for a detector
//
//  This class has NOT to be called by the user of SLitrani. If the user of SLitrani
// wants to define a detector, it has to associate to the TGeoVolume intended to be
// a detector a TLitVolume and then to call either TLitVolume::SetDetector() or
// TLitVolume::SetPhotoTube or TLitVolume::SetAPD(). TLitDetector will be instanciated
// by SLitrani each time they are needed.
//  It is important to understand that when you have defined a given TGeoVolume as
// being a detector by hooking to it a TLitVolume and by calling one of the 3 methods
// quoted above, you do not have defined ONE detector, but AS MANY detectors as there
// are instances [nodes] of the TGeoVolume being a detector in the setup. A given
// TGeoVolume can be inserted many times, with many different orientations in a
// setup. There will be correspondingly AS MANY TLitDetector classes instanciated
// by SLitrani! So the number of physical detectors in the setup can be huge. Since
// class TLitDetector defines histograms, the memory needed may become prohibitive.
// So enum LitMemForDet is provided to the user, in order to cope with this problem.
// The static variable LitMemForDet TLitDetector::fgMemForDet is public and can be
// changed by the user. Its default value is bighist, valid for the case where there
// are very few physical detectors in the setup. If fgMemForDet is:
//
//  nohist     : no histogram will be booked for the detectors. Only solution when
//               the number of detectors is huge.
//  punyhist   : TH1S used for 1dim histos
//               TH2C used for 2dim histos
//               and small number of channels used
//  smallhist  : TH1S used for 1dim histos
//               TH2C used for 2dim histos
//               number of channels a little bit bigger
//  normalhist : TH1F used for 1dim histos
//               TH2S used for 2dim histos
//               acceptable number of channels
//  bighist    : TH1D used for 1dim histos
//               TH2F used for 2dim histos
//               comfortable number of channels
//
//  Because the name of a particular TLitDetector is the path leading
// to it [look at the constructor], all TLitDetector will have a different name.
//  All these detectors are stored inside the static list 
//    - TLitDetector::fgLitDetGlob if valid for all runs
//    - TLitDetector::fgLitDetRun  if valid for the duration of a run
//  A specific detector can be retrieved with its name using static method 
// TLitDetector::FindDetector().
//
// Class variable fGlob contains various infos inside its bits:
//
//    bit   0  : 1 : global statistics for all runs
//               0 : statistics for the current run
//    bit   1  : 1 : detector is of type "gendet" general detector
//    bit   2  : 1 : detector is of type "phototube"
//    bit   3  : 1 : detector is of type "apd"
//    bit   4  : free for future type of detectors
//    bit   5  : free for future type of detectors
//    bit   6  : free for future type of detectors
//    bit   7  : free for future type of detectors
//    bit   8  : 1 : there is a test on quantum efficiency
//               0 : there is no test on quantum efficiency
//    bit   9  : 1 : TLitVolume::fStopAlways is true : detector is a surface detector
//               0 : TLitVolume::fStopAlways is false: detector is a volume detector
//    bit  10  : 1 : detector has an entrance face
//               0 : all faces of TGeoVolume can be entrance face of detector
//    bits 11 -> 15 free for the time being
//

LitMemForDet  TLitDetector::fgMemForDet    = bighist;
Double_t      TLitDetector::fgMaxCatAn     = 0.01;
TObjArray    *TLitDetector::fgLitDetGlob   = 0;
TObjArray    *TLitDetector::fgLitDetRun    = 0;
TLitDetector *TLitDetector::fgLastUsedDet  = 0;
TLitDetector *TLitDetector::fgLastUsedDetG = 0;
Int_t         TLitDetector::fgMaxHist      = 7;

TLitDetector::TLitDetector(const char *name,KindOfDet det,Bool_t b,Bool_t isQE,
  Bool_t StopAlways,Bool_t Entrance,Double_t dx,Double_t dy,Double_t dz) {
  //
  //      Arguments
  //
  //  name       : name of detector is the path leading to this detector,
  //                for example "/TOP_1/REV_2/APD_1". G is prepended to the name if
  //                this version of TLitDetector is valid for all runs [ = summarizes
  //                statistics and histograms for all runs]. In that case:
  //                "G/TOP_1/REV_2/APD_1". This Prepend has to be done in the call
  //                to this constructor
  //  det        : {none,gendet,phototube,apd}
  //  b          : true if global [all runs] statistics. False if per run statistics
  //  isQE       : true if there is a test on quantum efficiency
  //  StopAlways : false: photon only stops inside the TGeoVolume because of its finite
  //                      absorption length.
  //                true: photon reaching this TGeoVolume always stops in the TGeoVolume
  //  Entrance   : true  : detector has an entrance face
  //               false : all faces of TGeoVolume can be entrance face of detector
  //  dx         : semi-width of entrance face along 1st dimension
  //  dy         : semi-width of entrance face along 2nd dimension
  //  dz         : semi-depth of volume, in direction orthogonal to entrance face. It
  //               is transformed into full depth before being given to BookHistos.
  //               In case of APD, we are only interested in the sensitive region
  //               which is of the order of 100000nm = 0.01 cm
  //
  Init();
  if (!fgLitDetGlob) fgLitDetGlob = new TObjArray();
  if (!fgLitDetRun)  fgLitDetRun  = new TObjArray();
  fName = name;
  if (b)                fGlob |= 1;
  if (det == gendet)    fGlob |= 2;
  if (det == phototube) fGlob |= 4;
  if (det == apd)       {
    fGlob |= 8;
    dz = TMath::Min(2*dz,TLitDetector::fgMaxCatAn);
  }
  if (isQE)             fGlob |= 256;
  if (StopAlways)       fGlob |= 512;
  if (Entrance)         fGlob |= 1024;
  BookHistos(dx,dy,dz);
  AddMe();
}
TLitDetector::~TLitDetector() {
  //destructor
  ClearHistos();
}
void TLitDetector::AddMe() {
  // Add this TLitDetector in the lists of TLitDetector
  TLitDetector *p = 0;
  TString s;
  if (fGlob & 1) {
    s = GetName();
    p = TLitDetector::FindDetector(s,kTRUE);
    if (p) {
      fgLitDetGlob->Remove(p);
      delete p;
    }
    fgLitDetGlob->AddLast(this);
    fgLitDetGlob->Sort();
  }
  else {
    if (fgLitDetRun) {
      s = GetName();
      p = TLitDetector::FindDetector(s,kFALSE);
      if (p) {
        fgLitDetRun->Remove(p);
        delete p;
      }
      fgLitDetRun->AddLast(this);
      fgLitDetRun->Sort();
    }
  }
}
void TLitDetector::BookHistos(Double_t dx,Double_t dy,Double_t zmax) {
  // Booking of all histograms
  //
  //  dx   : 1st semi-dimension of entrance window, if any
  //  dy   : 2nd semi-dimension of entrance window, if any
  //  zmax : z max of Depth inside detector to be histogrammed
  //
  if (TLitDetector::fgMemForDet == nohist) return;
  const Int_t N1  = 20;  //Nb of channels for 1D histos in case fgMemForDet == punyhist
  const Int_t N2  = 50;  //Nb of channels for 1D histos in case fgMemForDet == smallhist
  const Int_t N3  = 100; //Nb of channels for 1D histos in case fgMemForDet == normalhist
  const Int_t N4  = 200; //Nb of channels for 1D histos in case fgMemForDet == bighist
  const Int_t M1X = 10;  //Nb of X channels for 2D histos in case fgMemForDet == punyhist
  const Int_t M1Y = 10;  //Nb of Y channels for 2D histos in case fgMemForDet == punyhist
  const Int_t M2X = 20;  //Nb of X channels for 2D histos in case fgMemForDet == smallhist
  const Int_t M2Y = 20;  //Nb of Y channels for 2D histos in case fgMemForDet == smallhist
  const Int_t M3X = 30;  //Nb of X channels for 2D histos in case fgMemForDet == normalhist
  const Int_t M3Y = 30;  //Nb of Y channels for 2D histos in case fgMemForDet == normalhist
  const Int_t M4X = 40;  //Nb of X channels for 2D histos in case fgMemForDet == bighist
  const Int_t M4Y = 40;  //Nb of Y channels for 2D histos in case fgMemForDet == bighist
  const Axis_t zero = 0.0;
  TLitHistParams *h;
  Int_t NDet; //Number of detectors already booked + 1
  TAxis *axis;
  TString s,st,det,detkind;
  TString sp = "";
  if (fGlob & 1) {
    sp = "G_";
    NDet = fgLitDetGlob->GetEntries();
  }
  else NDet = fgLitDetRun->GetEntries();
  NDet++;
  if (fGlob & 2) detkind = " gendet";
  if (fGlob & 4) detkind = " phototube";
  if (fGlob & 8) detkind = " APD";
  det = detkind;
  det.Append(' ');
  det.Append(GetName());
  //
  s = "TimeSeen_";
  if (fGlob & 1) s.Prepend(sp);
  s += NDet;
  st = GetName();
  st.Append(" Time of arrival (ps) of photons seen by");
  st.Append(detkind);
  h = TLitHistParams::Get("TimeSeen_");
  switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHTimeSeen = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHTimeSeen = new TH1S(s.Data(),st.Data(),N1,zero,TLitPhys::Get()->TooLate());
      break;
    case smallhist:
      if (h) fHTimeSeen = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHTimeSeen = new TH1S(s.Data(),st.Data(),N2,zero,TLitPhys::Get()->TooLate());
      break;
    case normalhist:
      if (h) fHTimeSeen = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHTimeSeen = new TH1F(s.Data(),st.Data(),N3,zero,TLitPhys::Get()->TooLate());
      break;
    case bighist:
      if (h) fHTimeSeen = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHTimeSeen = new TH1D(s.Data(),st.Data(),N4,zero,TLitPhys::Get()->TooLate());
      break;
  }
  axis = fHTimeSeen->GetXaxis();
  axis->SetTitle(det.Data());
  fNbHistos++;
  //
  s = "WvlgthSeen_";
  if (fGlob & 1) s.Prepend(sp);
  s += NDet;
  st = GetName();
  st.Append(" Wavelength (nm) of photons seen by");
  st.Append(detkind);
  h = TLitHistParams::Get("WvlgthSeen_");
  switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHWvlgthSeen = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHWvlgthSeen = new TH1S(s.Data(),st.Data(),N1,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case smallhist:
      if (h) fHWvlgthSeen = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHWvlgthSeen = new TH1S(s.Data(),st.Data(),N2,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case normalhist:
      if (h) fHWvlgthSeen = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHWvlgthSeen = new TH1F(s.Data(),st.Data(),N3,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case bighist:
      if (h) fHWvlgthSeen = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHWvlgthSeen = new TH1D(s.Data(),st.Data(),N4,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
  }
  axis = fHWvlgthSeen->GetXaxis();
  axis->SetTitle(det.Data());
  fNbHistos++;
  //
  s = "WvlgthNot_";
  if (fGlob & 1) s.Prepend(sp);
  s += NDet;
  st = GetName();
  st.Append(" Wavelength (nm) of photons not seen by");
  st.Append(detkind);
  h = TLitHistParams::Get("WvlgthNot_");
  switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHWvlgthNot = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHWvlgthNot = new TH1S(s.Data(),st.Data(),N1,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case smallhist:
      if (h) fHWvlgthNot = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHWvlgthNot = new TH1S(s.Data(),st.Data(),N2,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case normalhist:
      if (h) fHWvlgthNot = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHWvlgthNot = new TH1F(s.Data(),st.Data(),N3,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case bighist:
      if (h) fHWvlgthNot = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHWvlgthNot = new TH1D(s.Data(),st.Data(),N4,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
  }
  axis = fHWvlgthNot->GetXaxis();
  axis->SetTitle(det.Data());
  fNbHistos++;
  //
  // Following 2 histos only if test on quantum efficiency
  //
  if (fGlob & 256) {
    s = "QEff_";
    if (fGlob & 1) s.Prepend(sp);
    s += NDet;
    st = GetName();
    st.Append(" Quantum efficiency / wavelength (nm) of");
    st.Append(detkind);
    h = TLitHistParams::Get("QEff_");
    switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHQEff = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHQEff = new TH1S(s.Data(),st.Data(),N1,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case smallhist:
      if (h) fHQEff = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHQEff = new TH1S(s.Data(),st.Data(),N2,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case normalhist:
      if (h) fHQEff = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHQEff = new TH1F(s.Data(),st.Data(),N3,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case bighist:
      if (h) fHQEff = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHQEff = new TH1D(s.Data(),st.Data(),N4,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    }
    axis = fHQEff->GetXaxis();
    axis->SetTitle(det.Data());
    fNbHistos++;
    //
    s = "Inside_";
    if (fGlob & 1) s.Prepend(sp);
    s += NDet;
    st = GetName();
    st.Append(" Nb. of photons / wavelength (nm) of");
    st.Append(detkind);
    h = TLitHistParams::Get("Inside_");
    switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHInside = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHInside = new TH1S(s.Data(),st.Data(),N1,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case smallhist:
      if (h) fHInside = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHInside = new TH1S(s.Data(),st.Data(),N2,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case normalhist:
      if (h) fHInside = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHInside = new TH1F(s.Data(),st.Data(),N3,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    case bighist:
      if (h) fHInside = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHInside = new TH1D(s.Data(),st.Data(),N4,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
      break;
    }
    axis = fHInside->GetXaxis();
    axis->SetTitle(det.Data());
    fNbHistos++;
  }//end if (fGlob & 256)
  //
  // Following histogram only if volume detector and there is an entrance face
  //
  if (IsVolEnt()) {
    s  = "Depth_";
    if (fGlob & 1) s.Prepend(sp);
    s += NDet;
    st = GetName();
    st.Append(" Depth of photon from entrance face");
    st.Append(detkind);
    h = TLitHistParams::Get("Depth_");
    switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHDepth = new TH1S(s.Data(),st.Data(),N1,h->fHXlow,h->fHXup);
      else   fHDepth = new TH1S(s.Data(),st.Data(),N1,zero,zmax);
      break;
    case smallhist:
      if (h) fHDepth = new TH1S(s.Data(),st.Data(),N2,h->fHXlow,h->fHXup);
      else   fHDepth = new TH1S(s.Data(),st.Data(),N2,zero,zmax);
      break;
    case normalhist:
      if (h) fHDepth = new TH1F(s.Data(),st.Data(),N3,h->fHXlow,h->fHXup);
      else   fHDepth = new TH1F(s.Data(),st.Data(),N3,zero,zmax);
      break;
    case bighist:
      if (h) fHDepth = new TH1D(s.Data(),st.Data(),h->fHNbinsx,h->fHXlow,h->fHXup);
      else   fHDepth = new TH1D(s.Data(),st.Data(),N4,zero,zmax);
      break;
    }
    axis = fHDepth->GetXaxis();
    axis->SetTitle(det.Data());
    fNbHistos++;
  }//end if (IsVolEnt())
  //
  // Following histogram only if there is an entrance face
  //
  if (fGlob & 1024) {
    s  = "XYEntrance_";
    if (fGlob & 1) s.Prepend(sp);
    s += NDet;
    st = GetName();
    st.Append(" Projection of end point on entrance face");
    st.Append(detkind);
    h = TLitHistParams::Get("XYEntrance_");
    switch (TLitDetector::fgMemForDet) {
    case nohist:
      break;
    case punyhist:
      if (h) fHXYEntrance = new TH2C(s.Data(),st.Data(),h->fHNbinsx,-dx,dx,h->fHNbinsy,-dy,dy);
      else   fHXYEntrance = new TH2C(s.Data(),st.Data(),M1X,-dx,dx,M1Y,-dy,dy);
      break;
    case smallhist:
      if (h) fHXYEntrance = new TH2C(s.Data(),st.Data(),h->fHNbinsx,-dx,dx,h->fHNbinsy,-dy,dy);
      else   fHXYEntrance = new TH2C(s.Data(),st.Data(),M2X,-dx,dx,M2Y,-dy,dy);
      break;
    case normalhist:
      if (h) fHXYEntrance = new TH2S(s.Data(),st.Data(),h->fHNbinsx,-dx,dx,h->fHNbinsy,-dy,dy);
      else   fHXYEntrance = new TH2S(s.Data(),st.Data(),M3X,-dx,dx,M3Y,-dy,dy);
      break;
    case bighist:
      if (h) fHXYEntrance = new TH2F(s.Data(),st.Data(),h->fHNbinsx,-dx,dx,h->fHNbinsy,-dy,dy);
      else   fHXYEntrance = new TH2F(s.Data(),st.Data(),M4X,-dx,dx,M4Y,-dy,dy);
      break;
    }
    fNbHistos++;
  }//end if (fGlob & 1024)
}
void TLitDetector::ClearHistos() {
  // Clear all histograms
  if (fHTimeSeen) {
    delete fHTimeSeen;
    fHTimeSeen = 0;
  }
  if (fHWvlgthSeen) {
    delete fHWvlgthSeen;
    fHWvlgthSeen = 0;
  }
  if (fHWvlgthNot) {
    delete fHWvlgthNot;
    fHWvlgthNot = 0;
  }
  if (fHQEff) {
    delete fHQEff;
    fHQEff = 0;
  }
  if (fHInside) {
    delete fHInside;
    fHInside = 0;
  }
  if (fHDepth) {
    delete fHDepth;
    fHDepth = 0;
  }
  if (fHXYEntrance) {
    delete fHXYEntrance;
    fHXYEntrance = 0;
  }
  if (fHEfficiency) {
    delete fHEfficiency;
    fHEfficiency = 0;
  }
}
Int_t TLitDetector::Compare(const TObject *obj) const {
  Int_t k;
  TString s = GetName();
  TLitDetector *p = (TLitDetector*)obj;
  k = s.CompareTo(p->GetName());
  return k;
}
void TLitDetector::Conclusion() {
  // Last calculations before using the class
  if (fGlob & 256) {
    const Double_t un  = 1.0;
    const Int_t NbChan = 100;
    Int_t i;
    Axis_t a,num,den;
    for (i=1;i<=NbChan;i++) {
      num = fHQEff->GetBinContent(i);
      den = fHInside->GetBinContent(i);
      if (den<un) den = un;
      a   = num/den;
      fHQEff->SetBinContent(i,a);
    }
  }
}
TLitDetector *TLitDetector::FindDetector(TString namedet,Bool_t global) {
  //  Finds a TLitDetector of name namedet in TLitDetector::fgLitDetGlob if
  // global is true, in TLitDetector::fgLitDetRun if global is false. Uses
  // the fact that these collections are sorted.
  TLitDetector *det = 0;
  Int_t N;
  TObjArray *list;
  if (global) list = TLitDetector::fgLitDetGlob;
  else        list = TLitDetector::fgLitDetRun;
  N = list->GetEntries();
  if (N<8) det = (TLitDetector*)list->FindObject(namedet.Data());
  else {
    Bool_t found = kFALSE;
    Int_t N2,km,k,kl,kr,kd;
    TLitDetector *p;
    N2 = 8;
    while (N2<N) N2 *= 2;
    kd = 0;
    kl = 0;
    kr = N2;
    km = N2/2;
    while ((!found) && (kd<2)) {
      if (km<N) {
        p = (TLitDetector*)(*list)[km];
        k = namedet.CompareTo(p->GetName());
        switch (k) {
          case -1:
            kr  = km;
            km  = (kl+kr)/2;
            break;
          case 0:
            found = kTRUE;
            det = p;
            break;
          case 1:
            kl  = km;
            km  = (kl+kr)/2;
            break;
        }
        if ((kr-kl)<=1) kd++;
      }//end if (km<N)
      else {
        kr = km;
        km = (kl+kr)/2;
      }//end else if (km<N)
    }
  }
  return det;
}
void TLitDetector::Init() {
  // Initialization
  InitP();
  fGlob      = 0;
  fNbHistos  = 0;
  fNbSumHist = 0;
  fNpSeen    = 0;
  fNpLossAny = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
}
void TLitDetector::InitP() {
  // Pointers to 0
  fHTimeSeen   = 0;
  fHWvlgthSeen = 0;
  fHWvlgthNot  = 0;
  fHQEff       = 0;
  fHInside     = 0;
  fHDepth      = 0;
  fHXYEntrance = 0;
  //
  fHEfficiency = 0;
}
Bool_t TLitDetector::IsEqual(const TObject *obj) const {
  Int_t k;
  TString s = GetName();
  TLitDetector *p = (TLitDetector*)obj;
  k = s.CompareTo(p->GetName());
  return !k;
}
Bool_t TLitDetector::IsMe(const TString &name) const {
  // Return true if name is the name of this detector
  Int_t k = name.CompareTo(GetName());
  return !k;
}
void TLitDetector::Reset() {
  // Put everything to 0 for the beginning of a run
  fNpSeen    = 0;
  fNpLossAny = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
  ClearHistos();
}
