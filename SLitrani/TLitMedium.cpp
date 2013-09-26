// @(#)root/html:$Id: TLitMedium.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TObjArray.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitSellmeier.h"
#include "TLitSpectrum.h"
#include "TLitVolume.h"
#include "TLitMedium.h"

ClassImp(TLitMedium)
//______________________________________________________________________________
/* BEGIN_HTML
		<center>
			<h1>TLitMedium</h1>
		</center>
		<div align="justify">
			<p><code><b>TLitMedium</b></code> inherits from <a href="http://root.cern.ch/root/html/TGeoMedium.html"><code><b>TGeoMedium</b></code></a> 
and adds the properties wanted by <b><code>SLitrani</code></b> which are not in <a href="http://root.cern.ch/root/html/TGeoMedium.html"><code><b>TGeoMedium</b></code></a>, 
namely the characteristics of an optical medium. Notice that calling the constructor 
of the class is not enough for defining all the properties. The constructor defines, 
in addition to the entries required by <a href="http://root.cern.ch/root/html/TGeoMedium.html"><b><code>TGeoMedium</code></b>,</a> 
sensibility characteristics (detector or not), magnetic permeability and absorption 
length(s). The constructor does not define index of refraction or the components 
of the dielectric tensor, neither fluorescent characteristics of the medium, nor 
dE/dx deposit, nor diffusion characteristics, no radiation length, no Moliere radius, 
no critical energy. See below how to define these characteristics. A <code><b>TLitMedium</b></code> 
can describe an ordinary optical tracking medium or a wrapping, see below.&nbsp;It 
can also be used to imply the existence of a <a href="#thinslice">thin slice</a> 
between two <code><b>TLitMedium</b></code>. Finally, it can also be used to simulate 
depolished face between two <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>, 
in case a <code><b>TLitMedium</b></code> being not a wrapping and a <code><b>TLitMedium</b></code> 
being a wrapping are in contact. See <a href="#depolish">paragraph</a> about depolished 
face below.</p>
			<p></p>
		</div>
		<div align="center">
			<h2>Normal optical medium or wrapping</h2>
		</div>
		<div align="justify">
			<p><code><b>TLitMedium</b></code> is used to describe</p>
			<ul>
				<li><b>A normal optical tracking medium</b>, like <b><code>CsI, PbWO4</code></b>, 
glass, plastic, and so on, in which a photon <b>can</b> travel. Constructors 1 to 
9 define this kind of <b><code>TLitMedium</code></b>.

				<li><b>A wrapping (revetment)</b> like aluminium, mylar, and so on, with a complex 
index of refraction, in which a photon <b>cannot</b> travel, but from which a photon 
can be reflected or absorbed. Constructors 10 and 11 define this kind of <b><code>TLitMedium</code></b>.

			</ul>
			<p>You can declare that two <code><b>TLitMedium</b></code>, in case they are inside 
volumes which are contiguous, are separated by a thin slice.&nbsp;Look at the <a href="#thinslice">chapter 
about thin slice</a>.</p>
			<p>In this new version of <b><code>SLitrani</code></b>, a wrapping occupies a volume, 
not a surface as in the old <b><code>SLitrani</code></b>. In addition, when a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is wrapped with a revetment, there may or may not be a <b>slice of medium</b> (air, 
glue, grease, ...) between the shape and the wrapping. If you look at the class variables 
of <code><b>TLitMedium</b></code> in <code>TLitMedium.h</code>, you will find that 
they are divided into 2 categories:</p>
			<ul>
				<li>a first set of variables simply beginning with <b><code>f</code></b>... either 
specific for the case of a normal optical tracking medium, or used in the 2 cases, 
with meanings specific of the 2 cases.
				<li>a second set of variables beginning wit <b><code>fWrap</code></b>... specific 
for the case of a wrapping

			</ul>
			<p>Among the class variables which take a different meaning in the 2 cases, let 
us quote:</p>
			<ul>
				<li><b><code>fMu, fNameMu, fFitMu, fMuFound</code></b>
				<ul>
					<li>in case of normal optical tracking medium, they refer to the magnetic permeability 
<font face="Symbol"><b>m</b></font> of the medium	
					<li>in case of wrapping, they refer <b>not</b> to the magnetic permeability 
of the wrapping, <b>but</b> to the magnetic permeability of the slice of material 
(air, glue, grease, ...) between the shape and the wrapping.	
				</ul>
				<li><b><code>fIndex, fIndexM, fIndexS, fNameIndex, fFitIndex</code></b>
				<ul>
					<li>in case of normal optical tracking medium, they refer to the index of refraction 
of the medium

					<li>in case of wrapping, they refer <b>not</b> to the index of refraction of 
the wrapping, <b>but</b> to the index of refraction of the slice of material between 
the shape and the wrapping.	
				</ul>
			</ul>
			<p>Look below at the <a href="#wrappings">chapter &quot;wrappings&quot;</a>, for 
more details.</p>
			<p></p>
		</div>
		<div align="center">
			<h2>Normal optical medium</h2>
		</div>
		<div align="justify">
			<p></p>
		</div>
		<div align="center">
			<h3>Anisotropy</h3>
		</div>
		<div align="justify">
			<p>In <b><code>SLitrani</code></b>, not only the dielectric constant may be anisotropic 
(i.e. may become a dielectric tensor), but also the absorption length. Let us first 
recall that there exists a coordinate system, called <dfn><b>&quot;the coordinate 
system of the 3 optical axis&quot;</b></dfn>, in which the dielectric tensor <font face="Symbol" color="red"><b>e</b></font> 
is diagonal. In this system, the dielectric tensor looks like:</p>
		</div>
		<center>
			<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
				<tr>
					<td><font face="Symbol">e</font><font size="1"><sub>a </sub></font></td>
					<td>0</td>
					<td>0</td>
				</tr>
				<tr>
					<td>0</td>
					<td><font face="Symbol">e</font><font size="1"><sub>b</sub></font></td>
					<td>0</td>
				</tr>
				<tr>
					<td>0</td>
					<td>0</td>
					<td><font face="Symbol">e</font><font size="1"><sub>c</sub></font></td>
				</tr>
			</table>
		</center>
		<p><dfn>In the isotropic case</dfn> the absorption length is linked to a small 
imaginary part <font face="Symbol">f</font> subtracted from the dielectric constant</p>
		<div align="center">
			<p><font face="Symbol">e</font><font size="2"><sub>c</sub></font>&nbsp;= &nbsp;<font face="Symbol">e</font> 
- i <font face="Symbol">f</font></p>
		</div>
		<p> This results into a small imaginary part n<sub>I</sub> for the index of refraction: 
</p>
		<div align="center">
			<p>n<sub>C</sub> = n<sub>R</sub> - i n<sub>I</sub></p>
		</div>
		<p>If the plane wave is written:</p>
		<div align="center">
			<p><font color="blue"><b>E</b></font> = <font color="blue"><b>E</b></font><font size="1"><sub>0</sub></font> 
e<sup>i (<font face="Symbol">w</font>t - <font color="blue"><b>k</b></font>*<font color="blue"><b>x</b></font>)</sup></p>
		</div>
		<div align="left">
			<p>then the Maxwell equations leads to :</p>
		</div>
		<div align="center">
			<p><font color="black"><b>k</b></font><font size="1"><sup>2</sup></font> = (<font face="Symbol"> 
w</font><font face="Symbol" size="1"><sup>2</sup></font><font face="Symbol">em</font> 
/ c<font size="1"><sup>2</sup></font> )= (<font face="Symbol"> w</font><font face="Symbol" size="1"><sup>2</sup></font>n<font size="1"><sup>2</sup></font> 
/ c<font size="1"><sup>2</sup></font> ) ===&gt; n<sub>C</sub><sup>2</sup> = <font face="Symbol">e</font><font size="2"><sub>c</sub></font><font face="Symbol">m 
</font>===&gt; n<sub>R</sub><sup>2</sup> - n<sub>I</sub><sup>2</sup> - 2 i n<sub>R</sub> 
n<sub>I</sub> = <font face="Symbol">em</font> - i <font face="Symbol">fm</font></p>
			<p>===&gt; k = (<font face="Symbol"> w</font> / c) (n<sub>R</sub> - i n<sub>I</sub>)</p>
		</div>
		<div align="left">
			<p>implying a real absorption factor e<sup>- (  <font face="Symbol">w</font> n<sub>I</sub> 
x ) / c</sup> . This absorption factor is for the amplitude of the electric field. 
For going to an absorption length, we have to take not the amplitude, but the intensity.&nbsp;So 
the attenuation factor is e<sup>- ( 2 <font face="Symbol">w</font> n<sub>I</sub> 
x ) / c</sup> . leading to an absorption length of </p>
		</div>
		<div align="center">
			<p>L = c / 2 <font face="Symbol">w </font>n<sub>I</sub></p>
		</div>
		<center>
			<p>
			<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
				<tr>
					<td><font face="Symbol">e</font><font size="2"><sub>c</sub></font> = <font face="Symbol">e</font> 
- i <font face="Symbol">f </font></td>
					<td>n<font size="2"><sub>c</sub></font> = n - i <font face="Symbol">t</font></td>
					<td>n<font size="2"><sub>c</sub></font><font size="1"><sup>2</sup></font> = 
<font face="Symbol">e</font><font size="2"><sub>c</sub></font> <font face="Symbol">m</font>&nbsp;</td>
				</tr>
				<tr>
					<td>L = c / 2<font face="Symbol">w</font>n<sub>I</sub><sup> </sup></td>
					<td><font face="Symbol">f</font> = 2 n<sub>R</sub> n<sub>I</sub> / <font face="Symbol">m</font>&nbsp;</td>
					<td>n<sub>I</sub> = <font face="Symbol">f</font> <font face="Symbol">m</font> 
/ 2 n<sub>R</sub></td>
				</tr>
				<tr>
					<td>==&gt; (1)</td>
					<th colspan="2">L = ( n<sub>R</sub> c &nbsp;) / <font face="Symbol">( w f</font> 
<font face="Symbol">m</font> <font face="Symbol">)</font></th>
				</tr>
			</table>
		</center>
		<p>So the generalisation of this to <dfn>the anisotropic case</dfn> seems evident 
: we have to subtract a small imaginary part, this times to each of the diagonal 
elements of the dielectric tensor &nbsp;( see for instance &quot;Principles of Optics&quot; 
Max Born&amp;Emil Wolf page 708) :</p>
		<center>
			<p>
			<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
				<tr>
					<td><font face="Symbol">e</font><font size="1"><sub>a </sub></font>- i <font face="Symbol">f</font><font size="2"><sub>a</sub></font></td>
					<td>0</td>
					<td>0</td>
				</tr>
				<tr>
					<td>0</td>
					<td><font face="Symbol">e</font><font size="1"><sub>b</sub></font>- i <font face="Symbol">f</font><font size="2"><sub>b</sub></font></td>
					<td>0</td>
				</tr>
				<tr>
					<td>0</td>
					<td>0</td>
					<td><font face="Symbol">e</font><font size="1"><sub>c</sub></font> - i <font face="Symbol">f</font><font size="1"><sub>c</sub></font></td>
				</tr>
			</table>
		</center>
		<div align="justify">
			<p>When calculating the index of refraction ( which in the anisotropic case is 
dependent upon the direction of the <font color="blue"><b>k</b></font> vector ), 
we will include these 3 small imaginary parts, which will end up into a complex index 
n whose imaginary part will give the absorption length for this wave with vector 
<font color="blue"><b>k</b></font>. So we will have a different absorption length 
for each different <font color="blue"><b>k</b></font> vector. It would be very inconvenient 
to ask the user of <b><code>SLitrani</code></b> to enter the 3 values <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, &nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font>. 
Instead, we ask him to enter 3 values &nbsp;of absorption lengths L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>&nbsp;, which 
are linked to <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, &nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font> by equation 
(1), which SLitrani will apply to get <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, &nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font> 
. In general L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font>, 
L<font size="1"><sub>c</sub></font>&nbsp;will be given by mean of a fit being dependent 
upon the wavelength. More specifically, <strong>if the user only give L<font size="1"><sub>a</sub></font> 
</strong>, then it will be assumed that the user <strong>does not want an anisotropic 
treatment of the absorption length</strong> and L<font size="1"><sub>a</sub></font> 
will be taken as the unique, isotropic absorption length applicable to all waves, 
whatever their <font color="blue"><b>k</b></font> vector. On the contrary, <strong>if 
more than one value of L is given</strong>, then an <strong>anisotropic treatment 
of the absorption length</strong> will be undertaken by <b><code>SLitrani</code></b>. 
Notice that it does not make sense to declare the medium to be isotropic ( by a later 
call to <b><code>IsIsotropic()</code></b> ) and to give more than one value of L. 
There are 4 possible cases :</p>
		</div>
		<center>
			<p>
			<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
				<tr>
					<td>You intend to call <b><code>IsIsotropic()</code></b></td>
					<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> = <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;= 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
					<td>L<font size="1"><sub>a</sub></font> = L<font size="1"><sub>b</sub></font>&nbsp;= 
L<font size="1"><sub>c</sub></font></td>
					<td>give only L<font size="1"><sub>a</sub></font></td>
				</tr>
				<tr>
					<td>You intend to call <b><code>IsUniAxialNegBirefr()</code></b></td>
					<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> &lt; <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;= 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
					<td>L<font size="1"><sub>b</sub></font>&nbsp;= L<font size="1"><sub>c</sub></font> 
is
						<p>not imposed !</td>
					<td>you are allowed to give 3 different values for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>.
						<p>If you give only L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font> 
then L<font size="1"><sub>c</sub></font> == L<font size="1"><sub>b</sub></font></td>
				</tr>
				<tr>
					<td>You intend to call <b><code>IsUniAxialPosBirefr()</code></b></td>
					<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> = <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;&lt; 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font></td>
					<td>L<font size="1"><sub>b</sub></font>&nbsp;= L<font size="1"><sub>a</sub></font> 
is
						<p>not imposed !</td>
					<td>you are allowed to give 3 different values for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>.
						<p>you can give the same values (same fit) for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font></td>
				</tr>
				<tr>
					<td>You intend to call <b><code>IsBirefr()</code></b></td>
					<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> &lt; <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;&lt; 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
					<td>L<font size="1"><sub>a</sub></font> &lt;&gt; L<font size="1"><sub>b</sub></font>&nbsp;&lt;&gt; 
L<font size="1"><sub>c</sub></font></td>
					<td>give 3 different values for L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font>, 
L<font size="1"><sub>c</sub></font> or give only L<font size="1"><sub>a</sub></font>.</td>
				</tr>
				<tr>
					<th colspan="4">
						<center>
							In each of these 4 cases, you are allowed to give only L<font size="1"><sub>a</sub></font>, 
in which case absorption will be isotropic</center>
					</th>
				</tr>
			</table>
			<p></p>
		</center>
		<center>
			<h3>Absorption length put to 0</h3>
		</center>
		<div align="justify">
			<p>In <b><code>SLitrani</code></b>, there is a difference between an absorption 
length strictly put to 0 and a very small absorption length! When the user put the 
absorption length to 0, he implies doing so that it does not make sense to propagate 
a photon inside a shape made of this medium. The photon is immediatly killed and 
it is counted apart in the counter <b><code>fNpAbsL0</code></b> of the class <b><code>TLitResults</code></b>. 
The photons which have died inside a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
with medium of absorption length 0 are subtracted from the generated photons when 
calculating efficiency, i.e. they are not considered as having been emitted !</p>
			<p>When does it makes sense? Consider for instance photons emitted by an optical 
fibre. In <b><code>SLitrani</code></b>, photons emitted by an optical fibre are emitted 
from the exit face of the fibre, <b>but from the inner side of the fibre</b>, so 
that the possibility exists that the photon be reflected instead of refracted, i.e. 
returns inside the fibre without any chance to come back. These photons have never 
been emitted by the fibre and so have not to be counted among the emitted photons 
when doing statistics calculations. To understand more about this (on first sight) 
strange way of emitting photons from within the fibre, read the very detailed description 
of the simulation of optical fibre in <b><code>SLitrani</code></b> given in the description 
of the class <b><code><a href="TLitSpontan.html">TLitSpontan</a></code></b>.</p>
			<p></p>
		</div>
		<center>
			<h3>Diffusion length</h3>
		</center>
		<div align="justify">
			<p></p>
			<div align="justify">
				<p>It is observed that sometimes a material diffuses a photon in some other direction. 
This phenomenon is due to bubbles of impurities (for instance air) or to defaults 
in the crystal structure. A very big progress has been achieved in <b><code><font size="+1">SLitrani</font></code></b> 
starting from version 3_6. This is due to a very interesting thesis of David Wahl 
[<b><code><font size="+1"><a href="mailto:dwahl@fisica.pucp.edu.pe">dwahl@fisica.pucp.edu.pe</a></font></code></b>] 
:</p>
				<ul>
					<li><code><b><font size="+1">The Monte-Carlo refractive index matching technique 
for determining the input parameters for simulation of the light collection in scintillating 
crystals</font></b></code>
					<li><code><b><font size="+1">D. Wahl, V. Mikhailik and H. Kraus,</font></b></code>
					<li><code><b><font size="+1">Nuclear Instruments and Methods in Physics Research 
Section A</font></b></code>
				</ul>
				<p>Essentially, it is a method, using both a Monte-Carlo (<b><code><font size="+1">SLitrani</font></code></b>) 
and lab measurements, to determine precisely the amount of optical photons which 
are diffused, versus the ones which are absorbed. These measurements showed how important 
diffusion is in crystals.&nbsp;It was also discovered in the <b><code><font size="+1"><a href="http://cms.cern.ch/">CMS</a></font></code></b> 
experiment that a correct description of the <b><code><font size="+1">PbWO<sub>4</sub></font></code></b> 
crystals by <b><code><font size="+1">SLitrani</font></code></b> was only possible 
if one takes into account diffusion.</p>
				<p>David has made a big improvement of the code of <b><code><font size="+1">TLitMedium</font></code></b>, 
in such a way that <b><code><font size="+1">SLitrani</font></code></b> is now able 
to describe 3 kinds of diffusion processes:</p>
				<ol>
					<li><b>Simple diffusion.</b> The user has the possibility to introduce, in addition 
to the absorption length, a diffusion length. The way <b><code><font size="+1">SLitrani</font></code></b> 
simulates simple diffusion, is by absorbing the photon and reemitting it <b>at the 
same place</b>, with the <b>same wavelength</b> but with <b>a new </b><font color="blue"><b>k</b></font><b> 
vector</b>, which is randomly generated on 4<font face="Symbol">p</font>. The probability 
of having a diffusion is handled exactly the same way as the probability of having 
an absorption. The user has to introduce a &quot;diffusion length&quot;, similar 
to the &quot;absorption length&quot;. But contrary to the absorption length, the 
diffusion length is always taken as isitropic by <b><code><font size="+1">SLitrani</font></code></b>. 
The diffusion length may be fixed or depending upon wavelength. If depending upon 
wavelength, it is described by a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html">TSplineFit</a></font></code></b>. 
It is the reason to have 2 version of the method <b><code><font size="+1"><a href="#TLitMedium:SetDiffusionLength">TLitMedium::SetDiffusionLength</a>()</font></code></b>.
					<li><b>Diffusion (or absorption ?) with WaveLength Shifting</b>. The process 
is the same as the simple diffusion, except that the photon is reemitted <i>at a 
larger wavelength</i>, according to an emission spectrum of type <b><code><font size="+1"><a href="TLitSpectrum.html">TLitSpectrum</a></font></code></b>, 
which is specific to the <b><code><font size="+1">WLS</font></code></b> process of 
the material. We hesitate to name this process &quot;diffusion&quot; or &quot;absorption&quot;. 
One can argue that the photon is first absorbed and then remitted at an other wavelength, 
and for this reason should be called &quot;absorption&quot;.&nbsp;But we have included 
this process into the diffusion processes. This requires the introduction of a special 
diffusion (or absortion ) length, specific of the <b><code><font size="+1">WLS</font></code></b> 
process, which can be either a constant or depending upon wavelength.&nbsp;If depending 
upon a wavelength, it is described by a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html">TSplineFit</a></font></code></b>. 
Spectrum and diffusion length for <b><code><font size="+1">WLS</font></code></b> 
are given by a call to <b><code><font size="+1"><a href="#TLitMedium:SetWLS">TLitMedium::SetWLS</a></font></code></b>(). 
An example of definition of this diffusion (or absorption) length with <b><code><font size="+1">WLS</font></code></b> 
can be found in the <b><code><font size="+1">CINT</font></code></b> macro <b><code><font size="+1">AbsLengthWLS_Fibre.C</font></code></b> 
delivered with <b><code><font size="+1">SLitrani</font></code></b> in the directory 
<b><code><font size="+1">FitMacros</font></code></b>. It defines a fit of type <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html">TSplineFit</a></font></code></b>, 
of category 20, whose name begin with <b><code><font size="+1">&quot;AbsLengthWLS_&quot;</font></code></b> 
and whose title begin with <b><code><font size="+1">&quot;Absorption Length for WLS 
| &quot;</font></code></b>, to be stored into the database <b><code><font size="+1">SplineFitDB.rdb</font></code></b>. 
A simple example of <b><code><font size="+1">WLS</font></code></b> is given in the 
<b><code><font size="+1">CINT</font></code></b> macro <b><code><font size="+1">FibreWLS.C</font></code></b>, 
provided with <b><code><font size="+1">SLitrani</font></code></b> in the directory 
&quot;<b><code><font size="+1">Macros</font></code></b>&quot;, and using the fit 
<b><code><font size="+1">AbsLengthWLS_Fibre</font></code></b>.
					<li><b>Rayleigh scattering</b>. Rayleigh scattering refers to the process of 
scattering of light off particles much smaller than the wavelength of the photons, 
such as molecules or atoms. It differs from other scattering in that the photons 
do not scatter isotropically, as is implemented for diffusion in <b><code><font size="+1">SLitrani</font></code></b>. 
Rayleigh scattering is intrinsic to all materials, though often the scattering length 
is so long that it can be neglected. Rayleigh scattering has a preference for regeneration 
of photons in the forward and backward directions. Here again, the process needs 
the introduction of a &quot;scattering length&quot; which can be a constant or depending 
upon wavelength. If depending upon wavelength, it has to be introduced as a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html">TSplineFit</a></font></code></b> 
of category 19, whose name begin with <b><code><font size="+1">&quot;Rayleigh&quot;</font></code></b> 
and whose title begin with <b><code><font size="+1">&quot;Rayleigh length | &quot;</font></code></b>. 
The above quoted example macro <b><code><font size="+1">FibreWLS.C</font></code></b> 
also include Rayleigh scattering as an example. Asking for Rayleigh scattering is 
done by mean of a call to the method <b><code><font size="+1"><a href="#TLitMedium:SetRayleighLength">TLitMedium::SetRayleighLength()</a></font></code></b>.
				</ol>
				<p>The 3 diffusion processes are not exclusive of one another. All 3 can be implemented 
at the same time for a given material.</p>
			</div>
			<center>
				<h3></h3>
			</center>
		</div>
		<center>
			<h3>Electromagnetic showers</h3>
		</center>
		<div align="justify">
			<p>3 more values have to be defined, through the method <b><code>SetCascades()</code></b>, 
if you intend to instantiate later on the class <b><code>TLitCascades</code></b>&nbsp;:</p>
			<ol>
				<li>The radiation length X<sub>0</sub>
				<li>The Moliere radius R<sub>M</sub>
				<li>The Critical Energy E<sub>c</sub>.
			</ol>
			<p>These 3 quantities are only used by <b><code>TLitCascades</code></b>.&nbsp;You 
do not need to define them if you do not use <b><code>TLitCascades</code></b>.</p>
		</div>
		<center>
			<p>
			<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
				<caption>
					<h3>Description of the arguments of some constructors of <code>TLitMedium</code></h3>
				</caption>
				<tr>
					<th><font size="4"><b>Type</b></font></th>
					<th><font size="4"><b>Name</b></font></th>
					<th><font size="4">Default</font></th>
					<th><font size="4">Description</font></th>
				</tr>
				<tr>
					<td><b><code>Text_t*</code></b></td>
					<td><b><code>name</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td><font color="red">Same argument as in <b><code>TGeoMedium</code></b>.</font> 
Name of the optical medium.</td>
				</tr>
				<tr valign="top">
					<td><b><code>Int_t</code></b></td>
					<td><b><code>numed</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td valign="top"><font color="red">Same argument as in <b><code>TGeoMedium</code></b>.</font> 
A not so nice feature of <b><code>TGeoMedium</code></b> is that the user has to assign 
to each medium a medium index.&nbsp;We are obliged to keep to this convention.</td>
				</tr>
				<tr>
					<td><b><code>TGeoMaterial* or TGeoMixture*</code></b></td>
					<td><b><code>mat</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>
						<div align="justify">
							<font color="red">Same argument as in <b><code>TGeoMedium</code></b>.</font> 
Notice that <b><code>SLitrani</code></b> only needs of the <b><code>A</code></b>, 
<b><code>Z</code></b> and<b><code> rho</code></b> values in the <b><code>TGeoMaterial</code></b> 
when it has to calculate Compton scattering, in the class <b><code>TLitGamma</code></b>. 
You can ignore them if you do not use <b><code>TLitGamma</code></b>.</div>
					</td>
				</tr>
				<tr>
					<td><b><code>Bool_t</code></b></td>
					<td><b><code>sensible</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>
						<div align="justify">
							Decides wheter the medium is a detector. For instance, if you define a medium 
&quot;Silicium&quot;, intended to be the sensitive part of an <b><code>APD</code></b>, 
then sensible is true.</div>
					</td>
				</tr>
				<tr>
					<td><b><code>Text_t* &nbsp;or Double_t</code></b></td>
					<td><b><code>nameMu or mu</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>
							For the definition of the magnetic permeability <font face="Symbol">m</font>. 
If you use the first form, nameMu is the name of the fit of type <b><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html"><code>TSplineFit</code></a></b> 
giving the magnetic permeability as a function of wavelength. If you use the second 
form, mu is the value of the magnetic permeability, which is fixed in this case : 
it does not depend upon wavelength. In general, the magnetic permeability is equal 
to 1, but a value different from 1 or even depending upon wavelength is allowed by 
<b><code>SLitrani</code></b>.</td>
				</tr>
				<tr>
					<td><b><code>Text_t* &nbsp;or Double_t</code></b></td>
					<td><b><code>nameAbsLa or absLa</code></b></td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>Parameter L<font size="1"><sub>a</sub></font> for absorption length as described 
above. If you use the first form, <b><code>nameAbsLa</code></b> is the name of the 
fit or interpolation of class <b><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html"><code>TSplineFit</code></a></b> 
giving L<font size="1"><sub>a</sub></font> as a function of wavelength. If you use 
the second form, L<font size="1"><sub>a</sub></font> = <b><code>absLa</code></b> 
does not depend upon wavelength<font size="4">.</font></td>
				</tr>
				<tr>
					<td><b><code>Text_t* &nbsp;or Double_t</code></b></td>
					<td><b><code>nameAbsLb or absLb</code></b></td>
					<td>
						<div align="center">
							&quot;&quot;</div>
					</td>
					<td>Parameter L<font size="1"><sub>b</sub></font> for absorption length as described 
above. If you use the first form, <b><code>nameAbsLb</code></b> is the name of the 
fit or interpolation of class <b><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html"><code>TSplineFit</code></a></b> 
giving L<font size="1"><sub>b</sub></font> as a function of wavelength. If you use 
the second form, L<font size="1"><sub>b</sub></font> = <b><code>absLb</code></b> 
does not depend upon wavelength<font size="4">.</font></td>
				</tr>
				<tr>
					<td><b><code>Text_t* &nbsp;or Double_t</code></b></td>
					<td><b><code>nameAbsLc or absLc</code></b></td>
					<td>
						<div align="center">
							&quot;&quot;</div>
					</td>
					<td>Parameter L<font size="1"><sub>c</sub></font> for absorption length as described 
above. If you use the first form, <b><code>nameAbsLc</code></b> is the name of the 
fit or interpolation of class <b><a href="http://gentit.home.cern.ch/gentit/litrani/AllModules/SplineFitCode/TSplineFit.html"><code>TSplineFit</code></a></b> 
giving L<font size="1"><sub>c</sub></font> as a function of wavelength. If you use 
the second form, L<font size="1"><sub>c</sub></font> = <b><code>absLc</code></b> 
does not depend upon wavelength<font size="4">.</font></td>
				</tr>
				<tr>
					<td><b><code>Double_t*</code></b></td>
					<td><b><code>params</code></b></td>
					<td>
						<div align="center">
							0</div>
					</td>
					<td><font color="red">Same argument as in <b><code>TGeoMedium</code></b>.</font> 
List of parameters allowed by <b><code>TGeoMedium</code></b>.</td>
				</tr>
			</table>
		</center>
		<p>A typical, but a little bit pedantic [since we insist to give properly A, Z 
and rho], definition for an optical medium looks like :</p>
		<ul>
			<li><code><b>const Double_t PbWO4_radlen   = 0.893;  //radiation length of PbWO4</b></code>
			<div align="left">
				<li><code><b>const Double_t PbWO4_intlen   = 19.5;   //interaction length of 
PbWO4</b></code>
				<li><code><b>TGeoElementTable *table = gGeoManager-&gt;GetElementTable();</b></code>
				<li><code><b>TGeoElement *Pb = table-&gt;FindElement(&quot;LEAD&quot;);</b></code>
				<li><code><b>TGeoElement *W = table-&gt;FindElement(&quot;TUNGSTEN&quot;);</b></code>
				<li><code><b>TGeoElement *O = table-&gt;FindElement(&quot;OXYGEN&quot;);</b></code>
				<li><code><b>TGeoMixture *PbWO4 = new TGeoMixture(&quot;PbWO4&quot;,3,8.28);</b></code>
				<li><code><b>PbWO4-&gt;SetFillColor(PbWO4Color);</b></code>
				<li><code><b>PbWO4-&gt;DefineElement(0,Pb,0.4553);</b></code>
				<li><code><b>PbWO4-&gt;DefineElement(1, W,0.4040);</b></code>
				<li><code><b>PbWO4-&gt;DefineElement(2, O,0.1407);</b></code>
				<li><code><b>PbWO4-&gt;SetRadLen(-PbWO4_radlen,PbWO4_intlen);</b></code>
				<li><code><b>TLitMedium *pbwo4 = new TLitMedium(&quot;PbWO4&quot;,3,PbWO4,kFALSE,1.0, 
&quot;AbsLengthTensor_PbWO4__000&quot;, &quot;AbsLengthTensor_PbWO4__001&quot;);</b></code>
				<li><code><b>pbwo4-&gt;IsUniAxialNegBirefr(&quot;DielTensor_PbWO4__000&quot;,&quot;DielTensor_PbWO4__002&quot;);</b></code>
				<li><b><code>pbwo4-&gt;FindSpectrum(&quot;Spectrum_PbWO4La&quot;);</code></b>
			</div>
		</ul>
		<div align="justify">
			<p></p>
		</div>
		<div align="center">
			<h2></h2>
		</div>
		<div align="center">
			<h2><a name="wrappings"></a>Wrappings</h2>
		</div>
		<div align="left">
			<p>If you intend to define a wrapping, you have to use the 10th or 11th constructor.&nbsp;For 
a wrapping, the value of mu (magnetic permeability) given in the constructor is the 
magnetic permeability of the <b>slice of medium</b> (for instance air) between the 
wrapping and the contiguous shape,<b><font color="red"> not</font></b> the magnetic 
permeability of the wrapping! The other arguments of the constructor (<b><code>mat, 
params</code></b>), those transfered to the parent class <b><code>TGeoMedium</code></b>. 
The <b>slice of medium</b> in-between is considered very thin (we neglect its thickness), 
<b>so that we do not need its absorption length</b>. It is why absorption length 
has disappeared from the constructor.&nbsp;But we need its index of refraction, so 
that a call to <b><code>IsIsotropic()</code></b> is required. This <b>slice of medium</b> 
<b><font color="red">must not</font></b> be anisotropic. Notice that you are not 
obliged to have a <b>slice of medium</b> in-between. If you give &quot;&quot; as 
name of the fit for mu, or -1 as the value of mu, you will have no slice. The characteristics 
specific to the wrapping (like complex index of refraction, magnetic permeability, 
supplementary absorption,...) must be given given by a call to <b><code>SetAsWrapping()</code></b>. 
Notice the following limitation of <b><code><font size="+1">SLitrani</font></code></b>: 
<i>it is forbidden to have a wrapping without slice in contact with a <b><code>TLitMedium</code></b> 
which is anisotropic, except in the case where the wrapping is defined as totally 
absorbing, i.e. the 5th argument of <b><code>SetAsWrapping()</code></b>, <code><b>supplAbs</b></code>, 
is set to 1.0.</i></p>
		</div>
		<p></p>
		<div align="center">
			<table border="4" cellpadding="8" cellspacing="4" bgcolor="#ccffcc">
				<caption>
					<h3>Argument of the constructor of TLitMedium <font color="red">in case of wrapping</font></h3>
				</caption>
				<tr>
					<td><b>Type</b></td>
					<td><b>Name</b></td>
					<td><b>Default</b></td>
					<td>
						<div align="center">
							<b>Description</b></div>
					</td>
				</tr>
				<tr>
					<td>const char *</td>
					<td>name</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td><font color="red">Same argument as in TGeoMedium.</font> Name of the optical 
medium.</td>
				</tr>
				<tr>
					<td>Int_t</td>
					<td>numed</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td><font color="red">Same argument as in TGeoMedium.</font> A not so nice feature 
of TGeoMedium is that the user has to assign to each medium a medium index.&nbsp;We 
are obliged to keep to this convention.</td>
				</tr>
				<tr>
					<td>TGeoMaterial* or TGeoMixture*</td>
					<td>mat</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td><font color="red">Same argument as in TGeoMedium.</font> A, Z and rho values 
of the TGeoMaterial of the wrapping are not used by SLitrani for the time being.</td>
				</tr>
				<tr>
					<td>Text_t * or Double_t</td>
					<td>nameMu or mu</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>Magnetic permeability of the slice of medium (for instance air) between 
wrapping shape and contiguous shape. It is <b><font color="red">not</font></b> the 
magnetic permeability of the wrapping!</td>
				</tr>
				<tr>
					<td>Double_t *</td>
					<td>params</td>
					<td>
						<div align="center">
							0</div>
					</td>
					<td><font color="red">Same argument as in TGeoMedium.</font> List of parameters 
allowed by TGeoMedium.</td>
				</tr>
			</table>
		</div>
		<p></p>
		<div align="center">
			<table border="4" cellpadding="8" cellspacing="4" bgcolor="#ccffcc">
				<caption>
					<h3>Arguments of SetAsWrapping()</h3>
				</caption>
				<tr>
					<td>Type</td>
					<td>Name</td>
					<td>Default</td>
					<td>Description</td>
				</tr>
				<tr>
					<td>Text_t* or Double_t</td>
					<td>nameDiffus or diffus</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>Proportion [0..1] of photons which are diffused instead of reflected. All 
non-absorbed light which is not diffused is assumed to be reflected. This proportion 
of light diffused may itself be dependent upon the wavelength : in that case, nameDiffus 
is the name of the fit which gives this dependency, otherwise, diffus is the fixed 
value of this proportion.</td>
				</tr>
				<tr>
					<td>Text_t* or Double_t</td>
					<td>nameRindex or Rindex</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>Real part of the index of refraction of the wrapping. Here also, may be 
dependent or not upon wavelength, at will. It is the real part of the index of refraction 
which allow absorption of the wave. The bigger this real part, the greater the absorption.</td>
				</tr>
				<tr>
					<td>Text_t* or Double_t</td>
					<td>nameIindex or Iindex</td>
					<td>
						<div align="center">
							-</div>
					</td>
					<td>Imaginary part of the index of refraction of the wrapping.Notice that an 
imaginary part of an index of refraction must always be negative to have absorption 
( and not explosion !) of the wave. But, as the &quot;Handbook of optical constants&quot; 
gives it positive, you are requested to give it also positive, SLitrani will change 
its sign. Here also, may be dependent or not upon wavelength, at will.</td>
				</tr>
				<tr>
					<td>Text_t* or Double_t</td>
					<td>nameMu or mu</td>
					<td>
						<div align="center">
							1.0</div>
					</td>
					<td>Magnetic permeability <font face="Symbol"><b>m</b></font> of the wrapping. 
Here also, may be dependent or not upon wavelength, at will. If you do not give this 
parameter, the magnetic permeability <font face="Symbol"><b>m</b></font> will be 
set equal to 1 and constant. As the &quot;Handbook of optical constants of medium&quot; 
never quotes the magnetic permeability of a wrapping, I suppose that it is considered 
as always equal to 1. But for the sake of completeness, it is left free by litrani. 
After all, the formula for reflection and refraction do depend upon <font face="Symbol"><b>m</b></font> 
!</td>
				</tr>
				<tr>
					<td>Double_t</td>
					<td>supplAbs</td>
					<td>
						<div align="center">
							0.0</div>
					</td>
					<td>Possibility of a supplementary absorption. Default to 0.0, normal value, 
because the absorption by the revetment is calculated from the Fresnel formula with 
the complex index of refraction ( given by Rindex and Iindex ). If you give it a 
non-zero value, say 0.1, you will add by that a supplementary probability (10%) of 
absorption. It may be an handy way of studiing the effect of a revetment getting 
worse and worse. This parameter is also useful if you want to define a revetment 
which is 100% absorbing. In that case, put it to 1.0. You will spare by that the 
calculation of the Fresnel equations to the program. Notice that the photons which 
are not absorbed by Fresnel but are absorbed because of this supplementary, artificially 
added, absorption are said &quot;absorbed before revetment&quot; by SLitrani.</td>
				</tr>
				<tr>
					<td>Double_t</td>
					<td>ThetaMax</td>
					<td>
						<div align="center">
							90.0</div>
					</td>
					<td>Maximum theta angle for the diffused photon. By putting a value smaller 
than 90 degree, you avoid that diffused photons recoil almost parallel with the diffusing 
surface. Look at the part of the class description of TLitMedium concerning wrappings 
to learn why it may be necessary to limit the angle of recoil of the diffused photon 
to values less than 90&deg;.</td>
				</tr>
			</table>
		</div>
		<p></p>
		<div align="center">
			<h3>How diffusion is generated for a wrapping</h3>
		</div>
		<div align="left">
			<p>In <b><code>SLitrani</code></b>, diffusion on a wrapping is treated exactly 
in the same way as reflection, except that the reflection is done on a plane of &quot;any&quot; 
orientation. More precisely, the orientation of the reflecting plane is chosen in 
such a way that the reflected photon has a distribution of reemission which is :</p>
		</div>
		<div align="center">
			<p><code><b><font size="+1">cos(</font><font face="Symbol" size="+1">q</font><font size="+1">)sin(</font><font face="Symbol" size="+1">q</font><font size="+1">) 
d</font><font face="Symbol" size="+1">q</font><font size="+1"> d</font><font face="Symbol" size="+1">f</font><font size="+1"> 
    [ 0 &lt;= </font><font face="Symbol" size="+1">q</font><font size="+1"> &lt;= 
</font><font face="Symbol" size="+1">q</font><sub>max</sub><font size="+1"> ]</font></b></code></p>
		</div>
		<div align="left">
			<p>There is a supplementary factor <code><b>cos(<font face="Symbol">q</font>)</b></code> 
with respect to a totally isotropic distribution <code><b>sin(<font face="Symbol">q</font>)d<font face="Symbol">q 
</font>d<font face="Symbol">f</font></b></code>  in order to slightly favour the 
direction normal to the diffusing surface. By default <code><b><font face="Symbol">q</font><sub>max</sub></b></code> 
is 90&deg;. You cannot choose it bigger than 90&deg;, it would be meaningless. You 
can choose it smaller if you want to avoid that the diffused photon be almost tangent 
to the plane of the diffusing revetment. There is a good reason to choose it smaller 
than 90&deg;  in case of a diffusing revetment covering, without any slice of intermediate 
medium, a face of a shape made of an anisotropic medium : see tricky point c). Once 
one knows what the direction of the diffused photon will be, one calculates the orientation 
of a pseudo plane of reflection in such a way that the photon <i>reflected</i> on 
this pseudo plane of reflection has the wanted direction for the <i>diffused</i> 
photon. One does that assuming that the angle of reflection is equal to the incident 
angle, which is true only for isotropic medium. This is the origin of the 3rd tricky 
point, see below.</p>
		</div>
		<p></p>
		<div align="center">
			<h3>Tricky points about wrappings</h3>
		</div>
		<div align="left">
			<h4>a) - Totally absorbing revetment</h4>
			<p>Suppose you want that a revetment absorbs a photon each time the photon reaches 
a face of the covered medium. For instance, when a photon is inside the Si part of 
an APD and reaches the edge surface of the Si, you do not want that the photon be 
reflected or diffused, you want it to be considered as lost. But the possibility 
remains that the photon makes a total reflection on the slice of medium between Si 
and revetment! In <b><code>SLitrani</code></b>, we have decided to avoid this difficulty 
by imposing the following : if the user declares a supplementary absorption of 1.0, 
we decide that he wants the photon to be absorbed aniway, so that we cancel the possibility 
of total reflection on the slice of medium between face and revetment. Keep that 
in mind ! There is a big difference between supplAbs=1.0 and supplAbs=0.99 ! With 
supplAbs=1.0, no total reflection possible, with supplAbs=0.99, total reflection 
possible ! What to do if you want the (rather strange) possibility that the supplementary 
absorption be for instance 0.9 and that no total reflection can happen ? In that 
case, declare &quot;none&quot; as the medium of the slice when you define the revetment 
!</p>
			<h4>b) - Finding real and imaginary parts of index</h4>
			<p>In the way diffusion is calculated [ look at &quot;How diffusion is generated&quot;, 
above, and look at the method <b><code>TLitPhoton::Diffused()</code></b> ] it is 
tacitly assumed that you know real and imaginary part of the index of refraction 
of the diffusing revetment. It is easy to find such indexes for reflecting surfaces 
in the Handbook of Optical Constants, but not for diffusing surfaces. For diffusing 
surfaces, it is more current to find the albedo of the surface. But it is not at 
all evident to go from the albedo to the real and imaginary part of the index of 
refraction! There is a problem here that I do not know how to solve. If you get an 
idea, please tell it to me. On the contrary, you can go from the real and imaginary 
part of the index to the albedo by running <b><code>SLitrani</code></b> and looking 
at the proportions of photons absorbed by the revetment. As an example, the way the 
diffusing revetment &quot;tyvek&quot; has been introduced into <b><code>SLitrani</code></b> 
reflects the difficulty quoted here : I was not able to find real and imaginary part 
of the index of tyvek, but I found the albedo. What I have done was first to give 
to tyvek the real and imaginary part of aluminium. Then I realized that it gave a 
worse albedo than the one quoted for tyvek. So I have decreased the real part of 
the index of aluminium until it gave the proper albedo for tyvek. I completly agree 
that this way of handling diffusing surfaces is tedious, but I have not yet found 
a better one.</p>
			<h4>c) - Shape with anisotropic medium and diffusing revetment without slice of 
intermediate medium</h4>
			<p>In the way diffusion is calculated [ look at &quot;How diffusion is generated&quot;, 
above, and look at the method <b><code>TLitPhotonn::Diffused()</code></b> ] it is 
tacitly assumed that the angle of reflection is equal to the incident angle, which 
is not true for anisotropic medium. In consequence, in anisotropic medium, if the 
generated value for the angle <b><font face="Symbol">q</font></b> happens to be very 
close to 90&deg;, you can end up with a diffused photon which is not going backward 
after having been diffused by the revetment !!! This places <b><code>SLitrani</code></b> 
into a meaningless situation. You will encounter the following message : &quot;No 
intersection with any face !&quot; when <b><code>SLitrani</code></b>searches what 
will be the next face encountered by the photon ! There are 2 ways out of this problem 
:</p>
			<ul>
				<li>Either do not declare &quot;none&quot; for the 3rd par. nameMat when the 
revetment is diffusing and the medium of the shape covered by the revetment is anisotropic. 
If your intention was to have no total reflection (it was the reason why you intended 
to declare nameMat as &quot;none&quot;) declare for nameMat an isotropic medium whose 
index is slightly bigger than the biggest of the 2 indices of the anisotropic medium, 
so that total reflection will be rare.
				<li>Or take a <code><b><font face="Symbol">q</font><sub>max</sub></b></code> 
sufficiently smaller than 90&deg; in such a way that the problem never appears.
			</ul>
			<p></p>
		</div>
		<p></p>
		<div align="center">
			<h2><a name="thinslice"></a>Thin Slices</h2>
		</div>
		<p></p>
		<div align="left">
			<p>The existence of a thin slice between 2 media can change the optical properties 
dramatically. The most important case where one meets a thin slice is for the entrance 
window of an <b><code>APD</code></b>, which is covered by a thin slice which greatly 
enhances the light collection. <b><code>litrani</code></b> allows the insertion of 
thin slices between 2 <b><code>TLitMedium</code></b>, in case the 2 <b><code>TLitMedium</code></b> 
are inside volumes which are contiguous. This is done by a call to the method <b><code><a href="#TLitMedium:WithThinSlice">TLitMedium::WithThinSlice()</a></code></b>.&nbsp;Look 
at this method.</p>
			<p>If <b><code>TLitMedium &quot;med1&quot;</code></b> and <b><code>TLitMedium 
&quot;med2&quot;</code></b> are contiguous and if there is a thin slice between the 
two, it is not necessary to call <b><code><a href="#TLitMedium:WithThinSlice">TLitMedium::WithThinSlice()</a></code></b> 
twice, once for <b><code>&quot;med1&quot;</code></b> and once for <b><code>&quot;med2&quot;</code></b>. 
Calling it once, for <b><code>&quot;med1&quot;</code></b> <font color="#ce0000">or</font> 
for <b><code>&quot;med2&quot;</code></b> is enough.</p>
			<p>Notice that you are not permitted to introduce a thin slice between 2 TGeoVolume, 
one or both of which being made of an anisotropic material. In fact, this is not 
really a limitation, since for instance concerning the thin slice of the window of 
the <b><code>APD</code></b>, it appears between for instance glue and silicium, both 
of which being isotropic.</p>
			<p>In a<b> thin slice</b>, in the optical sense, the normal Fresnel equations 
do not apply but are to be replaced by the specific equations for thin slices, where 
the depth of the thin slice, which is of the order of the wavelength, plays a role.</p>
			<p>Notice the following concerning thinslices.&nbsp;If you have called <b><code>med1-&gt;WithThinSlice(med2,...)</code></b>, 
then each time a <b><code>TGeoVolume</code></b> made of <b><code>med1</code></b> 
is in contact with a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
made of <b><code>med2</code></b>, there will be a thin slice between the two. What 
to do if in your setup, there are sometimes thin slices between <b><code>med1</code></b> 
and <b><code>med2</code></b> and sometimes not? The solution is to declare a third 
<b><code>TLitMedium</code></b>, <b><code>med3</code></b>, identical with <b><code>med2</code></b> 
[except for its name] and to not call <b><code>WithThinSlice</code></b> for <b><code>med3</code></b>.</p>
			<p></p>
		</div>
		<div align="center">
			<h2><a name="depolish"></a>Depolished face</h2>
		</div>
		<p></p>
		<div align="left">
			<p>Suppose that you want that each time a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
1 made of <b><code>TlitMedium</code></b> <b><code>med1</code></b> [<b><code>med1</code></b> 
<i>being not</i> a wrapping] is in contact with an other <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
2 made of <b><code>TlitMedium</code></b> <b><code>med2</code></b> [<b><code>med2</code></b> 
<i>being</i> a wrapping], then the face of <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
1 in contact with <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
2<b><i> is considered as being depolished</i></b>, you can do that by calling method 
<b><code><a href="#TLitMedium:SetDepolished">TLitMedium::SetDepolished()</a></code></b>. 
Look at this method.&nbsp;Here again, if in your setup, sometimes this contact is 
with depolish, sometimes not, use the trick of defining a third <b><code>TlitMedium</code></b> 
<b><code>med3</code></b>, identical with <b><code>med2</code></b> except for the 
name.</p>
END_HTML */
//
Double_t   TLitMedium::fgMinimalGap    = 1.0e-6;
Double_t   TLitMedium::fgOldWavelength = 0.0;
Double_t   TLitMedium::fgMinWaveL      = 300.0;
Double_t   TLitMedium::fgMaxWaveL      = 1000.0;
Double_t   TLitMedium::fgCerWaveL      = 450.0;
TObjArray *TLitMedium::fgLitMedium     = 0;

TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,Bool_t sensible, const char *nameMu, const char *nameAbsLa,const char *nameAbsLb,const char *nameAbsLc,Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // Material referred to by pointer
  // Absorption length VARIABLE and magnetic permeability VARIABLE
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   nameMu    name of the fit of type SplineFit giving the magnetic permeability mu of
  //              the tracking medium as a function of wavelength.
  //   nameAbsLa name of the fit of type SplineFit giving La as a function of wavelength.
  //   nameAbsLb name of the fit of type SplineFit giving Lb as a function of wavelength.
  //   nameAbsLc name of the fit of type SplineFit giving Lc as a function of wavelength.
  //   params    array of possible additional parameters
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only nameAbsLa
  //has to be given. nameAbsLb/c are equal to "".
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t un = 1.0;
  TString s;
  TString snone = "";
  Init();
  fKind = OpticMedium;
  fSensible = sensible;
  Bool_t ok = kTRUE;
  const char *met = "TLitMedium";
  //magnetic permeability will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) {
    fNameMu = nameMu;
    fMuFound = kTRUE;
  }
  else {
    fMuFound = kFALSE;
    gCleanOut->MM(error,met,"Fit for magnetic permeability not found",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","nameMu",nameMu,ClassName());
    return;
  }
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for fAbsLa
  }
  else {
    ok = kFALSE;
    gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fAbsLa",nameAbsLa,ClassName());
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before material",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fAbsLb",nameAbsLb,ClassName());
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before material",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fAbsLc",nameAbsLc,ClassName());
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,Bool_t sensible, const char *nameMu, Double_t absLa,Double_t absLb,Double_t absLc,Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // Material referred to by pointer
  // Absorption length FIXED and magnetic permeability VARIABLE
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   nameMu    name of the fit of type SplineFit giving the magnetic permeability mu of
  //              the tracking medium as a function of wavelength.
  //   absLa     fixed value of absorption length La
  //   absLb     fixed value of absorption length Lb
  //   absLc     fixed value of absorption length Lc
  //   params    array of possible additional parameters
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only absLa
  //has to be given. absLb/c are equal to -1.
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t zero = 0.0;
  TString s;
  Init();
  fKind = OpticMedium;
  fSensible = sensible;
  Bool_t ok = kTRUE;
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) {
    fNameMu = nameMu;
    fMuFound = kTRUE;
  }
  else {
    ok = kFALSE;
    fMuFound = kFALSE;
    const char *met = "TLitMedium";
    gCleanOut->MM(error,met,"Fit for magnetic permeability not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for magnetic permeability before medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameMu",nameMu,ClassName());
    return;
  }
  //absorption length will not vary with wavelength
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,Bool_t sensible, Double_t mu, const char *nameAbsLa,const char *nameAbsLb,const char *nameAbsLc,Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // Material referred to by pointer
  // Absorption length VARIABLE and magnetic permeability FIXED
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   mu        fixed value of the magnetic permeability of the tracking medium
  //   nameAbsLa name of the fit of type SplineFit giving La as a function of wavelength.
  //   nameAbsLb name of the fit of type SplineFit giving Lb as a function of wavelength.
  //   nameAbsLc name of the fit of type SplineFit giving Lc as a function of wavelength.
  //   params    array of possible additional parameters
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only nameAbsLa
  //has to be given. nameAbsLb/c are equal to "".
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t un = 1.0;
  TString s;
  TString snone="";
  Init();
  fKind = OpticMedium;
  const char *met = "TLitMedium";
  fSensible = sensible;
  fMu = TMath::Abs(mu);
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  Bool_t ok = kTRUE;
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for Abs. length
  }
  else {
    ok = kFALSE;
    gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLa",nameAbsLa,ClassName());
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLb",nameAbsLb,ClassName());
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLc",nameAbsLc,ClassName());
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,Bool_t sensible, Double_t mu, Double_t absLa,Double_t absLb,Double_t absLc,Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // Material referred to by pointer
  // Absorption length FIXED and magnetic permeability FIXED
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   mu        fixed value of the magnetic permeability of the tracking medium
  //   absLa     fixed value of absorption length La
  //   absLb     fixed value of absorption length Lb
  //   absLc     fixed value of absorption length Lc
  //   params    array of possible additional parameters
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only absLa
  //has to be given. absLb/c are equal to -1.
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t zero = 0.0;
  Init();
  fKind = OpticMedium;
  fSensible = sensible;
  //magnetic permeability will not vary with wavelength : fixed at mu
  fMu = TMath::Abs(mu);
  //absorption length will not vary with wavelength : fixed at absL
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, Int_t imat,
                       Bool_t sensible, const char *nameMu, const char *nameAbsLa, const char *nameAbsLb,
                       const char *nameAbsLc,
                       Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
                       Double_t deemax, Double_t epsil, Double_t stmin):
