// @(#)root/html:$Id: TLitWave.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TTWave
#define LITR_TTWave
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitWave    characteristics of incident, reflected or transmitted    //
//            wave. All values in the incidence coordinate system ICS   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TComplex.h"
#include "TVector3.h"
#include "TMat33.h"

enum Sharing { NotInvolved,NotTaken,Taken,NormalTaken,ParallelTaken };

class TLitWave:public TNamed {

friend class TLitPhoton;

protected:

	Bool_t    fEva;     //True if wave evanescent
  Sharing   fShare;   //How this wave participates to process
  Short_t   fPhases;  //Number of phases for this wave
                        // 0 : nothing to add to the absolute phase of the wave
                        // 1 : it will have to be simply added to fBeta, the irrelevant absolute phase of the wave
                        // 2 : the parallel and normal components have different phases, wave elliptically polarized. We will have to choose between the 2
                        // 3 : if the number of phases is 3, the wave is absorbed.
  TComplex  fRfI;     //Refraction index associated with this wave
  Double_t  fDiel;    //Dielectric constant epsilon
	TComplex  fCtht;    //cos of angle between k vector and normal to surface separating the 2 media. tht is the angle of incidence, reflection or transmission according to the kind of wave.
	TComplex  fStht;    //sin of angle between k vector and normal to surface separating the 2 media. tht is the angle of incidence, reflection or transmission according to the kind of wave.
	Double_t  fAk;      //module of k vector
	TVector3  fUk;      //unit k vector in ICS
	TVector3  fVk;      //k vector in ICS
	TComplex  fE;       //module of electric field
	Double_t  fAE;      //absolute value of fE
	Double_t  fPE;      //phase of fE
	TComplex  fSdel;    //sin of delta angle of electric field
	TComplex  fCdel;    //cos of delta angle of electric field
  TComplex  fAlm1;    // = Stht*Sdel + Ctht*Cdel
	TComplex  fSalf;    //sin of alpha angle of electric field
	Double_t  fWsaf;    //absolute value of fSalf
	Double_t  fPsaf;    //phase of fSalf
	TComplex  fCalf;    //cos of alpha angle of electric field
	Double_t  fWcaf;    //absolute value of fCalf
	Double_t  fPcaf;    //phase of fCalf
	TVector3  fEf;      //electric field E in ICS [MKSA]
	TVector3  fEfn;     //E field normal to incident plane
	TVector3  fEfp;     //E field inside the incident plane
	TVector3  fBf;      //magnetic field B in ICS [MKSA]
	TVector3  fBfn;     //magnetic field associated with fEfn
	TVector3  fBfp;     //magnetic field associated with fEfp
	TVector3  fPg;      //Poyinting vector P in ICS [MKSA]
	TVector3  fPgn;     //Poyinting vector P in ICS [MKSA] associated with fEfn
	TVector3  fPgp;     //Poyinting vector P in ICS [MKSA] associated with fEfp
	TVector3  fUp;      //Unit Poyinting vector in ICS
	TVector3  fUpn;     //Unit Poyinting vector in ICS associated with fEfn
	TVector3  fUpp;     //Unit Poyinting vector in ICS associated with fEfp


public:

	TLitWave() { ; }
	TLitWave(const char*, const char*);
  void      Split(Bool_t);
  Bool_t    VerifySplit() const;
  Bool_t    VerifyStart(Double_t, const TMat33&) const;
  ClassDef(TLitWave,1)//characteristics of el-magn. wave
};
#endif
