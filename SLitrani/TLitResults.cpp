// @(#)root/html:$Id: TLitResults.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TStyle.h"
#include "TGeoManager.h"
#include "TwoPadDisplay.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitPhys.h"
#include "TLitHistParams.h"
#include "TLitMedium.h"
#include "TLitDetector.h"
#include "TLitResults.h"

TLitResults* gLitGs = 0;
TLitResults* gLitCs = 0;
Double_t TLitResults::fgEnergyOfGamma = 2.0;

ClassImp(TLitResults)
//______________________________________________________________________________
//
//
//  GENERAL PRESENTATION OF THE HISTOGRAMS AND STATISTICS AVAILABLE
//                      IN LITRANI
//
//  There are 5 kinds of histograms in SLitrani. At the end of running SLitrani,
// you have at hand the main canvas produced by TwoPadDisplay. This canvas is
// provided with buttons that allows you to display all the histograms produced
// by pushing buttons. It allows also to display all the fits of type TSplineFit
// and all the spectra of type TLitSpectrum used in the job. If you want to see
// the histograms of a previous job, SLitrani produces a .root file that you can
// open using the program "VisuLitrani", provided with SLitrani, and see again all
// histograms by hitting the same buttons. So that you do not need in fact all
// the explanations provided below! They are there only if you want to manipulate
// the histograms from the CINT code.
//
//    (1) - HISTOGRAMS ASSOCIATED WITH FITS AND INTERPOLATION.
//
//  All fits and interpolations of type TSplineFit that you create can be stored
// [it is recommended!]into the database "SplineFitDB.rdb". Look at the documentation
// of TSplineFit at:
// http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html
// If you want to have the list of all fits present, launch SplineFit (or SLitrani)
// and type:
//
//   ==> TSplineFit::ShowFitsInFile();
//
// If you want to draw a particular fit, do:
//
//   ==> TSplineFit *fit = TSplineFit::FindFit("AbsorptionLength_Silicium");
//   ==> fit->DrawFit();
//
// If you want to draw all fits present, in turn:
//
//   ==> TSplineFit::DrawFitsInFile();
//
//
//    (2) - HISTOGRAMS ASSOCIATED WITH EMISSION SPECTRA.
//
//  All spectra of type TLitSpectrum that you create can be stored
// [it is recommended!] into the database "SpectraDB.rdb". Look at the documentation
// of TLitSpectrum at:
// http://gentit.home.cern.ch/gentit/litrani/AllModules/LitraniCode/TLitSpectrum.html
// If you want to have the list of all spectra present, launch SLitrani and type:
//
//   ==> TLitSpectrum::ShowSpectraInFile();
//
// If you want to draw a particular spectrum, do:
//
//   ==> TLitSpectrum *sp = TLitSpectrum::FindSpectrum("Spectrum_CsITl");
//   ==> sp->DrawSpectrum();
//
// If you want to draw all spectra present, in turn:
//
//   ==> TLitSpectrum::DrawSpectraInFile();
//
//
//    (3) - HISTOGRAMS RELATED TO ONE SPECIFIC RUN
//
//  Each time you call the method Gen()  [ TLitSpontan::Gen() or TLitBeam::Gen()
// or TLitCascade::Gen()], you have the possibility to ask for histograms specific
// to the run you are launching. These histograms are defined in the class TLitResults
// (this one !). For a list of the histograms available, look at the public
// declarations of class TLitResults. These histograms are pointed to by the
// global pointer gLitCs. How to make gLitCs point towards the run you want to see
// will be explained below.
//
//    (4) - HISTOGRAMS SUM OF ALL RUNS
//
//  These histograms are the same as the one defined in (3), except that they contain
// the sum of all runs. They are also defined in the class TLitResults, with a run
// number fRun = 0. They are accessed using the pointer gLitGs.
//
//    (5) - HISTOGRAMS SHOWING QUANTITIES AS A FUNCTION OF A RUN DEPENDENT
//          PARAMETER
//
//  For instance :
//   - Efficiency of detection of photons as a function of position of the beam (if
//     beam position is what you have varied from run to run ).
//   - Loss in material as a function of tilt angle of the fibre ( if tilt angle of
//     fibre is what you have varied from run to run ).
//   - etc
//  These histograms are defined in the class TLitMarrow. See this class for
// more details. They are pointed to by the global pointer gLitGp.
//
//  Here, in the class TLitResults, we are interested only in the histograms
// of kind (3) and (4). Let us turn now to the question of finding the value
// of the pointers gLitGs and gLitCs.
//
//    FINDING THE WANTED POINTER TOWARDS STATISTICS AND HISTOGRAMS
//
//    (a) - If you are interested in the global statistics for all runs,
// histograms of kind (4), you have the pointer gLitGs at hand. No search is
// necessary. For instance, you can look at the histogram giving the distri-
// bution in time of all photons seen in all runs by :
//  ==> gLitGs->fHTimeSeen->Draw();
// or look simply at all these histograms by :
//  ==> gLitGs->N();
//  ==> gLitGs->D();
//
//    (b) - If you are interested in the statistics of the current run
// ( the last run achieved at this time ), you have the pointer gLitCs at hand.
// No search necessary. For instance, you can look at the histogram giving
// the distribution in time of all photons seen in this last run by :
//  ==> gLitCs->fHTimeSeen->Draw();
// or look simply at all the histograms of this last run by :
//  ==> gLitCs->N();
//  ==> gLitCs->D();
//
//    (c) - if you are interested neither in (a) nor in (b), but you wish
// to access, after the generation of all runs, after having closed the .root
// file by a call to CloseFiles(), the statistics and histograms of a given
// run of run number = irun, proceed like this :
//  ==> TLit::Get()->OpenFilesR("litrani");      //Open the .root file for read
//  ==> Bool_t b = TLit::Get()->SelectRun(irun); //now gLitCs points towards
//                                               //run irun if b true.
//  You can now for instance look at all histograms of run irun by :
//  ==> gLitCs->N();
//
//    (d) - In the case where you have many different sources of photons,
// each with a different name, the possibility arises that you have many
// different runs, with DIFFERENT names but IDENTICAL run number. In that
// case, use the method TLit::Get()->SelectRun(name, irun), name being the name of
// the source of photons having generated the run you are interested in.
//   Notice however that it is not a good idea to give twice the same run
// number even in the case where the name of the source of photons has
// changed : you would have problems using the class TLitMarrow, except in
// the case where you have defined the parameter "xparam" of the method
// Gen(), where everything is ok. Look at the method Gen() [ TLitSpontan::Gen()
// or TLitBeam::Gen() or TLitCascade::GEn() ].
//
//
//    DETECTOR HISTOGRAMS
//
//  Histograms of kind (3) or (4) have sub-histograms not appearing in the
// class declaration of TLitResults. These are the histograms relative to de-
// tectors, which are contained in the class TLitDetector. They are stored in
// the TObjArray TLitDetector::fgLitDetector. You must realize that in the
// setup used by SLitrani, you may have ANY number of detectors, and that these
// detectors may be of 3 kinds:
//    (a) - General volume detectors.
//    (b) - Phototubes.
//    (c) - APD
//  For more informations on detectors, look at the classes TLitVolume and
// TLitDetector.
//
//
//    LOOKING AT ALL HISTOGRAMS OF KIND (3) or (4)
//
//  Very often, at the end of a production, one does not want to see
// particular histograms, but simply ALL histograms, in order to fully check
// that everything was ok. For this, there exists the extremely simple
// methods 
//
//      N() : for looking at all histograms of a run (without the detector
//            histograms).
//      D() : for looking at all detector histograms of a run
//
//
//    ROOT FILE CONTAINING ALL STATISTICS AND HISTOGRAMS
//
//  This file has the default name "litrani.root". You can change this 
// default name by calling the static method TLit::SetParameters() or
// TLit::SetRootFileName();
//
//  At the end of SLitrani, if you open the .root file and list it by:
//
//     ==> TFile *f = new TFile("litrani.root","READ");
//     ==> f->ls();
//
//  you will get :
//
//  => TFile*         litrani.root
//  => KEY: TTree    T;1     Statistics and Histos for each run
//  => KEY: TLitResults GlobStat;1      Global statistics of all runs
//  => KEY: TLitMarrow     Summary;1       tilt angle of the fibre
//
//  You see that the file is composed of :
//
//  (1) "T"        : a TTree containing statistics and histograms of each
// individual runs, under the form of objects of class TLitResults. You look at
// the content of this tree using TLit::Get()->SelectRun() and the pointer gLitCs.
//  (2) "GlobStat" : an object of class TLitResults containing the global
// sum of the statistics and histograms of all runs. You look at it using
// the pointer gLitGs.
//  (3) "Summary"  : an object of class TLitMarrow containing histograms
// of quantities varying as a function of a run parameter. You look at it
// using the pointer gLitGp.
//
//  Notice that using the program "VisuLitrani" is by far the easiest way
//of looking at all the histograms and render this lengthy description
//obsolete!
//

