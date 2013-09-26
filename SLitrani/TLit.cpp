// @(#)root/html:$Id: TLit.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGClient.h"
#include "TGeoManager.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLitHistParams.h"
#include "TLitEqIndex.h"
#include "TLitPhys.h"
#include "TLitSpectrum.h"
#include "TLitResults.h"
#include "TLitMarrow.h"
#include "TLit.h"

Short_t  TLit::fgSize         = 5;
Int_t    TLit::fgNbButtons    = 20;
Int_t    TLit::fgTopXGen      = 2;
Int_t    TLit::fgTopYGen      = 2;
Bool_t   TLit::fgWithDate     = kFALSE;
TString* TLit::fgDate         = 0;
TString* TLit::fgTime         = 0;
TString* TLit::fgDateTime     = 0;
TString* TLit::fgRootFileName = 0;
TString* TLit::fgListFileName = 0;
Short_t  TLit::fgLineSize     = 119;
TLit*    TLit::fgLit          = 0;

ClassImp(TLit)
//______________________________________________________________________________
//
//  Steering class of SLitrani
//  TLit is a singleton. It means that by construction of the class:
//
//    - constructor and destructor being private
//    - pointer on the class TLit::fgLit being private
//    - static public method TLit::Get() to get pointer TLit::fgLit
//
//  it is impossible to create more than one instance of the class. The way
//to access public variables and methods of the class is by using static
//public method TLit::Get(). For instance, to book the canvas, call:
//
//    TLit::Get()->BookCanvas();
//
//  The constructor of TLit makes use of the following parameters, which are
//static private variables:
//
//    Short_t  TLit::fgSize         = 5;
//           Size of main canvas of TwoPadDisplay [ 1 <= fgSize <= 8 ]
//    Int_t    TLit::fgNbButtons    = 20;
//           Number of buttons wanted in TwoPadDisplay [ 12<=fgNbButtons<=48 ] 
//    Int_t    TLit::fgTopXGen      = 2;
//           Top x  of general canvas fCanvas
//    Int_t    TLit::fgTopYGen      = 2;
//           Top y  of general canvas fCanvas
//    Bool_t   TLit::fgWithDate     = kFALSE;
//           Date and time are appended or not to the name of the ROOT and listing
//    TString* TLit::fgRootFileName = 0;
//           Name of .root file created by SLitrani
//    TString* TLit::fgListFileName = 0;
//           Name of .lis  file created by SLitrani
//    Short_t  TLit::fgLineSize     = 119;
//           Length of line in output and listing. Default 119 char
//
//  These static variables have default values which can be changed, but only
//before the constructor is called, i.e. before any call to method TLit::Get()
//or TLit::Book(). See the static methods Set... concerning these variables
//and also static method TLit::SetParameters().
//
TLit::TLit() {
// Constructor of the class TLit
//
//  Notice method SaveFitSpec() handling class variable fSaveFitSpec. If
//fSaveFitSpec is true [default], then the fits of type TSplineFit and the
//spectra of type TLitSpectrum used are put on the ROOT file produced. If
//you do not want it, call SaveFitSpec(kFALSE) after the call to the constructor
//of TLit.
//
  InitP();
  Init();
  TSplineFit::InitStatic();
  InitRef();
  PublishVersion();
  if (!fgDateTime) SetDate();
  gStyle->SetHistFillColor(15);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleFillColor(191);
  gStyle->SetTitleTextColor(1);
  gStyle->SetLabelSize(0.03,"xyz");
//
//file name initialization
//
  if (!fgRootFileName) SetRootFileName();
  fLitBufSize  = 64000;
  fFilesClosed = kTRUE;
  if (!fgListFileName) SetListFileName();
//First initializes all physical constants used by SLitrani
  TLitPhys::Book();
  if (!TLitHistParams::fgFileName) TLitHistParams::fgFileName = new TString("histparams.xml");
  TLitHistParams::OpenXmlFileR();
//Book gLitIdx
  if (gLitIdx) delete gLitIdx;
  gLitIdx = new TLitEqIndex();
}
TLit::~TLit() {
//destructor
  if (fRef) delete fRef;
  CloseFiles();
  TLitHistParams::CloseXmlFile();
  if (!TLitPhys::Get()) TLitPhys::Kill();
  if (!gCleanOut) {
    delete gCleanOut;
    gCleanOut = 0;
  }
  fFitColl->Delete();
  TSplineFit::PurgeStatic();
  fFitColl = 0;
  TLitSpectrum::PurgeStatic();
  fSpecColl = 0;
  TLitHistParams::PurgeStatic();
}
void TLit::Book() {
  // Booking of fgLit
  if (!fgLit) fgLit = new TLit();
}
void TLit::BookCanvas() {
  // Book main canvas of SLitrani
  if (!gTwoPad) {
    if (!fRef) InitRef();
    new TwoPadDisplay(fRef,fgSize,fgNbButtons,fgTopXGen,fgTopYGen);
    gTwoPad->SetButtonFast(5,"Next Fit","TLit::Get()->fBTRchanged=kTRUE;TSplineFit::DrawNextInCollection();");
    gTwoPad->SetButtonFast(6,"============",";");
    gTwoPad->SetButtonFast(7,"Next Spectrum","TLitSpectrum::DrawNextInCollection();");
    gTwoPad->SetButtonFast(8,"RunDependent","gLitGp->N1();");
    gTwoPad->SetButtonFast(9,"Detectors","gLitGs->D1();");
    gTwoPad->SetButtonFast(10,"SumOfRuns","gLitGs->N1();");
    gTwoPad->SetButtonFast(11,"============",";");
    gTwoPad->UpdateCanvas();
  }
  if (fTRComment.Length()>0) gTwoPad->CommentTR(fTRComment.Data());
  if (fBRComment.Length()>0) gTwoPad->CommentBR(fBRComment.Data());
}
void TLit::BookCanvas(Short_t size) {
  // Book main canvas of SLitrani
  const Int_t safwidth  = 10;
  const Int_t safheight = 100;
  Double_t secor = (TMath::Sqrt(5.0) - 1.0)/2.0;
  UInt_t widthC = 600;  //Width  of this canvas
  UInt_t heigthC; //Heigth of this canvas
  Int_t w,h,wmax,hmax;
  size = TMath::Abs(size);
  if (size<1) size = 1;
  if (size>8) size = 8;
  w = gClient->GetDisplayWidth();
  h = gClient->GetDisplayHeight();
  switch (size) {
    case 1:
      //trying 600
      widthC = 600;
      break;
    case 2:
      //trying 800
      widthC = 800;
      break;
    case 3:
      //trying 1000
      widthC = 1000;
      break;
    case 4:
      //trying 1200
      widthC = 1200;
      break;
    case 5:
      //trying 1400
      widthC = 1400;
      break;
    case 6:
      //trying 1600
      widthC = 1600;
      break;
    case 7:
      //trying 1800
      widthC = 1800;
      break;
    case 8:
      //trying 2000
      widthC = 2000;
      break;
  }
  wmax = w - fgTopXGen - safwidth;
  hmax = h - fgTopYGen - safheight;
  heigthC = (Int_t)(secor*widthC);
  widthC  = TMath::Min((Int_t)widthC,wmax);
  heigthC = TMath::Min((Int_t)heigthC,hmax);
  if (!gTwoPad) {
    if (!fRef) InitRef();
    new TwoPadDisplay(fRef,fgNbButtons,fgTopXGen,fgTopYGen,widthC,heigthC);
    gTwoPad->SetButtonFast(5,"Next Fit","TLit::Get()->fBTRchanged=kTRUE;TSplineFit::DrawNextInCollection();");
    gTwoPad->SetButtonFast(6,"============",";");
    gTwoPad->SetButtonFast(7,"Next Spectrum","TLitSpectrum::DrawNextInCollection();");
    gTwoPad->SetButtonFast(8,"RunDependent","gLitGp->N1();");
    gTwoPad->SetButtonFast(9,"Detectors","gLitGs->D1();");
    gTwoPad->SetButtonFast(10,"SumOfRuns","gLitGs->N1();");
    gTwoPad->SetButtonFast(11,"============",";");
    gTwoPad->UpdateCanvas();
  }
  if (fTRComment.Length()>0) gTwoPad->CommentTR(fTRComment.Data());
  if (fBRComment.Length()>0) gTwoPad->CommentBR(fBRComment.Data());
}
void TLit::BookCanvas(Int_t widthC,Int_t heigthC) {
  // Book main canvas of SLitrani
  if (!gTwoPad) {
    widthC = TMath::Abs(widthC);
    heigthC = TMath::Abs(heigthC);
    if (!fRef) InitRef();
    new TwoPadDisplay(fRef,fgNbButtons,fgTopXGen,fgTopYGen,widthC,heigthC);
    gTwoPad->SetButtonFast(5,"Next Fit","TLit::Get()->fBTRchanged=kTRUE;TSplineFit::DrawNextInCollection();");
    gTwoPad->SetButtonFast(6,"============",";");
    gTwoPad->SetButtonFast(7,"Next Spectrum","TLitSpectrum::DrawNextInCollection();");
    gTwoPad->SetButtonFast(8,"RunDependent","gLitGp->N1();");
    gTwoPad->SetButtonFast(9,"Detectors","gLitGs->D1();");
    gTwoPad->SetButtonFast(10,"SumOfRuns","gLitGs->N1();");
    gTwoPad->SetButtonFast(11,"============",";");
    gTwoPad->UpdateCanvas();
  }
  if (fTRComment.Length()>0) gTwoPad->CommentTR(fTRComment.Data());
  if (fBRComment.Length()>0) gTwoPad->CommentBR(fBRComment.Data());
}
Short_t TLit::CheckPath(TString &path) const {
  //  Check the string returned by TGeoManager::GetPath(). Returns the number
  // of '/' found , i.e. the depth of the path. Returns 0 in case of anomaly
  const char slash = '/';
  Int_t k,L;
  Short_t depth = 0;
  L = path.Length();
  if ((L>1) && (path[0]==slash)) {
    for (k=0;k<L;k++)
      if (path[k]==slash) depth++;
  }
  return depth;
}
void TLit::CloseFiles() {
//To be called after all runs of generation of photons. Close files open
//for writing. Do not forget this important call at the end of your macros.
//
 if (!fFilesClosed) {
    Int_t Ns;
    gLitGs->Conclusion();
    fLitFile->cd();
    if (fLitTree) fLitTree->Write();
    gLitGs->Write();
    if (gLitGp) gLitGp->Write();
    if (fSaveFitSpec) {
      TSplineFit::WriteCollection();
      if (TLitSpectrum::fgSpectra) {
        Ns = TLitSpectrum::fgSpectra->GetEntries();
        if (Ns) TLitSpectrum::fgSpectra->Write("AllSpectra",1);
      }
    }
    fLitFile->Close();
    delete fLitFile;
    fLitFile     = 0;
    fLitTree     = 0;
    fLitBranch   = 0;
    fLitNb       = 0;
    fFilesClosed = kTRUE;
    gROOT->cd();
  }
}
void TLit::CloseFilesR() {
//To close the .root when it was opened for reading.
//
  if (!fFilesClosed) {
    fLitFile->cd();
    fLitFile->Close();
    delete fLitFile;
    fLitFile = 0;
    fLitNb = 0;
	  fFilesClosed = kTRUE;
    gROOT->cd();
  }
}
Bool_t TLit::GetWCSofNode(const char *path,TVector3 &source,TVector3 &dir) {
  // Given a path like "/T_1/A_2/B_2", returns the (x,y,z) coordinates in WCS
  //of the point (0,0,0) in local coordinate system of "/T_1/A_2/B_2"
  // Returns also coordinates in WCS of point source, given in local coordinate
  //system, and direction in WCS of dir, given in local coordinate system
  const char lim = '/';
  Short_t k,j,n,nn,N;
  Bool_t found1 = kFALSE;
  Bool_t found;
  Double_t local1[3],master1[3],localdir[3],masterdir[3];
  TGeoVolume *vol;
  TGeoNode *node;
  TGeoNode *daughter = 0;
  TGeoNode *foundnode[10];
  TObjArray *nodes;
  TString snode,sdaughter;
  TString s = path;
  N  = s.Length();
  for (k=0;k<3;k++) {
    local1[k]   = source[k];
    localdir[k] = dir[k];
  }
  k  = 0;
  n  = 0;
  nn = 2;
  while ((k<N) && (n<nn)) {
    if (s(k) == lim) n++;
    k++;
  }
  nn++; 
  vol = gGeoManager->GetTopVolume();
  found = kTRUE;
  while ((found) && (k<N)) {
    snode = "";
    while ((k<N) && (n<nn)) {
      if (s(k) == lim) n++;
      else snode.Append(s(k)); 
      k++;
    }
    nodes = vol->GetNodes();
    TIter next(nodes);
    found = kFALSE;
    while ((!found) && (node = (TGeoNode*)next())) {
      if (!snode.CompareTo(node->GetName())) {
        daughter = node;
        foundnode[nn-3] = node;
        found = kTRUE;
      }
    }
    if (found) vol = daughter->GetVolume();
    nn++;
  }
  found1 = found;
  nn -= 3;
  if (found1) {
    for (k=0;k<nn;k++) {
      node = foundnode[nn-k-1];
      node->LocalToMaster(local1,master1);
      node->LocalToMasterVect(localdir,masterdir);
      for (j=0;j<3;j++) {
        local1[j] = master1[j];
        localdir[j] = masterdir[j];
      }
    }
    for (j=0;j<3;j++) {
      source[j] = master1[j];
      dir[j]    = masterdir[j];
    }
  }
  return found1;
}
void TLit::Init() {
//Set default values in all variables
  fLitBufSize    = 64000;
  fFilesClosed   = kTRUE;
}
void TLit::InitP() {
//Set all pointers to 0
  fRef          = 0;
  fLitFile      = 0;
  fLitTree      = 0;
  fLitBranch    = 0;
  fFitColl      = 0;
  fSpecColl     = 0;
  fBTRchanged   = kFALSE;
  fTRComment    = "";
  fBRComment    = "";
}
void TLit::InitRef() {
  // Enter references of program
  TAuthor *a, *b;
  if (fRef) delete fRef;
  fRef = new TReference("SLitrani","Monte-Carlo of optical photons");
  fRef->SetVersion("1_4");
  fRef->SetWebSite("http://irfu.cea.fr/pisp/gentit/SLitrani/");
  fRef->SetLanguage("C++");
  fRef->SetFramework("ROOT");
  a = new TAuthor("Gentit","François-Xavier");
  a->AddAddress("DAPNIA/SPP CEA Saclay");
  a->AddMail("gentit@hep.saclay.cea.fr");
  a->AddWebSite("http://gentit.home.cern.ch/gentit/");
  a->AddCountry("France");
  fRef->AddAuthor(a);
  b = new TAuthor("Wahl","David");
  b->AddAddress("UNI, Lima, Peru");
  b->AddMail("david.n.wahl@gmail.com");
  b->AddCountry("Peru");
  fRef->AddAuthor(b);
}
void TLit::Kill() {
  // Only way to delete fgLit! This, combined with the fact that constructor
  //and destructor are private, makes of TLit a singleton
  if (fgLit) {
    delete fgLit;
    fgLit = 0;
  }
}
void TLit::OpenFilesR() {
// Open for read the .root file that SLitrani has created and whose name is
//in fgRootFileName.
  fLitNb = 0;
  fLitFile   = new TFile(fgRootFileName->Data(),"READ");
  gLitGs = (TLitResults *)fLitFile->Get("GlobStat;1");
  gLitGp = (TLitMarrow *)fLitFile->Get("Summary;1");
  fNFits     = 0;
  fKFit      = 0;
  fNSpecs    = 0;
  fKSpec     = 0;
  fFitColl   = 0;
  fFitColl   = (TObjArray *)fLitFile->Get("AllFits");
  if (fFitColl) fNFits     = fFitColl->GetEntries();
  fSpecColl  = 0;
  fSpecColl  = (TObjArray *)fLitFile->Get("AllSpectra");
  if (fSpecColl) fNSpecs    = fSpecColl->GetEntries();
  fLitTree   = (TTree *)fLitFile->Get("T");
  if (gLitCs) {
    delete gLitCs;
    gLitCs = 0;
  }
  if (fLitTree) {
    gLitCs = new TLitResults();
    fLitBranch = fLitTree->GetBranch("runstat");
    fLitBranch->SetAddress(&gLitCs);
  }
  fFilesClosed = kFALSE;
 }
