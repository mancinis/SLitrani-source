// @(#)root/html:$Id: TLitVolume.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TComplex.h"
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TGeoEltu.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TCleanOut.h"
#include "TLit.h"
#include "TLitDetector.h"
#include "TLitMedium.h"
#include "TLitVolume.h"

ClassImp(TLitVolume)
//______________________________________________________________________________
//
/*BEGIN_HTML
		<div align="center">
			<h2>Description of TLitVolume</h2>
		</div>
		<p>Addition to <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
needed by <b><code>SLitrani</code></b>. <b><code>TLitVolume</code></b> <font color="#ce0000"><b>does 
not</b></font> inherit from <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
It is <b><font color="#ce0000">linked with</font></b> a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
by the &quot;hook&quot; <b><code>fField</code></b> pointer provided in <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
A <b><code>TLitVolume</code></b> has same name and title as the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
with which it is linked. It is not always necessary to link a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
with a <b><code>TLitVolume</code></b>. It is only necessary if:</p>
		<ol>
			<li>You want to make finite steps during tracking photons inside this <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>, 
and not simply jump to the next boundary.
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
contains a <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> <i>which 
is anisotropic.</i>
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is a detector.			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
has suffered from radiation damages.	
			<li>The <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
is a source of photons, a photon cradle.
		</ol>
		<p>A <b><code>TLitVolume</code></b> contains among its class variables various 
points and vectors which are defined <i><b>in the local coordinate system</b></i> 
of the associated <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
With respect to this local coordinate system, these points and vectors <b><i>are 
fixed</i></b>. Now the same <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
can be inserted <b><i>at many different places and with many different orientations</i></b> 
in the setup.&nbsp;During tracking, we need these points and vectors to be defined 
in <b><code>WCS</code></b> [ World Coordinate System, the coordinate system of the 
<b><code>TOP</code></b> volume]. Depending upon which instance [node] of this <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
the track [photon] is in during tracking, the transformation from local coordinate 
system to <b><code>WCS</code></b> coordinate system is different, so that we have 
to make it anew each time we enter a <b><code>TLitVolume</code></b>. This is done 
by the method <b><code><a href="#TLitVolume:SetWCS">TLitVolume::SetWCS()</a></code></b>. 
But this method only works if the current track has entered a specific node of the 
<b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
associated with this <b><code>TLitVolume</code></b>. If one needs to have for instance 
the coordinate of the point of emission and of the direction of emission of a not 
yet created photon, it is necessary to specify the path, for instance <b><code>&quot;/TOP_1/REV_2/CRYSTAL_1&quot;</code></b> 
of the specific instance of the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
which will be used at this time to generate photons. This specific path is stored 
in the class variable <b><code>fPath</code></b>, and thanks to it, the transformation 
into the <b><code>WCS</code></b> system can be done. The method <b><code><a href="#TLitVolume:EmissionToWCS">TLitVolume::EmissionToWCS()</a> 
</code></b>does it.</p>
		<p>This is an important difference between the new <b><code>SLitrani</code></b> 
and the old one.&nbsp;In the very simple geometry used by the old <b><code>SLitrani</code></b>, 
there was only one instance for each defined shape, so that it was possible, at the 
end of the definition of the geometry, to ask for a global change from local coordinate 
system to <b><code>WCS</code></b> for all shapes of the setup.&nbsp;For the reasons 
explained above, it is not possible to proceed like this in the new <b><code>SLitrani</code></b> 
using <b><code>TGeo</code></b>. As a consequence, it is also important to notice 
that if you have associated a <b><code>TLitVolume</code></b> to a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
in order for instance to declare it as being a source of photons ( or as being anisotropic, 
or as being a detector, or as having suffered of radiation damages), <b><font color="#ce0000">all 
</font></b>instances [nodes] of this <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
will have this property.</p>
		<h3>Matrix for Optical Anisotropy</h3>
		<p>Let <b>Oxyz</b> be<font color="#9c0031"><i> </i></font><b><code>WCS</code></b> 
<i><font color="#9c0031">World Coordinate System, the coordinate system of the <b><code>TOP</code></b> 
volume</font></i>, <b>Ox'y'z'</b> be <font color="#9c0031"><cite>the system in which 
the dielectric tensor is diagonal</cite></font>, with eigenvalues <font face="Symbol"><b>e</b></font><sub>a</sub>, 
<font face="Symbol"><b>e</b></font><sub>b</sub> and <font face="Symbol"><b>e</b></font><sub>c</sub> 
. &nbsp;We call the <b>Ox'y'z'</b> system <font color="#9c0031">&quot;<cite>the system 
of the 3 optical axis</cite>&quot;</font>. By convention, <font face="Symbol"><b>e</b></font><sub>a</sub> 
&lt;= <font face="Symbol"><b>e</b></font><sub>b</sub> &lt;= <font face="Symbol"><b>e</b></font><sub>c</sub> 
. By convention also, <font face="Symbol"><b>e</b></font><sub>a</sub> is associated 
with the <b>Ox' </b>axis, <font face="Symbol"><b>e</b></font><sub>b</sub> with the 
<b>Oy'</b> axis and <font face="Symbol"><b>e</b></font><sub>c</sub> with the <b>Oz</b>' 
axis. Let <font color="#9c0031"><b>M</b></font> be the rotation matrix which converts 
the <b>Ox, Oy, Oz </b>axis to the <b>Ox'</b>, <b>Oy'</b> and <b>Oz' </b>axis :</p>
		<center>
			<p>
			<table border="2" cellpadding="2">
				<tr>
					<td><b>Ox' </b>= <font color="#9c0031"><b>M</b></font> <b>Ox</b></td>
				</tr>
				<tr>
					<td><b>Oy'</b> = <font color="#9c0031"><b>M</b></font> <b>Oy</b></td>
				</tr>
				<tr>
					<td><b>Oz'</b> = <font color="#9c0031"><b>M</b></font> <b>Oz</b></td>
				</tr>
			</table>
		</center>
		<p>If you have given</p>
		<ul>
			<li>the <b>Ox' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font> ) = theta<font size="1"><sub>x</sub></font> 
and phi<font size="1"><sub>x</sub></font> angle of the <b>Ox'</b> axis as seen in 
the <b>Oxyz</b> coordinate system
			<li>the <b>Oy' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font> ) = theta<font size="1"><sub>y</sub></font> 
and phi<font size="1"><sub>y</sub></font> angle of the <b>Oy'</b> axis as seen in 
the <b>Oxyz</b> coordinate system
			<li>the <b>Oz' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font> ) = theta<font size="2"><sub>z 
</sub></font><sub>&nbsp;</sub>and phi<font size="2"><sub>z</sub></font> angle of 
the <b>Oz'</b> axis as seen in the <b>Oxyz</b> coordinate system
		</ul>
		<p>Then we have :</p>
		<center>
			<p>
			<table border="2" cellpadding="2">
				<caption><font color="#9c0031"><b>M</b></font> matrix</caption>
				<tr>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font></td>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font></td>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font></td>
				</tr>
				<tr>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font></td>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font></td>
					<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font></td>
				</tr>
				<tr>
					<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font></td>
					<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font></td>
					<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font></td>
				</tr>
			</table>
		</center>
		<p>If <b>E</b> is a vector expressed in the <b><code>WCS</code></b> <b>Oxyz</b> 
and <b>E'</b> is the same vector expressed in the system where the dielectric tensor 
is diagonal , <cite>the system </cite><b>Ox'y'z'</b> <cite>of the optical axis,</cite> 
then : &nbsp;<b>E</b> = <font color="#9c0031"><b>M</b></font> <b>E'</b> &nbsp;( and 
not <b>E'</b> = <font color="#9c0031"><b>M</b></font> <b>E </b>! ) . This is the 
well known fact that vectors are contravariant if axis vector are covariant. &nbsp;Now 
let us take the Maxwell equation in both systems ( <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
diagonal matrix ):</p>
		<center>
			<p>
			<table border="1" cellpadding="2">
				<tr>
					<td></td>
					<td>In System <b><code>WCS </code>Oxyz</b></td>
					<td>In system <b>Ox'y'z'</b></td>
				</tr>
				<tr>
					<td>Maxwell equation</td>
					<td><font color="black"><b>D</b></font> = e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font> 
<font color="black"><b>E</b></font></td>
					<td><font color="black"><b>D'</b></font> = e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
<font color="black"><b>E' </b></font></td>
				</tr>
				<tr>
					<td>Transform</td>
					<td><font color="black"><b>E</b></font> = <font color="#9c0031"><b>M</b></font> 
<font color="black"><b>E' </b></font>&nbsp; &nbsp; &nbsp; &nbsp; <font color="black"><b>D</b></font>= 
<font color="#9c0031"><b>M</b></font><font color="black"><b> D'</b></font></td>
					<td><font color="black"><b>E' </b></font>= <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="black"><b> 
E</b></font>&nbsp; &nbsp;&nbsp; <font color="black"><b>D'</b></font>= <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
<font color="black"><b>D</b></font></td>
				</tr>
				<tr>
					<td>Maxwell equation rewritten</td>
					<td><font color="#9c0031"><b>M</b></font> <font color="black"><b>D'</b></font>= 
e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font color="#9c0031"> 
<b>M</b></font> <font color="black"><b>E'</b></font>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp;==&gt;</td>
					<td><font color="black"><b>D'</b></font>= e<sub>0</sub> <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="#9c0031"><b> 
</b></font><b><font face="Symbol" color="#9c0031" size="4">e</font><font color="#9c0031"> 
M</font></b> <font color="black"><b>E' </b></font></td>
				</tr>
				<tr>
					<td></td>
					<td><font face="Symbol" color="#9c0031" size="4"><b>e</b></font> = <font color="#9c0031"><b>M 
</b></font><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font><b><font color="#9c0031"> 
M</font></b><font color="#9c0031"> </font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&lt;==</td>
					<td><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
=<font color="#9c0031"> <b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="#9c0031"><b> 
</b></font><b><font face="Symbol" color="#9c0031" size="4">e</font><font color="#9c0031"> 
M</font></b></td>
				</tr>
			</table>
		</center>
		<p>This implies that knowing the dielectic tensor <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
in the system where it is diagonal with diagonal elements <font face="Symbol"><b>e</b></font><sub>a</sub>, 
<font face="Symbol"><b>e</b></font><sub>b</sub> and <font face="Symbol"><b>e</b></font><sub>c</sub> 
&nbsp;, we get the dielectric tensor in the <b><code>WCS</code></b> coordinate system 
through : <font face="Symbol" color="#9c0031" size="4"><b>e</b></font> = <font color="#9c0031"><b>M 
</b></font><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font><b><font color="#9c0031"> 
M</font></b><font color="#9c0031"> </font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
.</p>
		<h4>Note concerning opticians jargon.</h4>
		<p>Like Moli&egrave;re's physicians, opticians do have a jargon which can be misleading. 
We call here the <b>Ox'y'z'</b> system, the system in which the dielectric tensor 
is diagonal, <font color="#9c0031">&quot;<cite>the system of the 3 optical axis</cite>&quot;</font>. 
This is not to be confused with what is called <font color="#9c0031"><i>&quot;the 
optical axis&quot;</i></font>. <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> 
has a meaning only in the case of birefringence, where either <font face="Symbol"><b>e</b></font><sub>a</sub> 
= <font face="Symbol"><b>e</b></font><sub>b</sub> or <font face="Symbol"><b>e</b></font><sub>b</sub>&nbsp;= 
<font face="Symbol"><b>e</b></font><sub>c</sub> . <font color="#9c0031"><i>&quot;The 
optical axis&quot;</i></font> &nbsp;is a unique axis, corresponding to the diagonal 
element which is isolated. So</p>
		<ul>
			<li>if <font face="Symbol"><b>e</b></font><sub>a</sub> = <font face="Symbol"><b>e</b></font><sub>b</sub> 
then <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> is <b>Oz'</b> 
.
			<li>if <font face="Symbol"><b>e</b></font><sub>b</sub>&nbsp;= <font face="Symbol"><b>e</b></font><sub>c</sub> 
then <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> is <b>Ox'</b> 
.
		</ul>
		<p></p>
		<h3>Transforming a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
into a detector</h3>
		<p>For this, you have to create a <b><code>TLitVolume</code></b>, hooked with the 
<b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
having to upgrade to a detector. Then you have to call one of the methods <b><code><a href="#TLitVolume:SetDetector">TLitVolume::SetDetector()</a></code></b> 
or <b><code><a href="#TLitVolume:SetPhotoTube">TLitVolume::SetPhotoTube()</a></code></b> 
or <b><code><a href="#TLitVolume:SetAPD">TLitVolume::SetAPD()</a></code></b>. Notice 
that in the new <b><code>SLitrani</code></b>, there does not exist anymore such thing 
as a &quot;surface detector&quot;. <b><font color="#ce0000">All detectors are volumes</font></b>.&nbsp;What 
to do if you really need a &quot;surface detector&quot;? Then define a very thin 
volume, [ but not so thin as risking not to be seen by the navigation algorithms, 
let us say bigger than 10<sup>-5</sup> cm ], which will act as a surface detector. 
For instance, if you have a <b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b> 
filled with some <b><code><a href="TLitMedium.html">TLitMedium</a></code></b>, and 
you want that one of the faces of this <b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b> 
be a surface detector, then define an other <b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b>, 
filled with the same <b><code><a href="TLitMedium.html">TLitMedium</a></code></b>, 
having 2 of its dimensions equal to the dimension of the face intended to be a  surface 
detector, having the 3rd dimension very thin, and put this second <b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b> 
in contact with the face intended to be a surface detector. Then declare this second 
<b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b> 
as being a detector by calling one of the three methods quoted above.</p>
		<p>All <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
in the new <b><code>SLitrani</code></b> can upgrade to the state of a detector by 
hooking a <b><code>TLitVolume </code></b>4 types of detector may be defined :</p>
		<ul>
			<li><b><code>none : </code></b>this <b><code>TLitVolume</code></b> is not a detector
			<li><b><code>volumedet : </code></b>General kind of detector. See the method <b><code><a href="#TLitVolume:SetDetector">TLitVolume::SetDetector()</a></code></b>. 
One can specify either:
			<ul>
				<ul>
					<li>[ <b><code>fStopAlways = kFALSE</code></b> ] that a photon <b><font color="#ce0000">only</font></b> 
stops inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
because of its finite absorption length, or
					<li>[ <b><code>fStopAlways = kTRUE</code></b>   ] that a photon reaching this 
<b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
<b><font color="#ce0000">always stops</font></b> in the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
being a <b><code>volumedet</code></b>, and is detected if conditions on photocathode 
efficiency and acceptance angle [if asked for] are satisfied.
				</ul>
				<li>One can associate
				<ul>
					<li>an equivalent of photocathode efficiency, a &quot;volume efficiency&quot;, 
<i>depending upon wavelength</i>. This dependency is given by a <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></code></b> 
of category 10.
					<li>an acceptance angle condition for the photon. This acceptance angle is with 
reference with an axis given by the user in the local frame of the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
: <b><code>fDetAxis</code></b>.
				</ul>
			</ul>
			<li><b><code>phototube : </code></b> See the method<b><code> <a href="#TLitVolume:SetPhotoTube">TLitVolume::SetPhotoTube(). 
</a></code></b>A phototube is a special case of <b><code>volumedet</code></b>, with 
the following conditionss: 
			<ul>
				<li>the associated shape is a <b><code><a href="http://root.cern.ch/root/html/TGeoTube.html">TGeoTube</a></code></b> 
without hole representing the photocathode. Before this <b><code><a href="http://root.cern.ch/root/html/TGeoTube.html">TGeoTube</a></code></b> 
without hole is an other <b><code><a href="http://root.cern.ch/root/html/TGeoTube.html">TGeoTube</a></code></b> 
without hole representing the entrance window of the phototube. The <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> 
associated with the photocathode<b><font color="#ce0000"> is the same</font></b> 
as the <b><code><a href="TLitMedium.html">TLitMedium</a></code></b> filling the entrance 
window, in order that the photon goes dirctly from one to the other. <b><font color="#ce0000">Do 
not</font></b> fill the photocathode <b><code><a href="http://root.cern.ch/root/html/TGeoTube.html">TGeoTube</a></code></b> 
with the true material of the photocathode, but with the material of the entrance 
window! 
				<li>the photon always stops in the phototube: <b><code>fStopAlways = kTRUE.</code></b>
				<li><b><font color="#ce0000">There is</font></b> a condition of cathode efficiency, 
given by a <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></code></b> 
of category 10.
				<li><b><font color="#ce0000">there is not</font></b> a condition on acceptance 
angle. <b><code>fDetAxis</code></b> does not need to be defined.
			</ul>
			<li><b><code>apd : </code></b>See method <b><code><a href="#TLitVolume:SetAPD">TLitVolume::SetAPD()</a></code></b>. 
An <b><code>APD</code></b> is a more complicated volume detector.
			<ul>
				<li>It must be associated with a <b><code><a href="http://root.cern.ch/root/html/TGeoTube.html">TGeoTube</a></code></b> 
without hole or a <b><code><a href="http://root.cern.ch/root/html/TGeoBBox.html">TGeoBBox</a></code></b>.

				<li>There is no condition of the type &quot;cathode efficiency&quot;. What matters 
is whether or not the photon stops in the <b><code>APD</code></b> because of its 
finite absorption length.				<li>There is <b><font color="#ce0000">no</font></b> 
condition on acceptance angle, <b><font color="#ce0000">but</font></b>:
				<li><b><code>fDetAxis</code></b> must be defined and indicate the direction from 
the cathode to the anode. The two faces of the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
which are orthogonal to <b><code>fDetAxis</code></b> are the cathode and the anode.
				<li>The photon must stop inside the <b><code>APD</code></b> because of its absorption 
length in Silicium: <b><code>fStopAlways = kFALSE</code></b>.
				<li>A distribution function (of class <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></code></b>) 
gives the number of electrons generated by the stopping of the photon inside the 
volume at a coordinate z with respect to the axis <b><code>fDetAxis </code></b>going 
from cathode to anode. This distribution function is what is called the <b>gain profile</b> 
of the <b><code>APD</code></b>. The photon is considered as detected if it has stopped 
at a z coordinate such that at least 5 electrons have been emitted. This value of 
5 is arbitrary and may be changed by the user. In fact, what really matters here 
is not whether the photon has been detected or not (has emitted more or less than 
5 electrons), but how many electrons have been emitted and at what time they have 
reached the anode. This value is recorded into an histogram which gives directly 
the shape of the pulse emitted by the <b><code>APD</code></b> after the emission 
of all photons of the run. See method <b><code><a href="#TLitVolume:SetAPD">TLitVolume::SetAPD()</a></code></b>.			</ul>
		</ul>
		<p></p>
		<h3><a name="raddam"></a>Simulating radiation damages inside a shape</h3>
		<p>Let us consider a shape having suffered from radiation damages. The radiation 
damages have been produced by electromagnetic or hadronic showers having irradiated 
the shape.&nbsp;Let <b><code><font size="+1" color="#00008b">D</font></code></b> 
[<b>damaxis</b>] be the axis of the showers having caused the damages, and <b><code><font size="+1" color="#00008b">A</font></code></b> 
[<b>damstart</b>] the starting point of these showers. Let <code><font size="+1" color="#00008b"><b>P</b></font></code> 
(x,y,z) be a point inside the shape and <b><code><font size="+1" color="#00008b">Q</font></code></b> 
the projection of this point on the axis <b><code><font size="+1" color="#00008b">D</font></code></b>.&nbsp;We 
call z the distance <b><code><font size="+1" color="#00008b"><u>AQ</u></font></code></b> 
and r the distance <b><code><font size="+1" color="#00008b"><u>PQ</u></font></code></b>. 
We will try to describe the damages as a function of (z,r). Let:</p>
		<div align="center">
			<table border="8" cellpadding="4" cellspacing="2">
				<tr>
					<td width="200"><b><code><font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">) 
== 1/L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b></td>
					<td><b><code><font size="+1">L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b>be 
the absorption length of the material of the shape <b>before</b> radiation damage. 
It is not a function of <b><code><font size="+1">(z,r)</font></code></b>. It is a 
function of the wavelength <b><font face="Symbol">l</font></b>. <b><code><font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b> 
is defined because handier in the final formula than <b><code><font size="+1">L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b>.</td>
					<td></td>
				</tr>
				<tr>
					<td width="200"><b><code><font size="+1">L</font>(z,r,</code><font face="Symbol">l</font><code>)</code></b></td>
					<td colspan="2">be the absorption length of the material of the shape <b>after</b> 
irradiation.&nbsp;It is a function of <b><code>(z,r,</code><font face="Symbol">l</font><code>)</code></b>.</td>
				</tr>
				<tr>
					<td width="200"><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b></td>
					<td colspan="2">be what the absorption length of the material <b>would be</b> 
if the <b>only</b> reasons for a finite absorption length were the irradiation damages. 
See formula (1).</td>
				</tr>
				<tr>
					<td width="200">
						<div align="left">
							<b><code><font size="+1">L</font><sub>i</sub></code><code><sub>min</sub>(</code><font face="Symbol">l</font><code>)<sub> 
</sub>= <font size="+1">L</font><sub>i</sub>(<font size="+1">z<sub>max</sub>,0</font>,</code><font face="Symbol">l</font><code>)</code></b></div>
					</td>
					<td colspan="2"><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b> 
at the point where it is <b>minimal</b>, i.e. at the point where the damages are 
<b>maximal</b>.</td>
				</tr>
				<tr>
					<td width="200">
						<div align="left">
							<b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">) 
= 1/L</font><sub>imin</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b></div>
					</td>
					<td colspan="2"><b><code><font size="+1">K</font><sub>imax</sub></code></b> 
is handier then <b><code><font size="+1">L</font><sub>imin</sub></code></b> since 
<b><code><font size="+1">K</font><sub>imax</sub></code></b> is 0, but <b><code><font size="+1">L</font><sub>imin</sub></code></b> 
is infinite for 0 damage.</td>
				</tr>
				<tr>
					<td width="200">
						<div align="left">
							Now we have:</div>
					</td>
					<td>
						<div align="center">
							<code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font>(z,r,</code><font face="Symbol">l</font><code>) 
= </code></b><code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font><sub>0</sub>(</code><font face="Symbol">l</font><code>) 
+ </code></b><code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b></div>
					</td>
					<td>(1)</td>
				</tr>
				<tr>
					<td width="200">
						<div align="left">
							<b><code><font size="+1">f(z,r)</font></code></b></div>
					</td>
					<td colspan="2">is a distribution which is proportionnal to the damages.&nbsp;It 
is a function of <b><code><font size="+1">(z,r)</font></code></b>. For instance, 
for damages caused by electromagnetic showers, <b><code><font size="+1">f(z,r)</font></code></b> 
can be taken as the distribution of the damaging shower itself: the energy deposited 
at <b><code><font size="+1">(z,r)</font></code></b>. Or we can let the user give 
himself  <b><code><font size="+1">f(z,r)</font></code></b>.</td>
				</tr>
				<tr>
					<td width="200">
						<div align="left">
							<b><code><font size="+1">f<sub>max</sub> = f(z<sub>max</sub>,0)</font></code></b></div>
					</td>
					<td colspan="2">Value of <b><code><font size="+1">f(z,r)</font></code></b> at 
the point where it is maximum</td>
				</tr>
				<tr>
					<td colspan="3">
						<div align="center">We postulate then (hoping it is not too wrong an assumption)</div>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<div align="center">
							<b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>) 
= <font size="+1">L</font><sub>i</sub></code><code><sub>min</sub>(</code><font face="Symbol">l</font><code>)*(<font size="+1">f<sub>max</sub></font>/<font size="+1">f(z,r)</font>)</code></b></div>
					</td>
					<td>(2)</td>
				</tr>
				<tr>
					<td colspan="3">
						<div align="center">
							Which gives for the absorption length, using (1) and (2) :</div>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<div align="center">
							<b><font color="#9c0031"><code><font size="+1">L</font>(z,r,</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
= (<font size="+1">f</font><font size="+1"><sub>max</sub></font><font size="+1">*L</font><sub>0</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)*<font size="+1">L</font><sub>imin</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)<font size="+1">) 
/ (f(z,r)*L</font><sub>0</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
+ <font size="+1">f</font><font size="+1"><sub>max</sub></font>*<font size="+1">L</font><sub>imin</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)<font size="+1">)</font></code></font></b></div>
					</td>
					<td>(3)</td>
				</tr>
				<tr>
					<td colspan="2">
						<div align="center">
							<b><font color="#9c0031"><code><font size="+1">L</font></code><code>(z,r,</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
= </code><code><font size="+1">f</font></code><code><font size="+1"><sub>max </sub>/(</font></code><code><font size="+1">f</font></code><code><font size="+1"><sub>max</sub></font></code><code>*<font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">) 
+ f(z,r)*K</font><sub>imax</sub><font size="+1">(</font></code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code><font size="+1">))</font></code></font></b></div>
					</td>
					<td>(3)</td>
				</tr>
				<tr>
					<td colspan="3">
						<div align="center">
							The idea is now to proceed like this:</div>
					</td>
				</tr>
				<tr>
					<td width="200">damages by electromagnetic showers</td>
					<td colspan="2"><b><code><font size="+1">f(z,r)</font></code></b> taken as the 
energy deposit of the electromagnetic shower as described in the class description 
of <a href="TLitCascade.html">TLitCascade</a>, and in the Review of Particle Physics, 
Vol 3 N&deg;1-4, 1998 pages 148-149. By that, <b><code><font size="+1">f<sub>max</sub></font></code></b> 
is also given. We ask the user to provide <b><code><font size="+1">K</font><sub>imax</sub></code></b> 
either as a constant, not depending upon <b><font face="Symbol">l</font></b>, or 
as a fit describing the dependency of <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b> 
as a function of <b><font face="Symbol">l</font></b>. Nothing else has to be provided.</td>
				</tr>
				<tr>
					<td width="200">other type of damages</td>
					<td colspan="2">We continue to assume even in that case that the damages have 
an axis <b><code><font size="+1" color="#00008b">D</font></code></b> [<b>damaxis</b>] 
and a starting point <b><code><font size="+1" color="#00008b">A</font></code></b> 
[<b>damstart</b>] . We continue to describe the damages as a function of <b><code><font size="+1">(z,r)</font></code></b>. 
We continue to ask the user to provide <b><code><font size="+1">K</font><sub>imax</sub></code></b> 
either as a constant, not depending upon <b><font face="Symbol">l</font></b>, or 
as a fit describing the dependency of <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b> 
as a function of <b><font face="Symbol">l</font></b>. But we ask the user to define 
<b><code><font size="+1">f(z,r)</font></code></b>.&nbsp;For that, we propose 2 methods:</td>
				</tr>
				<tr>
					<td width="200"><b><code><font size="+1">f(z,r)</font></code></b> method 1</td>
					<td>
						<div align="center">
							<p><code><b><font size="+1">f(z,r) = exp(-(z-</font></b></code><b><code><font size="+1">z<sub>m</sub></font></code></b><code><b><font size="+1">)<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))*exp(-r<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))</font></b></code></p>
							<p><b><code><font size="+1">f<sub>max </sub></font></code></b><code><b><font size="+1">= 
1</font></b></code></p>
						</div>
						<p>The user has to provide the 3 constants <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b>. 
He has also to provide <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font> 
</code></b>either as a constant or as a fit describing the dependency upon <b><font face="Symbol">l</font></b>.</p>
					</td>
					<td>(4)</td>
				</tr>
				<tr>
					<td width="200"><b><code><font size="+1">f(z,r)</font></code></b> method 2</td>
					<td>
						<div align="center">
							<p><code><b><font size="+1">f(z,r) = </font></b></code><code><b><font size="+1">g(z)*exp(-r<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))</font></b></code></p>
							<p><b><code><font size="+1">f<sub>max </sub></font></code></b><code><b><font size="+1">= 
g<sub>max</sub></font></b></code></p>
						</div>
						<p>The user is asked to provide a fit for <code><b><font size="+1">g(z)</font></b></code>.&nbsp;This 
fit <b>has to be bounded</b> at a value <code><b><font size="+1">g<sub>max</sub></font></b></code> 
provided by the user. The user has also to provide <b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b>. 
He has also to provide <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font> 
</code></b>either as a constant or as a fit describing the dependency upon <b><font face="Symbol">l</font></b>.</p>
					</td>
					<td>(5)</td>
				</tr>
			</table>
		</div>
		<p></p>
		<p>The methods <code><font size="+1"><b>ShowerDamage() </b></font></code>or <code><font size="+1"><b>RadiationDamage()</b></font></code> 
 implement the processes described above.</p>
		<p></p>
		<h4><font color="#9c0031">ShowerDamage()</font></h4>
		<p>ShowerDamage() is to be preferred when the damages have been caused by the irradiation 
by electromagnetic showers, gammas or electrons. In order that <code><font size="+1"><b>ShowerDamage()</b></font></code> 
can be called for this shape, it is necessary that radiation length, critical energy 
and Moliere radius have been defined for the material of this shape. It means that 
you have called <a href="TLitMedium.html">TLitMedium::SetCascades()</a>. 
</p>
		<ul>
			<li>In the first instance of ShowerDamage(), the user has to provide <b><code><font size="+1">K</font><sub>imax</sub> 
</code></b>as a constant, not depending upon <b><font face="Symbol">l</font></b>.
			<li>In the second instance of the method ShowerDamage()<code><font size="+1"><b>, 
</b></font></code>he has to provide a pointer towards a fit, giving <b><code><font size="+1">K</font><sub>imax</sub> 
</code></b>as a function of wavelength <b><font face="Symbol">l</font></b>.
		</ul>
		<h4><font color="#9c0031">RadiationDamage()</font></h4>
		<p>RadiationDamage() has to be taken in all other cases, when the damages have 
not been caused by electromagnetic showers.&nbsp;For instance, take it when the damages 
have been caused by hadronic showers. </p>
		<ul>
			<li>In the 1st instance of this method, damages are fully described by gaussians, 
as seen in (4). The user has to provide the parameters of these gaussians:  <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
and a constant value for <b><code><font size="+1">K</font><sub>imax</sub></code></b>, 
which is not considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
			<li>In the 2nd instance of this method, damages are fully described by gaussians, 
as seen in (4). The user has to provide the parameters of these gaussians: <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
and a fit for <b><code><font size="+1">K</font><sub>imax</sub>(</code><font face="Symbol">l</font><code>)</code></b>, 
which is considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
			<li>In the 3rd instance of this method, the z dependency is not a gaussian, but 
is described by a fit provided by the user. The r dependency is still described by 
a gaussian, whose sigma <b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
has to be given by the user. The user has to provide a constant value for <b><code><font size="+1">K</font><sub>imax</sub></code></b>, 
which is not considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
			<li>In the 4th instance of this method, the z dependency is not a gaussian, but 
is described by a fit provided by the user. The r dependency is still described by 
a gaussian, whose sigma has to be given by the user. The user has to provide a fit 
for <b><code><font size="+1">K</font><sub>imax</sub>(</code><font face="Symbol">l</font><code>)</code></b>, 
which is considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
		</ul>
		<p></p>
		<h4><font color="#9c0031">Visual checks</font></h4>
		<p>Calling method <b><code><font size="+1">RecordDamages()</font></code></b> before 
any generation of photons and calling <b><code><font size="+1">ShowDamage()</font></code></b> 
after the generation of photons, you will have a visual check of the damage factors 
produced as a function of (z,r).&nbsp;In fact, what is plotted is </p>
		<div align="center">
			<p><b><code><font size="+1">1 - F, where F = L(z,r,</font><font face="Symbol" size="+1">l</font><font size="+1">)/L<sub>0</sub>(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b></p>
		</div>
		<p>Do call these methods only for checks, do not call them inside a production: 
they take time.</p>
		<p></p>
		<h4><font color="#9c0031">Examples of use</font></h4>
		<p>In the <b><code><a href="http://root.cern.ch/twiki/bin/view/ROOT/CINT">CINT</a></code></b> 
macros provided with <b><code>SLitrani</code></b>, you will find:</p>
		<ul>
			<li><b><code><font size="+1">xdamage1.C</font></code></b> which is identical to 
<b><code><font size="+1">xtransmission1.C</font></code></b>, except that we have 
added radiation damages to the crystal

			<li><b><code><font size="+1">xdambeam1.C</font></code></b> which is identical 
to <b><code><font size="+1">xanibeam1.C</font></code></b> plus radiation damages 
to the crystal.

			<li><b><code><font size="+1">xdambeam2.C</font></code></b> which is identical 
to <b><code><font size="+1">xanibeam2.C</font></code></b> plus radiation damages 
to the crystal. 
		</ul>
		<p></p>
		<p></p>
		<div align="center">
			<h2>Photon cradle</h2>
		</div>
		<p></p>
		<p>If a <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
has to be a source of photons, like for instance is the case if we want to generate 
spontaneous photons from a crystal, or if we want to simulate an optical fibre, a 
<b><code>TLitVolume</code></b> has to be associated with the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
which is the source of photons. The wanted characteristics for the emission of photons 
have to be specified by a call to <b><code><a href="#TLitVolume:SetEmission">TLitVolume::SetEmission()</a></code></b>.&nbsp;Look 
at this method. There are 3 possibilities for the point of emission of photons by 
the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>, 
specified with arguments 5-8 of  <b><code><a href="#TLitVolume:SetEmission">TLitVolume::SetEmission()</a></code></b>:</p>
		<ul>
			<li><b>anywhere in <code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>: 
photons are emitted from any point inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
with an equal probability [default]			<li><b>fixed point in <code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
:all photons are emitted from a fixed point inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>			<li><b>surface 
emission</b>: first, a point is generated inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
anywhere with an equal probability. Then the point is translated inside the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>, 
along a specified direction, until the edge of the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
It is the way <b><code>SLitrani</code></b> generates surface emission, without having 
[as the old <b><code>SLitrani</code></b>] any handling of faces. This method has the 
advantage of working, whatever the kind of <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b>. 
It is adequate to simulate emission by an optical fibre. For a full description of 
how to simulate an optical fibre, have a look at the class description of class <b><code><a href="TLitSpontan.html">TLitSpontan</a></code></b>, 
paragraph &quot;Simulating an optical fibre&quot;.		</ul>
		<p>The direction of emission of the photons is specified with arguments 1-4 of 
 <b><code><a href="#TLitVolume:SetEmission">TLitVolume::SetEmission()</a></code></b>:</p>
		<ol>
			<li><b><code>kind   :</code></b> type of distribution for the direction of the 
emitted photons around dir [default <code><b>on4pi</b></code>]
			<ol>
				<li><code><b>on4pi        ==&gt;</b></code> photons are emitted isotropically, 
with the distribution sin(theta)*dtheta*dphi, on 4 pi.
				<li><code><b>flat         ==&gt;</b></code> photons are emitted isotropically, 
with the distribution sin(<font face="Symbol"><b>q</b></font>)*d<font face="Symbol"><b>q</b></font>*d<font face="Symbol"><b>f</b></font>, 
with <font face="Symbol"><b>q</b></font> between 0 and <font face="Symbol"><b>q</b></font>max
				<li><b><code>sinuscosinus ==&gt;</code></b> photons are emitted with the non 
isotropic distribution sin(<font face="Symbol"><b>q</b></font>)*cos(<font face="Symbol"><b>q</b></font>)*d<font face="Symbol"><b>q</b></font>*d<font face="Symbol"><b>f</b></font> 
,with <font face="Symbol"><b>q</b></font> between 0 and <font face="Symbol"><b>q</b></font>max, 
favouring slightly the forward direction
				<li><b><code>provided     ==&gt;</code></b> photons are emitted using the distribution 
provided as a fit of type <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></code></b>.
			</ol>
			<li><b><code><font face="Symbol">q</font>max   : </code></b>maximum value for 
the theta angle around dir. tmax is given in degree [default 180.0]
			<li><b><code>dir    :</code></b> axis around which photons are emitted, in local 
coordinate system of TGeoVolume [default (0,0,1)
			<li><b><code>fit    :</code></b> in case kind == provided, fit to be used [default 
0]
		</ol>
		<p>The characteristics of the <b>E</b> field of the photons at emission are specified 
by a call to <b><code><a href="#TLitVolume:SetEField">TLitVolume::SetEField()</a></code></b>. 
If no polarization is required, you do not need to call it.</p>
		<p>After having specified the wanted characteristics by a call to <b><code><a href="#TLitVolume:SetEmission">TLitVolume::SetEmission()</a></code></b> 
and <b><code><a href="#TLitVolume:SetEField">TLitVolume::SetEField()</a></code></b> 
you initialize one photon by a call to <b><code><a href="#TLitVolume:Gen">TLitVolume::Gen(path)</a></code></b>, 
where <b><code>path</code></b> [for instance <b><code>&quot;/TOP_1/REV_2/CRYSTAL_1&quot;</code></b>] 
is the path of the specific instance [node] of the <b><code><a href="http://root.cern.ch/root/html/TGeoVolume.html">TGeoVolume</a></code></b> 
in which you want to generate this photon.</p>
		<p></p>
END_HTML */
//

