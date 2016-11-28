// @(#)root/html:$Id: TLitEqIndex.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TMath.h"
#include "TwoPadDisplay.h"
#include "TLit.h"
#include "TLitPol4.h"
#include "TLitEqIndex.h"

TLitEqIndex *gLitIdx = 0;

ClassImp(TLitEqIndex)
//______________________________________________________________________________
/* BEGIN_HTML
		<div align="center">
			<h2>TLitEqIndex</h2>
		</div>
		<p><b><code>TLitEqIndex</code></b> handles the equation :</p>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td><b><code><font size="+1">z<sup>2</sup>  + a*z + (b*z+c)*Sqrt(z-g) + d = 
0</font></code></b></td>
					<td><b><code>(E1)</code></b></td>
				</tr>
			</table>
		</div>
		<p>where <b><code>a, b, c, d, g</code></b> are always real, but the 4 solutions 
<b><code>z</code></b> may be complex. This is the equation which gives the 2 indices 
in case of anisotropy. It has the strange property of having 4 (not 2 !) solutions. 
Only 2 of the 4 solutions correspond to a wave having a Poyinting vector going in 
the right direction. But we cannot know in advance which 2 among the 4 are the right 
ones! We have to go until the calculation of the Poyinting vector to know which 2 
solutions have to be kept!</p>
		<p>Equation (E1) can be rewritten:</p>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td>
						<div align="center">
							<b><code><font size="+1">z<sup>2</sup> + a*z + d = - (b*z+c)*Sqrt(z-g)</font></code></b></div>
					</td>
					<td>
						<div align="center">
							<b><code>(E1)</code></b></div>
					</td>
				</tr>
				<tr>
					<td colspan="2">let us define:</td>
				</tr>
				<tr>
					<td colspan="2">
						<div align="center">
							<b><code><font face="Symbol" size="+1">D</font><font size="+1"> = Sqrt(a<sup>2</sup>-4d)</font></code></b></div>
					</td>
				</tr>
				<tr>
					<td><code><b><font size="+1">(z-0.5*[-a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">])</font></b></code><code><b><font size="+1">(z+0.5*[a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">]) 
= -b(z+c/b)*Sqrt(z-g)</font></b></code></td>
					<td><b><code>(E1)</code></b></td>
				</tr>
			</table>
		</div>
		<p></p>
		<p>It can happen (and indeed it really happens in case of birefringence and not 
full anisotropy) that either:</p>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td>(1)</td>
					<td><code><font size="+1"><b>0.5*[-a+</b></font></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><font size="+1"><b>] 
= -c/b</b></font></code></td>
					<td>or</td>
				</tr>
				<tr>
					<td>(2)</td>
					<td>-<code><font size="+1"><b>0.5*[+a+</b></font></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><font size="+1"><b>] 
= -c/b</b></font></code></td>
					<td></td>
				</tr>
			</table>
		</div>
		<p>In that case, we speak of degeneracy. Let us study first the 2 cases of degeneracy:</p>
		<p></p>
		<p><font size="+1"><code><b>(1) Degeneracy with 0.5*[-a+</b></code></font><b><code><font face="Symbol" size="+1">D</font></code></b><font size="+1"><code><b>] 
= -c/b</b></code></font></p>
		<p>This gives a first root:</p>
		<p></p>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td>
						<p><code><b><font size="+1">z<sub>0</sub> = -c/b = 0.5*[-a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">]</font></b></code></p>
					</td>
					<td><b><code>(E2)</code></b></td>
				</tr>
				<tr>
					<td colspan="2">let us define (using also (E2):</td>
				</tr>
				<tr>
					<td><b><code><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub> 
=  -0.5*[a+</font><font face="Symbol" size="+1">D</font><font size="+1">] = -a + 
c/b</font></code></b></td>
					<td><b><code>(E3)</code></b></td>
				</tr>
			</table>
			<p></p>
		</div>
		<div align="left">
			<p>It leaves us with the equation:</p>
			<p></p>
		</div>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td colspan="2"><code><b><font size="+1">z+0.5*[a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">] 
= -b*Sqrt(z-g)</font></b></code></td>
				</tr>
				<tr>
					<td colspan="2">using (E2) to replace <b><code><font face="Symbol" size="+1">D</font></code></b>:</td>
				</tr>
				<tr>
					<td colspan="2"><code><b><font size="+1">z+0.5*[2a-2c/b</font></b></code><code><b><font size="+1">] 
= -b*Sqrt(z-g)</font></b></code></td>
				</tr>
				<tr>
					<td>
						<div align="center">
							<code><b><font size="+1">z+a-c/b</font></b></code><code><b><font size="+1"> 
= -b*Sqrt(z-g)</font></b></code></div>
					</td>
					<td><code><b>(E4)</b></code></td>
				</tr>
				<tr>
					<td>
						<div align="center">
							<code><b><font size="+1">z-</font></b></code><b><code><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub></font></code></b><code><b><font size="+1"> 
= -b*Sqrt(z-g)</font></b></code></div>
					</td>
					<td><code><b>(E4)</b></code></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p></p>
			<p></p>
			<p><font size="+1"><code><b>(2) Degeneracy with -0.5*[a+</b></code></font><b><code><font face="Symbol" size="+1">D</font></code></b><font size="+1"><code><b>] 
= -c/b</b></code></font></p>
			<p>This gives a first root:</p>
			<p></p>
		</div>
		<div align="center">
			<div align="center">
				<table border="4" cellpadding="6" cellspacing="2">
					<tr>
						<td>
							<p><code><b><font size="+1">z<sub>0</sub> = -c/b = -0.5*[+a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">]</font></b></code></p>
						</td>
						<td><b><code>(E4)</code></b></td>
					</tr>
					<tr>
						<td colspan="2">let us define (using also (E4):</td>
					</tr>
					<tr>
						<td><b><code><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub> 
= +</font></code></b><code><b><font size="+1">0.5*[-a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">]</font></b></code><b><code><font size="+1"> 
= -a + c/b</font></code></b></td>
						<td><b><code>(E5)==(E3)</code></b></td>
					</tr>
				</table>
			</div>
		</div>
		<div align="left">
			<div align="center">
				<p></p>
			</div>
			<div align="left">
				<p>It leaves us with the equation:</p>
				<p></p>
			</div>
		</div>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td colspan="2"><code><b><font size="+1">z-0.5*[-a+</font></b></code><b><code><font face="Symbol" size="+1">D</font></code></b><code><b><font size="+1">] 
= -b*Sqrt(z-g)</font></b></code></td>
				</tr>
				<tr>
					<td colspan="2">using (E4):</td>
				</tr>
				<tr>
					<td colspan="2"><code><b><font size="+1">z-0.5*[-2a+2c/b</font></b></code><code><b><font size="+1">] 
= -b*Sqrt(z-g)</font></b></code></td>
				</tr>
				<tr>
					<td><code><b><font size="+1">z+a-c/b</font></b></code><code><b><font size="+1"> 
= -b*Sqrt(z-g)</font></b></code></td>
					<td><code><b>(E6)==(E4)</b></code></td>
				</tr>
				<tr>
					<td>
						<div align="center">
							<code><b><font size="+1">z-</font></b></code><b><code><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub></font></code></b><code><b><font size="+1"> 
= -b*Sqrt(z-g)</font></b></code></div>
					</td>
					<td><code><b>(E6)==(E4)</b></code></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p><b><i>The 2 cases of degeneracy end with the same equation. They do not need 
to be distinguished.</i></b> Squaring (E4) == (E6), we get:</p>
			<p></p>
		</div>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td><b><code><font size="+1">z<sup>2</sup> -2</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>z 
+ </font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub><sup>2</sup> 
= b<sup>2</sup>z - b<sup>2</sup>g</font></code></b></td>
					<td></td>
				</tr>
				<tr>
					<td><b><code><font size="+1">z<sup>2</sup> - (b</font></code><code><font size="+1"><sup>2</sup></font></code></b><code><b>+</b></code><b><code><font size="+1">2</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>)z 
+ </font><font size="+1"></font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub><sup>2 
</sup>+ b<sup>2</sup>g = 0</font></code></b></td>
					<td></td>
				</tr>
				<tr>
					<td>let us define:</td>
					<td></td>
				</tr>
				<tr>
					<td><code><b><font face="Symbol" size="+1">G</font><font size="+1"> = (b<sup>2</sup>+2</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>)<sup>2</sup> 
- 4(</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub><sup>2 
</sup>+ b<sup>2</sup>g)</font></b></code></td>
					<td></td>
				</tr>
				<tr>
					<td><code><b><font face="Symbol" size="+1">G</font><font size="+1"> = b<sup>4</sup>+4b<sup>2</sup></font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>+4</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub><sup>2</sup> 
- 4</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub><sup>2 
-</sup> 4b<sup>2</sup>g)</font></b></code></td>
					<td></td>
				</tr>
				<tr>
					<td><code><b><font face="Symbol" size="+1">G</font><font size="+1"> = b<sup>2</sup>(b<sup>2</sup>+4(</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub></font><font size="+1"><sup> 
-</sup> g))</font></b></code></td>
					<td></td>
				</tr>
				<tr>
					<td>then we get 2 more roots:</td>
					<td></td>
				</tr>
				<tr>
					<td><b><code><font size="+1">z<sub>1</sub> = (b<sup>2</sup>+2</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>+Sqrt(</font><font face="Symbol" size="+1">G</font><font size="+1">))/2</font></code></b></td>
					<td><b><code>(E7)</code></b></td>
				</tr>
				<tr>
					<td><b><code><font size="+1">z<sub>2</sub> = (b<sup>2</sup>+2</font><font face="Symbol" size="+1">z</font><font size="+1"><sub>1</sub>-Sqrt(</font><font face="Symbol" size="+1">G</font><font size="+1">))/2</font></code></b></td>
					<td><b><code>(E8)</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p></p>
			<p>Two remarks: </p>
			<ol>
				<li>Since we have squared equation (E4)==(E6), we have lost the information about 
the sign of <b><code>Sqrt(z-g)</code></b> associated with solutions <b><code><font size="+1">z<sub>1</sub></font></code></b> 
and <b><code><font size="+1">z<sub>2</sub></font></code></b>. We have to put <b><code><font size="+1">z<sub>1</sub></font></code></b> 
and <b><code><font size="+1">z<sub>2</sub></font></code></b> into equation (E4) and 
look which sign is the good one.,
				<li>On the first look, it appears that there are only 3 solutions: <b><code><font size="+1">z<sub>0</sub></font></code></b> 
, <b><code><font size="+1">z<sub>1</sub></font></code></b> and <b><code><font size="+1">z<sub>2</sub></font></code></b>. 
But in fact, <b><code><font size="+1">z<sub>0</sub></font></code></b> can at will 
be linked with +<b><code>Sqrt(z-g)</code></b> or -<b><code>Sqrt(z-g)</code></b>. 
So we will consider that we have 4 solutions: <code><b><font size="+1">z<sub>0</sub></font></b></code> 
linked with +<b><code>Sqrt(z-g)</code></b>, <code><b><font size="+1">z<sub>0</sub></font></b></code> 
linked with -<b><code>Sqrt(z-g)</code></b>, <b><code><font size="+1">z<sub>1</sub></font></code></b> 
and <b><code><font size="+1">z<sub>2</sub></font></code></b> with the right sign 
of <b><code>Sqrt(z-g)</code></b>.			</ol>
			<p></p>
			<p></p>
			<h3>Normal case, no degeneracy.</h3>
			<p>In this case, we begin by squaring equation (E1):</p>
			<p></p>
		</div>
		<div align="center">
			<table border="4" cellpadding="6" cellspacing="2">
				<tr>
					<td><b><code><font size="+1">z<sup>2</sup> + a*z + d = - (b*z+c)*Sqrt(z-g)</font></code></b></td>
					<td><b><code>(E1)</code></b></td>
				</tr>
				<tr>
					<td colspan="2">let us define:</td>
				</tr>
				<tr>
					<td colspan="2"><code><b><font size="+1">A = 2a-b<sup>2</sup></font></b></code></td>
				</tr>
				<tr>
					<td colspan="2"><b><code><font size="+1">B = a<sup>2</sup>+2d-2bc+b<sup>2</sup>g</font></code></b></td>
				</tr>
				<tr>
					<td colspan="2"><b><code><font size="+1">C = 2ad-c<sup>2</sup>+2bcg</font></code></b></td>
				</tr>
				<tr>
					<td colspan="2"><b><code><font size="+1">D = d<sup>2</sup>+c<sup>2</sup>g</font></code></b></td>
				</tr>
				<tr>
					<td colspan="2">we have then:</td>
				</tr>
				<tr>
					<td>
						<div align="center">
							<b><code><font size="+1">z<sup>4</sup>+Az<sup>3</sup>+Bz<sup>2</sup>+Cz+D 
= 0</font></code></b></div>
					</td>
					<td><b><code>(E9)</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p></p>
			<p>(E9) is solved using <b><code><a href="TLitPol4">TLitPol4</a></code></b>.&nbsp;The 
4 solutions found are then entered into (E1) to determine the sign of <b><code>Sqrt(z-g)</code></b>. 
<b><code><a href="TLitPol4">TLitPol4</a></code></b> has 2 methods to find the roots 
of the 4th order polynomial.&nbsp;One method starting with the analytical solution 
given by <b><code><a href="http://root.cern.ch/root/html/ROOT__Math__Polynomial.html">ROOT::Math::Polynomial</a></code></b>, 
<b><code><a href="TLitPol4.html#TLitPol4:FindRootsRMP">TLitPol4::FindRootsRMP()</a></code></b>, 
and another method, <b><code><a href="TLitPol4.html#TLitPol4:FindRoots">TLitPol4::FindRoots()</a></code></b>, 
not using <b><code><a href="http://root.cern.ch/root/html/ROOT__Math__Polynomial.html">ROOT::Math::Polynomial</a></code></b>, 
but starting with various Newton iteration methods. The default is <b><code><a href="TLitPol4.html#TLitPol4:FindRootsRMP">TLitPol4::FindRootsRMP()</a></code></b>. 
 You can change the default from within your CINT code with:</p>
		</div>
		<div align="center">
			<p><b><code>==&gt; gLitIdx-&gt;SetWithRMP(kFALSE);</code></b></p>
		</div>
		<div align="left">
			<p><b><code>gLitIdx</code></b> is a global pointer pointing towards the unique 
instance of <b><code>TLitEqIndex</code></b> used inside <b><code>SLitrani</code></b>.</p>
			<p><b><code>TLitEqIndex</code></b>, <b><code><a href="TLitPol4">TLitPol4</a></code></b> 
and <b><code><a href="TLitPol3">TLitPol3</a></code></b> can be used independently 
of <b><code>SLitrani</code></b>. <b><code>TLitEqIndex</code></b> derives from <b><code><a href="http://root.cern.ch/root/html/TObject.html">TObject</a></code></b>.&nbsp;So 
it can be stored into a <b><code><a href="http://root.cern.ch/drupal/">ROOT</a></code></b> 
collection or saved into a <b><code><a href="http://root.cern.ch/drupal/">ROOT</a></code></b> 
file.&nbsp;In the later case, notice that only the class variables defining the equation 
are stored into the <b><code><a href="http://root.cern.ch/drupal/">ROOT</a></code></b> 
file. The class variables containing the roots of the equation are not.&nbsp;You 
have to retrieve them using <b><code><a href="#TLitEqIndex:Solve">TLitEqIndex::Solve()</a></code></b>. 
There are various <b><code>Get...</code></b> methods to get all the informations 
concerning the roots of the equation.&nbsp;In particular, <b><code><a href="#TlitEqIndex:GetfZ">TLitEqIndex::GetfZ()</a></code></b> 
returns the 4 roots. If P is a pointer towards an instance of TLitEqIndex, the value 
of the equation at z with + or - sign of <b><code>Sqrt(z-g)</code></b> can be obtained 
by:</p>
			<p>    <b><code>==&gt; y = P-&gt;Y(z,kTRUE)  or simpler by y = P(z,kTRUE);</code></b></p>
			<p>    <b><code>==&gt; y = P-&gt;Y(z,kFALSE) or simpler by y = P(z,kFALSE);</code></b></p>
		</div>
END_HTML */
//
TLitEqIndex::TLitEqIndex(Bool_t hres,Bool_t DefinegIdx) {
  // Constructor not defining a,b,c,d,g. You have to call Set() later on
  //  - hres [default false] has to be set true if one wants to histogram the
  //     log of the total error fResidue
  //  - DefinegIdx [default false] If true, the global pointer TLitEqIndex::gLitIdx
  //     is first checked. If non-zero, it is deleted and then set equal to this
  InitP();
  if (hres) BookHRes();
  if (DefinegIdx) {
    if (gLitIdx) delete gLitIdx;
    gLitIdx = this;
  }
}
TLitEqIndex::TLitEqIndex(Int_t i) {
  // "Debug" constructor handling a collection of difficult cases.
  // i selects the case to handle. You have to call Solve() later on.
  // 0 <= i <= 16. The global pointer TLitEqIndex::gLitIdx is not set by this
  //constructor
  //
  // For instance, you can do:
  //
  //  ==> gLitIdx = new TLitEqIndex(14)
  //  ==> gLitIdx->SetDebug();
  //  ==> gLitIdx->Solve();
  //  ==> gLitIdx->ShowEqu();
  //
  InitP();
  i  = TMath::Abs(i);
  i %= 17;
  switch (i) {
    case 0:
      a = -10.3504799049;
      b = 0;
      c = 0;
      d = 26.7831085656;
      g = 4.94963995708e-032;
      //gd : 1.59940959851e-065
      break;
    case 1:
      a =  -8.178867598;
      b =  0.100537665;
      c =  -0.410329585;
      d =  16.72340342;
      g =  4.056168105;
      break;
    case 2:
      a =  -9.92656197;
      b =  -0.050933878;
      c =  0.236894902;
      d =  24.53312898;
      g =  5.642503553;
      break;
    case 3:
      a = -8.181866693;
      b = 0.100398773;
      c = -0.409762717;
      d = 16.73564384;
      g = 4.045059063;
      break;
    case 4:
      a = -8.936733016;
      b = -0.123588686;
      c = 0.568464495;
      d = 19.94906434;
      g = 2.616004553;
      break;
    case 5:
      a = -9.142767119;
      b = 0.171529796;
      c = -0.923607897;
      d = 20.66146166;
      g = 5.05766914;
      break;
    case 6:
      a = -8.55266665;
      b = 0.473426746;
      c = -1.857679281;
      d = 18.16281757;
      g = 4.684890774;
      break;
    case 7:
      a = -10.15001701;
      b = 0.181357029;
      c = -0.927470126;
      d = 25.75418659;
      g = 4.928623823;
      break;
    case 8:
      a = -10.08767843;
      b = -0.006808947;
      c = 0.031900746;
      d = 25.31163552;
      g = 5.538174615;
      break;
    case 9:
      a = -7.903871398;
      b = 0.007578013;
      c = -0.029455271;
      d = 15.61357111;
      g = 4.01693301;
      break;
    case 10:
      a = -8.259262806;
      b = -0.113456731;
      c = 0.463056825;
      d = 17.05152456;
      g = 4.1787326;
      break;
    case 11:
      a = -9.9455033681907;
      b = 0.13395666428851;
      c = -0.67481069492000;
      d = 24.725064307763;
      g = 4.9164625172000;
      break;
    case 12:
      a = -9.9909361989730;
      b = 0.086507298251473;
      c = -0.43534503197112;
      d = 24.954074234765;
      g = 4.9723218636147;
      break;
    case 13:
      a = -10.072670538675;
      b = -6.115646185501e-7;
      c = 3.2117853133233e-6;
      d = 25.318268855649;
      g = 5.1734532854320;
      break;
    case 14:
      a = -9.5952649378212;
      b = -3.0347345970167e-7;
      c = 1.5001682392998e-6;
      d = 22.996050705858;
      g = 4.6631865442249;
      break;
    case 15:
      a = -9.4980772729929;
      b = 0.073830803411596;
      c = -0.35073079100128;
      d = 22.553111378568;
      g = 2.4256796562818;
      break;
    case 16:
      a = -10.529263729689;
      b = 0.11156676113325;
      c = -0.59571593153035;
      d = 27.714501928828;
      g = 5.2281248129843;
      break;
  }
  Init();
}
TLitEqIndex::TLitEqIndex(Double_t a0,Double_t b0,Double_t c0,Double_t d0,Double_t g0,
  Double_t gd,Bool_t hres) {
  // Constructor with full initialization of equation.
  // gd only concerns the case where b==c==0, fTypeEq == bczero.  
  // gd is the determinant of the second order equation remaining:
  // if you do not provide it, it will be correctly calculated as:
  //    gd = a*a - 4*d
  // It is also the value provided by SLitrani in case of isotropy
  // In case of anisotropy, [and of b==c==0] there is a better way of 
  //calculating gd. This better way is used by SLitrani in that specific
  //case. It is why we leave the possibility to provide it. But the normal
  //case is to not provide it!
  // The global pointer TLitEqIndex::gLitIdx is set to "this" by this constructor
  //It is first deleted if found non-zero
  InitP();
  a=a0; b=b0;c=c0;d=d0;g=g0;
  fGd = gd;
  Init();
  if (hres) BookHRes();
  if (gLitIdx) delete gLitIdx;
  gLitIdx = this;
}
TLitEqIndex::~TLitEqIndex() {
  // Destructor
  if (fGraphP)    delete fGraphP;
  if (fGraphM)    delete fGraphM;
  if (fHRes)      delete fHRes;
}
Bool_t TLitEqIndex::BetterConj(TComplex &z1,TComplex &z2) {
  // Make z1 and z2 better complex conjugate if necessary and returns true
  // If not necessary, does nothing and returns false
  const Double_t eps = 1.0e-12;
  Bool_t ok = kFALSE;
  Double_t a,b;
  a = TMath::Abs(z1.Re() - z2.Re());
  b = TMath::Abs(z1.Im() + z2.Im());
  if ((a+b)>eps) {
    ok = kTRUE;
    a = (z1.Re() + z2.Re())/2.0;
    b = TMath::Abs((z1.Im() - z2.Im()))/2.0;
    z2(a,b);
    z1(a,-b);
  }
  return ok;
}
void TLitEqIndex::BookHRes() {
  // Book histogram fHRes, histogram of Log of the total error at the exit
  //of Solve()
  const Int_t nbin    =  100;
  const Double_t xlow = -18.0;
  const Double_t xup  = -2.0;
  fHRes = new TH1D("Residues","Log10 Total error at exit of Solve()",nbin,xlow,xup);
}
Short_t TLitEqIndex::ClosestFrom(Double_t ni,Bool_t bi) {
  //  Find which among the 4 solution is closest to ni, ni being real. So we look
  //only among the real solutions. If there is no real solution, returns -1.
  //The reason of this routine is that when calculating the transition from
  //anisotropic to isotropic medium, the index of refraction of the incident
  //wave is found among the 4 solutions. But this solution must be avoided !
  const Double_t eps = 1.0e-6;
  Short_t k = -1;
  Double_t ni2,d2;
  Double_t d1 = -1.0;
  Double_t x0,x1,x2,x3;
  ni2 = ni*ni;
  x0  = fZc[0].Re();
  x1  = fZc[1].Re();
  x2  = fZc[2].Re();
  x3  = fZc[3].Re();
  if (!fRc[0]) {
    d1 = TMath::Abs(x0 - ni2);
    k  = 0;
    if (fBc[0]==bi) d1 += eps;
  }
  if (!fRc[1]) {
    d2 = TMath::Abs(x1 - ni2);
    if (d2<d1) {
      d1 = d2;
      k  = 1;
      if (fBc[1]==bi) d1 += eps;
    }
  }
  if (!fRc[2]) {
    d2 = TMath::Abs(x2 - ni2);
    if (d2<d1) {
      d1 = d2;
      k  = 2;
      if (fBc[2]==bi) d1 += eps;
    }
  }
  if (!fRc[3]) {
    d2 = TMath::Abs(x3 - ni2);
    if (d2<d1) k = 3;
  }
  return k;
}
Bool_t TLitEqIndex::Disposition() {
  // Checks disposition of roots [real/complex]
  Bool_t dispos;
  dispos = (!fRc[0]) && (!fRc[1]) && (!fRc[2]) && (!fRc[3]);
  dispos = dispos || ((!fRc[0]) && (!fRc[1]) && fRc[2] && fRc[3]);
  dispos = dispos || (fRc[0] && fRc[1] && fRc[2] && fRc[3]);
  if (!dispos) {
    gCleanOut->MM(error,"Disposition","Bad disposition of roots",ClassName());
  }
  return dispos;
}
void TLitEqIndex::Exchange(Short_t i, Short_t j) {
  //Exchange solution i with solution j
  TComplex z;
  Double_t s;
  Bool_t b,r;
  z       = fZc[j];
  fZc[j]  = fZc[i];
  fZc[i]  = z;
  b       = fBc[j];
  fBc[j]  = fBc[i];
  fBc[i]  = b;
  r       = fRc[j];
  fRc[j]  = fRc[i];
  fRc[i]  = r;
  s       = fQsc[j];
  fQsc[j] = fQsc[i];
  fQsc[i] = s;
  s       = fQs2[j];
  fQs2[j] = fQs2[i];
  fQs2[i] = s;
}
void TLitEqIndex::GetCoefficients(Double_t &A,Double_t &B,Double_t &C,Double_t &D,Double_t &G) const {
  // Returns coefficients of equation
  A = a;
  B = b;
  C = c;
  D = d;
  G = g;
}
void TLitEqIndex::GetCoefficients(Double_t &A,Double_t &B,Double_t &C,Double_t &D,Double_t &G, Double_t &GD) const {
  // Returns coefficients of equation including fGd
  A  = a;
  B  = b;
  C  = c;
  D  = d;
  G  = g;
  GD = fGd;
}
void TLitEqIndex::GetfB(Bool_t &b0,Bool_t &b1,Bool_t &b2,Bool_t &b3) const {
  // Returns the 4 signs of Sqrt(z-g) for each solution
  b0 = fBc[0];
  b1 = fBc[1];
  b2 = fBc[2];
  b3 = fBc[3];
}
void TLitEqIndex::GetfD(TComplex &z0,TComplex &z1,TComplex &z2,TComplex &z3) const {
  // Returns the 4 associated cosines
  z0 = fDc[0];
  z1 = fDc[1];
  z2 = fDc[2];
  z3 = fDc[3];
}
void TLitEqIndex::GetfR(Bool_t &b0,Bool_t &b1,Bool_t &b2,Bool_t &b3) const {
  // Returns whether solution is complex or real. True if complex
  b0 = fRc[0];
  b1 = fRc[1];
  b2 = fRc[2];
  b3 = fRc[3];
}
void TLitEqIndex::GetfZ(TComplex &z0,TComplex &z1,TComplex &z2,TComplex &z3) const {
  // Returns the 4 roots
  z0 = fZc[0];
  z1 = fZc[1];
  z2 = fZc[2];
  z3 = fZc[3];
}
void TLitEqIndex::GetQs2(Double_t &q0,Double_t &q1,Double_t &q2,Double_t &q3) const {
  // Returns the quality of the solution with the wrong sign of Sqrt(z-g)
  q0 = fQs2[0];
  q1 = fQs2[1];
  q2 = fQs2[2];
  q3 = fQs2[3];
}
void TLitEqIndex::GetQsc(Double_t &q0,Double_t &q1,Double_t &q2,Double_t &q3) const {
  // Returns the quality of the solution
  q0 = fQsc[0];
  q1 = fQsc[1];
  q2 = fQsc[2];
  q3 = fQsc[3];
}
void TLitEqIndex::Init() {
  // Checks degeneracy and define fTypeEq
  const Double_t zero   = 0.0;
  const Double_t small  = 5.0e-7;
  const Double_t vsmall = 1.0e-12;
  Double_t ab,ac,y1;
  /*
    TLit::SetSize(5);
    TLit::Book();
  */
  fTypeEq  = general;
  ab = TMath::Abs(b);
  ac = TMath::Abs(c);
  if ((ab<small) && (ac<small)) {
    fTypeEq = bczero;
    b = zero;
    c = zero;
  }
  else {
    if (ab>ac*vsmall) {
      y1 = -c/b;
      ab = TMath::Abs(y1*y1 + a*y1 + d);
      if (ab<small) {
        fTypeEq = degenerate;
      }
    }
  }
}
void TLitEqIndex::InitP() {
  // All pointers to 0 and defaults to non-stored variables
  const Double_t zero = 0.0;
  Short_t k;
  fGraphP  =  0;
  fGraphM  =  0;
  fHRes    =  0;
  fWithRMP =  kTRUE;
  fGd      = -1.0e+99;
  fDebug   =  kFALSE;
  fNr      =  0;
  fNp      =  0;
  for (k=0;k<4;k++) {
    fZc[k]  = zero;
    fBc[k]  = kFALSE;
    fRc[k]  = kFALSE;
    fQsc[k] = zero;
    fQs2[k] = zero;
    fDc[k]  = zero;
  }
  fResidue = -1.0;
}
Bool_t TLitEqIndex::IsEqual(const TObject *obj) const {
  // 2 TLitEqIndex are equal if all there coefficients are equal
  const Double_t eps = 1.0e-20;
  Bool_t equal = kFALSE;
  Double_t dd;
  TLitEqIndex *P;
  P = (TLitEqIndex*)obj;
  dd   = 0.0;
  dd  += TMath::Abs(a - P->a);
  dd  += TMath::Abs(b - P->b);
  dd  += TMath::Abs(c - P->c);
  dd  += TMath::Abs(d - P->d);
  dd  += TMath::Abs(g - P->g);
  equal = dd<eps;
  return equal;
}
Double_t TLitEqIndex::Newton1(Double_t &x,Bool_t ssqr) const {
  // Tries to better the root x with +Sqrt(z-g) if ssqr else with -Sqrt(z-g)
  //by Newton method of 1st order. The may be better value is returned in x
  //and the returned value of the method is the absolute value of equation
  //(E1) at the new found x.
  const Double_t big    = 1.0e+12;
  const Int_t    kbonus = 2;       //Nb. of supplementary loops after convergence
  const Int_t    klmax  = 100;     //Max number of loops. If exceeded, failure
  const Double_t eps2   = 1.0e-12; //if |P(x)| > eps2, no convergence
  const Double_t epsd   = 5.0e-15; //for test derivative too small
  Int_t kloop,kl;
  TComplex z;
  Double_t x0;    //old value of x before Newton step
  Double_t x1;    //new value of x after Newton step
  Double_t dx;    //x1-x0
  Double_t F0;    //value of Y(x0)
  Double_t AF0;   //absolute value of F0
  Double_t F1;    //value of Y(1,x0), 1st derivative of (E1) at x0
  Double_t AF1;   //absolute value of F1
  Double_t G0;    //value of Y(x1)
  Double_t AG0;   //absolute value of G0
  Double_t error = big;
  if (x>g) {
    x0  = x;
    z   = Y(x0,ssqr);
    F0  = z.Re();
    z   = Y(1,x0,ssqr);
    F1  = z.Re();
    AF0 = TMath::Abs(F0);
    AF1 = TMath::Abs(F1);
    if (AF1< epsd*AF0) return error;
    kloop = 0;
    kl = 0;
    while ((kloop<=kbonus) && (kl<klmax)) {
      kl++;
      dx  = -F0/F1;
      x1  = x0+dx;
      if (x1<g) {
        error = big;
        return error;
      }
      z   = Y(x1,ssqr);
      G0  = z.Re();
      AG0 = YAbs(x1,ssqr);
      if (AG0<AF0) {
        x0  = x1;
        F0  = G0;
        AF0 = AG0;
        z   = Y(1,x1,ssqr);
        F1  = z.Re();
        AF1 = TMath::Abs(F1);
        if (AF1<epsd*AF0) kloop = kbonus + 1;
        if (AF0<=eps2) kloop++;
      }
      else kloop = kbonus + 1;
    }//end while ((kloop<=kbonus) && (kl<klmax))
    x     = x0;
    error = AF0;
  }//end if (x>g)
  return error;
}
Double_t TLitEqIndex::Newton2(TComplex &z,Bool_t ssqr) const {
  // Assumes that z is near a complex root and tries to better it
  //using the Newton method of 2nd order for complex roots. The may be better
  //value of the root is returned in z and the absolute value of equation (E1)
  //at z is the returned value. If the returned value is >= big, it
  //means that the Newton2 method has not converged.
  // Do not touch the class variables. It is up to the user if he
  //accepts the new z.
  const Double_t big    = 1.0e+12;
  const Int_t    kbonus = 2;       //Nb. of supplementary loops after convergence
  const Int_t    klmax  = 100;     //Max number of loops. If exceeded, failure
  const Double_t epsd   = 5.0e-15; //for test derivative too small
  const Double_t eps2   = 1.0e-12; //if |P(x)| > eps2, no convergence
  Int_t kloop,k1;
  Double_t error = big;
  TComplex z0;      //old value of complex root
  TComplex z1;      //new value of complex root
  TComplex dz;      //z1-z0
  TComplex Pz0;     //value of normalized polynomial at z0
  TComplex Pz1;     //value of normalized polynomial at z1
  Double_t Az0;     //absolute value of Pz0
  Double_t Az1;     //absolute value of Pz1
  TComplex A,B,C,D; //complex coefficients of 2nd order Newton polynomial
  TComplex DA;      // == 2*A
  Double_t AA;      //absolute value of A
  Double_t AB;      //absolute value of B
  TComplex dz1;     //value of Newton dz if 1st root of Newton polynomial taken
  Double_t adz1;    //absolute value of dz1
  TComplex dz2;     //value of Newton dz if 2nd root of Newton polynomial taken
  Double_t adz2;    //absolute value of dz2
  kloop = 0;
  z0  = z;
  C   = Y(z0,ssqr);
  B   = Y(1,z0,ssqr);
  DA  = Y(2,z0,ssqr);
  A   = 0.5*DA;
  Pz0 = C;
  Az0 = Pz0.Rho();
  AA  = A.Rho();
  k1  = 0;
  while ((kloop<=kbonus) && (k1<klmax)) {
    k1++;
    if (AA<epsd) {
      AB = B.Rho();
      if (AB<epsd) {
        error = big;
        return error;
      }
      else dz = -C/B;
    }
    else {
      D    = B*B-4.0*A*C;
      D    = TComplex::Sqrt(D);
      dz1  = -(B-D)/DA;
      dz2  = -(B+D)/DA;
      adz1 = dz1.Rho();
      adz2 = dz2.Rho();
      if (adz1<=adz2) dz = dz1;
      else            dz = dz2;
    }
    z1  = z0 + dz;
    Pz1 = Y(z1,ssqr);
    Az1 = Pz1.Rho();
    if ((Az1>Az0) && (Az0>eps2)) {
      //Does not converge, stop it and forget all
      error = big;
      return error;
    }
    else {
      if (Az1<Az0) {
        z0  = z1;
        C   = Y(z0,ssqr);
        B   = Y(1,z0,ssqr);
        DA  = Y(2,z0,ssqr);
        A   = 0.5*DA;
        Pz0 = C;
        Az0 = Pz0.Rho();
        if (Az0<eps2) kloop++;
      }
      else kloop = kbonus + 1;
    }//end else if ((Az1>Az0) && (Az0>eps2))
  }//end while ((kloop<=kbonus) && (k1<klmax))
  z  = z0;
  error = Az0;
  return error;
}
Bool_t TLitEqIndex::Order1() {
  //Put the 4 roots in increasing order of absolute value of imaginary part
  const Double_t eps = 1.0e-8;
  Bool_t exch = kFALSE;
  Bool_t ok   = kFALSE;
  Short_t k,k1;
  Bool_t bothcomp;
  Bool_t conj[4];
  Double_t d;
  while (!ok) {
    ok = kTRUE;
    k=0;
    while (k<3) {
      k1    = k+1;
      conj[k]  = kFALSE;
      conj[k1] = kFALSE;
      bothcomp = (fRc[k] && fRc[k1]);
      if (bothcomp) {
        d = TMath::Abs(fZc[k1].Im()+fZc[k].Im());
        if (d<eps) {
          conj[k]  = kTRUE;
          conj[k1] = kTRUE;
        }
        if (conj[k] && conj[k1]) {
          if (k==2) {
            if (conj[0] && conj[1]) {
              if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[0].Im())) {
                ok = kFALSE;
                Exchange(0,2);
                Exchange(1,3);
                exch = kTRUE;
              }//end if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[0].Im()))
              k += 2;
            }//end if (conj[0] && conj[1])
            else {
              //Here 2 and 3 complex conjugate, but not 0 and 1
              //If 0 real    and 1 real    ==> do nothing
              //If 0 real    and 1 complex ==> error
              //If 0 complex and 1 real    ==> error
              //If 0 complex and 1 complex ==> error
              if (!fRc[0] && !fRc[1]) k+=2;
              if (!fRc[0] && fRc[1]) {
                gCleanOut->MM(error,"Order1","3 complex roots",ClassName());
                if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[1].Im())) {
                  ok = kFALSE;
                  Exchange(1,2);
                  Exchange(2,3);
                  exch = kTRUE;
                }
                k+=2;
              }
              if (fRc[0] && !fRc[1]) {
                gCleanOut->MM(error,"Order1","3 complex roots",ClassName());
                if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[0].Im())) {
                  ok = kFALSE;
                  Exchange(0,2);
                  Exchange(2,3);
                  exch = kTRUE;
                }
                k+=2;
              }
              if (fRc[0] && fRc[1]) {
                gCleanOut->MM(error,"Order1","0/1 not cplx conjugate and 2/3 yes",ClassName());
                gCleanOut->MM(error,"Order1","We force complex conjugation",ClassName());
                BetterConj(fZc[0],fZc[1]);
                fQsc[0] = YAbs(fZc[0],fBc[0]);
                fQs2[0] = YAbs(fZc[0],!fBc[0]);
                fQsc[1] = YAbs(fZc[1],fBc[1]);
                fQs2[1] = YAbs(fZc[1],!fBc[1]);
                if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[0].Im())) {
                  ok = kFALSE;
                  Exchange(0,2);
                  Exchange(1,3);
                  exch = kTRUE;
                  k += 2;
                }//end if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[0].Im()))
              }//end if (fRc[0] && fRc[1])
            }//end else if (conj[0] && conj[1])
          }//end if (k==2)
          else {
            //Here k != 2 and k and k+1 cplx conjugate ==> do nothing
            k+=2;
          }
        }//end if (conj[k] && conj[k1])
        else {
          //Here k and k+1 are not complex conjugate. We have to make test
          if (TMath::Abs(fZc[k].Im())<TMath::Abs(fZc[k].Im())) {
            ok = kFALSE;
            Exchange(k,k1);
            exch = kTRUE;
          }
          k++;
        }//end else if (conj[k] && conj[k1])
      }//end if (bothcomp)
      else {
        // if k and k1 not both complex, no exchange
        k++;
      }
    }//end while (k<3)
  }//end while (!ok)
  return exch;
}
Bool_t TLitEqIndex::Order2() {
  // Group complex solutions into complex conjugate pairs. We also put the negative
  //imaginary of a complex conjugate pair before the positive imaginary. We also
  //put the less imaginary solution before
  Bool_t exch = kFALSE;
  Double_t dpsi[3];
  if (!fNr) {
    dpsi[0] = TMath::Abs(fZc[0].Im() + fZc[1].Im());
    dpsi[1] = TMath::Abs(fZc[0].Im() + fZc[2].Im());
    dpsi[2] = TMath::Abs(fZc[0].Im() + fZc[3].Im());
    if (dpsi[0]<=dpsi[1]) {
      if (dpsi[0]>dpsi[2]) {
        Exchange(1,3);
        exch = kTRUE;
        // Exchange(2,3);
      }//end if (dpsi[0]>dpsi[2])
    }//(dpsi[0]<=dpsi[1])
    else {
      if (dpsi[1]<dpsi[2]) {
        Exchange(1,2);
        exch = kTRUE;
      }
      else {
        Exchange(1,3);
        exch = kTRUE;
        // Exchange(2,3);
      }
    }//end else (dpsi[0]<=dpsi[1])
  }
  //Now we group the solutions into pairs of complex conjugate, putting the
  //less imaginary first. We also put the negative imaginary of a complex
  //conjugate pair before the positive imaginary.
  if (fNr!=4) {
    Short_t k,k0,k1;
    Bool_t cplx = kFALSE;
    k = 0;
    while ((k<4) && (!cplx)) {
      if (fRc[k]) cplx = kTRUE;
      else k++;
    }
    if (cplx && (k<3)) {
      k1 = k+1;
      if (fZc[k].Im()>fZc[k1].Im()) {
        Exchange(k,k1);
        exch = kTRUE;
      }
      if (fNr==0) {
        k0   = k+2;
        cplx = kFALSE;
        k    = k0;
        while ((k<4) && (!cplx)) {
          if (fRc[k]) cplx = kTRUE;
          else k++;
        }
        if (cplx && (k<3)) {
          k1 = k+1;
          if (fZc[k].Im()>fZc[k1].Im()) {
            Exchange(k,k1);
            exch = kTRUE;
          }//end if (fZc[k].Im()>fZc[k1].Im())
        }//end if (cplx && (k<3))
      }//end if (fNr==0)
    }//end if (cplx && (k<3))
  }//end if (fNr!=4)
  return exch;
}
Bool_t TLitEqIndex::Order3() {
  // Put solutions with +Sqrt(z-g) before solutions with -Sqrt(z-g)
  // If 2 real solutions are with +Sqrt(z-g) or -Sqrt(z-g), put the
  //biggest one first.
  Bool_t exch = kFALSE;
  Short_t k,k1;
  Bool_t ok = kFALSE;
  while (!ok) {
    ok = kTRUE;
    k = 0;
    while (k<3) {
      k1 = k+1;
      if ((!fBc[k] && fBc[k1]) && ((fRc[k] && fRc[k1]) || (!fRc[k] && !fRc[k1]))) {
        ok = kFALSE;
        Exchange(k,k1);
        exch = kTRUE;
      }
      k++;
    }//end while (k<3)
  }//end while (!ok)
  ok = kFALSE;
  while (!ok) {
    ok = kTRUE;
    k = 0;
    while (k<3) {
      k1 = k+1;
      if ((fBc[k] && fBc[k1]) || ((!fBc[k]) && (!fBc[k1]))) {
        if ((!fRc[k]) && (!fRc[k1])) {
          if (fZc[k].Re()<fZc[k1].Re()) {
            ok = kFALSE;
            Exchange(k,k1);
            exch = kTRUE;
          }
        }
      }
      k++;
    }//end while (k<3)
  }//end while (!ok)
  return exch;
}
void TLitEqIndex::PrintEquation(KindOfMessage kind,const char *text) const {
  // Prints coefficients of equation.
  //   kind = {always,debug,info,warning,error,killthis,fatal}
  //   text = text to be appended to title "Coefficients"
  TString s = "Coefficients ";
  s.Append(text);
  gCleanOut->MM(kind,"PrintEquation","",ClassName());
  gCleanOut->MM(kind,"PrintEquation",s.Data(),ClassName());
  gCleanOut->MM(kind,"PrintEquation","",ClassName());
  gCleanOut->MMR(kind,"PrintEquation","Value of a","a",a,ClassName());
  gCleanOut->MMR(kind,"PrintEquation","Value of b","b",b,ClassName());
  gCleanOut->MMR(kind,"PrintEquation","Value of c","c",c,ClassName());
  gCleanOut->MMR(kind,"PrintEquation","Value of d","d",d,ClassName());
  gCleanOut->MMR(kind,"PrintEquation","Value of g","g",g,ClassName());
  if (fGd>0.0) gCleanOut->MMR(kind,"PrintEquation","Value of fGd","fGd",fGd,ClassName());
}
void TLitEqIndex::PrintResult(KindOfMessage kind) const {
  // Prints roots found
  //   kind = {always,debug,info,warning,error,killthis,fatal}
  TString s1;
  s1 = "Final result. Type: ";
  switch (fTypeEq) {
    case bczero:
      s1.Append("bczero");
      break;
    case degenerate:
      s1.Append("degenerate");
      break;
    case general:
      s1.Append("general");
      break;
  }
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  gCleanOut->MMR(kind,"PrintResult",s1.Data(),"g",g,ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  //
  gCleanOut->MMC(kind,"PrintResult","1st root","fZc[0]",fZc[0],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue","fQsc[0]",fQsc[0],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue other sign","fQs2[0]",fQs2[0],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Sign of Sqrt","fBc[0]",fBc[0],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Solution complex","fRc[0]",fRc[0],ClassName());
  gCleanOut->MMC(kind,"PrintResult","1st cosine","fDc[0]",fDc[0],ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  //
  gCleanOut->MMC(kind,"PrintResult","2nd root","fZc[1]",fZc[1],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue","fQsc[1]",fQsc[1],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue other sign","fQs2[1]",fQs2[1],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Sign of Sqrt","fBc[1]",fBc[1],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Solution complex","fRc[1]",fRc[1],ClassName());
  gCleanOut->MMC(kind,"PrintResult","2nd cosine","fDc[1]",fDc[1],ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  //
  gCleanOut->MMC(kind,"PrintResult","3rd root","fZc[2]",fZc[2],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue","fQsc[2]",fQsc[2],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue other sign","fQs2[2]",fQs2[2],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Sign of Sqrt","fBc[2]",fBc[2],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Solution complex","fRc[2]",fRc[2],ClassName());
  gCleanOut->MMC(kind,"PrintResult","3rd cosine","fDc[2]",fDc[2],ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  //
  gCleanOut->MMC(kind,"PrintResult","4th root","fZc[3]",fZc[3],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue","fQsc[3]",fQsc[3],ClassName());
  gCleanOut->MMR(kind,"PrintResult","Residue other sign","fQs2[3]",fQs2[3],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Sign of Sqrt","fBc[3]",fBc[3],ClassName());
  gCleanOut->MMB(kind,"PrintResult","Solution complex","fRc[3]",fRc[3],ClassName());
  gCleanOut->MMC(kind,"PrintResult","4th cosine","fDc[3]",fDc[3],ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
  //
  gCleanOut->MMR(kind,"PrintResult","Total error","fResidue",fResidue,ClassName());
  gCleanOut->MM(kind,"PrintResult","",ClassName());
}
TComplex TLitEqIndex::Rac(const TComplex &z,Bool_t b) const {
  //Calculation of Sqrt(z-g)
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  Double_t x,y,r;
  TComplex zz;
  TComplex I(zero,un);
  x = z.Re();
  y = TMath::Abs(z.Im());
  if (y<wsmall) {
    //Quantity under Sqrt is real
    r = x - g;
    if (r>=zero) {
      //Quantity under Sqrt is real positiv
      r  = TMath::Sqrt(r);
      zz = r;
    }
    else {
      //Quantity under Sqrt is real negativ
      r  = TMath::Sqrt(-r);
      zz = r*I;
    }
  }
  else {
    //Quantity under Sqrt is complex
    zz = TComplex::Sqrt(z-g);
  }
  if (!b) zz = zero - zz;
  return zz;
}
Short_t TLitEqIndex::ReviSign0() {
  // Alternate sign between almost equal solutions
  const Double_t eps  = 2.0e-5; //old 1.0e-8
  const Double_t eps1 = 4.0e-6; //old 3.0e-6
  Short_t kchs = -1;
  Double_t yy;
  TComplex zz;
  if (fBc[0]==fBc[1]) {
    zz = fZc[0] - fZc[1];
    yy = zz.Rho();
    if (yy<eps) {
      if (fQs2[0]<=fQs2[1]) {
        if (fQs2[0]<eps1) {
          yy      = fQs2[0];
          fQs2[0] = fQsc[0];
          fQsc[0] = yy;
          if (fBc[0]) {
            fNp--;
            fBc[0] = kFALSE;
          }
          else {
            fNp++;
            fBc[0] = kTRUE;
          }
          kchs    = 0;
        }
      }
      else {
        if (fQs2[1]<eps1) {
          yy      = fQs2[1];
          fQs2[1] = fQsc[1];
          fQsc[1] = yy;
          if (fBc[1]) {
            fNp--;
            fBc[1] = kFALSE;
          }
          else {
            fNp++;
            fBc[1] = kTRUE;
          }
          kchs    = 1;
        }
      }
    }
  }
  if (fBc[2]==fBc[3]) {
    zz = fZc[2] - fZc[3];
    yy = zz.Rho();
    if (yy<eps) {
      if (fQs2[2]<=fQs2[3]) {
        if (fQs2[2]<eps1) {
          yy      = fQs2[2];
          fQs2[2] = fQsc[2];
          fQsc[2] = yy;
          if (fBc[2]) {
            fNp--;
            fBc[2] = kFALSE;
          }
          else {
            fNp++;
            fBc[2] = kTRUE;
          }
          kchs    = 2;
        }
      }
      else {
        if (fQs2[3]<eps1) {
          yy      = fQs2[3];
          fQs2[3] = fQsc[3];
          fQsc[3] = yy;
          if (fBc[3]) {
            fNp--;
            fBc[3] = kFALSE;
          }
          else {
            fNp++;
            fBc[3] = kTRUE;
          }
          kchs    = 3;
        }
      }
    }
  }
  return kchs;
}
Short_t TLitEqIndex::ReviSign1() {
  // Although possible, we consider as suspect the cases fNp==1 or fNp==3.
  //In that cases we try to change sign of one solution
  const Double_t eps  = 2.5e-6; //old 1.0e-6
  const Double_t eps8 = 2.0e-5; //old 1.0e-8
  const Double_t eps2 = 1.0e-9;
  const Double_t eps3 = 2.0e-6;
  const Double_t big  = 1.0e+99;
  Short_t kchs = -1;
  Short_t k;
  Short_t kmin = -1;
  Double_t min = big;
  Double_t yy,qs;
  TComplex zz;
  switch (fNp) {
    case 0:
      break;
    case 1:
      for (k=0;k<4;k++) {
        if (!fBc[k]) {
          if (fQs2[k]<min) {
            switch (k) {
              case 0:
              case 2:
                zz = fZc[k+1] - fZc[k];
                break;
              case 1:
              case 3:
                zz = fZc[k] - fZc[k-1];
                break;
            }
            yy = zz.Rho();
            if (yy>=eps8) {
              kmin = k;
              min  = fQs2[k];
            }
          }
        }
      }//end for (k=0;k<4;k++)
      if ((kmin>=0) && (min<eps)) {
        fQs2[kmin] = fQsc[kmin];
        fQsc[kmin] = min;
        fBc[kmin]  = kTRUE;
        fNp++;
        kchs       = kmin;
      }
      break;
    case 2:
      zz = fZc[0]-fZc[1];
      yy = zz.Rho();
      if (yy<eps2) {
        if (fBc[0]==fBc[1]) {
          if (fBc[0]) {
            if (fQs2[1]<eps3) {
              fBc[1]  = kFALSE;
              fNp--;
              qs      = fQs2[1];
              fQs2[1] = fQsc[1];
              fQsc[1] = qs;
              kchs    = 1;
            }
          }
          else {
            if (fQs2[0]<eps3) {
              fBc[0]  = kTRUE;
              fNp++;
              qs      = fQs2[0];
              fQs2[0] = fQsc[0];
              fQsc[0] = qs;
              kchs    = 0;
            }            
          }
        }
      }
      zz = fZc[2]-fZc[3];
      yy = zz.Rho();
      if (yy<eps2) {
        if (fBc[2]==fBc[3]) {
          if (fBc[2]) {
            if (fQs2[3]<eps3) {
              fBc[3]  = kFALSE;
              fNp--;
              qs      = fQs2[3];
              fQs2[3] = fQsc[3];
              fQsc[3] = qs;
              kchs    = 3;
            }
          }
          else {
            if (fQs2[2]<eps3) {
              fBc[2]  = kTRUE;
              fNp++;
              qs      = fQs2[2];
              fQs2[2] = fQsc[2];
              fQsc[2] = qs;
              kchs    = 2;
            }            
          }
        }
      }
      break;
    case 3:
      for (k=0;k<4;k++) {
        if (fBc[k]) {
          if (fQs2[k]<min) {
            switch (k) {
              case 0:
              case 2:
                zz = fZc[k+1] - fZc[k];
                break;
              case 1:
              case 3:
                zz = fZc[k] - fZc[k-1];
                break;
            }
            yy = zz.Rho();
            if (yy>=eps8) {
              kmin = k;
              min  = fQs2[k];
            }
          }
        }
      }//end for (k=0;k<4;k++)
      if ((kmin>=0) && (min<eps)) {
        fQs2[kmin] = fQsc[kmin];
        fQsc[kmin] = min;
        fBc[kmin]  = kFALSE;
        fNp--;
        kchs       = kmin;
      }
      break;
    case 4:
      break;
    default:
      break;
  }
  if (fDebug && (kchs>=0))
    gCleanOut->MMI(info,"ReviSign1","Changing sign for root","k",kchs,ClassName());
  return kchs;
}
void TLitEqIndex::Set(Double_t a0,Double_t b0,Double_t c0,Double_t d0,Double_t g0, Double_t gd) {
  // New equation to solve
  // gd only concerns the case where b==c==0, fTypeEq == bczero.  
  // gd is the determinant of the second order equation remaining:
  // if you do not provide it, it will be correctly calculated as:
  //    gd = a*a - 4*d
  // It is also the value provided by SLitrani in case of isotropy
  // In case of anisotropy, [and of b==c==0] there is a better way of 
  //calculating gd. This better way is used by SLitrani in that specific
  //case. It is why we leave the possibility to provide it. But the normal
  //case is to not provide it!
  a=a0; b=b0; c=c0; d=d0; g=g0;
  fGd = gd;
  Init();
}
void TLitEqIndex::ShowEqu(Int_t nbin,Double_t xlow,Double_t xup,Double_t ylow,Double_t yup) {
  // Show the shape of the absolute value of the equation (E1) for real z values
  //between xlow and xup, both with +Sqrt(z-g) and -Sqrt(z-g). If z>=g, the
  //equation y is real. If z<g, y is complex and |y| is drawn.
  // nbin is the number of points in the graph. [Default 1000]
  // If xlow>=xup [default] then suitable value for xlow and xup are calculated
  //by ShowEqu for the abscissa. Else xlow and xup are taken as given.
  // If ylow>=yup [default] then suitable value for ylow and yup are calculated
  //by ShowEqu for the ordinate. Else ylow and yup are taken as given.
  const Int_t    decim  = 3;
  const Double_t eps    = 1.0e-15;
  const Double_t big    = 1.0e+99;
  const Double_t dix    = 10.0;
  const Double_t margex = 0.5;
  const Double_t margey = 0.2;
  Int_t k,k1,k2;
  TString s1,s2;
  Double_t x,y1,y2,dx,dy,ymin,ymax,l1,sign;
  TComplex zp,zm;
  TArrayD xa(nbin),yp(nbin),ym(nbin);
  s1 = "Equation  with +Sqrt(z-g) type: ";
  s2 = "Equation  with -Sqrt(z-g) type: ";
  switch (fTypeEq) {
    case bczero:
      s1.Append("bczero");
      s2.Append("bczero");
      break;
    case degenerate:
      s1.Append("degenerate");
      s2.Append("degenerate");
      break;
    case general:
      s1.Append("general");
      s2.Append("general");
      break;
  }
  if (fResidue<0.0) fResidue = Solve(3);
  if (xlow>=xup) {
    xlow = fZc[0].Re();
    xup  = xlow;
    for (k=1;k<4;k++) {
      x = fZc[k].Re();
      if (x<xlow) xlow = x;
      if (x>xup)  xup  = x;
    }
    dx    = (xup-xlow)*margex;
    xlow -= dx;
    xup  += dx;
    sign  = xlow/TMath::Abs(xlow);
    l1    = TMath::Log10(TMath::Abs(xlow));
    k1    = Int_t(-l1 + decim);
    dx    = TMath::Abs(xlow)*TMath::Exp(k1*TMath::Log(dix));
    k2    = (Int_t)dx;
    if (sign>0.0) dx = k2;
    else          dx = k2+1;
    xlow  = sign*dx*TMath::Exp(-k1*TMath::Log(dix));
    //
    sign  = xup/TMath::Abs(xup);
    l1    = TMath::Log10(TMath::Abs(xup));
    k1    = Int_t(-l1 + decim);
    dx    = TMath::Abs(xup)*TMath::Exp(k1*TMath::Log(dix));
    k2    = Int_t(dx);
    if (sign>0.0) dx = k2+1;
    else          dx = k2;
    xup   = sign*dx*TMath::Exp(-k1*TMath::Log(dix));
  }
  dx = (xup-xlow)/nbin;
  x  = xlow + 0.5*dx;
  ymin =  big;
  ymax = -big;
  for (k=0;k<nbin;k++) {
    xa[k] = x;
    zp    = Y(x,kTRUE);
    zm    = Y(x,kFALSE);
    if (TMath::Abs(zp.Im())<eps) y1 = zp.Re();
    else                         y1 = zp.Rho();
    if (TMath::Abs(zm.Im())<eps) y2 = zm.Re();
    else                         y2 = zm.Rho();
    yp[k] = y1;
    ym[k] = y2;
    if (y1<ymin) ymin = y1;
    if (y1>ymax) ymax = y1;
    if (y2<ymin) ymin = y2;
    if (y2>ymax) ymax = y2;
    x += dx;
  }
  if (ymin>0.0) ymin = 0.0;
  dy    = (ymax-ymin)*margey;
  ymin -= dy;
  ymax += dy;
  sign = ymin/TMath::Abs(ymin);
  l1   = TMath::Log10(TMath::Abs(ymin));
  k1   = Int_t(-l1 + decim);
  dy   = TMath::Abs(ymin)*TMath::Exp(k1*TMath::Log(dix));
  k2   = Int_t(dy);
  if (sign>0.0) dy = k2;
  else          dy = k2+1;
  ymin = sign*dy*TMath::Exp(-k1*TMath::Log(dix));
  //
  sign = ymax/TMath::Abs(ymax);
  l1   = TMath::Log10(TMath::Abs(ymax));
  k1   = Int_t(-l1 + decim);
  dy   = TMath::Abs(ymax)*TMath::Exp(k1*TMath::Log(dix));
  k2   = Int_t(dy);
  if (sign>0.0) dy = k2+1;
  else          dy = k2;
  ymax  = sign*dy*TMath::Exp(-k1*TMath::Log(dix));
  if (!fGraphP) {
    fGraphP = new TGraph(nbin,xa.fArray,yp.fArray);
    fGraphP->SetName("Equation_E1+");
    fGraphP->SetTitle(s1.Data());
    if (ylow>=yup) {
      fGraphP->SetMinimum(ymin);
      fGraphP->SetMaximum(ymax);
    }
    else {
      fGraphP->SetMinimum(ylow);
      fGraphP->SetMaximum(yup);
    }
  }
  if (!fGraphM) {
    fGraphM = new TGraph(nbin,xa.fArray,ym.fArray);
    fGraphM->SetName("Equation_E1-");
    fGraphM->SetTitle(s2.Data());
    if (ylow>=yup) {
      fGraphM->SetMinimum(ymin);
      fGraphM->SetMaximum(ymax);
    }
    else {
      fGraphM->SetMinimum(ylow);
      fGraphM->SetMaximum(yup);
    }
  }
  if (!gTwoPad) {
    TLit::Get()->BookCanvas();
  }
  fGraphP->Draw("AP");
  gTwoPad->ChangePad();
  fGraphM->Draw("AP");
}
void TLitEqIndex::ShowResidue() const {
  // Shows histo of Log10 of residues
  if (fHRes) {
    if (!gTwoPad) TLit::Get()->BookCanvas();
    fHRes->Draw();
    gTwoPad->ChangePad();
  }
}
Double_t TLitEqIndex::Solve(Short_t ani) {
  // Solves the equation, using TLitPolynom in case fTypeEq == general.
  // ani is the anisotropic case
  //   0 ==> material is optically isotropic
  //   1 ==> material is "Uniaxial negative birefringent" ( epsa < epsb = epsc )
  //   2 ==> material is "Uniaxial positiv birefringent"  ( epsa = epsb < epsc )
  //   3 ==> material is "biaxial birefringent" or "totally anisotropic" ( epsa < epsb < epsc )
  // For a general solution, put ani=3 or forget it. Put ani=1 or ani = 2 only
  //in the case where you are sure that the coefficients are coming from a
  //material of the specified type
  //
  const Double_t limbad     = 1.0e-5;
  const Double_t small      = 1.0e-6;
  const Double_t smallt     = 4.0e-7; //old 7.0e-7
  const Double_t vsmall     = 1.0e-8;
  const Double_t epsRMP     = 8.0e-13;
  const Double_t limnewton  = 7.0e-7;   //old 2.0e-7
  const Double_t limidentic = 1.0e-12;
  const Double_t limres     = 1.0e-18;
  const Double_t big        = 1.0e+300;
  //  const Long64_t QNAN       = 0x7ff8000000000000;
  const char *met = "Solve";
  //  Bool_t errorconj;
  Bool_t exch,dispos;
  Bool_t recalculate = kFALSE;
  Short_t k,kchs;
  if (fDebug) {
    gCleanOut->MM(info,met,"Equation to be solved:",ClassName());
    gCleanOut->MMR(info,met,"1st coefficient","a",a,ClassName());
    gCleanOut->MMR(info,met,"2nd coefficient","b",b,ClassName());
    gCleanOut->MMR(info,met,"3rd coefficient","c",c,ClassName());
    gCleanOut->MMR(info,met,"4th coefficient","d",d,ClassName());
    gCleanOut->MMR(info,met,"5th coefficient","g",g,ClassName());
  }
  switch (fTypeEq) {
  case bczero:
    fResidue = SolveSimple();
    if (fResidue>=smallt) fTypeEq = general;
    break;
  case degenerate:
    fResidue = SolveDegen();
    if (fResidue>=smallt) fTypeEq = general;
    break;
  case general:
    break;
  }//end switch (fTypeEq)
  if (fTypeEq==general) {
    //    const Double_t big      = 1.0e+99;
    const Double_t limreal2 = 4.0e-7;   // old 1.0e-7
    const Double_t limreal3 = 6.0e-6;
    Bool_t haschanged;
    //    Bool_t bconj = kFALSE;
    // Short_t kp;
    Short_t k;
    // Short_t isol,again;
    // Double_t epsP4B,epsP4C;
    Double_t a2,b2,c2,d2,epsP4A;
    Double_t eps1,eps2;
    Double_t A,B,C,D; //coefficients of squared equation
    if (fDebug) gCleanOut->MMT(info,met,"Type of equation","type","general",ClassName());
    a2 = a*a;
    b2 = b*b;
    c2 = c*c;
    d2 = d*d;
    A  = 2*a-b2;
    B  = a2+2*d-2*b*c+b2*g;
    C  = 2*a*d-c2+2*b*c*g;
    D  = d2+c2*g;
    //    deux u;
    TLitPol4 P4(1.0,A,B,C,D);
    if (fWithRMP) {
      epsP4A = P4.FindRootsRMP();
      //      u.DD = epsP4A;
      //      if ((u.LL==QNAN) || (epsP4A>epsRMP)) {
      if (epsP4A>epsRMP) {
        epsP4A = P4.FindRoots();
      }
    }
    else {
      epsP4A = P4.FindRoots();
      if (epsP4A>vsmall) epsP4A = P4.FindRootsRMP();
    }
    if (epsP4A>vsmall)
      gCleanOut->MMR(error,met,"Bad solution from TLitPol4","residue",epsP4A,ClassName());
    for (k=0;k<4;k++) fZc[k] = P4.GetRoot(k);
    if (fDebug) {
      gCleanOut->MM(info,met,"Solution of TLitPol4",ClassName());
//      gCleanOut->MII(info,"again",again,"isol",isol);
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
      gCleanOut->MMR(info,met,"Error TLitPol4::FindRoots","epsP4A",epsP4A,ClassName());
      /*
      gCleanOut->MMR(info,met,"Error FindBetter(kTRUE)","epsP4A",epsP4A,ClassName());
      gCleanOut->MMR(info,met,"Error FindBetter(kFALSE)","epsP4B",epsP4B,ClassName());
      gCleanOut->MMR(info,met,"Error FindRawRoots()","epsP4C",epsP4C,ClassName());
      */
    }
    //
    // We force the solutions for which the absolute value of the imaginary part
    //is smaller than 10*limreal3 to be real
    //
    for (k=0;k<4;k++) {
      fRc[k] = kFALSE;
      if (P4.IsComplex(k)) {
        fRc[k] = P4.IsComplex(k,limreal3);
        if (!fRc[k]) fZc[k](fZc[k].Re(),0.0);
      }
    }
    dispos = Disposition();
    if (fDebug) {
      gCleanOut->MM(info,met,"After force real",ClassName());
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMB(info,met,"If true, complex","fRc[0]",fRc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMB(info,met,"If true, complex","fRc[1]",fRc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMB(info,met,"If true, complex","fRc[2]",fRc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
      gCleanOut->MMB(info,met,"If true, complex","fRc[3]",fRc[3],ClassName());
    }
    //
    // The portion of code between "NOT USEFUL" and "END NOT USEFUL" can probably
    //be withdrawn. To be checked
    //
    // "NOT USEFUL"
    //
    // P4.FindRootsRMP or P4.FindRoots always give 2 complex conjugate roots
    //one after the other. So if fRc[0] is true, fRc[1] must be true, for instance!
    //
    /*
    k = 0;
    while (k<3) {
      kp = k+1;
      if (fRc[k]) {
        if (!fRc[kp]) {
          fRc[kp] = kTRUE;
          errorconj = BetterConj(fZc[k],fZc[kp]);
          if (errorconj) {
            cout << endl;
            gCleanOut->MM(warning,met,"2 cplx conjugate roots not adjacent",ClassName());
            cout << endl;
          }
        }
        k += 2;
      }
      else k++;
    }
    if (fDebug) {
      gCleanOut->MM(info,met,"After force conjugation",ClassName());
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
    }
    */
    //
    // "END NOT USEFUL"
    //
    //
    // Choose right sign of Sqrt(z-g)
    //
    fNp = 0;
    for (k=0;k<4;k++) {
      eps1 = YAbs(fZc[k],kTRUE);
      eps2 = YAbs(fZc[k],kFALSE);
      if (eps1<=eps2) {
        fNp++;
        fBc[k]  = kTRUE;
        fQsc[k] = eps1;
        fQs2[k] = eps2;
      }
      else {
        fBc[k]  = kFALSE;
        fQsc[k] = eps2;
        fQs2[k] = eps1;
      }
    }
    //
    // if the quality of a root is worse than limnewton, we try to better it
    //using Newton1 or Newton2
    //
    haschanged = kFALSE;
    for (k=0;k<4;k++) {
      if (((fRc[k]) && (fQsc[k]>0.2*limnewton)) || ((!fRc[k]) && (fQsc[k]>limnewton))) {
        Double_t err;
        haschanged = kTRUE;
        if (fRc[k]) {
          TComplex zz;
          zz  = fZc[k];
          err = Newton2(zz,fBc[k]);
          if (err<fQsc[k]) {
            fZc[k]  = zz;
            if (TMath::Abs(fZc[k].Im())<limreal2) {
              // Result real after Newton
              fRc[k] = kFALSE;
              fZc[k](fZc[k].Re(),0.0);
              switch (k) {
                case 0:
                  break;
                case 1:
                  fRc[0] = kFALSE;
                  fZc[0](fZc[0].Re(),0.0);
                  break;
                case 2:
                  break;
                case 3:
                  fRc[2] = kFALSE;
                  fZc[2](fZc[2].Re(),0.0);
                  break;
              }
            }
            else {
              // Result complex after Newton
              fRc[k] = kTRUE;
              switch (k) {
                case 0:
                  fRc[1] = kTRUE;
                  fZc[1] = TComplex::Conjugate(fZc[0]);
                  k++;
                  break;
                case 1:
                  if (!fRc[0]) {
                    fRc[1] = kFALSE;
                    fZc[1](fZc[1].Re(),0.0);
                  }
                  break;
                case 2:
                  fRc[3] = kTRUE;
                  fZc[3] = TComplex::Conjugate(fZc[2]);
                  k++;
                  break;
                case 3:
                  if (!fRc[2]) {
                    fRc[3] = kFALSE;
                    fZc[3](fZc[3].Re(),0.0);
                  }
                  break;
              }
            }
            fQsc[k] = YAbs(fZc[k],fBc[k]);
            fQs2[k] = YAbs(fZc[k],!fBc[k]);
            if (fDebug)
              gCleanOut->MMI(info,met,"Root made better by Newton2","root",k,ClassName());
          }
        }
        else {
          Double_t xx;
          xx  = fZc[k].Re();
          err = Newton1(xx,fBc[k]);
          if (err<fQsc[k]) {
            fZc[k](xx,0.0);
            fQsc[k] = err;
            fQs2[k] = YAbs(xx,!fBc[k]);
            if (fDebug)
              gCleanOut->MMI(info,met,"Root made better by Newton1","root",k,ClassName());
          }
        }//end else if (fRc[k])
      }//end if (fQsc[k]>limnewton)
    }//end for (k=0;k<4;k++)
    //
    // Choose again right sign of Sqrt(z-g)
    //
    if (haschanged) {
      dispos = Disposition();
      fNp = 0;
      for (k=0;k<4;k++) {
        eps1 = YAbs(fZc[k],kTRUE);
        eps2 = YAbs(fZc[k],kFALSE);
        if (eps1<=eps2) {
          fNp++;
          fBc[k]  = kTRUE;
          fQsc[k] = eps1;
          fQs2[k] = eps2;
        }
        else {
          fBc[k]  = kFALSE;
          fQsc[k] = eps2;
          fQs2[k] = eps1;
        }
      }
    }
    kchs = ReviSign0();
    kchs = ReviSign1();
    recalculate = kFALSE;
    if (fDebug) {
      gCleanOut->MM(info,met,"After search sign",ClassName());
      gCleanOut->MMB(info,met,"Sign of Sqrt(z-g)","fBc[0]",fBc[0],ClassName());
      gCleanOut->MMR(info,met,"Residue","fQsc[0]",fQsc[0],ClassName());
      gCleanOut->MMR(info,met,"Residue other sign","fQs2[0]",fQs2[0],ClassName());
      gCleanOut->MMB(info,met,"Sign of Sqrt(z-g)","fBc[1]",fBc[1],ClassName());
      gCleanOut->MMR(info,met,"Residue","fQsc[1]",fQsc[1],ClassName());
      gCleanOut->MMR(info,met,"Residue other sign","fQs2[1]",fQs2[1],ClassName());
      gCleanOut->MMB(info,met,"Sign of Sqrt(z-g)","fBc[2]",fBc[2],ClassName());
      gCleanOut->MMR(info,met,"Residue","fQsc[2]",fQsc[2],ClassName());
      gCleanOut->MMR(info,met,"Residue other sign","fQs2[2]",fQs2[2],ClassName());
      gCleanOut->MMB(info,met,"Sign of Sqrt(z-g)","fBc[3]",fBc[3],ClassName());
      gCleanOut->MMR(info,met,"Residue","fQsc[3]",fQsc[3],ClassName());
      gCleanOut->MMR(info,met,"Residue other sign","fQs2[3]",fQs2[3],ClassName());
    }
    //
    //Ordering
    //
    fNr = 4;
    for (k=0;k<4;k++) {
      if (fRc[k]) fNr--;
    }
    if (fNr<4) {
      exch = Order1(); //4 roots in increasing order of absolute value of imaginary part
      if (fDebug && exch) {
        gCleanOut->MM(info,met,"After Order1",ClassName());
        gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
        gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
        gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
        gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
      }
    }
    /*
    exch = Order2();
    if (exch) {
      cout << endl;
      gCleanOut->MM(warning,met,"Order2 unexpectidly found useful",ClassName());
      cout << endl;
    }
    if (fDebug && exch) {
      gCleanOut->MM(info,met,"After Order2",ClassName());
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
    }
    */
    exch = Order3();
    if (fDebug && exch) {
      gCleanOut->MM(info,met,"After Order3",ClassName());
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
    }
    //
    // The portion of code between "NOT USEFUL" and "END NOT USEFUL" can probably
    //be withdrawn. To be checked
    //
    // "NOT USEFUL"
    //
    //
    // Makes solution better complex conjugate
    //
    /*
    errorconj = kFALSE;
    if (fRc[0]) {
      if (fRc[1]) {
        bconj = BetterConj(fZc[0],fZc[1]);
        if (bconj) recalculate = kTRUE;
        if (fRc[2]) {
          if (fRc[3]) {
            bconj = BetterConj(fZc[2],fZc[3]);
            if (bconj) recalculate = kTRUE;
          }//end if (fRc[3])
          else {
            errorconj = kTRUE;
          }//end else if (fRc[3])
        }//end if (fRc[2])
      }//end if (fRc[1])
      else {
        errorconj = kTRUE;
      }//end else if (fRc[1])
    }//end if (fRc[0])
    else {
      //0 real
      if (fRc[1]) {
        if (fRc[2]) {
          bconj = BetterConj(fZc[1],fZc[2]);
          if (bconj) recalculate = kTRUE;
        }//end if (fRc[2])
        else {
          errorconj = kTRUE;
        }//end else if (fRc[2])
      }//end if (fRc[1])
      else {
        // 0 and 1 real
        if (fRc[2]) {
          if (fRc[3]) {
            bconj = BetterConj(fZc[2],fZc[3]);
            if (bconj) recalculate = kTRUE;
          }//end if (fRc[3])
          else {
            errorconj = kTRUE;
          }//end else if (fRc[3])
        }//end if (fRc[2])
      }//end else if (fRc[1])
    }//end else if (fRc[0])
    if (errorconj) {
      cout << endl;
      gCleanOut->MM(error,met,"Complex conjugate roots not adjacent",ClassName());
      gCleanOut->MM(error,met," or not well conjugate",ClassName());
      cout << endl;
    }
    if (fDebug) {
      gCleanOut->MM(info,met,"After BetterConj",ClassName());
      gCleanOut->MMC(info,met,"1st root","fZc[0]",fZc[0],ClassName());
      gCleanOut->MMC(info,met,"2nd root","fZc[1]",fZc[1],ClassName());
      gCleanOut->MMC(info,met,"3rd root","fZc[2]",fZc[2],ClassName());
      gCleanOut->MMC(info,met,"4th root","fZc[3]",fZc[3],ClassName());
    }
    */
    //
    // "END NOT USEFUL"
    //
  }//end if (fTypeEq==general)
  //
  //Refuses that 2 solutions be identical. This check has to be done in any case,
  //{bczero,degenerate,general}
  //
  Short_t i = -1;
  Short_t j = -1;
  Bool_t identical = kFALSE;
  TComplex z1,z2;
  Bool_t   b1,b2;
  Double_t dd;
  while ((!identical) && (i<2)) {
    i += 1;
    j  = i;
    z1 = fZc[i];
    b1 = fBc[i];
    while ((!identical) && (j<3)) {
      j += 1;
      z2 = fZc[j];
      b2 = fBc[j];
      if (b1==b2) {
        dd = TComplex::Abs(z2-z1);
        if (dd<limidentic) identical = kTRUE;
      }
    }
  }
  //
  //Calculates the cosines
  //
  for (k=0;k<4;k++) {
    if (fRc[k]) fDc[k] = Rac(fZc[k],fBc[k])/TComplex::Sqrt(fZc[k]);
    else fDc[k] = Rac(fZc[k],fBc[k])/TMath::Sqrt(fZc[k].Re());
  }
  //
  //In the case of "uniaxial negativ birefringence", if the 4 solutions are
  //real, then the 2 highest solutions must be equal.
  //In the case of "uniaxial positiv birefringence", if the 4 solutions are
  //real, then the 2 smallest solutions must be equal.
  //We verify that !
  //
  Bool_t buni = kFALSE;
  switch (ani) {
  case 1:
  case 2:
    buni = (!fRc[0]) && (!fRc[1]) && (!fRc[2]) && (!fRc[3]);
    break;
  default:
    break;
  }
  if ((buni) && (fTypeEq == general)) {
    Double_t diff;
    Double_t amax1 = -1.0;
    Double_t amax2 = -1.0;
    Double_t amin1 = big;
    Double_t amin2 = big;
    Short_t k;
    Short_t i1 = 0;
    Short_t i2 = 0;
    switch (ani) {
    case 1:
      for (k=0;k<4;k++)
        if (fZc[k].Re()>amax1) {
          i1 = k;
          amax1 = fZc[k].Re();
        }
        for (k=0;k<4;k++)
          if ((k != i1) && (fZc[k].Re()>amax2)) {
            i2 = k;
            amax2 = fZc[k].Re();
          }
          diff = TMath::Abs(amax1-amax2);
          if (diff>small) {
            gCleanOut->MM(error,met,"Case Uniaxial neg. birefringence bad",ClassName());
          }
          else {
            diff = (amax1 + amax2)/2.0;
            fZc[i1] = diff;
            fZc[i2] = diff;
            recalculate = kTRUE;
          }
          break;
    case 2:
      for (k=0;k<4;k++)
        if (fZc[k].Re()<amin1) {
          i1 = k;
          amin1 = fZc[k].Re();
        }
        for (k=0;k<4;k++)
          if ((k != i1) && (fZc[k].Re()<amin2)) {
            i2 = k;
            amin2 = fZc[k].Re();
          }
          diff = TMath::Abs(amin1-amin2);
          if (diff>small) {
            gCleanOut->MM(error,met,"Case Uniaxial pos. birefringence bad",ClassName());
          }
          else {
            diff = (amin1 + amin2)/2.0;
            fZc[i1] = diff;
            fZc[i2] = diff;
            recalculate = kTRUE;
          }
          break;
    default:
      break;
    }//end switch (ani)
  }//end if ((buni) && (fTypeEq == general))
  if (recalculate) for (k=0;k<4;k++) {
    fQsc[k] = YAbs(fZc[k],fBc[k]);
    fQs2[k] = YAbs(fZc[k],!fBc[k]);
  }
  fResidue = 0.0;
  if (fDebug && identical)
    gCleanOut->MM(info,met,"2 solutions are identical",ClassName());
  for (k=0;k<4;k++) fResidue += fQsc[k];
  if (fResidue>limbad) PrintEquation(error,"of bad solution");
  if (fDebug || (fResidue>limbad)) {
    KindOfMessage kind;
    if (fResidue>limbad) kind = error;
    else                 kind = info;
    PrintResult(kind);
  }
  if (TMath::Abs(fResidue)<limres) fResidue = limres;
  if (fHRes) fHRes->Fill(TMath::Log10(fResidue));
  return fResidue;
}
Double_t TLitEqIndex::SolveDegen() {
  // Finds the 4 solutions in case of degeneracy (see class description)
  const Double_t limreal2 = 1.0e-7;
  const Double_t eps      = 5.0e-7;
  const TComplex I(0.0,1.0);
  Short_t k;
  TComplex rz2,rz3;
  Double_t csb; // c/b
  Double_t x0,x1,x2,y1,r1,bb,dd,eps1,eps2,eps3,eps4;
  // 1st 2 solutions
  csb  = c/b;
  x0   = -csb;
  y1   = -a + csb;
  fZc[0]  = x0;
  fBc[0]  = kTRUE;
  fRc[0]  = kFALSE;
  fQsc[0] = YAbs(x0,fBc[0]);
  fZc[3]  = x0;
  fBc[3]  = kFALSE;
  fRc[3]  = kFALSE;
  fQsc[3] = YAbs(x0,fBc[3]);
  // Last 2 solutions
  r1   = b*b;
  dd   = r1*(r1 + 4.0*(y1-g));
  bb   = r1 + 2.0*y1;
  if (dd>=0.0) {
    fRc[1] = kFALSE;
    fRc[2] = kFALSE;
    dd     = TMath::Sqrt(dd);
    x1     = (bb+dd)/2.0;
    fZc[1] = x1;
    eps1   = YAbs(x1,kTRUE);
    eps2   = YAbs(x1,kFALSE);
    if (eps1<=eps2) {
      fBc[1]  = kTRUE;
      fQsc[1] = eps1;
    }
    else {
      fBc[1]  = kFALSE;
      fQsc[1] = eps2;
    }
    x2     = (bb-dd)/2.0;
    fZc[2] = x2;
    eps3   = YAbs(x2,kTRUE);
    eps4   = YAbs(x2,kFALSE);
    if (eps3<eps4) {
      fBc[2]  = kTRUE;
      fQsc[2] = eps3;
    }
    else {
      fBc[2]  = kFALSE;
      fQsc[2] = eps4;
    }
  }
  else {
    fRc[1] = kTRUE;
    fRc[2] = kTRUE;
    dd     = TMath::Sqrt(-dd);
    //
    // solutions which are almost real are made real !
    //
    if (0.5*dd<limreal2) {
      dd = 0.0;
      fRc[1] = kFALSE;
      fRc[2] = kFALSE;
      if (fDebug)
        gCleanOut->MM(info,"SolveDegen","Solutions almost real made real",ClassName());
    }
    if (fRc[1]) fZc[1] = (bb - I*dd)/2.0;
    else        fZc[1] = bb/2.0;
    eps1   = YAbs(fZc[1],kTRUE);
    eps2   = YAbs(fZc[1],kFALSE);
    if (eps1<=eps2) {
      fBc[1]  = kTRUE;
      fQsc[1] = eps1;
    }
    else {
      fBc[1]  = kFALSE;
      fQsc[1] = eps2;
    }
    if (fRc[2]) fZc[2] = (bb + I*dd)/2.0;
    else       fZc[2] = bb/2.0;
    eps3   = YAbs(fZc[2],kTRUE);
    eps4   = YAbs(fZc[2],kFALSE);
    if (eps3<eps4) {
      fBc[2]  = kTRUE;
      fQsc[2] = eps3;
    }
    else {
      fBc[2]  = kFALSE;
      fQsc[2] = eps4;
    }
  }
  for (k=0;k<4;k++) fQs2[k]   = YAbs(fZc[k],!fBc[k]);
  if (fBc[1]==fBc[2]) {
    Double_t yy;
    if (fQs2[1]<fQs2[2]) {
      if (fQs2[1]<eps) {
        yy = fQs2[1];
        fQs2[1] = fQsc[1];
        fQsc[1] = yy;
        fBc[1]  = !fBc[1];
      }
    }
    else {
      if (fQs2[2]<eps) {
        yy = fQs2[2];
        fQs2[2] = fQsc[2];
        fQsc[2] = yy;
        fBc[2]  = !fBc[2];
      }
    }
  }
  //
  // We want the solution with +Sqrt(z-g) before the solutions with -Sqrt(z-g)
  //
  if ((fBc[2]) && (!fBc[1])) {
    TComplex zz4;
    Bool_t b4,r4;
    Double_t q4;
    zz4  = fZc[1];
    b4   = fBc[1];
    r4   = fRc[1];
    q4   = fQsc[1];
    fZc[1]  = fZc[2];
    fBc[1]  = fBc[2];
    fRc[1]  = fRc[2];
    fQsc[1] = fQsc[2];
    fZc[2]  = zz4;
    fBc[2]  = b4;
    fRc[2]  = r4;
    fQsc[2] = q4;
    if (fDebug)
      gCleanOut->MM(info,"SolveDegen","Exchanging roots 1 and 2",ClassName());
  }
  fResidue = 0.0;
  for (k=0;k<4;k++) fResidue += fQsc[k];
  if (fDebug) {
    gCleanOut->MM(info,"SolveDegen","Final result of SolveDegen",ClassName());
    gCleanOut->MMC(info,"SolveDegen","1st root","fZc[0]",fZc[0],ClassName());
    gCleanOut->MMR(info,"SolveDegen","1st Residue","fQsc[0]",fQsc[0],ClassName());
    gCleanOut->MMR(info,"SolveDegen","2nd Residue","fQs2[0]",fQs2[0],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Sign of Sqrt","fBc[0]",fBc[0],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Solution complex","fRc[0]",fRc[0],ClassName());
    gCleanOut->MMC(info,"SolveDegen","2nd root","fZc[1]",fZc[1],ClassName());
    gCleanOut->MMR(info,"SolveDegen","1st Residue","fQsc[1]",fQsc[1],ClassName());
    gCleanOut->MMR(info,"SolveDegen","2nd Residue","fQs2[1]",fQs2[1],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Sign of Sqrt","fBc[1]",fBc[1],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Solution complex","fRc[1]",fRc[1],ClassName());
    gCleanOut->MMC(info,"SolveDegen","3rd root","fZc[2]",fZc[2],ClassName());
    gCleanOut->MMR(info,"SolveDegen","1st Residue","fQsc[2]",fQsc[2],ClassName());
    gCleanOut->MMR(info,"SolveDegen","2nd Residue","fQs2[2]",fQs2[2],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Sign of Sqrt","fBc[2]",fBc[2],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Solution complex","fRc[2]",fRc[2],ClassName());
    gCleanOut->MMC(info,"SolveDegen","4th root","fZc[3]",fZc[3],ClassName());
    gCleanOut->MMR(info,"SolveDegen","1st Residue","fQsc[3]",fQsc[3],ClassName());
    gCleanOut->MMR(info,"SolveDegen","2nd Residue","fQs2[3]",fQs2[3],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Sign of Sqrt","fBc[3]",fBc[3],ClassName());
    gCleanOut->MMB(info,"SolveDegen","Solution complex","fRc[3]",fRc[3],ClassName());
    gCleanOut->MMR(info,"SolveDegen","Total error","fResidue",fResidue,ClassName());
  }
  return fResidue;
}
Double_t TLitEqIndex::SolveSimple() {
  //Solve in case b == c == 0 !
  const Double_t mbig = -1.0e+98;
  Short_t k;
  TComplex I(0.0,1.0);
  Bool_t cplx = kFALSE;
  TComplex cdd;
  Double_t dd;
  dd = fGd;
  if (dd<mbig) dd = a*a - 4.0*d;
  if (dd>=0.0) {
    dd  = TMath::Sqrt(dd);
    fZc[0]  = (-a + dd)/2.0;
    fZc[1]  = (-a - dd)/2.0;
  }
  else {
    cplx = kTRUE;
    dd  = TMath::Sqrt(-dd);
    cdd = I*dd;
    fZc[0]  = (-a - cdd)/2.0;
    fZc[1]  = (-a + cdd)/2.0;
  }
  fZc[2]  = fZc[0];
  fZc[3]  = fZc[1];
  fBc[0]  = kTRUE;
  fBc[1]  = kTRUE;
  fBc[2]  = kFALSE;
  fBc[3]  = kFALSE;
  fResidue = 0.0;
  for (k=0;k<4;k++) {
    fRc[k]    = cplx;
    fQsc[k]   = YAbs(fZc[k],fBc[k]);
    fQs2[k]   = YAbs(fZc[k],!fBc[k]);
    fResidue += fQsc[k];
  }
  if (fDebug) {
    gCleanOut->MM(info,"SolveSimple","Final result of SolveSimple",ClassName());
    gCleanOut->MMC(info,"SolveSimple","1st root","fZc[0]",fZc[0],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Sign of Sqrt","fBc[0]",fBc[0],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Solution complex","fRc[0]",fRc[0],ClassName());
    gCleanOut->MMR(info,"SolveSimple","Residue","fQsc[0]",fQsc[0],ClassName());
    gCleanOut->MMC(info,"SolveSimple","2nd root","fZc[1]",fZc[1],ClassName());
    gCleanOut->MMR(info,"SolveSimple","Residue","fQsc[1]",fQsc[1],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Sign of Sqrt","fBc[1]",fBc[1],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Solution complex","fRc[1]",fRc[1],ClassName());
    gCleanOut->MMC(info,"SolveSimple","3rd root","fZc[2]",fZc[2],ClassName());
    gCleanOut->MMR(info,"SolveSimple","Residue","fQsc[2]",fQsc[2],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Sign of Sqrt","fBc[2]",fBc[2],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Solution complex","fRc[2]",fRc[2],ClassName());
    gCleanOut->MMC(info,"SolveSimple","4th root","fZc[3]",fZc[3],ClassName());
    gCleanOut->MMR(info,"SolveSimple","Residue","fQsc[3]",fQsc[3],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Sign of Sqrt","fBc[3]",fBc[3],ClassName());
    gCleanOut->MMB(info,"SolveSimple","Solution complex","fRc[3]",fRc[3],ClassName());
    gCleanOut->MMR(info,"SolveSimple","Total error","fResidue",fResidue,ClassName());
  }
  return fResidue;
}
TComplex TLitEqIndex::Y(TComplex z,Bool_t ssqr) const {
  // Returns the value of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TComplex y,rac;
  if (ssqr) rac =  TComplex::Sqrt(z-g);
  else      rac = -TComplex::Sqrt(z-g);
  y = z*z+a*z+d+(b*z+c)*rac;
  return y;
}
TComplex TLitEqIndex::Y(Double_t x,Bool_t ssqr) const {
  // Returns the may be complex value of the equation at real x
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TComplex y,zg,rac;
  zg = x - g;
  if (ssqr) rac =  TComplex::Sqrt(zg);
  else      rac = -TComplex::Sqrt(zg);
  y = x*x+a*x+d+(b*x+c)*rac;
  return y;
}
TComplex TLitEqIndex::Y(Short_t k,TComplex z,Bool_t ssqr) const {
  // If k==0 returns the value of the equation at z
  // If k==1 returns the value of the 1st derivative of the equation at z
  // If k==2 returns the value of the 2nd derivative of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TComplex y(0.0,0.0);
  TComplex rac;
  if (ssqr) rac =  TComplex::Sqrt(z-g);
  else      rac = -TComplex::Sqrt(z-g);
  switch (k) {
    case 0:
      y = z*z+a*z+d+(b*z+c)*rac;
      break;
    case 1:
      y = 2.0*z+a+b*rac+(b*z+c)/(2.0*rac);
      break;
    case 2:
      y = 2.0 + b/(2.0*rac) + (b*z-2*b*g-c)/(4.0*rac*rac*rac);
      break;
    default:
      break;
  }
  return y;
}
TComplex TLitEqIndex::Y(Short_t k,Double_t x,Bool_t ssqr) const {
  // If k==0 returns the value of the equation at z
  // If k==1 returns the value of the 1st derivative of the equation at z
  // If k==2 returns the value of the 2nd derivative of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TComplex y(0.0,0.0);
  TComplex zg,rac;
  zg = x - g;
  if (ssqr) rac =  TComplex::Sqrt(zg);
  else      rac = -TComplex::Sqrt(zg);
  switch (k) {
    case 0:
      y = x*x+a*x+d+(b*x+c)*rac;
      break;
    case 1:
      y = 2*x+a+b*rac+(b*x+c)/(2.0*rac);
      break;
    case 2:
      y = 2.0 + b/(2.0*rac) + (b*x-2*b*g-c)/(4.0*rac*rac*rac);
      break;
    default:
      break;
  }
  return y;
}
Double_t TLitEqIndex::YAbs(TComplex z,Bool_t ssqr) const {
  // Returns the absolute value of the equation at z. If z is a
  //root, it is a measure of the quality of the root
  Double_t rho;
  TComplex y;
  y = Y(z,ssqr);
  rho = y.Rho();
  return rho;
}
Double_t TLitEqIndex::YAbs(Double_t x,Bool_t ssqr) const {
  // Returns the absolute value of the equation at x. If x is a
  //root, it is a measure of the quality of the root
  Double_t rho;
  TComplex y;
  y = Y(x,ssqr);
  rho = y.Rho();
  return rho;
}
TComplex TLitEqIndex::operator()(TComplex z,Bool_t ssqr) const {
  // Returns the value of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  return Y(z,ssqr);
}
TComplex TLitEqIndex::operator()(Double_t x,Bool_t ssqr) const {
  // Returns the may be complex value of the equation at real x
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  return Y(x,ssqr);
}
Bool_t TLitEqIndex::operator==(const TLitEqIndex &P) {
  return IsEqual(&P);
}
