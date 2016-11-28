// @(#)root/html:$Id: TLitMarrow.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TCleanOut.h"
#include "TwoPadDisplay.h"
#include "TLitResults.h"
#include "TLitDetector.h"
#include "TLitMarrow.h"

TLitMarrow* gLitGp = 0;

ClassImp(TLitMarrow)
//______________________________________________________________________________
//
//  TLitMarrow helps in making the final analysis, after all runs have
// been produced. TLitMarrow  Creates the final histograms presenting values
// (efficiency, losses,etc) as a function of the parameter varying from run
// to run.
//  For a general presentation of statistics and histograms in SLitrani, look
// first at the class description of TLitResults.
//
//  When you are finished with the generation of all runs, comes the time to
// analyze the results produced by SLitrani. In what concerns the results PER
// RUN, look at the class TLitResults, where everything is explained on how to
// look at the histograms of each run or at the histograms which are the
// summation of all runs.
//  But you may also be interested in seeing how some physical quantity va-
// ries as a function of run, or rather as a function of the parameter you
// have changed from run to run. For instance, may be you have varied the z
// position of the beam from run to run, z being along the long axis of some
// crystal, and you want to know how the efficiency for detecting photons
// with an APD varies with the position of the beam. TLitMarrow handles this
// type of analysis. Even in the case where the type of analysis done by
// TLitMarrow does not suit your needs, it is interesting for you to look at
// the code of TLitMarrow, in order to learn how to extract physical informations
// from SLitrani.
//  The analysis done in TLitMarrow provide you with the following :
//    (a) - an histogram giving the variation of the efficiency as a function
// of the parameter you have varied from run to run. Look at the method
// DrawEfficiency();
//    (b) - many histograms giving the variation of all kinds of losses as a
// function of the parameter you have varied from run to run. Look at all the
// methods beginning with Draw...
//    (c) - the possibility to look at the summary histograms of each detec-
// tor, for instance the histogram giving the variation of the efficiency
// OF SOME SPECIFIC DETECTOR as a function of the parameter you have varied
// from run to run. These summary histograms per detector are contained in
// the class TLitDetector.
//    (d) - the method N() allows you to look at all summary histograms in
// turn, even the summary histograms of each detector.
//
//  The class TLitMarrow is pointed to by the global pointer gLitGp. The user
// has not to define this pointer, it has already been done in the method
// Gen() of class TLitSpontan, TLitBeam, TLitCascade or TLitGamma . But there
// is a good reason for the user to change the title (second argument of the
// constructor of the class TLitMarrow) that was given when creating gLitGp.
// We will explain why :
//  When you have called the method Gen(), you had the possibility to give a
// free parameter xparam, intended to be used as the x axis parameter in the
// summary histograms. For instance, may be you have varied the z position of
// the beam from run to run, z being along the long axis of some crystal, and
// you want to know how the efficiency for detecting photons with an APD varies
// with the position of the beam. In that case, you have given as xparam when
// calling Gen() the value of the z position of the beam. It would be nice if
// the title of each summary histogram and the label of the x axis of these
// histograms would contain "z coordinate of beam". This is possible by
// giving "z coordinate of beam" as argument "title" for gLitGp. You can do
// that by :
//    gLitGp->SetTitle("z coordinate of beam");
//
//  For instance, the histogram fHEfficiency has for original title
// "Efficiency versus ". To this original title will be appended the title of
// gLitGp, so that the final title will be :
//      "Efficiency versus z coordinate of beam".
//  The same process apply for all summary histograms.
//
//  USING gLitGp
//
//  At the end of the production of all runs, do the following inside the
// a CINT macro or directly on the keyboard :
//
//    gLitGp->SetTitle("z coordinate of beam");
//    gLitGp->Summary();
//    TLit::Get()->CloseFiles();
//
//  Summary() will book and fill all summary histograms. You are now ready
// to look at the histograms.
//
//  (A) - If all you want to do is looking in turn at each histogram,
// including the detector summary histograms, simply do :
//
//    gLitGp->N();  You will then see the 1st histogram. To see the next one,
//                  simply type n<CR>. To stop the show, type any key, for
//                  instance q<CR>.
//  (B) - If you want to see a particular summary histogram, use one of the
// Draw method, for instance :
//
//    gLitGp->DrawAbsMat();
//
//  (C) - If you want to see all losses superimposed onto one histogram, do :
//
//    gLitGp->DrawAllLosses();
//         
//        in fact, you will get 2 plots, one in linear y scale, the other
//        in logarithmic y scale.
//
//  (D) - If you want to see a particular summary detector histogram, you
// must know the kind and the name of the detector. The detectors may be of 3
// kinds (class TLitDetector):
//    (a) - general volume detectors
//    (b) - Phototubes
//    (c) - APD
//  For more informations on detectors, look at the class TLitVolume and
// TLitDetector.
//
TLitMarrow::TLitMarrow(const char *name,const char *title, Bool_t inmem):TNamed(name,title) {
  //
  //  The constructor of TLitMarrow is called the first time a Gen() method
  // is called [ TLitSpontan::Gen() or TLitBeam::Gen() or TLitCascade::Gen() or
  // TLitGamma::Gen()]. The user of SLitrani has not to bother about calling the
  // constructor of this class.
  //
  //  The parameters are the following :
  //
  //  name   : arbitrary name.
  //  title  : if you have provided xparam when calling Gen() [ See
  //           TLitSpontan::Gen() or TLitBeam::Gen() ], i.e. if the final results
  //           have to be presented as a function of the parameter xparam
  //           used as abscissa, the title is the description of this para-
  //           meter. For instance, title could be :
  //               "Incident angle of fibre" or
  //               "Beam position along crystal axis", and so on.
  //           Otherwise, define title for instance as :
  //               "Run number"
  //  inmem  : the statistics of all run is in memory. TLitMarrow will not
  //           work otherwise.
  //
  InitP();
  fInMem    = inmem;
  fK        = 0;
  fNDet     = 0;
  fNHperDet = 1;
}
TLitMarrow::~TLitMarrow() {
  ClearHistos();
}
void TLitMarrow::BookHistos() {
  // Booking of all histograms
  const Axis_t z05 = 0.5;
  Int_t i,j,n,krun;
  Axis_t d2;
  Axis_t *xmid, *xlow;
  n = TLitResults::fgResults->GetEntries();
  fNbins = n - 1;
  xmid = new Axis_t[fNbins];
  xlow = new Axis_t[n];
  TLitResults *p;
  TIter next(TLitResults::fgResults);
  i = 0;
  while ((p = (TLitResults *)next())) {
    krun  = p->GetRun();
    if (krun != 0) {
      xmid[i] = p->GetXparam();
      i++;
    }
  }
  if (fNbins == 1) {
    xlow[0] = xmid[0] - z05;
    xlow[1] = xmid[0] + z05;
  }
  else {
    for (i=1;i<fNbins;i++) {
      j  = i - 1;
      d2 = z05*(xmid[i] - xmid[j]);
      xlow[i] = xmid[j] + d2;
      if (i==1) xlow[j] = xmid[j] - d2;
      if (i==fNbins-1) xlow[fNbins] = xmid[fNbins-1] + d2;
    }
  }
  delete [] xmid;
  TString ssd;
  TString fullname,fulltitle,title;
  title = GetTitle();
  fulltitle = "Efficiency versus ";
  fulltitle.Append(title);
  fHEfficiency = new TH1F("Efficiency_pr",fulltitle.Data(),fNbins,xlow);
  fHEfficiency->GetXaxis()->SetTitleSize(0.035);
  fHEfficiency->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost inside materials versus ";
  fulltitle.Append(title);
  fHAbsMat = new TH1F("AbsMat_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsMat->GetXaxis()->SetTitleSize(0.035);
  fHAbsMat->SetXTitle((char *)title.Data());
  //
  fulltitle = "Absorbed before wrapping versus ";
  fulltitle.Append(title);
  fHAbsBef = new TH1F("AbsBef_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsBef->GetXaxis()->SetTitleSize(0.035);
  fHAbsBef->SetXTitle((char *)title.Data());
  //
  fulltitle = "Absorbed by wrapping versus ";
  fulltitle.Append(title);
  fHAbsRvt = new TH1F("AbsRvt_pr",fulltitle.Data(),fNbins,xlow);
  fHAbsRvt->GetXaxis()->SetTitleSize(0.035);
  fHAbsRvt->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because leaving the setup versus ";
  fulltitle.Append(title);
  fHOutSide = new TH1F("OutSide_pr",fulltitle.Data(),fNbins,xlow);
  fHOutSide->GetXaxis()->SetTitleSize(0.035);
  fHOutSide->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost for abnormal reasons versus ";
  fulltitle.Append(title);
  fHAbnorm = new TH1F("Abnorm_pr",fulltitle.Data(),fNbins,xlow);
  fHAbnorm->GetXaxis()->SetTitleSize(0.035);
  fHAbnorm->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because too late versus ";
  fulltitle.Append(title);
  fHTooLate = new TH1F("TooLate_pr",fulltitle.Data(),fNbins,xlow);
  fHTooLate->GetXaxis()->SetTitleSize(0.035);
  fHTooLate->SetXTitle((char *)title.Data());
  //
  fulltitle = "Too few electrons in APD versus ";
  fulltitle.Append(title);
  fHTooFew = new TH1F("TooFew_pr",fulltitle.Data(),fNbins,xlow);
  fHTooFew->GetXaxis()->SetTitleSize(0.035);
  fHTooFew->SetXTitle((char *)title.Data());
  //
  fulltitle = "Lost because of acceptance angle versus ";
  fulltitle.Append(title);
  fHLossAng = new TH1F("LossAng_pr",fulltitle.Data(),fNbins,xlow);
  fHLossAng->GetXaxis()->SetTitleSize(0.035);
  fHLossAng->SetXTitle((char *)title.Data());
  //
  fulltitle = "Quantum Eff. losses (except APD) versus ";
  fulltitle.Append(title);
  fHLossQE = new TH1F("LossQE_pr",fulltitle.Data(),fNbins,xlow);
  fHLossQE->GetXaxis()->SetTitleSize(0.035);
  fHLossQE->SetXTitle((char *)title.Data());
  // Detectors
  TLitResults *pglob;
  pglob = GlobalP();
  if (pglob) {
    //Summary Histograms of all volume detectors
    fNDet = pglob->GetListOfDet()->GetEntries();
    if (fNDet) {
      i=1;
      TLitDetector *pdet;
      TIter next(pglob->GetListOfDet());
      while ((pdet = (TLitDetector *)next())) {
        TString detkind;
        TString s1 = "";
        s1 += i;
        if (pdet->IsGendet())    detkind = " gendet";
        if (pdet->IsPhototube()) detkind = " phototube";
        if (pdet->IsAPD())       detkind = " APD";
        fullname = "Efficiency_v";
        fullname.Append(s1);
        fulltitle = "Efficiency";
        fulltitle.Append(detkind);
        fulltitle.Append(" ");
        fulltitle.Append(pdet->GetName());
        fulltitle.Append(" versus ");
        fulltitle.Append(title);
        if (pdet->fHEfficiency) {
          delete pdet->fHEfficiency;
          pdet->fHEfficiency = 0;
        }
        pdet->fHEfficiency = new TH1F(fullname.Data(),fulltitle.Data(),fNbins,xlow);
        pdet->fHEfficiency->GetXaxis()->SetTitleSize(0.035);
        pdet->fHEfficiency->SetXTitle((char *)title.Data());
        i++;
      }//end while ((pdet = (TLitDetector *)next()))
    }//end if (fNDet)
  }//end if (pglob)
  delete [] xlow;
}
void TLitMarrow::ClearHistos() {
  // Delete all histograms
  if (fHEfficiency) {
    delete fHEfficiency;
    fHEfficiency = 0;
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
  if (fHAbnorm) {
    delete fHAbnorm;
    fHAbnorm = 0;
  }
  if (fHTooLate) {
    delete fHTooLate;
    fHTooLate = 0;
  }
  if (fHTooFew) {
    delete fHTooFew;
    fHTooFew = 0;
  }
  if (fHLossAng) {
    delete fHLossAng;
    fHLossAng = 0;
  }
  if (fHLossQE) {
    delete fHLossQE;
    fHLossQE = 0;
  }
}
void TLitMarrow::D(Int_t kdet,Int_t kh) const {
  // Draw histogram kh of detector kdet
  TLitResults *pglob;
  if (gLitGs) pglob = gLitGs;
  else        pglob = GlobalP();
  TObjArray *list = TLitDetector::fgLitDetGlob;
  kdet %= fNDet;
  if (kdet<fNDet) {
    TLitDetector *gvol;
    gvol = (TLitDetector*)(*list)[kdet];
    switch (kh) {
      default:
        gvol->fHEfficiency->Draw("E1");
        break;
    }
  }//end if (det1<pglob->fNVolDet)
}
void TLitMarrow::DrawAbnorm() const {
  // Draws histogram fHAbnorm
  fHAbnorm->Draw("E1");
}
void TLitMarrow::DrawAbsBef() const {
  // Draws histogram fHAbsBef
  fHAbsBef->Draw("E1");
}
void TLitMarrow::DrawAbsMat() const {
  // Draws histogram fHAbsMat
  fHAbsMat->Draw("E1");
}
void TLitMarrow::DrawAbsRvt() const {
  // Draws histogram fHAbsRvt
  fHAbsRvt->Draw("E1");
}
void TLitMarrow::DrawAllLosses() const {
  // Draw all losses, superimposed.
  char c='n';
  Int_t n;
  gTwoPad->SelectPad1();
  fHAbsMat->SetMaximum(1.0);
  fHAbsMat->Draw();
  fHAbsBef->Draw("SAME");
  fHAbsRvt->Draw("SAME");
  fHOutSide->Draw("SAME");
  fHAbnorm->Draw("SAME");
  fHTooLate->Draw("SAME");
  fHTooFew->Draw("SAME");
  fHLossAng->Draw("SAME");
  fHLossQE->Draw("SAME");
  gTwoPad->ChangePad();
  gTwoPad->GetPad2()->SetLogy(1);
  fHAbsMat->SetMinimum(0.001);
  fHAbsMat->Draw();
  n = Int_t(fHAbsBef->GetEntries());
  if (n) fHAbsBef->Draw("SAME");
  n = Int_t(fHAbsRvt->GetEntries());
  if (n) fHAbsRvt->Draw("SAME");
  n = Int_t(fHOutSide->GetEntries());
  if (n) fHOutSide->Draw("SAME");
  n = Int_t(fHAbnorm->GetEntries());
  if (n) fHAbnorm->Draw("SAME");
  n = Int_t(fHTooLate->GetEntries());
  if (n) fHTooLate->Draw("SAME");
  n = Int_t(fHTooFew->GetEntries());
  if (n) fHTooFew->Draw("SAME");
  n = Int_t(fHLossAng->GetEntries());
  if (n) fHLossAng->Draw("SAME");
  n = Int_t(fHLossQE->GetEntries());
  if (n) fHLossQE->Draw("SAME");
  gTwoPad->ChangePad();
  cout << "Press  any key to continue" << endl;
  cin >> c;
  gTwoPad->GetPad2()->SetLogy(0);
  fHAbsMat->SetMinimum(0.0);
}
void TLitMarrow::DrawEfficiency() const {
  // Draws histogram fHEfficiency
  fHEfficiency->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::DrawLossAng() const {
  // Draws histogram fHLossAng
  fHLossAng->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::DrawLossQE() const {
  // Draws histogram fHLossQE
  fHLossQE->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::DrawOutSide() const {
  // Draws histogram fHOutSide
  fHOutSide->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::DrawTooFew() const {
  // Draws histogram fHTooFew
  fHTooFew->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::DrawTooLate() const {
  // Draws histogram fHTooLate
  fHTooLate->Draw("E1");
  //  if (gTwoPad) gTwoPad->ChangePad();
}
void TLitMarrow::FillHistos() {
  // Filling of all histograms
  const Double_t eps = 1.0e-12;
  const Int_t NbOfHistos = 10;
  Int_t i,j,krun,NdetG,iden;
  Double_t num,den;
  Int_t los,bil;
  Stat_t y,dy;
  Bool_t b[NbOfHistos];
  for (i=0;i<NbOfHistos;i++) b[i] = kFALSE;
  TLitResults *p,*pglob;
  TString s;
  s = GetTitle();
  pglob = GlobalP();
  NdetG = TLitDetector::fgLitDetGlob->GetEntries();
  TIter next(TLitResults::fgResults);
  i=0;
  while ((p = (TLitResults *)next())) {
    krun  = p->GetRun();
    if (krun != 0) {
      i++;
      if (p->fNpSeen) b[0] = kTRUE;
      num  = p->fNpSeen;
      iden = p->fNpGener - p->fNpAbnorm - p->fNpAbsL0;
      if (iden<=0) den = eps;
      else         den = iden;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHEfficiency->SetBinContent(i,y);
      fHEfficiency->SetBinError(i,dy);
      los  = p->fNpAbsMat;
      if (p->fNpAbsMat) b[1] = kTRUE;
      num  = p->fNpAbsMat;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHAbsMat->SetBinContent(i,y);
      fHAbsMat->SetBinError(i,dy);
      los += p->fNpAbsBef;
      if (p->fNpAbsBef) b[2] = kTRUE;
      num  = p->fNpAbsBef;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHAbsBef->SetBinContent(i,y);
      fHAbsBef->SetBinError(i,dy);
      los += p->fNpAbsRvt;
      if (p->fNpAbsRvt) b[3] = kTRUE;
      num  = p->fNpAbsRvt;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHAbsRvt->SetBinContent(i,y);
      fHAbsRvt->SetBinError(i,dy);
      los += p->fNpOutSide;
      if (p->fNpOutSide) b[4] = kTRUE;
      num  = p->fNpOutSide;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHOutSide->SetBinContent(i,y);
      fHOutSide->SetBinError(i,dy);
      los += p->fNpAbnorm;
      if (p->fNpAbnorm) b[5] = kTRUE;
      num  = p->fNpAbnorm;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHAbnorm->SetBinContent(i,y);
      fHAbnorm->SetBinError(i,dy);
      los += p->fNpTooLate;
      if (p->fNpTooLate) b[6] = kTRUE;
      num  = p->fNpTooLate;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHTooLate->SetBinContent(i,y);
      fHTooLate->SetBinError(i,dy);
      los += p->fNpTooFew;
      if (p->fNpTooFew) b[7] = kTRUE;
      num  = p->fNpTooFew;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHTooFew->SetBinContent(i,y);
      fHTooFew->SetBinError(i,dy);
      los += p->fNpLossAng;
      if (p->fNpLossAng) b[8] = kTRUE;
      num  = p->fNpLossAng;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHLossAng->SetBinContent(i,y);
      fHLossAng->SetBinError(i,dy);
      los += p->fNpLossQE;
      if (p->fNpLossQE) b[9] = kTRUE;
      num  = p->fNpLossQE;
      y    = num/den;
      dy   = TMath::Sqrt(num*(1.0-y))/den;
      fHLossQE->SetBinContent(i,y);
      fHLossQE->SetBinError(i,dy);
      //Summary Histograms of all volume detectors
      TLitDetector *pvol,*gvol;
      TString detname,detn;
      for (j=0;j<NdetG;j++) {
        Int_t k,L;
        gvol = (TLitDetector *)(*TLitDetector::fgLitDetGlob)[j];
        detname = gvol->GetName();
        detn = "";
        L = detname.Length();
        for (k=1;k<L;k++) detn.Append(detname[k]);
        TLitDetector::fgLitDetRun = p->GetListOfDet();
        num = 0.0;
        if (TLitDetector::fgLitDetRun->GetEntries()) {
          pvol = TLitDetector::FindDetector(detn,kFALSE);
          if (pvol) num = pvol->fNpSeen;
        }
        y     = num/den;
        dy    = TMath::Sqrt(num*(1.0-y))/den;
        gvol->fHEfficiency->SetBinContent(i,y);
        gvol->fHEfficiency->SetBinError(i,dy);
      }//end for (j=0;j<NdetG;j++)
      //Tests
      bil  = p->fNpGener - p->fNpAbsL0 - p->fNpSeen - los;
      if (bil) gCleanOut->MM(error,"FillHistos","Balance of photons bad",ClassName());
    }//end if (krun != 0)
  }//end while ((p = (TLitResults *)next()))
  //  fHEfficiency->Fit("pol2","Q");
  //  fHAbsMat->Fit("pol2","Q");
  if (b[1]) fHAbsMat->SetEntries(1);
  //  fHAbsBef->Fit("pol2","Q");
  if (b[2]) fHAbsBef->SetEntries(1);
  //  fHAbsRvt->Fit("pol2","Q");
  if (b[3]) fHAbsRvt->SetEntries(1);
  //  fHOutSide->Fit("pol2","Q");
  if (b[4]) fHOutSide->SetEntries(1);
  //  fHAbnorm->Fit("pol2","Q");
  if (b[5]) fHAbnorm->SetEntries(1);
  //  fHTooLate->Fit("pol2","Q");
  if (b[6]) fHTooLate->SetEntries(1);
  //  fHTooFew->Fit("pol2","Q");
  if (b[7]) fHTooFew->SetEntries(1);
  //  fHLossAng->Fit("pol2","Q");
  if (b[8]) fHLossAng->SetEntries(1);
  //  fHLossQE->Fit("pol2","Q");
  if (b[9]) fHLossQE->SetEntries(1);
}
TLitResults* TLitMarrow::GlobalP() const {
  // Finds the pointer to the global statistics
  TLitResults *p;
  TLitResults *pglob = 0;
  TIter next(TLitResults::fgResults);
  Bool_t found = kFALSE;
  while ((!found) && (p = (TLitResults *)next())) {
    if (!p->GetRun()) {
      found = kTRUE;
      pglob = p;
    }
  }
  if (!found) {
    if (gLitGs) pglob = gLitGs;
    else {
      pglob = 0;
      gCleanOut->MM(error,"GlobalP","Global statistics not found",ClassName());
    }
  }
  return pglob;
}
void TLitMarrow::InitP() {
  // All pointers to 0
  fHEfficiency = 0;
  fHAbsMat     = 0;
  fHAbsBef     = 0;
  fHAbsRvt     = 0;
  fHOutSide    = 0;
  fHAbnorm     = 0;
  fHTooLate    = 0;
  fHTooFew     = 0;
  fHLossAng    = 0;
  fHLossQE     = 0;
}
void TLitMarrow::N() {
  //  Draw each histogram in turn when the user presses 'n'. Exit for any other
  // key hit.
  char c='n';
  gTwoPad->SelectPad1();
  while (c == 'n' ) {
    N1();
    cout << "Press n to see next histo, any other key to quit" << endl;
    cin >> c;
  }
}
void TLitMarrow::N1() {
  // Draws histogram fK
  const Int_t nbasehist = 10;
  Int_t ddt,kdet,kh;
  gTwoPad->ChangePad();
  if (fK>=nbasehist) {
    ddt  = fK - nbasehist;
    kdet  = ddt/fNHperDet;
    kh   = ddt % fNHperDet;
    D(kdet,kh);
  }
  else {
    switch (fK) {
    case 0:
      DrawEfficiency();
      break;
    case 1:
      DrawAbsMat();
      break;
    case 2:
      DrawAbsBef();
      break;
    case 3:
      DrawAbsRvt();
      break;
    case 4:
      DrawOutSide();
      break;
    case 5:
      DrawAbnorm();
      break;
    case 6:
      DrawTooLate();
      break;
    case 7:
      DrawTooFew();
      break;
    case 8:
      DrawLossAng();
      break;
    case 9:
      DrawLossQE();
      break;
    default:
      fK = 0;
      DrawEfficiency();
      break;
    }
  }
  fK += 1;
  fK %= nbasehist + fNDet*fNHperDet;
}
void TLitMarrow::Summary() {
  // Calculates the histograms summary of all runs
  gLitGs->Conclusion();
  if (fInMem) {
    if (TLitResults::fgResults->GetEntries()>=2) {
      TLitResults::fgResults->Sort();
      BookHistos();
      FillHistos();
    }
    else gCleanOut->MM(error,"Summary","No statistics available",ClassName());
  }
  else {
    gCleanOut->MM(error,"Summary","Statistics not in memory",ClassName());
    gCleanOut->MM(info,"Summary","Put runstat=kTRUE and forgetlast=kFALSE in Gen",ClassName());
  }
}
