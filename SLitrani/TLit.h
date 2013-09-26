// @(#)root/html:$Id: TLit.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLit
#define LIT_TLit

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLit    Steering class of SLitrani                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TTree.h"
#include "TObjArray.h"
#include "TVector3.h"
#include "TReference.h"

enum LitStat {generated,emitted,seen,lost,inmedNSD,inmedSD,supplND,supplD,wrapND,
     wrapD,leaving,late,lacke,accept,QE,notemitted,nonoptical,abnormal};

class TLit {

private:

  Bool_t          fTestCalc;      //if true, many checks to calculations

  static Short_t  fgSize;         //Size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
  static Int_t    fgNbButtons;    //Number of buttons wanted in TwoPadDisplay
  static Int_t    fgTopXGen;      //Top x  of general canvas fCanvas
  static Int_t    fgTopYGen;      //Top y  of general canvas fCanvas
  static Bool_t   fgWithDate;     //if true, date and time are appended to the name of the ROOT file and of the listing
  static TString *fgDate;         //Date when program runs
  static TString *fgTime;         //Time when program runs
  static TString *fgDateTime;     //Date and Time when program runs
  static TString *fgRootFileName; //Name of .root file created by SLitrani
  static TString *fgListFileName; //Name of .lis  file created by SLitrani
  static Short_t  fgLineSize;     //Length of line in output and listing. Default 119 char
  static TLit    *fgLit;

  TLit();                         //Private constructor: TLit is a singleton!
  virtual ~TLit();                //Private destructor:  TLit is a singleton!
  void            Init();
  void            InitP();
  void            InitRef();
  static void     SetDate();
  void            StatDef();

public:

  TReference     *fRef;           //Reference of program
  Bool_t          fBTRchanged;    //True if default comments have been changed
  TString         fTRComment;     //Comment appearing at top right of main canvas
  TString         fBRComment;     //Comment appearing at bottom right of main canvas
  Bool_t          fSaveFitSpec;   //If true, TSplinefit fits used and TLitSpectrum spectra used are saved on root file. Default
  TFile          *fLitFile;       //.root file created by SLitrani
  Int_t           fLitBufSize;    //buffer size of file
  Int_t           fLitNb;         //pointer on actual size of file
  TTree          *fLitTree;       //tree of histos and statistics for each run.
  TBranch        *fLitBranch;     //branch of tree
  Int_t           fNFits;         //Number of fits in fFitColl
  Int_t           fKFit;          //Counter of fits drawn
  TObjArray      *fFitColl;       //Collection of fits
  Int_t           fNSpecs;        //Number of spectra in fSpecColl
  Int_t           fKSpec;         //Counter of spectra drawn
  TObjArray      *fSpecColl;      //Collection of spectra
  Bool_t          fFilesClosed;   //True if summary files have been closed
  Int_t           fStatItems;     //Must be set equal to 17, dimension of fS...Def[]
  TString         fSShortDef[18]; //Summary statistics of photons. Short definitions
  TString         fSLongDef[18];  //Summary statistics of photons. Full definitions
  Bool_t          fSeen;          //true if photon seen
  Double_t        fX0;            //x coordinate of death of seen photons in WCS
  Double_t        fY0;            //y coordinate of death of seen photons in WCS
  Double_t        fZ0;            //z coordinate of death of seen photons in WCS


  static   void   Book();
  void            BookCanvas();
  void            BookCanvas(Short_t);
  void            BookCanvas(Int_t,Int_t);
  Short_t         CheckPath(TString&) const;
  void            CloseFiles();
  void            CloseFilesR();
  static TLit    *Get();
  static Short_t  GetLineSize()            { return fgLineSize;     }
  static TString *GetListFileName()        { return fgListFileName; }
  static Int_t    GetNbButtons()           { return fgNbButtons;    }
  static TString *GetRootFileName()        { return fgRootFileName; }
  static Short_t  GetSize()                { return fgSize;         }
  Bool_t          GetTestCalc()            { return fTestCalc;      }
  static Int_t    GetTopXGen()             { return fgTopXGen;      }
  static Int_t    GetTopYGen()             { return fgTopYGen;      }
  Bool_t          GetWCSofNode(const char*,TVector3&,TVector3&);
  static Bool_t   GetWithDate()            { return fgWithDate;     }
  static void     Kill();
  void            OpenFilesR();
  void            OpenFilesR(const char*);
  void            OpenFilesW(Bool_t = kTRUE);
  void            PrintPeriod(Long64_t=50000);
  void            PublishVersion() const;
  void            RestoreComments();
  Bool_t          SelectRun(Int_t);
  Bool_t          SelectRun(TString, Int_t);
  static void     SetLineSize(Short_t=119);
  static void     SetListFileName(const char* = "slitrani" );
  static void     SetNbButtons(Int_t=20);
  static void     SetParameters(Short_t=5,Int_t=20,Int_t=2,Int_t=2,Bool_t = kFALSE,
                    const char* = "slitrani",const char* = "slitrani",Short_t = 119);
  static void     SetRootFileName(const char* = "slitrani" );
  static void     SetSize(Short_t size);
  void            SetTestCalc(Bool_t test) { fTestCalc = test;      }
  static void     SetTopXGen(Int_t=2);
  static void     SetTopYGen(Int_t=2);
  static void     SetWithDate(Bool_t=kFALSE);
  ClassDef(TLit,0)  //Steering class of SLitrani
};
inline TLit* TLit::Get() {
  // Only way to access fgLit! Prevents to create more then one instance of
  //TLit. This, combined with the fact that constructor and destructor are
  //private, makes of TLit a singleton
  if (!fgLit) fgLit = new TLit();
  return fgLit;
}
#endif
