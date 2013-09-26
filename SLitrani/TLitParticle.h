// @(#)root/html:$Id: TLitParticle.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitParticle
#define LITR_TLitParticle
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitParticle  Defines a particle which will generate photons when  //
//                 crossing fluorescent materials                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TParticlePDG.h"
#include "TParticle.h"

enum PDistribution {singlevalued,straigth,gaussian,exponential};

class TLitParticle  : public TParticle {

protected:

  PDistribution fPdist;        //Type of momentum distribution
  Double_t      fPmean;        //Mean momentum in Gev/c in case singlevalued or gaussian
  Double_t      fPmin;         //Min. momentum in Gev/c, case straigth, gaussian or exponential
  Double_t      fPmax;         //Max. momentum in Gev/c, case straigth, gaussian or exponential
  Double_t      fSig;          //Sigma of gaussian distribution in Gev/c
  Double_t      fSlope;        //Slope of exponential in [Gev/c]-1
  Double_t      fExp1;         //1st constant in case {exponential}
  Double_t      fD;            //Difference fExp1 - fExp2
  Double_t      fP;            //! Generated momentum [Gev/c]
  Double_t      fBeta;         //! Beta = fP/fE of particle
  Double_t      fSpeed;        //! Speed of particle  [cm/picoseconds]
  Bool_t        fEmitCerenkov; //If true (default) particle will also emit Cerenkov light
  Double_t      fCerCos;       //Cosine of Cerenkov angle
  Double_t      fCerSin;       //Sine   of Cerenkov angle
  Int_t         fCerNphot;     //Number of cerenkov photons emitted

  void          Exp12();


public:

  TLitParticle() { ; }
  TLitParticle(Int_t,PDistribution = singlevalued,Double_t=100.0,Double_t=1.0,Double_t=10.0,Double_t=0.5,Double_t=1.0);
  virtual      ~TLitParticle();
  Double_t      CerCos() const            { return fCerCos;              }
  void          Cerenkov(Double_t,Double_t=1.0);
  Double_t      CerSin() const            { return fCerSin;              }
  Bool_t        EmitCerenkov() const      { return fEmitCerenkov;        }
  void          GenP();
  Double_t      GetBeta() const           { return fBeta;                }
  Double_t      GetD() const              { return fD;                   }
  Double_t      GetEnergyMev() const      { return 1000*Energy();        }
  Double_t      GetExp1() const           { return fExp1;                }
  Double_t      GetMomentum() const       { return fP;                   }
  PDistribution GetPdist() const          { return fPdist;               }
  Double_t      GetPmax() const           { return fPmax;                }
  Double_t      GetPmean() const          { return fPmean;               }
  Double_t      GetPmin() const           { return fPmin;                }
  Double_t      GetSig() const            { return fSig;                 }
  Double_t      GetSlope() const          { return fSlope;               }
  Double_t      GetSpeed() const          { return fSpeed;               }
  Double_t      Mass() const              { return fParticlePDG->Mass(); }
  Int_t         NbCerPhot() const         { return fCerNphot;            }
	void          Print() const;
  void          SetEmitCerenkov(Bool_t c) { fEmitCerenkov = c;           }
  void          SetLorentzE(Double_t,Double_t,Double_t,Double_t);
  ClassDef(TLitParticle,1) //Define a particle which will generate photons
};
#endif
