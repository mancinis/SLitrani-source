#include "Riostream.h"
int InitSLitrani(Short_t size=5,char *name = "",char *listing = "",char *upcom = "",
 char *downcom = "",Bool_t otherseq = kFALSE,Bool_t indebug = kFALSE,Bool_t WithDate = kFALSE)
{
//
// arguments of InitSLitrani:
//
// size     size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run SLitrani in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
//  It is a good idea to begin all CINT macros of SLitrani with a call to this macro InitSLitrani.C,
//which among other things defines the coordinates of the main canvas used by SLitrani.
  const Bool_t litdebug = kTRUE;
  Int_t kClass,kLoad1,kLoad2,kLoad3,kLoad4;
  ((TRint*)gROOT->GetApplication())->SetPrompt("slitrani [%d] ");
  if (litdebug) {
    cout << endl;
    cout << "size     : " << size << endl;
    cout << "name     : " << name << endl;
    cout << "listing  : " << listing << endl;
    cout << "upcom    : " << upcom << endl;
    cout << "downcom  : " << downcom << endl;
    cout << "otherseq : " << (Int_t)otherseq << endl;
    cout << "indebug  : " << (Int_t)indebug << endl;
    cout << "WithDate : " << (Int_t)WithDate << endl;
    cout << endl;
  }
// MABBOTT: Added explicit library loading
gSystem->Load("libGui");
gSystem->Load("libHtml");
gSystem->Load("libGuiHtml");
gSystem->Load("libEG");
gSystem->Load("libGeom");
gSystem->Load("libMathMore");
gSystem->Load("libSpectrum");
//  This first part is to cope with the fact that the user either has launched
//SLitrani, or has launched ROOT. In the second case, the shared libraries of SLitrani
//have to be loaded. So we detect whether shared library libSLitrani has to be loaded
//in bit 16 of gROOT.
//
  kClass = TClassTable::GetID("TLit");
  if (kClass<0) {
    kLoad1 = gSystem->Load("libTwoPad","",kTRUE);
    if (kLoad1) cout << "ERROR: Shared library libTwoPad not found" << endl;
    else if (litdebug) cout << "libTwoPad correctly loaded" << endl;
    kLoad2 = gSystem->Load("libSplineFit","",kTRUE);
    if (kLoad2) cout << "ERROR: Shared library libSplineFit not found" << endl;
    else if (litdebug) cout << "libSplineFit correctly loaded" << endl;
    kLoad3 = gSystem->Load("libPhysMore","",kTRUE);
    if (kLoad3) cout << "ERROR: Shared library libPhysMore not found" << endl;
    else if (litdebug) cout << "libPhysMore correctly loaded" << endl;
    kLoad4 = gSystem->Load("libSLitrani","",kTRUE);
    if (kLoad4) cout << "ERROR: Shared library libSLitrani not found" << endl;
    else if (litdebug) cout << "libSLitrani correctly loaded" << endl;
    if (litdebug) cout << "Start from ROOT, libSLitrani to be loaded" << endl;
  }
  else if (litdebug) cout << "Start from SLitrani, all libs loaded" << endl;
  TLit::Kill();
  if (name    == "") name    = "slitrani";
  if (listing == "") listing = "slitrani";
//
//  The 7 arguments of the static method TLit::SetParameters() are the following :
//
//    size    : size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
//    nbbut   : Number of buttons wanted in TwoPadDisplay. Bigger than 10.
//              [default 24]
//    xminC   : x coordinate, in pixel, of top left corner of main canvas
//              [default 2]
//    yminC   : y coordinate, in pixel, of top left corner of main canvas
//              [default 2]
//    WithDate: if true, date and time are appended to the name of the ROOT file
//              and of the listing, in order not to loose them when starting again
//              the job.
//              [default false]     
//    name    : the name of the .root file that SLitrani produces, containing
//              statistics and histograms of all runs. Do not give the suffix .root.
//              It will be appended.
//              [default "slitrani"]
//    listing : the name of the .lis file that SLitrani produces, containing
//              messages issued by slitrani. Do not give the suffix .lis . It will
//              be appended.
//              [default "slitrani"] 
//    ls:     : number of characters in a line of output or listing [default 119]
//
  cout << "Booking TLit" << endl;
  TLit::SetParameters(size,24,20,2,WithDate,name,listing);
//
// if otherseq true, other sequence of random number when restarting run
  if (otherseq) gRandom3->SetSeed(0);
//  TLit::Get()->SetTesTCalc(kTRUE) asks for many many checks in the computations.
  TLit::Get()->SetTestCalc(indebug);
//  TLitPhys::Get()->SetLate(50.0);
// Comments of Canvas
  if (upcom   != "") TLit::Get()->fTRComment = upcom;
  if (downcom != "") TLit::Get()->fBRComment = downcom;
  return 0;
}
