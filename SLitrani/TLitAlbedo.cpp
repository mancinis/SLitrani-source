// @(#)root/html:$Id: TLitAlbedo.cpp 2009-06-16
// Author: F.X. Gentit <http://gentitfx.web.cern.ch/gentitfx/> IRFU/SPP CEA Saclay

/*********************************************************************************
 * Copyright The same as the ones of ROOT                                        *
*********************************************************************************/
#include "TROOT.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLitSpontan.h"
#include "TLitAlbedo.h"

ClassImp(TLitAlbedo)
//________________________________________________________________________
//
//  Complex index of wrapping from albedo
//
/////////////////////////////////////////////////////////////////////////
//                                                                     //
//              DO NOT USE YET. IN PREPARATION!                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
//
//  In the way diffusion is calculated in SLitrani [look at the method
// TLitPhoton::Diffused()] it is tacitly assumed that you know real and
// imaginary part of the index of refraction of the diffusing wrapping.
// It is easy to find such indexes for reflecting wrappings in the Handbook
// of Optical Constants, but NOT for diffusing wrappings. For diffusing
// wrappings, it is more current to find the albedo of the wrapping. But it
// is not at all evident to go from the albedo to the real and imaginary
// part of the index of refraction! There is a problem here that I do not
// know how to solve. If you get an idea, please tell it to me. On the
// contrary, you can go from the real and imaginary part of the index to
// the albedo by running SLitrani and looking at the proportions of photons
// absorbed by the wrapping. As an example, the way the diffusing wrapping
// "tyvek" has been introduced into SLitrani reflects the difficulty quoted
// here : I was not able to find real and imaginary part of the index of
// tyvek, but I found the albedo. What I have done was first to give to
// tyvek the real and imaginary part of aluminium. Then I realized that it
// gave a worse albedo than the one quoted for tyvek. So I have decreased
// the real part of the index of aluminium until it gave the proper albedo
// for tyvek.
//  Class TLitAlbedo is intended to be an help in such a process. Given:
//  - the albedo of a diffusing wrapping
//  - an ARBITRARY chosen distribution of imaginary part of index of
//     refraction [by default the one of aluminium]
//  class TLitAlbedo finds the real part of index of refraction which will
// give the correct albedo for the wrapping. TLitAlbedo does this by
// constructing a setup which is a parallelipiped [TGeoBBox] contained
// inside an other parallelipiped [TGeoBBox] which is a 100% efficient
// detector. The face at -dx of the first parallelipiped is the wrapping
// for which we intend to find the complex index of refraction. TLitAlbedo
// generates photon of all wavelength betwwen 300 and 1000 nm, varying the
// real part of the index of refraction until it finds the value giving the
// good albedo. TLitAlbedo produces 2 text files which are the CINT code
// of the TSplineFit for the real and imaginary part of the index of
// refraction for the diffusing wrapping. It is up to the user to introduce
// later on these 2 TSpineFit into the database SplineFitDB.rdb.
//  It is important to understand that this way of going from albedo to
// complex index of refraction is NOT AT ALL strict on a physical point
// of view. This is because we provide in a totally arbitrary way the
// imaginary part of the index of refraction. This way of doing is justified
// only by the fact that SLitrani needs absolutly a complex index of
// refraction for handling diffused wrapping. If you know of a better way
// of doing, please let me know!
//  To activate TLitAlbedo, simply type the following CINT code: [in case
// you handle for instance vm2000 wrapping and you agree to use the imaginary
// part of the index of aluminium]
//
// ==> TLitAlbedo *L = new TLitAlbedo("vm2000","vm2000");
// ==> L->Go();
//
//  Notice that if you are not satisfied with any characteristics of the
// default setup used by TLitAlbedo, you can change it using any of the
// Set... methods. For instance, if you do not want steps of 10 degrees
// but want steps of 5 degrees, do:
//
// ==> TLitAlbedo *L = new TLitAlbedo("vm2000","vm2000");
// ==> L->SetStepAngle(5.0);
// ==> L->Go();
//
TLitAlbedo::TLitAlbedo(const char *name,const char *title,
  const char *ImIndex):TNamed(name,title) {
  // Constructor
  //
  // Arguments
  //
  // name           : name of class TLitAlbedo == name of the fit of type
  //                  TSplineFit [without the prefix "Albedo_"] giving the
  //                  albedo of the wrapping for which we aim to find the
  //                  real part of index
  // title          : title of class TLitAlbedo == title of the fit of type
  //                  TSplineFit [without the prefix "Albedo of wrapping | "]
  //                  giving the albedo of the wrapping for which we aim to
  //                  find the real part of index
  // ImIndex        : name [without prefix "IIndex_"] of ARBITRARY imaginary
  //                  part of the index of refraction of the wrapping "name"
  //                  for which TLitAlbedo is intended to find the real part
  //                  For instance: "Aluminium" if one intends to take the
  //                  SplineFit fit "IIndexRev_Aluminium".
  //                  "IIndexRev_"ImIndex"" must either be present in
  //                  SplineFitDB.rdb or "IIndexRev_"ImIndex".C" be present
  //                  in directory "FitMacros".
  //
  TString sname,cintline,stitle,sImIndex;
  InitP();
  Init();
  TLit::Kill();
  sname = "Albedo_";
  sname.Append(name);
  stitle = "Albedo of wrapping | ";
  stitle.Append(title);
  sImIndex = "IIndexRev_";
  sImIndex.Append(ImIndex);
  fAlbedo = TSplineFit::FindFit(sname.Data(),21,kTRUE);
  if (!fAlbedo) {
    cintline = sname;
    cintline.Prepend(".x ");
    cintline.Append(".C(kFALSE);");
    gROOT->ProcessLine(cintline.Data());
    fAlbedo = TSplineFit::FindFit(sname.Data(),21,kTRUE); 
  }
  fImPart = TSplineFit::FindFit(sImIndex.Data(),4,kTRUE);
  if (!fImPart) {
    cintline = sImIndex;
    cintline.Prepend(".x ");
    cintline.Append(".C(kFALSE);");
    gROOT->ProcessLine(cintline.Data());
    fImPart = TSplineFit::FindFit(sImIndex.Data(),4,kTRUE);
  }
}
TLitAlbedo::~TLitAlbedo() {
  // Destructor
  ClearP();
}
void TLitAlbedo::BuildGeometry() {
  //  Builds the whole setup for getting the real part of the index from
  // the albedo and from the arbitrary chosen imaginary part.
  //
  //  The setup consists of 4 TGeoBBox included inside oneanother
  //
  // (1) "SOURCE"   the TGeoBBox emitting photons. Filled with air.
  //                Dimensions [fL-fD,fL-fD,fL-fD]
  //                Translation of -0.5*fD with respect to DETECTOR
  // (2) "DETECTOR" a 100% efficient detector, surrounding completly
  //                "SOURCE", except for the face at -dx where "SOURCE"
  //                and "WRAPPING" are directly in contact. Filled with
  //                arbitrary TLitMedium "DetMed".
  //                Dimensions [fL-0.5*fD,fL,fL]
  //                Translation of +0.5*fD with respect to WRAPPING
  // (3) "WRAPPING" the wrapping for which we intend to produce the complex
  //                index of refraction. All faces of "WRAPPING" surrounds
  //                "DETECTOR", except for the face at -dx, where "WRAPPING"
  //                is directly in contact with "SOURCE". A photon can go
  //                from "SOURCE" to "WRAPPING" only through the face at -dx
  //                since "DETECTOR" is a 100% efficient detector.
  //                Dimensions [fL,fL,fL]
  //                No translation with respect to TOP
  // (1) "TOP"      fTop volume defining the world coordinate system,
  //                containing all other volumes
  //                Dimensions [fL+fD,fL+fD,fL+fD]
  //
  //  Since we define "DETECTOR" as a 100% efficient detector, the TLitMedium
  // it is made of does not matter. We name it "DetMed". We give to it the
  // same index of refraction as the one of "SOURCE", so that the photons
  // coming from "SOURCE" will always enter "DETECTOR". The given absorption
  // length "AbsLAir" for DETECTOR does not matter since stopalways is given
  // as true in the call fLitDet->SetDetector().
  //  The value (1.0,1.0) of the complex index of refraction in the call
  // fWrapMed->SetAsWrapping() is irrelevant since this value will be
  // changed later on by calls to TLitMedium::SetWrapIndex().
  //
  const Double_t IrrA           = 0.0;    //Value used for A   when irrelevant !
  const Double_t IrrZ           = 0.0;    //Value used for Z   when irrelevant !
  const Double_t IrrRho         = 0.0;    //Value used for Rho when irrelevant !
  const Double_t AbsLAir        = 10000.0;//Absorption length of air
  const Double_t Air_RefrIndex  = 1.0003; //Refractive index of air
  const Color_t  WrapColor      = 3;
  const Color_t  DetColor       = 1;
  const Color_t  SourceColor    =  2;
  Double_t D;   //0.5*fD
  Double_t LD;  //fL + fD
  Double_t LmD; //fL - fD
  Int_t mediumindex = 1;
  gCleanOut->fMaxInfo = 1000;
  TLit::Get()->fTRComment = fUpcom;
  TLit::Get()->fBRComment = fDowncom;
  new TGeoManager("ReFromAlbedo","Real part of index from albedo and Im part");
  //____________________________________________________________________________
  //
  // Materials and media
  //____________________________________________________________________________
  //
  fSrcMat    = new TGeoMaterial("Air",IrrA,IrrZ,IrrRho);
  fSrcMed    = new TLitMedium("Air",mediumindex++,fSrcMat,kFALSE,1.0,AbsLAir);
  fSrcMed->IsIsotropic(Air_RefrIndex);
  fDetMat    = new TGeoMaterial("DetMed",IrrA,IrrZ,IrrRho);
  fDetMed    = new TLitMedium("DetMed",mediumindex++,fDetMat,kTRUE,1.0,AbsLAir);
  fDetMed->IsIsotropic(Air_RefrIndex);
  fWrapMat   = new TGeoMaterial(GetName(),IrrA,IrrZ,IrrRho);
  fWrapMed   = new TLitMedium(GetName(),mediumindex++,fWrapMat,-1.0);
  fWrapMed->SetAsWrapping(fWrapDiffus,1.0,1.0,1.0);
  fVacuumMat = new TGeoMaterial("Vacuum",IrrA,IrrZ,IrrRho);
  fVacuum    = new TGeoMedium("Vacuum",mediumindex++,fVacuumMat);
  //____________________________________________________________________________
  //
  // Positionning (translations)
  //____________________________________________________________________________
  //
  D   = 0.5*fD;
  LD  = fL + fD;
  LmD = fL - fD;
  fT1 = new TGeoTranslation("t1",D,0.0,0.0);
  fT1->RegisterYourself();
  fT2 = new TGeoTranslation("t2",-D,0.0,0.0);
  fT2->RegisterYourself();
  //____________________________________________________________________________
  //
  // Volumes and nodes
  //____________________________________________________________________________
  //
  fTop      = gGeoManager->MakeBox("TOP",fVacuum,LD,LD,LD);
  gGeoManager->SetTopVolume(fTop);
  fWrapping = gGeoManager->MakeBox("WRAPPING",fWrapMed,fL,fL,fL);
  fTop->AddNode(fWrapping,1);
  fDetector = gGeoManager->MakeBox("DETECTOR",fDetMed,fL-D,fL,fL);
  fWrapping->AddNode(fDetector,1,fT1);
  fLitDet   = new TLitVolume(fDetector);
  fLitDet->SetDetector(kTRUE,"",91.0);
  fSource   = gGeoManager->MakeBox("SOURCE",fSrcMed,LmD,LmD,LmD);
  fDetector->AddNode(fSource,1,fT2);
  fLitSource = new TLitVolume(fSource);
  fSourcePoint.SetXYZ(fDistPoint,0.0,0.0);
  fSourceDir.SetXYZ(TMath::Cos(fStartAngle),TMath::Sin(fStartAngle),0.0);
  fLitSource->SetEmission(flat,fAperture,fSourceDir,"",kTRUE,fSourcePoint);
  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.01);
  //
  //____________________________________________________________________________
  //
  // Colors and drawing
  //____________________________________________________________________________
  //
  fTop->SetVisibility(kFALSE);
  fTop->SetVisContainers();
  fTop->SetLineColor(1);
  fTop->SetLineWidth(1);
  fWrapping->SetLineColor(WrapColor);
  fWrapping->SetLineWidth(1);
  fDetector->SetVisibility(kTRUE);
  fDetector->SetLineColor(DetColor);
  fDetector->SetLineWidth(1);
  fSource->SetVisibility(kTRUE);
  fSource->SetLineColor(SourceColor);
  fSource->SetLineWidth(1);
  gGeoManager->SetVisLevel(4);
  TLit::Get()->BookCanvas();
  gTwoPad->SetAllGrey();
  fTop->Draw("");
}
void TLitAlbedo::ClearP() {
  // Delete all pointers
  if (fAlbedo) {
    delete fAlbedo;
    fAlbedo = 0;
  }
  if (fImPart) {
    delete fImPart;
    fImPart = 0;
  }
  if (fWaveLength) {
    delete [] fWaveLength;
    fWaveLength = 0;
  }
}
void TLitAlbedo::Gen() {
  //  Generates the photon at various wavelengths and with various values
  // of real part of index until the good value is found
  Int_t k,kw,kt,kr;
  Double_t wavelength;   //wavelength tried
  Double_t realvalue;    //tried value for real part of index
  Double_t imvalue;      //tried value for imaginary part of index
  Double_t albedo;       //value of albedo at wavelength
  //  TLitSpontan *S;        //spontaneous source of photons
  //  for (kw=0;kw<fNWaveLength;kw++) {
  for (kw=0;kw<1;kw++) {
    Double_t startreal;  //first value to be tried for real part of index
    Double_t stopreal;   //last value to be tried for real part of index
    Double_t stepreal;   //step in the list of values to be tried for real part of index
    Double_t *realval;   //list of values to be tried for real part of index
    wavelength = fWaveLength[kw];
    albedo     = fAlbedo->V(wavelength);
    imvalue    = fImPart->V(wavelength);
    startreal  = 0.0;
    stopreal   = fMaxReal;
    realval  = new Double_t [fNbStepsReal];
    //we approach the good value in 3 steps
    for (kt=0;kt<3;kt++) {
      stepreal = (stopreal-startreal)/fNbStepsReal;
      for (k=0;k<fNbStepsReal;k++) realval[k] = k*stepreal;
      for (kr=0;kr<fNbStepsReal;kr++) {
        //go through list of real values to tes
        realvalue = realval[kr];
        fWrapMed->SetWrapIndex(realvalue,imvalue);
      }
    }//end for (kt=0;kt<3;kt++)
    delete realval;
    realval = 0;
  }//end for (kw=0;kw<fNWaveLength;kw++)
}
Double_t TLitAlbedo::GetStartAngle() const {
  //  Returns start angle for the emission of photons in degree
  return (180.0*fStartAngle)/TMath::Pi();
}
Double_t TLitAlbedo::GetWavelength(Int_t k) const {
  //  Get kth used wavelength
  Double_t w = 0.0;
  k = TMath::Abs(k);
  if (k<fNWaveLength) w = fWaveLength[k];
  return w;
}
void TLitAlbedo::Go() {
  // Generates setup and then photons
  TLit::SetParameters(fSize,fNbButtons,fXminC,fYminC,fWithDate,fNameFile.Data(),fListFile.Data());
  if (gGeoManager) delete gGeoManager;
  BuildGeometry();
  Gen();
}
void TLitAlbedo::Init() {
  //  Default values
  const Double_t eps   = 0.0001;
  const Double_t torad = TMath::Pi()/180.0;
  const Double_t safe  = 10.0;
  fWrapDiffus  = 1.0;
  fSize        = 4;
  fNbButtons   = 24;
  fXminC       = 2;
  fYminC       = 2;
  fWithDate    = kTRUE;
  fNameFile    = "albedo";
  fListFile    = "albedo";
  fUpcom       = "Finding real part of index of ";
  fUpcom.Append(GetName());
  fDowncom     = "from albedo and imaginary part";
  fL           = 10.0;
  fD           = 1.0;
  fAperture    = 5.0;
  fDistPoint   = -(fL-2*fD);
  fStartAngle  =  -(safe*torad + TMath::PiOver2());
  fLastAngle   =  -(270.0-safe)*torad;
  fStepAngle   =  -safe*torad;
  fNAngles     = Int_t((-fLastAngle+fStartAngle+eps)/TMath::Abs(fStepAngle));
  fNAngles++;
  fMaxReal     = 4.0;
  fNbStepsReal = 9;
  SetDefWaveL();
}
void TLitAlbedo::InitP() {
  // All pointers to 0
  fAlbedo     = 0;
  fImPart     = 0;
  fSrcMat     = 0;
  fSrcMed     = 0;
  fDetMat     = 0;
  fDetMed     = 0;
  fWrapMat    = 0;
  fWrapMed    = 0;
  fVacuumMat  = 0;
  fVacuum     = 0;
  fT1         = 0;
  fT2         = 0;
  fTop        = 0;
  fWrapping   = 0;
  fDetector   = 0;
  fLitDet     = 0;
  fSource     = 0;
  fLitSource  = 0;
  fWaveLength = 0;
}
void TLitAlbedo::PrintAngles() const {
  const char *met = "PrintAngles";
  gCleanOut->MMR(info,met,"Start angle","fStartAngle",fStartAngle,ClassName());
  gCleanOut->MMR(info,met,"End   angle","fLastAngle",fLastAngle,ClassName());
  gCleanOut->MMR(info,met,"Step","fStepAngle",fStepAngle,ClassName());
  gCleanOut->MMI(info,met,"Number of steps","fNAngles",fNAngles,ClassName());
  gCleanOut->MMR(info,met,"X coord of emission point in SOURCE","fDistPoint",fDistPoint,ClassName());
}
void TLitAlbedo::SetDefWaveL() {
  //  Set default values for wavelengths used. You can change these
  // default values calling SetWaveL()
  fNWaveLength = 39;
  if (fWaveLength) {
    delete [] fWaveLength;
    fWaveLength = 0;
  }
  fWaveLength = new Double_t [fNWaveLength];
  fWaveLength[ 0] = 300.0;
  fWaveLength[ 1] = 310.0;
  fWaveLength[ 2] = 320.0;
  fWaveLength[ 3] = 330.0;
  fWaveLength[ 4] = 340.0;
  fWaveLength[ 5] = 350.0;
  fWaveLength[ 6] = 360.0;
  fWaveLength[ 7] = 370.0;
  fWaveLength[ 8] = 380.0;
  fWaveLength[ 9] = 390.0;
  fWaveLength[10] = 400.0;
  fWaveLength[11] = 410.0;
  fWaveLength[12] = 420.0;
  fWaveLength[13] = 430.0;
  fWaveLength[14] = 440.0;
  fWaveLength[15] = 450.0;
  fWaveLength[16] = 460.0;
  fWaveLength[17] = 470.0;
  fWaveLength[18] = 480.0;
  fWaveLength[19] = 490.0;
  fWaveLength[20] = 500.0;
  fWaveLength[21] = 510.0;
  fWaveLength[22] = 520.0;
  fWaveLength[23] = 530.0;
  fWaveLength[24] = 540.0;
  fWaveLength[25] = 550.0;
  fWaveLength[26] = 560.0;
  fWaveLength[27] = 570.0;
  fWaveLength[28] = 580.0;
  fWaveLength[29] = 590.0;
  fWaveLength[30] = 600.0;
  fWaveLength[31] = 650.0;
  fWaveLength[32] = 700.0;
  fWaveLength[33] = 750.0;
  fWaveLength[34] = 800.0;
  fWaveLength[35] = 850.0;
  fWaveLength[36] = 900.0;
  fWaveLength[37] = 950.0;
  fWaveLength[38] = 1000.0;
}
void TLitAlbedo::SetDistPoint(Double_t x) {
  //  Set x coordinate of point of emission inside SOURCE in the local
  // coordinate system of SOURCE. We must have -(fL-fD) < x < +(fL-fD)
  const Double_t eps = 1.0e-6;
  Double_t d = fL-fD;
  if (x<-d) x = -d + eps;
  if (x> d) x =  d - eps;
  fDistPoint = x;
}
void TLitAlbedo::SetNAngles(Int_t n) {
  //  Set the number of steps for the emission angle between fStartAngle
  // and fLastAngle. Must be odd.
  n = TMath::Abs(n);
  if (!(n%2)) n++;
  fNAngles   = n;
  fStepAngle = (fLastAngle-fStartAngle)/(fNAngles-1);
  PrintAngles();
}
void TLitAlbedo::SetStartAngle(Double_t d) {
  //  Set start angle for photon emission in degree. The direction of emission
  // of the beam of photons being towards -Ox, the start angle d must be
  // negativ and smaller than -90.0. Since the beam of photons has an
  // aperture of fAperture in degree, we must require in fact, in order that
  // each photon is directed towards face at -Ox, that:
  //
  //   -180 + fAperture <  d < -90 - fAperture     or:
  //     90 + fAperture < -d < 180 - fAperture
  //
  //  The emission angle fEmitAngle will vary between:
  //
  //    fStartAngle + fNAngles*fStepAngle <= fEmitAngle <= fStartAngle
  //
  // or if you prefer: [fStartAngle, fEmitAngle, fStepAngle are all negativ]
  //
  //    -fStarAngle <= -fEmitAngle <= -fStartAngle - fNAngles*fStepAngle
  //
  const Double_t torad = TMath::Pi()/180.0;
  Double_t safe;
  Double_t limL = -180.0 + fAperture;
  Double_t limH =  -90.0 - fAperture;
  d = -TMath::Abs(d);
  if (d<limL) d = limL;
  if (d>limH) d = limH;
  safe = TMath::Abs(d) - 90.0;
  fStartAngle = d*torad;
  fLastAngle  = (-270.0 + safe)*torad;
  fStepAngle  = (fLastAngle - fStartAngle)/(fNAngles-1);
  PrintAngles();
}
void TLitAlbedo::SetWaveL(Int_t N,Double_t *w) {
  //  Replaces the default list of wavelength by a new one
  //
  //  N : number of wavelength in the new list
  //  w : list of wavelength in increasing order
  //
  Int_t k;
  N = TMath::Abs(N);
  fNWaveLength = N;
  if (fWaveLength) {
    delete [] fWaveLength;
    fWaveLength = 0;
  }
  fWaveLength = new Double_t [fNWaveLength];
  for (k=0;k<fNWaveLength;k++) fWaveLength[k] = w[k];
}