TGeoMedium(name,numed,imat,isvol,ifield,fieldm,tmaxfd,stemax,deemax,epsil,stmin) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // Material referred to by number ID
  // 8 first values of fParam given, but not used by SLitrani.
  // Absorption length VARIABLE and magnetic permeability VARIABLE
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   imat      unique ID of the material
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   nameMu    name of the fit of type SplineFit giving the magnetic permeability mu of
  //              the tracking medium as a function of wavelength.
  //   nameAbsLa name of the fit of type SplineFit giving La as a function of wavelength.
  //   nameAbsLb name of the fit of type SplineFit giving Lb as a function of wavelength.
  //   nameAbsLc name of the fit of type SplineFit giving Lc as a function of wavelength.
  //   isvol     = fParam[0] Sensitive volume flag
  //   ifield    = fParam[1] Magnetic field
  //   fieldm    = fParam[2] Max. field value (Kilogauss)
  //   tmaxfd    = fParam[3] Max. angle due to field (deg/step)
  //   stemax    = fParam[4] Max. step allowed
  //   deemax    = fParam[5] Max. fraction of energy lost in a step
  //   epsil     = fParam[6] Tracking precision (cm)
  //   stmin     = fParam[7] Min. step due to continuos processes (cm)
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only nameAbsLa
  //has to be given. nameAbsLb/c are equal to "".
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t un = 1.0;
  TString s;
  TString snone = "";
  Init();
  fKind = OpticMedium;
  const char *met = "TLitMedium";
  fSensible = sensible;
  Bool_t ok = kTRUE;
  //magnetic permeability will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) {
    fNameMu = nameMu;
    fMuFound = kTRUE;
  }
  else {
    fMuFound = kFALSE;
    gCleanOut->MM(error,met,"Fit for magnetic permeability not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for magnetic permeability before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameMu",nameMu,ClassName());
    return;
  }
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for fAbsLa
  }
  else {
    ok = kFALSE;
    gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLa",nameAbsLa,ClassName());
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLb",nameAbsLb,ClassName());
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLc",nameAbsLc,ClassName());
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, Int_t imat,
                       Bool_t sensible, const char *nameMu,  Double_t absLa,Double_t absLb,Double_t absLc,
                       Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
                       Double_t deemax, Double_t epsil, Double_t stmin):
TGeoMedium(name,numed,imat,isvol,ifield,fieldm,tmaxfd,stemax,deemax,epsil,stmin) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // 8 first values of fParam given, but not used by SLitrani.
  // Material referred to by number ID
  // Absorption length FIXED and magnetic permeability VARIABLE
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   imat      unique ID of the material
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   nameMu    name of the fit of type SplineFit giving the magnetic permeability mu of
  //              the tracking medium as a function of wavelength.
  //   absLa     fixed value of absorption length La
  //   absLb     fixed value of absorption length Lb
  //   absLc     fixed value of absorption length Lc
  //   isvol     = fParam[0] Sensitive volume flag
  //   ifield    = fParam[1] Magnetic field
  //   fieldm    = fParam[2] Max. field value (Kilogauss)
  //   tmaxfd    = fParam[3] Max. angle due to field (deg/step)
  //   stemax    = fParam[4] Max. step allowed
  //   deemax    = fParam[5] Max. fraction of energy lost in a step
  //   epsil     = fParam[6] Tracking precision (cm)
  //   stmin     = fParam[7] Min. step due to continuos processes (cm)
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only absLa
  //has to be given. absLb/c are equal to -1.
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t zero = 0.0;
  TString s;
  Init();
  fKind = OpticMedium;
  fSensible = sensible;
  Bool_t ok = kTRUE;
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) {
    fNameMu = nameMu;
    fMuFound = kTRUE;
  }
  else {
    ok = kFALSE;
    fMuFound = kFALSE;
    const char *met = "TLitMedium";
    gCleanOut->MM(error,met,"Fit for magnetic permeability not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for magnetic permeability before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameMu",nameMu,ClassName());
    return;
  }
  //absorption length will not vary with wavelength
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, Int_t imat,
                       Bool_t sensible, Double_t mu, const char *nameAbsLa, const char *nameAbsLb, const char *nameAbsLc,
                       Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
                       Double_t deemax, Double_t epsil, Double_t stmin):
TGeoMedium(name,numed,imat,isvol,ifield,fieldm,tmaxfd,stemax,deemax,epsil,stmin) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // 8 first values of fParam given, but not used by SLitrani.
  // Material referred to by number ID
  // Absorption length VARIABLE and magnetic permeability FIXED
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   mu        fixed value of the magnetic permeability of the tracking medium
  //   nameAbsLa name of the fit of type SplineFit giving La as a function of wavelength.
  //   nameAbsLb name of the fit of type SplineFit giving Lb as a function of wavelength.
  //   nameAbsLc name of the fit of type SplineFit giving Lc as a function of wavelength.
  //   isvol     = fParam[0] Sensitive volume flag
  //   ifield    = fParam[1] Magnetic field
  //   fieldm    = fParam[2] Max. field value (Kilogauss)
  //   tmaxfd    = fParam[3] Max. angle due to field (deg/step)
  //   stemax    = fParam[4] Max. step allowed
  //   deemax    = fParam[5] Max. fraction of energy lost in a step
  //   epsil     = fParam[6] Tracking precision (cm)
  //   stmin     = fParam[7] Min. step due to continuos processes (cm)
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only nameAbsLa
  //has to be given. nameAbsLb/c are equal to "".
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t un = 1.0;
  TString s;
  TString snone="";
  Init();
  fKind = OpticMedium;
  const char *met = "TLitMedium";
  fSensible = sensible;
  fMu = TMath::Abs(mu);
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  Bool_t ok = kTRUE;
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for Abs. length
  }
  else {
    ok = kFALSE;
    gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLa",nameAbsLa,ClassName());
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLb",nameAbsLb,ClassName());
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gCleanOut->MM(error,met,"Fit for absorption length not found",ClassName());
      gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","fNameAbsLc",nameAbsLc,ClassName());
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok) AddMe();
}  
TLitMedium::TLitMedium(const char *name, Int_t numed, Int_t imat,
                       Bool_t sensible, Double_t mu,  Double_t absLa,Double_t absLb,Double_t absLc,
                       Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
                       Double_t deemax, Double_t epsil, Double_t stmin):
