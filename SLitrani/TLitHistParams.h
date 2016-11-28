// @(#)root/html:$Id: TLitHistParams.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitHistParams
#define LIT_TLitHistParams
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitHistParams   Histogram parameters for SLitrani into xml file      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TString.h"

class TFile;
class TObjArray;

class TLitHistParams : public TObject {

protected:

  static TFile          *fgHistParams;  //Xml file containing histogram parameters

public:

  TString     fHKind;   //TH1C, TH1F, TH1D, TH2C, TH2F, TH2D, ... TGraphErrors
  TString     fHName;   //Name of histogram or TGraph
  TString     fHTitle;  //Title of histogram or TGraph
  Int_t       fHNbinsx; //Number of bins in x
  Double_t    fHXlow;   //Left of 1st bin in x
  Double_t    fHXup;    //Right of last bin in x
  Int_t       fHNbinsy; //Number of bins in y
  Double_t    fHYlow;   //Left of 1st bin in y
  Double_t    fHYup;    //Right of last bin in y

  static Bool_t          fgHistParamsOk; //True if xml file has been found
  static TObjArray      *fgListHParams;  //Collection of histogram parameters to be saved into xml file
  static TString        *fgFileName;     //Name or TreeName of xml file containing histogram parameters

  TLitHistParams();
  TLitHistParams(const char *,const char *,const char *,Int_t,Double_t,Double_t,Int_t=0,Double_t=0.0,Double_t=0.0);
  virtual               ~TLitHistParams();
  static void            Add(TLitHistParams*);
  static void            Add(const char *,const char *,const char *,Int_t,Double_t,Double_t,Int_t=0,Double_t=0.0,Double_t=0.0);
  static void            CloseXmlFile();
  Int_t                  Compare(const TObject* obj) const;
  static void            CreateHistParams();
  static TLitHistParams *Get(const char *);
  Bool_t                 IsEqual(const TObject* obj) const;
  Bool_t                 IsSortable() const  { return kTRUE; }
  static void            OpenXmlFileR();
  static void            Purge();
  static void            PurgeStatic();
  ClassDef(TLitHistParams,1)  //Histogram parameters for SLitrani into xml file
};
#endif
