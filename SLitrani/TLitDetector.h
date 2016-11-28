// @(#)root/html:$Id: TLitDetector.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitDetector
#define LIT_TLitDetector

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TLitDetector  Contains all statistics and histograms for a detector     //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#include "TH2.h"
#include "TObjArray.h"
#include "TLitPhys.h"

enum LitMemForDet {nohist,punyhist,smallhist,normalhist,bighist};

class TLitDetector : public TObject {

protected:

  TString     fName;        //Name of detector == path in geometry
  Short_t     fGlob;        //Bit infos. See class description
  Short_t     fNbHistos;    //Number of histograms or graphs to be plotted
  Short_t     fNbSumHist;   //Number of summary (abscissa depending upon run) histograms to be plotted


  void        AddMe();
  void        Init();
  void        InitP();
  void        BookHistos(Double_t,Double_t,Double_t);

public:

  static LitMemForDet  fgMemForDet;    //To control memory used by histograms
  static Double_t      fgMaxCatAn;     //Maximum possible distance from cathode to anode for an APD in cm
  static TObjArray    *fgLitDetGlob;   //List of all TLitDetector with fGlob = true
  static TObjArray    *fgLitDetRun;    //List of all TLitDetector with fGlob = false
  static TLitDetector *fgLastUsedDet;  //Last used run specific detector
  static TLitDetector *fgLastUsedDetG; //Last used global detector
  static Int_t         fgMaxHist;      //Maximum number of possible histos

  Int_t       fNpSeen;      //Nb. of photons seen by this volume detector
  Int_t       fNpLossAny;   //Nb. of photons lost by this detector for ANY reason
  Int_t       fNpLossAng;   //Lost by this detector because of acceptance angle
  Int_t       fNpLossQE;    //Lost by this detector because of quantum efficiency
  TH1        *fHTimeSeen;   //(1) - time of arrival of photons accepted by this detector
  TH1        *fHWvlgthSeen; //(2) - wave length of all photons seen by this detector
  TH1        *fHWvlgthNot;  //(3) - wave length of all photons lost by this detector
  TH1        *fHQEff;       //(4) - Quantum efficiency of detector (+1 if inside and ok in test of QE). At the end, divided by FHInside
	TH1        *fHInside;     //(5) - Nb. of photons inside detector (detected or not) versus wavelength
  TH1        *fHDepth;      //(6) - Depth of end point of photon inside volume detector
  TH2        *fHXYEntrance; //(7) - Projection on entrance face of end point of photon
//SUMMARY HISTOGRAMS
  TH1        *fHEfficiency; //(1) - Efficiency as a function of run. Only if fGlob.

  TLitDetector()                   { InitP();             }
  TLitDetector(const char*,KindOfDet,Bool_t,Bool_t,Bool_t,Bool_t=kFALSE,Double_t=0.0,Double_t=0.0,Double_t=0.0);
  virtual      ~TLitDetector();
  void          ClearHistos();
  Int_t         Compare(const TObject*) const;
	void          Conclusion();
  inline Int_t  FillDepth(Double_t) const;
  inline Int_t  FillInside(Double_t) const;
  inline Int_t  FillQEff(Double_t) const;
  inline Int_t  FillTimeSeen(Double_t) const;
  inline Int_t  FillWvlgthNot(Double_t) const;
  inline Int_t  FillWvlgthSeen(Double_t) const;
  inline Int_t  FillXYEntrance(Double_t,Double_t) const;
  static TLitDetector *FindDetector(TString,Bool_t);
  Short_t       GetGlob() const      { return fGlob;        }
  const char   *GetName() const      { return fName.Data(); }
  Short_t       GetNbHistos() const  { return fNbHistos;    }
  Short_t       GetNbSumHist() const { return fNbSumHist;   }
  Bool_t        IsAPD() const        { return (fGlob & 8);  }
  Bool_t        IsEntrance() const   { return (fGlob & 512);}
  Bool_t        IsEqual(const TObject*) const;
  Bool_t        IsGendet() const     { return (fGlob & 2);  }
  Bool_t        IsGlob() const       { return (fGlob & 1);  }
  Bool_t        IsMe(const TString&) const;
  Bool_t        IsPhototube() const  { return (fGlob & 4);  }
  Bool_t        IsSortable() const   { return kTRUE;        }
  Bool_t        IsStopAlways() const { return (fGlob & 256);}
  Bool_t        IsVolEnt() const     { return (!(fGlob & 512) && (fGlob & 1024)); }
  void          Reset();
  ClassDef(TLitDetector,1)//Contains all statistics and histograms for a detector
};
inline Int_t    TLitDetector::FillDepth(Double_t x) const {
  Int_t bin = -1;
  if (fHDepth) bin = fHDepth->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillInside(Double_t x) const {
  Int_t bin = -1;
  if (fHInside) bin = fHInside->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillQEff(Double_t x) const {
  Int_t bin = -1;
  if (fHQEff) bin = fHQEff->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillTimeSeen(Double_t x) const {
  Int_t bin = -1;
  if (fHTimeSeen) bin = fHTimeSeen->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillWvlgthNot(Double_t x) const {
  Int_t bin = -1;
  if (fHWvlgthNot) bin = fHWvlgthNot->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillWvlgthSeen(Double_t x) const {
  Int_t bin = -1;
  if (fHWvlgthSeen) bin = fHWvlgthSeen->Fill(x);
  return bin;
}
inline Int_t    TLitDetector::FillXYEntrance(Double_t x,Double_t y) const {
  Int_t bin = -1;
  if (fHXYEntrance) bin = fHXYEntrance->Fill(x,y);
  return bin;
}
#endif
