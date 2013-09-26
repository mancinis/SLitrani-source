// @(#)root/html:$Id: TLitGamma.cpp 2008-06-11
// Author: D. Wahl <mailto:david.n.wahl@gmail.com>,F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TROOT.h"
#include "TMath.h"
#include "TH1.h"
#include "TGeoTube.h"
#include "TGeoEltu.h"
#include "TGeoNode.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoPhysicalNode.h"
#include "TVirtualGeoTrack.h"
#include "TwoPadDisplay.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitPhys.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLitDetector.h"
#include "TLitResults.h"
#include "TLitMarrow.h"
#include "TLitPhoton.h"
#include "TLitGamma.h"

ClassImp(TLitGamma)
//___________________________________________________________________________
//
/*BEGIN_HTML
		<div align="left">
			<p>The class <b><code>TLitGamma</code></b> is designed to simulate energy deposition 
by the <b>photo-electric effect</b> and <b>Compton scattering</b>. It has an approximate 
validity range of up to <b><code>1 MeV</code></b>. There are 3 approximations made 
in the implementation of the class:</p>
		</div>
		<ol>
			<li>Coherent scattering is not included.
			<li>Scattered electrons are not tracked: all the energy of the scattered electron 
emits light at the vertex position where the Compton scattering occured. Care should 
be taken when simulating gammas which have sufficient energy to eject electrons which 
will have a non-negligible path length. &quot;Non-negligible&quot; will depend on 
the application simulated, as well as the materials involved and the homogeneity 
of the detector.			<li>Pair creation is not implemented.
		</ol>
		<p>Points 2 and 3 are the reason to limit the use of this class to energies less 
than <b><code>1 MeV</code></b>. The Compton scattering algorithm is based on the 
paper:</p>
		<div align="center">
			<p><b><code>F.Arqueros and G.D.Montesinos, Am. J. Phys. 71(1), January 2003</code></b><font size="+1">.</font></p>
		</div>
		<ul>
			<li>The cross section for Compton scattering is calculated analytically by <b><code>TLitGamma</code></b>. 
Notice that the calculation of the Compton cross-section requires that the values 
<b><code>A, Z</code></b> and <b><code>rho</code></b> be given for the <b><code><a href="http://root.cern.ch/root/html/TGeoMedium.html">TGeoMedium</a></code></b>. 
It is the only case in <b><code><font size="+1">SLitrani</font></code></b> where this 
is required.
			<li>The photoelectric cross-section must be provided by the user in the form of 
a fit of type <b><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html"><code>TSplineFit</code></a></b> 
of category 18. The fit must provide the photo-electric cross section multiplied 
by 10<sup>24</sup> [cm<sup>2</sup>]. Cross sections can be obtained for most materials 
at: 
			<ul>
				<li><b><a href="http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html"><code>XCOM: 
Photon Cross Sections Database (NIST Standard Reference Database 8 (XGAM).</code></a><code><a href="http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html"><font size="+1"> 
</font></a></code></b>An example photo-electric fit is provided with <b><code>SLitrani</code></b> 
in the directory <b><code>FitMacros</code></b>: <b><code>PhotoEl_CaWO4.C</code></b>.			</ul>
		</ul>
		<p>As for the other classes generating photons, [<b><code><a href="TLitSpontan.html">TLitSpontan</a>, 
<a href="TLitBeam.html">TLitBeam</a> and <a href="TLitCascade.html">TLitCascade</a></code></b>], 
you have to call the constructor of the class, then you have to define the emission 
characteristics of the beam of gammas by a call to <b><code><a href="#TLitGamma:SetEmission">TLitGamma::SetEmission()</a></code></b>. 
If you forget to call <b><code><a href="#TLitGamma:SetEmission">TLitGamma::SetEmission()</a></code></b>, 
it will be called with all default values, which will probably not suit your needs. 
Notice that the emission characteristics of the beam of gammas are <b><i>exactly 
the same</i></b> as the ones defined for <b><code><a href="TLitBeam.html">TLitBeam</a></code></b>. 
Everything explained for <b><code><a href="TLitBeam.html">TLitBeam</a></code></b> 
is valid here for <b><code>TLitGamma:</code></b> There are 3 possibilities for the 
point of emission of gammas:</p>
		<ol>
			<li>gammas are emitted from any point inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
<b><code>fStartGeoVol</code></b> emitting gammas, with an equal probability.
			<li>all particles are emitted from fixed point inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>.
			<li>first, a point is generated inside <b><code>fStartGeoVol</code></b> with an 
equal probability. Then the point is translated inside <b><code>fStartGeoVol</code></b>, 
along a given direction, until the edge of <b><code>fStartGeoVol</code></b>. It is 
the way <b><code>SLitrani</code></b> generates surface emission, without having [as 
the old <b><code>SLitrani</code></b>] any handling of faces. This method has the advantage 
of working, whatever the kind of <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>.
		</ol>
		<p>The energy distribution of the gammas is defined by the constructor exactly 
as for a <b><code><a href="TlitParticle.html">TLitParticle</a></code></b>: <b><code>TLitGamma</code></b> 
inherits from <b><code><a href="TlitParticle.html">TLitParticle</a></code></b>. The 
momentum distribution can be :</p>
		<ul>
			<li><code>singlevalued : particles have always the same momentum</code>
			<li><code>straigth     : flat between fPmin and fPmax</code>
			<li><code>gaussian     : gaussian distributed, with mean fPmean ans sigma fSig.</code>
			<li><code>exponential  : exp(-b*p), with b = fSlope.</code>
		</ul>
		<p>Then you have to call method <b><code><a href="#TLitGamma:Gen">TLitGamma::Gen()</a></code></b>.</p>
		<p>Notice the parameter <b><code>moving</code></b> of the <b><code><a href="#TLitGamma:TLitGamma">constructor</a></code></b>: 
if <b><code>moving</code></b> is true [default false], the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
<b><code>fStartGeoVol</code></b> acting as beam cradle will be moved from run to 
run, by a call to method <b><code><a href="#TLitGamma:MoveCradle">MoveCradle()</a></code></b>. 
Notice that displacing a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
<b><i>after</i></b> the geometry has been closed by a call to <b><code>gGeoManager-&gt;CloseGeometry()</code></b> 
cannot be done simply by calling <b><code>TGeoVolume::RemoveNode()</code></b> and 
<b><code>TGeoVolume::AddNode()</code></b>. Read &sect; &quot;<b><i><font color="#cc3333">Representing 
Misalignments of the Ideal Geometry</font></i></b>&quot;, p340 of the <b><code><a href="ftp://root.cern.ch/root/doc/18Geometry.pdf">ROOT 
user manual</a></code></b> to understand how it is done, using class <b><code><a href="http://root.cern.ch/root/html/TGeoPhysicalNode.html">TGeoPhysicalNode</a></code></b>.</p>
		<p> During its travel through a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>, 
there are 6 possible cases:</p>
		<ol>
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="http://root.cern.ch/root/html/TGeoMedium.html">TGeoMedium</a></code></b> 
[not a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b>] for which <b><code>A, 
Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are not</font></i></b> 
defined [are 0]. Then the gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
doing nothing, and continues until the next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
No Compton scattering, no photo-electric effect, no loss of energy for the gamma. 
If you want loss of energy, you have to define <b><code>A, Z</code></b> and <b><code>rho</code></b>!
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="http://root.cern.ch/root/html/TGeoMedium.html">TGeoMedium</a></code></b> 
[not a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b>] for which <b><code>A, 
Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are</font></i></b> 
defined. Then Compton scattering is possible: we are able to calculate the Compton 
scattering cross-section. But we are not able to get a photo-electric cross-section. 
No photo-electric effect. The gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
loosing energy by Compton effect until the next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
But no light is emitted, since we need a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> 
to be able to emit light.
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> for 
which <b><code>A, Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are 
not</font></i></b> defined, and for which <b><code><a href="#TLitMedium:SetXSectnPE">TLitMedium::SetXSectnPE()</a></code></b> 
<b><i><font color="#cc3333">has not</font></i></b> been called, i.e. the photo-electric 
cross-section has not been initialized [<b><code><a href="#TLitMedium:GetPEOk">TLitMedium::GetPEOk()</a></code></b> 
returns false]. Then the gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
doing nothing, and continue until the next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
No Compton scattering, no photo-electric effect, no loss of energy for the gamma. 
If you want loss of energy, you have to define <b><code>A, Z</code></b> and <b><code>rho</code></b>!
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> for 
which <b><code>A, Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are 
not</font></i></b> defined, but for which <b><code><a href="#TLitMedium:SetXSectnPE">TLitMedium::SetXSectnPE()</a></code></b> 
<b><i><font color="#cc3333">has</font></i></b> been called, i.e. the photo-electric 
cross-section has been initialized [<b><code><a href="#TLitMedium:GetPEOk">TLitMedium::GetPEOk()</a></code></b> 
returns true]. Then the gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
doing only photo-electric effect. The gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
loosing perhaps all its energy by photo-electric effect until absorbed or until the 
next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
Light will be emitted if <b><code><a href="#TLitMedium:FindSpectrum">TLitMedium::FindSpectrum()</a></code></b> 
and <b><code><a href="#TLitMedium:SetPhotMev">TLitMedium::SetPhotMev()</a></code></b> 
have been called [<b><code><a href="#TLitMedium:IsFluorescent">TLitMedium::IsFluorescent()</a></code></b> 
returns true]. If not, no light emitted.
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> for 
which <b><code>A, Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are</font></i></b> 
defined, and for which <b><code><a href="#TLitMedium:SetXSectnPE">TLitMedium::SetXSectnPE()</a></code></b> 
<b><i><font color="#cc3333">has not</font></i></b> been called, i.e. the photo-electric 
cross-section has not been initialized [<b><code><a href="#TLitMedium:GetPEOk">TLitMedium::GetPEOk()</a></code></b> 
returns false]. Then Compton scattering is possible: we are able to calculate the 
Compton scattering cross-section. But we are not able to get a photo-electric cross-section. 
No photo-electric effect. The gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
loosing energy by Compton effect only until the next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
Light will be emitted if <b><code><a href="#TLitMedium:FindSpectrum">TLitMedium::FindSpectrum()</a></code></b> 
and <b><code><a href="#TLitMedium:SetPhotMev">TLitMedium::SetPhotMev()</a></code></b> 
have been called [<b><code><a href="#TLitMedium:IsFluorescent">TLitMedium::IsFluorescent()</a></code></b> 
returns true]. If not, no light emitted.
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is filled with a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> for 
which <b><code>A, Z</code></b> and <b><code>rho</code></b> <b><i><font color="#cc3333">are</font></i></b> 
defined, and for which <b><code><a href="#TLitMedium:SetXSectnPE">TLitMedium::SetXSectnPE()</a></code></b> 
<b><i><font color="#cc3333">has</font></i></b> been called, i.e. the photo-electric 
cross-section has been initialized [<b><code><a href="#TLitMedium:GetPEOk">TLitMedium::GetPEOk()</a></code></b> 
returns true]. Then Compton scattering and Photo-electric effect are possible: we 
are able to calculate both cross-sections. The gamma travels through the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
loosing energy by Compton scattering and by photo-electric effect until absorbed 
or until the next <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
Light will be emitted if <b><code><a href="#TLitMedium:FindSpectrum">TLitMedium::FindSpectrum()</a></code></b> 
and <b><code><a href="#TLitMedium:SetPhotMev">TLitMedium::SetPhotMev()</a></code></b> 
have been called [<b><code><a href="#TLitMedium:IsFluorescent">TLitMedium::IsFluorescent()</a></code></b> 
returns true]. If not, no light emitted.
		</ol>
		<p>On a physical point of view, only (6) makes sense, but be aware of what happens 
in your setup in the other cases.</p>
		<p>Examples of use of <b><code>TLitGamma</code></b> are provided with <b><code>SLitrani</code></b> 
in the directory <b><code>Macros</code></b>:</p>
		<ul>
			<li><b><code>Gamma.C and Gamma2.C</code></b>.
		</ul>
END_HTML */
//
TLitGamma::TLitGamma(const char *name, const char *title,const char *path,Double_t E0,
  Bool_t moving,Bool_t checkmove,PDistribution dist,Double_t Pmin,Double_t Pmax,
  Double_t sig,Double_t b) {
  //  Defines a beam of gammas, starting from a TGeoVolume pointed by node "path". 
  // Calling this constructor is not enough: you have to define the emission
  // characteristics of the beam of gammas by a call to TLitGamma::SetEmission().
  // If you forget to call TLitGamma::SetEmission(), it will be called with
  // all default values, which will probably not suit your needs.
  //  Notice that the emission characteristics of the beam of gammas are exactly
  // the same as the ones defined for TLitBeam. Everything explained for TLitBeam
  // is valid here for TLitGamma.
  //  The distribution in energy of the gammas produced are the one defined for
  // a TLitParticle. They are specified with the 5 last parameters. If you accept
  // all default values, you will have gammas of fixed energy E0.
  //
  //  Parameters
  //
  //  name      : name  of this source of gammas
  //  title     : title of this source of gammas
  //  path      : path name, like "/TOP_1/CRADLE_1", pointing to the TGeoVolume
  //              from which gammas can be emitted
  //  E0        : energy [==momentum] of gamma IN MEV [default 1.0]
  //  moving    : if moving is true [default false], the TGeoVolume acting as
  //              beam cradle and pointed to by path will be moved from run
  //              to run, by a call to method MoveCradle(). Notice that displacing
  //              a TGeoVolume AFTER the geometry has been closed by a call to
  //              gGeoManager->CloseGeometry() cannot be done simply by calling
  //              TGeoVolume::RemoveNode() and TGeoVolume::AddNode(). Read
  //              § "Representing Misalignments of the Ideal Geometry", p340 of the
  //              ROOT user manual to understand how it is done, using class
  //              TGeoPhysicalNode.
  //              [default false]
  //  checkmove : if true [default false], ask TGeo to check that no extrusion
  //              have occured during the move
  //              [default false]
  //  dist      : distribution in momentum : {singlevalued,straigth,gaussian,exponential}
  //              [default : singlevalued]  
  //  Pmin      : minimum momentum IN MEV/c, valid for {straigth,gaussian,exponential}
  //              [default : 1.0]
  //  Pmax      : maximum momentum IN MEV/c, valid for {straigth,gaussian,exponential}
  //              [default : 1.0]
  //  sig       : standard deviation of distribution IN MEV/c, valid for {gaussian}
  //              [default : 0.1]
  //  b         : slope of exponential distribution in [MEV/c]-1
  //              [default : 1000.0]
  //
  const char      *met = "TLitGamma";
  const Double_t ingev = 0.001;
  TString nameGeoV = "";
  Init();
  fName       = name;
  fTitle      = title;
  fSourcePath = path;
  NameFromPath(nameGeoV);
  fStartGeoVol = gGeoManager->FindVolumeFast(nameGeoV.Data());
  if (!fStartGeoVol) gCleanOut->MM(fatal,met,"TGeoVolume not found",ClassName());
  fStartLitVol = (TLitVolume*)fStartGeoVol->GetField();
  fMoving     = moving;
  fCheckMove  = checkmove;
  if (fMoving) {
    fGeoPhysNode = new TGeoPhysicalNode(fSourcePath.Data());
    if (!fGeoPhysNode) gCleanOut->MM(fatal,met,"TGeoPhysicalNode not created",ClassName());
    if (nameGeoV.CompareTo(fStartGeoVol->GetName()))
      gCleanOut->MM(fatal,met,"Bad name for TGeoPhysicalNode",ClassName());
  }
  E0         *= ingev;
  fPmean      = E0;
  SetPdgCode(22);
  //  This setting of SetMomentum corresponds to SetEmission called without
  // arguments. It will be corrected if you call SetEmmission with arguments
  SetMomentum(0.0,E0,0.0,E0);
  fPdist      = dist;
  fPmin       = ingev*Pmin;
  fPmax       = ingev*Pmax;
  fSig        = ingev*sig;
  fSlope      = b/ingev;
  Exp12();
  fEmitCerenkov = kFALSE;
  fCerNphot     = 0;
  fFillDeath  = kFALSE;
  if (!gLitGs) {
    gLitGs = new TLitResults("GlobStat","Global statistics of all runs",0);
    AdaptHistMom(gLitGs->fHPMomentum);
  }
  if (!SetEmission())
    gCleanOut->MM(fatal,met,"SetEmission() failed",ClassName());
}
TLitGamma::~TLitGamma() {
  Clear();
}
void TLitGamma::AdaptHistMom(TH1D *h) {
  //  Histogram TLitResults::fHPMomentum has to be modified to be suited for
  // TLitGamma instead of TLitBeam
  TAxis *xaxis;
  xaxis = h->GetXaxis();
  xaxis->SetLimits(0.0,TLitResults::fgEnergyOfGamma);
  h->SetTitle("Momentum [Mev] of generated gammas");
}
void TLitGamma::Clear() {
  // Clear pointers owned by TLitGamma
  if (fPhot) {
    delete fPhot;
    fPhot = 0;
  }
}
void TLitGamma::ComptonOk() {
  // Calculation of Compton effect requires A,Z and rho
  const Double_t zero = 0.0;
  fZ = TLitPhys::Get()->GetZ(fGeoMedium);
  fA = TLitPhys::Get()->GetA(fGeoMedium);
  fDensity = fGeoMedium->GetMaterial()->GetDensity();
  fComptonOk = (fZ>zero);
  if (fComptonOk) {
    fComptonOk = (fDensity>zero);
    if (fComptonOk) {
      fComptonOk = (fA>zero);
    }
  }
}
Bool_t TLitGamma::DepositEnergy() {
  //  The gamma has a position fX, a direction fDir and an energy fEIncident
  // = GetEnergyMev(). This method checks if Photo-electric effect or Compton
  // scattering occur in the TGeoVolume where fX is.
  //   - If yes, decides which process occurs. If photo-electric, gamma is absorbed
  //     and dead. If Compton, calculates new energy and new direction for the gamma
  //   - If no, returns with the gamma just entering in the next TGeoVolume.
  //     If the next TGeoVolume does not exists, (the gamma has reached the end
  //     of the geometry) the returned value is false
  //
  const char *met = "DepositEnergy";
  const Double_t zero       = 0.0;
  const Double_t eps        = 1.0e-12;
  const Double_t theta_min  = 0.0;
  const Double_t theta_max  = TMath::Pi();
  const Double_t Fmin       = 0.0;
  Bool_t pursue = kTRUE;
  Int_t bin;
  const Double_t *point;
  const Double_t *dir;
  Double_t Phi,sPhi,cPhi;
  Double_t Fmax;
  Double_t Theta,sTheta,cTheta;
  Double_t F;
  Double_t m1,m2,m3;
  Double_t n1,n2;
  Double_t p1, p2, p3,p12;
  Double_t q1,q2,q3;
  Double_t Theta1,sTheta1,cTheta1;
  Double_t Phi1,sPhi1,cPhi1;
  Double_t pathlength;
  TVector3 v;
  fEDeposit  = zero;
  fEIncident = GetEnergyMev();
  GetXSectnKN();
  GetXSectnPE();
  // Find coordinates of interaction and check if in shape
  pathlength = GetPathLength();
  if (pathlength>fStepMax) {
    pursue = NextCrossing();
    return pursue;
  }
  else {
    gGeoManager->SetStep(pathlength);
    fNextNode = gGeoManager->Step(kFALSE,kFALSE);
    if (fNextNode!=fNode)
      gCleanOut->MM(error,met,"Not allowed step outside volume",ClassName());
    point  = gGeoManager->GetCurrentPoint();
    dir    = gGeoManager->GetCurrentDirection();
    fX.SetXYZ(point[0],point[1],point[2]);
    fDir.SetXYZ(dir[0],dir[1],dir[2]);
    fRoad += pathlength;
    fTime += pathlength/GetSpeed();
    if (fToDraw) fCurrentTrack->AddPoint(fX[0],fX[1],fX[2],fTime);
  }
  //cout << "-------- Depositing Energy: " << endl;
  //Compton Scattering or PE?
  if (gRandom3->Rndm() < (fXsecPhotoEl/(fXsecPhotoEl + fXsecCompton))) { // Photo-Electric
    fProcess = kPhotoElectric;
    fTotEdeposit += fEIncident;
    fEDeposit   = fEIncident;
    fGammaDead  = kTRUE;
    fEScattered = zero;
    fEIncident  = zero;
  }
  else { // Compton
    fProcess = kComptonScattering;
    Phi  = 2*TMath::Pi()*gRandom3->Rndm();
    sPhi = TMath::Sin(Phi);
    cPhi = TMath::Cos(Phi);
    Fmax = Get_dSigma_dOmega(theta_min);
    //Random variables for acceptance-rejection method
    do {
      Theta = theta_min +  gRandom3->Rndm()*(theta_max - theta_min);
      sTheta = TMath::Sin(Theta);
      cTheta = TMath::Cos(Theta);
      F = Fmin + gRandom3->Rndm()*(Fmax - Fmin);
    } while (F > Get_dSigma_dOmega(Theta)*sTheta);
    // Calculate next direction of the gamma
    // This other method avoid the calculations of Sin(), Cos(), ACos() ...
    // It constructs a provisional coord. system where Oz is || to fDir,
    // and Oy is horizontal.
    p1 = fDir.X();
    p2 = fDir.Y();
    p3 = fDir.Z();
    fDirElec = fEIncident*fDir;
    p12 = TMath::Sqrt(p1*p1 + p2*p2);
    if (p12<eps) {
      Theta1  = Theta;
      Phi1    = Phi;
      sTheta1 = sTheta;
      cTheta1 = cTheta;
      sPhi1   = sPhi;
      cPhi1   = cPhi;
      q1      = sTheta1*cPhi1;
      q2      = sTheta1*sPhi1;
      q3      = cTheta1;
    }
    else {
      n1 =  p2/p12;        // == +sPhi0
      n2 = -p1/p12;        // == -cPhi0
      m1 =  n2*p3;         // == -cTheta0*cPhi0
      m2 = -n1*p3;         // == -cTheta0*sPhi0
      m3 =  n1*p2 - n2*p1; // ==  sTheta0
      q1 =  sTheta*cPhi*m1 + sTheta*sPhi*n1 + cTheta*p1;
      q2 =  sTheta*cPhi*m2 + sTheta*sPhi*n2 + cTheta*p2;
      q3 =  sTheta*cPhi*m3                  + cTheta*p3;

    }
    fDir.SetXYZ(q1,q2,q3);
    fDir = fDir.Unit();  // to cope with numerical errors
    q1   = fDir.X();
    q2   = fDir.Y();
    q3   = fDir.Z();
    gGeoManager->SetCurrentDirection(q1,q2,q3);
    fEDeposit  = fEIncident - fEIncident/(1+(fEIncident/TLitPhys::Get()->Me())*(1-cTheta));
    fEScattered = fEIncident - fEDeposit;
    SetLorentzE(q1,q1,q3,0.001*fEScattered);
    fTotEdeposit += fEDeposit;
    fDirElec = fDirElec - fEScattered*fDir;
    fDirElec = fDirElec.Unit();
    fNCompton++;
  }
  if (fKGeoVolume>=0) {
    gCleanOut->MMR(info,met,fGeoVolume->GetName(),"Mev",fEDeposit,ClassName()); 
    if (gLitGs) bin = gLitGs->fHEnergyInShape->Fill((Double_t)fKGeoVolume,fEDeposit);
    if (gLitCs) bin = gLitCs->fHEnergyInShape->Fill((Double_t)fKGeoVolume,fEDeposit);
  }
  return pursue;
}
Bool_t TLitGamma::EmissionToWCS() {
  //  Given a fSourcePath like "/T_1/A_2/B_2", pointing to a particular instance [node] of
  // a TGeoVolume, calculates:
  //
  //  (1) fSourcePoint => fWCSSourcePoint  position of emission point in WCS
  //  (2) fSourceDir   => fWCSSourceDir    direction of emission in WCS
  //  (4) fDirFace     => fWCSDirFace      direction to take to reach face, in WCS
  //
  //  This method also transfers the emission point fWCSSourcePoint towards the
  // face of the TGeoVolume, along direction fWCSDirFace in case fFromFace is
  // true.
  //  fSourcePath must have been set before calling this method
  //  To be called only AFTER closing of the geometry
  //
  const char *met = "EmissionToWCS";
  const char lim = '/';
  Int_t kk;
  Short_t depth; //depth of fSourcePath
  Short_t k;     //index inside fSourcePath
  Short_t nn;    //next  '/' to be reached
  Short_t n;     //counter of '/' encountered
  Ssiz_t  N;     //Length of string fSourcePath
  Short_t j;
  Bool_t found1 = kFALSE;
  Bool_t found;
  Double_t x,y;
  Double_t local1[3],master1[3];       // for fSourcePoint
  Double_t localdir[3],masterdir[3];   // for fSourceDir
  Double_t local3dir[3],master3dir[3]; // for fDirFace
  TGeoVolume *vol;
  TGeoNode *node;
  TGeoNode *daughter = 0;
  TGeoNode *foundnode[10];
  TObjArray *nodes;
  TString snode,sdaughter;
  depth = TLit::Get()->CheckPath(fSourcePath);
  switch (depth) {
    case 0:
      gCleanOut->MM(fatal,met,"Bad path of Tgeo",ClassName());
      break;
    case 1:
      fWCSSourcePoint = fSourcePoint;
      fWCSSourceDir   = fSourceDir;
      if (fFromFace) fWCSDirFace = fDirFace;
      found1    = kTRUE;
      break;
    default:
      N  = fSourcePath.Length();
      if (fSourcePath(N-1)==lim) fSourcePath.Resize(--N);
      for (k=0;k<3;k++) {
        local1[k]   = fSourcePoint[k];
        localdir[k] = fSourceDir[k];
        if (fFromFace)     local3dir[k] = fDirFace[k];
      }
      k  = 0;
      n  = 0;
      nn = 2;
      while ((k<N) && (n<nn)) {
        if (fSourcePath(k) == lim) n++;
        k++;
      }
      nn++; 
      vol = gGeoManager->GetTopVolume();
      found = kTRUE;
      while ((found) && (k<N)) {
        snode = "";
        while ((k<N) && (n<nn)) {
          if (fSourcePath(k) == lim) n++;
          else snode.Append(fSourcePath(k)); 
          k++;
        }
        nodes = vol->GetNodes();
        TIter next(nodes);
        found = kFALSE;
        while ((!found) && (node = (TGeoNode*)next())) {
          if (!snode.CompareTo(node->GetName())) {
            daughter = node;
            foundnode[nn-3] = node;
            found = kTRUE;
          }
        }
        if (found) vol = daughter->GetVolume();
        nn++;
      }// end while ((found) && (k<N))
      found1 = found;
      nn -= 3;
      if (found1) {
        for (k=0;k<nn;k++) {
          node = foundnode[nn-k-1];
          node->LocalToMaster(local1,master1);
          node->LocalToMasterVect(localdir,masterdir);
          if (fFromFace)     node->LocalToMasterVect(local3dir,master3dir);
          for (j=0;j<3;j++) {
            local1[j] = master1[j];
            localdir[j] = masterdir[j];
            if (fFromFace)     local3dir[j] = master3dir[j];
          }
        }
        for (j=0;j<3;j++) {
          fWCSSourcePoint[j] = master1[j];
          fWCSSourceDir[j]   = masterdir[j];
          if (fFromFace)     fWCSDirFace[j] = master3dir[j];
        }
      }
      break;
  }//end switch (depth)
  //  Generation of a complete coordinate system in WCS, having fWCSSourceDir as
  // z axis.
  fWCSSourceDir = fWCSSourceDir.Unit();
  fAxisb1.SetXYZ(1.0,0.0,0.0);
  fAxisb2.SetXYZ(0.0,1.0,0.0);
  x = TMath::Abs(fAxisb1*fWCSSourceDir);
  y = TMath::Abs(fAxisb2*fWCSSourceDir);
  if (x<=y) {
    fAxisb2 = fWCSSourceDir.Cross(fAxisb1);
    fAxisb2 = fAxisb2.Unit();
    fAxisb1 = fAxisb2.Cross(fWCSSourceDir);
    fAxisb1 = fAxisb1.Unit();
  }
  else {
    fAxisb1 = fWCSSourceDir.Cross(fAxisb2);
    fAxisb1 = fAxisb1.Unit();
    fAxisb2 = fWCSSourceDir.Cross(fAxisb1);
    fAxisb2 = fAxisb2.Unit();
  }
  if (TLit::Get()->GetTestCalc()) {
    const Double_t eps = 1.0e-12;
    x = TMath::Abs(fAxisb1*fAxisb2);
    if (x>eps) gCleanOut->MM(error,met,"fAxisb1 not orthogonal to fAxisb2",ClassName());
    x = TMath::Abs(fAxisb1*fWCSSourceDir);
    if (x>eps) gCleanOut->MM(error,met,"fAxisb1 not orthogonal to fWCSSourceDir",ClassName());
    x = TMath::Abs(fAxisb2*fWCSSourceDir);
    if (x>eps) gCleanOut->MM(error,met,"fAxisb2 not orthogonal to fWCSSourceDir",ClassName());
    x = TMath::Abs(1.0-fAxisb1*(fAxisb2.Cross(fWCSSourceDir)));
    if (x>eps) gCleanOut->MM(error,met,"System not orthonormal",ClassName());
  }
  const char *path;
  if (fFromFace) {
    const Double_t *point;
    fWCSDirFace = fWCSDirFace.Unit();
    fNode = gGeoManager->InitTrack(fWCSSourcePoint[0],fWCSSourcePoint[1],fWCSSourcePoint[2],
      fWCSDirFace[0],fWCSDirFace[1],fWCSDirFace[2]);
    if (!fNode) gCleanOut->MM(fatal,met,"Gamma created outside world",ClassName());
    path = gGeoManager->GetPath();
    // Go just after the exit face
    fNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    point = gGeoManager->GetCurrentPoint();
    fWCSSourcePoint.SetXYZ(point[0],point[1],point[2]);
    gGeoManager->SetCurrentDirection(fWCSSourceDir[0],fWCSSourceDir[1],fWCSSourceDir[2]);
  }
  else {
    fNode = gGeoManager->InitTrack(fWCSSourcePoint[0],fWCSSourcePoint[1],fWCSSourcePoint[2],
      fWCSSourceDir[0],fWCSSourceDir[1],fWCSSourceDir[2]);
    if (!fNode) gCleanOut->MM(fatal,met,"Gamma created outside world",ClassName());
    path = gGeoManager->GetPath();
  }
  kk = fSourcePath.CompareTo(path);
  if (kk) {
    found1 = kFALSE;
    gCleanOut->MM(error,met,"User path different from found path",ClassName());
    gCleanOut->MMT(error,met,fSourcePath.Data(),"given by","user",ClassName());
    gCleanOut->MMT(fatal,met,path,"found by","TGeo",ClassName());
  }
  if (!found1) gCleanOut->MM(fatal,met,"EmissionToWCS failed",ClassName());
  return found1;
}
void TLitGamma::EmitLight() {
  //  Emit the corrresponding number of photons for a deposited energy of
  // fEDeposit Mev at the point fX in the shape.
  const char *met = "EmitLight";
  Int_t kfluo,bin,kphot,nphot;
  Axis_t afluo;
  TVector3 K;         //k vector of generated photon
  TVector3 E;         //direction of the E field of the photon
  nphot = fLitMedium->GetNPhot(fEDeposit);
  //
  // Loop on generation of photons
  //
  for (kphot=1;kphot<=nphot;kphot++) {
    gLitGs->fNpGener++;
    if (gLitCs) gLitCs->fNpGener++;
    // determines wavelength and time of emission of photon and store results
    // in histograms
    fLitMedium->WaveAndLife(fWvlgth,fT0,kfluo);
    //update all values depending upon wavelength
    TLitMedium::NewWavelengthAll(fWvlgth);
    TLitVolume::NewWavelengthAll(fWvlgth);
    bin = gLitGs->fHTimeAll->Fill(fT0);
    bin = gLitGs->fHWvlgthAll->Fill(fWvlgth);
    if (gLitCs) {
      bin = gLitCs->fHTimeAll->Fill(fT0);
      bin = gLitCs->fHWvlgthAll->Fill(fWvlgth);
    }
    if (gLitGs->HasFluo()) {
      afluo = kfluo;
      bin = gLitGs->fHTimeEach->Fill(fT0,afluo);
      bin = gLitGs->fHWvlgthEach->Fill(fWvlgth,afluo);
    }  //if (gLitGs->HasFluo())
    if ((gLitCs) && (gLitCs->HasFluo())) {
      afluo = kfluo;
      bin = gLitCs->fHTimeEach->Fill(fT0,afluo);
      bin = gLitCs->fHWvlgthEach->Fill(fWvlgth,afluo);
    }  //if (gLitCs)
    //generates the photon
    fT0 += fTime;
    fX0 = fX;
    GenK(K);
    GenE(K,E);
    fPhot = new TLitPhoton(fRun,fNphot+kphot,fX0,fGeoVolume,fPath.Data(),fWvlgth,fT0,K,E,0);
    //follow the photon from its birth to its death
    fPhot->Move();
    if (gLitGs->fNpAbnorm>TLitPhys::Get()->Anomalies()) {
      gCleanOut->MM(fatal,met,"Too many anomalies",ClassName());
    }
    if (fFillDeath) FillDeath();
    delete fPhot;
    fPhot = 0;
  }//end for loop on photons
  TString s;
  switch (fProcess) {
    case kPhotoElectric:
      s.Append("PhotoElectric effect: ");
      break;
    case kComptonScattering:
      s.Append("Compton scattering  : ");
      break;
    default:
      break;
  }
  s.Append("Nb. of photons");
  gCleanOut->MMI(info,met,s.Data(),"nphot",nphot,ClassName());
  fNphot += nphot;
  fNode = gGeoManager->InitTrack(fX[0],fX[1],fX[2],fDir[0],fDir[1],fDir[2]);
  if (!fNode) gCleanOut->MM(fatal,met,"fNode = 0 at exit of EmitLight",ClassName());
  if (fToDraw) {
    gGeoManager->SetCurrentTrack(fTrackIndex);
    fCurrentTrack = gGeoManager->GetCurrentTrack();
  }
}
void TLitGamma::FillDeath() const {
  // This method is called when a photon dies, in order to record the
  // coordinates of the death point, for the cases where the photon is seen.
  Int_t bin;
  if ((fFillDeath) && (TLit::Get()->fSeen)) {
    bin = fHX0->Fill(TLit::Get()->fX0);
    bin = fHY0->Fill(TLit::Get()->fY0);
    bin = fHZ0->Fill(TLit::Get()->fZ0);
  }
}
void TLitGamma::Gen(Int_t run, Int_t Ngamma, Double_t xparam, Bool_t runstat,Bool_t forgetlast) {
  //  Starts a run generating Ngamma gammas. During its travel through a TGeoVolume,
  // there are 6 possible cases:
  //   (1) - The TGeoVolume is filled with a TGeoMedium [not a TLitMedium] for which
  // A,Z and rho ARE NOT defined [are 0]. Then the gamma travels through the TGeoVolume
  // doing nothing, and continues until the next TGeoVolume. No Compton scattering, no
  // photo-electric effect, no loss of energy for the gamma. If you want loss of energy,
  // you have to define A, Z and rho!
  //   (2) - The TGeoVolume is filled with a TGeoMedium [not a TLitMedium] for which
  // A,Z and rho ARE defined. Then Compton scattering is possible: we are able to
  // calculate the Compton scattering cross-section. But we are not able to get a
  // photo-electric cross-section. No photo-electric effect. The gamma travels through
  // the TGeoVolume loosing energy by Compton effect until the next TGeoVolume. But
  // no light is emitted, since we need a TLitMedium to be able to emit light.
  //   (3) - The TGeoVolume is filled with a TLitMedium for which A, Z and rho ARE NOT
  // defined, and for which TLitMedium::SetXSectnPE() HAS NOT been called, i.e. the
  // photo-electric cross-section has not been initialized [TLitMedium::GetPEOk()
  // returns false]. Then the gamma travels through the TGeoVolume doing nothing, and
  // continue until the next TGeoVolume. No Compton scattering, no photo-electric
  // effect, no loss of energy for the gamma. If you want loss of energy, you have to
  // define A, Z and rho!
  //   (4) - The TGeoVolume is filled with a TLitMedium for which A, Z and rho ARE NOT
  // defined, but for which TLitMedium::SetXSectnPE() HAS been called, i.e. the
  // photo-electric cross-section has been initialized [TLitMedium::GetPEOk()
  // returns true]. Then the gamma travels through the TGeoVolume doing only Photo-
  // electric effect. The gamma travels through the TGeoVolume loosing perhaps all its
  // energy by Photo-electric effect until absorbed or until the next TGeoVolume.
  // Light will be emitted if TLitMedium::FindSpectrum() and TLitMedium::SetPhotMev()
  // have been called [TLitMedium::IsFluorescent() returns true]. If not, no light emitted.
  //   (5) - The TGeoVolume is filled with a TLitMedium for which A, Z and rho ARE
  // defined, and for which TLitMedium::SetXSectnPE() HAS NOT been called, i.e. the
  // photo-electric cross-section has not been initialized [TLitMedium::GetPEOk()
  // returns false]. Then Compton scattering is possible: we are able to calculate
  // the Compton scattering cross-section. But we are not able to get a photo-electric
  // cross-section. No photo-electric effect. The gamma travels through the TGeoVolume
  // loosing energy by Compton effect only until the next TGeoVolume. Light will be
  // emitted if TLitMedium::FindSpectrum() and TLitMedium::SetPhotMev() have been called
  // [TLitMedium::IsFluorescent() returns true]. If not, no light emitted.
  //   (6) - The TGeoVolume is filled with a TLitMedium for which A, Z and rho ARE
  // defined, and for which TLitMedium::SetXSectnPE() HAS been called, i.e. the
  // photo-electric cross-section has been initialized [TLitMedium::GetPEOk()
  // returns true]. Then Compton scattering and Photo-electric effect are possible:
  // we are able to calculate both cross-sections. The gamma travels through the
  // TGeoVolume loosing energy by Compton scattering and by photo-electric effect
  // until absorbed or until the next TGeoVolume. Light will be emitted if TLitMedium::
  // FindSpectrum() and TLitMedium::SetPhotMev() have been called [TLitMedium::
  // IsFluorescent() returns true]. If not, no light emitted.
  //
  //  On a physical point of view, only (6) makes sense, but be aware of what happens
  // in your setup in the other cases.
  //
  //  PARAMETERS :
  //
  //  run        : run number. Arbitrary, but has to be greater than 0.
  //  Ngamma     : number of gammas to be generated
  //  xparam     : this parameter is used as abscissa in the plotting of the
  //               results by the class TLitMarrow. For instance, if you
  //    have a setup with a crystal and a beam of gammas transversed to the axis
  //    of the crystal and you make 10 runs varying the coordinate of the beam along
  //    the axis of the crystal, you can choose this coordinate as xparam. All
  //    plots provided by TLitMarrow will then have this coordinate as x coor-
  //    dinate. You will get efficiency versus beam position along axis of crystal,
  //    and so on. If you do not give xparam, or give a value smaller than -1.0e+20,
  //    all plots of TLitMarrow will have the run number as x coordinate.
  //    The title of class TLitMarrow, pointed to by the global pointer gLitGp,
  //    will help giving a meaningful title to the x axis. For instance, calling:
  //
  //       ==> gLitGp->SetTitle("beam position along axis");
  //
  //   the title of the x axis of the histogram of efficiency will be:
  //
  //       ==> "Efficiency versus beam position along axis"  //
  //
  //  runstat and ForgetLast :
  //
  //    if runstat == true [Default] AND ForgetLast == false [Default] :
  //
  //      in memory   : this run statistics keeped but this run histos deleted
  //                     when next run begins
  //      on the file : this run statistics and histos recorded
  //
  //            It is the only configuration which allows the working of
  //          TPublication, i.e. allowing to show histograms of quantities
  //          varying as a function of a run dependent parameter.
  //          Usage : normal case : try it first.
  //
  //    if runstat == true AND ForgetLast == true :
  //
  //      in memory   : this run statistics and histograms deleted when next
  //                     run begins.
  //      on the file : this run statistics and histos recorded
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //          Advantage    : no increase of memory with runs
  //                         per run histograms still available on file
  //          Usage : use this configuration when your number of runs is big
  //                  but you still want per run statistics and histograms
  //                  on file.
  //
  //    if runstat == false ( ForgetLast true or false ) :
  //
  //      in memory   : no statistics and histograms for this run
  //      on the file : no statistics and histograms for this run
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //                         per run statistics and histograms not available
  //          Advantage    : no increase of memory with runs
  //                         a bit faster, half less histograms to fill
  //          Usage : use this configuration when your number of runs is very
  //                  big and you are not interested in the per run statistics
  //                  and histograms.
  //
  //  Notice that in any case, the global statistics and histograms for all
  //  runs is always present in memory and on the file. This global statistics
  //  is an object of class TLitResults, pointed to by the pointer gLitGs.
  //
  const char     *met = "Gen";
  const Double_t zero = 0.0;
  const Double_t eps  = 1.0e-8;
  Bool_t IsgGpok,crossing,light;
  Int_t iGamma,bin;
  fRunStat    = runstat;
  fForgetLast = forgetlast;
  if (run<=0) {
    gCleanOut->MM(error,met,"Run number must be >= 1",ClassName());
    gCleanOut->MM(error,met,"Abs() + 1 taken",ClassName());
    run = TMath::Abs(run);
    run += 1;
  }
  fRun = run;
  NameFile();
  gCleanOut->M(info,"");
  gCleanOut->MM(info,met,fFullTitle.Data(),ClassName());
  // Initializes the summary statistics of class TLitMarrow
  IsgGpok = (fRunStat && (!fForgetLast));
  if (!gLitGp && IsgGpok) gLitGp = new TLitMarrow("Summary","Runs",IsgGpok);
  if (gLitCs) {
    // If gLitCs is different from 0 here, it is the gLitCs of the previous run
    if (fForgetLast) {
      //  If fForgetLast is true, the gLitCs of the previous run is removed from
      // the collection and then deleted. The content of TLitDetector::fgLitDetRun
      // is also deleted. No increase of memory used from run to run, but statistics
      // calculation at the end only possible by reading the ROOT file
      TLitResults::fgResults->Remove(gLitCs);
      delete gLitCs;
      gLitCs = 0;
    }
    else {
      //  If fForgetLast is false, the variable containing the statistics, like
      // fNpGener,fNpSeen,fNpLossAny,...and so on are preserved, but all histograms
      // are deleted. It is also the case for the detectors in TLitDetector::fgLitDetRun:
      // statistics variable preserved, but histograms deleted. gLitCs of the previous
      // run remains without its histograms inside collection TLitResults::fgResults,
      // allowing statistics calculations at the end of all runs, without opening
      // the ROOT file
      gLitCs->DelHistos();
    }
  }
  gLitCs = 0;
  //  Books a new gLitCs for this run. A new collection of detectors
  // TLitDetector::fgLitDetRun will also be booked
  if (fRunStat) {
    gLitCs = new TLitResults(fFullName.Data(),fFullTitle.Data(),fRun,xparam);
    AdaptHistMom(gLitCs->fHPMomentum);
  }
  // Open the .root file for writing, if not yet done
  if (TLit::Get()->fFilesClosed) {
    TLit::Get()->OpenFilesW(fRunStat);
    gROOT->cd();
    if ((!fRunStat) && (gLitCs)) {
      delete gLitCs;
      gLitCs = 0;
    }
  }
  // Loop on generation of gammas
  TLitDetector::fgLastUsedDet = 0;
  for (iGamma=1;iGamma<=Ngamma;iGamma++) {
    StartOneGamma();
    bin = gLitGs->fHPMomentum->Fill(GetEnergyMev());
    bin = gLitGs->fHPTheta->Fill(fTheta);
    if (gLitCs) {
      bin = gLitCs->fHPMomentum->Fill(GetEnergyMev());
      bin = gLitCs->fHPTheta->Fill(fTheta);
    }
    IsToDraw(iGamma);
    if (fToDraw) {
      fRecordedTracks++;
      fTrackIndex   = gGeoManager->AddTrack(iGamma,GetPdgCode());
      gGeoManager->SetCurrentTrack(fTrackIndex);
      fCurrentTrack = gGeoManager->GetCurrentTrack();
      fCurrentTrack->AddPoint(fX[0],fX[1],fX[2],fTime);
    }
    crossing = kTRUE;
    while (crossing) {
      if ((!fComptonOk) && (!fPhotoElecOk)) crossing = NextCrossing();
      else {
        fNextNode = gGeoManager->FindNextBoundary(TLitPhys::Get()->BiggestStep());
        fStepMax  = gGeoManager->GetStep();
        crossing = DepositEnergy();
        light = kFALSE;
        if (fLitMedium) {
          light = (fLitMedium->IsFluorescent()) && (fLitMedium->GetPhotMev()>zero)
            && (fEDeposit>eps);
        }
        if (light) EmitLight();
        if (fGammaDead) crossing = kFALSE;
      }//end else ((!fComptonOk) && (!fPhotoElecOk))
    }//end while (crossing)
  }//end for (iGamma=0;iGamma<Ngamma;iGamma++)
  if (fRecordedTracks) {
    if (!gTwoPad) TLit::Get()->BookCanvas();
    gGeoManager->DrawTracks();
  }
  //Put statistics of this run into the tree.
  if (gLitCs) {
    gLitCs->Conclusion();
    TLit::Get()->fLitFile->cd();
    TLit::Get()->fLitNb += TLit::Get()->fLitTree->Fill();
    gROOT->cd();
  }
}
void TLitGamma::Gen4pi() {
  // Generates fTheta and fPhi according to sin(t)dtdf   0<fTheta<pi
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  Double_t x,y;
  x = gRandom3->Rndm();
  y = un - deux*x;
  fTheta = TLitPhys::ACos(y);
  fPhi   = deux*TMath::Pi()*gRandom3->Rndm();
}
void TLitGamma::GenE(const TVector3& K,TVector3& E) const {
  //  Generates the direction of the E field of the photon in the plane perpendicular
  // to K. Refuses to do it inside anisotropic material, where one is not at all free
  // to choose it in the plane orthogonal to k, but where it is fixed when K is fixed.
  const char *met = "GenE";
  Double_t s,c,phi,x;
  if (!fLitMedium->GetAnisotropy()) {
    TVector3 Ox(1.0,0.0,0.0);
    TVector3 Oy(0.0,1.0,0.0);
    s = TMath::Abs(K*Ox);
    c = TMath::Abs(K*Oy);
    if (s>c) Ox = Oy;
    Oy = K.Cross(Ox);
    Oy = Oy.Unit();
    Ox = Oy.Cross(K);
    Ox = Ox.Unit();
    if (TLit::Get()->GetTestCalc()) {
      const Double_t eps = 1.0e-12;
      x = TMath::Abs(Ox*Oy);
      if (x>eps) gCleanOut->MM(error,met,"Ox not orthogonal to Oy",ClassName());
      x = TMath::Abs(Ox*K);
      if (x>eps) gCleanOut->MM(error,met,"Ox not orthogonal to K",ClassName());
      x = TMath::Abs(Oy*K);
      if (x>eps) gCleanOut->MM(error,met,"Oy not orthogonal to K",ClassName());
      x = TMath::Abs(1.0-Ox*(Oy.Cross(K)));
      if (x>eps) gCleanOut->MM(error,met,"System not orthonormal",ClassName());
    }
    phi = 2.0*TMath::Pi()*gRandom3->Rndm();
    s   = TMath::Sin(phi);
    c   = TMath::Cos(phi);
    E   = c*Ox + s*Oy;
  }
  else E.SetXYZ(0.0,0.0,0.0);
}
void TLitGamma::GenFlat() {
  // Generates fTheta and fPhi according to sin(t)dtdf   0<fTheta<fThetaMax
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fThetaMax<vsmall) fTheta = zero;
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = un - x*(un - fApercos);
    fTheta = TLitPhys::ACos(y);
  }
  fPhi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TLitGamma::GenK(TVector3 &K) const {
  // Generate isotropically the direction of the K vector of the photon
  Double_t theta,phi;
  Double_t x,y,z,st,ct,sf,cf;
  x = gRandom3->Rndm();
  y = 1.0 - 2.0*x;
  theta = TLitPhys::ACos(y);
  phi   = 2.0*TMath::Pi()*gRandom3->Rndm();
  st = TMath::Sin(theta);
  ct = TMath::Cos(theta);
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  K.SetXYZ(x,y,z);
}
Bool_t TLitGamma::GenProvided() {
  //  Generates fTheta and fPhi according to provided distribution.
  //    0<fTheta<fThetaMax
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  const Int_t ntrymax   = 1000000;
  Bool_t ok = kTRUE;
  if (fThetaMax<vsmall) fTheta = zero;
  else {
    ok = kFALSE;
    Int_t ntry = 0;
    while ((!ok) && (ntry<ntrymax)) {
      ntry++;
      fTheta = fFitDist->GetRandom();
      if (fTheta<=fThetaMax) ok = kTRUE;
    }
    if (!ok) gCleanOut->MM(fatal,"GenProvided","Problem generating fTheta",ClassName());
  }
  fPhi = deux*TMath::Pi()*gRandom3->Rndm();
  return ok;
}
void TLitGamma::GenSinCos() {
  // Generates fTheta and fPhi according to sin(t)cos(t)dtdf   0<fTheta<fThetaMax
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fThetaMax<vsmall) {
    fTheta = zero;
  }
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = fApersin*TMath::Sqrt(x);
    fTheta = TLitPhys::ASin(y);
  }
  fPhi = deux*TMath::Pi()*gRandom3->Rndm();
}
Bool_t TLitGamma::GenSourceDir() {
  // Generates the direction of the beam particle
  Double_t x,y,z,st,ct,sf,cf;
  Bool_t ok = kTRUE;
  switch (fKindOfEmission) {
  case on4pi:
    Gen4pi();
    break;
  case flat:
    GenFlat();
    break;
  case sinuscosinus:
    GenSinCos();
    break;
  case provided:
    ok = GenProvided();
    break;
  }
  st = TMath::Sin(fTheta);
  ct = TMath::Cos(fTheta);
  sf = TMath::Sin(fPhi);
  cf = TMath::Cos(fPhi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  fSourceDir = x*fAxisDirx + y*fAxisDiry + z*fAxisDirz;
  fSourceDir = fSourceDir.Unit();
  return ok;
}
Bool_t TLitGamma::GenSourcePoint() {
  //  Generates randomly and with equal probability for all positions inside fStartGeoVol
  // a source of emission, except if fSourceFixed is true, in which case takes simply
  // the value already in fSourcePoint.
  Bool_t ok = kTRUE;
  if (!fSourceFixed) {
    const Int_t Nmax = 10000000;
    const char *met = "GenSourcePoint";
    enum KindOfShape {shbox,shtube,sheltu,shother};
    Int_t N;
    Short_t k;
    Double_t dx,dy,dz;
    Double_t x[3];
    Double_t r,phi,r0,r1,r02,r12,xx,a,b;
    Bool_t isin;
    KindOfShape sh;
    TString sn;
    TGeoShape *shape;
    TGeoBBox  *box;
    TGeoTube  *tube;
    TGeoEltu  *eltu;
    shape = fStartGeoVol->GetShape();
    sn    = shape->ClassName();
    if (!sn.CompareTo("TGeoBBox")) sh = shbox;
    else {
      if (!sn.CompareTo("TGeoTube")) sh = shtube;
      else {
        if (!sn.CompareTo("TGeoEltu")) sh = sheltu;
        else                           sh = shother;
      }
    }
    switch (sh) {
      case shbox:
        box = (TGeoBBox*)shape;
        dx = box->GetDX();
        dy = box->GetDY();
        dz = box->GetDZ();
        x[0] = dx*(2*gRandom3->Rndm() - 1.0);
        x[1] = dy*(2*gRandom3->Rndm() - 1.0);
        x[2] = dz*(2*gRandom3->Rndm() - 1.0);
        for (k=0;k<3;k++) fSourcePoint[k] = x[k];
        break;
      case shtube:
        tube = (TGeoTube*)shape;
        r0   = tube->GetRmin();
        r1   = tube->GetRmax();
        dz   = tube->GetDz();
        r02  = r0*r0;
        r12  = r1*r1;
        xx   = gRandom3->Rndm();
        r    = TMath::Sqrt(r02 + (r12-r02)*xx);
        phi  = 2*TMath::Pi()*gRandom3->Rndm();
        fSourcePoint[2] = dz*(2*gRandom3->Rndm() - 1.0);
        fSourcePoint[0] = r*TMath::Cos(phi);
        fSourcePoint[1] = r*TMath::Sin(phi);
        break;
      case sheltu:
        eltu = (TGeoEltu*)shape;
        a    = eltu->GetA();
        b    = eltu->GetB();
        dz   = eltu->GetDz();
        xx   = gRandom3->Rndm();
        r    = TMath::Sqrt(xx);
        phi  = 2*TMath::Pi()*gRandom3->Rndm();
        fSourcePoint[2] = dz*(2*gRandom3->Rndm() - 1.0);
        fSourcePoint[0] = a*r*TMath::Cos(phi);
        fSourcePoint[1] = b*r*TMath::Sin(phi);
        break;
      case shother:
        box = (TGeoBBox*)shape;
        isin = kFALSE;
        dx = box->GetDX();
        dy = box->GetDY();
        dz = box->GetDZ();
        N = 0;
        while ((!isin) && (N<Nmax)) {
          x[0] = dx*(2*gRandom3->Rndm() - 1.0);
          x[1] = dy*(2*gRandom3->Rndm() - 1.0);
          x[2] = dz*(2*gRandom3->Rndm() - 1.0);
          isin = box->Contains(x);
          N++;
        }
        if (!isin) {
          ok = kFALSE;
          gCleanOut->MM(fatal,met,"Impossible to find a point inside TGeoVolume",ClassName());
        }
        else for (k=0;k<3;k++) fSourcePoint[k] = x[k];
        break;
    }//end switch (sh)
  }
  return ok;
}
Double_t TLitGamma::Get_dSigma_dOmega(Double_t theta) const {
  //  Calculates the differenial cross section for a Compton Event
  // Differential Compton X Section for angular distribution
  const Double_t un = 1.0;
  Double_t sig,ctheta,b,g,d1,d2,n2,F1,F2;
  ctheta = TMath::Cos(theta);
  g      = fEIncident/TLitPhys::Get()->Me();
  b      = un - ctheta;
  n2     = g*b;
  d2     = un + n2;
  d1     = d2*d2;
  F1     = un/d1;
  n2    *= n2;
  F2     = un + ctheta*ctheta + n2/d2;
  sig    = 2*TMath::Pi()*F1*F2;
  return sig;
}
Double_t TLitGamma::GetPathLength() {
  //  Get random value for path length, cross-sections for Compton scattering
  // and photo-electric effect having been calculated through call to
  //   - GetXSectnKN()  and GetXSectnPE()
  // Barn conversion: 10E23 from avogadro and 10E24 for cross-section
  const Double_t un   =  1.0;
  const Double_t barn = 10.0;
  const Double_t eps  =  1.0e-20;
  const Double_t big  =  1.0e+20;
  Double_t denom,matN,x;
  Double_t path = big;
  if (fComptonOk || fPhotoElecOk) {
    matN  = (fDensity*TLitPhys::Get()->AvoDivE26())/fA;
    x     = gRandom3->Rndm();
    denom = fXsecCompton + fXsecPhotoEl;
    if (denom>eps) path = -barn*TMath::Log(un - x)/(matN*denom);
  }
  return path;
}
void TLitGamma::GetXSectnKN() {
  //  Calculates in fXsecCompton the integrated Klein-Nishina cross section for
  // incoherent scattering [=Compton scattering], in case Compton effect possible
  // in the current TGeoVolume. Else put fXsecCompton = 0.
  //  The relevance of this formula breaks down at around cut == 0.1 MeV as Thompson
  // scattering becomes more important. But since SLitrani does not handle Thompson
  // scattering, one could decide to go below cut == 0.1 Mev. For that, put fComptonCut
  // to false by a call to TLitGamma::ComptonCut(). Default is true.
  const Double_t cut = 0.1;
  fXsecCompton = 0.0;
  if (fComptonOk) {
    Bool_t activ;
    activ   = (!fComptonCut) || (fEIncident >= cut);
    if (activ) {
      Double_t a,twopRe2,g,sig1,sig2,sig3,sig4,sig5;
      twopRe2 = TMath::TwoPi()*TLitPhys::Get()->R02();
      g       = fEIncident/TLitPhys::Get()->Me();
      sig1    = (1+g)/(g*g);
      sig2    = (2*(1+g)/(1+(2*g)));
      sig3    = TMath::Log(1 + (2*g))/g;
      sig4    = 0.5*sig3;
      a       = 1+2*g;
      sig5    = (1+3*g)/(a*a);
      fXsecCompton     = twopRe2*(sig1*(sig2 - sig3) + sig4 - sig5);
      fXsecCompton    *= TLitPhys::Get()->GetZ(fGeoMedium);
    }
  }
}
void TLitGamma::GetXSectnPE() {
  //  Calculates in fXsecPhotoEl the PhotoElectric Cross Section from TLitMedium,
  // in case PhotoElectric effect possible in the current TGeoVolume. Else put
  // fXsecPhotoEl = 0.
  if (fPhotoElecOk) fXsecPhotoEl = fLitMedium->GetXSectnPE(fEIncident);
  else              fXsecPhotoEl = 0.0;
}
void TLitGamma::IndexGeoVol() {
  // Finds the index of fGeoVolume inside collection TGeoManager::fVolumes
  Bool_t found = kFALSE;
  Int_t k,N;
  TGeoVolume *vol;
  TObjArray *volumes;
  volumes = gGeoManager->GetListOfVolumes();
  N = volumes->GetEntries();
  TIter next(volumes);
  fKGeoVolume = -1;
  k = 0;
  while ((!found) && (k<N) && (vol = (TGeoVolume*)next())) {
    if (fGeoVolume==vol) {
      found = kTRUE;
      fKGeoVolume = k;
    }
    else k++;
  }
  if (fKGeoVolume<0) {
    TString s = "fGeoVolume ";
    s.Append(fGeoVolume->GetName());
    s.Append(" not found in TGeoManager::fVolumes");
    gCleanOut->MM(fatal,"IndexGeoVol",s.Data(),ClassName());
  }
}
void TLitGamma::Init() {
  // Initialization of class variables
  InitP();
  fDrawCode   = 1;
  fComptonCut = kTRUE;
}
void TLitGamma::InitP() {
  // Pointers to 0
  fGeoPhysNode = 0;
  fStartGeoVol = 0;
  fStartLitVol = 0;
  fFitDist     = 0;
  fNode        = 0;
  fGeoVolume   = 0;
  fLitVolume   = 0;
  fGeoMedium   = 0;
  fLitMedium   = 0;
  fPhot        = 0;
  fHX0         = 0;
  fHY0         = 0;
  fHZ0         = 0;
}
void TLitGamma::InitTracking() {
  //  Initialize tracking. Finds pointer to TGeoVolume and pointer to optical material.
  // Verifies that starting volume is ok.
  const char *met = "InitTracking";
  Bool_t IsLitMedium,outside;
  fX    = fWCSSourcePoint;
  fDir  = fWCSSourceDir;
  if (!fNode) gCleanOut->MM(fatal,met,"Gamma created outside world",ClassName());
  else {
    outside = gGeoManager->IsOutside();
    if (outside) gCleanOut->MM(fatal,met,"Start tracking outside geometry",ClassName());
    else {
      fGeoVolume  = fNode->GetVolume();
      if (!fGeoVolume) gCleanOut->MM(fatal,met,"fGeoVolume not found",ClassName());
      IndexGeoVol();
      const char *path = gGeoManager->GetPath();
      fPath = path;
      fLitVolume = (TLitVolume*)fGeoVolume->GetField();
      fGeoMedium = fGeoVolume->GetMedium();
      IsLitMedium = fGeoMedium->InheritsFrom("TLitMedium");
      fPhotoElecOk = kFALSE;
      if (IsLitMedium) {
        fLitMedium = (TLitMedium*)fGeoMedium;
        fPhotoElecOk = fLitMedium->GetPEOk();
      }//end if (IsLitMedium)
      ComptonOk();
    }//end else if (outside)
  }//end else if (!fNode)
}//end InitTracking()
void TLitGamma::InitTV() {
  // Initialise tracking variables
  fRecordedTracks = 0;
  fNphot          = 0;
  fTotEdeposit    = 0.0;
  fNCompton       = 0;
  fGammaDead      = kFALSE;
}
void TLitGamma::IsToDraw(Int_t k) {
  // Decides if gamma nb. k is to be drawn or not
  fToDraw = kFALSE;
  if (fDrawCode) {
    if (fDrawCode>-1000000) {
      if (fDrawCode>0) fToDraw = (k ==  fDrawCode);
      else             fToDraw = (k <= -fDrawCode);
    }
  }
}
void TLitGamma::MoveCradle(TGeoMatrix *M,Bool_t todraw) {
  // Moves the photon cradle at the new position M
  if (fMoving) {
    fGeoPhysNode->Align(M,0,fCheckMove);
    fStartGeoVol = fGeoPhysNode->GetVolume();
    if (fStartLitVol) fStartLitVol->SetGeoVolume(fStartGeoVol);
    if (todraw) gGeoManager->GetTopVolume()->Draw("");
  }
  else gCleanOut->MM(error,"MoveCradle","Cradle has not been announced as moving",ClassName());
}
void TLitGamma::NameFile() {
  //  Give name and title to the histogram file for one run.
  // The name of the statistics and histograms file for a run is name ( the
  // name you have given in the constructor ) appended with '_'
  // followed by the run number and '.root'.
  //
  fFullName  = fName;
  fFullTitle = fTitle;
  TString srun = "";
  if (fRun<10000) srun.Append('0');
  if (fRun<1000) srun.Append('0');
  if (fRun<100) srun.Append('0');
  if (fRun<10) srun.Append('0');
  srun += fRun;
  fFullName.Append('_');
  fFullName.Append(srun);
  fFullTitle.Append(" run ");
  fFullTitle.Append(srun);
}
void TLitGamma::NameFromPath(TString &s) const {
  // Returns name of TGeoVolume of beam cradle using fSourcePath
  Int_t k,N;
  Bool_t found = kFALSE;
  s = "";
  N = fSourcePath.Length();
  k = N-1;
  while ((k>=0) && (!found)) {
    if (fSourcePath[k]=='/') found = kTRUE;
    else k--;
  }
  if (!found) gCleanOut->MM(fatal,"NameFromPath","Bad path name",ClassName());
  k++;
  while ((k<N) && (fSourcePath[k]!='_')) {
    s.Append(fSourcePath[k]);
    k++;
  }
}
Bool_t TLitGamma::NextCrossing() {
  //  Go until next TGeoVolume. Returns true if inside geometry, false if exiting
  // from the geometry.
  Double_t step;
  const Double_t *point;
  const Double_t *dir;
  const char     *path;
  Bool_t outside = kFALSE;
  Bool_t IsLitMedium;
  fNextNode = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
  if (!fNextNode) outside = kTRUE;
  else {
    outside = gGeoManager->IsOutside();
    if (!outside) {
      step   = gGeoManager->GetStep();
      fRoad += step;
      point  = gGeoManager->GetCurrentPoint();
      dir    = gGeoManager->GetCurrentDirection();
      fX.SetXYZ(point[0],point[1],point[2]);
      fDir.SetXYZ(dir[0],dir[1],dir[2]);
      fTime      += step/GetSpeed();
      if (fToDraw) fCurrentTrack->AddPoint(fX[0],fX[1],fX[2],fTime);
      fNode       = fNextNode;
      fGeoVolume  = fNode->GetVolume();
      if (!fGeoVolume) gCleanOut->MM(fatal,"NextCrossing","fGeoVolume not found",ClassName());
      IndexGeoVol();
      path        = gGeoManager->GetPath();
      fPath       = path;
      fLitVolume  = (TLitVolume*)fGeoVolume->GetField();
      fGeoMedium  = fGeoVolume->GetMedium();
      IsLitMedium = fGeoMedium->InheritsFrom("TLitMedium");
      fPhotoElecOk = kFALSE;
      if (IsLitMedium) {
        fLitMedium = (TLitMedium*)fGeoMedium;
        fPhotoElecOk = fLitMedium->GetPEOk();
      }//end if (IsLitMedium)
      ComptonOk();
    }
  }
  return !outside;
}
Bool_t TLitGamma::SetEmission(KindOfDist kind,Double_t tmax,TVector3 dir,
  const char *fitname,Bool_t srcfix,TVector3 source,Bool_t emface,TVector3 dirfce) {
  //  Declares that the TGeoVolume fStartGeoVol is able to emit gammas. All points
  // and vectors of this method are given in the local coordinate system of fStartGeoVol.
  //  There are 3 possibilities for the point of emission of gammas:
  //
  //  (1) -  gammas are emitted from any point inside fStartGeoVol with an equal
  //         probability
  //  (2) -  [if srcfix true]: all particles are emitted from point "source" inside
  //         the TGeoVolume
  //  (3) -  [if emface true and srcfix false]: first, a point is generated inside
  //         fStartGeoVol with an equal probability. Then the point is translated
  //         inside fStartGeoVol, along direction dirfce, until the edge of
  //         fStartGeoVol. It is the way SLitrani generates surface emission, without
  //         having [as the old SLitrani] any handling of faces. This method has the
  //         advantage of working, whatever the kind of TGeoVolume.
  //         
  //  Arguments:
  //
  // kind   : type of distribution for the direction of the emitted particles around dir
  //          [default flat]
  //
  //        on4pi        ==> particles are emitted isotropically, with the distribution
  //                      sin(theta)*dtheta*dphi, on 4 pi.
  //        flat         ==> particles are emitted isotropically, with the distribution
  //                      sin(theta)*dtheta*dphi,with theta between 0 and tmax
  //        sinuscosinus ==> particles are emitted with the non isotropic distribution
  //                      sin(theta)*cos(theta)*dtheta*dphi ,with theta between 0 and
  //                      tmax, favouring slightly the forward direction
  //        provided     ==> particles are emitted using the distribution provided
  //                      as a fit of type TSplineFit of name fitname
  //
  // tmax   : maximum value for the theta angle around dir. tmax is given in degree
  //          [default 1°]
  // dir    : axis around which particles are emitted, in local coordinate system of
  //          TGeoVolume [default (0,1,0)
  // fitname: in case kind == provided, name of fit to be used
  //          [default ""]
  // srcfix : if true, all particles are emitted from the fixed point source [ given in
  //          local coordinate system of TGeoVolume ] inside TGeoVolume
  //          [default false]
  // source : if srcfix true, fixed point in LCS within the TGeoVolume from which all
  //          particles are emitted. if srcfix false, irrelevant. Default (0,0,0)
  // emface : if true and srcfix false, particles are emitted from a face of the TGeoVolume
  //          according to possibility (3) described above. [Default true].
  // dirfce : after having generated with equal probability a point anywhere inside the
  //          TGeoVolume, the point is translated along direction "dirfce" until the
  //          edge of the TGeoVolume. [Default (0,1,0)]
  //
  const Double_t eps   = 1.0e-20;
  const Double_t pideg = 180.0;
  const char *met = "SetEmission";
  Bool_t ok = kTRUE;
  Short_t k;
  Double_t L,x,y;
  fKindOfEmission = kind;
  fFromFace = kFALSE;
  tmax = TMath::Abs(tmax);
  if (tmax > pideg) tmax = pideg;
  fThetaMax = (tmax*TMath::Pi())/pideg;
  fApersin  = TMath::Sin(fThetaMax);
  fApercos  = TMath::Cos(fThetaMax);
  L = dir.Mag2();
  if (L<eps) {
    dir[0] = 0.0;
    dir[1] = 1.0;
    dir[2] = 0.0;
  }
  fAxisDirz = dir;
  fAxisDirz = fAxisDirz.Unit();
  fAxisDirx.SetXYZ(1.0,0.0,0.0);
  fAxisDiry.SetXYZ(0.0,1.0,0.0);
  x = TMath::Abs(fAxisDirx*fAxisDirz);
  y = TMath::Abs(fAxisDiry*fAxisDirz);
  if (x<=y) {
    fAxisDiry = fAxisDirz.Cross(fAxisDirx);
    fAxisDiry = fAxisDiry.Unit();
    fAxisDirx = fAxisDiry.Cross(fAxisDirz);
    fAxisDirx = fAxisDirx.Unit();
  }
  else {
    fAxisDirx = fAxisDirz.Cross(fAxisDiry);
    fAxisDirx = fAxisDirx.Unit();
    fAxisDiry = fAxisDirz.Cross(fAxisDirx);
    fAxisDiry = fAxisDiry.Unit();
  }
  if (TLit::Get()->GetTestCalc()) {
    const Double_t eps = 1.0e-12;
    x = TMath::Abs(fAxisDirx*fAxisDiry);
    if (x>eps) gCleanOut->MM(error,met,"fAxisDirx not orthogonal to fAxisDiry",ClassName());
    x = TMath::Abs(fAxisDirx*fAxisDirz);
    if (x>eps) gCleanOut->MM(error,met,"fAxisDirx not orthogonal to fAxisDirz",ClassName());
    x = TMath::Abs(fAxisDiry*fAxisDirz);
    if (x>eps) gCleanOut->MM(error,met,"fAxisDiry not orthogonal to fAxisDirz",ClassName());
    x = TMath::Abs(1.0-fAxisDirx*(fAxisDiry.Cross(fAxisDirz)));
    if (x>eps) gCleanOut->MM(error,met,"System not orthonormal",ClassName());
  }
  L = dirfce.Mag2();
  if (L<eps) {
    dirfce[0] = 0.0;
    dirfce[1] = 1.0;
    dirfce[2] = 0.0;
  }
  fDirFace = dirfce;
  fDirFace = fDirFace.Unit();
  fDistName = fitname;
  fFitDist = 0;
  TSplineFit *fit = 0;
  Int_t NL = fDistName.Length();
  if (NL>5) {
    fit = TSplineFit::FindFit(fitname,12);
    if (!fit) gCleanOut->MM(error,met,"Fit not found",ClassName());
  }
  if (fit) {
    if (fKindOfEmission == provided) fFitDist = fit;
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit provided but kind of emission != provided",ClassName());
    }
  }
  else {
    if (fKindOfEmission == provided) {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Kind of emission == provided, but fit missing",ClassName());
      gCleanOut->MM(error,met,"We change kind of emission to flat !",ClassName());
      fKindOfEmission = flat;
    }
  }
  fSourceFixed = srcfix;
  if (fSourceFixed) {
    Double_t x[3];
    fSourcePoint = source;
    for (k=0;k<3;k++) x[k] = fSourcePoint[k];
    if (!fGeoVolume->Contains(x)) {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Given fixed point is not inside TGeoVolume",ClassName());
      gCleanOut->MM(error,met,"We switch to no fix point generation",ClassName());
      fSourceFixed = kFALSE;
    }
  }//end if (fSourceFixed)
  else {
    if (emface) fFromFace = kTRUE;
  }//end else if (fSourceFixed)
  return ok;
}
void TLitGamma::SetFillDeath(TH1F *h1,TH1F *h2,TH1F *h3) {
  //  To be called once in order that the coordinates of the photons seen
  // be recorded into the provided histograms : 
  //  - h1 for the x World Coordinate
  //  - h2 for the y World Coordinate
  //  - h3 for the z World Coordinate
  //  The creation and deletion of the histograms h1/2/3 has to be handled
  // by the user inside his CINT code. h1/2/3 must have been booked before
  // calling SetFillDeath
  fFillDeath = kTRUE;
  fHX0 = h1;
  fHY0 = h2;
  fHZ0 = h3;
}
Bool_t TLitGamma::StartOneGamma() {
  //  Generates a gamma conform to all characteristics provided in the cons-
  // tructor and in the method SetEmission().
  //  Generates momentum, calculates speed and energy, generates source point
  // and direction, calculates 3 components of momentum.
  Bool_t ok = kTRUE;
  InitTV();
  GenP();
  ok = GenSourcePoint();
  if (ok) {
    ok = GenSourceDir();
    if (ok) {
      ok = EmissionToWCS();
      if (ok) {
        fTime        = 0;
        fRoad        = 0;
        SetLorentzE(fWCSSourceDir.X(),fWCSSourceDir.Y(),fWCSSourceDir.Z(),GetMomentum());
        InitTracking();
      }
    }
  }
  if (!ok) gCleanOut->MM(fatal,"StartOneParticle","Method failed",ClassName());
  return ok;
}
void TLitGamma::WhichToDraw(Int_t N) {
  //
  // N == 0                ==> no gamma to be drawn
  // N  > 0                ==> gamma number N is to be drawn
  // N >= 1000000          ==> all gammas are to be drawn
  // N == -k               ==> gammas 1 to k have to be drawn
  // N <= -1000000         ==> no gamma to be drawn
  //
  fDrawCode = N;
}
