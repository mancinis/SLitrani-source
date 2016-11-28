// @(#)root/html:$Id: TLitEqIndex.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitEqIndex
#define LITR_TLitEqIndex
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitEqIndex       handles the Equation giving the 2 indices in case  //
//                of anisotropy :                                       //
//                                                                      //
//                z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0     (E1)      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TGraph.h"
#include "TH1.h"
#include "TComplex.h"
#include "TCleanOut.h"

enum TypeOfEquation {bczero,degenerate,general};

class TLitEqIndex : public TObject {

protected:

  TypeOfEquation  fTypeEq;          //{bczero,degenerate,general}
  Bool_t          fWithRMP;         //If true, uses ROOT::Math::Polynomial for 4th degree polynomial
  Double_t        a,b,c,d,g;        //Coefficients of the equation (E1)
  Double_t        fGd;              //Determinant for SolveSimple
  Bool_t          fDebug;           //! True if debugging: many prints. Default false
  Short_t         fNr;              //! Number of solutions which are real
  Short_t         fNp;              //! Number of solutions with +Sqrt(z-g)
  TComplex        fZc[4];           //! The 4 solutions
  Bool_t          fBc[4];           //! The sign of Sqrt(z-g) for each solution
  Bool_t          fRc[4];           //! Whether solution is complex or real. True if complex
  Double_t        fQsc[4];          //! The quality of the solution
  Double_t        fQs2[4];          //! The quality of the solution with the other sign of Sqrt(z-g)
  TComplex        fDc[4];           //! The 4 associated cosines
  Double_t        fResidue;         //! Negativ if Solve() not yet called. Total error of solution if Solve() called
  TGraph         *fGraphP;          //! Graph of eq. (E1) with + sign for Sqrt(z-g)
  TGraph         *fGraphM;          //! Graph of eq. (E1) with - sign for Sqrt(z-g)
  TH1D           *fHRes;            //! Histogram of total error fResidue

  Bool_t          Disposition();
  void            Exchange(Short_t,Short_t);
  void            Init();
	void            InitP();
  Double_t        Newton1(Double_t&,Bool_t) const;
  Double_t        Newton2(TComplex&,Bool_t) const;
  Bool_t          Order1();
  Bool_t          Order2();
  Bool_t          Order3();
  TComplex        Rac(const TComplex&,Bool_t) const;
  Short_t         ReviSign0();
  Short_t         ReviSign1();
  Double_t        SolveDegen();
  Double_t        SolveSimple();

public:

  TLitEqIndex(Bool_t=kFALSE,Bool_t=kFALSE);
  TLitEqIndex(Int_t i);
  TLitEqIndex(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t=-1.0e+99,Bool_t=kFALSE);
  virtual        ~TLitEqIndex();
  static Bool_t   BetterConj(TComplex&,TComplex&);
  void            BookHRes();
  Short_t         ClosestFrom(Double_t,Bool_t);
  void            GetCoefficients(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&) const;
  void            GetCoefficients(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&) const;
  void            GetfB(Bool_t&,Bool_t&,Bool_t&,Bool_t&) const;
  Bool_t          GetfB(Short_t i) const  { i = TMath::Abs(i); i %= 4; return fBc[i]; }
  void            GetfD(TComplex&,TComplex&,TComplex&,TComplex&) const;
  TComplex        GetfD(Short_t i) const  { i = TMath::Abs(i); i %= 4; return fDc[i]; }
  void            GetfR(Bool_t&,Bool_t&,Bool_t&,Bool_t&) const;
  Bool_t          GetfR(Short_t i) const  { i = TMath::Abs(i); i %= 4; return fRc[i]; }
  void            GetfZ(TComplex&,TComplex&,TComplex&,TComplex&) const;
  TComplex        GetfZ(Short_t i) const  { i = TMath::Abs(i); i %= 4; return fZc[i]; }
  Double_t        GetGd() const           { return fGd;                               }
  Short_t         GetNr() const           { return fNr;                               }
  void            GetQs2(Double_t&,Double_t&,Double_t&,Double_t&) const;
  Double_t        GetQs2(Short_t i) const { i = TMath::Abs(i); i %= 4; return fQs2[i];}
  void            GetQsc(Double_t&,Double_t&,Double_t&,Double_t&) const;
  Double_t        GetQsc(Short_t i) const { i = TMath::Abs(i); i %= 4; return fQsc[i];}
  Double_t        GetResidue() const      { return fResidue;                          }
  TypeOfEquation  GetTypeEq() const       { return fTypeEq;                           }
  Bool_t          GetWithRMP() const      { return fWithRMP;                          }
  virtual Bool_t  IsEqual(const TObject*) const;
  virtual Bool_t  IsSortable() const      { return kFALSE;                            }
  void            PrintEquation(KindOfMessage=info,const char * = "") const;
  void            PrintResult(KindOfMessage=info) const;
  void            Set(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t=-1.0e+99);
  void            SetDebug(Bool_t b=kTRUE){ fDebug = b;                               }
  void            SetWithRMP(Bool_t b)    { fWithRMP=b;                               }
  void            ShowEqu(Int_t=1000,Double_t=0.0,Double_t=-1.0,Double_t=0.0,Double_t=-1.0);
  void            ShowResidue() const;
  Double_t        Solve(Short_t=3);
  TComplex        Y(TComplex,Bool_t=kTRUE) const;
  TComplex        Y(Double_t,Bool_t=kTRUE) const;
  TComplex        Y(Short_t,TComplex,Bool_t=kTRUE) const;
  TComplex        Y(Short_t,Double_t,Bool_t=kTRUE) const;
  Double_t        YAbs(TComplex,Bool_t=kTRUE) const;
  Double_t        YAbs(Double_t,Bool_t=kTRUE) const;
  TComplex        operator()(TComplex,Bool_t=kTRUE) const;
  TComplex        operator()(Double_t,Bool_t=kTRUE) const;
  Bool_t          operator==(const TLitEqIndex&);
  ClassDef(TLitEqIndex,1) //Equation giving the 2 indices in case of anisotropy
};
R__EXTERN TLitEqIndex *gLitIdx;
#endif
