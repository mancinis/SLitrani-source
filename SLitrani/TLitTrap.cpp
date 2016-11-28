// @(#)root/html:$Id: TLitTrap.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "TCleanOut.h"
#include "TMat33.h"
#include "TLitTrap.h"

ClassImp(TLitTrap)
//______________________________________________________________________________
//
/* BEGIN_HTML
		<center>
			<h1>TLitTrap</h1>
		</center>
		<p></p>
		<p>A <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is a general trapezoid. See the drawing of a <b><code>TGeoTrap</code></b> in the 
<b><code>ROOT</code></b> documentation. The faces perpendicular to z are trapeze 
and their centers are not necessarily on a line parallel to the z axis. The standard 
constructor of this shape has 11 parameters, shown in the table below. Due to the 
complexity of these parameters, it is rarely the case that one knows all of them 
when one has to define a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>! 
It is more frequently the case that one knows the 8 positions in space of the 8 corners 
of the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>, 
or, as is for instance the case for the crystals of the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
experiment, one knows the length of some of the ridges of the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>. 
The <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
representing a <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystal is precisely defined at page 53 of &quot;<b><code>The Electromagnetic Calorimeter 
Project - Technical Design Report</code></b>&quot;, by giving the 6 sides <b><code>AR, 
BR, CR, AF, BF, CF</code></b> of the crystal and the height <b><code>dz</code></b>. 
The aim of this class <b><code>TLitTrap</code></b> is to ease the definition of a 
<b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>, 
by using either:</p>
		<ul>
			<li><b><font color="#cc3333">The first constructor</font></b> of <b><code>TLitTrap</code></b> 
takes the eleven standard parameters <b><code>dz, theta, phi, h1, bl1, tl1, alpha1, 
h2, bl2, tl2, alpha2</code></b>, as the constructor of <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>,<font color="#cc3333"> 
but verifies that they give rise to a shape with <b>planar</b> faces, which is not 
at all warranted!</font>
			<li><b><font color="#cc3333">The second constructor</font></b> of <b><code>TLitTrap</code></b> 
starts from the coordinates of the 8 apexes of the shape, changing them minimally 
but such that the 6 faces of the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
are planar and calculates from there the standard parameters <b><code>dz, theta ,phi, 
h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2</code></b>.
			<li><b><font color="#cc3333">The third constructor</font></b> of <b><code>TLitTrap 
</code></b>allows to get the standard parameters <b><code>dz, theta, phi, h1, bl1, 
tl1, alpha1, h2, bl2, tl2, alpha2</code></b> starting from the definition of the 
ridges <b><code>dz,AR,BR,CR,AF,BF,CF</code></b>, as is done for the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystal.&nbsp;Notice that if one can define a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
with only these 7 parameters, the so defined <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is not general, it satisfies in addition the conditions <b><code>x3 = x0, x4 = x0, 
x7 = x4, y4 = y0</code></b>, which is the case for a <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystal.
			<li><b><font color="#cc3333">The fourth constructor</font></b> is strictly reserved 
for use with the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystals.&nbsp;Knowing that there are only 18 crystal types in <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b>, 
it is sufficient for this constructor to give the type of the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystal [1-18], and it delivers the standard parameters <b><code>dz, theta ,phi, 
h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2.</code></b>
		</ul>
		<p></p>
		<p></p>
		<div align="center">
			<h2><font color="#cc3333">1st constructor</font></h2>
			<h2></h2>
		</div>
		<table border="6" cellpadding="2" cellspacing="3" bgcolor="#cccc99">
			<caption>
				<h4>11 parameters of the first constructor</h4>
			</caption>
			<tr>
				<td>
					<center>
						<h4><b><code><font color="#cc3333">Type</font></code></b></h4>
					</center>
				</td>
				<td>
					<center>
						<h4><b><code><font color="#cc3333">Parameter</font></code></b></h4>
					</center>
				</td>
				<td>
					<center>
						<h4><b><code><font color="#cc3333">fName</font></code></b></h4>
					</center>
				</td>
				<td>
					<center>
						<h4><b><code><font color="#cc3333">Notation</font></code></b></h4>
					</center>
				</td>
				<td>
					<center>
						<h4><b><code><font color="#cc3333">Definition</font></code></b></h4>
					</center>
				</td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>dz</code></b></td>
				<td><b><code>fDz</code></b></td>
				<td><b><code>d<sub>z</sub></code></b></td>
				<td><b><code>half-length along the z axis in cm</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>theta</code></b></td>
				<td><b><code>fTheta</code></b></td>
				<td><font face="Symbol"><b><code>q</code></b></font></td>
				<td><b><code>polar angle of the line joining the centre of the face at -dz to 
the centre of the one at +dz.. <font color="#9c0031">0 &lt;= </font><font face="Symbol" color="#9c0031">q</font><font color="#9c0031"> 
&lt; 90&deg;</font></code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>phi</code></b></td>
				<td><b><code>fPhi</code></b></td>
				<td><font face="Symbol"><b><code>f</code></b></font></td>
				<td><b><code>azimuthal angle of the line joining the centre of the face at -dz 
to the centre of the one at +dz. <font color="#9c0031">0 &lt;= </font><font face="Symbol" color="#9c0031">f</font><font color="#9c0031"> 
&lt; 360&deg;</font></code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>h1</code></b></td>
				<td><b><code>fH1</code></b></td>
				<td><b><code>h<font size="2"><sub>1</sub></font></code></b></td>
				<td><b><code>half-length along y of the face at -dz in cm</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>bl1</code></b></td>
				<td><b><code>fBl1</code></b></td>
				<td><b><code>b<sub>1</sub></code></b></td>
				<td><b><code>half-length along x of the side at -h1 in y of the face at -dz in 
z in cm.</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>tl1</code></b></td>
				<td><b><code>fTl1</code></b></td>
				<td><b><code>t<sub>1</sub></code></b></td>
				<td><b><code>half-length along x of the side at +h1 in y of the face at -dz in 
z in cm.</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>alpha1</code></b></td>
				<td><b><code>fAlpha1</code></b></td>
				<td><b><code><font face="Symbol">a</font><sub>1</sub></code></b></td>
				<td><b><code>angle with respect to the y axis from the centre of the side at 
-h1 in y to the centre of the side at +h1 in y of the face at -dz in z.</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>h2</code></b></td>
				<td><b><code>fH2</code></b></td>
				<td><b><code>h<sub>2</sub></code></b></td>
				<td><b><code>half-length along y of the face at +dz in cm.</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>bl2</code></b></td>
				<td><b><code>fBl2</code></b></td>
				<td><b><code>b<sub>2</sub></code></b></td>
				<td><b><code>half-length along x of the side at -h2 in y of the face at +dz in 
z in cm.</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>tl2</code></b></td>
				<td><b><code>fTl2</code></b></td>
				<td><b><code>t<sub>2</sub></code></b></td>
				<td><b><code>half-length along x of the side at +h2 in y of the face at +dz in 
z in cm</code></b></td>
			</tr>
			<tr>
				<td><b><code>Double_t</code></b></td>
				<td><b><code>alpha2</code></b></td>
				<td><b><code>fAlpha2</code></b></td>
				<td><b><code><font face="Symbol">a</font><sub>2</sub></code></b></td>
				<td><b><code>angle with respect to the y axis from the centre of the side at 
-h2 in y to the centre of the side at +h2 in y of the face at +dz in z.</code></b></td>
			</tr>
		</table>
		<p></p>
		<p><b><code>TLitTrap</code></b> does not inherit from <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>! 
It is only there as an help before defining a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>! 
Once you have called the right constructor of <b><code>TLitTrap</code></b>, you can 
get the 11 parameters of the constructor of <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
by calling method <b><code><a href="#TLitTrap:AllParams">TLitTrap::AllParams()</a></code></b>, 
or directly book the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
by calling method <b><code><a href="#TLitTrap:GeoTrap">TLitTrap::GeoTrap()</a></code></b>. 
You can then delete <b><code>TLitTrap</code></b>, you do not need it anymore.</p>
		<p></p>
		<p><b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
presents the following difficulty : nothing prevents the user to give values to the 
parameters such that the lateral faces are not plane ! In order to prevent this, 
checks are done in the constructors of <b><code>TLitTrap</code></b> and values are 
changed in order that faces become plane. More specifically, the 8 points [apexes] 
of the shape are calculated using all the input parameters. Points<b><code> 0 1 2 
3 </code></b> [the points at the bottom] are always taken as given, points <b><code>4 
5 6 7</code></b> [at the top] are moved the least possible, but such that lateral 
faces become plane. Then one recalculates the new values of the parameters from the 
new coordinates of the points and inform the user by a print that the parameters 
have been changed. The new values of the parameters or of the points can then be 
obtained by the <b><code>Get...</code></b> methods, in particular <b><code><a href="#TLitTrap:AllParams">TLitTrap::AllParams()</a></code></b> 
and <b><code><a href="#TLitTrap:GetPoints">TLitTrap::GetPoints()</a></code></b>.</p>
		<p>Let us show how one goes from parameters to coordinates and vice-versa.</p>
		<h3>Relation between those parameters and coordinates of the apexes of the shape.</h3>
		<p>Let us define <b><code>(x<sub>k</sub>,y<sub>k</sub>,z<sub>k</sub>) k=0..7</code></b> 
the coordinates of the 8 apexes of a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>. 
Suppose that we have <b>a very general shape with 8 apexes</b>, satisfying <b>only</b> 
to the following conditions :</p>
		<center>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption></caption>
				<tr>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td>
						<center>
							<h4><b><code>conditions</code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td><b><code>(1)</code></b></td>
					<td><b><code>z<sub>0</sub> = z<sub>1</sub> = z<sub>2</sub> = z<sub>3</sub> = 
-d<sub>z</sub></code></b></td>
				</tr>
				<tr>
					<td><b><code>(2)</code></b></td>
					<td><b><code>z<sub>4</sub> = z<sub>5</sub> = z<sub>6</sub> = z<sub>7</sub> = 
+d<sub>z</sub></code></b></td>
				</tr>
				<tr>
					<td><b><code>(3)</code></b></td>
					<td><b><code>y<sub>1</sub> = y<sub>0</sub></code></b></td>
				</tr>
				<tr>
					<td><b><code>(4)</code></b></td>
					<td><b><code>y<sub>3</sub> = y<sub>2</sub></code></b></td>
				</tr>
				<tr>
					<td><b><code>(5)</code></b></td>
					<td><b><code>y<sub>5</sub> = y<sub>4</sub></code></b></td>
				</tr>
				<tr>
					<td><b><code>(6)</code></b></td>
					<td><b><code>y<sub>7</sub> = y<sub>6</sub></code></b></td>
				</tr>
			</table>
		</center>
		<p>Then we intend to show first that this shape <b>can be represented</b> by a 
<b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>. 
( If your shape satisfies conditions (1) to (4), but not (5) nor (6), then your last 
hope is a <b><code><a href="http://root.cern.ch/root/html/TGeoGtra.html">TGeoGtra</a></code></b> 
! ). First let us remind that we are always allowed to make a translation along <b><code>Ox</code></b> 
and a translation along <b><code>Oy</code></b>, i.e. to choose the position of the 
center of the coordinate system in the plane <b><code>Oxy</code></b>.&nbsp;For that 
reason, we do not write &quot;<b><code>x<sub>k</sub></code></b>&quot;, but &quot;<b><code>x<sub>k</sub> 
+ t<sub>x</sub></code></b>&quot;, and not &quot;<b><code>y<sub>k</sub></code></b>&quot; 
but &quot;<b><code>y<sub>k</sub>+t<sub>y</sub></code></b>&quot;, and we keep the 
possibility to choose <b><code>t<sub>x</sub></code></b> and <b><code>t<sub>y</sub></code></b> 
in the most clever way until the end of the calculation. We intend to show that starting 
from the coordinates , satisfying the 6 conditions, we can find back the values of 
the parameters of the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>, 
which proves what we intended to prove.</p>
		<center>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>relation between the coordinates and the parameters</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">coord.</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">relation</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Eq.</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">comments</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>0</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> 
+ t<sub>1</sub></code></b></td>
					<td><b><code>(1)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>1</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> 
- t<sub>1</sub></code></b></td>
					<td><b><code>(2)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>2</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> 
- b<sub>1</sub></code></b></td>
					<td><b><code>(3)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>3</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> 
+ b<sub>1</sub></code></b></td>
					<td><b><code>(4)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>y<sub>0 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(5)</code></b></td>
					<td><b><code>condition (3) ok :</code></b></td>
				</tr>
				<tr>
					<td><b><code>y<sub>1 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(6)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; y<sub>1 </sub>= y<sub>0</sub></code></b></font></td>
				</tr>
				<tr>
					<td><b><code>y<sub>2 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>-h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(7)</code></b></td>
					<td><b><code>condition (4) ok :</code></b></td>
				</tr>
				<tr>
					<td><b><code>y<sub>3 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>-h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(8)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; y<sub>3 </sub>= y<sub>2</sub></code></b></font></td>
				</tr>
				<tr>
					<td><b><code>x<sub>4</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> 
+ t<sub>2</sub></code></b></td>
					<td><b><code>(9)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>5</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> 
- t<sub>2</sub></code></b></td>
					<td><b><code>(10)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>6</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> 
- b<sub>2</sub></code></b></td>
					<td><b><code>(11)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>x<sub>7</sub> + t<sub>x</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> 
+ b<sub>2</sub></code></b></td>
					<td><b><code>(12)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>y<sub>4 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(13)</code></b></td>
					<td><b><code>condition (5) ok :</code></b></td>
				</tr>
				<tr>
					<td><b><code>y<sub>5 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(14)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; y<sub>5 </sub>= y<sub>4</sub></code></b></font></td>
				</tr>
				<tr>
					<td><b><code>y<sub>6 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>-h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(15)</code></b></td>
					<td><b><code>condition (6) ok :</code></b></td>
				</tr>
				<tr>
					<td><b><code>y<sub>7 </sub>+ t<sub>y</sub></code></b></td>
					<td>
						<center>
							<b><code>
							=</code></b></center>
					</td>
					<td><b><code>-h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> 
sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(16)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; y<sub>7 </sub>= y<sub>6</sub></code></b></font></td>
				</tr>
			</table>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>From equations (1) to (8), we obtain :</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">From</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Eq.</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">comments</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td>
						<div align="left">
							<b><code>
							(5) - (7)</code></b></div>
					</td>
					<td>
						<div align="right">
							<b><code>
							y<sub>0 </sub>- y<sub>2</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>2h<font size="2"><sub>1</sub></font></code></b></td>
					<td><b><code>(17)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; h</font><font size="2" color="#9c3031"><sub>1</sub></font><font color="#9c3031"> 
is found</font></code></b></td>
				</tr>
				<tr>
					<td><b><code>(1) - (2)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>0</sub> - x<sub>1</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>2t<sub>1</sub></code></b></td>
					<td><b><code>(18)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; t<sub>1</sub> is found</code></b></font></td>
				</tr>
				<tr>
					<td><b><code>(3) - (4)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>2</sub> - x<sub>3</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>-2b<sub>1</sub></code></b></td>
					<td><b><code>(19)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; b<sub>1</sub> is found</code></b></font></td>
				</tr>
				<tr>
					<td><b><code>(5) + (7)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							y<sub>0 </sub>+ y<sub>2 </sub>+ 2t<sub>y</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- 2d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(20)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(1) + (2)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>0</sub> + x<sub>1</sub> + 2t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ 2h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></code></b></td>
					<td><b><code>(21)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(3) + (4)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>2</sub> + x<sub>3</sub> + 2t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- 2h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></code></b></td>
					<td><b><code>(22)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>Definition :</code></b></td>
					<td>
						<div align="right">
							<b><code>
							X<sub>03</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>x<sub>0</sub> + x<sub>1</sub> + x<sub>2</sub> + x<sub>3</sub></code></b></td>
					<td><b><code>(23)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(21) + (22)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							X<sub>03</sub> + 4t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- 4d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font></code></b></td>
					<td><b><code>(24)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>Definition :</code></b></td>
					<td>
						<div align="right">
							<b><code>
							Y<sub>03</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>y<sub>0 </sub>+ y<sub>1 </sub>+ y<sub>2 </sub>+ y<sub>3</sub></code></b></td>
					<td><b><code>(25)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(20) and c3/4</code></b></td>
					<td>
						<div align="right">
							<b><code>
							Y<sub>03 </sub>+ 4t<sub>y</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- 4d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(26)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
			</table>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>From relations (9) to (16) we obtain :</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">From</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Eq.</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">comments</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td>
						<div align="left">
							<b><code>
							(13) - (15)</code></b></div>
					</td>
					<td>
						<div align="right">
							<b><code>
							y<sub>4 </sub>- y<sub>6</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>2h<font size="2"><sub>2</sub></font></code></b></td>
					<td><b><code>(27)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; h</font><font size="2" color="#9c3031"><sub>2</sub></font><font color="#9c3031"> 
is found</font></code></b></td>
				</tr>
				<tr>
					<td><b><code>(9) - (10)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>4</sub> - x<sub>5</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>2t<sub>2</sub></code></b></td>
					<td><b><code>(28)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; t<sub>2</sub> is found</code></b></font></td>
				</tr>
				<tr>
					<td><b><code>(11) - (12)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>6</sub> - x<sub>7</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>-2b<sub>2</sub></code></b></td>
					<td><b><code>(29)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; b<sub>2</sub> is found</code></b></font></td>
				</tr>
				<tr>
					<td><b><code>(13) + (15)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							y<sub>4 </sub>+ y<sub>6 </sub>+ 2t<sub>y</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(30)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(9) + (10)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>4</sub> + x<sub>5</sub> + 2t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
+ 2h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></code></b></td>
					<td><b><code>(31)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(11) + (12)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							x<sub>6</sub> + x<sub>7</sub> + 2t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> 
- 2h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></code></b></td>
					<td><b><code>(32)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>Definition :</code></b></td>
					<td>
						<div align="right">
							<b><code>
							X<sub>47</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>x<sub>4</sub> + x<sub>5</sub> + x<sub>6</sub> + x<sub>7</sub></code></b></td>
					<td><b><code>(33)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(30) + (31)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							X<sub>47</sub> + 4t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>+ 4d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font></code></b></td>
					<td><b><code>(34)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>Definition :</code></b></td>
					<td>
						<div align="right">
							<b><code>
							Y<sub>47</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>y<sub>4 </sub>+ y<sub>5 </sub>+ y<sub>6 </sub>+ y<sub>7</sub></code></b></td>
					<td><b><code>(35)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(29) +c3/4</code></b></td>
					<td>
						<div align="right">
							<b><code>
							Y<sub>47 </sub>+ 4t<sub>y</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>+ 4d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></code></b></td>
					<td><b><code>(36)</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
				</tr>
			</table>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>We get t<sub>x</sub> and t<sub>y</sub> and find then all remaining parameters 
:</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">From</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<b><code><font color="#cc3333">
							-</font></code></b></center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Eq.</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">comments</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td>
						<div align="left">
							<b><code>
							(24) + (34)</code></b></div>
					</td>
					<td>
						<div align="right">
							<b><code>
							t<sub>x</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- ( X<sub>03</sub> + X<sub>47 </sub>) /&nbsp;8</code></b></td>
					<td><b><code>(36)</code></b></td>
					<td><font color="#9c3031"><b><code>==&gt; t<sub>x</sub> is found</code></b></font></td>
				</tr>
				<tr>
					<td>
						<div align="left">
							<b><code>
							(26) + (36)</code></b></div>
					</td>
					<td>
						<div align="right">
							<b><code>
							t<sub>y</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>- ( Y<sub>03 </sub>+ Y<sub>47 </sub>) /&nbsp;8</code></b></td>
					<td><b><code>(37)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; t<sub>y</sub> is found</font>-</code></b></td>
				</tr>
				<tr>
					<td><b><code>(24) and (26)</code></b></td>
					<td><b><code>( Y<sub>03 </sub>+ 4t<sub>y</sub> ) / ( X<sub>03</sub> + 4t<sub>x</sub> 
)</code></b></td>
					<td><b><code>=</code></b></td>
					<td><b><code>tg<font face="Symbol">f</font></code></b></td>
					<td><b><code>(38)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; </font><font face="Symbol" color="#9c3031">f</font><font color="#9c3031"> 
is found</font></code></b></td>
				</tr>
				<tr>
					<td><b><code>(34) and (36)</code></b></td>
					<td><b><code>( Y<sub>47 </sub>+ 4t<sub>y</sub> ) / ( X<sub>47</sub> + 4t<sub>x</sub> 
)</code></b></td>
					<td><b><code>=</code></b></td>
					<td><b><code>( Y<sub>03 </sub>+ 4t<sub>y</sub> ) / ( X<sub>03</sub> + 4t<sub>x</sub> 
)</code></b></td>
					<td><b><code>(39)</code></b></td>
					<td>
						<center>
							<font color="#9c0031"><b><code>ok with (36) &amp; (37)</code></b></font></center>
					</td>
				</tr>
				<tr>
					<td><b><code>(24)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							tg<font face="Symbol">q</font></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td>
						<div align="left">
							<b><code>
							- ( X<sub>03</sub> + 4t<sub>x</sub> ) / ( 4d<sub>z</sub> cos<font face="Symbol">f</font> 
)</code></b></div>
					</td>
					<td><b><code>(40)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; </font><font face="Symbol" color="#9c3031">q</font><font color="#9c3031"> 
is found</font></code></b></td>
				</tr>
				<tr>
					<td><b><code>(21) - (22)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							4 h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>x<sub>0</sub> + x<sub>1</sub> - x<sub>2</sub> - x<sub>3</sub></code></b></td>
					<td><b><code>(41)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; </font><font face="Symbol" color="#9c3031">a</font><font color="#9c3031"><sub>1</sub> 
is found</font></code></b></td>
				</tr>
				<tr>
					<td><b><code>(31) - (32)</code></b></td>
					<td>
						<div align="right">
							<b><code>
							4 h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></code></b></div>
					</td>
					<td><b><code>=</code></b></td>
					<td><b><code>x<sub>4</sub> + x<sub>5</sub> - x<sub>6</sub> - x<sub>7</sub></code></b></td>
					<td><b><code>(42)</code></b></td>
					<td><b><code><font color="#9c3031">==&gt; </font><font face="Symbol" color="#9c3031">a</font><font color="#9c3031"><sub>2</sub> 
is found</font></code></b></td>
				</tr>
			</table>
		</center>
		<p>We have found back all the parameters starting from coordinates satisfying conditions 
(1) to (6). It proves that all shapes satisfying conditions (1) to (6) can be represented 
by a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>.</p>
		<p></p>
		<p></p>
		<div align="center">
			<h2><font color="#cc3333">2nd Constructor</font></h2>
		</div>
		<p>Due to this fact, we are able to offer a second constructor of <b><code>TLitTrap</code></b>, 
whose arguments are the coordinates of the points of the apexes instead of the usual 
argument of a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
used in the first constructor.</p>
		<p>Notice this important point concerning the second constructor : unless the user 
has chosen his<b><code> (x<sub>k</sub>, y<sub>k</sub>)</code></b> values in such 
a way that equations (36) and (37) give <b><code>t<sub>x</sub> = t<sub>y</sub> = 
0</code></b> , i.e. <b>such that </b></p>
		<ul>
			<li><b><code>x<sub>0</sub> + x<sub>1</sub> + x<sub>2</sub> + x<sub>3</sub> + x<sub>4</sub> 
+ x<sub>5</sub> + x<sub>6</sub> + x<sub>7</sub> = 0</code></b>
			<li><b><code>y<sub>0 </sub>+ y<sub>1 </sub>+ y<sub>2 </sub>+ y<sub>3</sub> + y<sub>4 
</sub>+ y<sub>5 </sub>+ y<sub>6 </sub>+  y<sub>7</sub> = 0</code></b>
		</ul>
		<p>, the <b><code>(x<sub>k</sub>, y<sub>k</sub>)</code></b> values proposed by 
the user <b>will be shifted by t<sub>x</sub> and t<sub>y</sub></b>. <b><code>t<sub>x</sub></code></b> 
and <b><code>t<sub>y</sub></code></b> are returned by the second constructor so that 
the user is informed about this translation and can compensate for it in the call 
to <b><code><a href="http://root.cern.ch/root/html/TGeoNode.html">TGeoNode</a></code></b>.&nbsp;For 
instance, if he wants that the shape be positionned exactly at the given points, 
he has only to call <b><code><a href="http://root.cern.ch/root/html/TGeoNode.html">TGeoNode</a></code></b> 
with the translation vector <b><code>(-t<sub>x</sub>, -t<sub>y</sub>, 0.0)</code></b>.</p>
		<p>Notice also that<b><code> y<sub>1</sub>, y<sub>3</sub>, y<sub>5</sub>, y<sub>7</sub></code></b> 
 are not given because they are fixed by the conditions (3) - (6).</p>
		<center>
			<table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
				<caption>
					<h4>20 parameters of the second constructor</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Type</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Parameter</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>class name</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Notation</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Definition</code></b></font></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>dz</code></b></td>
					<td><b><code>fDdz</code></b></td>
					<td>
						<div align="left">
							<b><code>d<sub>z</sub></code></b></div>
					</td>
					<td><b><code>half-length along the z axis in cm</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x0</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td>
						<div align="left">
							<b><code>x<sub>0</sub></code></b></div>
					</td>
					<td><b><code>x coordinate of point (0) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x1</code></b></td>
					<td>
						<center>
							<b><code>-</code></b></center>
					</td>
					<td>
						<div align="left">
							<b><code>x<sub>1</sub></code></b></div>
					</td>
					<td><b><code>x coordinate of point (1) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x2</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td><b><code>x<sub>2</sub></code></b></td>
					<td><b><code>x coordinate of point (2) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x3</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td><b><code>x<sub>3</sub></code></b></td>
					<td><b><code>x coordinate of point (3) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x4</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>4</sub></code></b></td>
					<td><b><code>x coordinate of point (4) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x5</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>5</sub></code></b></td>
					<td><b><code>x coordinate of point (5) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x6</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>6</sub></code></b></td>
					<td><b><code>x coordinate of point (6) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x7</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>7</sub></code></b></td>
					<td><b><code>x coordinate of point (7) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y0</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>0</sub></code></b></td>
					<td><b><code>y coordinate of point (0) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y2</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>2</sub></code></b></td>
					<td><b><code>y coordinate of point (2) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y4</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>4</sub></code></b></td>
					<td><b><code>y coordinate of point (4) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y6</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>6</sub></code></b></td>
					<td><b><code>y coordinate of point (6) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t &amp;</code></b></td>
					<td><b><code>tx</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>t<sub>x</sub></code></b></td>
					<td><b><code>translation that will be affected to all x<sub>k</sub> values</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t &amp;</code></b></td>
					<td><b><code>ty</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>t<sub>y</sub></code></b></td>
					<td><b><code>translation that will be affected to all y<sub>k</sub> values</code></b></td>
				</tr>
			</table>
		</center>
		<p>Finally a <b>not necessary</b>, but <b>convenient</b> condition. If it disturbs 
you to have negative values for the parameters <b><code>t<sub>1</sub> , b<sub>1</sub> 
, h<sub>1 </sub>, t<sub>2</sub> , b<sub>2</sub>, h<sub>2</sub></code></b> ( on a 
mathematical point of view, it does not matter, on a geometrical point of view, it 
does ),  number your points in such a way that :</p>
		<center>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>not necessary, but convenient conditions</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Point</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Point</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(0)</code></b></center>
					</td>
					<td><b><code>x<sub>0</sub>&nbsp;&gt; 0 or x<sub>0</sub> big</code></b></td>
					<td><b><code>y<sub>0 </sub>&gt; 0 or y<sub>0</sub> big</code></b></td>
					<td>
						<center>
							<b><code>
							(4)</code></b></center>
					</td>
					<td><b><code>x<sub>4</sub>&nbsp;&gt; 0 or x<sub>4</sub> big</code></b></td>
					<td><b><code>y<sub>4 </sub>&gt; 0 or y<sub>4</sub> big</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(1)</code></b></center>
					</td>
					<td><b><code>x<sub>1</sub>&nbsp;&lt; 0 or x<sub>1</sub> small</code></b></td>
					<td><b><code>y<sub>1 </sub>&gt; 0 or y<sub>1</sub> big</code></b></td>
					<td>
						<center>
							<b><code>
							(5)</code></b></center>
					</td>
					<td><b><code>x<sub>5</sub>&nbsp;&lt; 0 or x<sub>5</sub> small</code></b></td>
					<td><b><code>y<sub>5 </sub>&gt; 0 or y<sub>5</sub> big</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(2)</code></b></center>
					</td>
					<td><b><code>x<sub>2</sub>&nbsp;&lt; 0 or x<sub>2</sub> small</code></b></td>
					<td><b><code>y<sub>2 </sub>&lt; 0 or y<sub>2</sub> small</code></b></td>
					<td>
						<center>
							<b><code>
							(6)</code></b></center>
					</td>
					<td><b><code>x<sub>6</sub>&nbsp;&lt; 0 or x<sub>6</sub> small</code></b></td>
					<td><b><code>y<sub>6 </sub>&lt; 0 or y<sub>6</sub> small</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(3)</code></b></center>
					</td>
					<td><b><code>x<sub>3</sub>&nbsp;&gt; 0 or x<sub>3</sub> big</code></b></td>
					<td><b><code>y<sub>3 </sub>&lt; 0 or y<sub>3</sub> small</code></b></td>
					<td>
						<center>
							<b><code>
							(7)</code></b></center>
					</td>
					<td><b><code>x<sub>7</sub>&nbsp;&gt; 0 or x<sub>7</sub> big</code></b></td>
					<td><b><code>y<sub>7 </sub>&lt; 0 or y<sub>7</sub> small</code></b></td>
				</tr>
			</table>
			<p></p>
			<p></p>
			<h2><font color="#cc3333">3rd Constructor</font></h2>
			<p></p>
		</center>
		<div align="left">
			<p>This constructor is for a <b><i><font color="#cc3333">special </font></i></b> 
<b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>, 
satisfying<b><i><font color="#cc3333"> in addition</font></i></b> to the following 
conditions :</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>(7)</code></b></td>
					<td>
						<p><b><code>x3 = x0</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(8)</code></b></td>
					<td>
						<p><b><code>x4 = x0</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(9)</code></b></td>
					<td>
						<p><b><code>x7 = x4</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(10)</code></b></td>
					<td>
						<p><b><code>y4 = y0</code></b></p>
					</td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>This 3rd constructor is of general use when conditions <b><code>(7) - (10)</code></b> 
are satisfied!  Use it only if your <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
satisfies to these additionnal conditions. It is also<b> <i><font color="#cc3333">assumed</font></i></b> 
by this constructor that the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is centered, ie</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td>
						<p><b><code>x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0</code></b></p>
					</td>
				</tr>
				<tr>
					<td>
						<p><b><code>y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0</code></b></p>
					</td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>Under these conditions, if we define :</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>AF = x2 - x3</code></b></td>
					<td><b><code> AR = x6 - x7</code></b></td>
				</tr>
				<tr>
					<td><b><code>BF = y3 - y0</code></b></td>
					<td><b><code>BR = y7 - y4</code></b></td>
				</tr>
				<tr>
					<td><b><code>CF = x1 - x0</code></b></td>
					<td><b><code>CR = x5 - x4</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>Then, the special <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is sufficiently described by the 6 parameters <b><code>AF, BF, CF, AR, BR, CR</code></b> 
and the half height <b><code>dz</code></b>. For examples, all crystals of the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
experiment are of this special <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
type. Notice that if you give such values for <b><code>AF, BF, CF, AR, BR, CR</code></b> 
that the faces are not plane, the values for <b><code>AR, BR, CR</code></b> effectively 
used may be changed a little bit. You will be informed of that by a warning.</p>
			<p>Then the arguments of the 3rd constructor are the following</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>dz</code></b></td>
					<td><b><code>half height </code></b></td>
				</tr>
				<tr>
					<td><b><code>AF,BF,CF,AR,BR,CR</code></b></td>
					<td><b><code>the parameters explained above</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p></p>
			<p></p>
		</div>
		<div align="center">
			<h2><font color="#cc3333">4th Constructor</font></h2>
		</div>
		<div align="left">
			<p></p>
			<p>This constructor is <b><i><font color="#cc3333">strictly reserved for describing</font></i></b> 
the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystals.&nbsp;Do not use it for anything else. There are 18 shapes of crystals in 
<b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b>.&nbsp;All 
dimensions are hard-coded in the method <b><code><a href="#TLitTrap:GetCMSdim">TLitTrap::GetCMSdim()</a></code></b>, 
so that the first argument of this constructor is the crystal type [1-18]. <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystals satisfy the supplementary conditions described above for the 3rd constructor.&nbsp;So, 
<b><code><a href="#TLitTrap:GetCMSdim">TLitTrap::GetCMSdim()</a></code></b> returns 
the parameter <b><code>AF, BF, CF, AR, BR, CR</code></b> and the half height <b><code>dz</code></b> 
and then proceed as the 3rd constructor.</p>
			<p>It is also<b> <i><font color="#cc3333">assumed</font></i></b> by this constructor 
that the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is centered, ie</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td>
						<p><b><code>x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0</code></b></p>
					</td>
				</tr>
				<tr>
					<td>
						<p><b><code>y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0</code></b></p>
					</td>
				</tr>
			</table>
			<p></p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h3>Arguments of 4th constructor</h3>
				</caption>
				<tr>
					<td><b><code>crystaltype</code></b></td>
					<td><b><code>type of CMS crystal</code></b></td>
				</tr>
				<tr>
					<td><b><code>iso_dx</code></b></td>
					<td><b><code>If iso_dx is not 0, the length iso_dx is subtracted from all crystal 
dimensions in the Ox direction. This is useful in the case one wants to represent 
the crystal as 2 (not 1) TGeoTrap, the second TGeoTrap being a small sheet of ISOTROPIC 
material in contact with face 1 of the first TGeoTrap. Face 1 of the CMS crystal 
is the one which is depolished, and SLitrani is not able to simulate depolish when 
the material of the shape is anisotropic. It is why this small sheet is useful!</code></b></td>
				</tr>
			</table>
			<p></p>
		</div>
END_HTML */
//
TLitTrap::TLitTrap(Double_t dz,Double_t theta,Double_t phi,Double_t h1,Double_t bl1,
   Double_t tl1,Double_t alpha1,Double_t h2,Double_t bl2,Double_t tl2,
   Double_t alpha2) {
// Given the standard arguments for a TGeoTrap, this constructor correct them
//in such a way that all the faces are planar! For more details, look at the
//class description.
//
  const Double_t zero    = 0.0;
  const Double_t small   = 1.0e-4;
	const Double_t pis2d   = 90.0;
	const Double_t deuxpid = 360.0;
	Double_t tx,ty,planarity;
  InitP();
  fDz     = dz;
  theta = TMath::Abs(theta);
  while (theta>pis2d) theta -= pis2d;
  fTheta  = theta;
  while (phi<zero) phi += deuxpid;
  fPhi    = phi;
  fH1     = h1;
  fBl1    = bl1;
  fTl1    = tl1;
  fAlpha1 = alpha1;
  fH2     = h2;
  fBl2    = bl2;
  fTl2    = tl2;
  fAlpha2 = alpha2;
  fGCenter.SetXYZ(zero,zero,zero);
	ToPoints();
	planarity = MakePlanar();
	ToParam(tx,ty);
  if (planarity>small) {
    gCleanOut->MM(warning,"TLitTrap","Parameters changed in order to have plane faces",ClassName());
		Print();
  }
}
TLitTrap::TLitTrap(Double_t dz,Double_t x0,Double_t x1,Double_t x2,Double_t x3,
   Double_t x4,Double_t x5,Double_t x6,Double_t x7,Double_t y0,
   Double_t y2,Double_t y4,Double_t y6,Double_t &tx,Double_t &ty) {
/* BEGIN_HTML
		<div align="center">
			<h2><font color="#cc3333">2nd Constructor</font></h2>
		</div>
		<p>Due to this fact, we are able to offer a second constructor of <b><code>TLitTrap</code></b>, 
whose arguments are the coordinates of the points of the apexes instead of the usual 
argument of a <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
used in the first constructor.</p>
		<p>Notice this important point concerning the second constructor : unless the user 
has chosen his<b><code> (x<sub>k</sub>, y<sub>k</sub>)</code></b> values in such 
a way that equations (36) and (37) give <b><code>t<sub>x</sub> = t<sub>y</sub> = 
0</code></b> , i.e. <b>such that </b></p>
		<ul>
			<li><b><code>x<sub>0</sub> + x<sub>1</sub> + x<sub>2</sub> + x<sub>3</sub> + x<sub>4</sub> 
+ x<sub>5</sub> + x<sub>6</sub> + x<sub>7</sub> = 0</code></b>
			<li><b><code>y<sub>0 </sub>+ y<sub>1 </sub>+ y<sub>2 </sub>+ y<sub>3</sub> + y<sub>4 
</sub>+ y<sub>5 </sub>+ y<sub>6 </sub>+  y<sub>7</sub> = 0</code></b>
		</ul>
		<p>, the <b><code>(x<sub>k</sub>, y<sub>k</sub>)</code></b> values proposed by 
the user <b>will be shifted by t<sub>x</sub> and t<sub>y</sub></b>. <b><code>t<sub>x</sub></code></b> 
and <b><code>t<sub>y</sub></code></b> are returned by the second constructor so that 
the user is informed about this translation and can compensate for it in the call 
to <b><code><a href="http://root.cern.ch/root/html/TGeoNode.html">TGeoNode</a></code></b>.&nbsp;For 
instance, if he wants that the shape be positionned exactly at the given points, 
he has only to call <b><code><a href="http://root.cern.ch/root/html/TGeoNode.html">TGeoNode</a></code></b> 
with the translation vector <b><code>(-t<sub>x</sub>, -t<sub>y</sub>, 0.0)</code></b>.</p>
		<p>Notice also that<b><code> y<sub>1</sub>, y<sub>3</sub>, y<sub>5</sub>, y<sub>7</sub></code></b> 
 are not given because they are fixed by the conditions (3) - (6).</p>
		<center>
			<table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
				<caption>
					<h4>20 parameters of the second constructor</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Type</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Parameter</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>class name</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Notation</code></b></font></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><font color="#cc3333"><b><code>Definition</code></b></font></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>dz</code></b></td>
					<td><b><code>fDdz</code></b></td>
					<td>
						<div align="left">
							<b><code>d<sub>z</sub></code></b></div>
					</td>
					<td><b><code>half-length along the z axis in cm</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x0</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td>
						<div align="left">
							<b><code>x<sub>0</sub></code></b></div>
					</td>
					<td><b><code>x coordinate of point (0) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x1</code></b></td>
					<td>
						<center>
							<b><code>-</code></b></center>
					</td>
					<td>
						<div align="left">
							<b><code>x<sub>1</sub></code></b></div>
					</td>
					<td><b><code>x coordinate of point (1) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x2</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td><b><code>x<sub>2</sub></code></b></td>
					<td><b><code>x coordinate of point (2) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x3</code></b></td>
					<td>
						<div align="center">
							<b><code>-</code></b></div>
					</td>
					<td><b><code>x<sub>3</sub></code></b></td>
					<td><b><code>x coordinate of point (3) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x4</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>4</sub></code></b></td>
					<td><b><code>x coordinate of point (4) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x5</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>5</sub></code></b></td>
					<td><b><code>x coordinate of point (5) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x6</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>6</sub></code></b></td>
					<td><b><code>x coordinate of point (6) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>x7</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>x<sub>7</sub></code></b></td>
					<td><b><code>x coordinate of point (7) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y0</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>0</sub></code></b></td>
					<td><b><code>y coordinate of point (0) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y2</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>2</sub></code></b></td>
					<td><b><code>y coordinate of point (2) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y4</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>4</sub></code></b></td>
					<td><b><code>y coordinate of point (4) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t</code></b></td>
					<td><b><code>y6</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>y<sub>6</sub></code></b></td>
					<td><b><code>y coordinate of point (6) wanted by the user.</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t &amp;</code></b></td>
					<td><b><code>tx</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>t<sub>x</sub></code></b></td>
					<td><b><code>translation that will be affected to all x<sub>k</sub> values</code></b></td>
				</tr>
				<tr>
					<td><b><code>Double_t &amp;</code></b></td>
					<td><b><code>ty</code></b></td>
					<td>
						<center>
							<b><code>
							-</code></b></center>
					</td>
					<td><b><code>t<sub>y</sub></code></b></td>
					<td><b><code>translation that will be affected to all y<sub>k</sub> values</code></b></td>
				</tr>
			</table>
		</center>
		<p>Finally a <b>not necessary</b>, but <b>convenient</b> condition. If it disturbs 
you to have negative values for the parameters <b><code>t<sub>1</sub> , b<sub>1</sub> 
, h<sub>1 </sub>, t<sub>2</sub> , b<sub>2</sub>, h<sub>2</sub></code></b> ( on a 
mathematical point of view, it does not matter, on a geometrical point of view, it 
does ),  number your points in such a way that :</p>
		<center>
			<p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h4>not necessary, but convenient conditions</h4>
				</caption>
				<tr>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Point</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">Point</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
					<td>
						<center>
							<h4><b><code><font color="#cc3333">condition</font></code></b></h4>
						</center>
					</td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(0)</code></b></center>
					</td>
					<td><b><code>x<sub>0</sub>&nbsp;&gt; 0 or x<sub>0</sub> big</code></b></td>
					<td><b><code>y<sub>0 </sub>&gt; 0 or y<sub>0</sub> big</code></b></td>
					<td>
						<center>
							<b><code>
							(4)</code></b></center>
					</td>
					<td><b><code>x<sub>4</sub>&nbsp;&gt; 0 or x<sub>4</sub> big</code></b></td>
					<td><b><code>y<sub>4 </sub>&gt; 0 or y<sub>4</sub> big</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(1)</code></b></center>
					</td>
					<td><b><code>x<sub>1</sub>&nbsp;&lt; 0 or x<sub>1</sub> small</code></b></td>
					<td><b><code>y<sub>1 </sub>&gt; 0 or y<sub>1</sub> big</code></b></td>
					<td>
						<center>
							<b><code>
							(5)</code></b></center>
					</td>
					<td><b><code>x<sub>5</sub>&nbsp;&lt; 0 or x<sub>5</sub> small</code></b></td>
					<td><b><code>y<sub>5 </sub>&gt; 0 or y<sub>5</sub> big</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(2)</code></b></center>
					</td>
					<td><b><code>x<sub>2</sub>&nbsp;&lt; 0 or x<sub>2</sub> small</code></b></td>
					<td><b><code>y<sub>2 </sub>&lt; 0 or y<sub>2</sub> small</code></b></td>
					<td>
						<center>
							<b><code>
							(6)</code></b></center>
					</td>
					<td><b><code>x<sub>6</sub>&nbsp;&lt; 0 or x<sub>6</sub> small</code></b></td>
					<td><b><code>y<sub>6 </sub>&lt; 0 or y<sub>6</sub> small</code></b></td>
				</tr>
				<tr>
					<td>
						<center>
							<b><code>
							(3)</code></b></center>
					</td>
					<td><b><code>x<sub>3</sub>&nbsp;&gt; 0 or x<sub>3</sub> big</code></b></td>
					<td><b><code>y<sub>3 </sub>&lt; 0 or y<sub>3</sub> small</code></b></td>
					<td>
						<center>
							<b><code>
							(7)</code></b></center>
					</td>
					<td><b><code>x<sub>7</sub>&nbsp;&gt; 0 or x<sub>7</sub> big</code></b></td>
					<td><b><code>y<sub>7 </sub>&lt; 0 or y<sub>7</sub> small</code></b></td>
				</tr>
			</table>
END_HTML */
  const Double_t zero = 0.0;
  const Double_t small  = 1.0e-4;
	Double_t planarity;
	Double_t y1,y3,y5,y7;
  InitP();
  fGCenter.SetXYZ(zero,zero,zero);
	fDz  = dz;
	y1   = y0;
	y3   = y2;
	y5   = y4;
	y7   = y6;
	fV0.SetXYZ(x0,y0,-dz);
	fV1.SetXYZ(x1,y1,-dz);
	fV2.SetXYZ(x2,y2,-dz);
	fV3.SetXYZ(x3,y3,-dz);
	fV4.SetXYZ(x4,y4,dz);
	fV5.SetXYZ(x5,y5,dz);
	fV6.SetXYZ(x6,y6,dz);
	fV7.SetXYZ(x7,y7,dz);
	planarity = MakePlanar();
  if (planarity>small) {
    gCleanOut->MM(warning,"TLitTrap","Coordinate changed in order to have plane faces",ClassName());
		PrintPts();
  }
	ToParam(tx,ty);
}
TLitTrap::TLitTrap(Double_t dz,Double_t AF,Double_t BF,Double_t CF,Double_t AR,
   Double_t BR,Double_t CR) {
/* BEGIN_HTML
		<div align="center">
			<h2><font color="#cc3333">3rd Constructor</font></h2>
		</div>
			<p></p>
		<div align="left">
			<p>This constructor is for a <b><i><font color="#cc3333">special </font></i></b> 
<b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b>, 
satisfying<b><i><font color="#cc3333"> in addition</font></i></b> to the following 
conditions :</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>(7)</code></b></td>
					<td>
						<p><b><code>x3 = x0</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(8)</code></b></td>
					<td>
						<p><b><code>x4 = x0</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(9)</code></b></td>
					<td>
						<p><b><code>x7 = x4</code></b></p>
					</td>
				</tr>
				<tr>
					<td><b><code>(10)</code></b></td>
					<td>
						<p><b><code>y4 = y0</code></b></p>
					</td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>This 3rd constructor is of general use when conditions <b><code>(7) - (10)</code></b> 
are satisfied!  Use it only if your <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
satisfies to these additionnal conditions. It is also<b> <i><font color="#cc3333">assumed</font></i></b> 
by this constructor that the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is centered, ie</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td>
						<p><b><code>x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0</code></b></p>
					</td>
				</tr>
				<tr>
					<td>
						<p><b><code>y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0</code></b></p>
					</td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>Under these conditions, if we define :</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>AF = x2 - x3</code></b></td>
					<td><b><code> AR = x6 - x7</code></b></td>
				</tr>
				<tr>
					<td><b><code>BF = y3 - y0</code></b></td>
					<td><b><code>BR = y7 - y4</code></b></td>
				</tr>
				<tr>
					<td><b><code>CF = x1 - x0</code></b></td>
					<td><b><code>CR = x5 - x4</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p>Then, the special <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is sufficiently described by the 6 parameters <b><code>AF, BF, CF, AR, BR, CR</code></b> 
and the half height <b><code>dz</code></b>. For examples, all crystals of the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
experiment are of this special <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
type. Notice that if you give such values for <b><code>AF, BF, CF, AR, BR, CR</code></b> 
that the faces are not plane, the values for <b><code>AR, BR, CR</code></b> effectively 
used may be changed a little bit. You will be informed of that by a warning.</p>
			<p>Then the arguments of the 3rd constructor are the following</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td><b><code>dz</code></b></td>
					<td><b><code>half height </code></b></td>
				</tr>
				<tr>
					<td><b><code>AF,BF,CF,AR,BR,CR</code></b></td>
					<td><b><code>the parameters explained above</code></b></td>
				</tr>
			</table>
		</div>
		<div align="left">
			<p></p>
			<p></p>
		</div>
END_HTML */
//
  const Int_t    ntimes = 100;
  const Double_t   zero = 0.0;
  const Double_t     un = 1.0;
  const Double_t small  = 1.0e-4;
  const Double_t small2 = 1.0e-8;
  Int_t itimes;
	Double_t planarity;
	Double_t x0,x1,x2,x3,x4,x5,x6,x7;
	Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  Double_t tx,ty;
  InitP();
  fGCenter.SetXYZ(zero,zero,zero);
	fDz = dz;
  itimes = 0;
  planarity = un;
  while ((itimes<ntimes) && (planarity>small2)) {
    itimes++;
    x0  = -(AF + CF + CR + AR)/8.0;
    x1  = x0 + CF;
    x3  = x0;
    x2  = x3 + AF;
    x4  = x0;
    x5  = x4 + CR;
    x7  = x4;
    x6  = x7 + AR;
    y0  = -(BF + BR)/4.0;
  	y1  = y0;
    y3  = y0 + BF;
  	y2  = y3;
  	y4  = y0;
    y5  = y4;
  	y7  = y4 + BR;
    y6  = y7;
  	fV0.SetXYZ(x0,y0,-dz);
  	fV1.SetXYZ(x1,y1,-dz);
  	fV2.SetXYZ(x2,y2,-dz);
  	fV3.SetXYZ(x3,y3,-dz);
  	fV4.SetXYZ(x4,y4,dz);
  	fV5.SetXYZ(x5,y5,dz);
  	fV6.SetXYZ(x6,y6,dz);
  	fV7.SetXYZ(x7,y7,dz);
  	planarity = MakePlanar();
    x0  = fV0(0);
    y0  = fV0(1);
    x1  = fV1(0);
    y1  = fV1(1);
    x2  = fV2(0);
    y2  = fV2(1);
    x3  = fV3(0);
    y3  = fV3(1);
    x4  = fV4(0);
    y4  = fV4(1);
    x5  = fV5(0);
    y5  = fV5(1);
    x6  = fV6(0);
    y6  = fV6(1);
    x7  = fV7(0);
    y7  = fV7(1);
    AF  = x2 - x3;
    BF  = y3 - y0;
    CF  = x1 - x0;
    AR  = x6 - x7;
    BR  = y7 - y4;
    CR  = x5 - x4;
    if (planarity>small) {
      gCleanOut->MM(warning,"TLitTrap","Parameters changed in order to have plane faces",ClassName());
      cout << "AF : " << AF << "   BF : " << BF << "   CF : " << CF << endl;
      cout << "AR : " << AR << "   BR : " << BR << "   CR : " << CR << endl;
    }
  }
  if (itimes>=ntimes) {
    gCleanOut->MM(fatal,"TLitTrap","Impossible to get plane faces",ClassName());
  }
	ToParam(tx,ty);
}
TLitTrap::TLitTrap(Short_t crystaltype,Double_t iso_dx) {
/* BEGIN_HTML
		<div align="center">
			<h2><font color="#cc3333">4th Constructor</font></h2>
		</div>
		<div align="left">
			<p></p>
			<p>This constructor is <b><i><font color="#cc3333">strictly reserved for describing</font></i></b> 
the <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystals.&nbsp;Do not use it for anything else. There are 18 shapes of crystals in 
<b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b>.&nbsp;All 
dimensions are hard-coded in the method <b><code><a href="#TLitTrap:GetCMSdim">TLitTrap::GetCMSdim()</a></code></b>, 
so that the first argument of this constructor is the crystal type [1-18]. <b><code><a href="http://cmsdoc.cern.ch/cms/outreach/html/index.shtml">CMS</a></code></b> 
crystals satisfy the supplementary conditions described above for the 3rd constructor.&nbsp;So, 
<b><code><a href="#TLitTrap:GetCMSdim">TLitTrap::GetCMSdim()</a></code></b> returns 
the parameter <b><code>AF, BF, CF, AR, BR, CR</code></b> and the half height <b><code>dz</code></b> 
and then proceed as the 3rd constructor.</p>
			<p>It is also<b> <i><font color="#cc3333">assumed</font></i></b> by this constructor 
that the <b><code><a href="http://root.cern.ch/root/html/TGeoTrap.html">TGeoTrap</a></code></b> 
is centered, ie</p>
		</div>
		<div align="center">
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<tr>
					<td>
						<p><b><code>x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0</code></b></p>
					</td>
				</tr>
				<tr>
					<td>
						<p><b><code>y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0</code></b></p>
					</td>
				</tr>
			</table>
			<p></p>
			<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
				<caption>
					<h3>Arguments of 4th constructor</h3>
				</caption>
				<tr>
					<td><b><code>crystaltype</code></b></td>
					<td><b><code>type of CMS crystal</code></b></td>
				</tr>
				<tr>
					<td><b><code>iso_dx</code></b></td>
					<td><b><code>If iso_dx is not 0, the length iso_dx is subtracted from all crystal 
dimensions in the Ox direction. This is useful in the case one wants to represent 
the crystal as 2 (not 1) TGeoTrap, the second TGeoTrap being a small sheet of ISOTROPIC 
material in contact with face 1 of the first TGeoTrap. Face 1 of the CMS crystal 
is the one which is depolished, and SLitrani is not able to simulate depolish when 
the material of the shape is anisotropic. It is why this small sheet is useful!</code></b></td>
				</tr>
			</table>
			<p></p>
		</div>
END_HTML */
//
  const Int_t    ntimes = 100;
  const Double_t   zero = 0.0;
  const Double_t     un = 1.0;
  const Double_t small  = 1.0e-3;
  const Double_t small2 = 1.0e-8;
  const Double_t     dz = 11.5;   //half length of barrel crystal
  const Double_t   dzEC = 11.0;   //half length of end-cap crystal
  Int_t itimes;
	Double_t planarity;
	Double_t x0,x1,x2,x3,x4,x5,x6,x7;
	Double_t y0,y1,y2,y3,y4,y5,y6,y7;
	Double_t z0,z1,z2,z3,z4,z5,z6,z7;
  Double_t AF,BF,CF,AR,BR,CR;
  Double_t ferule_BF;   //not used, but useful from CINT
  Double_t tx,ty;
  InitP();
  fGCenter.SetXYZ(zero,zero,zero);
	fDz = dz;
  crystaltype--;
  crystaltype = TMath::Abs(crystaltype);
  crystaltype %= 18;
  crystaltype++;
  if (crystaltype == 18) fDz = dzEC;
  GetCMSdim(crystaltype,ferule_BF,AF,BF,CF,AR,BR,CR,iso_dx);
  itimes = 0;
  planarity = un;
  while ((itimes<ntimes) && (planarity>small2)) {
    itimes++;
    x0  = -(AF + CF + CR + AR)/8.0;
    x1  = x0 + CF;
    x3  = x0;
    x2  = x3 + AF;
    x4  = x0;
    x5  = x4 + CR;
    x7  = x4;
    x6  = x7 + AR;
    y0  = -(BF + BR)/4.0;
  	y1  = y0;
    y3  = y0 + BF;
  	y2  = y3;
  	y4  = y0;
    y5  = y4;
  	y7  = y4 + BR;
    y6  = y7;
    fV0.SetXYZ(x0,y0,-fDz);
    fV1.SetXYZ(x1,y1,-fDz);
    fV2.SetXYZ(x2,y2,-fDz);
    fV3.SetXYZ(x3,y3,-fDz);
    fV4.SetXYZ(x4,y4,fDz);
    fV5.SetXYZ(x5,y5,fDz);
    fV6.SetXYZ(x6,y6,fDz);
    fV7.SetXYZ(x7,y7,fDz);
    planarity = MakePlanar();
    x0 = fV0.X();
    y0 = fV0.Y();
    z0 = fV0.Z();

    x1 = fV1.X();
    y1 = fV1.Y();
    z1 = fV1.Z();

    x2 = fV2.X();
    y2 = fV2.Y();
    z2 = fV2.Z();

    x3 = fV3.X();
    y3 = fV3.Y();
    z3 = fV3.Z();

    x4 = fV4.X();
    y4 = fV4.Y();
    z4 = fV4.Z();

    x5 = fV5.X();
    y5 = fV5.Y();
    z5 = fV5.Z();

    x6 = fV6.X();
    y6 = fV6.Y();
    z6 = fV6.Z();

    x7 = fV7.X();
    y7 = fV7.Y();
    z7 = fV7.Z();

    AF  = x2 - x3;
    BF  = y3 - y0;
    CF  = x1 - x0;
    AR  = x6 - x7;
    BR  = y7 - y4;
    CR  = x5 - x4;
    if (planarity>small) {
      gCleanOut->MM(warning,"TLitTrap","Parameters changed in order to have plane faces",ClassName());
      cout << "AF : " << AF << "   BF : " << BF << "   CF : " << CF << endl;
      cout << "AR : " << AR << "   BR : " << BR << "   CR : " << CR << endl;
    }
  }
  if (itimes>=ntimes) {
    gCleanOut->MM(fatal,"TLitTrap","Impossible to get plane faces",ClassName());
  }
	ToParam(tx,ty);
}
TLitTrap::~TLitTrap() {
//Destructor
}
void TLitTrap::AllParams(Double_t &Dz,Double_t &Theta,Double_t &Phi,
  Double_t &H1,Double_t &Bl1,Double_t &Tl1,Double_t &Alpha1,Double_t &H2,
  Double_t &Bl2,Double_t &Tl2,Double_t &Alpha2) {
  // Returns all parameter ready to define a correct TGeoTrap
  Dz     = fDz;
  Theta  = fTheta;
  Phi    = fPhi;
  H1     = fH1;
  Bl1    = fBl1;
  Tl1    = fTl1;
  Alpha1 = fAlpha1;
  H2     = fH2;
  Bl2    = fBl2;
  Tl2    = fTl2;
  Alpha2 = fAlpha2;
}
void TLitTrap::DelPoints() {
//Delete the 8 points of the apexes
  const Double_t zero = 0.0;
	fV0.SetXYZ(zero,zero,zero);
	fV1.SetXYZ(zero,zero,zero);
	fV2.SetXYZ(zero,zero,zero);
	fV3.SetXYZ(zero,zero,zero);
	fV4.SetXYZ(zero,zero,zero);
	fV5.SetXYZ(zero,zero,zero);
	fV6.SetXYZ(zero,zero,zero);
	fV7.SetXYZ(zero,zero,zero);
}
void TLitTrap::GetCMSdim(Short_t crystaltype,Double_t &ferule_BF,
  Double_t &AF,Double_t &BF,Double_t &CF,
  Double_t &AR,Double_t &BR,Double_t &CR,Double_t iso_dx) const {
//  This method is only for use with CMS crystals! It gives the 6 dimensions AF,BF,CF,
//AR,BR,CR as a function of the CMS crystal type 1-17. It also gives the BF dimension
//of the "ferule" on the small face of the crystal.
//  Crystal of type 18 is the end cap crystal.
//  If iso_dx is not 0, the length iso_dx is subtracted from all crystal dimensions
//in the Ox direction. This is usefule in the case one wants to represent the crystal
//as 2 (not 1) TGeoTrap, the second TGeoTrap being a small sheet of ISOTROPIC material
//in contact with face 1 of the first TGeoTrap. Face 1 of the CMS crystal is the one
//which is depolished, and SLitrani is not able to simulate depolish when the material
//of the shape is anisotropic. It is why this small sheet is useful!
//
  AF   = 2.183;
  switch (crystaltype) {
  case 1:
    BF  = 2.359; //side back,  small face |_ to OZ
    CF  = 2.185; //side right, small face |_ to OZ
    AR  = 2.584; //side left,  large face |_ to OZ
    BR  = 2.548; //side back,  large face |_ to OZ
    CR  = 2.586; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 2:
    BF  = 2.222; //side back,  small face |_ to OZ
    CF  = 2.187; //side right, small face |_ to OZ
    AR  = 2.581; //side left,  large face |_ to OZ
    BR  = 2.622; //side back,  large face |_ to OZ
    CR  = 2.586; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 3:
    BF  = 2.234; //side back,  small face |_ to OZ
    CF  = 2.191; //side right, small face |_ to OZ
    AR  = 2.575; //side left,  large face |_ to OZ
    BR  = 2.628; //side back,  large face |_ to OZ
    CR  = 2.584; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 4:
    BF  = 2.247; //side back,  small face |_ to OZ
    CF  = 2.194; //side right, small face |_ to OZ
    AR  = 2.567; //side left,  large face |_ to OZ
    BR  = 2.632; //side back,  large face |_ to OZ
    CR  = 2.580; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 5:
    BF  = 2.261; //side back,  small face |_ to OZ
    CF  = 2.197; //side right, small face |_ to OZ
    AR  = 2.556; //side left,  large face |_ to OZ
    BR  = 2.634; //side back,  large face |_ to OZ
    CR  = 2.572; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 6:
    BF  = 2.260; //side back,  small face |_ to OZ
    CF  = 2.200; //side right, small face |_ to OZ
    AR  = 2.543; //side left,  large face |_ to OZ
    BR  = 2.618; //side back,  large face |_ to OZ
    CR  = 2.563; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 7:
    BF  = 2.255; //side back,  small face |_ to OZ
    CF  = 2.203; //side right, small face |_ to OZ
    AR  = 2.529; //side left,  large face |_ to OZ
    BR  = 2.596; //side back,  large face |_ to OZ
    CR  = 2.552; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 8:
    BF  = 2.267; //side back,  small face |_ to OZ
    CF  = 2.205; //side right, small face |_ to OZ
    AR  = 2.514; //side left,  large face |_ to OZ
    BR  = 2.592; //side back,  large face |_ to OZ
    CR  = 2.539; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 9:
    BF  = 2.282; //side back,  small face |_ to OZ
    CF  = 2.208; //side right, small face |_ to OZ
    AR  = 2.498; //side left,  large face |_ to OZ
    BR  = 2.590; //side back,  large face |_ to OZ
    CR  = 2.526; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 10:
    BF  = 2.308; //side back,  small face |_ to OZ
    CF  = 2.210; //side right, small face |_ to OZ
    AR  = 2.482; //side left,  large face |_ to OZ
    BR  = 2.600; //side back,  large face |_ to OZ
    CR  = 2.512; //side right, large face |_ to OZ
    ferule_BF = 2.315;//ferule of type 3
    break;
  case 11:
    BF  = 2.314; //side back,  small face |_ to OZ
    CF  = 2.212; //side right, small face |_ to OZ
    AR  = 2.465; //side left,  large face |_ to OZ
    BR  = 2.589; //side back,  large face |_ to OZ
    CR  = 2.497; //side right, large face |_ to OZ
    ferule_BF = 2.315;//ferule of type 3
    break;
  case 12:
    BF  = 2.329; //side back,  small face |_ to OZ
    CF  = 2.214; //side right, small face |_ to OZ
    AR  = 2.449; //side left,  large face |_ to OZ
    BR  = 2.586; //side back,  large face |_ to OZ
    CR  = 2.483; //side right, large face |_ to OZ
    ferule_BF = 2.336;//ferule of type 4
    break;
  case 13:
    BF  = 2.347; //side back,  small face |_ to OZ
    CF  = 2.215; //side right, small face |_ to OZ
    AR  = 2.433; //side left,  large face |_ to OZ
    BR  = 2.587; //side back,  large face |_ to OZ
    CR  = 2.468; //side right, large face |_ to OZ
    ferule_BF = 2.336;//ferule of type 4
    break;
  case 14:
    BF  = 2.371; //side back,  small face |_ to OZ
    CF  = 2.217; //side right, small face |_ to OZ
    AR  = 2.417; //side left,  large face |_ to OZ
    BR  = 2.595; //side back,  large face |_ to OZ
    CR  = 2.454; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 15:
    BF  = 2.388; //side back,  small face |_ to OZ
    CF  = 2.218; //side right, small face |_ to OZ
    AR  = 2.402; //side left,  large face |_ to OZ
    BR  = 2.596; //side back,  large face |_ to OZ
    CR  = 2.440; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 16:
    BF  = 2.406; //side back,  small face |_ to OZ
    CF  = 2.220; //side right, small face |_ to OZ
    AR  = 2.388; //side left,  large face |_ to OZ
    BR  = 2.599; //side back,  large face |_ to OZ
    CR  = 2.427; //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  case 17:
    BF  = 2.429; //side back,  small face |_ to OZ
    CF  = 2.221; //side right, small face |_ to OZ
    AR  = 2.374; //side left,  large face |_ to OZ
    BR  = 2.607; //side back,  large face |_ to OZ
    CR  = 2.415; //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  case 18:
//End cap crystal
    AF  = 2.862;
    BF  = 2.862; //side back,  small face |_ to OZ
    CF  = 2.862; //side right, small face |_ to OZ
    AR  = 3.0;   //side left,  large face |_ to OZ
    BR  = 3.0;   //side back,  large face |_ to OZ
    CR  = 3.0;   //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  default:
    gCleanOut->MM(fatal,"GetCMSdim","Type of crystal must lie between 1 and 18",ClassName());
    break;
  }
  if (ferule_BF>BF) ferule_BF = BF;
  iso_dx = TMath::Abs(iso_dx);
  AF -= iso_dx;
  CF -= iso_dx;
  AR -= iso_dx;
  CR -= iso_dx;
}
TGeoTrap* TLitTrap::GeoTrap() const {
  // Create the TGeoTrap when all parameters are ok !
  TGeoTrap *geotrap;
  geotrap = new TGeoTrap(fDz,fTheta,fPhi,fH1,fBl1,fTl1,fAlpha1,fH2,fBl2,fTl2,fAlpha2);
  return geotrap;
}
TVector3* TLitTrap::GetfV(Int_t i) {
//return the coordinates of the selected point
	TVector3 *v;
	switch (i) {
		case 0:
			v = &fV0;
			break;
		case 1:
			v = &fV1;
			break;
		case 2:
			v = &fV2;
			break;
		case 3:
			v = &fV3;
			break;
		case 4:
			v = &fV4;
			break;
		case 5:
			v = &fV5;
			break;
		case 6:
			v = &fV6;
			break;
		case 7:
			v = &fV7;
			break;
		default:
			v = &fV0;
			break;
	}
	return v;
}
void TLitTrap::GetPoints(Double_t &x0,Double_t &y0,Double_t &z0,
                       Double_t &x1,Double_t &y1,Double_t &z1,
                       Double_t &x2,Double_t &y2,Double_t &z2,
                       Double_t &x3,Double_t &y3,Double_t &z3,
                       Double_t &x4,Double_t &y4,Double_t &z4,
                       Double_t &x5,Double_t &y5,Double_t &z5,
                       Double_t &x6,Double_t &y6,Double_t &z6,
                       Double_t &x7,Double_t &y7,Double_t &z7) const {
// Get all coordinates of Points
    x0 = fV0.X();
    y0 = fV0.Y();
    z0 = fV0.Z();

    x1 = fV1.X();
    y1 = fV1.Y();
    z1 = fV1.Z();

    x2 = fV2.X();
    y2 = fV2.Y();
    z2 = fV2.Z();

    x3 = fV3.X();
    y3 = fV3.Y();
    z3 = fV3.Z();

    x4 = fV4.X();
    y4 = fV4.Y();
    z4 = fV4.Z();

    x5 = fV5.X();
    y5 = fV5.Y();
    z5 = fV5.Z();

    x6 = fV6.X();
    y6 = fV6.Y();
    z6 = fV6.Z();

    x7 = fV7.X();
    y7 = fV7.Y();
    z7 = fV7.Z();
}
void TLitTrap::InitP() {
// Pointers to 0
  const Double_t zero = 0.0;
	fV0.SetXYZ(zero,zero,zero);
	fV1.SetXYZ(zero,zero,zero);
	fV2.SetXYZ(zero,zero,zero);
	fV3.SetXYZ(zero,zero,zero);
	fV4.SetXYZ(zero,zero,zero);
	fV5.SetXYZ(zero,zero,zero);
	fV6.SetXYZ(zero,zero,zero);
	fV7.SetXYZ(zero,zero,zero);
}
Double_t TLitTrap::MakePlanar() {
//  Verifies that the points in fV0..fV7 give plane faces. The return value
//is a measure of how non-planar were the faces, before that MakePlanar()
//moves them in order to have plane faces.
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-12;
	Int_t n;
  Double_t tt1,tt2,a,b;
  Double_t ddkeep = zero;
  Double_t l;
  Double_t epsx, epsy;
  Double_t dd  = 0.0;
  Double_t dtt = 100.0;
  TVector3 v(zero,zero,zero);
  TVector3 n0(zero,zero,zero);
  TVector3 n1(zero,zero,zero);
  TVector3 n2(zero,zero,zero);
  TVector3 n3(zero,zero,zero);
  n = 0;
  while ((dtt>vsmall) && (n<20)) {
    dtt = zero;
// FACE 0  optimizes fV5
    n0   = (fV1 - fV0)^(fV4 - fV0);
    v    = fV5 - fV0;
    tt1  = n0*v;
    a    = n0(0);  b = n0(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    fV5(0) = fV5(0) + epsx;
    fV5(1) = fV5(1) + epsy;
// FACE 1  optimizes fV6
    n1   = (fV2 - fV1)^(fV5 - fV1);
    v    = fV6 - fV1;
    tt1  = n1*v;
    a    = n1(0);             b = n1(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    fV6(0) = fV6(0) + epsx;
    fV6(1) = fV6(1) + epsy;
// FACE 2  optimizes fV7
    n2   = (fV3 - fV2)^(fV6 - fV2);
    v    = fV7 - fV2;
    tt1  = n2*v;
    a    = n2(0);             b = n2(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    fV7(0) = fV7(0) + epsx;
    fV7(1) = fV7(1) + epsy;
// FACE 3  optimizes fV4
    n3   = (fV0 - fV3)^(fV7 - fV3);
    v    = fV4 - fV3;
    tt1  = n3*v;
    a    = n3(0);             b = n3(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    fV4(0) = fV4(0) + epsx;
    fV4(1) = fV4(1) + epsy;
    if (!n) ddkeep = dtt;
    n   += 1;
  }
  if (dtt>vsmall) {
    gCleanOut->MM(fatal,"MakePlanar","Cannot get faces to be plane",ClassName());
  }
	return ddkeep;
}
void TLitTrap::Print() const {
// Prints the parameters defining the TLitTrap
  cout << endl;
  cout << "Values of parameters" << endl;
  cout << endl;
  cout << "fDz    : ";
  cout.width(18);
  cout.precision(10);
  cout << fDz     << endl;
  cout << "fTheta : ";
  cout.width(18);
  cout.precision(10);
  cout << fTheta  << endl;
  cout << "fPhi   : ";
  cout.width(18);
  cout.precision(10);
  cout << fPhi    << endl;
  cout << "fH1    : ";
  cout.width(18);
  cout.precision(10);
  cout << fH1     << endl;
  cout << "fBl1   : ";
  cout.width(18);
  cout.precision(10);
  cout << fBl1    << endl;
  cout << "fTl1   : ";
  cout.width(18);
  cout.precision(10);
  cout << fTl1    << endl;
  cout << "fAlpha1: ";
  cout.width(18);
  cout.precision(10);
  cout << fAlpha1 << endl;
  cout << "fH2    : ";
  cout.width(18);
  cout.precision(10);
  cout << fH2     << endl;
  cout << "fBl2   : ";
  cout.width(18);
  cout.precision(10);
  cout << fBl2    << endl;
  cout << "fTl2   : ";
  cout.width(18);
  cout.precision(10);
  cout << fTl2    << endl;
  cout << "fAlpha2: ";
  cout.width(18);
  cout.precision(10);
  cout << fAlpha2 << endl;
}
void TLitTrap::PrintPts() const {
// Prints the coordinates of the points of the TGeoTrap
  cout << endl;
  cout << "Coordinate of the points of the TLitTrap" << endl;
  cout << endl;
  cout << "fV0 : "; fV0.Print();
  cout << "fV1 : "; fV1.Print();
  cout << "fV2 : "; fV2.Print();
  cout << "fV3 : "; fV3.Print();
  cout << "fV4 : "; fV4.Print();
  cout << "fV5 : "; fV5.Print();
  cout << "fV6 : "; fV6.Print();
  cout << "fV7 : "; fV7.Print();
}
void TLitTrap::ToParam(Double_t &tx, Double_t &ty) {
//Given that the 8 vectors fV0-7 contain the coordinates of the 8 apexes,
//satisfying to the conditions :
//
//  (1)  z0 = z1 = z2 = z3 = -dz
//  (2)  z4 = z5 = z6 = z7 = +dz
//  (3)  y1 = y0
//  (4)  y3 = y2
//  (5)  y5 = y4
//  (6)  y7 = y6
//
// then ToParam retrieves the value of the basic parameters of the TLitTrap :
//
//    fDz, fTheta, fPhi, 
//    fH1, fBl1, fTl1, fAlpha1,
//    fH2, fBl2, fTl2, fAlpha2.
//
// It is only possible if one allows for a translation
//    tx  along the Ox axis
//    ty  along the Oy axis.
// ToParam returns the needed values for tx and ty.
// For more details, see the class description.
//
	const Double_t q0     = 0.0;
	const Double_t q1     = 1.0;
	const Double_t q2     = 2.0;
	const Double_t q4     = 4.0;
	const Double_t q8     = 8.0;
	const Double_t pideg  = 180.0;
	const Double_t lim    = 1.0e-6;
	const Double_t vsmall = 1.0e-12;
	Double_t pi,deuxpi,facdeg;
	Double_t eps;
  Double_t phi = 0.0;
	Double_t alpha1,alpha2,theta,sphi,cphi,tht,ta1,ta2;
	Double_t x0,x1,x2,x3,x4,x5,x6,x7;
	Double_t y0,y1,y2,y3,y4,y5,y6,y7;
	Double_t z0,z1,z2,z3,z4,z5,z6,z7;
	Double_t X03,X47,Y03,Y47;
	pi = TMath::Pi();
	deuxpi = q2*pi;
	facdeg = pideg/pi;
  x0 = fV0(0);
  x1 = fV1(0);
  x2 = fV2(0);
  x3 = fV3(0);
  x4 = fV4(0);
  x5 = fV5(0);
  x6 = fV6(0);
  x7 = fV7(0);
  y0 = fV0(1);
  y1 = fV1(1);
  y2 = fV2(1);
  y3 = fV3(1);
  y4 = fV4(1);
  y5 = fV5(1);
  y6 = fV6(1);
  y7 = fV7(1);
  z0 = fV0(2);
  z1 = fV1(2);
  z2 = fV2(2);
  z3 = fV3(2);
  z4 = fV4(2);
  z5 = fV5(2);
  z6 = fV6(2);
  z7 = fV7(2);
//Condition (1)
	fDz = z4;
	eps  = TMath::Abs(z0+fDz) + TMath::Abs(z1+fDz) +
		     TMath::Abs(z2+fDz) + TMath::Abs(z3+fDz);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (1) not ok",ClassName());
	z0        = -fDz;
	z1        = -fDz;
	z2        = -fDz;
	z3        = -fDz;
  fV0(2) = -fDz;
  fV1(2) = -fDz;
  fV2(2) = -fDz;
  fV3(2) = -fDz;
//Condition (2)
	eps = TMath::Abs(z5-fDz) + TMath::Abs(z6-fDz) + TMath::Abs(z7-fDz);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (2) not ok",ClassName());
	z5        = fDz;
	z6        = fDz;
	z7        = fDz;
  fV5(2) = fDz;
  fV6(2) = fDz;
  fV7(2) = fDz;
//Condition (3)
	eps = TMath::Abs(y1-y0);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (3) not ok",ClassName());
	y1        = y0;
  fV1(1) = y0;
//Condition (4)
	eps = TMath::Abs(y3-y2);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (4) not ok",ClassName());
	y3        = y2;
  fV3(1) = y2;
//Condition (5)
	eps = TMath::Abs(y5-y4);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (5) not ok",ClassName());
	y5        = y4;
  fV5(1) = y4;
//Condition (6)
	eps = TMath::Abs(y7-y6);
  if (eps>lim) gCleanOut->MM(fatal,"ToParam","Condition (6) not ok",ClassName());
	y7        = y6;
  fV7(1) = y6;
//
//  Retrieving parameters
//
//	fH1  = (y0 - y2)/q2;
//	fTl1 = (x0 - x1)/q2;
//	fBl1 = (x3 - x2)/q2;
//	fH2  = (y4 - y6)/q2;
//	fTl2 = (x4 - x5)/q2;
//	fBl2 = (x7 - x6)/q2;
  fH1  = TMath::Abs((y0 - y2)/q2);
	fTl1 = TMath::Abs((x0 - x1)/q2);
	fBl1 = TMath::Abs((x3 - x2)/q2);
	fH2  = TMath::Abs((y4 - y6)/q2);
	fTl2 = TMath::Abs((x4 - x5)/q2);
	fBl2 = TMath::Abs((x7 - x6)/q2);
	X03   = x0 + x1 + x2 + x3;
	X47   = x4 + x5 + x6 + x7;
	Y03   = y0 + y1 + y2 + y3;
	Y47   = y4 + y5 + y6 + y7;
	tx    = -(X03 + X47)/q8;
	ty    = -(Y03 + Y47)/q8;
	sphi  = (Y47 + q4*ty)/(q4*fDz);
	cphi  = (X47 + q4*tx)/(q4*fDz);
	tht   = TMath::Sqrt(sphi*sphi + cphi*cphi);
	if (tht>vsmall) {
  	sphi  = sphi/tht;
	  cphi  = cphi/tht;
	  phi   = TMath::ATan2(sphi,cphi);
	  if (sphi<q0) phi += deuxpi;
	}
	else {
		tht  = q0;
		sphi = q0;
		cphi = q1;
	}
	theta = TMath::ATan(tht);
	ta1   = (x0 + x1 - x2 - x3)/(q4*fH1);
	ta2   = (x4 + x5 - x6 - x7)/(q4*fH2);
	alpha1 = TMath::ATan(ta1);
	alpha2 = TMath::ATan(ta2);
//values in degrees
	fAlpha1 = facdeg*alpha1;
	fAlpha2 = facdeg*alpha2;
	fTheta  = facdeg*theta;
	fPhi    = facdeg*phi;
  fV0(0) += tx;
  fV1(0) += tx;
  fV2(0) += tx;
  fV3(0) += tx;
  fV4(0) += tx;
  fV5(0) += tx;
  fV6(0) += tx;
  fV7(0) += tx;
  fV0(1) += ty;
  fV1(1) += ty;
  fV2(1) += ty;
  fV3(1) += ty;
  fV4(1) += ty;
  fV5(1) += ty;
  fV6(1) += ty;
  fV7(1) += ty;
}
void TLitTrap::ToPoints() {
//Calculates the coordinates of the 8 apexes of the TLitTrap.
	const Double_t pideg  = 180.0;
	Double_t pi;
	Double_t alpha1,alpha2,theta,phi;
	Double_t x,y,z,tth,tx,ty,sphi,cphi,tth1,tth2;
	pi = TMath::Pi();
	Double_t facrad;
	facrad = pi/pideg;
//
	DelPoints();
	alpha1 = facrad*fAlpha1;
	alpha2 = facrad*fAlpha2;
	theta  = facrad*fTheta;
	phi    = facrad*fPhi;
	tth1   = TMath::Tan(alpha1);
	tth2   = TMath::Tan(alpha2);
  tth    = TMath::Tan(theta);
	sphi   = TMath::Sin(phi);
	cphi   = TMath::Cos(phi);
	tx     = tth*cphi;
	ty     = tth*sphi;
// (0)
//	x   = -fDz*tx + fH1*tth1 + fTl1;
	x   = -fDz*tx + fH1*tth1 - fTl1;
//	y   =  fH1 - fDz*ty;
	y   =  -fH1 - fDz*ty;
	z   = -fDz;
  fV0.SetXYZ(x,y,z);
// (1)
//	x   = -fDz*tx + fH1*tth1 - fTl1;
	x   = -fDz*tx + fH1*tth1 + fTl1;
	fV1.SetXYZ(x,y,z);
// (2)
//	x   = -fDz*tx - fH1*tth1 - fBl1;
	x   = -fDz*tx - fH1*tth1 + fBl1;
//	y   = -fH1 - fDz*ty;
	y   = fH1 - fDz*ty;
	fV2.SetXYZ(x,y,z);
// (3)
//	x   = -fDz*tx - fH1*tth1 + fBl1;
	x   = -fDz*tx - fH1*tth1 - fBl1;
//	y   = -fH1 - fDz*ty;
	y   = fH1 - fDz*ty;
	fV3.SetXYZ(x,y,z);
// (4)
//	x   =  fDz*tx + fH2*tth2 + fTl2;
	x   =  fDz*tx + fH2*tth2 - fTl2;
//	y   =  fH2 + fDz*ty;
	y   =  -fH2 + fDz*ty;
	z   =  fDz;
	fV4.SetXYZ(x,y,z);
// (5)
//	x   =  fDz*tx + fH2*tth2 - fTl2;
	x   =  fDz*tx + fH2*tth2 + fTl2;
	fV5.SetXYZ(x,y,z);
// (6)
//	x   =  fDz*tx - fH2*tth2 - fBl2;
	x   =  fDz*tx - fH2*tth2 + fBl2;
//	y   = -fH2 + fDz*ty;
	y   = fH2 + fDz*ty;
	fV6.SetXYZ(x,y,z);
// (7)
//	x   =  fDz*tx - fH2*tth2 + fBl2;
	x   =  fDz*tx - fH2*tth2 - fBl2;
	fV7.SetXYZ(x,y,z);
}
