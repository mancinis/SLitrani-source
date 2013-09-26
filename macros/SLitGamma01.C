{
  char *name    = "SLitGamma01";
  char *listing = "SLitGamma01";
  char *upcom   = "Gammas of 1Mev, crystal and APD";
  char *downcom = "Simple example with gammas issued from an ellipse";
  gROOT->ProcessLine(".x InitSLitrani.C(5,name,listing,upcom,downcom,kFALSE,kTRUE,kFALSE)");
  //
  // arguments of InitSLitrani:
  //
  // size     size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
  // name     name of the produced ROOT file (without ".root")
  // listing  name of the listing file (without ".lis")
  // upcom    name of the comment appearing on top    left of canvas
  // downcom  name of the comment appearing on bottom left of canvas
  // otherseq true if one wants an other sequence of random numbers at each new launching
  // indebug  true if one wants to run Litrani in debug mode
  // WithDate if true, appends date and time to name of root file and name of listing
  //
  //  Simulation of an isotropic CaWO4 crystal having the form of a TGeoBBox, on top of which
  // is an APD without thin slice having the form of a smaller TGeoBBox. Between the APD and the
  // crystal, there is a slice of glue, having the shape of a TGeoBBox.
  //  The whole is surrounded by an aluminium coating. This aluminium wrapping has in consequence
  // a shape which is the union of 2 TGeoBBox, slightly bigger than the sum of the crystal and the
  // APD. The use of union for defining the aluminium wrapping is necessary in order that no overlap
  // occurs and that all 6 faces of the crystal be covered with aluminium, including the part of the
  // upper face not covered by the APD. This is by far simpler than painfully defining thin slices
  // of aluminium for covering each face of the crystal. A small complication arises because we do
  // not want that the wrapping of the APD be made of aluminium, but we require that it be a totally
  // absorbing wrapping, named "totabsorbing". For that, we define a TGeoBBox made of "totabsorbing",
  // slightly bigger than the APD and containing it and included inside the union shape of aluminium.
  // This is a not too complicate way of describing an equipped crystal, using the possibility offered
  // by the TGeo geometry of placing shapes inside other shapes, and using union in order to avoid
  // extrusion.
  //  Everything is contained in the TGeoBBox "TOP", defining the WCS, world coordinate system. Notice
  // that the second argument of AddNode(), the copy nb, is always 1 in this example, since it never
  // happens in this example that the same volume is inserted twice inside some mother volume.
  //  To describe the beam of gammas, which is intended to be issued from an ellipse, we place inside
  // TOP a TGeoEltu, an elliptical tube, named "BEAMCRADLE" at the left of the crystal. We rotate it
  // by 90° around the Ox axis, so that the elliptical face of the TGeoEltu points towards the crystal.
  // We displace the TGeoEltu from run to run along the Oz direction, to study the dependence of the
  // efficiency versus the z position of the ellipse emitting gammas.
  //  Notice that the TGeoEltu "BEAMCRADLE" is displaced AFTER the geometry has been closed by a
  // call to gGeoManager->CloseGeometry()! To understand how it is possible [calling
  // TGeoVolume::RemoveNode() and TGeoVolume::AddNode() after having closed the geometry is
  // forbidden!], read § "Representing Misalignments of the Ideal Geometry", p340 of the ROOT
  // user manual. Method TLitGamma::MoveCradle() do the trick.
  //
  const char *comTL = "Beam cradle is a TGeoEltu";
  const Color_t  matVacuumColor =  0;
  const Color_t  CaWO4Color     =  9;
  const Color_t  AluColor       =  3;
  const Color_t  GlueColor      = 49;
  const Color_t  TotAbsColor    =  1;
  const Color_t  SiliciumColor  =  2;
  const Color_t  CradleColor    =  5;
  const Double_t IrrA           = 0.0; //Value used for A   when irrelevant !
  const Double_t IrrZ           = 0.0; //Value used for Z   when irrelevant !
  const Double_t IrrRho         = 0.0; //Value used for Rho when irrelevant !
  Int_t mediumindex = 1;
  gCleanOut->fMaxInfo = 1000;
  Bool_t  ok = kFALSE;
  TLitPhys::Get()->SetLate(2000000);
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Building the geometry
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  TGeoManager *geom = new TGeoManager("setup","SLitGamma01 of new Litrani");
  //____________________________________________________________________________
  //
  // Materials and media
  //____________________________________________________________________________
  //
  // (1) Air for "TOP"
  //
  //  We define here air as a TLitMedium. Compton effect and photo-electric
  // effect do occur!
  //
  const Double_t Air_RefrIndex     = 1.0003;   //Refractive index of air
  TGeoElementTable *table = gGeoManager->GetElementTable();
  TGeoElement *N  = table->FindElement("NITROGEN");
  TGeoElement *O  = table->FindElement("OXYGEN");
  TGeoMixture *air_mix = new TGeoMixture("Air",2,0.00129);
  air_mix->AddElement(N,0.7);
  air_mix->AddElement(O,0.3);
  TLitMedium *air = new TLitMedium("Air",mediumindex++,air_mix,kFALSE,1.0,10000.0);
  air->IsIsotropic(Air_RefrIndex);
  gROOT->ProcessLine(".L PhotoEl_Air.C"); //Found in directory FitMacros, not in SplineFitDB.rdb
  TSplineFit *fitPEAir = PhotoEl_Air();
  air->SetXSectnPE(fitPEAir);
  //
  // (2) Full pedantic definition of CaWO4!
  //
  const Double_t nPhotMeV = 10000;
  TLitSpectrum *sp;
  TGeoElement *Ca = table->FindElement("CALCIUM");
  TGeoElement *W  = table->FindElement("TUNGSTEN");
  TGeoMixture *cawo4_mix = new TGeoMixture("CaWO4",3,6.06);
  cawo4_mix->AddElement(Ca,1);
  cawo4_mix->AddElement(W,1);
  cawo4_mix->AddElement(O,4);
  TLitMedium *cawo4 = new TLitMedium("CaWO4",mediumindex++,cawo4_mix,kFALSE,1.0,100.0);
  /*
  cout << "Z/A Ca : " << Ca->Z() << "    " << Ca->A() << endl;
  cout << "Z/A W  : " <<  W->Z() << "    " <<  W->A() << endl;
  cout << "Z/A O  : " <<  O->Z() << "    " <<  O->A() << endl;
  cout << "Z tot  : " << TLitPhys::Get()->GetZ(cawo4) << endl;
  cout << "A tot  : " << TLitPhys::Get()->GetA(cawo4) << endl;
  */
  cawo4->IsIsotropic(2.0);
  sp = new TLitSpectrum("Spectrum_CaWO4","Emission Spectrum | CaWO4");
  sp->AddOneComponent(447.0,25.0,1.0,2000.0,-1.0,0.01,320000.0,-1.0,0.99);
  sp->Normalize();
  cawo4->SetLitSpectrum(sp);
  cawo4->SetPhotMev(nPhotMeV);
  cawo4->SetXSectnPE("PhotoEl_CaWO4"); //Found in SplineFitDB.rdb
  //
  // (3) Full pedantic definition of Silicium
  //
  TGeoElement *Si = table->FindElement("SILICON");
  TGeoMaterial *silicium_mat = new TGeoMaterial("Silicium",Si,2.33);
  TLitMedium *silicium = new TLitMedium("Silicium",mediumindex++,silicium_mat,kTRUE,1.0,
   "AbsorptionLength_Silicium");
  silicium->IsIsotropic("RefrIndex_Silicium");
  //
  //  (4) Definition of medium "glue"
  //
  TGeoMaterial *glue_mat = new TGeoMaterial("Glue",IrrA,IrrZ,IrrRho);
  TLitMedium *glue = new TLitMedium("Glue",mediumindex++,glue_mat,kFALSE,1.0,
    "AbsorptionLength_Meltmount1704");
  glue->IsIsotropic("RefrIndex_RTV3145");
  //
  //  (5) Definition of meium "Strontium" for the radioactive source. We define it as a
  //      TLitMedium, in order that Compton effect and Photo-electric effect be available
  //
  const Double_t rho_strontium   = 2.64;
  const Double_t absl_strontium  = 0.5; //Irrelevant. Chosen at random!
  const Double_t index_strontium = 2.5; //Irrelevant. Chosen at random!
  TGeoMaterial *sr_mat = new TGeoMaterial("Strontium",table->FindElement("STRONTIUM"),rho_strontium);
  TLitMedium *sr = new TLitMedium("Strontium",mediumindex++,sr_mat,kFALSE,1.0,absl_strontium);
  sr->IsIsotropic(index_strontium); //To avoid a warning in TLitMedium::NewWaveLength()
  gROOT->ProcessLine(".L PhotoEl_Strontium.C"); //Found in directory FitMacros, not in SplineFitDB.rdb
  TSplineFit *fitPESr = PhotoEl_Strontium();
  sr->SetXSectnPE(fitPESr);
  //____________________________________________________________________________
  //
  // Wrappings
  //____________________________________________________________________________
  //
  //
  //  (6) Definition of "TotAbsorbing": a totally absorbing wrapping
  //
  TGeoMaterial *totabs_mat = new TGeoMaterial("TotAbsorbing",IrrA,IrrZ,IrrRho);
  TLitMedium *totabsorbing = new TLitMedium("TotAbsorbing",mediumindex++,totabs_mat,-1.0);
  totabsorbing->SetAsWrapping(0.0,0.0,1.0,1.0,1.0);
  totabsorbing->IsIsotropic(1.0);
  //
  //  (7) Definition of wrapping medium aluminium
  //
  //  Notice that we get the photo-electric cross-section not looking inside the database file
  // SplineFitDB.rdb, but loading the fit macro "PhotoEl_Aluminium.C". This is because this
  // fit is not [yet] inserted inside the SplineFitDB.rdb.
  //  Notice also this interesting feature that Compton effect and photo-electric effect will
  // be generated inside a wrapping, which was not possible in the old Litrani!
  const Double_t aluminium_diffus  = 0.01;
  const Double_t AluminiumSupplAbs = 0.1;
  TGeoElement *Al = table->FindElement("ALUMINIUM");
  TGeoMaterial *aluminium_mat = new TGeoMaterial("Aluminium",Al,2.6995);
  TLitMedium *aluminium = new TLitMedium("Aluminium",mediumindex++,aluminium_mat,1.0);
  aluminium->SetAsWrapping(aluminium_diffus,"RIndexRev_Aluminium","IIndexRev_Aluminium",1.0,AluminiumSupplAbs);
  aluminium->IsIsotropic(Air_RefrIndex);
  gROOT->ProcessLine(".L PhotoEl_Aluminium.C");
  TSplineFit *fitPEAlu = PhotoEl_Aluminium();
  aluminium->SetXSectnPE(fitPEAlu);
  //____________________________________________________________________________
  //
  // Dimensions
  //____________________________________________________________________________
  //
  //
  // Dimensions of crystal
  //
  const Double_t crys_dx = 2.0;
  const Double_t crys_dy = 2.0;
  const Double_t crys_dz = 8.0;
  //
  // Dimensions of beam cradle of type TGeoEltu inside TOP
  //
  const Double_t crad_dx = 0.2;
  const Double_t crad_dy = 0.1;
  const Double_t crad_dz = 0.2;
  //
  // Start position of beam cradle and step size
  //
  const Double_t frsp   = 1.0; //frsp == free space
  const Double_t stepz  = 1.0;
  const Double_t starty = -4.0;
  Double_t startz = -crys_dz + frsp;
  Double_t arun  = 2*(crys_dz-frsp)/stepz + 1.0;
  Int_t nrun = (Int_t)arun;
  //
  // Dimensions of APD
  //
  const Double_t apd_dx = 0.8;
  const Double_t apd_dy = 0.8;
  const Double_t apd_dz = 1.0;
  //
  // Dimensions for glue of APD
  //
  const Double_t glueapd_dx = apd_dx;
  const Double_t glueapd_dy = apd_dy;
  const Double_t glueapd_dz = 0.05;
  //
  // Width of wrapping is wid
  //
  const Double_t wid = 0.1;
  Double_t       ws2 = 0.5*wid;
  //
  // Dimensions of TGEOBBox of alu around crystal "CRYSTALWS"
  //
  Double_t aluc_dx = crys_dx + wid;
  Double_t aluc_dy = crys_dy + wid;
  Double_t aluc_dz = crys_dz + wid;
  //
  // Dimensions of TGEOBBox of alu around APD and glue of APD [partially] "APDWS"
  //
  Double_t alua_dx = apd_dx + wid;
  Double_t alua_dy = apd_dy + wid;
  Double_t alua_dz = apd_dz + glueapd_dz;
  //
  // Dimensions of TotAbsorbing wrapping around APD and glue of APD
  //
  Double_t tot_dx = apd_dx + wid;
  Double_t tot_dy = apd_dy + wid;
  Double_t tot_dz = apd_dz + glueapd_dz + ws2;
  //
  // Dimensions of TOP
  //
  const Double_t top_dx    = 6.0;
  const Double_t top_dy    = 6.0;
  Double_t top_dz = wid + crys_dz + glueapd_dz + apd_dz + 1.0;
  //____________________________________________________________________________
  //
  // Positionning (translations)
  //____________________________________________________________________________
  //
  //  t1 is for positionning the TGeoBBox of aluminium around the TGeoBBox of "totabsorbing" [itself
  // around TGeoBBox APD and TGeoBBox of glue of APD] with respect to the TGeoBBox of aluminium around
  // crystal [or equivalently, with respect to the composite shape "rev_shape"].
  //  t1 is used for the creation of composite shape, so it has to be named and registered by
  // a call to RegisterYourself().
  //
  const Double_t t_alua_crys_z = aluc_dz + alua_dz;
  TGeoTranslation *t1 = new TGeoTranslation("t1",0.0,0.0,t_alua_crys_z);
  t1->RegisterYourself();
  //  t3 is for positionning the TGeoBBox of "totabsorbing" around APD and around glue of APD
  // with respect to the TGeoBBox of aluminium around crystal [or equivalently, with respect
  // to the composite shape "rev_shape"].
  //
  const Double_t t_tot_crys_z = crys_dz + alua_dz + ws2;
  TGeoTranslation *t3 = new TGeoTranslation("t3",0.0,0.0,t_tot_crys_z);
  //  t4 is for positionning the TGeoBBox of APD inside the TGeoBBox of "totabsorbing"
  //  around APD and glue of APD "TOT"
  //
  Double_t t_apd_tot_z = glueapd_dz - ws2;
  TGeoTranslation *t4 = new TGeoTranslation("t4",0.0,0.0,t_apd_tot_z);
  //  t5 is for positionning TGeoBBox of glue of the APD inside the TGeoBBox of "totabsorbing"
  //  around APD and glue of APD "TOT"
  //
  const Double_t t_glueapd_tot_z = -(apd_dz + ws2);
  TGeoTranslation *t5 = new TGeoTranslation("t5",0.0,0.0,t_glueapd_tot_z);
  //  ph6 is for the start position of the beam cradle inside TOP
  Double_t phi,theta,psi;
  TGeoRotation r6;
  TEuler euler(90.0,0.0,180.0,90.0);
  euler.GetEulerAngles(phi,theta,psi);
  r6.SetAngles(phi,theta,psi);
  TGeoTranslation t6(0.0,starty,startz);
  TGeoCombiTrans *c6 = new TGeoCombiTrans(t6,r6);
  TGeoHMatrix *ph6 = new TGeoHMatrix(*c6);
  //____________________________________________________________________________
  //
  // Volumes and nodes
  //____________________________________________________________________________
  //
  //
  // top box containing everything and defining the WCS, world coordinate system
  //
  TGeoVolume *top = geom->MakeBox("TOP",air,top_dx,top_dy,top_dz);
  geom->SetTopVolume(top);
  //
  // Composite shape aluminium
  //
  TGeoBBox *revcrys_shape = new TGeoBBox("CRYSTALWS",aluc_dx,aluc_dy,aluc_dz);
  TGeoBBox *revapd_shape  = new TGeoBBox("APDWS",alua_dx,alua_dy,alua_dz);
  TGeoCompositeShape *rev_shape = new TGeoCompositeShape("REV","CRYSTALWS+APDWS:t1");
  TGeoVolume *rev = new TGeoVolume("REV",rev_shape,aluminium);
  top->AddNode(rev,1);
  //
  // TGeoBBox of "totabsorbing" around APD and glue of APD
  //
  TGeoVolume *tot = geom->MakeBox("TOT",totabsorbing,tot_dx,tot_dy,tot_dz);
  rev->AddNode(tot,1,t3);
  //
  //  TGeoBBox APD of silicium
  //
  //  Notice that in this example, we have given to 2*apd_dz [which should be the distance from
  // cathode to anode of the APD] a totally unrealistic value of 1 cm. Refer to the description of
  // method TLitVolume::SetAPD(), where it is explained: 
  // - Important notice concerning distance from cathode to anode: the distance
  // from cathode to anode for an APD is of the order of 0.01 cm. It does not make sense
  // to give for instance to this distance a value of for instance 1 cm. But for
  // reasons of appearance of the APD on the drawing of the geometry, the user
  // can be tempted to give to the APD a depth much bigger than the true distance
  // between cathode and anode. If Litrani finds an unrealistic depth for the APD
  // when looking at the geometry of the shape being the APD, then Litrani replaces
  // this unrealistic depth by the value of the static variable TLitVolume::fgMaxCatAn
  // By default, TLitVolume::fgMaxCatAn is equal to 0.01 cm. The user of Litrani
  // can freely change this value from his CINT macro, since TLitVolume::fgMaxCatAn
  // is a public variable.
  TGeoVolume *apd = geom->MakeBox("APD",silicium,apd_dx,apd_dy,apd_dz);
  tot->AddNode(apd,1,t4);
  //  apd being a detector, booking of a TLitVolume is necessary. All characteristics of the
  // APD will then be given by a call to TLitVolume::SetAPD().
  TLitVolume *lit_apd = new TLitVolume(apd);
  lit_apd->SetAPD("GainProfile_CMSAPD",3);
  //
  // TGeoBBox of glue of APD
  //
  TGeoVolume *glueapd = geom->MakeBox("GlueAPD",glue,glueapd_dx,glueapd_dy,glueapd_dz);
  tot->AddNode(glueapd,1,t5);
  //
  // TGeoBBox crystal made of CaWO4 is centered inside "rev"
  //
  TGeoVolume *crystal = geom->MakeBox("CRYSTAL",cawo4,crys_dx,crys_dy,crys_dz);
  rev->AddNode(crystal,1);
  //
  // TGeoEltu gamma cradle [radio-active source] made of vacuum inside TOP.
  //
  TGeoVolume *cradle = geom->MakeEltu("BEAMCRADLE",sr,crad_dx,crad_dy,crad_dz);
  top->AddNode(cradle,1,ph6);
  //
  geom->CloseGeometry();
  geom->CheckOverlaps(0.01);
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // End of Building the geometry
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  //____________________________________________________________________________
  //
  // Colors and drawing
  //____________________________________________________________________________
  //
  TLit::Get()->BookCanvas();
  gTwoPad->SetAllGrey();
  gTwoPad->CommentTL(comTL);
  top->SetVisibility(kFALSE);
  top->SetVisContainers();
  top->SetLineColor(1);
  top->SetLineWidth(1);
  rev->SetLineColor(AluColor);
  rev->SetLineWidth(1);
  tot->SetVisibility(kTRUE);
  tot->SetLineColor(TotAbsColor);
  tot->SetLineWidth(1);
  apd->SetVisibility(kTRUE);
  apd->SetLineColor(SiliciumColor);
  apd->SetLineWidth(1);
  glueapd->SetVisibility(kTRUE);
  glueapd->SetLineColor(GlueColor);
  glueapd->SetLineWidth(1);
  crystal->SetVisibility(kTRUE);
  crystal->SetLineColor(CaWO4Color);
  crystal->SetLineWidth(1);
  cradle->SetVisibility(kTRUE);
  cradle->SetLineColor(CradleColor);
  cradle->SetLineWidth(1);
  gGeoManager->SetTopVisible(1);
  gGeoManager->SetVisLevel(4);
  //  gStyle->SetCanvasPreferGL(kTRUE);
  // top->Draw("ogl");
  top->Draw("");
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Generation of gammas of 1Mev
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  //  Notice that the TGeoEltu "BEAMCRADLE" is displaced AFTER the geometry has been closed by a
  // call to gGeoManager->CloseGeometry()! To understand how it is possible [calling
  // TGeoVolume::RemoveNode() and TGeoVolume::AddNode() after having closed the geometry is
  // forbidden!], read § "Representing Misalignments of the Ideal Geometry", p340 of the ROOT
  // user manual. Method TLitGamma::MoveCradle() do the trick.
  //  The gamma is generated with a gaussian distribution in energy. Mean value of 1 Mev, sigma
  // of 0.01 Mev, energy only accepted if 0.9 <= E <= 1.1 Mev. User has the choice of emitting
  // the gammas from anywhere inside the beam cradle, or from the elliptical face of the beam
  // cradle. In any case, the gammas are not emitted isotropically, but in direction of the
  // crystal, with theta values between 0 and 2 degrees. This simulates the effect of a beam
  // collimator.
  //
  Int_t krun,k;
  Bool_t ok;
  Bool_t FromFace = kFALSE;
  TLitGamma *gamma;
  TVector3 dir(0.0,0.0,1.0);
  TVector3 dirfce(0.0,0.0,1.0);
  gamma = new TLitGamma("gammas","Strontium source","/TOP_1/BEAMCRADLE_1",1.0,kTRUE,kFALSE,
    gaussian,0.9,1.1,0.01);
  k = 0;
  while ((k<1) || (k>2)) {
    cout << "Do you want that the emitted gammas come:" << endl;
    cout << " (1) - from anywhere inside the beam cradle" << endl;
    cout << " (2) - from the elliptical exit face of the beam cradle" << endl;
    cout << "Answer 1 or 2 : ";
    cin  >> k;
  }
  FromFace = (k==2);
  if (FromFace) ok = gamma->SetEmission(flat,2.0,dir,"",kFALSE,0.0,kTRUE,dirfce);
  else          ok = gamma->SetEmission(flat,2.0,dir,"",kFALSE,0.0,kFALSE);
  if (!ok) gCleanOut->MM(error,"SLitGamma01","Problem in TLitGamma::SetEmmisiion","CINT");
  Double_t xyz[3];
  xyz[0] = 0.0;
  xyz[1] = starty;
  for (krun=1;krun<=nrun;krun++) {
    gamma->Gen(krun,1,startz);
    startz += stepz;
    if (krun != nrun) {
      xyz[2] = startz;
      ph6->SetTranslation(xyz);
      gamma->MoveCradle(ph6,kTRUE);
    }
  }
  gLitGp->SetTitle("position z along crystal axis");
  gLitGp->Summary();
  gTwoPad->ChangePad();
  gLitGs->DoStat();
  TLit::Get()->CloseFiles();
}
