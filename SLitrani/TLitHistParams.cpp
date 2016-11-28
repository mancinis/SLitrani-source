// @(#)root/html:$Id: TLitHistParams.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TSystem.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TLitPhys.h"
#include "TLitHistParams.h"

TFile      *TLitHistParams::fgHistParams   = 0;
TString    *TLitHistParams::fgFileName     = 0;
TObjArray  *TLitHistParams::fgListHParams  = 0;
Bool_t      TLitHistParams::fgHistParamsOk = kFALSE;


ClassImp(TLitHistParams)
//______________________________________________________________________________
//
//  Histogram parameters for SLitrani into xml file.
//
//  In order that SLitrani histogram (or TGraphErrors) parameters can be modified
// without recompiling SLitrani, they are put into an xml file, "histparams.xml",
// that can be edited by the user before launching SLitrani.
//
TLitHistParams::TLitHistParams() {
  // Default constructor
  fHNbinsy = 0;
  fHYlow   = 0.0;
  fHYup    = 0.0;
}
TLitHistParams::TLitHistParams(const char *kind,const char *name, const char *title,
  Int_t nbinsx,Double_t xlow,Double_t xup,Int_t nbinsy,Double_t ylow,Double_t yup) {
  // Constructor
  fHKind   = kind;
  fHName   = name;
  fHTitle  = title;
  fHNbinsx = nbinsx;
  fHXlow   = xlow;
  fHXup    = xup;
  if (nbinsy<=1) {
    fHNbinsy = 0;
    fHYlow   = 0.0;
    fHYup    = 0.0;
  }
  else {
    fHNbinsy = nbinsy;
    fHYlow   = ylow;
    fHYup    = yup;
  }
}
TLitHistParams::~TLitHistParams() {
// Destructor
}
void TLitHistParams::Add(TLitHistParams *p) {
  // Add an entry into collection
  fgListHParams->AddLast(p);
  fgListHParams->Sort();
}
void TLitHistParams::Add(const char *kind,const char *name, const char *title,
  Int_t nbinsx,Double_t xlow,Double_t xup,Int_t nbinsy,Double_t ylow,Double_t yup) {
  // Creates an entry and add it into collection
  TLitHistParams *p;
  p = new TLitHistParams(kind,name,title,nbinsx,xlow,xup,nbinsy,ylow,yup);
  Add(p);
}
void TLitHistParams::CloseXmlFile() {
  // Close and delete xml file histparams.xml
  if ( fgHistParams ) {
    fgHistParams->Close();
    delete fgHistParams;
    fgHistParams = 0;
  }
}
Int_t TLitHistParams::Compare(const TObject *obj) const {
// Alphabetical order in fHName
  Int_t k;
  TString s;
  s = ((TLitHistParams*)obj)->fHName;
  k = fHName.CompareTo(s);
  return k;
}
void TLitHistParams::CreateHistParams() {
  // Recreate xml file histparams.xml with default values
  if (!fgFileName) fgFileName = new TString("histparams.xml");
  TLitPhys::Book();
  if (fgListHParams) {
    fgListHParams->Delete();
    delete fgListHParams;
    fgListHParams = 0;
  }
  CloseXmlFile();
  fgHistParams = TFile::Open(fgFileName->Data(),"recreate");
  fgListHParams = new TObjArray();
  //
  // TLitResults::CInit
  //
  Add("TH1D","Points","Number of points on photon trajectory",250,0.0,250.0,0,0.0,0.0);
  Add("TH1D","TimeSeen","time of arrival (ps) of photons on detector",100,0.0,TLitPhys::Get()->TooLate(),0,0.0,0.0);
  Add("TH1D","DistSeen","distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  Add("TH1D","WvlgthSeen","Wavelength (nm) of photons seen",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","AbsAny","distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  Add("TH1D","AbsMat","distance (cm) travelled by photons absorbed in material",100,0.0,400.0);
  Add("TH1D","AbsBef","distance (cm) of photons absorbed before wrapping",100,0.0,400.0);
  Add("TH1D","AbsRvt","distance (cm) of photons absorbed by wrapping",100,0.0,400.0);
  Add("TH1D","OutSide","distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  Add("TH1D","AbnLost","distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  Add("TH1D","TooLate","Wavelength of photons seen, but too late",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","Diffusion","Number of diffusions per photon",16,0.0,16.0);
  Add("TH1D","TimeAll","Generated time (ps) for all components",100,0.0,TLitPhys::Get()->TooLate());
  Add("TH1D","WvlgthAll","Generated wavelength (nm) for all components",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","TimeCer","Generated time (ps) for Cerenkov photons",100,0.0,TLitPhys::Get()->TooLate());
  Add("TH1D","WvlgthCer","Generated wavelength (nm) for Cerenkov photons",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  // fNfluo being unknown at this stage, it is impossible to fully preset histogram
  //"TimeEach" or "WvlgthEach". fNfluo is replaced by 1
  Add("TH2F","TimeEach","Generated time for each component",64,0.0,TLitPhys::Get()->TooLate(),1,0.0,1.0);
  Add("TH2F","WvlgthEach","Generated wavelength (nm) for each component",64,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL(),1,0.0,1.0);
  Add("TH1D","Momentum","Momentum [Gev/c] of generated particles",100,0.0,100.0);
  Add("TH1D","Theta","Theta distribution of generated particles",80,0.0,3.2);
  // nshapes being unknown at this stage, it is impossible to fully preset histogram
  //"EinShape". nshapes is replaced by 1, fgEnergyOfGamma by 1.0
  Add("TH2F","EinShape","Energy [Mev] deposited by gamma in all shapes",1,0.0,10.0,50,0.0,1.0);
  // Histogram "MatToMat", "ReflByMat", "MatToRev", "RevToMat" cannot be preset before
  // knowing the geometry
  //
  // TLitResults::GInit
  //
  Add("TH1D","G_Points","Number of points on photon trajectory",250,0.0,250.0,0,0.0,0.0);
  Add("TH1D","G_TimeSeen","time of arrival (ps) of photons on detector",100,0.0,TLitPhys::Get()->TooLate(),0,0.0,0.0);
  Add("TH1D","G_DistSeen","distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  Add("TH1D","G_WvlgthSeen","Wavelength (nm) of photons seen",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","G_AbsAny","distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  Add("TH1D","G_AbsMat","distance (cm) travelled by photons absorbed in material",100,0.0,400.0);
  Add("TH1D","G_AbsBef","distance (cm) of photons absorbed before wrapping",100,0.0,400.0);
  Add("TH1D","G_AbsRvt","distance (cm) of photons absorbed by wrapping",100,0.0,400.0);
  Add("TH1D","G_OutSide","distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  Add("TH1D","G_AbnLost","distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  Add("TH1D","G_TooLate","Wavelength of photons seen, but too late",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","G_Diffusion","Number of diffusions per photon",16,0.0,16.0);
  Add("TH1D","G_TimeAll","Generated time (ps) for all components",100,0.0,TLitPhys::Get()->TooLate());
  Add("TH1D","G_WvlgthAll","Generated wavelength (nm) for all components",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1D","G_TimeCer","Generated time (ps) for Cerenkov photons",100,0.0,TLitPhys::Get()->TooLate());
  Add("TH1D","G_WvlgthCer","Generated wavelength (nm) for Cerenkov photons",100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  // fNfluo being unknown at this stage, it is impossible to fully preset histogram
  //"TimeEach". fNfluo is replaced by 1
  Add("TH2F","G_TimeEach","Generated time for each component",64,0.0,TLitPhys::Get()->TooLate(),1,0.0,1.0);
  Add("TH2F","G_WvlgthEach","Generated wavelength (nm) for each component",64,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL(),1,0.0,1.0);
  Add("TH1D","G_Momentum","Momentum [Gev/c] of generated particles",100,0.0,100.0);
  Add("TH1D","G_Theta","Theta distribution of generated particles",80,0.0,3.2);
  // nshapes being unknown at this stage, it is impossible to fully preset histogram
  //"EinShape". nshapes is replaced by 1, fgEnergyOfGamma by 1.0
  Add("TH2F","G_EinShape","Energy [Mev] deposited by gamma in all shapes",1,0.0,1.0,50,0.0,1.0);
  // Histogram "MatToMat", "ReflByMat", "MatToRev", "RevToMat" cannot be preset before
  // knowing the geometry
  //
  // TLitMarrow
  //
  // Histograms of TLitMarrow cannot be preset
  //
  //
  // TLitDetector::BookHistos
  //
  Add("TH1","TimeSeen_","",200,0.0,TLitPhys::Get()->TooLate());
  Add("TH1","WvlgthSeen_","",200,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1","WvlgthNot_","",200,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1","QEff_","",200,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1","Inside_","",200,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  Add("TH1","Depth_","",200,0.0,0.005);
  Add("TH2","XYEntrance_","",40,-0.1,0.1,40,-0.1,0.1);
  //
  fgListHParams->Write("histparams",1);
  fgHistParams->Close();
  delete fgHistParams;
  fgHistParams   = 0;
  fgHistParamsOk = kTRUE;
}
TLitHistParams *TLitHistParams::Get(const char *name) {
  // Find element with fHName == name
  Bool_t found = kFALSE;
  Int_t N,nlow,nmid,nup,k,nw;
  TString s = name;
  TLitHistParams *p = 0;
  if (fgHistParamsOk) {
    N = fgListHParams->GetEntries();
    if (N>1) {
      nw = 1;
      while (nw<N) nw *= 2;
      nlow = 0;
      nmid = nw/2;
      nw   = nmid;
      nup  = N-1;
      while ((!found) && (nw>=0)) {
        p = (TLitHistParams*)(*fgListHParams)[nmid];
        k = s.CompareTo(p->fHName);
        if (nw>0) nw  /=  2;
        else      nw   = -1;
        switch (k) {
      case -1:
        nup  = nmid;
        nmid = nlow + nw;
        break;
      case  0:
        found = kTRUE;
        break;
      case  1:
        nlow = nmid;
        nmid = nlow + nw;
        break;
        }
      }
    }
    else {
      p = (TLitHistParams*)(*fgListHParams)[0];
      k = s.CompareTo(p->fHName);
      if (k!=0) p=0;
    }
    if (!found) p = 0;
  }
  return p;
}
Bool_t TLitHistParams::IsEqual(const TObject *obj) const {
  Int_t k = fHName.CompareTo(((TLitHistParams*)obj)->fHName);
  return k==0;
}
void TLitHistParams::OpenXmlFileR() {
  // Open xml file and read it
  Int_t N;
  TString s,s2;
  if (!fgFileName) fgFileName = new TString("histparams.xml");
  s2 = fgFileName->Data();
  fgHistParamsOk = kFALSE;
  if (fgListHParams) {
    fgListHParams->Delete();
    delete fgListHParams;
    fgListHParams = 0;
  }
  s = gSystem->FindFile(".",s2);
  if (s.Length()<=10) TLitHistParams::CreateHistParams();
  fgHistParams = TFile::Open(fgFileName->Data(),"read");
  if (fgHistParams) {
    if (fgListHParams) {
      fgListHParams->Delete();
      delete fgListHParams;
      fgListHParams = 0;
    }
    fgListHParams = (TObjArray*)fgHistParams->Get("histparams");
    if (fgListHParams) {
      fgHistParamsOk = kTRUE;
      N = fgListHParams->GetEntries();
    }
  }
}
void TLitHistParams::Purge() {
// The collection fgListHParams is purged
  if (!fgListHParams) fgListHParams = new TObjArray();
  fgListHParams->Delete();
}
void TLitHistParams::PurgeStatic() {
  // delete all static pointers
  if (fgHistParams) {
    fgHistParams->Close();
    delete fgHistParams;
    fgHistParams   = 0;
  }
  if (fgListHParams) {
    fgListHParams->Delete();
    delete fgListHParams;
    fgListHParams  = 0;
  }
  if (fgFileName) {
    delete fgFileName;
    fgFileName = 0;
  }
}
