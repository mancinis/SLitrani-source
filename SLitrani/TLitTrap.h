// @(#)root/html:$Id: TLitTrap.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitTrap
#define LITR_TLitTrap
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// TLitTrap  Inherits from TTRAP and add all informations asked by SLitrani //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TGeoArb8.h"
#include "TVector3.h"

class TLitTrap : public TObject {

protected:

  Double_t  fDz;     //half length in z but in double precision.
  Double_t  fTheta;  //half length in x but in double precision. DEGREE
  Double_t  fPhi;    //half length in y but in double precision. DEGREE
  Double_t  fH1;     //half length in y at low z
  Double_t  fBl1;    //half length in x at low z and y low edge
  Double_t  fTl1;    //half length in x at low z and y high edge
  Double_t  fAlpha1; //angle w.r.t. the y axis.  DEGREE
  Double_t  fH2;     //half length in y at high z
  Double_t  fBl2;    //half length in x at high z and y low edge
  Double_t  fTl2;    //half length in x at high z and y high edge
  Double_t  fAlpha2; //angle w.r.t. the y axis.  DEGREE

  TVector3  fGCenter; //Center of shape
	TVector3  fV0;      //point 0 of the TLitTrap
	TVector3  fV1;      //point 1 of the TLitTrap
	TVector3  fV2;      //point 2 of the TLitTrap
	TVector3  fV3;      //point 3 of the TLitTrap
	TVector3  fV4;      //point 4 of the TLitTrap
	TVector3  fV5;      //point 5 of the TLitTrap
	TVector3  fV6;      //point 6 of the TLitTrap
	TVector3  fV7;      //point 7 of the TLitTrap

	void      DelPoints();
  void      InitP();
	Double_t  MakePlanar();
	void      ToParam(Double_t&, Double_t&);
	void      ToPoints();

public:

  TLitTrap() { InitP(); }
  TLitTrap(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t);
  TLitTrap(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,Double_t,Double_t,Double_t&,Double_t&);
  TLitTrap(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  TLitTrap(Short_t,Double_t=0.0);
  virtual  ~TLitTrap();
  void      AllParams(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
                      Double_t&,Double_t&,Double_t&,Double_t&,Double_t&);
  TGeoTrap *GeoTrap() const;
	Double_t  Getalpha1() const { return fAlpha1; }
	Double_t  Getalpha2() const { return fAlpha2; }
	Double_t  Getbl1() const    { return fBl1;    }
	Double_t  Getbl2() const    { return fBl2;    }
  void      GetCMSdim(Short_t,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
              Double_t&,Double_t=0.0) const;
	Double_t  Getdz() const     { return fDz;     }
	TVector3* GetfV(Int_t);
	Double_t  Geth1() const     { return fH1;     }
	Double_t  Geth2() const     { return fH2;     }
	Double_t  Getphi() const    { return fPhi;    }
  void      GetPoints(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
              Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
              Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
              Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&) const;
	Double_t  Gettl1() const    { return fTl1;    }
	Double_t  Gettl2() const    { return fTl2;    }
	Double_t  Gettheta() const  { return fTheta;  }
	void      Print() const;
	void      PrintPts() const;
  ClassDef(TLitTrap,0) //Help for defining TGeoTrap
};
#endif