TObjArray *TLitVolume::fgLitVolume  = 0;

TLitVolume::TLitVolume(TGeoVolume *vol) {
  // Constructor giving the TGeoVolume with which this TLitVolume is
  //associated and nothing else. If the TGeoVolume contains a TLitMedium
  //which is anisotropic, you have to call in addition SetOpticalAxis(),
  //or better, use instead the next constructor.
  Init();
  fGeoVolume = vol;
  SetName(vol->GetName());
  SetTitle(vol->GetTitle());
  fLitMedium = (TLitMedium*)vol->GetMedium();
  fAnisotropic = fLitMedium->GetAnisotropy();
  if (fAnisotropic>0) {
    gCleanOut->MM(warning,"TLitVolume","Medium is anisotropic",ClassName());
    gCleanOut->MM(warning,"TLitVolume","Please call SetOpticalAxis",ClassName());
  }
  fDielChange  = fLitMedium->GetIChangeWithWvl();
  fAniAbsL     = fLitMedium->GetAniAbsL();
  vol->SetField(this);
  AddMe();
}
TLitVolume::TLitVolume(TGeoVolume *vol,Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) {
  //  Constructor giving the TGeoVolume with which this TLitVolume is
  //associated AND the optical axis
  //  Sets the 3 optical axis fAniOx, fAniOy, fAniOz. The 3 optical axis are
  //the 3 axis of the coordinate system in which the dielectric tensor is
  //diagonal, with eigenvalues
  //
  //   ea   along the Ox axis
  //   eb   along the Oy axis
  //   ec   along the Oz axis
  //
  //   ea <  eb <  ec  for totally anisotropic crystals (==birefringent)
  //   ea <  eb =  ec  for "uniaxial negativ birefringent" crystals
  //   ea =  eb <  ec  for "uniaxial positiv birefringent" crystals
  //
  //  These 3 axis are given "a la GEANT" : give 
  //   theta and phi of fAniOx axis   (thetax,phix)
  //   theta and phi of fAniOy axis   (thetay,phiy)
  //  It is not necessary to give theta and phi of fAniOz : fAniOz can be
  //determined with fAniOx and fAniOy. This way of giving a coordinate system
  //inside the local coordinate system is intuitive, but inelegant, because it
  //is redundant. If you give an fAniOy axis which is not orthogonal to the
  //fAniOx axis, you will be warned and your fAniOy axis will be corrected.
  //
  //  GIVE ALL ANGLES IN DEGREE !
  //
  //  To understand the construction of the dielectric tensor both in Local
  //Coordinate System and in the World Coordinate system, read the general
  //description of the class TLitVolume.
  //
  Init();
  fGeoVolume = vol;
  SetName(vol->GetName());
  SetTitle(vol->GetTitle());
  fLitMedium = (TLitMedium*)vol->GetMedium();
  fAnisotropic = fLitMedium->GetAnisotropy();
  fDielChange  = fLitMedium->GetIChangeWithWvl();
  fAniAbsL     = fLitMedium->GetAniAbsL();
  SetOpticalAxis(thetax,phix,thetay,phiy);
  vol->SetField(this);
  AddMe();
}
TLitVolume::~TLitVolume() {
//destructor
  if (fHDamage) {
    delete fHDamage;
    fHDamage = 0;
  }
  if (fGeoVolume) fGeoVolume->SetField(0);
}
void TLitVolume::AddMe() {
  //  Add this LitVolume to the list fgLitVolume. If a volume of the same name
  //was present in the list, it is killed.
  Int_t i,k,N;
  TString s1,s2;
  TLitVolume *pv;
  TIter next(fgLitVolume);
  N = fgLitVolume->GetEntries();
  if (N<=0) fgLitVolume->AddLast(this);
  else {
    Bool_t found = kFALSE;
    s1 = GetName();
    i = 0;
    while ((pv = (TLitVolume*)next()) && (!found) && (i<N)) {
      s2 = pv->GetName();
      k = s1.CompareTo(s2);
      if (k==0) {
        fgLitVolume->Remove(pv);
        delete pv;
        found = kTRUE;
      }
      i++;
    }
    fgLitVolume->AddLast(this);
  }
}
void TLitVolume::ClearEmission() {
  //  Declares that the TGeoVolume associated with this TLitVolume is NOT able
  // to generate photons. The call to this method is only useful if you have
  // previously called SetEmission() and for strange reasons, you want to
  // cancel the effect of SetEmission().
  fEmitPhoton  = kFALSE;
  fSourceFixed = kFALSE;
  fSourcePoint = {0.0,0.0,0.0};
  fSourceDir   = {0.0,0.0,0.0};
  fThetaMax   = TMath::Pi();
  fFitDist     = 0;
  fFromFace    = kFALSE;
  fDirFace     = {0.0,0.0,0.0};
  fPolarized   = kFALSE;
  fPolar       = {0.0,0.0,0.0};
}
Bool_t TLitVolume::EmissionToWCS() {
  //  Given a fPath like "/T_1/A_2/B_2", pointing to a particular instance [node] of
  // the TGeoVolume associated to this TLitVolume, calculates:
  //
  //  (1) fSourcePoint => fWCSSourcePoint  position of emission point in WCS
  //  (2) fSourceDir   => fWCSSourceDir    direction of emission in WCS
  //  (3) fE           => fWCSE            generated direction of E field in WCS
  //  (4) fDirFace     => fWCSDirFace      direction to take to reach face, in WCS
  //
  //  This method also transfers the emission point fWCSSourcePoint towards the
  // face of the TGeoVolume, along direction fWCSDirFace in case fFromFace is
  // true.
  //  Since a given TGeoVolume like this one can be inserted many times,
  // at many different physical locations, with many different orientations, it is
  // necessary to give the specific path concerned and to recalculate each time
  // one consider an other instance of this TgeoVolume
  // fPath must have been set before calling this method
  //  To be called only AFTER closing of the geometry
  //
  const char *met = "EmissionToWCS";
  const char lim = '/';
  Short_t depth; //depth of fPath
  Short_t k;     //index inside fPath
  Short_t nn;    //next  '/' to be reached
  Short_t n;     //counter of '/' encountered
  Ssiz_t  N;     //Length of string fPath
  Short_t j;
  Bool_t found1 = kFALSE;
  Bool_t found;
  Double_t local1[3],master1[3];       // for fSourcePoint
  Double_t localdir[3],masterdir[3];   // for fSourceDir
  Double_t local2dir[3],master2dir[3]; // for fE
  Double_t local3dir[3],master3dir[3]; // for fDirFace
  TGeoVolume *vol;
  TGeoNode *node;
  TGeoNode *daughter = 0;
  TGeoNode *foundnode[10];
  TObjArray *nodes;
  TString snode,sdaughter;
  depth = TLit::Get()->CheckPath(fPath);
  switch (depth) {
    case 0:
      gCleanOut->MM(fatal,met,"Bad path of Tgeo",ClassName());
      break;
    case 1:
      fWCSSourcePoint = fSourcePoint;
      fWCSSourceDir   = fSourceDir;
      if (!fAnisotropic) fWCSE = fE;
      if (fFromFace) fWCSDirFace = fDirFace;
      found1    = kTRUE;
      break;
    default:
      N  = fPath.Length();
      if (fPath(N-1)==lim) fPath.Resize(--N);
      for (k=0;k<3;k++) {
        local1[k]   = fSourcePoint[k];
        localdir[k] = fSourceDir[k];
        if (!fAnisotropic) local2dir[k] = fE[k];
        if (fFromFace)     local3dir[k] = fDirFace[k];
      }
      k  = 0;
      n  = 0;
      nn = 2;
      while ((k<N) && (n<nn)) {
        if (fPath(k) == lim) n++;
        k++;
      }
      nn++; 
      vol = gGeoManager->GetTopVolume();
      found = kTRUE;
      while ((found) && (k<N)) {
        snode = "";
        while ((k<N) && (n<nn)) {
          if (fPath(k) == lim) n++;
          else snode.Append(fPath(k)); 
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
          if (!fAnisotropic) node->LocalToMasterVect(local2dir,master2dir);
          if (fFromFace)     node->LocalToMasterVect(local3dir,master3dir);
          for (j=0;j<3;j++) {
            local1[j] = master1[j];
            localdir[j] = masterdir[j];
            if (!fAnisotropic) local2dir[j] = master2dir[j];
            if (fFromFace)     local3dir[j] = master3dir[j];
          }
        }
        for (j=0;j<3;j++) {
          fWCSSourcePoint[j] = master1[j];
          fWCSSourceDir[j]   = masterdir[j];
          if (!fAnisotropic) fWCSE[j] = master2dir[j];
          if (fFromFace)     fWCSDirFace[j] = master3dir[j];
        }
      }
      break;
  }//end switch (depth)
  fWCSSourceDir = fWCSSourceDir.Unit();
  if (!fAnisotropic) fWCSE = fWCSE.Unit();
  if (fFromFace) {
    Int_t kk;
    TGeoNode *Node;
    const Double_t *point;
    fWCSDirFace = fWCSDirFace.Unit();
    Node = gGeoManager->InitTrack(fWCSSourcePoint[0],fWCSSourcePoint[1],fWCSSourcePoint[2],
      fWCSDirFace[0],fWCSDirFace[1],fWCSDirFace[2]);
    const char *path = gGeoManager->GetPath();
    kk = fPath.CompareTo(path);
    if (kk) {
      gCleanOut->MM(error,met,"User path different from found path",ClassName());
      gCleanOut->MMT(error,met,fPath.Data(),"given by","user",ClassName());
      gCleanOut->MMT(fatal,met,path,"found by","TGeo",ClassName());
    }
    Node = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    gGeoManager->SetCurrentDirection(-fWCSDirFace[0],-fWCSDirFace[1],-fWCSDirFace[2]);
    Node = gGeoManager->FindNextBoundaryAndStep(TLitPhys::Get()->BiggestStep());
    point = gGeoManager->GetCurrentPoint();
    fWCSSourcePoint.SetXYZ(point[0],point[1],point[2]);
  }
  return found1;
}
Bool_t TLitVolume::Gen(const char *path) {
  //  Generates starting point and starting direction and then converts these
  // into WCS. The conversion is valid for the instance of this TLitVolume
  // having the path specified.
  //  In the case where the TGeoVolume fGeoVolume associated with this
  // TLitVolume contains daughters TGeoVolume, the generated source point
  // for the photons must NOT come from the daughters, but from the part
  // of fGeoVolume not occupied by daughters! That is the reason for the
  // while statement, which repeats operations until it is the case.
  //  To be called only AFTER closing of the geometry
  const char *met  = "Gen";
  const Int_t Nmax = 100000;
  Bool_t ok;
  Int_t k;
  TGeoNode *node = 0;
  TGeoVolume *volume = 0;
  fPath = path;
  k = 0;
  ok = kFALSE;
  while ((!ok) && (k<Nmax)) {
    k++;
    ok = GenSourcePoint();
    if (ok) {
      ok = GenSourceDir();
      if (ok) {
        GenEField();
        ok = EmissionToWCS();
        // Checking that the source point is in fGeoVolume, not in a daughter
        if (ok) {
          //direction does not matter
          node = gGeoManager->InitTrack(fWCSSourcePoint[0],fWCSSourcePoint[1],fWCSSourcePoint[2],0.0,0.0,1.0);
          if (!node) {
            ok = kFALSE;
            gCleanOut->MM(fatal,met,"Source point without node",ClassName());
          }//end if (!node)
          else {
            volume  = node->GetVolume();
            if (volume != fGeoVolume) {
              ok = kFALSE;
            }
          }//end else if (!node)
        }//end if (ok) EmissionToWCS()
        else gCleanOut->MM(error,met,"EmissionToWCS() failed",ClassName());
      }//end if (ok) GenSourceDir()
      else gCleanOut->MM(error,met,"GenSourceDir() failed",ClassName());
    }//end if (ok) GenSourcePoint()
    else gCleanOut->MM(error,met,"GenSourcePoint() failed",ClassName());
  }//end while ((!ok) && (k<Nmax))
  if (k>=Nmax) gCleanOut->MM(fatal,met,"No source point found in fGeoVolume",ClassName());
  return ok;
}
void TLitVolume::GenEField() {
  //  Generates the unit fE vector, direction of the E field.
  // Refuses to do it inside anisotropic material. GenSourceDir() must have been
  // called before.
  if (!fAnisotropic) {
    Bool_t parallel;
    Double_t change;
    if (fPolarized) {
      TMat33 M(fPolar,fSourceDir,parallel,change);
      fE = M.GetLine(0);
    }//end if (fPolarized)
    else {
      Double_t s,c,phi;
      TVector3 Ox(1.0,0.0,0.0);
      TVector3 Oy(0.0,1.0,0.0);
      s = TMath::Abs(fSourceDir*Ox);
      c = TMath::Abs(fSourceDir*Oy);
      if (s<c) {
        TMat33 M(Ox,fSourceDir,parallel,change);
        Ox = M.GetLine(0);
        Oy = fSourceDir.Cross(Ox);
        Oy = Oy.Unit();
      }
      else {
        TMat33 M(Oy,fSourceDir,parallel,change);
        Oy = M.GetLine(0);
        Ox = Oy.Cross(fSourceDir);
        Ox = Ox.Unit();
      }
      phi = 2.0*TMath::Pi()*gRandom3->Rndm();
      s   = TMath::Sin(phi);
      c   = TMath::Cos(phi);
      fE  = c*Ox + s*Oy;
    }//end else if (fPolarized)
    if (TLit::Get()->GetTestCalc()) {
      // Verifies that E is orthogonal to k and that E is of length 1
      const char *met = "GenEField";
      const Double_t eps = 1.0e-10;
      change = TMath::Abs(fE*fSourceDir);
      if (change>eps)
        gCleanOut->MMR(fatal,met,"E not orthogonal to k","product",change,ClassName());
      change = fE*fE;
      if (TMath::Abs(change-1.0)>eps)
        gCleanOut->MMR(fatal,met,"E not of length 1","Length",change,ClassName());
    }
  }//end if (!fAnisotropic)
  else fE.SetXYZ(0.0,0.0,0.0);
}
Bool_t TLitVolume::GenSourceDir() {
  // Generates a direction of emission for the photon, according to the kind of
  //distribution chosen in fKindOfEmission
  const Double_t un     = 1.0;
  const Double_t eps    = 1.0e-14;
  const Double_t zero   = 0.0;
  const Int_t ntrymax   = 1000000;
  const char *met = "GenSourceDir";
  Bool_t ok = kTRUE;
  Short_t k;
  TVector3 Oxp, Oyp, Ozp;
  Double_t theta,theta1,theta2,stheta,ctheta,phi,sphi,cphi;
  Double_t b,xx,sthetamax,cthetamax;
// Going into system around fAxisDir
  Ozp    = fAxisDir;
  ctheta = Ozp[2];
  stheta = TMath::Sqrt(1.0 - ctheta*ctheta);
  if (stheta>eps) {
    sphi = Ozp[1]/stheta;
    cphi = Ozp[0]/stheta;
    b    = TMath::Sqrt(sphi*sphi + cphi*cphi);
    sphi /= b;
    cphi /= b;
  }
  else {
    sphi = 0.0;
    cphi = 1.0;
  }
  Oxp[0] =  ctheta*cphi;
  Oxp[1] =  ctheta*sphi;
  Oxp[2] = -stheta;
  Oyp[0] = -sphi;
  Oyp[1] =  cphi;
  Oyp[2] =  0.0;
// Generatng angles around fAxisDir
  sthetamax = TMath::Sin(fThetaMax);
  cthetamax = TMath::Cos(fThetaMax);
  xx     = gRandom3->Rndm();
  phi    = 2*TMath::Pi()*gRandom3->Rndm();
  sphi   = TMath::Sin(phi);
  cphi   = TMath::Cos(phi);
  switch (fKindOfEmission) {
    case on4pi:
    case flat:
      ctheta = un - (un - cthetamax)*xx;
      stheta = TMath::Sqrt(un-ctheta*ctheta);
      break;
    case sinuscosinus:
      stheta = sthetamax*TMath::Sqrt(xx);
      theta1 = TMath::ASin(stheta);
      theta2 = TMath::Pi() - theta1;
      if (theta2>fThetaMax) theta = theta1;
      else {
        xx = gRandom3->Rndm();
        if (xx>=0.5) theta = theta2;
        else         theta = theta1;
      }
      ctheta = TMath::Cos(theta);
      break;
    case provided:
      if (fThetaMax<eps) theta = zero;
       else {
        Bool_t ok2 = kFALSE;
        Int_t ntry = 0;
        while ((!ok2) && (ntry<ntrymax)) {
          ntry++;
          theta = fFitDist->GetRandom();
          if (theta<=fThetaMax) ok2 = kTRUE;
        }
        if (!ok2) {
          ok = kFALSE;
          gCleanOut->MM(killthis,met,"Problem generating theta with provided distribution",ClassName());
        }
        stheta = TMath::Sin(theta);
        ctheta = TMath::Cos(theta);
      }
      break;
  }
  for (k=0;k<3;k++)
    fSourceDir[k] = stheta*cphi*Oxp[k] + stheta*sphi*Oyp[k] + ctheta*Ozp[k];
  return ok;
}
Bool_t TLitVolume::GenSourcePoint() {
  //  Generates randomly and with equal probability for all positions inside
  // fGeoVolume a source of emission, except if fSourceFixed is true, in
  // which case takes simply the value already in fSourcePoint.
  Bool_t ok = kTRUE;
  if (!fSourceFixed) {
    const Int_t Nmax = 100000;
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
    shape = fGeoVolume->GetShape();
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
Double_t TLitVolume::GetDamage(const TVector3 *x,Double_t L0) {
//  Get the value with which the absorption length of the material of
//the TGeoVolume has to be multiplied in order to simulate radiation damages.
//  TVector3 x is the coordinate of the point for which we want the damage
//factor. See explanation of method RadiationDamage() or of the method
//ShowerDamage().
//      z is the distance between the projection of the point x on the  damage
//        axis fDamAxis and the starting point fDamStart.
//      r is the distance between point x and the axis of the shower. It is
//        the radial distance to be taken in the damage or shower
//        distribution.
//  L0 is the absorption length without damage
//
  const Double_t un   = 1.0;
  const Double_t deux = 1.0;
  const Double_t b    = 0.5;
  const Double_t eps  = 1.0e-8;
  Double_t L;  //Absorption length after damage
  Double_t K0; // 1/L0
  Double_t F = un;
  if (fDamage) {
    Double_t z;
    TVector3 d;
    Double_t r,fzr;
    TVector3 R;
    L0 = TMath::Abs(L0);
    if (L0<eps) L0 = eps;
    K0 = un/L0;
    d = *x - fWCSDamStart;
    z = fWCSDamAxis*d;
    R = d.Cross(fWCSDamAxis);
    r = R.Mag();
    if (fDamByShower) {
      Double_t t,am1,den;
      t    = z/fLitMedium->GetX0();
      am1  = fDamA - un;
      den  = r*r + fRk2;
      den *= den;
      fzr  = fDamFac*TMath::Exp(am1*TMath::Log(b*t))*TMath::Exp(-b*t);
      fzr /= den;
    }
    else {
      Double_t a1,a2;
      a2  = r/fDamRs;
      a2 *= a2;
      a2 /= deux;
      if (fDamFitGz) {
        fDamGz = fDamFitGz->V(z);
        fzr    = fDamGz*TMath::Exp(-a2);
      }
      else {
        a1  = (z-fDamZm)/fDamZs;
        a1 *= a1;
        a1 /= deux;
        fzr = TMath::Exp(-a1)*TMath::Exp(-a2);
      }
    }
    L = fDamFmax/(fDamFmax*K0 + fzr*fDamKimax*fDamSupFac);
    F = L/L0;
    if (fHDamage) {
      Int_t kbin;
      kbin = fHDamage->FindBin(z,r);
      fHDamage->SetBinContent(kbin,un-F);
    }
  }//end if (fDamage)
  return F;
}
void TLitVolume::GetEmission(TVector3 &x,TVector3 &d,TVector3 &E) const {
  // Get :
  //  x : point of emission in WCS
  //  d : direction of emission in WCS
  //  E : direction of E field in WCS
  x = fWCSSourcePoint;
  d = fWCSSourceDir;
  E = fWCSE;
}
void TLitVolume::Init() {
  // Initialization
  InitP();
  if (!fgLitVolume) fgLitVolume = new TObjArray();
  fWCSdone        = kFALSE;
  fStepping       = kFALSE;
  fStep           = -1.0;
  fAnisotropic    = 0;
  fDetKind        = none;
  SetNoDamage();
  fEmitPhoton     = kFALSE;
  fKindOfEmission = on4pi;
  fThetaMax       = 2*TMath::Pi();
}
void TLitVolume::InitP() {
//All pointers to 0
  fGeoVolume   = 0;
  fLitMedium   = 0;
  fFitQE       = 0;
  fGP          = 0;
  fDamFitKimax = 0;
  fHDamage     = 0;
  fDamFitGz    = 0;
  fFitDist     = 0;
}
void TLitVolume::NewWavelength(Double_t wavelength) {
  //Recalculates all parameters depending upon wavelength. wavelength is in
  //nanometers
  if (fFitQE) fQE = fFitQE->V(wavelength);
  if (fDamFitKimax) fDamKimax = TMath::Abs(fDamFitKimax->V(wavelength));
}
void TLitVolume::NewWavelengthAll(Double_t wavelength,Bool_t ForceIt) {
  // Wavelength has changed. We recalculate ALL values depending upon wavelength
  //for ALL TLitVolume of the setup
  const Double_t eps = 1.0e-10;
  if ((TMath::Abs(TLitMedium::fgOldWavelength-wavelength) > eps) || ForceIt) {
    TLitMedium::fgOldWavelength = wavelength;
    TLitVolume *plitvol;
    TIter next1(TLitVolume::fgLitVolume);
    while ((plitvol = (TLitVolume *)next1())) {
      plitvol->NewWavelength(wavelength);
    }
  }
}
Bool_t TLitVolume::RadiationDamage(const TVector3 *damaxis, const TVector3 *damstart,
  Double_t Kimax, Double_t zmean, Double_t zsig, Double_t rsig, Double_t step,
  Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
//  RadiationDamage gives the possibility of imposing for this TGeoVolume a
//radiation damage, i.e. a (z,r) dependent factor affecting the radia-
//tion length inside this TGeoVolume.
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  The damages are assumed here as having a gaussian TGeoVolume in z and in r.
//The gaussian giving the damages in z has a mean zmean and a sigma zsig.
//The gaussian giving the damages in r is centered on 0 and has a sigma
//rsig. If the damages have been produced by electromagnetic showers
//look rather at the method ShowerDamage().
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume.
//
//  If the distribution of the damages is f(z,r), we have:
//
//    f(z,r) = exp(-(z-zmean)^2/(2*zsig^2))exp(-r^2/(2*rsig^2))
//
//  Parameters :
//
// damaxis   : axis along which damage varies
// damstart  : starting point for calculating the damages
// Kimax     : 1/Limin, where Limin is the contribution to the absorption
//             length of the damages, at the point where it is maximal. In
//             this instance of ShowerDamage(), Kimax is not taken as depending
//             upon wavelength.
// zmean     : depth along damaxis where the damages are at maximum
// zsig      : gives the width along z of the damages.
// rsig      : gives the width along r of the damages.
// step      : steps for calculating absorption [cm]. Default 0.4. Refuses
//             values smaller than 0.001.
// damsupfac : possible supplementary constant factor multiplying Ki(lambda), i.e.
//             increasing the damages if bigger than 1.
//
  const Double_t un    = 1.0;
  const Double_t small = 0.001;
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  fDamage      = kTRUE;
  fDamByShower = kFALSE;
  fDamKimax    = Kimax;
  fDamAxis     = *damaxis;
  fDamAxis     = fDamAxis.Unit();
  fDamStart    = *damstart;
  fDamZm       = zmean;
  fDamZs       = TMath::Abs(zsig);
  fDamRs       = TMath::Abs(rsig);
  fDamStep     = step;
  fStepping    = kTRUE;
  if (fStep>fDamStep) fStep = fDamStep;
  fDamFmax     = un;
  return fDamage;
}
Bool_t TLitVolume::RadiationDamage(const TVector3 *damaxis, const TVector3 *damstart,
  const char *namefitkimax, Double_t zmean, Double_t zsig, Double_t rsig, Double_t step,
  Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
//  RadiationDamage gives the possibility of imposing for this TGeoVolume a
//radiation damage, i.e. a (z,r) dependent factor affecting the radia-
//tion length inside this TGeoVolume.
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  The damages are assumed here as having a gaussian TGeoVolume in z and in r.
//The gaussian giving the damages in z has a mean zmean and a sigma zsig.
//The gaussian giving the damages in r is centered on 0 and has a sigma
//rsig. If the damages have been produced by electromagnetic showers
//look rather at the method ShowerDamage().
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume. It means that you have to call RadiationDamage() BEFORE the
//TGeoVolume goes into world coordinates, i.e. before the constructor of TGeoNode
//for this TGeoVolume is called, which makes all coordinates to go into world
//coordinates.
//  If the distribution of the damages is f(z,r), we have:
//
//    f(z,r) = exp(-(z-zmean)^2/(2*zsig^2))exp(-r^2/(2*rsig^2))
//
//  Parameters :
//
// damaxis      : axis along which damage varies
// damstart     : starting point for calculating the damages
// namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
//                 contribution to the absorption length of the damages, at the
//                 point where it is maximal. In this instance of ShowerDamage(),
//                 Kimax is depending upon wavelength.
// zmean        : depth along damaxis where the damages are at maximum
// zsig         : gives the width along z of the damages.
// rsig         : gives the width along r of the damages.
// step         : steps for calculating absorption [cm]. Default 0.4. Refuses
//                 values smaller than 0.001.
// damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
//                increasing the damages if bigger than 1.
//
  const Double_t un    = 1.0;
  const Double_t small = 0.001;
  SetNoDamage();
  fDamSupFac = damsupfac;
  if (fDamFitKimax) {
    delete fDamFitKimax;
    fDamFitKimax = 0;
  }
  fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
  if (fDamFitKimax) {
    fDamage      = kTRUE;
    fDamByShower = kFALSE;
    step  = TMath::Abs(step);
    if (step<small) step = small;
    fDamAxis     = *damaxis;
    fDamAxis     = fDamAxis.Unit();
    fDamStart    = *damstart;
    fDamZm       = zmean;
    fDamZs       = TMath::Abs(zsig);
    fDamRs       = TMath::Abs(rsig);
    fDamStep     = step;
    fStepping    = kTRUE;
    if (fStep>fDamStep) fStep = fDamStep;
    fDamFmax     = un;
  }
  else gCleanOut->MM(fatal,"RadiationDamage","Fit for Kimax not found",ClassName());
  return fDamage;
}
Bool_t TLitVolume::RadiationDamage(const TVector3 *damaxis, const TVector3 *damstart,
  Double_t Kimax, const char *namefitgz, Double_t rsig, Double_t step,
  Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
//  RadiationDamage gives the possibility of imposing for this TGeoVolume a
//radiation damage, i.e. a (z,r) dependent factor affecting the radia-
//tion length inside this TGeoVolume.
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  The damages are assumed here as having a gaussian TGeoVolume in z and in r.
//The gaussian giving the damages in z has a mean zmean and a sigma zsig.
//The gaussian giving the damages in r is centered on 0 and has a sigma
//rsig. If the damages have been produced by electromagnetic showers
//look rather at the method ShowerDamage().
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume. It means that you have to call RadiationDamage() BEFORE the
//TGeoVolume goes into world coordinates, i.e. before the constructor of TGeoNode
//for this TGeoVolume is called, which makes all coordinates to go into world
//coordinates.
//  If the distribution of the damages is f(z,r), we have:
//
//  f(z,r) = g(z)*exp(-r^2/(2*rsig^2))   where g(z) is a fit given by the user
//
//  Parameters :
//
// damaxis   : axis along which damage varies
// damstart  : starting point for calculating the damages
// Kimax     : 1/Limin, where Limin is the contribution to the absorption
//             length of the damages, at the point where it is maximal. In
//             this instance of ShowerDamage(), Kimax is not taken as depending
//             upon wavelength.
// namefitgz : name of the fit giving g(z), the z dependent part of f(z,r)
// rsig      : gives the width along r of the damages.
// step      : steps for calculating absorption [cm]. Default 0.4. Refuses
//             values smaller than 0.001.
// damsupfac : possible supplementary constant factor multiplying Ki(lambda), i.e.
//             increasing the damages if bigger than 1.
//
  const Double_t small = 0.001;
  SetNoDamage();
  Bool_t upbound = kFALSE;
  fDamSupFac = damsupfac;
  if (fDamFitGz) {
    delete fDamFitGz;
    fDamFitGz = 0;
  }
  fDamFitGz = TSplineFit::FindFit(namefitgz,15);
  if (fDamFitGz) {
    upbound = fDamFitGz->GetUpBound(fDamGzMax);
    if (!upbound)
      gCleanOut->MM(fatal,"RadiationDamage","Fit for gz MUST have an upper bound",ClassName());
    else {
      step  = TMath::Abs(step);
      if (step<small) step = small;
      fDamage      = kTRUE;
      fDamByShower = kFALSE;
      fDamKimax    = Kimax;
      fDamAxis     = *damaxis;
      fDamAxis     = fDamAxis.Unit();
      fDamStart    = *damstart;
      fDamRs       = TMath::Abs(rsig);
      fDamStep     = step;
      fStepping    = kTRUE;
      if (fStep>fDamStep) fStep = fDamStep;
      fDamFmax     = fDamGzMax;
    }//end else if (!upbound)
  }//end if (fDamFitGz)
  else gCleanOut->MM(fatal,"RadiationDamage","Fit for gz not found",ClassName());
  return fDamage;
}
Bool_t TLitVolume::RadiationDamage(const TVector3 *damaxis, const TVector3 *damstart,
  const char *namefitkimax, const char *namefitgz, Double_t rsig, Double_t step,
  Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
//  RadiationDamage gives the possibility of imposing for this TGeoVolume a
//radiation damage, i.e. a (z,r) dependent factor affecting the radia-
//tion length inside this TGeoVolume.
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  The damages are assumed here as having a gaussian TGeoVolume in z and in r.
//The gaussian giving the damages in z has a mean zmean and a sigma zsig.
//The gaussian giving the damages in r is centered on 0 and has a sigma
//rsig. If the damages have been produced by electromagnetic showers
//look rather at the method ShowerDamage().
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume. It means that you have to call RadiationDamage() BEFORE the
//TGeoVolume goes into world coordinates, i.e. before the constructor of TGeoNode
//for this TGeoVolume is called, which makes all coordinates to go into world
//coordinates.
//  If the distribution of the damages is f(z,r), we have:
//
//    f(z,r) = g(z)*exp(-r^2/(2*rsig^2))
//
//  Parameters :
//
// damaxis      : axis along which damage varies
// damstart     : starting point for calculating the damages
// namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
//                 contribution to the absorption length of the damages, at the
//                 point where it is maximal. In this instance of ShowerDamage(),
//                 Kimax is depending upon wavelength.
// namefitgz    : name of the fit giving g(z), the z dependent part of f(z,r)
// rsig         : gives the width along r of the damages.
// step         : steps for calculating absorption [cm]. Default 0.4. Refuses
//                 values smaller than 0.001.
// damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
//                increasing the damages if bigger than 1.
//
  const Double_t small = 0.001;
  SetNoDamage();
  fDamSupFac = damsupfac;
  Bool_t upbound = kFALSE;
  if (fDamFitGz) {
    delete fDamFitGz;
    fDamFitGz = 0;
  }
  fDamFitGz = TSplineFit::FindFit(namefitgz,15);
  if (fDamFitGz) {
    upbound = fDamFitGz->GetUpBound(fDamGzMax);
    if (!upbound) gCleanOut->MM(fatal,"RadiationDamage","Fit for gz MUST have an upper bound",ClassName());
    else {
      if (fDamFitKimax) {
        delete fDamFitKimax;
        fDamFitKimax = 0;
      }
      fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
      if (fDamFitKimax) {
        fDamage      = kTRUE;
        fDamByShower = kFALSE;
        step  = TMath::Abs(step);
        if (step<small) step = small;
        fDamAxis     = *damaxis;
        fDamAxis     = fDamAxis.Unit();
        fDamStart    = *damstart;
        fDamRs       = TMath::Abs(rsig);
        fDamStep     = step;
        fStepping    = kTRUE;
        if (fStep>fDamStep) fStep = fDamStep;
        fDamFmax     = fDamGzMax;
      }//end if (fDamFitKimax)
      else gCleanOut->MM(fatal,"RadiationDamage","Fit for Kimax not found",ClassName());
    }//end else if (!upbound)
  }//end if (fDamFitGz)
  else gCleanOut->MM(fatal,"RadiationDamage","Fit for g(z) not found",ClassName());
  return fDamage;
}
void TLitVolume::RecordDamages(Int_t nz,Double_t zmax,Int_t nr,Double_t rmax) {
// If you call this method before any generation, each damage factor generated
//will be recorded into a 2D histogram. The histogram will not be filled by
//call to Fill() [it is not the number of hits which interests us], the value
//of 1 - damage factor will be put as the content of the bin, such that the
//histogram will show the calculated distribution for 1-damage factor, not
//the number of hits. After all generations, you can call ShowDamages() to see
//this histogram.
// Call this method only as a check. Do not use it in production. It is
//costly in time.
//
// Arguments:
//
//  nz    : number of channels in z (coord. along damaxis from damstart)
//  zmax  : maximum z recorded. zmin is always 0
//  nr    : number of channels in r (distance to damaxis)
//  rmax  : maximum r recorded. rmin is always 0
//
//
  const Double_t zero = 0.0;
  fHDamage = new TH2D("Damages","1 minus damage factor as a function of (z,r)",
    nz,zero,zmax,nr,zero,rmax);
}
void TLitVolume::SetAPD( const char *nameGP,Short_t dir,Double_t threshold,Double_t vdrifte) {
//
// An APD, an Avalanche PhotoDiode, is a special case of a volume detector.
// - The most natural TGeoVolume for an APD is a TGeoBBox or a TGeoTube, although it
//    is not imposed by SLitrani. But be aware of the fact that if you choose a
//    TGeoVolume which is neither a TGeoBBox nor a TGeoTube, it is the surrounding
//    TGeoBBox of your TGeoVolume which will be used to calculate the depth inside
//    the TGeoVolume where the photon has died.
// - There is no condition of the type "cathode efficiency". What matters is
//    whether or not the photon stops in the APD because of its finite absorption
//    length.
// - There is no condition on acceptance angle, BUT:
// - fDetAxis MUST be defined and indicate the direction from the cathode to the
//    anode. The two faces of the TGeoVolume which are orthogonal to fDetAxis are
//    the cathode and the anode. So, there is clearly an entrance face for an
//    APD the cathode, and fEntrance is this face number.
//       (1) - For a TGeoTube, fDetAxis can only be parallel or anti-parallel
//             with the Oz axis
//       (2) - Otherwise fDetAxis can have only 6 discrete directions:
//             parallel or anti-parallel with Ox or Oy or Oz of the TGeoBBox
//             surrounding your TGeoVolume
//    The argument "dir" choose among these 6 possibilities. See the description
//    of "dir" below.
// - Important notice concerning distance from cathode to anode: the distance
//    from cathode to anode for an APD is of the order of 0.01 cm. It does not make
//    sense to give for instance to this distance a value of for instance 1 cm. But for
//    reasons of appearance of the APD on the drawing of the geometry, the user
//    can be tempted to give to the APD a depth much bigger than the true distance
//    between cathode and anode. If SLitrani finds an unrealistic depth for the APD
//    when looking at the geometry of the TGeoVolume being the APD, then SLitrani replaces
//    this unrealistiv depth by the value of the static variable
//      TLitDetector::fgMaxCatAn
//    By default, TLitDetector::fgMaxCatAn is equal to 0.01 cm. The user of SLitrani
//    can freely change this value from his CINT macro, since TLitDetector::fgMaxCatAn
//    is a public variable.
// - The photon stops inside the APD because of its absorption length in Silicium:
//    fStopAlways = false.
// - A distribution function (of class TSplineFit, category 13) gives the number
//    of electrons generated by the stopping of the photon inside the volume at
//    a coordinate z with respect to the axis fDetAxis going from cathode to anode.
//    This distribution function is what is called the gain profile of the APD.
//    To be meaningful, a gain profile must have a qualitative shape defining
//    6 regions :
//     (1) - a first region named "dead zone", where the electric field is 0
//       and consequently the gain is 0. Width of this region : ~200 nm.
//     (2) - a second region where the gain is rapidly rising to its maximal
//       value ( of the order of 60). Width of this region : ~250 nm.
//     (3) - a third region where the gain approximatly stays at its maximal
//       value ( of the order of 60). Width of this region : ~2500 nm.
//     (4) - a fourth region where the gain drops towards a value close to 2.
//       Width of this region : ~5000 nm.	
//     (5) - a fifth region where the gain almost stays at 2.
//       Width of this region : ~90000 nm.
//     (6) - a sixth region where it drops to zero.
//     	Total Width of APD : ~100000nm.
//    Take these considerations into account when you define the interpolation (never
//    use a fit for this kind of curve, it will always be bad. Always use a linear
//    interpolation) of type TSplineFit giving the gain profile. Do not forget
//    that for this gain profile, you have to give the coordinate z in nanometers
//    and not in cm (contrary to the shapes which are defined in cm).
//    The photon is considered as detected if it has stopped at a z coordinate
//    such that at least 5 electrons have been emitted. This value of 5 is
//    arbitrary and may be changed by the user. In fact, what really matters here
//    is not whether the photon has been detected or not (has emitted more or less
//    than 5 electrons), but how many electrons have been emitted and at what time
//    they have reached the anode. This value is recorded into an histogram which
//    gives directly the shape of the pulse emitted by the APD after the emission
//    of all photons of the run.
//
//  The arguments are the following :
//
//  nameGP    : the name of the fit or interpolation, of type TSplineFit giving
//              the gain profile of the APD.
//  dir       : used for defining the normalized axis giving the direction
//              from cathode to anode. The cathode is the side from which the
//              photons enter, driving then towards the anode. This axis is in
//              the local coordinate system of the associated TGeoShape. The
//              meaning of dir is the following:
//               dir = +1 ==> axis parallel to Ox        (case of TGeoBBox)
//               dir = -1 ==> axis anti-parallel to Ox   (case of TGeoBBox)
//               dir = +2 ==> axis parallel to Oy        (case of TGeoBBox)
//               dir = -2 ==> axis anti-parallel to Oy   (case of TGeoBBox)
//               dir = +3 ==> axis parallel to Oz        (TGeoBBox or TGeoTube)
//               dir = -3 ==> axis anti-parallel to Oz   (TGeoBBox or TGeoTube)
//  threshold : the threshold number of emitted electrons needed to consider
//              the photon as seen by the APD. If you do not give this argument,
//              the default value of 5.0 contained in TLitPhys will be taken.
//              The photon is considered as detected if it has stopped at a z
//              coordinate such that at least nthreshe (default 5) electrons
//              have been emitted. In fact, what really matters here is not
//              whether the photon has been detected or not (has emitted more
//              or less than nthreshe electrons), but how many electrons have
//              been emitted and at what time they have reached the anode.
//              This value is recorded into an histogram which gives directly
//              the TGeoVolume of the pulse emitted by the APD after the emission
//              of all photons of the run. 
//  vdrifte   : the speed of electrons inside the APD (~100000 cm/s). This pa-
//              rameter is not very important because this drift velocity is
//              extremely fast and so does not affect very much the shape of
//              the pulse given by the APD. You can omit it, in that case, the
//              velocity of 10^8 cm/s will be taken.
//
  const Double_t zero   = 0.0;
  const char *met = "SetAPD";
  Short_t adir;
  Double_t dx,dy,dz;
  TGeoShape  *geoshape;
  TGeoTube   *geotube;
  TGeoBBox   *geobbox;
  fDetKind    = apd;
  fIsQE       = kFALSE;
  fFitQE      = 0;
  fNameQE     = "";
  fStopAlways = kFALSE;
  fIsAccept   = kFALSE;
  fCosAccept  = 1.0;
  fNameGP = nameGP;
  fGP = TSplineFit::FindFit(nameGP,13);
  if (!fGP)
    gCleanOut->MMT(fatal,met,"Gain profile of APD not found","gain",nameGP,ClassName());
  adir = TMath::Abs(dir);
  geoshape = fGeoVolume->GetShape();
  if (geoshape->InheritsFrom("TGeoTube")) {
    geotube = (TGeoTube*)geoshape;
    if (adir != 3)
      gCleanOut->MM(fatal,met,"dir should be +-3 for TGeoTube",ClassName());
    else {
      fEz = geotube->GetDz();
      fEx = geotube->GetRmax();
      fEy = fEx;
      fDistCatAn = TMath::Min(2.0*fEz,TLitDetector::fgMaxCatAn);
      if (dir==3) {
        fDetAxis.SetXYZ(0.0,0.0,1.0);
        fXYZEntrance.SetXYZ(0.0,0.0,-fEz);
        fEntrance = 4;
      }
      else {
        fDetAxis.SetXYZ(0.0,0.0,-1.0);
        fXYZEntrance.SetXYZ(0.0,0.0,fEz);
        fEntrance = 5;
      }
    }
    fDistCatAn = TMath::Min(2.0*geotube->GetDz(),TLitDetector::fgMaxCatAn);
  }
  else {
    if (geoshape->InheritsFrom("TGeoBBox")) {
      geobbox = (TGeoBBox*)geoshape;
      dx = geobbox->GetDX();
      dy = geobbox->GetDY();
      dz = geobbox->GetDZ();
      if ((adir < 1) || (adir > 3)) {
        gCleanOut->MM(fatal,met,"dir should be +-1/2/3 for TGeoBBox",ClassName());
      }
      else {
        switch (dir) {
          case -3:
            fDetAxis.SetXYZ(0.0,0.0,-1.0);
            fDistCatAn = TMath::Min(2.0*dz,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(0.0,0.0,dz);
            fEx       = dx;
            fEy       = dy;
            fEz       = dz;
            fEntrance = 5;
            break;
          case -2:
            fDetAxis.SetXYZ(0.0,-1.0,0.0);
            fDistCatAn = TMath::Min(2.0*dy,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(0.0,dy,0.0);
            fEx       = dx;
            fEy       = dz;
            fEz       = dy;
            fEntrance = 2;
            break;
          case -1:
            fDetAxis.SetXYZ(-1.0,0.0,0.0);
            fDistCatAn = TMath::Min(2.0*dx,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(dx,0.0,0.0);
            fEx       = dy;
            fEy       = dz;
            fEz       = dx;
            fEntrance = 1;
            break;
          case 1:
            fDetAxis.SetXYZ(1.0,0.0,0.0);
            fDistCatAn = TMath::Min(2.0*dx,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(-dx,0.0,0.0);
            fEx       = dy;
            fEy       = dz;
            fEz       = dx;
            fEntrance = 3;
            break;
          case 2:
            fDetAxis.SetXYZ(0.0,1.0,0.0);
            fDistCatAn = TMath::Min(2.0*dy,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(0.0,-dy,0.0);
            fEx       = dx;
            fEy       = dz;
            fEz       = dy;
            fEntrance = 0;
            break;
          case 3:
            fDetAxis.SetXYZ(0.0,0.0,1.0);
            fDistCatAn = TMath::Min(2.0*dz,TLitDetector::fgMaxCatAn);
            fXYZEntrance.SetXYZ(0.0,0.0,-dz);
            fEx       = dx;
            fEy       = dy;
            fEz       = dz;
            fEntrance = 4;
            break;
        }//end switch (dir)
      }//end else if ((adir == 0) || (adir < 1) || (adir > 3))
    }//end if (geoshape->InheritsFrom("TGeoBBox")
    else gCleanOut->MM(fatal,met,"TGeoShape not inheriting from TGeoBBox",ClassName());
  }//end else if (geoshape->InheritsFrom("TGeoTube")
  if (threshold>zero) fAPDThreshold = threshold;
  else                fAPDThreshold = TLitPhys::Get()->Threshe();
  fVDrifte = vdrifte;
}
void TLitVolume::SetDetector(Bool_t stopalways, const char *nameqe,
  Double_t accept, Double_t nx, Double_t ny, Double_t nz) {
//  Declares that this TLitVolume is a general kind of volume detector. In the
// most general case, a volume detector has no preferred entrance face. The
// photon can enter in the TGeoVolume through any face, the only thing that matters
// is whether
//  [stopalways == false] it will be stopped because of the non-zero absorption
//                             length of the TLitMedium filling the TGeoVolume
//  [stopalways == true]  it will stop immediatly on the face it has reached
//
//  BUT!
//
//  If you define an acceptance angle and its associated reference axis
// fDetAxis =(nx,ny,nz), you define by that an entrance face, the one which
// is pointed by - [minus] fDetAxis.
//  The fact that an entrance face is defined or not only matters for histogramming
// If an entrance face is defined, then a 2 dim histo can be defined with (x',y')
// being the projection onto this entrance face of the end point of the photon
// with respect to the fDetAxis axis of projection.
//
// Arguments
//
// stopalways : if false ==> a photon only stops inside the TGeoVolume because
//               of its finite absorption length.
//              if true  ==> a photon reaching this TGeoVolume immediatly stops
//               on the face of the TGeoVolume it has reached and is detected if
//               conditions on quantum efficiency and acceptance angle [if asked for]
//               are satisfied. 
// nameqe     : if nameqe != "" and nameqe != "none", then nameqe is the name
//               of an efficiency depending upon wavelength [like the cathode
//               efficiency of a PM], given in the form of a TSplineFit of
//               category 10.
// accept     : acceptance angle of the photon in degree. The photon will only
//               have a chance to be detected if it enters into this TLitVolume
//               with an angle smaller than accept, with respect to the
//               axis (nx,ny,nz). If you give to accept a value bigger than
//               90.0 or smaller than 0, there will simply not be any condition
//               on acceptance angle.
// (nx,ny,nz) : axis with respect to which the (entrance) angle of the photon
//               will be calculated. It is given in the local coordinate of
//               the TGeoVolume associated with this TLitVolume. Not relevant
//               if accept > 90.0 or <= 0.0.
//
  const Double_t zero = 0.0;
  const Double_t eps = 1.0e-10;
  const char *met = "SetDetector";
  fDetKind    = gendet;
  fIsQE       = kFALSE;
  fFitQE      = 0;
  TString s1  = "";
  TString s2  = "none";
  fNameQE     = "";
  fStopAlways = stopalways;
  fEntrance   = -1;
  fIsQE = !(!s1.CompareTo(nameqe) || !s2.CompareTo(nameqe));
	if (fIsQE) {
    fFitQE = TSplineFit::FindFit(nameqe,10);
    if (!fFitQE) {
      gCleanOut->MM(error,met,"Fit on quantum efficiency not found",ClassName());
      gCleanOut->MM(warning,met,"No quantum efficiency will be used",ClassName());
      fIsQE   = kFALSE;
      fNameQE = "";
    }
    else fNameQE = nameqe;
  }
  fDetAxis.SetXYZ(nx,ny,nz);
  fDetAxis = fDetAxis.Unit();
  nx = fDetAxis.X();
  ny = fDetAxis.Y();
  nz = fDetAxis.Z();
  fXYZEntrance.SetXYZ(zero,zero,zero);
  if ((accept>90.0) || (accept<=zero)) {
    fIsAccept  = kFALSE;
    fCosAccept = 1.0;
  }
  else {
    Bool_t found = kFALSE;
    fIsAccept    = kTRUE;
    fCosAccept   = TMath::Cos((accept*TMath::Pi())/180.0);
    Double_t t,x,y,z,dx,dy,dz;
    dx = ((TGeoBBox*)fGeoVolume->GetShape())->GetDX();
    dy = ((TGeoBBox*)fGeoVolume->GetShape())->GetDY();
    dz = ((TGeoBBox*)fGeoVolume->GetShape())->GetDZ();
    // Intersection with face 0 (at -dy)
    if (TMath::Abs(ny)>eps) {
      t = -dy/ny;
      x =  nx*t;
      z =  nz*t;
      found = ((t<zero) && (x>=-dx) && (x<=dx) && (z>=-dz) && (z<=dz));
      if (found) {
        fEntrance = 0;
        fXYZEntrance.SetXYZ(zero,-dy,zero);
        fEx       = dx;
        fEy       = dz;
        fEz       = dy;
      }
    }
    // Intersection with face 1 (at +dx)
    if (!found) {
      if (TMath::Abs(nx)>eps) {
        t = dx/nx;
        y = ny*t;
        z = nz*t;
        found = ((t<zero) && (y>=-dy) && (y<=dy) && (z>=-dz) && (z<=dz));
        if (found) {
          fEntrance = 1;
          fXYZEntrance.SetXYZ(dx,zero,zero);
          fEx       = dy;
          fEy       = dz;
          fEz       = dx;
        }
      }
    }
    // Intersection with face 2 (at +dy)
    if (!found) {
      if (TMath::Abs(ny)>eps) {
        t = dy/ny;
        x = nx*t;
        z = nz*t;
        found = ((t<zero) && (x>=-dx) && (x<=dx) && (z>=-dz) && (z<=dz));
        if (found) {
          fEntrance = 2;
          fXYZEntrance.SetXYZ(zero,dy,zero);
          fEx       = dx;
          fEy       = dz;
          fEz       = dy;
        }
      }
    }
    // Intersection with face 3 (at -dx)
    if (!found) {
      if (TMath::Abs(nx)>eps) {
        t = -dx/nx;
        y =  ny*t;
        z =  nz*t;
        found = ((t<zero) && (y>=-dy) && (y<=dy) && (z>=-dz) && (z<=dz));
        if (found) {
          fEntrance = 3;
          fXYZEntrance.SetXYZ(-dx,zero,zero);
          fEx       = dy;
          fEy       = dz;
          fEz       = dx;
        }
      }
    }
    // Intersection with face 4 (at -dz)
    if (!found) {
      if (TMath::Abs(nz)>eps) {
        t = -dz/nz;
        x =  nx*t;
        y =  ny*t;
        found = ((t<zero) && (x>=-dx) && (x<=dx) && (y>=-dy) && (y<=dy));
        if (found) {
          fEntrance = 4;
          fXYZEntrance.SetXYZ(zero,zero,-dz);
          fEx       = dx;
          fEy       = dy;
          fEz       = dz;
        }
      }
    }
    // Intersection with face 5 (at +dz)
    if (!found) {
      if (TMath::Abs(nz)>eps) {
        t = dz/nz;
        x = nx*t;
        y = ny*t;
        found = ((t<zero) && (x>=-dx) && (x<=dx) && (y>=-dy) && (y<=dy));
        if (found) {
          fEntrance = 4;
          fXYZEntrance.SetXYZ(zero,zero,dz);
          fEx       = dx;
          fEy       = dy;
          fEz       = dz;
        }
      }
    }
    if (!found) {
      fEntrance = -1;
      gCleanOut->MM(error,met,"No intersection with any face",ClassName());
    }
  }//end else if ((accept>=90.0) || (accept<=zero))
}
void TLitVolume::SetDielTensor() {
  //  Calculates the dielectric tensor when rotation matrix M is known. The
  // dielectric tensor is depending upon wavelength, which may change the
  // values ea,eb and ec.
  //  The dielectric tensor must be recalculated each time we enter a new
  // anisotropic TLitVolume, since each instance of the TGeoVolume associated
  // with the TLitVolume may have different space orientation, so different
  // fWCSAniOx/y/z building matrix M
  //  Notice that fDTmu is not the dielectric tensor : it is the dielectric
  // tensor multiplied by the magnetic permeability!
  //
  if (fAnisotropic) {
    TMat33 diag;
    Double_t muea,mueb,muec,mu;
    mu = fLitMedium->GetMu();
    muea = mu*fLitMedium->GetEpsa();
    mueb = mu*fLitMedium->GetEpsb();
    muec = mu*fLitMedium->GetEpsc();
    diag.Diagonal(muea,mueb,muec);
    fDTmu = fM*diag*fMt;
    if (fAniAbsL) {
      Double_t muphia,muphib,muphic;
      muphia = - fLitMedium->GetPhimua();
      muphib = - fLitMedium->GetPhimub();
      muphic = - fLitMedium->GetPhimuc();
      TComplex c00(muea,muphia);
      TComplex c11(mueb,muphib);
      TComplex c22(muec,muphic);
      TMat33C cdiag;
      cdiag.Diagonal(c00,c11,c22);
      fCDTmu = fM*cdiag*fMt;
    }
  }
}
Bool_t TLitVolume::SetEField(Bool_t polar,Double_t thetp, Double_t phip,ChoiceOfn selectn) {
  // Set the characteristics of the E field at the emission of the photon
  //  polar   : true if the beam is polarized. Notice that it is forbidden
  //            to choose a polarized beam inside an anisotropic material.
  //            if you want a polarized beam inside an anisotropic material,
  //            generates the polarized beam inside an isotropic material,
  //            and let it enter into the anisotropic material.
  //            [default kFALSE]
  //  thetp   : theta angle, in degree, in LCS of the TGeoVolume associated with
  //            this TLitVolume, of fPolar, the axis along which the electric field E
  //            of the photons has to be parallel if polar is true.
  //            [default 90.0]
  //  phip    : phi angle, in degree, in LCS of the TGeoVolume associated with
  //            this TLitVolume, of fPolar, the axis along which the electric field E
  //            of the photons has to be parallel if polar is true.
  //            [default 0.0] 
  //  selectn : this last argument only concerns the generation of photons
  //            inside anisotropic material. In anisotropic material, once
  //            the direction of the k vector is fixed, there are 2 possible
  //            solutions for the index, n1 or n2.
  //            - If selectn == random, a random choice will be done
  //               between n1 and n2 [ this is the default]
  //            - If selectn == biggest, the biggest among (n1,n2) is taken
  //            - If selectn == smallest, the smallest is taken.
  //
  //  Notice that you will be only partially obeyed when you ask that the
  // polarization of the photon ( the direction of its electric field E ) has
  // a given direction! The E field HAS to be orthogonal to the k vector of
  // the photon in isotropic material. The direction you have required for the
  // E field will be changed, as few as possible, so that k and E are ortho-
  // gonal.
  //  This method is called with all default parameters by method SetEmission().
  // User needs to call it only if he wants no default parameters, for instance
  // if he wants linearly polarized photons.
  //
  const Double_t torad = TMath::Pi()/180.0;
  Bool_t ok = kTRUE;
  Double_t st,ct,sf,cf;
  fChooseIndex = selectn;
  fPolarized = polar;
  if (fPolarized) {
    if (!fAnisotropic) {
      thetp *= torad;
      phip  *= torad;
      st = TMath::Sin(thetp);
      ct = TMath::Cos(thetp);
      sf = TMath::Sin(phip);
      cf = TMath::Cos(phip);
      fPolar.SetXYZ(st*cf,st*sf,ct);
      fPolar = fPolar.Unit();
    }
    else {
      const char *met = "SetEField";
      ok = kFALSE;
      fPolarized = kFALSE;
      gCleanOut->MM(warning,met,"Polarization of E field only in isotropic media",ClassName());
      gCleanOut->MM(warning,met,"No polarization taken",ClassName());
    }
  }
  return ok;
}
Bool_t TLitVolume::SetEmission(KindOfDist kind,Double_t tmax,TVector3 dir,
  const char *fitname,Bool_t srcfix,TVector3 source,Bool_t emface,TVector3 dirfce) {
  //  Declares that the TGeoVolume associated with this TLitVolume is able to emit
  // photons. All points and vectors of this method are given in the local coordinate
  // system of the TGeoVolume associated with this TLitVolume.
  //  There are 3 possibilities for the point of emission of photons by the
  // TGeoVolume:
  //
  //  (1) -  photons are emitted from any point inside the TGeoVolume with an equal
  //         probability [default]
  //  (2) -  [if srcfix true]: all photons are emitted from point "source" inside
  //         the TGeoVolume
  //  (3) -  [if emface true and srcfix false]: first, a point is generated inside
  //         the TGeoVolume with an equal probability. Then the point is translated
  //         inside the TGeoVolume, along direction dirfce, until the edge of the
  //         TGeoVolume. It is the way SLitrani generates surface emission, without
  //         having [as the old Litrani] any handling of faces. This method has the
  //         advantage of working, whatever the kind of TGeoVolume.
  //         
  //  Arguments:
  //
  // kind   : type of distribution for the direction of the emitted photons around dir
  //          [default on4pi]
  //
  //        on4pi        ==> photons are emitted isotropically, with the distribution
  //                      sin(theta)*dtheta*dphi, on 4 pi.
  //        flat         ==> photons are emitted isotropically, with the distribution
  //                      sin(theta)*dtheta*dphi,with theta between 0 and tmax
  //        sinuscosinus ==> photons are emitted with the non isotropic distribution
  //                      sin(theta)*cos(theta)*dtheta*dphi ,with theta between 0 and
  //                      tmax, favouring slightly the forward direction
  //        provided     ==> photons are emitted using the distribution provided
  //                      as a fit of type TSplineFit
  //
  // tmax   : maximum value for the theta angle around dir. tmax is given in degree
  //          [default 180.0]
  // dir    : axis around which photons are emitted, in local coordinate system of
  //          TGeoVolume [default (0,0,1)
  // fitname: in case kind == provided, name of fit to be used
  //          [default ""]
  // srcfix : if true, all photons are emitted from the fixed point source [ given in
  //          local coordinate system of TGeoVolume ] inside TGeoVolume
  //          [default false]
  // source : if srcfix true, fixed point within the TGeoVolume from which all photons are
  //          emitted. if srcfix false, irrelevant
  // emface : if true and srcfix false, photons are emitted from a face of the TGeoVolume
  //          according to possibility (3) described above.
  // dirfce : after having generated with equal probability a point anywhere inside the
  //          TGeoVolume, the point is translated along direction "dirfce" until the
  //          edge of the TGeoVolume.
  //
  const Double_t eps   = 1.0e-20;
  const Double_t pideg = 180.0;
  const char *met = "SetEmission";
  Bool_t ok = kTRUE;
  Short_t k;
  Double_t L;
  ClearEmission();
  fEmitPhoton     = kTRUE;
  fKindOfEmission = kind;
  tmax = TMath::Abs(tmax);
  if (tmax > pideg) tmax = pideg;
  fThetaMax = (tmax*TMath::Pi())/pideg;
  L = dir.Mag2();
  if (L<eps) {
    dir[0] = 0.0;
    dir[1] = 0.0;
    dir[2] = 1.0;
  }
  fAxisDir = dir;
  fAxisDir = fAxisDir.Unit();
  TString fname = fitname;
  TSplineFit *fit = 0;
  Int_t NL = fname.Length();
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
    if (emface) {
      fFromFace = kTRUE;
      fDirFace  = dirfce;
      fDirFace  = fDirFace.Unit();
    }
  }//end else if (fSourceFixed)
  ok = ok && SetEField();
  return ok;
}
void TLitVolume::SetM() {
  // fM is the matrix which transforms the 3 axis (Ox,Oy,Oz) [ Ox = (1,0,0),
  //Oy = (0,1,0), Oz = (0,0,1) ] of the WCS coordinate system into the 3
  //axis (Ox',Oy',Oz') of the coordinate system in which the dielectric
  //tensor is diagonal [DTS system]
  //
  //    Ox' = fM*Ox    Oy' = fM*Oy    Oz' = fM*Oz
  //
  // As a consequence, if E is a vector in the (Ox,Oy,Oz) WCS coordinate system,
  //and E' the corresponding vector in the DTS coordinate system, we have:
  //
  //     E' = fMt*E
  //
  //where fMt is the transposed of fM.
  //  fM and fMt allow to calculate the dielectric tensor fDTmu in WCS, once
  //the 3 diagonal elements ea, eb and ec are known.
  //
  if (fAnisotropic) {
    fM.SetColumn(0,fWCSAniOx(0),fWCSAniOx(1),fWCSAniOx(2));
    fM.SetColumn(1,fWCSAniOy(0),fWCSAniOy(1),fWCSAniOy(2));
    fM.SetColumn(2,fWCSAniOz(0),fWCSAniOz(1),fWCSAniOz(2));
    fMt.TransposedOf(fM);
    SetDielTensor();
  }
}
void TLitVolume::SetWCS() {
  //  Once a track has penetrated into one instance of the TGeoVolume associated
  // with this TlitVolume, it is possible to call
  //
  //    gGeoManager->LocalToMaster()  or gGeoManager->LocalToMasterVect()
  //
  // in order to get the transient values of all vectors used in TLitVolume in WCS,
  // valid only for the instance of the TGeoVolume in which the track is now. An
  // other instance [node] of the TGeoVolume associated with this TLitVolume, placed
  // elsewhere in the setup, would have other values in WCS for its vectors
  //
  Short_t k;
  Double_t loc_pt[3], glob_pt[3], loc_dir[3], glob_dir[3];
  if (fAnisotropic>0) {
    for (k=0;k<3;k++) loc_dir[k] = fAniOx[k];
    gGeoManager->LocalToMasterVect(loc_dir,glob_dir);
    for (k=0;k<3;k++) fWCSAniOx[k] = glob_dir[k];
    for (k=0;k<3;k++) loc_dir[k] = fAniOy[k];
    gGeoManager->LocalToMasterVect(loc_dir,glob_dir);
    for (k=0;k<3;k++) fWCSAniOy[k] = glob_dir[k];
    for (k=0;k<3;k++) loc_dir[k] = fAniOz[k];
    gGeoManager->LocalToMasterVect(loc_dir,glob_dir);
    for (k=0;k<3;k++) fWCSAniOz[k] = glob_dir[k];
    SetM();
  }
  switch (fDetKind) {
    case none:
      break;
    case apd:
      for (k=0;k<3;k++) loc_pt[k] = fXYZEntrance[k];
      gGeoManager->LocalToMaster(loc_pt,glob_pt);
      for (k=0;k<3;k++) fWCSEntrance[k] = glob_pt[k];
    case gendet:
    case phototube:
      break;
  }
  if (fDamage) {
    for (k=0;k<3;k++) loc_dir[k] = fDamAxis[k];
    gGeoManager->LocalToMasterVect(loc_dir,glob_dir);
    for (k=0;k<3;k++) fWCSDamAxis[k] = glob_dir[k];
    for (k=0;k<3;k++) loc_pt[k] = fDamStart[k];
    gGeoManager->LocalToMaster(loc_pt,glob_pt);
    for (k=0;k<3;k++) fWCSDamStart[k] = glob_pt[k];
  }
  fWCSdone = kTRUE;
}
void TLitVolume::SetNoDamage(Bool_t keepstepping) {
// Suppress radiation damage. We do not delete fits. They will be deleted
//by TSplineFit::PurgeStatic() in the destructor of TLit
// If radiation damage was the only reason to proceed by finite steps for the
//tracking of photons in this TLitMedium [ instead of simply jumping until
//next volume ], then stepping is disabled by SetNoDamage() if keepstepping
//is false [default]. Else, if there is an other reason to proceed by finite
//steps, put keepstepping to true.
  const Double_t minusone = -1.0;
  fDamage      = kFALSE;
  fDamKimax    = minusone;
  fDamFmax     = minusone;
  if (fStepping && !keepstepping) {
    fStepping = kFALSE;
    fStep = -1.0;
  }
  fDamStep     = -1.0;
  fDamZm       = minusone;
  fDamZs       = minusone;
  fDamRs       = minusone;
  fDamGz       = minusone;
  fDamGzMax    = minusone;
  fDamByShower = kFALSE;
  fDamE0       = minusone;
  fDamGamma    = kTRUE;
  fDamA        = minusone;
  fRk2         = minusone;
  fDamFac      = minusone;
  if (fHDamage) {
    delete fHDamage;
    fHDamage = 0;
  }
// Do not do:   delete fDamFitKimax;
  fDamFitKimax = 0;
// Do not do:   delete fDamFitGz;
  fDamFitGz = 0;
}
void TLitVolume::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) {
  //  Sets the 3 optical axis fAniOx, fAniOy, fAniOz in LOCAL coordinates of
  // associated TGeoVolume. The 3 optical axis are the 3 axis of the coordinate
  // system in which the dielectric tensor is diagonal, with eigenvalues
  //
  //   ea   along the Ox axis
  //   eb   along the Oy axis
  //   ec   along the Oz axis
  //
  //   ea <  eb <  ec  for totally anisotropic crystals (==birefringent)
  //   ea <  eb =  ec  for "uniaxial negativ birefringent" crystals
  //   ea =  eb <  ec  for "uniaxial positiv birefringent" crystals
  //
  //  These 3 axis are given "a la GEANT" : give theta and phi of fAniOx axis
  // (thetax,phix), theta and phi of fAniOy axis   (thetay,phiy). It is not
  // necessary to give theta and phi of fAniOz : fAniOz can be determined with
  // fAniOx and fAniOy. This way of giving a coordinate system inside the local
  // coordinate system is intuitive, but inelegant, because it is redundant. If
  // you give an fAniOy axis which is not orthogonal to the fAniOx axis, you will
  // be warned and your fAniOy axis will be corrected. GIVE ALL ANGLES IN DEGREE !
  //  To understand the construction of the dielectric tensor both in Local
  // Coordinate System and in the World Coordinate system, read the general
  // description of the classes TLitMedium and TLitVolume.
  //
  const Double_t zero   = 0.0;
  const Double_t limit  = 1.0e-6;
  const Double_t degrad = 0.0174532925199432958;
  const Double_t wsmall = 1.0e-16;
  Bool_t problem = kFALSE;
  Double_t test;
  Double_t sthetax, cthetax, sthetay, cthetay;
  Double_t sphix, cphix, sphiy, cphiy;
  TVector3 yv, zv;
  thetax *= degrad;  thetay *= degrad;
  phix   *= degrad;  phiy   *= degrad;
  sthetax = TMath::Sin(thetax);  cthetax = TMath::Cos(thetax);
  sthetay = TMath::Sin(thetay);  cthetay = TMath::Cos(thetay);
  sphix   = TMath::Sin(phix);    cphix   = TMath::Cos(phix);
  sphiy   = TMath::Sin(phiy);    cphiy   = TMath::Cos(phiy);
  if (TMath::Abs(sthetax)<wsmall) sthetax = zero;
  if (TMath::Abs(cthetax)<wsmall) cthetax = zero;
  if (TMath::Abs(sthetay)<wsmall) sthetay = zero;
  if (TMath::Abs(cthetay)<wsmall) cthetay = zero;
  if (TMath::Abs(sphix)<wsmall) sphix = zero;
  if (TMath::Abs(cphix)<wsmall) cphix = zero;
  if (TMath::Abs(sphiy)<wsmall) sphiy = zero;
  if (TMath::Abs(cphiy)<wsmall) cphiy = zero;
  fAniOx(0) = sthetax*cphix;
  fAniOx(1) = sthetax*sphix;
  fAniOx(2) = cthetax;
  fAniOy(0) = sthetay*cphiy;
  fAniOy(1) = sthetay*sphiy;
  fAniOy(2) = cthetay;
  fAniOx    = fAniOx.Unit();
  fAniOy    = fAniOy.Unit();
  //verification of orthogonality of fAniOy with fAniOx
  zv = fAniOx.Cross(fAniOy);
  zv = zv.Unit();
  yv = zv.Cross(fAniOx);
  yv = yv.Unit();
  if (TMath::Abs(yv(0))<wsmall) yv(0) = zero;
  if (TMath::Abs(yv(1))<wsmall) yv(1) = zero;
  if (TMath::Abs(yv(2))<wsmall) yv(2) = zero;
  test = TMath::Abs(fAniOx*fAniOy);
  if (test>limit) {
    problem = kTRUE;
    gCleanOut->MMR(error,"SetOpticalAxis","x and y axis is not orthogonal","test",test,ClassName());
    gCleanOut->MM(info,"SetOpticalAxis","We change it !",ClassName());
  }
  fAniOy = yv;
  zv = fAniOx^fAniOy;
  zv = zv.Unit();
  fAniOz = zv;
  if (problem) {
    gCleanOut->MM(warning,"SetOpticalAxis","Values taken for the optical axis :",ClassName());
    fAniOx.Print();
    fAniOy.Print();
    fAniOz.Print();
  }
  if (!fAnisotropic) {
    gCleanOut->MM(warning,"SetOpticalAxis","Why defining optical axis for an isotropic media ?",ClassName());
  }
}
void TLitVolume::SetPhotoTube(const char *nameqe,Double_t dir,Double_t accept) {
  //  Before calling this metod to declare a phototube, you have to define,
  // during the building of the geometry, at least 3 TGeoTube, and to place
  // them inside the setup.
  //  (1) - the 1st TGeoTube, let us call it PMrev, represent the container
  // of the phototube. The window of the PM and the photocathode are inside
  // PMrev. PMrev is filled with a TlitMedium which is a wrapping. Photons
  // can hit this wrapping from 2 sides:
  //      (1a) - from inside: photons reaching the PM window, but not
  //            succeeding in crossing it and reach the photocathode, but
  //            escaping from the lateral cylindrical face of the window
  //            of the PM will hit this wrapping and either be absorbed or
  //            reflected.
  //      (1b) - from outside: may be your phototube is inside a TgeoVolume
  //            where photons can travel. In that case, a photon can hit
  //            the wall of the phototube and be absorbed or reflected.
  // In the case where the characteristics of the surface inside and outside
  // of PMrev are different, you will be obliged to define 2 TgeoTube of
  // type PMrev, one inside the other. You will then have 4 TGeoTube in all.
  //  (2) - the 2nd TGeoTube, let us call it PMwin, represent the entrance
  // window ot the phototube, which is may be made of TLitMedium "sodocal"
  // which is not a wrapping. Photons can travel inside PMwin, from the
  // plane circular entrance face to the plane circular exit face, which is
  // considered as in direct contact with the photocathode. The TLitMedium
  // inside PMwin, for instance "sodocal" must be declared as NOT sensible,
  // [4th argument of constructor], since a photon diing inside the window
  // because of finite absorption length of "sodocal", must be considered
  // as lost, not as detected.
  //  (3) - the 3rd TGeoTube, let us call it PM, represent the activ part
  // of the phototube. Belonging to PM, but in contact with PMwin is the
  // photocathode. Since a photon always stops when in contact with the
  // photocathode, a photon never travels inside PM. However, in order that
  // a photon reaching the exit face of the window PMwin directly hit the
  // photocathode, without being possibly reflected [this possibility is
  // already included when giving the quantum efficiency of the photocathode.
  // allowing for it in SLitrani would be double counting!], it is advisable
  // to give to the TLitMedium filling PM [let us call it "cathode"], exactly
  // the same characteristics as the TLitMedium "sodocal" of PMwin. But
  // "cathode has to be declared sensible [4th argument of constructor],
  // since here the photon can be detected. Notice also that the width 2*dz
  // that you give to PM does not matter, since photons immediatly stop at
  // the photocathode. But for aesthetical reasons, it is nicer to let
  // PM almost fill PMrev-PMwin.
  //  Then you have to upgrade the 3rd TGeoTube, PM, the sensible one,
  // into a TLitVolume, and call SetPhotoTube() for this TLitVolume, for
  // instance like this:
  //
  //     ==> TLitVolume *Lit_PM = new TLitVolume(PM);
  //     ==> Lit_PM->SetPhotoTube("QuantumEff_XP2020",1.0);
  //
  //  You can find an implementation of phototubes as described above in
  // the CINT macro Nucifer.C in directory SMacros of SLitrani.
  //
  // Arguments:
  //
  //    nameqe : There is a condition of cathode quantum efficiency versus
  //             wavelength, given by a TSplineFit of category 10. nameqe
  //             is the name of this TSplineFit nameqe cannot be "" nor
  //             "none".
  //    dir    : If the Oz axis of the TGeoTube PMwin representing the
  //             entrance window points towards the TGeoTube PM representing
  //             the photocathode, then dir is +1.0
  //             [ it is the case for instance for a vertical crystal having a
  //             phototube at its top ]
  //             if not, i.e. if the -Oz [minus Oz] axis of the TGeoTube
  //             representing the entrance window points towards the
  //             TGeoTube representing the photocathode, then dir is -1.0 
  //             [ it is the case for instance for a vertical crystal having a
  //             phototube at its bottom ]
  //    accept : By default, there is no acceptance angle, but you can impose
  //             one, if you want, by giving to accept a value between 0.0
  //             and 90.0.
  //
  const char *met = "SetPhotoTube";
  fDetKind    = phototube;
  fIsQE       = kTRUE;
  fFitQE      = 0;
  TString s1  = "";
  TString s2  = "none";
  fNameQE     = "";
  fStopAlways = kTRUE;
  fIsQE = !(!s1.CompareTo(nameqe) || !s2.CompareTo(nameqe));
  if (fIsQE) {
    fFitQE = TSplineFit::FindFit(nameqe,10);
    if ((fIsQE) && (!fFitQE)) {
      gCleanOut->MM(fatal,met,"Fit on quantum efficiency not found",ClassName());
    }
    else fNameQE = nameqe;
  }
  else gCleanOut->MM(fatal,met,"Phototube must have quantum efficiency",ClassName());
  Double_t dz = ((TGeoBBox*)fGeoVolume->GetShape())->GetDZ();
  fEx         = ((TGeoBBox*)fGeoVolume->GetShape())->GetDX();
  fEy         = ((TGeoBBox*)fGeoVolume->GetShape())->GetDY();
  fEz         = 0.0; // Because fStopAlways == true
  if (dir>=0.0) {
    fDetAxis.SetXYZ(0.0,0.0,1.0);
    fXYZEntrance.SetXYZ(0.0,0.0,-dz);
    fEntrance = 4;
  }
  else {
    fDetAxis.SetXYZ(0.0,0.0,-1.0);
    fXYZEntrance.SetXYZ(0.0,0.0,dz);
    fEntrance = 5;
  }
  if ((accept<=0.0) || (accept >= 90.0)) {
    fIsAccept  = kFALSE;
    fCosAccept = 1.0;
  }
  else {
    fIsAccept  = kTRUE;
    fCosAccept = TMath::Cos((accept*TMath::Pi())/180.0);
  }
}
void TLitVolume::SetAxisDir(Double_t nx,Double_t ny,Double_t nz) {
  // Define axis of emission of photon in local coordinate system of TGeoVolume
  fAxisDir[0] = nx;
  fAxisDir[1] = ny;
  fAxisDir[2] = nz;
  fAxisDir    = fAxisDir.Unit();
}
Bool_t TLitVolume::SetSourcePoint(Bool_t srcfix,TVector3 source) {
  // If srcfix is true, decides that all photons are emitted from point source
  //in local coordinate of the TGeoVolume.
  // If srcfix is false, photons are emitted from anywhere inside the TGeoVolume,
  //with equal probability. source is then irrelevant
  Bool_t ok = kTRUE;
  fSourceFixed = srcfix;
  if (fSourceFixed) {
    Short_t k;
    Double_t x[3];
    for (k=0;k<3;k++) x[k] = source[k];
    if (!fGeoVolume->Contains(x)) {
      const char *met = "SetSourcePoint";
      gCleanOut->MM(error,met,"Given point is not inside TGeoVolume",ClassName());
      gCleanOut->MM(error,met,"We switch to no fix point",ClassName());
      fSourceFixed = kFALSE;
      ok = kFALSE;
    }
    fSourcePoint = source;
  }
  return ok;
}
Double_t TLitVolume::SetStep(Double_t st) {
  // If st>0  and fDamage is false, stepping is required with a step equal to st
  // If st>0  and fDamage is true,  stepping is required with a step equal to
  //                                 Min(st,fDamStep)
  // If st<=0 and fDamage is false, stepping is disabled
  // If st<=0 and fDamage is true,  stepping is keeped with a step equal to fDamStep
  if (st>0) {
    fStepping = kTRUE;
    if (fDamage) fStep = TMath::Min(st,fDamStep);
    else         fStep = st;
  }
  else {
    if (fDamage) {
      fStepping = kTRUE;
      fStep     = fDamStep;
    }
    else {
      fStepping = kFALSE;
      fStep     = -1.0;
    }
  }
  return fStep;
}
void TLitVolume::ShowDamage() const {
// Shows calculated distribution 1 - damage factor
  if (fDamage) {
    if (fHDamage) {
      fHDamage->Draw("LEGO2");
      if (gTwoPad) gTwoPad->ChangePad();
      else {
        gPad->Modified();
        gPad->Update();
      }
    }
  }
}
Bool_t TLitVolume::ShowerDamage(const TVector3 *damaxis, const TVector3 *damstart,
  Double_t Kimax, Double_t E0, Bool_t gamma, Double_t step,Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
// ShowerDamage() is an alternative to RadiationDamage() to be preferred when
//the damages have been caused by the irradiation by electromagnetic
//showers, gammas or electrons. ShowerDamage() will generate damages with
//the same (z,r) distributions as the showers which have caused the damages.
// The chosen (z,r) distribution is the one which is described in the
//class description of TLitCascade, and which is also described in the
//Review of Particle Physics, Vol 3 N°1-4, 1998 pages 148-149.
// In order that ShowerDamage() can be called for this TGeoVolume, it is necessary
//that radiation length, critical energy and Moliere radius have been
//defined for the material of this TGeoVolume. It means that you have called 
//TLitMedium::SetCascades(),
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume. It means that you have to call ShowerDamage() BEFORE the
//TGeoVolume goes into world coordinates, i.e. before the constructor of TGeoNode
//for this TGeoVolume is called, which makes all coordinates to go into world
//coordinates.
//
//  Parameters :
//
// damaxis   : axis along which damage varies
// damstart  : starting point for calculating the damages == starting point
//             of the cascades having produced the damages.
// Kimax     : 1/Limin, where Limin is the contribution to the absorption
//             length of the damages, at the point where it is maximal. In
//             this instance of ShowerDamage(), Kimax is not taken as depending
//             upon wavelength.
// E0        : energy in [Gev] of the damaging cascades
// gamma     : if true, damaging cascades are gammas. If false : electrons
// step      : steps for calculating absorption [cm]. Default 0.4. Refuses
//             values smaller than 0.001.
// damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
//                increasing the damages if bigger than 1.
//
  const Double_t zero = 0.0;
  const Double_t b    = 0.5;
  const Double_t un   = 1.0;
  const Double_t small = 0.001;
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  Double_t X0,Rm,Ec;
  X0 = fLitMedium->GetX0();
  Rm = fLitMedium->GetRm();
  Ec = fLitMedium->GetEc();
  if ((X0>zero) && (Rm>zero) && (Ec>zero)) {
    Double_t Cj,y,tmax,am1,Rk;
    Double_t facdEdz,facdEdr;
    fDamByShower = kTRUE;
    fDamage      = kTRUE;
    fDamKimax    = Kimax;
    fDamStep     = step;
    fStepping    = kTRUE;
    if (fStep>fDamStep) fStep = fDamStep;
    fDamAxis     = *damaxis;
    fDamAxis     = fDamAxis.Unit();
    fDamStart    = *damstart;
    fDamE0       = 1000.0*E0;
    fDamGamma    = gamma;
    if (fDamGamma) Cj =  0.5;
    else           Cj = -0.5;
    y            = fDamE0/Ec;
    tmax         = TMath::Log(y) + Cj;
    fDamA        = b*tmax + un;
    am1          = fDamA - un;
    Rk           = Rm/3.0;
    fRk2         = Rk*Rk;
    facdEdz      = (fDamE0*b)/(X0*TMath::Gamma(fDamA));
    facdEdr      = fRk2/TMath::Pi();
    fDamFac      = facdEdz*facdEdr;
    fDamFmax     = fDamFac*TMath::Exp(am1*TMath::Log(am1))*TMath::Exp(-am1);
    fDamFmax    /= fRk2*fRk2;
    fDamFmax     = TMath::Abs(fDamFmax);
  }
  else gCleanOut->MM(fatal,"ShowerDamage","TLitMedium::SetCascades MUST be called first!",ClassName());
  if (!fDamage) gCleanOut->MM(error,"ShowerDamage","No radiation damage assumed",ClassName());
  return fDamage;
}
Bool_t TLitVolume::ShowerDamage(const TVector3 *damaxis, const TVector3 *damstart,
  const char *namefitkimax, Double_t E0, Bool_t gamma, Double_t step,Double_t damsupfac) {
// Please read first the chapter "Simulating radiation damages inside a
//TGeoVolume" in the class description of this class for a full understanding of
//how radiation damages are implemented in SLitrani.
// ShowerDamage() is an alternative to RadiationDamage() to be preferred when
//the damages have been caused by the irradiation by electromagnetic
//showers, gammas or electrons. ShowerDamage() will generate damages with
//the same (z,r) distributions as the showers which have caused the damages.
// The chosen (z,r) distribution is the one which is described in the
//class description of TLitCascade, and which is also described in the
//Review of Particle Physics, Vol 3 N°1-4, 1998 pages 148-149.
// In order that ShowerDamage() can be called for this TGeoVolume, it is necessary
//that radiation length, critical energy and Moliere radius have been
//defined for the material of this TGeoVolume. It means that you have called 
//TLitMedium::SetCascades(),
//  For a point (x,y,z) in local coordinate of the TGeoVolume, the position
//dependent factor is obtained by projecting the point (x,y,z) on the
//the axis having the direction "damaxis" and going through the point
//"damstart". The distance z on the axis from this projected point to the
//starting point "damstart" and the distance r from the point (x,y,z) to
//the axis damaxis give the couple (z,r) for which the damage is calculated.
//  damaxis and damstart are to be given in the local coordinate system of
//the TGeoVolume. It means that you have to call ShowerDamage() BEFORE the
//TGeoVolume goes into world coordinates, i.e. before the constructor of TGeoNode
//for this TGeoVolume is called, which makes all coordinates to go into world
//coordinates.
//
//  Parameters :
//
// damaxis      : axis along which damage varies
// damstart     : starting point for calculating the damages == starting point
//                 of the cascades having produced the damages.
// namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
//                 contribution to the absorption length of the damages, at the
//                 point where it is maximal. In this instance of ShowerDamage(),
//                 Kimax is depending upon wavelength.
// E0           : energy in [Gev] of the damaging cascades
// gamma        : if true, damaging cascades are gammas. If false : electrons
// step         : steps for calculating absorption [cm]. Default 0.4. Refuses
//                 values smaller than 0.001.
// damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
//                increasing the damages if bigger than 1.
//
  const Double_t zero = 0.0;
  const Double_t b    = 0.5;
  const Double_t un   = 1.0;
  const Double_t small = 0.001;
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  Double_t X0,Rm,Ec;
  X0 = fLitMedium->GetX0();
  Rm = fLitMedium->GetRm();
  Ec = fLitMedium->GetEc();
  if ((X0>zero) && (Rm>zero) && (Ec>zero)) {
    if (fDamFitKimax) {
      delete fDamFitKimax;
      fDamFitKimax = 0;
    }
    fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
    if (fDamFitKimax) {
      Double_t Cj,y,tmax,am1,Rk;
      Double_t facdEdz,facdEdr;
      fDamByShower = kTRUE;
      fDamage      = kTRUE;
      fDamStep     = step;
      fStepping    = kTRUE;
      if (fStep>fDamStep) fStep = fDamStep;
      fDamAxis     = *damaxis;
      fDamAxis     = fDamAxis.Unit();
      fDamStart    = *damstart;
      fDamE0       = 1000.0*E0;
      fDamGamma    = gamma;
      if (fDamGamma) Cj =  0.5;
      else           Cj = -0.5;
      y            = fDamE0/Ec;
      tmax         = TMath::Log(y) + Cj;
      fDamA        = b*tmax + un;
      am1          = fDamA - un;
      Rk           = Rm/3.0;
      fRk2         = Rk*Rk;
      facdEdz      = (fDamE0*b)/(X0*TMath::Gamma(fDamA));
      facdEdr      = fRk2/TMath::Pi();
      fDamFac      = facdEdz*facdEdr;
      fDamFmax     = fDamFac*TMath::Exp(am1*TMath::Log(am1))*TMath::Exp(-am1);
      fDamFmax    /= fRk2*fRk2;
      fDamFmax     = TMath::Abs(fDamFmax);
    }
    else gCleanOut->MM(error,"ShowerDamage","Fit for damages not found",ClassName());
  }
  else gCleanOut->MM(fatal,"ShowerDamage","TLitMedium::SetCascades MUST be called first!",ClassName());
  if (!fDamage) gCleanOut->MM(error,"ShowerDamage","No radiation damage assumed",ClassName());
  return fDamage;
}