TObjArray *TLitResults::fgResults = 0;

TLitResults::TLitResults(const char *name, const char *title, Int_t run,
                   Double_t xparam) : TNamed(name,title) {
//constructor for this class of statistics and summary for run "name" of run
//number run. If run == 0, this is the global statistics of all runs.
  const Double_t xparamlim = -1.0e+20;
	Bool_t b;
  if (!fgResults) fgResults = new TObjArray();
  Init();
  fRun  = run;
  if ((xparam>xparamlim) || (!fRun)) fXparam = xparam;
  else                               fXparam = fRun;
  if (!fRun) {
    if (!TLitDetector::fgLitDetGlob) TLitDetector::fgLitDetGlob = new TObjArray();
    fListOfDet = TLitDetector::fgLitDetGlob;
    GInit();
  }
  else {
    TLitDetector::fgLitDetRun = new TObjArray();
    fListOfDet = TLitDetector::fgLitDetRun;
    CInit();
  }
	b = AddToList();
}
TLitResults::~TLitResults() {
  // TLitResults is owner of TLitDetector::fgLitDetGlob and TLitDetector::fgLitDetRun
  DelPaveT();
  DelHistos();
  if (fRun) {
    if (TLitDetector::fgLitDetRun) {
      TLitDetector::fgLitDetRun->Delete();
      delete TLitDetector::fgLitDetRun;
      TLitDetector::fgLitDetRun  = 0;
    }
  }
  else {
    if (TLitDetector::fgLitDetGlob) {
      TLitDetector::fgLitDetGlob->Delete();
      delete TLitDetector::fgLitDetGlob;
      TLitDetector::fgLitDetGlob = 0;
    }
  }
  fListOfDet = 0;
  fgResults->Remove(this);
}
Bool_t TLitResults::AddToList() {
//Add one Results to the list fResults.
  Int_t i;
  TString s1,s2;
  TLitResults *p;
  TIter next(fgResults);
  s1 = GetName();
  while ((p = (TLitResults *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
        fgResults->Remove(p);
        gCleanOut->MM(warning,"AddToList","twice same name for results",ClassName());
    }
  }
  fgResults->Add(this);
  return kTRUE;
}
Short_t TLitResults::AtLeastOne() {
//if there is fluorescent materials in the setup, verifies that at least one
//is set so that its fluorescent characteristics be histogrammed
//kNfluo is the number of fluorescent components
	Bool_t ThereIs = kFALSE;
	Bool_t found = kFALSE;
	Int_t khist = 0;
	TLitMedium *pmat;
	TLitMedium *pmats = 0;
	Short_t kNfluo = 0;
	TIter next(TLitMedium::fgLitMedium);
	while ((!ThereIs) && (pmat = (TLitMedium *)next()))
		if (pmat->IsFluorescent()) ThereIs = kTRUE;
	if (ThereIs) {
		next.Reset();
		while ((pmat = (TLitMedium *)next()))
			if ((pmat->IsFluorescent()) && (pmat->GetHistFluo())) {
				khist += 1;
				if (khist == 1) pmats = pmat;
				if (khist>1) {
					pmat->SetHistFluo(kFALSE);
					khist -= 1;
				}
			//end if ((pmat->IsFluorescent() && (pmat->GetHistFluo()))
			}
		if (!khist) {
			next.Reset();
			while ((!found) && (pmat = (TLitMedium *)next())) {
				if (pmat->IsFluorescent()) {
					found = kTRUE;
					pmat->SetHistFluo(kTRUE);
					pmats = pmat;
				//end if (pmat->IsFluorescent())
				}
			//end while ((!found) && (pmat = (TLitMedium *)next()))
			}
		//end if (!khist)
		}
		if (pmats) kNfluo = pmats->GetFluorescent();
	//end if (ThereIs)
	}
	return kNfluo;
}
void TLitResults::CInit() {
//Groups everything common to all constructors
	Axis_t late,MinW,MaxW;
  TLitHistParams *h;
  TAxis *xaxis1,*xaxis2,*xaxis3,*xaxis4,*xaxis5,*xaxis6,*xaxis7,
        *yaxis1,*yaxis2,*yaxis3,*yaxis4;
	late = TLitPhys::Get()->TooLate();
	MinW = TLitPhys::Get()->MinWaveL();
	MaxW = TLitPhys::Get()->MaxWaveL();
	fNpGener   = 0;
  fNbHistos  = 0;
//Detection of photons
  fNpSeen    = 0;
  fPhweigth  = 0.0;
  h = TLitHistParams::Get("Points");
  if (h) fHPoints   = new TH1D("Points",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPoints   = new TH1D("Points","Number of points on photon trajectory",250,0.0,250.0);
  fNbHistos++;
  h = TLitHistParams::Get("TimeSeen");
  if (h) fHTimeSeen = new TH1D("TimeSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,late);
	else   fHTimeSeen = new TH1D("TimeSeen","time of arrival (ps) of photons on detector",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("DistSeen");
  if (h) fHDistSeen = new TH1D("DistSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHDistSeen = new TH1D("DistSeen","distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("WvlgthSeen");
  if (h) fHWvlgthSeen = new TH1D("WvlgthSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthSeen = new TH1D("WvlgthSeen","Wavelength (nm) of photons seen",100,MinW,MaxW);
  fNbHistos++;
//Loss of photons
  fNpLossAny = 0;
  fNpAbsMat  = 0;
  fNpAbsBef  = 0;
  fNpAbsRvt  = 0;
  fNpOutSide = 0;
	fNpAbnorm  = 0;
  fNpTooLate = 0;
  fNpTooFew  = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
  fNpAbsL0   = 0;
  h = TLitHistParams::Get("AbsAny");
  if (h) fHAbsAny = new TH1D("AbsAny",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHAbsAny = new TH1D("AbsAny","distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("OutSide");
  if (h) fHOutSide = new TH1D("OutSide",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHOutSide = new TH1D("OutSide","distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("AbnLost");
  if (h) fHAbnLost = new TH1D("AbnLost",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHAbnLost = new TH1D("AbnLost","distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("TooLate");
  if (h) fHTooLate = new TH1D("TooLate",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHTooLate = new TH1D("TooLate","Wavelength of photons seen, but too late",100,MinW,MaxW);
  fNbHistos++;
//Diffusion of photons
  h = TLitHistParams::Get("Diffusion");
  if (h) fHDiffusion = new TH1D("Diffusion",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHDiffusion = new TH1D("Diffusion","Number of diffusions per photon",16,0.0,16.0);
  fNbHistos++;
//Time and Wavelength
  h = TLitHistParams::Get("TimeAll");
  if (h) fHTimeAll = new TH1D("TimeAll",h->fHTitle,h->fHNbinsx,h->fHXlow,late);
	else   fHTimeAll = new TH1D("TimeAll","Generated time (ps) for all components",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("WvlgthAll");
  if (h) fHWvlgthAll = new TH1D("WvlgthAll",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthAll = new TH1D("WvlgthAll","Generated wavelength (nm) for all components",100,MinW,MaxW);
  fNbHistos++;
  h = TLitHistParams::Get("TimeCer");
  if (h) fHTimeCer = new TH1D("TimeCer",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHTimeCer = new TH1D("TimeCer","Generated time (ps) for Cerenkov photons",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("WvlgthCer");
  if (h) fHWvlgthCer = new TH1D("WvlgthCer",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthCer = new TH1D("WvlgthCer","Generated wavelength (nm) for Cerenkov photons",100,MinW,MaxW);
  fNbHistos++;
	fNfluo = AtLeastOne();
	fHTimeEach   = 0;
	fHWvlgthEach = 0;
  if (fNfluo) {
    Axis_t afluo = fNfluo;
    h = TLitHistParams::Get("TimeEach");
    if (h) fHTimeEach = new TH2F("TimeEach",h->fHTitle,h->fHNbinsx,h->fHXlow,late,fNfluo,0.0,afluo);
    else   fHTimeEach = new TH2F("TimeEach","Generated time for each component",64,0.0,late,fNfluo,0.0,afluo);
    fNbHistos++;
    h = TLitHistParams::Get("WvlgthEach");
    if (h) fHWvlgthEach = new TH2F("WvlgthEach",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup,fNfluo,0.0,afluo);
    else   fHWvlgthEach = new TH2F("WvlgthEach","Generated wavelength (nm) for each component",64,MinW,MaxW,fNfluo,0.0,afluo);
    fNbHistos++;
  }
//Particle generation
  fNbPart = 0;
  h = TLitHistParams::Get("Momentum");
  if (h) fHPMomentum = new TH1D("Momentum",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPMomentum = new TH1D("Momentum","Momentum [Gev] of generated particles",100,0.0,100.0);
  fNbHistos++;
  h = TLitHistParams::Get("Theta");
  if (h) fHPTheta = new TH1D("Theta",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPTheta = new TH1D("Theta","Theta distribution of generated particles",80,0.0,3.2);
  fNbHistos++;
  Int_t kvol;
  TGeoVolume *v;
  TIter next1(gGeoManager->GetListOfVolumes());
  Int_t nshapes = gGeoManager->GetListOfVolumes()->GetEntries();
  h = TLitHistParams::Get("EinShape");
  if (h) fHEnergyInShape = new TH2F("EinShape",h->fHTitle,nshapes,0.0,(Double_t)nshapes,h->fHNbinsy,0.0,fgEnergyOfGamma);
  else   fHEnergyInShape = new TH2F("EinShape","Energy [Mev] deposited by gamma in all shapes",nshapes,0.0,(Double_t)nshapes,50,0.0,fgEnergyOfGamma);
  fNbHistos++;
  xaxis1 = fHEnergyInShape->GetXaxis();
  kvol = 0;
  while ((v = (TGeoVolume*)next1())) {
    kvol++;
    xaxis1->SetBinLabel(kvol,v->GetName());
  }
  fHEnergyInShape->SetStats(0);
  fHEnergyInShape->LabelsOption("v");
//Transitions
  Int_t kmat,krev;
  Int_t nmat = 0;
  Int_t nrev = 0;
  TLitMedium *p;
  TIter next(TLitMedium::fgLitMedium);
  while ((p = (TLitMedium *)next())) {
    if (p->GetKind() == OpticWrapping) nrev++;
    else                               nmat++;
  }
  fHAbsMat = new TH1D("AbsMat","Nb. of photons absorbed in various material",nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHAbsBef = new TH1D("AbsBef","Nb. of photons absorbed before various wrappings",nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHAbsRvt = new TH1D("AbsRvt","Nb. of photons absorbed by various wrappings",nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHMatMat = new TH2F("MatToMat","Transition from material to material",
    nmat,0.0,(Double_t)nmat,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHMatNot = new TH2F("ReflByMat","Reflection from material by material",
    nmat,0.0,(Double_t)nmat,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHMatRev = new TH2F("MatToRev","Absorption by wrapping when coming from mat",
    nmat,0.0,(Double_t)nmat,nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHRevMat = new TH2F("RevToMat","Reflection by wrapping when coming from mat",
    nrev,0.0,(Double_t)nrev,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  xaxis1 = fHMatMat->GetXaxis();
  yaxis1 = fHMatMat->GetYaxis();
  xaxis2 = fHMatRev->GetXaxis();
  yaxis2 = fHMatRev->GetYaxis();
  xaxis3 = fHRevMat->GetXaxis();
  yaxis3 = fHRevMat->GetYaxis();
  xaxis4 = fHMatNot->GetXaxis();
  yaxis4 = fHMatNot->GetYaxis();
  xaxis5 = fHAbsMat->GetXaxis();
  xaxis6 = fHAbsBef->GetXaxis();
  xaxis7 = fHAbsRvt->GetXaxis();
  next.Reset();
  while ((p = (TLitMedium *)next())) {
    if (p->GetKind() == OpticWrapping) {
      krev = p->GetRevCounter();
      yaxis2->SetBinLabel(krev,p->GetName());
      xaxis3->SetBinLabel(krev,p->GetName());
      xaxis6->SetBinLabel(krev,p->GetName());
      xaxis7->SetBinLabel(krev,p->GetName());
    }
    else {
      kmat = p->GetMatCounter();
      xaxis1->SetBinLabel(kmat,p->GetName());
      yaxis1->SetBinLabel(kmat,p->GetName());
      xaxis2->SetBinLabel(kmat,p->GetName());
      yaxis3->SetBinLabel(kmat,p->GetName());
      xaxis4->SetBinLabel(kmat,p->GetName());
      yaxis4->SetBinLabel(kmat,p->GetName());
      xaxis5->SetBinLabel(kmat,p->GetName());
    }
  }
  fHMatMat->SetStats(0);
  fHMatNot->SetStats(0);
  fHMatRev->SetStats(0);
  fHRevMat->SetStats(0);
  fHAbsMat->LabelsOption("v");
  fHAbsBef->LabelsOption("v");
  fHAbsRvt->LabelsOption("v");
  fHMatMat->LabelsOption("v");
  fHMatNot->LabelsOption("v");
  fHMatRev->LabelsOption("v");
  fHRevMat->LabelsOption("v");
}
Int_t TLitResults::Compare(const TObject *obj) const {
  if (fXparam>((TLitResults *)obj)->fXparam)
    return 1;
  else if (fXparam<((TLitResults *)obj)->fXparam)
    return -1;
  else return 0; 
}
void TLitResults::Conclusion() {
  //Last calculations before using the class
  if (!fConclusion) {
    Int_t n;
    n = fListOfDet->GetEntries();
    if (n) {
      Int_t i;
      TLitDetector *pdet;
      TIter next(fListOfDet);
      for (i=0;i<n;i++) {
        pdet  = (TLitDetector *)next();
        pdet->Conclusion();
      }
    }
    fConclusion = kTRUE;
  }
}
void TLitResults::D() {
  //displays next detector histogram if the user type "n", exit otherwise
  Int_t k,M,ntot;
  TLitDetector *det;
  if (fNdet<0) {
    fNdet = fListOfDet->GetEntries();
  }
  if (fNdet) {
    char c='n';
    if (!gTwoPad) TLit::Get()->BookCanvas();
    det  = (TLitDetector*)(*fListOfDet)[0];
    ntot = fNdet*TLitDetector::fgMaxHist;
    while (c == 'n' ) {
      k = 0;
      M = 0;
      while ((!M) && (k<ntot)) {
        M = D1();
        k++;
      }
      if (k>=ntot) {
        gCleanOut->MM(warning,"D","No histo to plot",ClassName());
        c = 'q';
      }
      else {
        cout << "Press n to see next histo, any other key to quit" << endl;
        cin >> c;
      }
    }
  }
  else gCleanOut->MM(error,"D","No detector in setup",ClassName());
}
Int_t TLitResults::D1() {
  //Display all detector histograms, in turn
  Int_t M = 0;
  TLitDetector *det;
  if (fNdet<0) fNdet = fListOfDet->GetEntries();
  if (fNdet>0) {
    Bool_t pub = kFALSE;
    det = (TLitDetector*)(*fListOfDet)[fKDet];
    while (!pub) {
      switch (fKD1) {
      case 0:
        if (det->fHTimeSeen) {
          M = Int_t(det->fHTimeSeen->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            gStyle->SetOptStat(111111);
            det->fHTimeSeen->Draw();
          }
        }
        break;
      case 1:
        if (det->fHWvlgthSeen) {
          M = Int_t(det->fHWvlgthSeen->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHWvlgthSeen->Draw();
          }
        }
        break;
      case 2:
        if (det->fHWvlgthNot) {
          M = Int_t(det->fHWvlgthNot->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHWvlgthNot->Draw();
          }
        }
        break;
      case 3:
        if (det->fHQEff) {
          M = Int_t(det->fHQEff->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHQEff->Draw();
          }
        }
        break;
      case 4:
        if (det->fHInside) {
          M = Int_t(det->fHInside->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHInside->Draw();
          }
        }
        break;
      case 5:
        if (det->fHDepth) {
          M = Int_t(det->fHDepth->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHDepth->Draw();
          }
        }
        break;
      case 6:
        if (det->fHXYEntrance) {
          M = Int_t(det->fHXYEntrance->GetEntries());
          if (M) {
            gTwoPad->ChangePad();
            pub = kTRUE;
            det->fHXYEntrance->Draw();
          }
        }
        break;
      default:
        fKD1 = 0;
        M    = 0;
        break;
      }//end switch (fKD1)
      fKD1++;
      if (fKD1 >= TLitDetector::fgMaxHist) {
        fKD1   = 0;
        fKDet += 1;
        if (fKDet >= fNdet) fKDet = 0;
        det = (TLitDetector*)(*fListOfDet)[fKDet];
      }//end if (fKD1 >= TLitDetector::fgMaxHist)
    }//end while (!pub)
  }//end if (fNdet>0)
  return M;
}
void TLitResults::DelHistos() {
//Delete all histograms
  Int_t Ndet;
  TLitDetector *det;
  if (fHPoints) {
    delete fHPoints;
    fHPoints = 0;
  }
  if (fHTimeSeen) {
    delete fHTimeSeen;
    fHTimeSeen = 0;
  }
  if (fHDistSeen) {
    delete fHDistSeen;
    fHDistSeen = 0;
  }
  if (fHWvlgthSeen) {
    delete fHWvlgthSeen;
    fHWvlgthSeen = 0;
  }
  if (fHAbsAny) {
    delete fHAbsAny;
    fHAbsAny = 0;
  }
  if (fHAbsMat) {
    delete fHAbsMat;
    fHAbsMat = 0;
  }
  if (fHAbsBef) {
    delete fHAbsBef;
    fHAbsBef = 0;
  }
  if (fHAbsRvt) {
    delete fHAbsRvt;
    fHAbsRvt = 0;
  }
  if (fHOutSide) {
    delete fHOutSide;
    fHOutSide = 0;
  }
  if (fHAbnLost) {
    delete fHAbnLost;
    fHAbnLost = 0;
  }
  if (fHTooLate) {
    delete fHTooLate;
    fHTooLate = 0;
  }
  if (fHDiffusion) {
    delete fHDiffusion;
	  fHDiffusion = 0;
  }
  if (fHTimeAll) {
    delete fHTimeAll;
    fHTimeAll = 0;
  }
  if (fHTimeEach) {
    delete fHTimeEach;
    fHTimeEach = 0;
  }
  if (fHWvlgthAll) {
    delete fHWvlgthAll;
    fHWvlgthAll = 0;
  }
  if (fHWvlgthEach) {
    delete fHWvlgthEach;
    fHWvlgthEach = 0;
  }
  if (fHTimeCer) {
    delete fHTimeCer;
    fHTimeCer = 0;
  }
  if (fHWvlgthCer) {
    delete fHWvlgthCer;
    fHWvlgthCer = 0;
  }
  if (fHPMomentum) {
    delete fHPMomentum;
    fHPMomentum = 0;
  }
  if (fHPTheta) {
    delete fHPTheta;
    fHPTheta = 0;
  }
  if (fHEnergyInShape) {
    delete fHEnergyInShape;
    fHEnergyInShape = 0;
  }
  if (fHMatMat) {
    delete fHMatMat;
    fHMatMat = 0;
  }
  if (fHMatNot) {
    delete fHMatNot;
    fHMatNot = 0;
  }
  if (fHMatRev) {
    delete fHMatRev;
    fHMatRev = 0;
  }
  if (fHRevMat) {
    delete fHRevMat;
    fHRevMat = 0;
  }
//Detector histograms
  Ndet = fListOfDet->GetEntries();
  TIter next(fListOfDet);
  while ((det = (TLitDetector*)next())) {
    det->ClearHistos();
  }
}
void TLitResults::DelPaveT() {
//Delete the TPaveLabel and the TPaveStat of DoStat()
  if (fPavel) {
    delete fPavel;
    fPavel = 0;
  }
  if (fPavet) {
    delete fPavet;
    fPavet = 0;
  }
}
void TLitResults::DoStat() {
//Shows photon statistics
  const Double_t un   = 1.0;
  const Double_t x1pl = 0.1;
  const Double_t y1pl = 0.9;
  const Double_t x2pl = un - x1pl;
  const Double_t y2pl = 0.98;
  const Double_t x1pt = 0.05;
  const Double_t y1pt = 0.05;
  const Double_t x2pt = un - x1pt;
  const Double_t y2pt = 0.85;
  TString s,sf;
  TString stitle;
  Double_t num,den,eff,def;
  stitle = GetName();
  DelPaveT();
  fPavel = new TPaveLabel(x1pl,y1pl,x2pl,y2pl,stitle.Data(),"BRNDCARC");
  fPavel->SetBorderSize(5);
  fPavel->SetFillColor(41);
  fPavel->SetTextColor(2);
  fPavet = new TPaveText(x1pt,y1pt,x2pt,y2pt,"BRNDCARC");
  fPavet->SetBorderSize(8);
  fPavet->SetFillColor(21);
  fPavet->SetTextAlign(12);
  fPavet->SetTextFont(101);
  fPavet->SetTextSize(0.03);
  s  = "Nb. of photons generated   : ";
  s += fNpGener;
  fPavet->AddText(s.Data());
  s  = "Lost for abnormal reasons  : ";
  s += fNpAbnorm;
  fPavet->AddText(s.Data());
  s  = "Lost because abs. length 0 : ";
  s += fNpAbsL0;
  fPavet->AddText(s.Data());
  s  = "Eff. nb. of gen. photons   : ";
  s += fNpGener  - fNpAbnorm - fNpAbsL0;
  fPavet->AddText(s.Data());
  s  = "Nb. of photons seen        : ";
  s += fNpSeen;
  fPavet->AddText(s.Data());
  s  = "Efficiency                 : ";
  num = fNpSeen;
  den = fNpGener  - fNpAbnorm - fNpAbsL0;
  eff = num/den;
//  def = TMath::Sqrt(num)/den;
  def = TMath::Sqrt((eff*(1.0-eff))/den);
  sf = Form("%10.6g",eff);
  s.Append(sf);
  fPavet->AddText(s.Data());
  s  = "   error                   :  +/-";
  sf = Form("%10.6g",def);
  s.Append(sf);
  fPavet->AddText(s.Data());
  s  = "Lost for any reason        : ";
  s += fNpLossAny;
  fPavet->AddText(s.Data());
  s  = "Lost in materials          : ";
  s += fNpAbsMat;
  fPavet->AddText(s.Data());
  s  = "Lost before wrapping       : ";
  s += fNpAbsBef;
  fPavet->AddText(s.Data());
  s  = "Lost in wrapping           : ";
  s += fNpAbsRvt;
  fPavet->AddText(s.Data());
  s  = "Lost leaving setup         : ";
  s += fNpOutSide;
  fPavet->AddText(s.Data());
  s  = "Lost because too late      : ";
  s += fNpTooLate;
  fPavet->AddText(s.Data());
  s  = "Lost b. too few e- in APD  : ";
  s += fNpTooFew;
  fPavet->AddText(s.Data());
  s  = "Lost b. acceptance angle   : ";
  s += fNpLossAng;
  fPavet->AddText(s.Data());
  s  = "Lost b. quantum efficiency : ";
  s += fNpLossQE;
  fPavet->AddText(s.Data());
  gPad->Clear();
  fPavel->Draw();
  fPavet->Draw();
  gPad->Modified();
  gPad->Update();
  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitResults::GInit() {
//Groups everything common to all constructors
//General
	Axis_t late,MinW,MaxW;
  TLitHistParams *h;
  TAxis *xaxis1,*xaxis2,*xaxis3,*xaxis4,*xaxis5,*xaxis6,*xaxis7,
        *yaxis1,*yaxis2,*yaxis3,*yaxis4;
	late = TLitPhys::Get()->TooLate();
	MinW = TLitPhys::Get()->MinWaveL();
	MaxW = TLitPhys::Get()->MaxWaveL();
	fNpGener = 0;
  fNbHistos  = 0;
//Detection of photons
  fNpSeen  = 0;
  fPhweigth = 0.0;
  h = TLitHistParams::Get("G_Points");
  if (h) fHPoints   = new TH1D("G_Points",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPoints   = new TH1D("G_Points","Number of points on photon trajectory",250,0.0,250.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_TimeSeen");
  if (h) fHTimeSeen = new TH1D("G_TimeSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,late);
	else   fHTimeSeen = new TH1D("G_TimeSeen","time of arrival (ps) of photons on detector",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("G_DistSeen");
  if (h) fHDistSeen = new TH1D("G_DistSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHDistSeen = new TH1D("G_DistSeen","distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_WvlgthSeen");
  if (h) fHWvlgthSeen = new TH1D("G_WvlgthSeen",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthSeen = new TH1D("G_WvlgthSeen","Wavelength (nm) of photons seen",100,MinW,MaxW);
  fNbHistos++;
//Loss of photons
  fNpLossAny = 0;
  fNpAbsMat  = 0;
  fNpAbsBef  = 0;
  fNpAbsRvt  = 0;
  fNpOutSide = 0;
	fNpAbnorm  = 0;
  fNpTooLate = 0;
  fNpTooFew  = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
  fNpAbsL0   = 0;
  h = TLitHistParams::Get("G_AbsAny");
  if (h) fHAbsAny = new TH1D("G_AbsAny",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHAbsAny = new TH1D("G_AbsAny","distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_OutSide");
  if (h) fHOutSide = new TH1D("G_OutSide",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHOutSide = new TH1D("G_OutSide","distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_AbnLost");
  if (h) fHAbnLost = new TH1D("G_AbnLost",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHAbnLost = new TH1D("G_AbnLost","distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_TooLate");
  if (h) fHTooLate = new TH1D("G_TooLate",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHTooLate = new TH1D("G_TooLate","Wavelength of photons seen, but too late",100,MinW,MaxW);
  fNbHistos++;
//Diffusion of photons
  h = TLitHistParams::Get("G_Diffusion");
  if (h) fHDiffusion = new TH1D("G_Diffusion",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHDiffusion = new TH1D("G_Diffusion","Number of diffusions per photon",16,0.0,16.0);
  fNbHistos++;
//Time and Wavelength
  h = TLitHistParams::Get("G_TimeAll");
  if (h) fHTimeAll = new TH1D("G_TimeAll",h->fHTitle,h->fHNbinsx,h->fHXlow,late);
	else   fHTimeAll = new TH1D("G_TimeAll","Generated time (ps) for all components",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("G_WvlgthAll");
  if (h) fHWvlgthAll = new TH1D("G_WvlgthAll",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthAll = new TH1D("G_WvlgthAll","Generated wavelength (nm) for all components",100,MinW,MaxW);
  fNbHistos++;
  h = TLitHistParams::Get("G_TimeCer");
  if (h) fHTimeCer = new TH1D("G_TimeCer",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHTimeCer = new TH1D("G_TimeCer","Generated time (ps) for Cerenkov photons",100,0.0,late);
  fNbHistos++;
  h = TLitHistParams::Get("G_WvlgthCer");
  if (h) fHWvlgthCer = new TH1D("G_WvlgthCer",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
	else   fHWvlgthCer = new TH1D("G_WvlgthCer","Generated wavelength (nm) for Cerenkov photons",100,MinW,MaxW);
  fNbHistos++;
	fNfluo = AtLeastOne();
	fHTimeEach   = 0;
	fHWvlgthEach = 0;
	if (fNfluo) {
		Axis_t afluo = fNfluo;
    h = TLitHistParams::Get("G_TimeEach");
    if (h) fHTimeEach = new TH2F("G_TimeEach",h->fHTitle,h->fHNbinsx,h->fHXlow,late,fNfluo,0.0,afluo);
		else   fHTimeEach = new TH2F("G_TimeEach","Generated time for each component",100,0.0,late,fNfluo,0.0,afluo);
    fNbHistos++;
    h = TLitHistParams::Get("G_WvlgthEach");
    if (h) fHWvlgthEach = new TH2F("G_WvlgthEach",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup,fNfluo,0.0,afluo);
		else   fHWvlgthEach = new TH2F("G_WvlgthEach","Generated wavelength (nm) for each component",100,MinW,MaxW,fNfluo,0.0,afluo);
    fNbHistos++;
  }
//Particle generation
  fNbPart = 0;
  h = TLitHistParams::Get("G_Momentum");
  if (h) fHPMomentum = new TH1D("G_Momentum",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPMomentum = new TH1D("G_Momentum","Momentum [Gev] of generated particles",100,0.0,100.0);
  fNbHistos++;
  h = TLitHistParams::Get("G_Theta");
  if (h) fHPTheta = new TH1D("G_Theta",h->fHTitle,h->fHNbinsx,h->fHXlow,h->fHXup);
  else   fHPTheta = new TH1D("G_Theta","Theta distribution of generated particles",80,0.0,3.2);
  fNbHistos++;
  Int_t kvol;
  TGeoVolume *v;
  TIter next1(gGeoManager->GetListOfVolumes());
  Int_t nshapes = gGeoManager->GetListOfVolumes()->GetEntries();
  h = TLitHistParams::Get("G_EinShape");
  if (h) fHEnergyInShape = new TH2F("G_EinShape",h->fHTitle,nshapes,0.0,(Double_t)nshapes,h->fHNbinsy,0.0,fgEnergyOfGamma);
  else   fHEnergyInShape = new TH2F("G_EinShape","Energy [Mev] deposited by gamma in all shapes",nshapes,0.0,(Double_t)nshapes,50,0.0,fgEnergyOfGamma);
  fNbHistos++;
  xaxis1 = fHEnergyInShape->GetXaxis();
  kvol = 0;
  while ((v = (TGeoVolume*)next1())) {
    kvol++;
    xaxis1->SetBinLabel(kvol,v->GetName());
  }
  fHEnergyInShape->SetStats(0);
  fHEnergyInShape->LabelsOption("v");
//Transitions
  Int_t kmat,krev;
  Int_t nmat = 0;
  Int_t nrev = 0;
  TLitMedium *p;
  TIter next(TLitMedium::fgLitMedium);
  while ((p = (TLitMedium *)next())) {
    if (p->GetKind() == OpticWrapping) nrev++;
    else                               nmat++;
  }
  fHAbsMat = new TH1D("G_AbsMat","Nb. of photons absorbed in various material",nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHAbsBef = new TH1D("G_AbsBef","Nb. of photons absorbed before various wrappings",nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHAbsRvt = new TH1D("G_AbsRvt","Nb. of photons absorbed by various wrappings",nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHMatMat = new TH2F("G_MatToMat","Transition from material to material",
    nmat,0.0,(Double_t)nmat,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHMatNot = new TH2F("G_ReflByMat","Reflection from material by material",
    nmat,0.0,(Double_t)nmat,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  fHMatRev = new TH2F("G_MatToRev","Absorption by wrapping when coming from mat",
    nmat,0.0,(Double_t)nmat,nrev,0.0,(Double_t)nrev);
  fNbHistos++;
  fHRevMat = new TH2F("G_RevToMat","Reflection by wrapping when coming from mat",
    nrev,0.0,(Double_t)nrev,nmat,0.0,(Double_t)nmat);
  fNbHistos++;
  xaxis1 = fHMatMat->GetXaxis();
  yaxis1 = fHMatMat->GetYaxis();
  xaxis2 = fHMatRev->GetXaxis();
  yaxis2 = fHMatRev->GetYaxis();
  xaxis3 = fHRevMat->GetXaxis();
  yaxis3 = fHRevMat->GetYaxis();
  xaxis4 = fHMatNot->GetXaxis();
  yaxis4 = fHMatNot->GetYaxis();
  xaxis5 = fHAbsMat->GetXaxis();
  xaxis6 = fHAbsBef->GetXaxis();
  xaxis7 = fHAbsRvt->GetXaxis();
  next.Reset();
  while ((p = (TLitMedium *)next())) {
    if (p->GetKind() == OpticWrapping) {
      krev = p->GetRevCounter();
      yaxis2->SetBinLabel(krev,p->GetName());
      xaxis3->SetBinLabel(krev,p->GetName());
      xaxis6->SetBinLabel(krev,p->GetName());
      xaxis7->SetBinLabel(krev,p->GetName());
    }
    else {
      kmat = p->GetMatCounter();
      xaxis1->SetBinLabel(kmat,p->GetName());
      yaxis1->SetBinLabel(kmat,p->GetName());
      xaxis2->SetBinLabel(kmat,p->GetName());
      yaxis3->SetBinLabel(kmat,p->GetName());
      xaxis4->SetBinLabel(kmat,p->GetName());
      yaxis4->SetBinLabel(kmat,p->GetName());
      xaxis5->SetBinLabel(kmat,p->GetName());
    }
  }
  fHMatMat->SetStats(0);
  fHMatNot->SetStats(0);
  fHMatRev->SetStats(0);
  fHRevMat->SetStats(0);
  fHAbsMat->LabelsOption("v");
  fHAbsBef->LabelsOption("v");
  fHAbsRvt->LabelsOption("v");
  fHMatMat->LabelsOption("v");
  fHMatNot->LabelsOption("v");
  fHMatRev->LabelsOption("v");
  fHRevMat->LabelsOption("v");
}
void TLitResults::Init() {
  // Initialization of some class variable
  InitP();
  fConclusion = kFALSE;
  fNbHistos   = 0;
  fKN1        = 0;
  fKD1        = 0;
  fKDet       = 0;
  fNdet       = -1;
  fNh         = 0;
}
void TLitResults::InitP() {
//Put all pointers to 0
  fListOfDet      = 0;
  fPavel          = 0;
  fPavet          = 0;
  fHPoints        = 0;
  fHTimeSeen      = 0;
  fHDistSeen      = 0;
  fHWvlgthSeen    = 0;
  fHAbsAny        = 0;
  fHAbsMat        = 0;
  fHAbsBef        = 0;
  fHAbsRvt        = 0;
  fHOutSide       = 0;
  fHAbnLost       = 0;
  fHTooLate       = 0;
  fHDiffusion     = 0;
  fHTimeAll       = 0;
  fHWvlgthAll     = 0;
  fHTimeCer       = 0;
  fHWvlgthCer     = 0;
  fHTimeEach      = 0;
  fHWvlgthEach    = 0;
  fHPMomentum     = 0;
  fHPTheta        = 0;
  fHEnergyInShape = 0;
  fHMatMat        = 0;
  fHMatNot        = 0;
  fHMatRev        = 0;
  fHRevMat        = 0;
}
Bool_t TLitResults::IsEqual(const TObject *obj) const {
  return (fXparam==((TLitResults *)obj)->fXparam);
}
void TLitResults::N() {
  //displays next histogram if the user type "n", exit otherwise
  Int_t k,M;
  char c='n';
  if (!gTwoPad) TLit::Get()->BookCanvas();
  while (c == 'n' ) {
    k=0;
    M=0;
    while ((!M) && (k<fNbHistos)) {
      M = N1();
      k++;
    }
    if (k>=fNbHistos) {
      gCleanOut->MM(warning,"N","No histo to plot",ClassName());
      c = 'q';
    }
    else {
      cout << "Press n to see next histo, any other key to quit" << endl;
      cin >> c;
    }
  }
}
Int_t TLitResults::N1() {
//For printing each histogram, one after the other
  Int_t N = 0;
  Double_t aN,aB,a;
  gTwoPad->ChangePad();
  switch (fKN1) {
  case 0:
    N = (Int_t)fHPoints->GetEntries();
    fHPoints->Draw();
    break;
  case 1:
    N = (Int_t)fHTimeSeen->GetEntries();
    gStyle->SetOptStat(111111);
    fHTimeSeen->Draw();
    break;
  case 2:
    N = (Int_t)fHDistSeen->GetEntries();
    fHDistSeen->Draw();
    break;
  case 3:
    N = (Int_t)fHWvlgthSeen->GetEntries();
    fHWvlgthSeen->Draw();
    break;
  case 4:
    N = (Int_t)fHAbsAny->GetEntries();
    fHAbsAny->Draw();
    break;
  case 5:
    N = (Int_t)fHAbsMat->GetEntries();
    fHAbsMat->Draw();
    break;
  case 6:
    N = (Int_t)fHAbsBef->GetEntries();
    fHAbsBef->Draw();
    break;
  case 7:
    N = (Int_t)fHAbsRvt->GetEntries();
    fHAbsRvt->Draw();
    break;
  case 8:
    N = (Int_t)fHOutSide->GetEntries();
    fHOutSide->Draw();
    break;
  case 9:
    N = (Int_t)fHAbnLost->GetEntries();
    fHAbnLost->Draw();
    break;
  case 10:
    N = (Int_t)fHTooLate->GetEntries();
    fHTooLate->Draw();
    break;
	case 11:
		N = (Int_t)fHDiffusion->GetEntries();
    fHDiffusion->Draw();
		break;
  case 12:
    N = (Int_t)fHTimeAll->GetEntries();
    fHTimeAll->Draw();
    break;
  case 13:
    N = (Int_t)fHWvlgthAll->GetEntries();
    fHWvlgthAll->Draw();
    break;
  case 14:
    N = (Int_t)fHTimeCer->GetEntries();
    fHTimeCer->Draw();
    break;
  case 15:
    N = (Int_t)fHWvlgthCer->GetEntries();
    fHWvlgthCer->Draw();
    break;
  case 16:
    if (fHTimeEach) {
      N = (Int_t)fHTimeEach->GetEntries();
      fHTimeEach->Draw("LEGO2Z");
    }
    break;
  case 17:
    if (fHWvlgthEach) {
      N = (Int_t)fHWvlgthEach->GetEntries();
      fHWvlgthEach->Draw("LEGO2Z");
    }
    break;
  case 18:
    N = (Int_t)fHPMomentum->GetEntries();
    fHPMomentum->Draw();
    break;
  case 19:
    N = (Int_t)fHPTheta->GetEntries();
    fHPTheta->Draw();
    break;
  case 20:
    N = (Int_t)fHEnergyInShape->GetEntries();
    fHEnergyInShape->Draw("LEGO2Z");
//    fHEnergyInShape->Draw("TEXT");
    break;
  case 21:
    aN = fHMatMat->GetEntries();
    N  = (Int_t)aN;
    aB = fHMatMat->GetNbinsX()*fHMatMat->GetNbinsY();
    a  = aN/aB;
    if (a>100000.0) fHMatMat->Draw("scat");
    else            fHMatMat->Draw("text");
    break;
  case 22:
    aN = fHMatNot->GetEntries();
    N  = (Int_t)aN;
    aB = fHMatNot->GetNbinsX()*fHMatNot->GetNbinsY();
    a  = aN/aB;
    if (a>100000.0) fHMatNot->Draw("scat");
    else            fHMatNot->Draw("text");
    break;
  case 23:
    aN = fHMatRev->GetEntries();
    N  = (Int_t)aN;
    aB = fHMatRev->GetNbinsX()*fHMatRev->GetNbinsY();
    a  = aN/aB;
    if (a>100000.0) fHMatRev->Draw("scat");
    else            fHMatRev->Draw("text");
    break;
  case 24:
    aN = fHRevMat->GetEntries();
    N  = (Int_t)aN;
    aB = fHRevMat->GetNbinsX()*fHRevMat->GetNbinsY();
    a  = aN/aB;
    if (a>100000.0) fHRevMat->Draw("scat");
    else            fHRevMat->Draw("text");
    break;
  default:
    fKN1 = 0;
    N    = 0;
    break;
  }//end switch (fKN1)
  fKN1++;
  fKN1 %= fNbHistos;
  return N;
}
void TLitResults::ResetCounters() {
//counters to 0
  fKN1  = 0;
  fKD1  = 0;
  fKDet = 0;
}
