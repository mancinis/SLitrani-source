// @(#)root/html:$Id: TLitSellmeier.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TLitSellmeier
#define LIT_TLitSellmeier

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitSellmeier   defines index of refraction or diagonal components   //
//                 of dielectric tensor by mean of the LitSellmeier law //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TArrayD.h"

class TLitSellmeier : public TObject {

protected:

  Int_t    fN;    //Number of resonant frequencies in the LitSellmeier law
  Double_t fNs0;  //1st numerator term, associated with wavelength 0.
  TArrayD  fNsi;  //All numerator terms, associated with wavelength fLsi.
  TArrayD  fLsi;  //The resonant wavelengths

public:

  TLitSellmeier() { ; }
  TLitSellmeier(Int_t,Double_t,Double_t,Double_t,Double_t=0.0,Double_t=0.0,Double_t=0.0,Double_t=0.0);
  void     AddComponent(Double_t,Double_t);
  Double_t GetDiel(Double_t) const;
  Double_t GetIndex(Double_t,Double_t=1.0) const;
  ClassDef(TLitSellmeier,1)   //Defines LitSellmeier law
};
#endif