TGeoMedium(name,numed,imat,isvol,ifield,fieldm,tmaxfd,stemax,deemax,epsil,stmin) {
  //
  // Constructor of a normal medium, with real index of refraction, i.e. not a wrapping.
  //
  // 8 first values of fParam given, but not used by SLitrani.
  // Material referred to by number ID
  // Absorption length FIXED and magnetic permeability FIXED
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   sensible  Decides wheter the medium is a detector. For instance, if you define
  //              a medium "Silicium", intended to be the sensitive part of an APD, then
  //              sensible is true 
  //   mu        fixed value of the magnetic permeability of the tracking medium
  //   absLa     fixed value of absorption length La
  //   absLb     fixed value of absorption length Lb
  //   absLc     fixed value of absorption length Lc
  //   isvol     = fParam[0] Sensitive volume flag
  //   ifield    = fParam[1] Magnetic field
  //   fieldm    = fParam[2] Max. field value (Kilogauss)
  //   tmaxfd    = fParam[3] Max. angle due to field (deg/step)
  //   stemax    = fParam[4] Max. step allowed
  //   deemax    = fParam[5] Max. fraction of energy lost in a step
  //   epsil     = fParam[6] Tracking precision (cm)
  //   stmin     = fParam[7] Min. step due to continuos processes (cm)
  //
  // If the medium is isotropic with respect to absorption length, La=Lb=Lc, only absLa
  //has to be given. absLb/c are equal to -1.
  // This constructor does not provide the tracking medium with an index of refraction or
  //a dielectric tensor. you must call IsIsotropic() or IsBiref() acoording to case where
  //the medium is optically isotropic or anisotropic.
  //
  const Double_t zero = 0.0;
  Init();
  fKind = OpticMedium;
  fSensible = sensible;
  //magnetic permeability will not vary with wavelength : fixed at mu
  fMu = TMath::Abs(mu);
  //absorption length will not vary with wavelength : fixed at absL
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  AddMe();
}
//
// Constructors for wrappings media
//
TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,const char *nameMu, Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  //  Constructor of a WRAPPING medium, with magnetic permeability depending upon
  // wavelength
  //
  // Arguments:
  //
  //   name      name of the wrapping medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   nameMu    name of the fit of type SplineFit giving the value of mu (magnetic
  //              permeability) of the slice of material between this wrapping
  //              medium and the medium being wrapped. Here mu depends upon wavelength.
  //              Be careful, this does not concern the magnetic permeability of the
  //              wrapping! The magnetic permeability of the wrapping will be given
  //              when you call SetAsWrapping!
  //   params    array of possible additional parameters
  //
  //  Constructor to be used ONLY if you intend to define the medium of a wrapping,
  // i.e. a medium with complex index of refraction, in which the photons cannot
  // travel, but against which photon may be absorbed, diffused or reflected.
  //  In this new version of SLitrani, wrappings occupies a volume, not a surface
  // as in the old Litrani.
  //  In general, when a shape is wrapped with a wrapping, there is a slice of material
  // (for instance air) between the shape and the wrapping. This slice is so thin
  // that it is not necessary to provide it with an absorption length, nor to define
  // its width. But it MUST have an index of refraction, i.e. you MUST call IsIsotropic()
  // later on. This call to IsIsotropic() will provide the slice of material in-between
  // (not the wrapping!) with a real index of refraction.
  //  The value of mu (magnetic permeability) given by the fit nameMu is the magnetic
  // permeability of the slice of material, NOT the magnetic permeability of the wrapping!
  // Complex index of refraction and magnetic permeability of the wrapping have to
  // be provided later by a call to TLitMedium::SetAsWrapping().
  //  On the contrary, mat and params refer to the wrapping, not to the slice of
  // material in-between.
  //  Notice that you are not obliged to have a slice of material in-between. If
  // you give "" for nameMu, or omit it, you will have no slice. But notice the following
  // limitation of SLitrani: it is FORBIDDEN to have a wrapping WITHOUT slice in contact
  // with a TLitMedium which is anisotropic, EXCEPT in the case where the wrapping is
  // defined as totally absorbing, i.e. the 5th argument of TLitMedium::SetAsWrapping(),
  // supplAbs, is set to 1.0.
  //
  TString snone = "";
  Init();
  fKind = OpticWrapping;
  fAnisotropic = 0;
  if (snone.CompareTo(nameMu)) {
    //magnetic permeability will vary with wavelength.
    //fit giving this variation must have been defined before
    fFitMu = TSplineFit::FindFit(nameMu,14);
    if (fFitMu) {
      fNameMu      = nameMu;
      fMuFound     = kTRUE;
      fWrapIsSlice = kTRUE;
    }
    else {
      const char *met = "TLitMedium";
      fMuFound = kFALSE;
      gCleanOut->MM(error,met,"Fit for magnetic permeability not found",ClassName());
      gCleanOut->MMT(fatal,met,"Not found:","nameMu",nameMu,ClassName());
      return;
    }
  }
  else fWrapIsSlice = kFALSE;
  SetDiffusionLength(-1.0);
  AddMe();
}
TLitMedium::TLitMedium(const char *name, Int_t numed, const TGeoMaterial *mat,Double_t mu,
  Double_t *params):TGeoMedium(name,numed,mat,params) {
  //
  //  Constructor of a WRAPPING medium, with magnetic permeability fixed!
  //
  // Arguments:
  //
  //   name      name of the tracking medium
  //   numed     user defined medium index
  //   mat       pointer to material or mixture
  //   mu        fixed value of the magnetic permeability) of the slice of material
  //             between this wrapping medium and the medium being wrapped. 
  //             Be careful, this does not concern the magnetic permeability of the
  //             wrapping! The magnetic permeability of the wrapping will be given
  //             when you call TLitMedium::SetAsWrapping!
  //   params    array of possible additional parameters
  //
  //  Constructor to be used ONLY if you intend to define the medium of a wrapping,
  // i.e. a medium with complex index of refraction, in which the photons cannot
  // travel, but against which photon may be absorbed, diffused or reflected.
  //  In this new version of SLitrani, wrappings occupies a volume, not a surface
  // as in the old Litrani.
  //  In general, when a shape is wrapped with a wrapping, there is a slice of material
  // (for instance air) between the shape and the wrapping. This slice is so thin
  // that it is not necessary to provide it with an absorption length, nor to define
  // its width. But it MUST have an index of refraction, i.e. you MUST call IsIsotropic()
  // later on. This call to IsIsotropic() will provide the slice of material in-between
  // (not the wrapping!) with a real index of refraction.
  //  The value of mu (magnetic permeability) given here is the magnetic permeability
  // of the slice of material in-between, NOT the magnetic permeability of the wrapping!
  // Complex index of refraction and magnetic permeability of the wrapping have to
  // be provided later by a call to SetAsWrapping().
  //  On the contrary, mat and params refer to the wrapping, not to the slice of
  // material in-between.
  //  Notice that you are not obliged to have a slice of material in-between. If
  // you give a negative value for mu, you will have no slice. But notice the following
  // limitation of SLitrani: it is FORBIDDEN to have a wrapping WITHOUT slice in contact
  // with a TLitMedium which is anisotropic, EXCEPT in the case where the wrapping is
  // defined as totally absorbing, i.e. the 5th argument of TLitMedium::SetAsWrapping(),
  // supplAbs, is set to 1.0.
  //
  const Double_t zero = 0.0;
  Init();
  fKind = OpticWrapping;
  fAnisotropic = 0;
  fMu = mu;
  fWrapIsSlice = (fMu>zero);
  SetDiffusionLength(-1.0);
  AddMe();
}
TLitMedium::~TLitMedium() {
  //destructor
  fgLitMedium->Remove(this);
  if (fIndexS) delete fIndexS;
  if (fEpsaS)  delete fEpsaS;
  if (fEpsbS)  delete fEpsbS;
  if (fEpscS)  delete fEpscS;
}
void TLitMedium::AddMe() {
  //  Add this medium to the list of TLitMedium. If a medium of the same name
  //was present in the list, it is killed.
  Int_t i,k,N,kmat,krev;
  TString s1,s2;
  TLitMedium *pm;
  TIter next(fgLitMedium);
  kmat = 0;
  krev = 0;
  N = fgLitMedium->GetEntries();
  if (N<=0) {
    if (fKind==OpticWrapping) fRevCounter = 1;
    else                      fMatCounter = 1;
    fgLitMedium->AddLast(this);
  }
  else {
    Bool_t found = kFALSE;
    s1 = GetName();
    i = 0;
    while ((pm = (TLitMedium*)next()) && (!found) && (i<N)) {
      s2 = pm->GetName();
      k = s1.CompareTo(s2);
      if (k==0) {
        fgLitMedium->Remove(pm);
        delete pm;
        found = kTRUE;
      }
      i++;
    }
    next.Reset();
    while ((pm = (TLitMedium*)next())) {
      if (pm->GetKind()==OpticWrapping) krev++;
      else                              kmat++;
    }
    if (fKind==OpticWrapping) fRevCounter = ++krev;
    else                      fMatCounter = ++kmat;
    fgLitMedium->AddLast(this);
  }
}
Bool_t TLitMedium::CheckIt() {
  //Checks that everything needed has been defined. Call this method to
  //be sure you have defined properly this optical medium. If you get no message,
  //everything is ok !
  if (fChecked) return fChecked;
  const char *met = "CheckIt";
  //absorption length
  if (fAbsLa<0.0) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Absorption Length fAbsLa not entered properly",ClassName());
    return fChecked;
  }
  if ((fAniAbsL) && (fAbsLb<0.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Absorption Length fAbsLb not entered properly",ClassName());
    return fChecked;
  }
  if ((fAniAbsL>1) && (fAbsLc<0.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Absorption Length fAbsLc not entered properly",ClassName());
    return fChecked;
  }
  if ((fAnisotropic==0) && (fAniAbsL!=0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Abs. Length cannot be anisotropic if diel tensor isotropic",ClassName());
    return fChecked;
  }
  if ((fAnisotropic<0) || (fAnisotropic>3)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Optical properties not defined",ClassName());
    gCleanOut->MM(error,met,"Call IsIsotropic or IsBirefr or IsUniA...",ClassName());
    return fChecked;
  }
  if ((fAnisotropic==0) && (fIndex<1.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Index of refraction smaller than 1",ClassName());
    return fChecked;
  }
  if (fMu<=0.0) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Magnetic permeability not entered properly",ClassName());
    return fChecked;
  }
  if ((fAnisotropic) && (fEpsa<=0.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Epsa of diel. tensor not defined properly",ClassName());
    return fChecked;
  }
  if ((fAnisotropic==3) && (fEpsb<=0.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Epsb of diel. tensor not defined properly",ClassName());
    return fChecked;
  }
  if ((fAnisotropic) && (fEpsc<=0.0)) {
    ProblemHeader();
    gCleanOut->MM(error,met,"Epsc of diel. tensor not defined properly",ClassName());
    return fChecked;
  }
  switch (fdEdxUsed) {
  case nodEdx:
    break;
  case WithoutLandau:
    if (!fDeposit) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Fit or interpolation for dEdx not found",ClassName());
      return fChecked;
    }
    break;
  case WithLandau:
    if (!fDeposit) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Family of fits for dE/dx not found",ClassName());
      return fChecked;
    }
    break;
  }
  if (fKind == OpticWrapping) {
    const Double_t plus05  =  0.5;
    const Double_t minus05 = -0.5;
    if (fWrapDiffus<minus05) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Fit for % of diffusion not found",ClassName());
      return fChecked;
    }
    if (fWrapRindex<minus05) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Fit for real part of index not found",ClassName());
      return fChecked;
    }
    if (fWrapIindex>plus05) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Fit for imaginary part of index not found",ClassName());
      return fChecked;
    }
    if (fWrapMu<minus05) {
      ProblemHeader();
      gCleanOut->MM(error,met,"Fit for magnetic permeability mu not found",ClassName());
      return fChecked;
    }
  }
  fChecked = kTRUE;
  return fChecked;
}
void TLitMedium::dEdx(const char *nameDist,Double_t photmev,Bool_t Landau) {
  //  Declares the optical medium as having a distribution dE/dx for the generation
  // of deposited energy and that photmev photons are generated per mev of
  // deposited energy.
  //  There are 2 possibilities :
  //    (1) - you want Landau fluctuations in the deposited energy. Then put
  // Landau to true and give for nameDist the name of the first element [ ending
  // in _000 ] of a family of fits of type TSplineFit. Each member (each fit) in
  // the family will then give a randomized value of dEdx distributed according
  // the Landau distribution valid for the energy range for which this fit is valid.
  // Look at the class description of the class TSplineFit for more informations.
  //  For example, in this family of fits, you can have one distribution for
  // muons between 0 and 10 Gev, an other for muons between 10 and 20 Gev, and
  // so on.
  //    (2) - you do not want Landau fluctuations in the deposited energy, but
  // you want that the deposited energy be fixed for a fixed energy of the par-
  // ticle. Then put Landau to false. The deposited energy, however, varies if
  // the energy of the particle varies, and this variation is described by a fit
  // or an interpolation of type TSplineFit whose name is nameDist, and which
  // does not belong to a family of fits.
  //
  const Double_t MinPhotMev=1.0e-4;
  TString s;
  const char *met = "dEdx";
  if (Landau) fdEdxUsed = WithLandau;
  else        fdEdxUsed = WithoutLandau;
  switch (fdEdxUsed) {
  case WithoutLandau:
    fDeposit = TSplineFit::FindFit(nameDist,11);
    if (fDeposit) fNamedEdx = nameDist;
    else {
      gCleanOut->MM(error,met,"Distribution for dE/dx not found",ClassName());
      gCleanOut->MM(error,met,"Please define distribution for dE/dx before calling dEdx",ClassName());
      gCleanOut->MMT(fatal,met,"Missing:","fNamedEdx",nameDist,ClassName());
      return;
    }
    break;
  case WithLandau:
    fDeposit = TSplineFit::FindFit(nameDist,11);
    if (fDeposit) {
      fNamedEdx = nameDist;
      fNbdEdx   = fDeposit->LoadFamily();
      // TSplineFit::fgFits->Sort();
      /*
      //Provi
      Bool_t ufr;
      ufr = fDeposit->GetUseForRandom();
      TSplineFit *fit1;
      fit1 = TSplineFit::FindFit("DEdx_MusInPbWO4__001");
      ufr = fit1->GetUseForRandom();
      cout << ufr << endl;
      //End Provi
      */
    }
    else {
      gCleanOut->MM(error,met,"Family distribution for dE/dx not found",ClassName());
      gCleanOut->MM(error,met,"Please define Family distribution for dE/dx before calling dEdx",ClassName());
      gCleanOut->MMT(fatal,met,"Missing:","fNamedEdx",nameDist,ClassName());
      return;
    }
    break;
  default:
    break;
  }
  photmev  = TMath::Abs(photmev);
  fPhotMev = photmev;
  if (photmev<MinPhotMev)
    gCleanOut->MM(warning,met,"your number of photons per mev is extraordinary small ?",ClassName());
}
void TLitMedium::DoWLS(Double_t &wavelength,Double_t &lifetime,Int_t &kfluo) {
// Generate the new wavelength of a photon which undergoes wavelength shifting.
//It is assumed that the emission spectrum is the same whatever the wavelength
//of the absorbed photon. Since there is often an overlap between the emission
//and absorption spectra, the routine checks that the photons are not shifted 
//up in energy by the WLS process.
// TLitMedium::DoWLS() is called by TLitPhoton in method TLitPhoton::Regenerate().
  Double_t oldwavelength = wavelength;
  Double_t lt = 0;
  // Condition to avoid that the energy of the photon increases with WLS
  while (!(wavelength>oldwavelength)) {
    kfluo = fWLSSpectrum->GenWaveLAndDecayT(wavelength,lt);}
  lifetime += 1000*lt;//Should this be in ps or ns?
  TLitMedium::NewWavelengthAll(wavelength);
  TLitVolume::NewWavelengthAll(wavelength);
}
void TLitMedium::FindIindex(const TString &s1) {
  //Finds the fit giving the imaginary part of index of refraction of wrapping
  const char *met = "FindIindex";
  if (fKind == OpticWrapping) {
    fWrapFitIindex = TSplineFit::FindFit(s1.Data(),4);
    if (fWrapFitIindex) fWrapNameIindex=s1;
    else {
      fWrapNameIindex="";
      gCleanOut->MMT(error,met,"Fit for imaginary part of index not found","s1",s1.Data(),ClassName());
      gCleanOut->MM(fatal,met,"Define this fit before defining the wrapping !",ClassName());
    }
  }
  else {
    gCleanOut->MMT(error,met,"Medium is not a wrapping","s1",s1.Data(),ClassName());
    gCleanOut->MM(error,met,"Trying to find im. index for a not-wrapping medium",ClassName());
    fWrapIindex = 0.0;
    fWrapNameIindex="";
  }
}
TLitMedium *TLitMedium::FindLitMedium(const char *name) {
  // Finds pointer to TLitMedium named "name"
  Bool_t found = kFALSE;
  TString lname;
  TLitMedium *p = 0;
  TLitMedium *q;
  TIter next(fgLitMedium);
  while ((!found) && (q = (TLitMedium*)next())) {
    lname = q->GetName();
    if (!lname.CompareTo(name)) {
      found = kTRUE;
      p = q;
    }
  }
  return p;
}
void TLitMedium::FindSpectrum(const char *namesp) {
// Look for a TLitSpectrum with name "namesp" first in the collection TLitSpectrum::fgSpectra,
//and if not found there, then in the "database" file of spectra, SpectraDB.rdb. Not
//finding the spectrum is a fatal error
  fLitSpectrum = TLitSpectrum::FindSpectrum(namesp);
  if (!fLitSpectrum) {
    gCleanOut->MM(killthis,"FindSpectrum","Specified optical spectrum not found",ClassName());
  }
}
void TLitMedium::FindRindex(const TString &s1) {
  //Finds the fit giving the real part of index of refraction of wrapping.
  const char *met = "FindRindex";
  if (fKind == OpticWrapping) {
    fWrapFitRindex = TSplineFit::FindFit(s1.Data(),3);
    if (fWrapFitRindex) fWrapNameRindex=s1;
    else {
      fWrapNameRindex="";
      gCleanOut->MMT(error,met,"Fit for real part of index not found","s1",s1.Data(),ClassName());
      gCleanOut->MM(fatal,met,"Define this fit before defining the wrapping !",ClassName());
    }
  }
  else {
    gCleanOut->MMT(error,met,"Medium is not a wrapping","s1",s1.Data(),ClassName());
    gCleanOut->MM(error,met,"Trying to find real index for a not-wrapping medium",ClassName());
    fWrapRindex = 0.0;
    fWrapNameRindex="";
  }
}
void TLitMedium::FindWrapDiffus(const TString &s1) {
  //Finds the fit or interpolation giving the % of diffusion versus
  //reflection in case of wrapping.
  const Double_t minus1 = -1.0;
  const char *met = "FindWrapDiffus";
  if (fKind == OpticWrapping) {
    fWrapFitDiffus = TSplineFit::FindFit(s1.Data(),9);
    if (fWrapFitDiffus) fWrapNameDiffus = s1;
    else {
      gCleanOut->MMT(error,met,"Fit for % of diffusion not found","s1",s1.Data(),ClassName());
      gCleanOut->MM(fatal,met,"Define this fit before defining the wrapping !",ClassName());
      return;
    }
  }
  else {
    gCleanOut->MMT(error,met,"Medium is not a wrapping","s1",s1.Data(),ClassName());
    gCleanOut->MM(error,met,"Trying to find % of diffusion for a not-wrapping medium",ClassName());
    fWrapFitDiffus = 0;
    fWrapDiffus    = minus1;
  }
}
void TLitMedium::FindWrapMu(const TString &s1) {
  //Finds the fit giving the magnetic permeability mu of wrapping.
  const char *met = "FindWrapMu";
  if (fKind == OpticWrapping) {
    fWrapFitMu = TSplineFit::FindFit(s1.Data(),14);
    if (fWrapFitMu) fWrapNameMu=s1;
    else {
      fWrapNameMu="";
      gCleanOut->MMT(error,met,"Fit for magnetic permeability mu not found","s1",s1.Data(),ClassName());
      gCleanOut->MM(fatal,met,"Define this fit before defining the wrapping !",ClassName());
    }
  }
  else {
    gCleanOut->MMT(error,met,"Medium is not a wrapping","s1",s1.Data(),ClassName());
    gCleanOut->MM(error,met,"Trying to find mu for a not-wrapping medium",ClassName());
    fWrapMu = 0.0;
    fWrapNameMu="";
  }
}
void TLitMedium::FluoComponent(Double_t wavel,Double_t stddev,Double_t weigth,Double_t lifetime) {
//
//  This method is obsolete. It has been kept only to be compatible with old CINT macros
//driving SLitrani. We advise you not to use it and to rather define the fluorescent
//characteristics of this material outside TLitMedium, as is detailed in the
//class description of the class TLitSpectrum, and then to link the created TLitSpectrum
//to this material, calling the method TLitMedium::FindSpectrum(const char *).
//
//  Add one component of fluorescence. You can add as many components of
//fluorescence as you like, by calling "FluoComponent" several times.
//
//  Arguments
//
//    wavel   - the mean value of the wavelength (in nanometers ) of this component.
//    stddev  - is the standard deviation ( in nanometers ) of the generated spectrum of
//              this component. The spectrum for one component is assumed gaussian.
//    weigth  - is the weigth of this component. You do not need to make sure that the
//              sum of all weigths of all components is 1 : the weigths will be
//              renormalized to 1 by the program.
//   lifetime - is the lifetime of this component in nanoseconds.
//
  const Double_t m1 = -1.0;
  TLitSpectrumCp *q;
  if (!fLitSpectrum) {
    TString s1 = "Spectrum_";
    TString s2 = "Emission Spectrum | ";
    s1.Append(GetName());
    s2.Append(GetTitle());
    fLitSpectrum = new TLitSpectrum(s1.Data(),s2.Data());
    fLitSpectrum->AddOneComponent(wavel,stddev,m1,lifetime,m1,weigth);
  }
  else {
    q = fLitSpectrum->FindComponent(wavel);
    if (q) fLitSpectrum->AddTimeComponent(q,lifetime,m1,weigth);
    else   fLitSpectrum->AddOneComponent(wavel,stddev,m1,lifetime,m1,weigth);
  }
}
Double_t TLitMedium::GetdEdx(Double_t E) const {
  //returns a value for dE/dx in Mev for a particle of energy E
  const Double_t zero = 0.0;
  Double_t dedx = zero;
  switch (fdEdxUsed) {
  case nodEdx:
    gCleanOut->MM(error,"GetdEdx","No dEdx defined for this optical medium",ClassName());
    break;
  case WithoutLandau:
    dedx = fDeposit->V(E);
    break;
  case WithLandau:
    dedx = fDeposit->GetRandom(fNbdEdx,E,TLit::Get()->GetTestCalc());
    break;
  }
  return dedx;
}
Short_t TLitMedium::GetFluorescent() const {
  // Returns number of gaussian components of fluorescence
  Short_t k = 0;
  if (fLitSpectrum) k = fLitSpectrum->GetNbOfComponents();
  return k;
}
Bool_t TLitMedium::GetIChangeWithWvl() const {
  //returns true if index or dielectric tensor changes with wavelength
  Bool_t b = kFALSE;
  switch (fIndexM) {
    case undefined:
    case constant:
      break;
    case byfit:
    case sellmeier:
      b = kTRUE;
      break;
  }
  if (!b) {
    switch (fEpsaM) {
      case undefined:
      case constant:
        break;
      case byfit:
      case sellmeier:
        b = kTRUE;
        break;
    }
  }
  if (!b) {
    switch (fEpsbM) {
      case undefined:
      case constant:
        break;
      case byfit:
      case sellmeier:
        b = kTRUE;
        break;
    }
  }
  if (!b) {
    switch (fEpscM) {
      case undefined:
      case constant:
        break;
      case byfit:
      case sellmeier:
        b = kTRUE;
        break;
    }
  }
  return b;
}
Double_t TLitMedium::GetnCerenkov() const {
  //  Delivers the value of the index of refraction used in the formulae for
  //Cerenkov radiation.
  const Double_t un = 1.0;
  Double_t mu;
  Double_t epsc = 0.0;
  Double_t n = 0.0;
  const char *met = "GetnCerenkov";
  if (fFitMu) mu = TMath::Abs(fFitMu->V(fgCerWaveL));
  else mu = fMu;
  switch (fAnisotropic) {
  case 0:
    switch (fIndexM) {
    case undefined:
      gCleanOut->MM(fatal,met,"method for index not defined");
      break;
    case constant:
      n = fIndex;
      break;
    case byfit:
      n = TMath::Abs(fFitIndex->V(fgCerWaveL));
      if (n<un) n = un;
      break;
    case sellmeier:
      n = fIndexS->GetIndex(fgCerWaveL,mu);
      if (n<un) n = un;
      break;
    }//end switch (fIndexM)
    break;
  default:
    switch (fEpscM) {
    case undefined:
      gCleanOut->MM(fatal,met,"method for Epsc not defined");
      break;
    case constant:
      epsc = fEpsc;
      break;
    case byfit:
      epsc = TMath::Abs(fFitEpsc->V(fgCerWaveL));
      break;
    case sellmeier:
      epsc = fEpscS->GetDiel(fgCerWaveL);
      break;
    }//end switch (fEpscM)
    n = TMath::Sqrt(epsc*mu);
    break;
  }
  return n;
}
Int_t TLitMedium::GetNPhot(Double_t Edep) const {
  //  This method returns the number of photons generated for a deposit of Edep
  // Mev, but affecting it with gaussian statistical fluctuations [intrinsic
  // resolution, Fano factor], and also, if fFitNPLY is non-zero, with a
  // non-linearity factor between the number of photons generated and the deposited
  // energy.
  //  If you do not want these 2 features, call GetPhotMev() instead of
  // GetNPhot() and multiply by Edep.
  //
  Int_t iphot;
  Double_t num,nphot,x,r; 
  if (fHasNPLY) {
    if (!fFitNPLY) {
      gCleanOut->MM(error,"GetNPhot","The Non-proportionality has not been set correctly",ClassName());
      gCleanOut->MM(killthis,"GetNPhot","Please check SetNonPropLY() for material",ClassName());
    }
    num = Edep*fPhotMev*(fFitNPLY->V(Edep));
  }
  else num = Edep*fPhotMev;
  nphot = gRandom3->Gaus(num,fResolutionScale*TMath::Sqrt(num));
  iphot = (Int_t)nphot;
  r     = nphot - iphot;
  x     = gRandom3->Rndm();
  if (x<=r) iphot++;
  return iphot; 
}
Double_t TLitMedium::GetPhotMev() const {
  // Backward compatible GetPhotMev which does not include statistical broadening
  const Double_t un  = 1.0;
  const Double_t eps = 1.0e-8;
  Double_t t;
  t = TMath::Abs(fResolutionScale - un);
  if (fHasNPLY) {
    gCleanOut->MM(error,"GetPhotMev","Material has non-proportional lightyield response",ClassName());
    gCleanOut->MM(error,"GetPhotMev","Cannot call GetPhotMev() in this case",ClassName());
    gCleanOut->MM(fatal,"GetPhotMev","Please use GetPhotMev(Double_t Edep)",ClassName());
  }
  if (t>eps) {
    gCleanOut->MM(error,"GetPhotMev","GetPhotMeV which does not support fResolutionScale",ClassName());
    gCleanOut->MM(fatal,"GetPhotMev","Use GetNPhot(Double_t Edep) to include the resolution scale",ClassName());
  }
  return fPhotMev;
}
Double_t TLitMedium::GetXSectnPE(Double_t MeV) const {
  // Return the photo-electric cross section for a photon of energy MeV
  if (!fPEOk) return 0.0;
  return fFitPE->V(MeV);
}
Bool_t TLitMedium::GoodForCPE() const {
// Check if all the quantities necessary for the photo-electric effect and 
//Compton scattering have been initialised. This routine is called internally 
//by SLitrani for each material before trying to use these processes.
  if (!fPEOk) return kFALSE;
  if (GetMaterial()->GetZ() <= 0) return kFALSE;
  if (GetMaterial()->GetDensity() <= 0) return kFALSE;
  if (GetMaterial()->GetA() <= 0) return kFALSE;
  return kTRUE;
}
void TLitMedium::Init() {
  // Do basic initialization
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  if (!fgLitMedium) fgLitMedium = new TObjArray();
  InitP();
  fMatCounter      = 0;
  fRevCounter      = 0;
  fSensible        = kFALSE;    //by default, not sensible;
  fFacAbsL         = un;        //by default, no factor for all values of abs. length
  fAniAbsL         = 0;         //abs. length isotropic by default
  fAbsLa           = -un;       //means Abs. length not yet initialized
  fPhimua          = zero;      //default imaginary part of fEpsa
  fAbsLb           = -un;       //means Abs. length not yet initialized
  fPhimub          = zero;      //default imaginary part of fEpsb
  fAbsLc           = -un;       //means Abs. length not yet initialized
  fPhimuc          = zero;      //default imaginary part of fEpsc
  fAnisotropic     = -1;        //optical properties defined later
  fIndexM          = undefined;
  fEpsaM           = undefined;
  fEpsbM           = undefined;
  fEpscM           = undefined;
  fIndex           = -1;        //means not yet initilized
  fEpsa            = -1;        //    "
  fEpsb            = -1;        //    "
  fEpsc            = -1;        //    "
  fMuFound         = kTRUE;
//Everything concerning dE/dx not initialized in the constructor,
// but requires further calls to other methods
  fdEdxUsed        = nodEdx;
  fPhotMev         = zero;
//Everything concerning Fluorescence not initialized in the constructor,
// but requires further calls to other methods
  fHistFluo        = kFALSE;
//Everything concerning electromagnetic showers
  fRm              = -un;
  fEc              = -un;
  fHasNPLY         = kFALSE;
  fPEOk            = kFALSE;
  fResolutionScale = 1.0;
  fHasDiffusion    = kFALSE;
  fDiffusLength    = zero;
  fHasRayleigh     = kFALSE;
  fRayleighLength  = zero;
  fHasWLS          = kFALSE;
  fWLSL            = zero;
  fHasAbsorption   = kTRUE;
//Case of wrapping
  fWrapAbsorbing   = 0;
  fWrapIsSlice     = kFALSE;
  fWrapDiffus      = zero;
  fWrapRindex      = zero;
  fWrapIindex      = zero;
  fWrapMu          = zero;
  fWrapSinThetaMax = zero;
  fTSThinSlice     = kFALSE;
  fDPLIsDepol      = kFALSE;
  InitWrapping();
  fNameMu      = "";
  fNameAbsLa   = "";
  fNameAbsLb   = "";
  fNameAbsLc   = "";
  fNameIndex   = "";
  fNameEpsa    = "";
  fNameEpsb    = "";
  fNameEpsb    = "";
  fNamedEdx    = "";
  fNameNPLY    = "";
  fNamePE      = "";
  fTSNameIndex = "";
  fTSNameMu    = "";
}
void TLitMedium::InitP() {
  //All pointers to 0
  fFitAbsLa      = 0;
  fFitAbsLb      = 0;
  fFitAbsLc      = 0;
  fFitDiffusL    = 0;
  fFitRayleighL  = 0;
  fIndexS        = 0;
  fFitIndex      = 0;
  fFitMu         = 0;
  fEpsaS         = 0;
  fFitEpsa       = 0;
  fEpsbS         = 0;
  fFitEpsb       = 0;
  fEpscS         = 0;
  fFitEpsc       = 0;
  fDeposit       = 0;
  fLitSpectrum   = 0;
  fFitNPLY       = 0;
  fFitPE         = 0;
  fWLSSpectrum   = 0;
  fFitWLSL       = 0;
  fWrapFitDiffus = 0;
  fWrapFitRindex = 0;
  fWrapFitIindex = 0;
  fWrapFitMu     = 0;
  fTSNextMed     = 0;
  fTSFitIndex    = 0;
  fTSFitMu       = 0;
  fDPLNextMed    = 0;
}
void TLitMedium::InitWrapping() {
  //basic default initialization for wrapping variables
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  //Pointers to 0
  fWrapFitDiffus   = 0;
  fWrapFitRindex   = 0;
  fWrapFitIindex   = 0;
  fWrapFitMu       = 0;
  fWrapIsSlice     = kFALSE;
  fWrapDiffus      = zero;
  fWrapNameDiffus  = "";
  fWrapRindex      = zero;
  fWrapNameRindex  = "";
  fWrapIindex      = zero;
  fWrapNameIindex  = "";
  fWrapMu          = zero;
  fWrapNameMu      = "";
  fWrapAbsorbing   = 0;
  fWrapSupplAbs    = zero;
  fWrapSinThetaMax = un;
}
void TLitMedium::IsBirefr(const char *nameEpsa,const char *nameEpsb,const char *nameEpsc) {
  // To be called when the optical medium is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention.
  //  This instance of IsBirefr handles the case where epsa, epsb and epsc
  //varies with wavelength according to fits of type TSplineFit provided by the
  //user. epsa, epsb and epsc are the 3 diagonal elements of the dielectric
  //tensor in the coordinate system where it is diagonal. nameEpsa, nameEpsb
  //and nameEpsc are the names of fits (of class TSplineFit) giving epsa, epsb
  //and epsc as a function of wavelength.
  //
  const char *met = "IsBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 3;
  TString s;
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gCleanOut->MMT(error,met,"Fit for low component Epsa not found","fNameEpsa",nameEpsa,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsa before defining optical optical medium",ClassName());
    return;
  }
  fFitEpsb = TSplineFit::FindFit(nameEpsb,2);
  if (fFitEpsb) {
    fNameEpsb = nameEpsb;
    fEpsbM    = byfit;
    fEpsb     = 1.0;    //means initialization ok for Epsb
  }
  else {
    gCleanOut->MMT(error,met,"Fit for medium component Epsb not found","fNameEpsb",nameEpsb,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsb before defining optical optical medium",ClassName());
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gCleanOut->MMT(error,met,"Fit for high component Epsc not found","fNameEpsc",nameEpsc,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsc before defining optical optical medium",ClassName());
    return;
  }
}
void TLitMedium::IsBirefr(Double_t epsa,Double_t epsb,Double_t epsc) {
  // To be called when the optical medium is totally anisotropic (birefringent) :
  //                     epsa < epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. The first argument
  //MUST be the smallest, the last argument MUST be the biggest.
  //  This instance of IsBirefr handles the case where epsa, epsb and epsc do
  //not vary with wavelength. epsa, epsb and epsc are the 3 diagonal elements
  //of the dielectric tensor in the coordinate system where it is diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  const char *met = "IsBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  Double_t a;
  Bool_t toocloseab = kFALSE;
  Bool_t tooclosebc = kFALSE;
  fAnisotropic = 3;
  epsa = TMath::Abs(epsa);
  epsb = TMath::Abs(epsb);
  epsc = TMath::Abs(epsc);
  //verifies that epsb > epsa
  if (epsa>=epsb) {
    gCleanOut->MM(warning,met,"epsb must be bigger than epsa",ClassName());
    gCleanOut->MM(warning,met,"We interchange epsa and epsb !",ClassName());
    a = epsb;
    epsb = epsa;
    epsa = a;
  }
  //verifies that epsc > epsb
  if (epsb>=epsc) {
    gCleanOut->MM(warning,met,"epsc must be bigger than epsb",ClassName());
    gCleanOut->MM(warning,met,"We interchange epsc and epsb !",ClassName());
    a = epsc;
    epsc = epsb;
    epsb = a;
  }
  //verifies minimal gap between epsa and epsb
  if (epsb < (epsa+fgMinimalGap)) {
    epsb = epsb + fgMinimalGap;
    toocloseab = kTRUE;
  }
  //verifies minimal gap between epsb and epsc
  while (epsc < (epsb+fgMinimalGap)) {
    epsc = epsc + fgMinimalGap;
    tooclosebc = kTRUE;
  }
  if (toocloseab) gCleanOut->M(warning,"epsa ~= epsb ! You should call IsUniAxialNegBirefr instead of IsBirefr");
  if (tooclosebc) gCleanOut->M(warning,"epsb ~= epsc ! You should call IsUniAxialPosBirefr instead of IsBirefr");
  fEpsa  = epsa;
  fEpsb  = epsb;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpsbM = constant;
  fEpscM = constant;
}
void TLitMedium::IsBirefr(Double_t ns0a,Double_t ns1a,Double_t ls1a,Double_t ns0b,Double_t ns1b,Double_t ls1b,Double_t ns0c,Double_t ns1c,Double_t ls1c) {
  //  To be called when the optical medium is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance of
  //of IsBirefr handles the case where epsa, epsb and epsc vary according
  //to the Sellmeier law with exactly 1 term in the summation. If you need
  //more than 1 term in the summation, either for epsa or for epsc or for
  //epsc or for all, look at the next instance of IsBirefr. If you need
  //information about the Sellmeier law, look at the class TLitSellmeier. The
  //parameters of type lsi are resonant wavelengths that are to be given in
  //nanometers. The parameters of type nsi have no dimension and are of "kind"
  //index of refraction.
  //
  //  The parameters are the following :
  //
  // for epsa :
  //
  // ns0a       : 1st constant term, associated with wavelength 0
  // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsb :
  //
  // ns0b       : 1st constant term, associated with wavelength 0
  // (ns1b,ls1b): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsc :
  //
  // ns0c       : 1st constant term, associated with wavelength 0
  // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
  //
  const char *met = "IsBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 3;
  fEpsaM = sellmeier;
  fEpsbM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = new TLitSellmeier(1,ns0a,ns1a,ls1a);
  fEpsbS = new TLitSellmeier(1,ns0b,ns1b,ls1b);
  fEpscS = new TLitSellmeier(1,ns0c,ns1c,ls1c);
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsb  = 1.0;    //means initialization ok for Epsb
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TLitMedium::IsBirefr(TLitSellmeier *const pa,TLitSellmeier *const pb,TLitSellmeier *const pc) {
  //  To be called when the optical medium is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsBirefr handles the case where epsa, epsb and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa, epsb or epsc or all. In that case, it is up to you to define 3
  //pointer pa, pb and pc to class TLitSellmeier, which is easy if you look at
  //the documentation associated with the class TLitSellmeier. For instance, let
  //us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa, 3 terms for epsb and 2 terms for epsc. You
  //can proceed like this in CINT (assuming pmat is your pointer towards the
  //TLitMedium) :
  //
  //  TLitSellmeier *pa,*pb,*pc;
  //  pa = new TLitSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pb = new TLitSellmeier(3,ns0b,ns1b,ls1b,ns2b,ls2b,ns3b,ls3b);
  //  pc = new TLitSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsBirefr(pa,pb,pc);
  //  
  //  Do NOT delete pa, pb or pc !
  //
  const char *met = "IsBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 3;
  fEpsaM = sellmeier;
  fEpsbM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpsbS = pb;
  fEpscS = pc;
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsb  = 1.0;    //means initialization ok for Epsb
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
Bool_t TLitMedium::IsDepolish(const TLitMedium *med) const {
  //  Asks whether there is depolish between TLitMedium "this" and TLitMedium
  // "med"
  Bool_t DPL = fDPLIsDepol && fDPLNextMed->GetDPLIsDepol() && (fDPLNextMed == med);
  return DPL;
}
void TLitMedium::IsIsotropic(const char *nameIndex) {
  //  To be called when the optical medium is optically isotropic: the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies with wavelength according
  //to a fit of type TSplineFit defined by the user. The argument is the name of
  //the fit giving the index of refraction as a function of wavelength for this
  //optical medium.
  //  If the TlitMedium is a wrapping, nameIndex does NOT concern the index of
  //refraction of the wrapping, BUT the index of refraction of the slice of
  //material [air, glue, grease, ...] between the shape and the wrapping.
  //The complex index of refraction of the wrapping is given in the call to
  //SetAsWrapping().
  fAnisotropic = 0;
  TString s;
  fIndexM = byfit;
  fFitIndex = TSplineFit::FindFit(nameIndex,1);
  if (fFitIndex) {
    fNameIndex = nameIndex;
    fIndex = 1.0;    //means initialization ok for index
  }
  else {
    const char *met = "IsIsotropic";
    gCleanOut->MMT(error,met,"Fit for index of refraction not found","nameIndex",nameIndex,ClassName());
    gCleanOut->MM(error,met,"Define fit for refr. index before defining optical medium",ClassName());
    return;
  }
}
void TLitMedium::IsIsotropic(Double_t index) {
  //  To be called when the optical medium is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction does not vary with wavelength.
  //The argument is the fixed value of the index of refraction.
  //  If the TlitMedium is a wrapping, index does NOT concern the index of
  //refraction of the wrapping, BUT the index of refraction of the slice of
  //material [air, glue, grease, ...] between the shape and the wrapping.
  //The complex index of refraction of the wrapping is given in the call to
  //SetAsWrapping().
  const Double_t un = 1.0;
  fAnisotropic = 0;
  fIndexM = constant;
  index = TMath::Abs(index);
  if (index<un) {
    const char *met = "IsIsotropic";
    gCleanOut->MMR(error,met,"Refr. index < 1","index",index,ClassName());
    fIndex = un;
  }
  else fIndex = index;
}
void TLitMedium::IsIsotropic(Int_t n,Double_t ns0,Double_t ns1,Double_t ls1,Double_t ns2,Double_t ls2,Double_t ns3,Double_t ls3) {
  //  To be called when the optical medium is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies according to the Sellmeier
  //law with at most 3 terms ( and at least 1 ) in the summation. If you need
  //more than 3 terms in the summation, look at the next instance of
  //IsIsotropic. If you need information about the Sellmeier law, look at the
  //class TLitSellmeier. The parameters of type lsi are resonant wavelengths
  //that are to be given in nanometers. The parameters of type nsi have no
  //dimension and are of "kind" index of refraction.
  //
  //  The parameters are the following :
  //
  // n        : nb. of couples (nsi,lsi) used. nsi : constant in numerator
  //             1 <= n <= 3                   lsi : resonant wavelength in nm
  // ns0      : 1st constant term, associated with wavelength 0
  // (ns1,ls1): 1st couple of value (nsi,lsi) is mandatory
  // (ns2,ls2): 2nd couple, mandatory only if n >=2.
  // (ns3,ls3): 3rd couple, mandatory only if n >=3.
  //
  //  If the TlitMedium is a wrapping, index does NOT concern the index of
  //refraction of the wrapping, BUT the index of refraction of the slice of
  //material [air, glue, grease, ...] between the shape and the wrapping.
  //The complex index of refraction of the wrapping is given in the call to
  //SetAsWrapping().
  fAnisotropic = 0;
  fIndexM = sellmeier;
  fIndexS = new TLitSellmeier(n,ns0,ns1,ls1,ns2,ls2,ns3,ls3);
  fIndex  = 1.0;    //means initialization ok for index
}
void TLitMedium::IsIsotropic(TLitSellmeier *const p) {
  //  To be called when the optical medium is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies according to the Sellmeier
  //law and you need more than 3 couples of values (nsi,lsi). In that case, it
  //is up to you to define a pointer to a class TLitSellmeier, which is easy if
  //you look at the class TLitSellmeier. For instance, let us suppose you want
  //a Sellmeier formula with 5 terms in the sum. You can proceed like this in
  //CINT (assuming pmat is your pointer towards the TLitMedium) :
  //
  //  TLitSellmeier *p;
  //  p = new TLitSellmeier(3,ns0,ns1,ls1,ns2,ls2,ns3,ls3);
  //  p->AddComponent(ns4,ls4);
  //  p->AddComponent(ns5,ls5);
  //  pmat->IsIsotropic(p);
  //  
  //  Do NOT delete p !
  //
  //  If the TlitMedium is a wrapping, index does NOT concern the index of
  //refraction of the wrapping, BUT the index of refraction of the slice of
  //material [air, glue, grease, ...] between the shape and the wrapping.
  //The complex index of refraction of the wrapping is given in the call to
  //SetAsWrapping().
  fAnisotropic = 0;
  fIndexM = sellmeier;
  fIndexS = p;
  fIndex  = 1.0;    //means initialization ok for index
}
Bool_t TLitMedium::IsThinSlice(const TLitMedium *med) const {
  //  Asks whether there is a thin slice between TLitMedium "this" and TLitMedium
  // "med"
  Bool_t TS = fTSThinSlice && fTSNextMed->GetTSThinSlice() && (fTSNextMed == med);
  return TS;
}
void TLitMedium::IsUniAxialNegBirefr(const char *nameEpsa,const char *nameEpsc) {
  // To be called when the optical medium is "Uniaxial negative birefringent" :
  //               epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention.
  // This instance of IsUniAxialNegBirefr handles the case where  epsa and
  //epsc varies with wavelength according to fits of type TSplineFit provided
  //by the user. epsa and epsc are the 2 diagonal elements of the dielectric
  //tensor in the coordinate system where it is diagonal. nameEpsa and nameEpsc
  //are the names of fits (of class TSplineFit) giving epsa and epsc as a function
  //of wavelength.
  // This method cannot be called for a wrapping !
  const char *met = "IsUniAxialNegBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 1;
  TString s;
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gCleanOut->MMT(error,met,"Fit for Epsa not found","nameEpsa",nameEpsa,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsa before optical medium",ClassName());
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gCleanOut->MMT(error,met,"Fit for high component Epsc not found","nameEpsc",nameEpsc,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsc before optical medium",ClassName());
    return;
  }
}
void TLitMedium::IsUniAxialNegBirefr(Double_t epsa,Double_t epsc) {
  //  To be called when the optical medium is "Uniaxial negative birefringent" :
  //                epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. The first argument
  //MUST be the smallest, the last argument MUST be the biggest.
  // This instance of IsUniAxialNegBirefr handles the case where epsa and
  //epsc do not vary with wavelength. epsa and epsc are the 2 diagonal
  //elements of the dielectric tensor in the coordinate system where it is
  //diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  const char *met = "IsUniAxialNegBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 1;
  epsa = TMath::Abs(epsa);
  epsc = TMath::Abs(epsc);
  if (epsa>=epsc) {
    gCleanOut->MM(warning,met,"epsc must be bigger than epsa",ClassName());
    gCleanOut->MM(warning,met,"We interchange epsa and epsc !",ClassName());
    Double_t a;
    a = epsc;
    epsc = epsa;
    epsa = a;
  }
  fEpsa  = epsa;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpscM = constant;
}
void TLitMedium::IsUniAxialNegBirefr(Double_t ns0a,Double_t ns1a,Double_t ls1a,Double_t ns0c,Double_t ns1c,Double_t ls1c) {
  //  To be called when the optical medium is "Uniaxial negative birefringent" :
  //                    epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law with exactly 1 term in the summation. If you need
  //more than 1 term in the summation, either for epsa or for epsc or for
  //both, look at the next instance of IsUniAxialNegBirefr. If you need
  //information about the Sellmeier law, look at the class TLitSellmeier. The
  //parameters of type lsi are resonant wavelengths that are to be given in
  //nanometers. The parameters of type nsi have no dimension and are of "kind"
  //index of refraction.
  //
  //  The parameters are the following :
  //
  // for epsa :
  //
  // ns0a       : 1st constant term, associated with wavelength 0
  // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsc :
  //
  // ns0c       : 1st constant term, associated with wavelength 0
  // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
  //
  const char *met = "IsUniAxialNegBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 1;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = new TLitSellmeier(1,ns0a,ns1a,ls1a);
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpscS = new TLitSellmeier(1,ns0c,ns1c,ls1c);
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TLitMedium::IsUniAxialNegBirefr(TLitSellmeier *const pa,TLitSellmeier *const pc) {
  //  To be called when the optical medium is "Uniaxial negative birefringent" :
  //                    epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa or epsc or both. In that case, it is up to you to define 2
  //pointer pa, pb to class TLitSellmeier, which is easy if you look at the class
  //TLitSellmeier. For instance, let us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa and 2 terms for epsc. You can proceed like this
  //in CINT (assuming pmat is your pointer towards the TLitMedium) :
  //
  //  TLitSellmeier *pa,*pc;
  //  pa = new TLitSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pc = new TLitSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsUniAxialNegBirefr(pa,pc);
  //  
  //  Do NOT delete pa nor pc !
  //
  const char *met = "IsUniAxialNegBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 1;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpscS = pc;
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TLitMedium::IsUniAxialPosBirefr(const char *nameEpsa,const char *nameEpsc) {
  //  To be called when the optical medium is "Uniaxial positive birefringent" :
  //                    epsa = epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc the
  //biggest element. Do NOT change this convention.
  //This instance of IsUniAxialPosBirefr handles the case where epsa and epsc
  //vary with wavelength according to fits of type TSplineFit provided by the user.
  //epsa and epsc are the 2 diagonal elements of the dielectric tensor in the
  //coordinate system where it is diagonal, nameEpsa and nameEpsc are the
  //names of fits (of class TSplineFit) giving epsa and epsc as a function of
  //wavelength.
  //
  const char *met = "IsUniAxialPosBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 2;
  TString s;
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gCleanOut->MMT(error,met,"Fit for low component Epsa not found","nameEpsa",nameEpsa,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsa before optical medium",ClassName());
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gCleanOut->MMT(error,met,"Fit for high component Epsc not found","nameEpsc",nameEpsc,ClassName());
    gCleanOut->MM(error,met,"Define fit for Epsc before optical medium",ClassName());
    return;
  }
}
void TLitMedium::IsUniAxialPosBirefr(Double_t epsa,Double_t epsc) {
  //  To be called when the optical medium is "UniAxial positive birefringent" :
  //                     epsa = epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc the
  //biggest element. Do NOT change this convention.
  // This instance of IsUniAxialPosBirefr handles the case where epsa and epsc
  //do not vary with wavelength. epsa and epsc are the 2 diagonal elements of
  //the dielectric tensor in the coordinate system where it is diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  const char *met = "IsUniAxialPosBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 2;
  epsa = TMath::Abs(epsa);
  epsc = TMath::Abs(epsc);
  if (epsa>=epsc) {
    gCleanOut->MM(warning,met,"epsc must be bigger than epsa",ClassName());
    gCleanOut->MM(warning,met,"We interchange epsa and epsc !",ClassName());
    Double_t a;
    a = epsc;
    epsc = epsa;
    epsa = a;
  }
  fEpsa  = epsa;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpscM = constant;
}
void TLitMedium::IsUniAxialPosBirefr(Double_t ns0a,Double_t ns1a,Double_t ls1a,Double_t ns0c,Double_t ns1c,Double_t ls1c) {
  //  To be called when the optical medium is "Uniaxial positive birefringent" :
  //                      epsa = epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialPosBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law with exactly 1 term in the summation. If you need
  //more than 1 term in the summation, either for epsa or for epsc or for
  //both, look at the next instance of IsUniAxialPosBirefr. If you need
  //information about the Sellmeier law, look at the class TLitSellmeier. The
  //parameters of type lsi are resonant wavelengths that are to be given in
  //nanometers. The parameters of type nsi have no dimension and are of "kind"
  //index of refraction.
  //
  //  The parameters are the following :
  //
  // for epsa :
  //
  // ns0a       : 1st constant term, associated with wavelength 0
  // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsc :
  //
  // ns0c       : 1st constant term, associated with wavelength 0
  // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
  //
  const char *met = "IsUniAxialPosBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 2;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = new TLitSellmeier(1,ns0a,ns1a,ls1a);
  fEpscS = new TLitSellmeier(1,ns0c,ns1c,ls1c);
  fEpsa     = 1.0;    //means initialization ok for Epsa
  fEpsc     = 1.0;    //means initialization ok for Epsc
}
void TLitMedium::IsUniAxialPosBirefr(TLitSellmeier *const pa,TLitSellmeier *const pc) {
  // To be called when the optical medium is "Uniaxial positive birefringent" :
  //                    epsa = epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa or epsc or both. In that case, it is up to you to define 2
  //pointer pa, pb to class TLitSellmeier, which is easy if you look at the class
  //TLitSellmeier. For instance, let us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa and 2 terms for epsc. You can proceed like this
  //in CINT (assuming pmat is your pointer towards the TLitMedium) :
  //
  //  TLitSellmeier *pa,*pc;
  //  pa = new TLitSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pc = new TLitSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsUniAxialPosBirefr(pa,pc);
  //  
  //  Do NOT delete pa nor pc !
  //
  const char *met = "IsUniAxialPosBirefr";
  if (fKind==OpticWrapping)
    gCleanOut->MM(fatal,met,"Cannot be called for a wrapping",ClassName());
  fAnisotropic = 2;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpscS = pc;
  fEpsa     = 1.0;    //means initialization ok for Epsa
  fEpsc     = 1.0;    //means initialization ok for Epsc
}
Bool_t TLitMedium::IsThinSlice(TLitMedium *nextmed) {
  // Check if between "this" and "nextmed", there is a thin slice
  Bool_t isTS = kFALSE;
  if (fKind == OpticMediumWithTS) {
    if (nextmed->fKind == OpticMediumWithTS) {
      if (fTSNextMed == nextmed) isTS = kTRUE;
    }
  }
  return isTS;
}
void TLitMedium::MoreAbsorption(Double_t supplAbs) {
  //Look whether there is a supplementary absorption, besides the normal
  //Fresnel absorption due to real part of complex index of refraction.
  //This supplementary absorption may be used to simulate dust or to make
  //the wrapping 100% absorbing by setting supplAbs = 1.0
  const Double_t zero  = 0.0;
  const Double_t one   = 1.0;
  const Double_t small = 1.0e-7;
  Double_t almostone;
  almostone  = one-small;
  fWrapAbsorbing = 1;
  fWrapSupplAbs  = supplAbs;
  if (supplAbs<=small) {
    fWrapSupplAbs  = zero;
    fWrapAbsorbing = 0;
  }
  if (supplAbs>=almostone) {
    fWrapSupplAbs = one;
    fWrapAbsorbing = 2;
  }
}
void TLitMedium::NewPhiMu(Double_t wavelength) {
  //  Calculates the imaginary parts fPhimua/b/c of the diagonal elements
  //fEpsa/b/c of the dielectric tensor from the values of the 3 wavelength
  //fAbsLA/b/c.  wavelength is in nanometers.
  const Double_t quatre  = 4.0;
  const Double_t In_cm_s = 100.0;
  const Double_t In_cm   = 1.0e-7;
  Double_t c;       //speed of light in [cm/s]
  Double_t c2;      //square of it
  Double_t w;       // 2*pi*frequency
  Double_t w2,w4;   //w^2, w^4
  Double_t L;       //wavelength in [cm]
  Double_t b;       //for intermediate calculations
  Double_t la2,la4; //absorption length^2 or ^4
  //calculates w
  c  = In_cm_s*TLitPhys::Get()->Cmks();  //speed of light in cm/s
  c2 = c*c;
  L  = In_cm*wavelength;       //wavelength in cm
  w  = TMath::TwoPi();
  w  = (w*c)/L;
  w2 = w*w;
  w4 = w2*w2;
  b  = quatre*fMu*w2;
  //fPhimua
  la2     = fAbsLa*fAbsLa;
  la4     = la2*la2;
  fPhimua = TMath::Sqrt((c2*(c2 + b*fEpsa*la2))/(quatre*w4*la4));
  //fPhimub
  la2     = fAbsLb*fAbsLb;
  la4     = la2*la2;
  fPhimub = TMath::Sqrt((c2*(c2 + b*fEpsb*la2))/(quatre*w4*la4));
  //fPhimuc
  la2     = fAbsLc*fAbsLc;
  la4     = la2*la2;
  fPhimuc = TMath::Sqrt((c2*(c2 + b*fEpsc*la2))/(quatre*w4*la4));
}
void TLitMedium::NewWavelength(Double_t wavelength) {
  //  Recalculates all parameters depending upon wavelength. wavelength is in
  // nanometers. The values to recalculate depend upon the TLitMedium being
  // an OpticMedium or an OpticWrapping
  const char     *met = "NewWavelength";
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  // (1) absorption length fAbsLa/b/c
  if (!(fKind==OpticWrapping)) {
    if (fFitAbsLa) {
      fAbsLa = fFacAbsL*fFitAbsLa->V(wavelength);
      if (fFacDiffus > zero) fDiffusLength = fFacDiffus*fAbsLa;
    }
    switch (fAniAbsL) {
        case 1:
          if (fFitAbsLb) fAbsLb = fFacAbsL*fFitAbsLb->V(wavelength);
          fAbsLc = fAbsLb;
          break;
        case 2:
          fAbsLb = fAbsLa;
          if (fFitAbsLc) fAbsLc = fFacAbsL*fFitAbsLc->V(wavelength);
          break;
        case 3:
          if (fFitAbsLb) fAbsLb = fFacAbsL*fFitAbsLb->V(wavelength);
          if (fFitAbsLc) fAbsLc = fFacAbsL*fFitAbsLc->V(wavelength);
          break;
        default:
          fAbsLb = fAbsLa;
          fAbsLc = fAbsLa;
          break;
    }
  }//end if (!(fKind==OpticWrapping))
  // (2) magnetic permeability fMu
  if (fFitMu) fMu = TMath::Abs(fFitMu->V(wavelength));
  // (3) index of refraction fIndex or dielectric tensor fEpsa/b/c
  // (3) or index of refraction fIndex of slice of material
  switch (fAnisotropic) {
    case -1:
      gCleanOut->MM(warning,met,GetName(),ClassName());
      gCleanOut->MM(warning,met,"optical properties not defined",ClassName());
      break;
    case  0:
      switch (fIndexM) {
        case undefined:
          gCleanOut->MM(fatal,met,"method for index not defined",ClassName());
          break;
        case constant:
          break;
        case byfit:
          fIndex = TMath::Abs(fFitIndex->V(wavelength));
          if (fIndex<un) fIndex = un;
          break;
        case sellmeier:
          fIndex = fIndexS->GetIndex(wavelength,fMu);
          if (fIndex<un) fIndex = un;
          break;
          }
      break; //end case 0: switch (fAnisotropic)
    case  3:
      switch (fEpsbM) {
        case undefined:
          gCleanOut->MM(fatal,met,"method for Epsb not defined",ClassName());
          break;
        case constant:
          break;
        case byfit:
          fEpsb = TMath::Abs(fFitEpsb->V(wavelength));
          break;
        case sellmeier:
          fEpsb = fEpsbS->GetDiel(wavelength);
          break;
      }//end switch (fEpsbM)
    case  1:
    case  2:
      switch (fEpsaM) {
        case undefined:
          gCleanOut->MM(fatal,met,"method for Epsa not defined",ClassName());
          break;
        case constant:
          break;
        case byfit:
          fEpsa = TMath::Abs(fFitEpsa->V(wavelength));
          break;
        case sellmeier:
          fEpsa = fEpsaS->GetDiel(wavelength);
          break;
      }//end switch (fEpsaM)
      switch (fEpscM) {
        case undefined:
          gCleanOut->MM(fatal,met,"method for Epsc not defined",ClassName());
          break;
        case constant:
          break;
        case byfit:
          fEpsc = TMath::Abs(fFitEpsc->V(wavelength));
          break;
        case sellmeier:
          fEpsc = fEpscS->GetDiel(wavelength);
          break;
      }//end switch (fEpscM)
      break;//end case 1/2
  };//end switch (fAnisotropic)
  if (fAnisotropic==1) fEpsb = fEpsc;
  if (fAnisotropic==2) fEpsb = fEpsa;  
  if (fAniAbsL) NewPhiMu(wavelength);
  if (fHasDiffusion && fFitDiffusL)
    fDiffusLength = TMath::Abs(fFitDiffusL->V(wavelength));
  // (8) Rayleigh scattering length
  if (fHasRayleigh && fFitRayleighL)
    fRayleighLength = TMath::Abs(fFitRayleighL->V(wavelength));
  // (9) Wavelength shifting
  if (fHasWLS && fFitWLSL) fWLSL = TMath::Abs(fFitWLSL->V(wavelength));
  if (fKind==OpticMediumWithTS) {
    //(10) Index of refraction of thin slice as a function of wavelength
    if (fTSFitIndex) fTSIndex = fTSFitIndex->V(wavelength);
    //(11) Magnetic permeability mu of thin slice as a function of wavelength
    if (fTSFitMu) fTSMu = fTSFitMu->V(wavelength);
  }
  if (fKind==OpticWrapping) {
    if ((fWrapIsSlice) && (fFitMu)) fMu = TMath::Abs(fFitMu->V(wavelength));
    // (4) Complex index of refraction of wrapping
    if (fWrapFitRindex) fWrapRindex = TMath::Abs(fWrapFitRindex->V(wavelength));
    //Notice the minus sign for the imaginary part of the index of refraction !
    if (fWrapFitIindex) fWrapIindex = - TMath::Abs(fWrapFitIindex->V(wavelength));
    // (5) Magnetic permeability of wrapping
    if (fWrapFitMu) fWrapMu = TMath::Abs(fWrapFitMu->V(wavelength));
    // (6) Proportion of diffusion for wrapping
    if (fWrapFitDiffus) {
      fWrapDiffus = fWrapFitDiffus->V(wavelength);
      if (fWrapDiffus<zero) fWrapDiffus = zero;
      if (fWrapDiffus>un)   fWrapDiffus = un;
    }
  }
}
void TLitMedium::NewWavelengthAll(Double_t wavelength,Bool_t ForceIt) {
  // Wavelength has changed. We recalculate ALL values depending upon wavelength
  //for ALL TLitMedium of the setup. We do not update TLitMedium::fgOldWavelength!
  //It will be done by TLitVolume::NewWavelengthAll().
  const Double_t eps = 1.0e-10;
  if ((TMath::Abs(fgOldWavelength-wavelength) > eps) || ForceIt) {
    TLitMedium *plitmed;
    TIter next1(TLitMedium::fgLitMedium);
    while ((plitmed = (TLitMedium *)next1())) {
      plitmed->NewWavelength(wavelength);
    }
  }
}
void TLitMedium::Print() {
  //Print all characteristics of optical medium
  Bool_t ok = kTRUE;
  if (!fChecked) ok = CheckIt();
  if (!ok) return;
  PrintName();
  //detector characteristics
  if (fSensible)gCleanOut->M(always,"optical medium is a detector of photons");
  else gCleanOut->M(always,"optical medium is not a detector of photons");
  //magnetic permeability
  if (fFitMu) {
    gCleanOut->M(always,"magnetic permeability varies with wavelength");
    gCleanOut->MMT(always,"","","name of fit",fNameMu.Data());
  }
  else gCleanOut->MMR(always,"","magnetic permeability fixed","fMu",fMu);
  //absorption lengths
  switch (fAniAbsL) {
  case 0:
    gCleanOut->M(always,"absorption length isotropic");
    if (fFitAbsLa) {
      gCleanOut->M(always,"absorption length varies with wavelength");
      gCleanOut->MMT(always,"","","name of fit",fNameAbsLa.Data());
    }
    else gCleanOut->MMR(always,"","absorption length fixed","fAbsLa",fAbsLa);
    break;
  case 1:
    gCleanOut->M(always,"absorption length anisotropic  La <> Lb = Lc");
    if (fFitAbsLa) {
      gCleanOut->M(always,"La, Lb and Lc vary with wavelength");
      gCleanOut->MTT(always,"fNameAbsLa",fNameAbsLa.Data(),"fNameAbsLb/c",fNameAbsLb.Data());
    }
    else {
      gCleanOut->MMR(always,"","La fixed","fAbsLa",fAbsLa);
      gCleanOut->MMR(always,"","Lb fixed","fAbsLb",fAbsLb);
      gCleanOut->MMR(always,"","Lc fixed","fAbsLc",fAbsLc);
    }
    break;
  case 2:
    gCleanOut->M(always,"absorption length anisotropic  La = Lb <> Lc");
    if (fFitAbsLa) {
      gCleanOut->M(always,"La, Lb and Lc vary with wavelength");
      gCleanOut->MMT(always,"","name of fit for La","fNameAbsLa",fNameAbsLa.Data());
      gCleanOut->MMT(always,"","name of fit for Lb and Lc","fNameAbsLc",fNameAbsLc.Data());
    }
    else {
      gCleanOut->MMR(always,"","La fixed","fAbsLa",fAbsLa);
      gCleanOut->MMR(always,"","Lb fixed","fAbsLb",fAbsLb);
      gCleanOut->MMR(always,"","Lc fixed","fAbsLc",fAbsLc);
    }
    break;
  case 3:
    gCleanOut->M(always,"absorption length anisotropic  La <> Lb <> Lc");
    if (fFitAbsLa) {
      gCleanOut->M(always,"La, Lb and Lc vary with wavelength");
      gCleanOut->MMT(always,"","name of fit for La","fNameAbsLa",fNameAbsLa.Data());
      gCleanOut->MMT(always,"","name of fit for Lb","fNameAbsLb",fNameAbsLb.Data());
      gCleanOut->MMT(always,"","name of fit for Lc","fNameAbsLc",fNameAbsLc.Data());
    }
    else {
      gCleanOut->MMR(always,"","La fixed","fAbsLa",fAbsLa);
      gCleanOut->MMR(always,"","Lb fixed","fAbsLb",fAbsLb);
      gCleanOut->MMR(always,"","Lc fixed","fAbsLc",fAbsLc);
    }
    break;
  }
  //optical properties
  gCleanOut->M(always,"  ---");
  gCleanOut->M(always,"  Optical properties :");
  gCleanOut->M(always,"  ---");
  switch ( fAnisotropic ) {
  case  0 :
    gCleanOut->M(always,"optical medium optically isotropic");
    switch (fIndexM) {
  case undefined:
    gCleanOut->M(error,"method to get index undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","index of refraction fixed","fIndex",fIndex);
    break;
  case byfit:
    gCleanOut->M(always,"index of refraction varies with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameIndex",fNameIndex.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"index of refraction by Sellmeier law");
    break;
    }
    break;
  case  1 :
    gCleanOut->M(always,"Optical medium uniaxial negative birefringent");
    switch (fEpsaM) {
  case undefined:
    gCleanOut->M(error,"method to get epsa undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","1st diagonal element epsa fixed","fEpsa",fEpsa);
    break;
  case byfit:
    gCleanOut->M(always,"1st diagonal element epsa varies with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsa",fNameEpsa.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"1st diagonal element epsa by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gCleanOut->M(error,"method to get epsc undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","2nd diag element epsb fixed","fEpsb",fEpsb);
    gCleanOut->MMR(always,"","3rd diag element epsc fixed","fEpsc",fEpsc);
    break;
  case byfit:
    gCleanOut->M(always,"2nd and 3rd diag. el. epsb/c vary with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsb/c",fNameEpsc.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"2nd and 3rd diag. el. epsb/c by Sellmeier law");
    break;
    }
    break;
  case  2 :
    gCleanOut->M(always,"optical medium uniaxial positive birefringent");
    switch (fEpsaM) {
  case undefined:
    gCleanOut->M(error,"method to get epsa and epsb undefined");
     break;
  case constant:
    gCleanOut->MMR(always,"","1st diag element epsa fixed","fEpsa",fEpsa);
    gCleanOut->MMR(always,"","2nd diag element epsb fixed","fEpsb",fEpsb);
    break;
  case byfit:
    gCleanOut->M(always,"1st and 2nd diag. el. epsa/b vary with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsa",fNameEpsa.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"1st and 2nd diag. el. epsa/b by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gCleanOut->M(error,"method to get epsc undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","3rd diag element epsc fixed","fEpsc",fEpsc);
    break;
  case byfit:
    gCleanOut->M(always,"3rd diagonal element epsc vary with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsc",fNameEpsc.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"3rd diagonal element epsc by Sellmeier law");
    break;
    }
    break;
  case  3 :
    gCleanOut->M(always,"optical medium birefringent, i.e. totally anisotropic");
    switch (fEpsaM) {
  case undefined:
    gCleanOut->M(error,"method to get epsa undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","1st diag element epsa fixed","fEpsa",fEpsa);
    break;
  case byfit:
    gCleanOut->M(always,"1st diagonal element epsa varies with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsa",fNameEpsa.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"1st diagonal element epsa by Sellmeier law");
    break;
    }
    switch (fEpsbM) {
  case undefined:
    gCleanOut->M(error,"method to get epsb undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","2nd diag element epsb fixed","fEpsb",fEpsb);
    break;
  case byfit:
    gCleanOut->M(always,"2nd diagonal element epsb varies with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsb",fNameEpsb.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"2nd diagonal element epsb by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gCleanOut->M(error,"method to get epsc undefined");
    break;
  case constant:
    gCleanOut->MMR(always,"","3rd diag element epsc fixed","fEpsc",fEpsc);
    break;
  case byfit:
    gCleanOut->M(always,"3rd diagonal element epsc vary with wavelength");
    gCleanOut->MMT(always,"","Name of fit","fNameEpsc",fNameEpsc.Data());
    break;
  case sellmeier:
    gCleanOut->M(always,"3rd diagonal element epsc by Sellmeier law");
    break;
    }
    break;
  default:
    gCleanOut->M(warning,"optical characteristics undefined");
    break;
  }
  switch (fdEdxUsed) {
  case nodEdx:
    gCleanOut->M(always,"optical medium optically inactiv when crossed by particle");
    break;
  case WithoutLandau:
    gCleanOut->M(always,"optical medium optically activ when crossed by particle");
    gCleanOut->MMR(always,"","Nb of photons emitted per Mev","fPhotMev",fPhotMev);
    gCleanOut->MMT(always,"","Name of fit for dE/dx versus E","fNamedEdx",fNamedEdx.Data());
    break;
  case WithLandau:
    gCleanOut->M(always,"optical medium optically activ when crossed by particle");
    gCleanOut->MMR(always,"","Nb of photons emitted per Mev","fPhotMev",fPhotMev);
    gCleanOut->MMT(always,"","Name of family of fits for dE/dx with Landau","fNamedEdx",fNamedEdx.Data());
    break;
  }
  if (fKind == OpticWrapping) {
    gCleanOut->M(always,"  ---");
    gCleanOut->M(always,"    Optical medium is a wrapping" );
    gCleanOut->M(always,"  ---");
    if (fWrapIsSlice) gCleanOut->M(always,"slice of material between face and wrapping");
    else              gCleanOut->M(always,"no slice of material between face and wrapping");
    if (fWrapFitDiffus) {
      gCleanOut->M(always,"% of diffusion varies with wavelength");
      gCleanOut->MMT(always,"","Name of fit for % diffusion","fWrapNameDiffus",fWrapNameDiffus.Data());
    }
    else gCleanOut->MMR(always,"","% of diffusion fixed","fWrapDiffus",fWrapDiffus);
    if (fWrapFitRindex) {
      gCleanOut->M(always,"real part of index varies with wavelength");
      gCleanOut->MMT(always,"","name of fit for Real(index)","fWrapNameRindex",fWrapNameRindex.Data());
    }
    else gCleanOut->MMR(always,"","Real(index) fixed","fWrapRindex",fWrapRindex);
    if (fWrapFitIindex) {
      gCleanOut->M(always,"imaginary part of index varies with wavelength");
      gCleanOut->MMR(always,"","name of fit for Im(index)","fWrapIindex",fWrapIindex);
    }
    else gCleanOut->MMR(always,"","Im(index) fixed","fWrapIindex",fWrapIindex);
    if (fWrapFitMu) {
      gCleanOut->M(always,"magnetic permeability mu varies with wavelength");
      gCleanOut->MMT(always,"","name of fit for mu","fWrapNameMu",fWrapNameMu.Data());
    }
    else gCleanOut->MMR(always,"","mu fixed","fWrapMu",fWrapMu);
     switch (fWrapAbsorbing) {
      case 0:
        gCleanOut->M(always,"there is no supplementary absorption");
        break;
      case 1:
        gCleanOut->MMR(always,"","supplementary absorption","fWrapSupplAbs",fWrapSupplAbs);
        break;
      case 2:
        gCleanOut->M(always,"wrapping is 100% absorbing");
        break;
    }
  }
}
void TLitMedium::PrintName() const {
  //Prints name and title of the medium
  gCleanOut->M(always,"   ---");
  gCleanOut->MMT(always,"","Medium","Name",GetName());
  gCleanOut->MMT(always,"","Medium","Title",GetTitle());
  gCleanOut->M(always,"   ---");
}
void TLitMedium::PrintProblem() const {
  //Header to point out a problem
  gCleanOut->M(always,"   ---");
  gCleanOut->M(always,"THERE IS A PROBLEM IN THE FOLLOWING TLitMedium :");
  gCleanOut->M(always,"   ---");
}
void TLitMedium::ProblemHeader() const {
  //header before reporting a problem
  PrintProblem();
  PrintName();
  return;
}
void TLitMedium::SetAbsLa(Double_t absLa,Bool_t destroy) {
  // To change the absorption length versus wavelength (or in case of anisotropy,
  //the part of the absorption length linked with epsa) for this optical medium.
  //Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLa
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLa = absLa;
  if (fFitAbsLa) {
    if (destroy) delete fFitAbsLa;
    fFitAbsLa = 0;
  }
}
void TLitMedium::SetAbsLa(const char *nameAbsLa,Bool_t destroy) {
  // To change the absorption length versus wavelength (or in case of anisotropy,
  //the part of the absorption length linked with epsa) for this optical medium.
  //Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLa.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  TString s;
  if (fFitAbsLa && destroy) delete fFitAbsLa;
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for Abs. length
  }
  else {
    const char *met = "SetAbsLa";
    gCleanOut->MM(error,met,"Fit for absorption length fAbsLa not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Name of missing fit","nameAbsLa",nameAbsLa,ClassName());
    return;
  }
}
void TLitMedium::SetAbsLb(Double_t absLb,Bool_t destroy) {
  // To change the part of the absorption length linked with epsb) for this
  //anisotropic optical medium.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLb
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLb = absLb;
  if (fFitAbsLb) {
    if (destroy) delete fFitAbsLb;
    fFitAbsLb = 0;
  }
}
void TLitMedium::SetAbsLb(const char *nameAbsLb,Bool_t destroy) {
  // To change the part of the absorption length linked with epsb for this
  //anisotropic optical medium.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLb.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  if (fFitAbsLb && destroy) delete fFitAbsLb;
  fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
  if (fFitAbsLb) {
    fNameAbsLb = nameAbsLb;
    fAbsLb     = un;    //means initialization ok for Abs. length
  }
  else {
    const char *met = "SetAbsLb";
    gCleanOut->MM(error,met,"Fit for absorption length fAbsLb not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Name of missing fit","nameAbsLb",nameAbsLb,ClassName());
    return;
  }
}
void TLitMedium::SetAbsLc(Double_t absLc,Bool_t destroy) {
  // To change the part of the absorption length linked with epsc for this
  //anisotropic optical medium.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLc
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLc = absLc;
  if (fFitAbsLc) {
    if (destroy) delete fFitAbsLc;
    fFitAbsLc = 0;
  }
}
void TLitMedium::SetAbsLc(const char *nameAbsLc,Bool_t destroy) {
  // To change the part of the absorption length linked with epsc for this
  //anisotropic optical medium.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLc.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  if (fFitAbsLc && destroy) delete fFitAbsLc;
  fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
  if (fFitAbsLc) {
    fNameAbsLc = nameAbsLc;
    fAbsLc     = un;    //means initialization ok for Abs. length
  }
  else {
    const char *met = "SetAbsLc";
    gCleanOut->MM(error,met,"Fit for absorption length fAbsLc not found",ClassName());
    gCleanOut->MM(error,met,"Define fit for absorption length before optical medium",ClassName());
    gCleanOut->MMT(fatal,met,"Name of missing fit","nameAbsLc",nameAbsLc,ClassName());
    return;
  }
}
void TLitMedium::SetAsWrapping(const char *nameDiffus, const char *nameRindex,
  const char *nameIindex,const char *nameMu,Double_t supplAbs,Double_t ThetaMax) {
  //  xxx
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    nameDiffus   Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 dependent upon the wavelength: nameDiffus is the name of the
  //                 fit of type TSplineFit of category 9 which gives this dependency.
  //    nameRindex   Real part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 3 with name nameRindex. It is the real
  //                 part of the index of refraction which allows absorption of the
  //                 wave. The bigger this real part, the greater the absorption.
  //    nameIindex   Imaginary part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 4 with name nameIindex. Notice that an 
  //                 imaginary part of an index of refraction must always be negative
  //                 to have absorption (and not explosion !) of the wave. But, as the
  //                 "Handbook of optical constants" gives it positive, you are requested
  //                 to give it also positive, SLitrani will change its sign. 
  //    nameMu       Magnetic permeability of the wrapping, dependent upon wavelength.
  //                 This dependency is provided by a fit of type TSplineFit of category
  //                 14 with name nameMu. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    s = nameDiffus;
    FindWrapDiffus(s);
    s = nameRindex;
    FindRindex(s);
    s = nameIindex;
    FindIindex(s);
    s = nameMu;
    FindWrapMu(s);
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(const char *nameDiffus, const char *nameRindex,
  const char *nameIindex,Double_t mu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    nameDiffus   Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 dependent upon the wavelength: nameDiffus is the name of the
  //                 fit of type TSplineFit of category 9 which gives this dependency.
  //    nameRindex   Real part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 3 with name nameRindex. It is the real
  //                 part of the index of refraction which allows absorption of the
  //                 wave. The bigger this real part, the greater the absorption.
  //    nameIindex   Imaginary part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 4 with name nameIindex. Notice that an 
  //                 imaginary part of an index of refraction must always be negative
  //                 to have absorption (and not explosion !) of the wave. But, as the
  //                 "Handbook of optical constants" gives it positive, you are requested
  //                 to give it also positive, SLitrani will change its sign. 
  //    mu           Magnetic permeability of the wrapping, fixed, i.e. not dependent
  //                 upon wavelength. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //                 [ Default 1.0 ]
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    s = nameDiffus;
    FindWrapDiffus(s);
    s = nameRindex;
    FindRindex(s);
    s = nameIindex;
    FindIindex(s);
    fWrapMu = TMath::Abs(mu);
    fWrapNameMu = snone;
    fWrapFitMu  = 0;
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(const char *nameDiffus, Double_t Rindex, Double_t Iindex,
  const char *nameMu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    nameDiffus   Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 dependent upon the wavelength: nameDiffus is the name of the
  //                 fit of type TSplineFit of category 9 which gives this dependency.
  //    Rindex       Real part of the index of refraction of the wrapping, fixed, i.e.
  //                 not dependent upon wavelength. It is the real part of the index of
  //                 refraction which allows absorption of the wave. The bigger this
  //                 real part, the greater the absorption.
  //    Iindex       Imaginary part of the index of refraction of the wrapping, fixed,
  //                 i.e. not  dependent upon wavelength. Notice that an imaginary part
  //                 of an index of refraction must always be negative to have absorption
  //                 (and not explosion !) of the wave. But, as the "Handbook of optical
  //                 constants" gives it positive, you are requested to give it also
  //                 positive, SLitrani will change its sign. 
  //    nameMu       Magnetic permeability of the wrapping, dependent upon wavelength.
  //                 This dependency is provided by a fit of type TSplineFit of category
  //                 14 with name nameMu. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    s = nameDiffus;
    FindWrapDiffus(s);
    fWrapRindex     = TMath::Abs(Rindex);
    fWrapNameRindex = snone;
    fWrapFitRindex  = 0;
    //Notice the minus sign for fWrapIindex
    fWrapIindex     = -TMath::Abs(Iindex);
    fWrapNameIindex = snone;
    fWrapFitIindex  = 0;
    s = nameMu;
    FindWrapMu(s);
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(const char *nameDiffus, Double_t Rindex, Double_t Iindex,
  Double_t mu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    nameDiffus   Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 dependent upon the wavelength: nameDiffus is the name of the
  //                 fit of type TSplineFit of category 9 which gives this dependency.
  //    Rindex       Real part of the index of refraction of the wrapping, fixed, i.e.
  //                 not dependent upon wavelength. It is the real part of the index of
  //                 refraction which allows absorption of the wave. The bigger this
  //                 real part, the greater the absorption.
  //    Iindex       Imaginary part of the index of refraction of the wrapping, fixed,
  //                 i.e. not  dependent upon wavelength. Notice that an imaginary part
  //                 of an index of refraction must always be negative to have absorption
  //                 (and not explosion !) of the wave. But, as the "Handbook of optical
  //                 constants" gives it positive, you are requested to give it also
  //                 positive, SLitrani will change its sign. 
  //    mu           Magnetic permeability of the wrapping, fixed, i.e. not dependent
  //                 upon wavelength. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //                 [ Default 1.0 ]
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    s = nameDiffus;
    FindWrapDiffus(s);
    fWrapRindex     = TMath::Abs(Rindex);
    fWrapNameRindex = snone;
    fWrapFitRindex  = 0;
    //Notice the minus sign for fWrapIindex
    fWrapIindex     = -TMath::Abs(Iindex);
    fWrapNameIindex = snone;
    fWrapFitIindex  = 0;
    fWrapMu         = TMath::Abs(mu);
    fWrapNameMu     = snone;
    fWrapFitMu      = 0;
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(Double_t diffus, const char *nameRindex,
  const char *nameIindex,const char *nameMu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    diffus       Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 fixed, not dependent upon wavelength.
  //    nameRindex   Real part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 3 with name nameRindex. It is the real
  //                 part of the index of refraction which allows absorption of the
  //                 wave. The bigger this real part, the greater the absorption.
  //    nameIindex   Imaginary part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 4 with name nameIindex. Notice that an 
  //                 imaginary part of an index of refraction must always be negative
  //                 to have absorption (and not explosion !) of the wave. But, as the
  //                 "Handbook of optical constants" gives it positive, you are requested
  //                 to give it also positive, SLitrani will change its sign. 
  //    nameMu       Magnetic permeability of the wrapping, dependent upon wavelength.
  //                 This dependency is provided by a fit of type TSplineFit of category
  //                 14 with name nameMu. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t un = 1.0;
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    diffus = TMath::Abs(diffus);
    if (diffus>un) diffus = un;
    fWrapDiffus     = diffus;
    fWrapNameDiffus = snone;
    s = nameRindex;
    FindRindex(s);
    s = nameIindex;
    FindIindex(s);
    s = nameMu;
    FindWrapMu(s);
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(Double_t diffus, const char *nameRindex,
  const char *nameIindex,Double_t mu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    diffus       Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 fixed, not dependent upon wavelength.
  //    nameRindex   Real part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 3 with name nameRindex. It is the real
  //                 part of the index of refraction which allows absorption of the
  //                 wave. The bigger this real part, the greater the absorption.
  //    nameIindex   Imaginary part of the index of refraction of the wrapping, dependent 
  //                 upon wavelength. This dependency is provided by a fit of type
  //                 TSplineFit of category 4 with name nameIindex. Notice that an 
  //                 imaginary part of an index of refraction must always be negative
  //                 to have absorption (and not explosion !) of the wave. But, as the
  //                 "Handbook of optical constants" gives it positive, you are requested
  //                 to give it also positive, SLitrani will change its sign. 
  //    mu           Magnetic permeability of the wrapping, fixed, i.e. not dependent
  //                 upon wavelength. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //                 [ Default 1.0 ]
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t un = 1.0;
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    diffus = TMath::Abs(diffus);
    if (diffus>un) diffus = un;
    fWrapDiffus     = diffus;
    s = nameRindex;
    FindRindex(s);
    s = nameIindex;
    FindIindex(s);
    fWrapMu = TMath::Abs(mu);
    fWrapNameMu = snone;
    fWrapFitMu  = 0;
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(Double_t diffus, Double_t Rindex, Double_t Iindex,
  const char *nameMu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    diffus       Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 fixed, not dependent upon wavelength.
  //    Rindex       Real part of the index of refraction of the wrapping, fixed, i.e.
  //                 not dependent upon wavelength. It is the real part of the index of
  //                 refraction which allows absorption of the wave. The bigger this
  //                 real part, the greater the absorption.
  //    Iindex       Imaginary part of the index of refraction of the wrapping, fixed,
  //                 i.e. not  dependent upon wavelength. Notice that an imaginary part
  //                 of an index of refraction must always be negative to have absorption
  //                 (and not explosion !) of the wave. But, as the "Handbook of optical
  //                 constants" gives it positive, you are requested to give it also
  //                 positive, SLitrani will change its sign. 
  //    nameMu       Magnetic permeability of the wrapping, dependent upon wavelength.
  //                 This dependency is provided by a fit of type TSplineFit of category
  //                 14 with name nameMu. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t un = 1.0;
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    diffus = TMath::Abs(diffus);
    if (diffus>un) diffus = un;
    fWrapDiffus = diffus;
    fWrapRindex     = TMath::Abs(Rindex);
    fWrapNameRindex = snone;
    fWrapFitRindex  = 0;
    //Notice the minus sign for fWrapIindex
    fWrapIindex     = -TMath::Abs(Iindex);
    fWrapNameIindex = snone;
    fWrapFitIindex  = 0;
    s = nameMu;
    FindWrapMu(s);
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
}
void TLitMedium::SetAsWrapping(Double_t diffus, Double_t Rindex, Double_t Iindex,
  Double_t mu,Double_t supplAbs,Double_t ThetaMax) {
  //  SetAsWrapping transforms this optical medium into a wrapping. In particular, 
  // it provides it with a complex index of refraction, a proportion of diffusion versus 
  // reflection, a magnetic permeability, and a supplementary absorption, if needed. This 
  // wrapping may or may not have a slice of medium (for instance air) between the
  // wrapping and the contiguous TGeoVolume. The existence or not of a slice of material
  // between the wrapping and the TGeoMedium in front of it has been decided by the value
  // given to the 4th argument [mu or nameMu] of the constructor [the ones for wrappings].
  // Notice the following limitation of SLitrani: it is FORBIDDEN to have a wrapping
  // WITHOUT slice in contact with a TLitMedium which is anisotropic, EXCEPT in the case
  // where the wrapping is defined as totally absorbing, i.e. the 5th argument of
  // SetAsWrapping(), supplAbs, is set to 1.0.
  //
  //  Arguments of SetAsWrapping()
  //
  //    diffus       Proportion [0..1] of photons which are diffused instead of
  //                 reflected. All non-absorbed light which is not diffused is
  //                 assumed to be reflected. This proportion of light diffused is
  //                 fixed, not dependent upon wavelength.
  //    Rindex       Real part of the index of refraction of the wrapping, fixed, i.e.
  //                 not dependent upon wavelength. It is the real part of the index of
  //                 refraction which allows absorption of the wave. The bigger this
  //                 real part, the greater the absorption.
  //    Iindex       Imaginary part of the index of refraction of the wrapping, fixed,
  //                 i.e. not  dependent upon wavelength. Notice that an imaginary part
  //                 of an index of refraction must always be negative to have absorption
  //                 (and not explosion !) of the wave. But, as the "Handbook of optical
  //                 constants" gives it positive, you are requested to give it also
  //                 positive, SLitrani will change its sign. 
  //    mu           Magnetic permeability of the wrapping, fixed, i.e. not dependent
  //                 upon wavelength. As the "Handbook of optical constants" never
  //                 quotes the magnetic permeability of a wrapping, I suppose that it
  //                 is considered as always equal to 1. But for the sake of generality,
  //                 it is left adjustable by litrani. After all, the formula for
  //                 reflection and refraction do depend upon mu.
  //                 [ Default 1.0 ]
  //    supplAbs     Possibility of a supplementary absorption. Default to 0.0, normal
  //                 value, because the absorption by the wrapping is calculated from
  //                 the Fresnel formula with complex index of refraction ( given by
  //                 Rindex and Iindex ). If you give it a non-zero value, say 0.1, you
  //                 will add by that a supplementary probability (10%) of absorption.
  //                 It may be an handy way of studiing the effect of a wrapping getting 
  //                 worse and worse. This parameter is also useful if you want to define
  //                 a wrapping which is 100% absorbing. In that case, put it to 1.0.
  //                 You will spare by that the calculation of the Fresnel equations to
  //                 the program. Notice that the photons which are not absorbed by
  //                 Fresnel but are absorbed because of this supplementary, artificially 
  //                 added, absorption are said "absorbed before wrapping" by SLitrani.
  //                 [ Default 0.0 ]
  //    ThetaMax     Maximum theta angle for the diffused photon. By putting a value
  //                 smaller than 90 degree, you avoid that diffused photons recoil
  //                 almost parallel with the diffusing surface. Look at the part of
  //                 the class description of TLitMedium concerning wrappings to learn
  //                 why it may be necessary to limit the angle of recoil of the
  //                 diffused photon to values less than 90°
  //                 [ Default 90.0 ]
  //
  const Double_t un = 1.0;
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  TString s;
  TString snone = "none";
  if (fAnisotropic == 0) {
    fKind = OpticWrapping;
    ThetaMax   = TMath::Abs(ThetaMax);
    if (ThetaMax>nonante) ThetaMax = nonante;
    ThetaMax  *= TMath::Pi()/pideg;
    fWrapSinThetaMax = TMath::Sin(ThetaMax);
    diffus = TMath::Abs(diffus);
    if (diffus>un) diffus = un;
    fWrapDiffus = diffus;
    fWrapRindex     = TMath::Abs(Rindex);
    fWrapNameRindex = snone;
    fWrapFitRindex  = 0;
    //Notice the minus sign for fWrapIindex
    fWrapIindex     = -TMath::Abs(Iindex);
    fWrapNameIindex = snone;
    fWrapFitIindex  = 0;
    fWrapMu = TMath::Abs(mu);
    fWrapNameMu = snone;
    fWrapFitMu  = 0;
    MoreAbsorption(supplAbs);
  }
  else gCleanOut->MM(fatal,"SetAsWrapping","Slice between wrapping and shape must be isotropic",ClassName());
 }
void TLitMedium::SetCanStopElectron(Double_t T0, Double_t T1, Double_t b0,
                                        Double_t b1, Double_t b2, Double_t b3) {
  // Material properties necessary to calculate electron ionisation dE/dX
  //See S.M. Seltzer and M.J. Berger, Evaluation of the collision stopping
  //power of elements and compounds for electrons and positrons,
  //Int. J. Appl. Radiat., 33 (1982) 1189-1218
  // This method not used for the time being
  fEP[0] = T0;
  fEP[1] = T1;
  fEP[2] = b0;
  fEP[3] = b1;
  fEP[4] = b2;
  fEP[5] = b3;
  fCanStopElectron = kTRUE;
}
void TLitMedium::SetCascades(Double_t rm, Double_t ec) {
  // Call this version of SetCascades only when the radiation length has
  //already been defined by
  //
  //  GetMaterial()->SetRadLen(x0);  //or by
  //  SetX0(x0);
  //
  //  to define :
  //
  //  rm : Moliere's radius  in cm
  //  ec : critical energy   in Mev
  //
  //  If you do not give ec, it will be calculated through the formula :
  //
  //  (23.21)  rm = (x0*es)/ec            with es = 21 Mev.
  //
  //  If you do give ec, pay attention that formula (23.21) be almost ok !
  //
  const Double_t zero   = 0.0;
  const Double_t es     = 21.0;
  const Double_t limlow = 0.25;
  const Double_t limup  = 4.0;
  Double_t x0,ec1,ec2,rap;
  x0 = GetMaterial()->GetRadLen();
  fRm = TMath::Abs(rm);
  ec1 = TMath::Abs(ec);
  ec2 = (x0*es)/fRm;
  if (ec>zero)  {
    rap = ec1/ec2;
    if ((rap>limlow) && (rap<limup)) fEc = ec1;
    else {
      const char *met = "SetCascades";
      gCleanOut->MMR(error,met,"Impossible value for critical energy","ec",ec,ClassName());
      gCleanOut->MMR(error,met,"We take (X0*es)/rm !","ec2",ec2,ClassName());
      fEc = ec2;
    }
  }
  if (ec<=zero) fEc = ec2;
}
void TLitMedium::SetCascades(Double_t x0, Double_t rm, Double_t ec) {
  //  To define :
  //
  //  x0 : radiation length  in cm
  //  rm : Moliere's radius  in cm
  //  ec : critical energy   in Mev
  //
  //  If you give a negative ec, it will be calculated through the formula :
  //
  //  (23.21)  rm = (x0*es)/ec            with es = 21 Mev.
  //
  //  If you do give ec, pay attention that formula (23.21) be almost ok !
  //
  const Double_t zero   = 0.0;
  const Double_t es     = 21.0;
  const Double_t limlow = 0.25;
  const Double_t limup  = 4.0;
  Double_t ec1,ec2,rap;
  x0 = TMath::Abs(x0);
  GetMaterial()->SetRadLen(x0);
  fRm = TMath::Abs(rm);
  ec1 = TMath::Abs(ec);
  ec2 = (x0*es)/fRm;
  if (ec>zero)  {
    rap = ec1/ec2;
    if ((rap>limlow) && (rap<limup)) fEc = ec1;
    else {
      const char *met = "SetCascades";
      gCleanOut->MMR(error,met,"Impossible value for critical energy","ec",ec,ClassName());
      gCleanOut->MMR(error,met,"We take (X0*es)/rm !","ec2",ec2,ClassName());
      fEc = ec2;
    }
  }
  if (ec<=zero) fEc = ec2;
}
void TLitMedium::SetDepolished(TLitMedium *med,Double_t ThetaDepls) {
  //
  //    DEPOLISHED SURFACES
  //
  //  When TLitMedium "this" and TlitMedium "med" are in contact, depolish
  // surface will be assumed if one of the two is a wrapping with slice of
  // material before the wrapping and the other is a normal tracking medium,
  // not anisotropic.
  //  It is not necessary to call SetDepolished() twice, once for "this" and
  // once for "med". Only one call is enough.
  //
  //  Arguments:
  //
  //   med        : the other TLitMedium in contact. If you put 0, you will set
  //                the state to "not depolished" fDPLIsDepol = false, suppressing
  //                the effect of a previous call to SetDepolished().
  //   ThetaDepls : angle of depolish in degree [0 - 90 ](roughness of surface)
  //
  //  Depolishing is simulated in the following way :
  //
  //  The normal to the surface at the point hit by the photon is randomly tilted
  // ( with respect to the true normal of the surface ) by an angle theta. theta
  // is generated randomly according to a distribution sin(theta) dtheta dphi,
  // between 0 and ThetaDepls. So ThetaDepls can adjust the roughness of the grinding.
  // You have to give ThetaDepls in degree. If you omit it, the value of 60° will
  // be taken.
  //
  //  Important restrictions :
  //
  //  The use of grinded ( depolished ) surface is only allowed for the faces of a
  // TGeoVolume containing an isotropic material. For an anisotropic material, SLitrani
  // gives up, for reasons explained below. Notice also that even if you declare a
  // face to be grinded (depolished), the part of this face which is in contact
  // with an other face of an other shape is never treated as grinded by SLitrani
  // Only the part of the face which is in contact with a wrapping is treated as
  // grinded. An other restriction is that you have not the rigth to declare as
  // depolished a face having a wrapping WITHOUT slice (of air or something else)
  // between the face and the wrapping.
  //
  //  Notice the following problems in this rather simple way of simulating
  //depolishing : 
  //
  //  (1) - after the normal at the point hit by the photon has been randomly tilted,
  // it may be that the angle of this new normal with the Poyinting vector of the
  // photon has become greater than 90 degree, which would mean that the photon
  // does not reach any more the surface at this point and would give meaningless
  // results in the calculation of the Fresnel equations ! If this happens, we
  // simply generate again theta until this does not happen any more. 
  //  (2) - after the normal at the point hit by the photon has been randomly tilted,
  // it may be that in case of total reflection, the outgoing photon does not any
  // more reenter the original shape ! If this happens, we simply generate again
  // theta until this does not happen any more. 
  //  (3) - after the normal at the point hit by the photon has been randomly tilted,
  // it may be that in case of refraction, the outgoing wave would make an angle
  // greater than 90 degree with the untilted normal to the surface, i-e the
  // refracted photon would not reach the wrapping ! If this happens, we simply
  // generate again theta until this does not happen any more. 
  //  (4) - As in the case of anisotropy, angle of reflection is not necessarily
  // equal to the incident angle, it is much too complicate to calculate the
  // possibility that difficulty (2) or (3) arises or not. For that reason, we
  // have forbidden the use of grinded surface associated with a shape
  // containing an anisotropic material. 
  //  (5) - when the photon does not make a total reflection, but goes out of the
  // face towards the wrapping, and is reflected back by the wrapping, we have
  // not implemented for that "return" case a grinded surface, but in that case we
  // leave the normal to the surface as it is ( as if the surface was polished ).
  // Had we not done that simplification, we would have had the difficulties (1)
  // (2) and (3) again on return, which we considered too much ! 
  //
  //  So keep in mind that our implementation of grinding (depolishing) is
  //rather simplified !
  //
  const Double_t zero  = 0.0;
  const Double_t pid   = 180.0;
  const Double_t pis2d = 90.0;
  const char *met = "SetDepolished";
  fDPLIsDepol = kFALSE;
  fDPLNextMed = 0;
  if (med) {
    if ((ThetaDepls<=zero) || (ThetaDepls>=pis2d)) {
      gCleanOut->MM(error,met,"Depolish angle must be within (0,90)",ClassName());
    }//end if ((ThetaDepls<=zero) || (ThetaDepls>=pis2d))
    else {
      if (IsWrapping()) {
        if (IsSlice()) {
          if (!med->IsWrapping()) {
            if (!med->GetAnisotropy()) {
              fDPLIsDepol  = kTRUE;
              fDPLTheta    = (ThetaDepls*TMath::Pi())/pid;
              fDPLCosTheta = TMath::Cos(fDPLTheta);
              if (!med->GetDPLNextMed()) med->SetDepolished(this,ThetaDepls);
            }
            else {
              gCleanOut->MM(error,met,"Depolish at exit of anisotropic medium is forbidden",ClassName());
            }
          }
          else {
            gCleanOut->MM(error,met,"Wrapping to be associated with non-wrapping",ClassName());
          }
        }
        else {
          gCleanOut->MM(error,met,"For depolish, a slice of material is required",ClassName());
         }
      }//end if (IsWrapping())
      else {
        if (!GetAnisotropy()) {
          if (med->IsWrapping()) {
            if (med->IsSlice()) {
              fDPLIsDepol  = kTRUE;
              fDPLTheta    = (ThetaDepls*TMath::Pi())/pid;
              fDPLCosTheta = TMath::Cos(fDPLTheta);
              if (!med->GetDPLNextMed()) med->SetDepolished(this,ThetaDepls);
            }//end if (med->IsSlice())
            else {
              gCleanOut->MM(error,met,"For depolish, a slice of material is required",ClassName());
            }//end else if (med->IsSlice())
          }//end if (med->IsWrapping())
          else {
            gCleanOut->MM(error,met,"Wrapping to be associated with non-wrapping",ClassName());
          }
        }//end if (!GetAnisotropy())
        else {
          gCleanOut->MM(error,met,"Depolish at exit of anisotropic medium is forbidden",ClassName());
        }//end else if (!GetAnisotropy())
      }//end else if (IsWrapping())
    }//end else if ((ThetaDepls<=zero) || (ThetaDepls>=pis2d))
    if (!fDPLIsDepol) gCleanOut->MM(error,met,"Depolishing refused",ClassName());
  }//end if (med)
}
void TLitMedium::SetDiffusionLength(Double_t diffL, Bool_t isdiff) {
  //  Declares that this optical medium has a diffusion length. In this version of
  //SetDiffusionLength(), the diffusion length may be given as a fixed value,
  //not depending upon wavelength, or the diffusion length may be obtained as
  //being equal to the absorption length multiplied by a factor :
  //
  //  - if isdiff is true (default) diffL is the diffusion length. It is fixed
  //      not depending upon wavelength.
  //  - if isdiff is false, the diffusion length is obtained taking the absor-
  //      ption length fAbsLa and multiplying it by the factor diffL. the
  //      diffusion length vary as a function of wavelength as the absorption
  //      length, multiplied by diffL.
  //  If you want a diffusion length varying with wavelength according to a
  //fit of type TSplineFit, look at the other version of SetDiffusionLength().
  //
  //  It is observed that sometimes a optical medium diffuses a photon in some other
  //direction. This phenomenon is due to bubbles of impurities (for instance
  //air) or to defaults in the crystal structure. SLitrani does not reproduces
  //exactly the diffusion on a bubble or on a defect. The way SLitrani simula-
  //tes diffusion, is by absorbing the photon and reemitting it at the same
  //place, with the same wavelength but with a new k vector, which is randomly
  //generated on 4pi. The probability of having a diffusion is handled exactly
  //the same way as the probability of having an absorption. The user has to
  //introduce a "diffusion length", similar to the "absorption length".
  //
  //  Parameters :
  //
  //   diffL    : if <=0 : there is no diffusion
  //              if > 0 : there is diffusion, and :
  //                if isdiff is true, diffL is the value of the diffusion
  //                  length, fixed, not depending upon wavelength
  //                if isdiff is false, diffL is a factor which gives the
  //                  diffusion length if multiplied by the absorption length
  //   isdiff   : see above
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  fFitDiffusL   = 0;
  if (diffL<=zero) {
    fHasDiffusion = kFALSE;
    fFacDiffus    = -un;
    fDiffusLength = -un;
  }
  else {
    if (isdiff) {
      fHasDiffusion = kTRUE;
      fFacDiffus    = -un;
      fDiffusLength = diffL;
    }
    else {
      fHasDiffusion = kTRUE;
      fFacDiffus    = diffL;
      fDiffusLength = diffL*fAbsLa;
    }
  }
}
void TLitMedium::SetDiffusionLength(const char *name) {
  //Declares that this optical medium has a diffusion length. The value of the dif-
  //fusion length as a function of wavelength is obtained by a fit of type
  //TSplineFit whose name is name.
  //  It is observed that sometimes a optical medium diffuses a photon in some other
  //direction. This phenomenon is due to bubbles of impurities (for instance
  //air) or to defaults in the crystal structure. SLitrani does not reproduces
  //exactly the diffusion on a bubble or on a defect. The way SLitrani simula-
  //tes diffusion, is by absorbing the photon and reemitting it at the same
  //place, with the same wavelength but with a new K vector, which is randomly
  //generated on 4pi. The probability of having a diffusion is handled exactly
  //the same way as the probability of having an absorption. The user has to
  //introduce a "diffusion length", similar to the "absorption length".
  //  The diffusion length may be fixed, not depending upon wavelength, or
  //varying proportionnaly to the absorption length. In that case the other
  //version of SetDiffusionLength() has to be called.
  //  Or the diffusion length is depending upon wavelength. In that case, 
  //this version of SetDiffusionLength() has to be called.
  //
  //   const char *name : name of the fit of type TSplineFit describing the diffu-
  //                  length as a function of wavelength. If name is "" or is
  //                  omitted, it is assumed that there is no diffusion.
  const Double_t un = 1.0;
  TString s;
  TString snone = "";
  if (!snone.CompareTo(name)) {
    fHasDiffusion = kFALSE;
    fDiffusLength = -un;
    fFitDiffusL   = 0;
  }
  else {
    fHasDiffusion = kTRUE;
    fFitDiffusL = TSplineFit::FindFit(name,7);
    if (fFitDiffusL) {
      fNameDiffusL  = name;
      fDiffusLength = un;    //means initialization ok for Diffus. length
    }
    else {
      const char *met = "SetDiffusionLength";
      gCleanOut->MMT(error,met,"Fit for diffusion length not found","name",name,ClassName());
      gCleanOut->MM(error,met,"No diffusion length assumed",ClassName());
      fHasDiffusion = kFALSE;
      fDiffusLength = -un;
      fFitDiffusL   = 0;
    }
  }
}
void TLitMedium::SetFactorOnAbsL(Double_t f) {
  //  All values of absorption lengths 
  // - [fAbsLa in case of isotropic absorption length
  // - [fAbsLa,fAbsLb,fAbsLc] in case of anisotropic absorption lengths
  // will be multiplied by the factor f before being used. It is useful for
  // instance when studying the effect of a degradation of the absorption
  // length.
  Double_t g;
  f = TMath::Abs(f);
  g = f/fFacAbsL;
  if (!fFitAbsLa) fAbsLa = g*fAbsLa;
  switch (fAniAbsL) {
  case 1:
    if (!fFitAbsLb) fAbsLb = g*fAbsLb;
    fAbsLc = fAbsLb;
    break;
  case 2:
    fAbsLb = fAbsLa;
    if (!fFitAbsLc) fAbsLc = g*fAbsLc;
    break;
  case 3:
    if (!fFitAbsLb) fAbsLb = g*fAbsLb;
    if (!fFitAbsLc) fAbsLc = g*fAbsLc;
    break;
  default:
    fAbsLb = fAbsLa;
    fAbsLc = fAbsLa;
    break;
  }
  fFacAbsL = f;
}
void TLitMedium::SetHistFluo(Bool_t tobe) {
  //Asks that the generated spectrum for this fluorescent material be histogrammed
  //or not during the runs. Notice that in your set-up, ONLY 1 FLUORESCENT MATERIAL may
  //be histogrammed. So if you ask that this fluorescent material be histogrammed
  //it will be so that no other fluorescent material in the set-up will be histo-
  //grammed.
  if (fLitSpectrum) {
    if (GetFluorescent()) {
      if (tobe) {
        TLitMedium *pmat;
        TIter next(fgLitMedium);
        while ((pmat = (TLitMedium *)next()))
          if ((pmat->GetFluorescent()) && (pmat->fHistFluo))
            pmat->fHistFluo = kFALSE;
        fHistFluo = kTRUE;
      }
      else fHistFluo = kFALSE;
    }
  }//end if (fLitSpectrum)
}
void TLitMedium::SetNonPropLightYield(const char* name) {
  // Inorganic scintillators are often found to have a light yield which is not
  //proportional to energy of the incoming radiation particle. In the literature
  //this effect is referred to as the non-proportionality of the light yield.
  // Organic scintillators are known to have a light yield which varies with
  //ionistation density which can be accounted for using Birk's formula.
  // The SetNonPropLightYield(const char* name) function provides a method to
  //correct for these effects if desired
  TString s;
  TString snone = "";
  if (!snone.CompareTo(name)) {
    fHasNPLY = kFALSE;
    fFitNPLY   = 0;
  }
  else {
    fFitNPLY = TSplineFit::FindFit(name);
    if (fFitNPLY) {
      fNameNPLY = name;
      fHasNPLY = kTRUE;}
    else {
      gCleanOut->MM(error,"SetNonPropLightYield","Fit for non proportionality not found",ClassName());
      gCleanOut->MM(info,"SetNonPropLightYield","No non proportionality assumed",ClassName());
      fHasNPLY = kFALSE;
      fFitNPLY   = 0;
    }
  }
}
void TLitMedium::SetPhotMev(Double_t photmev, Double_t resScale) {
  // Method to set the intrinsic light yield (photmev) and the resolution
  //scale (resScale).
  // If you declare your material as non-proportional, photmev will be scaled
  //according to the TSplinefit fFitNPLY.
  // The default value for resScale = 1.0
  fPhotMev         = photmev; 
  fResolutionScale = resScale;
}
void TLitMedium::SetRayleighLength(Double_t rl, Bool_t destroy) {
// Declares that this material has Rayliegh scattering. The value of the 
//Rayleigh scattering length is Double_t rl (the first argument). The second
//argument specifies whether to delete the fit fFitRayleighL. 
// Rayleigh scattering refers to the process of scattering of light off 
//particles much smaller than the wavelength of the photons, such as molecules
//or atoms. It differs from other scattering in that the photons do not 
//scatter isotropically, as is implemented for Diffusion in SLitrani. Rayleigh 
//scattering is intrinsic to all materials, though often the scattering length
//is so long that it can be neglected.
  fRayleighLength = rl;
  if (fFitRayleighL) {
    if (destroy) delete fFitRayleighL;
    fFitRayleighL = 0;
  }
  if (fRayleighLength > 0.0) fHasRayleigh = kTRUE;
}
void TLitMedium::SetRayleighLength(const char *name) {
// Declares that this material has Rayliegh scattering. The value of the 
//Rayleigh scattering length is given by the TSplineFit with name "name". 
//Rayleigh scattering refers to the process of scattering of light off 
//particles much smaller than the wavelength of the photons, such as molecules
//or atoms. It differs from other scattering in that the photons do not 
//scatter isotropically, as is implemented for Diffusion in SLitrani. Rayleigh 
//scattering is intrinsic to all materials, though often the scattering length
//is so long that it can be neglected.
  TString s;
  if (fFitRayleighL) delete fFitRayleighL;
  fFitRayleighL = TSplineFit::FindFit(name);
  if (!fFitRayleighL) {
    gCleanOut->MM(error,"SetRayleighLength","Fit for Rayleigh scattering length not found",ClassName());
    gCleanOut->MM(fatal,"SetRayleighLength","Define fit for Rayleigh scattering before TLitMedium",ClassName());
    return;
  }
  fNameRayleighL = name;
  fHasRayleigh = kTRUE;
}
void TLitMedium::SetWLS(TLitSpectrum* sp, Double_t wlsl) {
// Set the wavelength independent characteristic length for wavelength 
//shifting and the emission spectrum. The second argument is the wavelength 
//shifting length to be applied for all wavelengths. The first argument is 
//the pointer to the re-emission TLitSpectrum.
  fWLSSpectrum = sp;
  if (!fWLSSpectrum)
    gCleanOut->MM(fatal,"SetWLS","Specified WLS spectrum not found",ClassName());
  fWLSL = wlsl;
  fHasWLS = kTRUE;
}
void TLitMedium::SetWLS(TLitSpectrum* sp, const char* wlslFitName) {
// Set the wavelength dependent characteristic length for wavelength shifting 
//and the emission spectrum. The second argument is the name of the TSplineFit
//to be used for the wavelength dependent WLS length to be used. The first 
//argument is pointer to the re-emission TLitSpectrum.
//
  fWLSSpectrum = sp;
  if (!fWLSSpectrum)
    gCleanOut->MM(fatal,"SetWLS","Specified WLS spectrum not found",ClassName());
  fNameWLSL = wlslFitName;
  fFitWLSL = TSplineFit::FindFit(fNameWLSL);
  if (!fFitWLSL)
    gCleanOut->MM(fatal,"SetWLS","WLS absorption length fit not found",ClassName());
  fHasWLS = kTRUE;
}
void TLitMedium::SetWrapIndex(Double_t ri,Double_t ii) {
  //  Change the complex index of refraction of the wrapping and replaces
  // it with (ri,ii). Be careful! index of refraction is then no more
  // dependent upon wavelength!
  if (fKind==OpticWrapping) {
    fWrapRindex     = TMath::Abs(ri);
    fWrapNameRindex = "";
    fWrapFitRindex  = 0;
    //Notice the minus sign for fWrapIindex
    fWrapIindex     = -TMath::Abs(ii);
    fWrapNameIindex = "";
    fWrapFitIindex  = 0;
  }
  else gCleanOut->MM(warning,"SetWrapIndex","Medium is not a wrapping",ClassName());
}
void TLitMedium::SetXSectnPE(const char* name) {
  //  Set the name of the TSplineFit for the photoelectric effect
  //  In cases where you do not have access to a model or a table,
  // you may like to look at the XCOM page of the NIST (Photon cross
  // sections for compounds and mixtures of the National Institute of
  // Standards and Technology (USA)
  //
  // http://physics.nist.gov/PhysRefData/Xcom/html/xcom1.html
  //
  // Units of the cross section must be entered as
  //  sigma = (Atomic number * (cm2/g))/0.602
  // Sigma is x10^+24 cm2
  //
  // For an element this is equivalent to barns/atom
  fFitPE = TSplineFit::FindFit(name);
  if (fFitPE) {
    fNamePE = name;
    fPEOk   = kTRUE;
  }
  else {
    gCleanOut->MM(error,"SetXSectnPE","Fit for photo-electric XSectn not found",ClassName());
    gCleanOut->MM(info,"SetXSectnPE","photo-electric XSectn put to 0",ClassName());
    fFitPE  = 0;
    fPEOk   = kFALSE;
  }
}
void TLitMedium::SetXSectnPE(TSplineFit *fit) {
  //  Set the TSplineFit for the photoelectric cross-section
  //  In cases where you do not have access to a model or a table,
  // you may like to look at the XCOM page of the NIST (Photon cross
  // sections for compounds and mixtures of the National Institute of
  // Standards and Technology (USA)
  //
  // http://physics.nist.gov/PhysRefData/Xcom/html/xcom1.html
  //
  // Units of the cross section must be entered as
  //  sigma = (Atomic number * (cm2/g))/0.602
  // Sigma is x10^+24 cm2
  //
  // For an element this is equivalent to barns/atom
  if (fit->GetCategory()==18) {
    fFitPE = fit;
    fNamePE = fFitPE->GetName();
    fPEOk   = kTRUE;
  }
  else {
    gCleanOut->MM(error,"SetXSectnPE","Bad category of fit",ClassName());
    gCleanOut->MM(error,"SetXSectnPE","No photo-electric effect",ClassName());
    fFitPE  = 0;
    fPEOk   = kFALSE;
  }
}
void TLitMedium::WaveAndLife(Double_t &wavelength,Double_t &lifetime,Int_t &kfluo) {
//  Generates new values for wavelength and lifetime of a new photon according to the
// fluorescent characteristics of this material, pointed to by fLitSpectrum.
// Returns wavelength in nanometers and lifetime in picoseconds
  kfluo = fLitSpectrum->GenWaveLAndDecayT(wavelength,lifetime);
  lifetime *= 1000;
}
void TLitMedium::WithThinSlice(TLitMedium *nextmed,Double_t width,const char *nameIndex,const char *nameMu) {
  //  Declares that this TLitMedium, if contiguous with TLitMedium nextmed, implies
  // the crossing of a thin slice whose characteristics are:
  //
  //   nextmed   pointer to TLitMedium after the thin slice. If the next TLitMedium
  //             to be encountered is nextmed, then there is a thin slice, else not!
  //             If you give nextmed == 0, it will have the effect of killing the
  //             thin slice eventually created by a previous call to WithThinSlice(),
  //             putting fTSThinSlice to false and fTSNextMed = 0.
  //   width     true width of the thin slice in nanometers
  //   nameIndex name of the fit ( of type TSplineFit ) giving index of refraction
  //              of thin slice as a function of wavelength
  //   nameMu    name of the fit ( of type TSplineFit ) giving magnetic
  //              permeability mu of thin slice as a function of wavelength
  //
  //  If "this" and "nextmed" are contiguous and if there is a thin slice between the
  // two, it is not necessary to call TLitMedium::WithThinSlice() twice, once for "this"
  // and once for "nextmed". Calling it once, for "this" OR for "nextmed" is enough.
  //  Neither "this", nor "nextmed" can be a wrapping. There are thin slices only
  // between two normal optical tracking media
  //
  const Double_t dixm7 = 1.0e-7;
  const char *met = "WithThinSlice";
  fTSThinSlice = kFALSE;
  fTSNextMed   = 0;
  if (nextmed) {
    if ((!IsWrapping()) && (!nextmed->IsWrapping())) {
      fTSThinSlice = kTRUE;
      fKind        = OpticMediumWithTS;
      fTSNextMed   = nextmed;
      fTSWidth     = dixm7*TMath::Abs(width);
      fTSIndex     = -1.0;
      fTSNameIndex = nameIndex;
      fTSFitIndex  = TSplineFit::FindFit(nameIndex,1);
      if (!fTSFitIndex)
        gCleanOut->MM(fatal,"WithThinSlice","Fit for index of refr of thin slice not found",ClassName());
      fTSNameIndex = nameIndex;
      fTSMu        = -1.0;
      fTSFitMu = TSplineFit::FindFit(nameMu,14);
      if (!fTSFitMu)
        gCleanOut->MM(fatal,"WithThinSlice","Fit for mu of thin slice not found",ClassName());
      fTSNameMu    = nameMu;
      if (!fTSNextMed->GetTSNextMed())
        fTSNextMed->WithThinSlice(this,width,nameIndex,nameMu);
    }//end if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    else {
      gCleanOut->MM(error,met,"Wrappings not allowed contiguous to thin slices",ClassName());
    }//end else if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    if (!fTSThinSlice) gCleanOut->MM(error,met,"Thin slice refused",ClassName());
  }//end if (nextmed)
}
void TLitMedium::WithThinSlice(TLitMedium *nextmed,Double_t width,const char *nameIndex,Double_t mu) {
  //  Declares that this TLitMedium, if contiguous with TLitMedium nextmed, implies
  // the crossing of a thin slice whose characteristics are:
  //
  //   nextmed   pointer to TLitMedium after the thin slice. If the next TLitMedium
  //             to be encountered is nextmed, then there is a thin slice, else not!
  //             If you give nextmed == 0, it will have the effect of killing the
  //             thin slice eventually created by a previous call to WithThinSlice(),
  //             putting fTSThinSlice to false and fTSNextMed = 0.
  //   width     true width of the thin slice in nanometers
  //   nameIndex name of the fit ( of type TSplineFit ) giving index of refraction
  //              of thin slice as a function of wavelength
  //   mu        fixed value of magnetic permeability of thin slice
  //
  // If "this" and "nextmed" are contiguous and if there is a thin slice between the
  //two, it is not necessary to call TLitMedium::WithThinSlice() twice, once for "this"
  //and once for "nextmed". Calling it once, for "this" OR for "nextmed" is enough.
  //  Neither "this", nor "nextmed" can be a wrapping. There are thin slices only
  // between two normal optical tracking media
  //
  const Double_t dixm7 = 1.0e-7;
  const char *met = "WithThinSlice";
  fTSThinSlice = kFALSE;
  fTSNextMed   = 0;
  if (nextmed) {
    if ((!IsWrapping()) && (!nextmed->IsWrapping())) {
      fTSThinSlice = kTRUE;
      fKind        = OpticMediumWithTS;
      fTSNextMed   = nextmed;
      fTSWidth     = dixm7*TMath::Abs(width);
      fTSIndex     = -1.0;
      fTSFitIndex  = TSplineFit::FindFit(nameIndex,1);
      if (!fTSFitIndex)
        gCleanOut->MM(fatal,"WithThinSlice","Fit for index of refr of thin slice not found",ClassName());
      fTSNameIndex = nameIndex;
      fTSMu        = mu;
      fTSNameMu    = "";
      fTSFitMu     = 0;
      if (!fTSNextMed->GetTSNextMed())
        fTSNextMed->WithThinSlice(this,width,nameIndex,mu);
    }//end if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    else {
      gCleanOut->MM(error,met,"Wrappings not allowed contiguous to thin slices",ClassName());
    }//end else if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    if (!fTSThinSlice) gCleanOut->MM(error,met,"Thin slice refused",ClassName());
  }//end if (nextmed)
}
void TLitMedium::WithThinSlice(TLitMedium *nextmed,Double_t width,Double_t index,const char *nameMu) {
  //  Declares that this TLitMedium, if contiguous with TLitMedium nextmed, implies
  // the crossing of a thin slice whose characteristics are:
  //
  //   nextmed   pointer to TLitMedium after the thin slice. If the next TLitMedium
  //             to be encountered is nextmed, then there is a thin slice, else not!
  //             If you give nextmed == 0, it will have the effect of killing the
  //             thin slice eventually created by a previous call to WithThinSlice(),
  //             putting fTSThinSlice to false and fTSNextMed = 0.
  //   width     true width of the thin slice in nanometers
  //   index     fixed value of index of refraction of thin slice
  //   nameMu    name of the fit ( of type TSplineFit ) giving magnetic
  //              permeability mu of thin slice as a function of wavelength
  //
  // If "this" and "nextmed" are contiguous and if there is a thin slice between the
  //two, it is not necessary to call TLitMedium::WithThinSlice() twice, once for "this"
  //and once for "nextmed". Calling it once, for "this" OR for "nextmed" is enough.
  //  Neither "this", nor "nextmed" can be a wrapping. There are thin slices only
  // between two normal optical tracking media
  //
  const Double_t dixm7 = 1.0e-7;
  const char *met = "WithThinSlice";
  fTSThinSlice = kFALSE;
  fTSNextMed   = 0;
  if (nextmed) {
    if ((!IsWrapping()) && (!nextmed->IsWrapping())) {
      fTSThinSlice = kTRUE;
      fKind        = OpticMediumWithTS;
      fTSNextMed   = nextmed;
      fTSWidth     = dixm7*TMath::Abs(width);
      fTSIndex     = index;
      fTSNameIndex = "";
      fTSFitIndex  = 0;
      fTSMu        = -1.0;
      fTSFitMu = TSplineFit::FindFit(nameMu,14);
      if (!fTSFitMu)
        gCleanOut->MM(fatal,"WithThinSlice","Fit for mu of thin slice not found",ClassName());
      fTSNameMu    = nameMu;
      if (!fTSNextMed->GetTSNextMed())
        fTSNextMed->WithThinSlice(this,width,index,nameMu);
    }//end if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    else {
      gCleanOut->MM(error,met,"Wrappings not allowed contiguous to thin slices",ClassName());
    }//end else if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    if (!fTSThinSlice) gCleanOut->MM(error,met,"Thin slice refused",ClassName());
  }//end if (nextmed)
}
void TLitMedium::WithThinSlice(TLitMedium *nextmed,Double_t width,Double_t index,Double_t mu) {
  // Declares that this TLitMedium, if contiguous with TLitMedium nextmed, implies
  //the crossing of a thin slice whose characteristics are:
  //
  //   nextmed   pointer to TLitMedium after the thin slice. If the next TLitMedium
  //             to be encountered is nextmed, then there is a thin slice, else not!
  //             If you give nextmed == 0, it will have the effect of killing the
  //             thin slice eventually created by a previous call to WithThinSlice(),
  //             putting fTSThinSlice to false and fTSNextMed = 0.
  //   width     true width of the thin slice in nanometers
  //   index     fixed value of index of refraction of thin slice
  //   mu        fixed value of magnetic permeability of thin slice
  //
  // If "this" and "nextmed" are contiguous and if there is a thin slice between the
  //two, it is not necessary to call TLitMedium::WithThinSlice() twice, once for "this"
  //and once for "nextmed". Calling it once, for "this" OR for "nextmed" is enough.
  //  Neither "this", nor "nextmed" can be a wrapping. There are thin slices only
  // between two normal optical tracking media
  //
  const Double_t dixm7 = 1.0e-7;
  const char *met = "WithThinSlice";
  fTSThinSlice = kFALSE;
  fTSNextMed   = 0;
  if (nextmed) {
    if ((!IsWrapping()) && (!nextmed->IsWrapping())) {
      fTSThinSlice = kTRUE;
      fKind    = OpticMediumWithTS;
      fTSNextMed   = nextmed;
      fTSWidth = dixm7*TMath::Abs(width);
      fTSIndex     = index;
      fTSNameIndex = "";
      fTSFitIndex  = 0;
      fTSMu        = mu;
      fTSNameMu    = "";
      fTSFitMu     = 0;
      if (!fTSNextMed->GetTSNextMed())
        fTSNextMed->WithThinSlice(this,width,index,mu);
    }//end if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    else {
      gCleanOut->MM(error,met,"Wrappings not allowed contiguous to thin slices",ClassName());
    }//end else if ((!IsWrapping()) && (!nextmed->IsWrapping()))
    if (!fTSThinSlice) gCleanOut->MM(error,met,"Thin slice refused",ClassName());
  }//end if (nextmed)
}