void TLit::OpenFilesR(const char *name) {
  //  Open for read the .root file that SLitrani has created. "name" is the
  //name of the file, without the suffix ".root". Default "slitrani".
  SetRootFileName(name);
  OpenFilesR();
}
void TLit::OpenFilesW(Bool_t withTree) {
//  Open for writing the .root file that SLitrani will create. "name" is the
//name of the file, without the suffix ".root". Default "slitrani".
  fLitNb = 0;
  fLitFile   = new TFile(fgRootFileName->Data(),"RECREATE");
  if (withTree) {
    fLitTree   = new TTree("T","Statistics and Histos for each run");
    fLitBranch = fLitTree->Branch("runstat","TLitResults",&gLitCs,fLitBufSize,0);
  }
	fFilesClosed = kFALSE;
}
void TLit::PrintPeriod(Long64_t period) {
  // Set period with which photon number will be printed
  gCleanOut->SetPeriod2(period);
}
void TLit::PublishVersion() const {
// Initial Welcome and version number
  cout << endl;
  cout << "  *******************************************" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *      W E L CO M E to L I T R A N I      *" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *            Version ";
  cout << fRef->GetVersion();
  cout << "                  *" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *******************************************" << endl;
  cout << endl;
}
void TLit::RestoreComments() {
  // Restore default comments if fBTRchanged is true
  if (gTwoPad && fBTRchanged) {
    gTwoPad->CommentBR(fBRComment.Data());
    gTwoPad->CommentTR(fTRComment.Data());
    fBTRchanged = kFALSE;
  }
}
Bool_t TLit::SelectRun(Int_t run) {
  //  Given the run number, make the current statistics pointer gLitCs point
  // towards this run. If there are many different runs with different
  // names but identical run number, use the other version of SelectRun !
  //  This method is NOT intended to be used during the generation of photons,
  // between 2 runs. It is intended to be used :
  //    (1) - after all runs have been produced,
  //    (2) - after TLit::Get()->CloseFiles() has been called,
  //    (3) - and before calling it, you must have opened the .root file for
  //           reading by a call to TLit::Get()->OpenFilesR().
  //
  const char *met = "SelectRun";
  const char *classname = "TLit";
  Int_t k,nentries,krun;
  krun = 0;
  if (fFilesClosed) {
    gCleanOut->MM(error,met,"You must first open the .root file calling OpenFilesR",classname);
    gCleanOut->MM(warning,met,"We try to call OpenFilesR with argument <slitrani>",classname);
    OpenFilesR();
  }
  if (fLitTree) {
    nentries = (Int_t)fLitTree->GetEntries(); 
    k = 0;
    fLitNb = 0;
    while ((krun != run) && (k<nentries)) {
      fLitNb += fLitTree->GetEntry(k,1);
      krun = gLitCs->GetRun();
      k++;
    }
  }
  else gCleanOut->MM(warning,met,"Tree of individual runs not on .root file",classname);
  return (krun == run);
}
Bool_t TLit::SelectRun(TString name, Int_t run) {
  //  Given the name of the run and the run number, make the current
  // statistics pointer gLitCs point towards the statistics and histograms of
  // the corresponding run. If you have many different sources of photons,
  // having each a different name, it is quite possible that you have twice
  // the same run number, each belonging to a different source of photon.
  // If it is the case in your set-up, call this instance of SelectRun,
  // not the preceeding one.
  //  Notice that name is the name of the source of photons having generated
  // this run.
  //  This method is NOT intended to be used during the generation of photons,
  // between 2 runs. It is intended to be used :
  //    (1) - after all runs have been produced,
  //    (2) - after TLit::Get()->CloseFiles() has been called,
  //    (3) - and before calling it, you must have opened the .root file for
  //           reading by a call to TLit::Get()->OpenFilesR().
  //
  const char *met = "SelectRun";
  const char *classname = "TLit";
  Int_t k,nentries,krun;
  TString fullname;
  TString srun = "";
  krun = 0;
  if (fFilesClosed) {
    gCleanOut->MM(error,met,"You must first open the .root file calling OpenFilesR",classname);
    gCleanOut->MM(warning,met,"We try to call OpenFilesR with argument <slitrani>",classname);
    OpenFilesR();
  }
  //construct the full name with run number
  if (run<10000) srun.Append('0');
  if (run<1000) srun.Append('0');
  if (run<100) srun.Append('0');
  if (run<10) srun.Append('0');
  srun += run;
  fullname = name;
  fullname.Append('_');
  fullname.Append(srun);
  if (fLitTree) {
    nentries = (Int_t)fLitTree->GetEntries(); 
    k = 0;
    fLitNb = 0;
    while (((krun != run) || (srun.CompareTo(fullname))) && (k<nentries)) {
      fLitNb += fLitTree->GetEntry(k,1);
      krun = gLitCs->GetRun();
      srun = gLitCs->GetName();
      k++;
    }
  }
  else gCleanOut->MM(warning,met,"Tree of individual runs not on .root file",classname);
  return (krun == run);
}
void TLit::SetDate() {
  //Finds the date of to-day
  if (!fgLit) {
    const Int_t cent = 100;
    Int_t date,time;
    Int_t day,month,year;
    Int_t hour,minute,second;
    TDatime td;
    date   = td.GetDate();
    time   = td.GetTime();
    //
    day    = date % cent;
    date  /= cent;
    month  = date % cent;
    date  /= cent;
    year   = date;
    second = time % cent;
    time  /= cent;
    minute = time % cent;
    time  /= cent;
    hour   = time;
    //
    if (fgDate) delete fgDate;
    fgDate  = new TString("  ");
    *fgDate += day;
    fgDate->Append(" / ");
    *fgDate += month;
    fgDate->Append(" / ");
    *fgDate += year;
    fgDate->Append("  ");
    //
    if (fgTime) delete fgTime;
    fgTime  = new TString("");
    *fgTime += hour;
    fgTime->Append('_');
    *fgTime += minute;
    if (fgDateTime) delete fgDateTime;
    fgDateTime = new TString(fgDate->Data());
    fgDateTime->ReplaceAll("  ","");
    fgDateTime->ReplaceAll(' ',"");
    fgDateTime->ReplaceAll('/','_');
    fgDateTime->Append('_');
    fgDateTime->Append(fgTime->Data());
    fgDateTime->Prepend('_');
  }
  else cout <<"Warning: try changing fgDateTime after fgLit booked" << endl;
}
void TLit::SetLineSize(Short_t n) {
  if (!fgLit) {
    if (n<79)  n = 79;
    if (n>300) n = 300;
    fgLineSize = n;
  }
  else cout << "Warning: try changing fgLineSize after fgLit booked" << endl;
}
void TLit::SetListFileName(const char *name) {
  //  Allow to change the name of the listing [default slitrani.lis] created by
  //SLitrani. Do not give the suffix ".lis", it will be appended. Do not
  //change the name of the listing between runs ! Do it before the generation
  //of the first photon.
  if (!fgLit) {
    if (!fgDate) SetDate();
    if (fgListFileName) delete fgListFileName;
    fgListFileName = new TString(name);
    if (fgWithDate) fgListFileName->Append(fgDateTime->Data());
    if (gCleanOut) delete gCleanOut;
    gCleanOut = new TCleanOut(fgListFileName->Data(),"lis",fgLineSize);
    fgListFileName->Append(".lis");
  }
  else cout << "Warning: try changing listing filename after fgLit booked" << endl;
}
void TLit::SetNbButtons(Int_t n) {
  // Defines Number of buttons wanted in TwoPadDisplay. Bigger than 12 [default 20]
  if (!fgLit) {
    n = TMath::Abs(n);
    if (n < 12) n = 12;
    if (n > 48) n = 48;
    fgNbButtons = n;
  }
  else cout << "Warning: try changing fgNbButtons after fgLit booked" << endl;
}
void TLit::SetParameters(Short_t s,Int_t n,Int_t x,Int_t y,Bool_t wd,const char *nr,const char *nl,Short_t ls) {
  // Set all parameters and book fgLit (no need to call Book().
  if (!fgLit) {
    SetSize(s);
    SetNbButtons(n);
    SetTopXGen(x);
    SetTopYGen(y);
    if (!fgDateTime) SetDate();
    SetLineSize(ls);
    SetWithDate(wd);
    SetRootFileName(nr);
    SetListFileName(nl);
    fgLit = new TLit();
  }
  else cout << "Warning: try changing parameters after fgLit booked" << endl;
}
void TLit::SetRootFileName(const char *name) {
  //  Allow to change the name of the I/O file [default slitrani.root] used by
  //SLitrani. Do not give the suffix ".root", it will be appended. Do not
  //change the name of the I/O file between runs ! Do it before the generation
  //of the first photon.
  if (!fgLit) {
    if (!fgDate) SetDate();
    if (fgRootFileName) delete fgRootFileName;
    fgRootFileName = new TString(name);
    if (fgWithDate) fgRootFileName->Append(fgDateTime->Data());
    fgRootFileName->Append(".root");
  }
  else cout << "Warning: try changing Root filename after fgLit booked" << endl;
}
void TLit::SetSize(Short_t size) {
  // Defines size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
  if (!fgLit) {
    fgSize = size;
    if (fgSize<1) fgSize = 1;
    if (fgSize>8) fgSize = 8;
  }
  else cout << "Warning: try changing fgSize after fgLit booked" << endl;
}
void TLit::SetTopXGen(Int_t x) {
  // x coordinate, in pixel, of top left corner of main canvas [default 2]
  if (!fgLit) fgTopXGen = x;
  else cout << "Warning: try changing fgTopXGen after fgLit booked" << endl;
}
void TLit::SetTopYGen(Int_t y) {
  // y coordinate, in pixel, of top left corner of main canvas [default 2]
  if (!fgLit) fgTopYGen = y;
  else cout << "Warning: try changing fgTopYGen after fgLit booked" << endl;
}
void TLit::SetWithDate(Bool_t b) {
  // Ask or not for date and time in file name
  if (!fgLit) fgWithDate = b;
  else cout << "Warning: try changing fgWithDate after fgLit booked" << endl;
}
void TLit::StatDef() {
// Definitions of the variables for the statistics of photons
  fStatItems     = 18;
  fSShortDef[0]  = "Generated";             //generated
  fSLongDef[0]   = "Nb. of photons generated";
  fSShortDef[1]  = "Emitted";                //emitted
  fSLongDef[1]   = "Nb. of photons generated - loss in fibre - abnorm.";
  fSShortDef[2]  = "Seen";                   //seen
  fSLongDef[2]   = "Nb. of photons seen";
  fSShortDef[3]  = "Lost";                   //lost
  fSLongDef[3]   = "Total of lost photons";
//
  fSShortDef[4]  = "In med. of NSD";         //inmedNSD
  fSLongDef[4]   = "Lost in any medium except media of surface detectors";
  fSShortDef[5]  = "In med. of SD";          //inmedSD
  fSLongDef[5]   = "Lost in media of surface detectors";
//
  fSShortDef[6]  = "Suppl. Wrap. ND";        //supplND
  fSLongDef[6]   = "Lost in wrapping by suppl. abs. except wrapping of detectors";
  fSShortDef[7]  = "Suppl. Wrap  D";         //supplD
  fSLongDef[7]   = "Lost in wrapping of detectors by suppl. absorption";
  fSShortDef[8]  = "Wrapping of ND";         //wrapND
  fSLongDef[8]   = "Lost in wrapping except wrapping of detectors";
  fSShortDef[9]  = "Wrapping of D";          //wrapD
  fSLongDef[9]   = "Lost in wrapping of detectors";
//
  fSShortDef[10]  = "Leaving setup";          //leaving
  fSLongDef[10]   = "Lost leaving the setup";
  fSShortDef[11] = "Too late";               //late
  fSLongDef[11]  = "Lost because too late";
  fSShortDef[12] = "Not enough e-";          //lacke
  fSLongDef[12]  = "Lost because too few e- emitted in APD";
  fSShortDef[13] = "Acceptance";             //accept
  fSLongDef[13]  = "Lost because of acceptance angle";
  fSShortDef[14] = "Lost by QE";             //QE
  fSLongDef[14]  = "Lost because of quantum efficiency";
  fSShortDef[15] = "Not emitted";            //notemitted
  fSLongDef[15]  = "Lost because not emitted by the fibre";
  fSShortDef[16] = "Non optical";            //nonoptical
  fSLongDef[16]  = "Lost because non optical medium encountered";
  fSShortDef[17] = "Abnormally";             //abnormal
  fSLongDef[17]  = "Lost abnormally";
}
