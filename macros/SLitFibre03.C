{
  char *name    = "SLitFibre03";
  char *listing = "SLitFibre03";
  char *upcom   = "Setup crystal and APD";
  char *downcom = "Simple example without thin slice or anisotropy";
  gROOT->ProcessLine(".x InitSLitrani.C(5,name,listing,upcom,downcom,kFALSE,kFALSE,kFALSE)");
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
  //  Simulation of an isotropic PbWO4 crystal having the form of a TGeoBBox, on top of which
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
  //  In this example, we want to study the dependence of the efficiency versus the point of emission
  // of the photons along the z axis of the crystal. For that, we create a small TGeoBBox "CRADLE"
  // inside the TGeoBBox of the crystal. This small TGeoBBox will then be displaced from run to run
  // along the z axis of the crystal. This small TGEoBBox "CRADLE" is made of the same TLitMedium
  // "pbwo4" as the crystal. The photons are emitted by the small TGeoBBox according to the spectrum
  // "Spectrum_PbWO4La" of pbwo4.
  //  Notice that the small TGeoBBox "CRADLE" is displaced AFTER the geometry has been closed by a
  // call to gGeoManager->CloseGeometry()! To understand how it is possible [calling
  // TGeoVolume::RemoveNode() and TGeoVolume::AddNode() after having closed the geometry is
  // forbidden!], read § "Representing Misalignments of the Ideal Geometry", p340 of the ROOT
  // user manual.
  //
  const char *comTL = "crystal wrapped with aluminium";
  const Color_t  matVacuumColor =  0;
  const Color_t  PbWO4Color     =  9;
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
  Short_t ns = -1;
  Bool_t  ok = kFALSE;
  /*
  while (!ok) {
    cout << "Please choose between:" << endl;
    cout << " - a setup with realistic dimensions [answer 0]" << endl;
    cout << " - a setup easy to see on drawing    [answer 1]" << endl;
    cout << "   your choice: ";
    cin  >> ns;
    ok = (ns==0) || (ns==1);
  }
  */
  ns = 0;
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Building the geometry
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  TGeoManager *geom = new TGeoManager("setup","SLitFibre03 of new Litrani");
  //____________________________________________________________________________
  //
  // Materials and media
  //____________________________________________________________________________
  //
  // (1) Vacuum for "TOP"
  //
  //  We define here vacuum as a TGeoMedium, not as a TlitMedium, so that photons cannot
  // travel inside it.
  //
  TGeoMaterial *vacuum_mat = new TGeoMaterial("Vacuum",IrrA,IrrZ,IrrRho);
  TGeoMedium *vacuum = new TGeoMedium("Vacuum",mediumindex++,vacuum_mat);
  //
  // (2) Full pedantic definition of PbWO4!
  //
  //  By this definition, A,Z and density are defined. They are needed by Litrani ONLY if
  // class TLitGamma is used, which is not the case here. We give them here just
  // as an example. Radiation length, interaction length and Moliere radius are also given
  // here, although not used. They would be used only with class TLitCascade.
  //
  const Double_t PbWO4_radlen   = 0.893;  //radiation length of PbWO4
  const Double_t PbWO4_intlen   = 19.5;   //interaction length of PbWO4
  const Double_t PbWO4_rMoliere = 2.0;    //Moliere radius of PbWO4 in cm
  TGeoElementTable *table = gGeoManager->GetElementTable();
  TGeoElement *Pb = table->FindElement("LEAD");
  TGeoElement *W  = table->FindElement("TUNGSTEN");
  TGeoElement *O  = table->FindElement("OXYGEN");
  TGeoMixture *pbwo4_mix = new TGeoMixture("PbWO4",3,8.28);
  pbwo4_mix->AddElement(Pb,1);
  pbwo4_mix->AddElement(W,1);
  pbwo4_mix->AddElement(O,4);
  pbwo4_mix->SetRadLen(-PbWO4_radlen,PbWO4_intlen);
  TLitMedium *pbwo4 = new TLitMedium("PbWO4",mediumindex++,pbwo4_mix,kFALSE,1.0,"AbsorptionLength_PbWO4");
  const Double_t ns0 = 1.5861;   //index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.1062;   //index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 270.63;   //1st resonant wavelength for the Sellmeier law for PbWO4
  pbwo4->IsIsotropic(1,ns0,ns1,ls1);
  pbwo4->FindSpectrum("Spectrum_PbWO4La");
  pbwo4->SetCascades(PbWO4_rMoliere); //only useful when TLitCascade used
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
  switch (ns) {
  case 1:
    TLitMedium *glue = new TLitMedium("Glue",mediumindex++,glue_mat,kFALSE,1.0,10.0);
    break;
  default:
    TLitMedium *glue = new TLitMedium("Glue",mediumindex++,glue_mat,kFALSE,1.0,
      "AbsorptionLength_Meltmount1704");
    break;
  }
  glue->IsIsotropic("RefrIndex_RTV3145");
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
  const Double_t Air_RefrIndex     = 1.0003;   //Refractive index of air
  const Double_t aluminium_diffus  = 0.01;
  const Double_t AluminiumSupplAbs = 0.1;
  TGeoMaterial *aluminium_mat = new TGeoMaterial("Aluminium",IrrA,IrrZ,IrrRho);
  TLitMedium *aluminium = new TLitMedium("Aluminium",mediumindex++,aluminium_mat,1.0);
  aluminium->SetAsWrapping(aluminium_diffus,"RIndexRev_Aluminium","IIndexRev_Aluminium",1.0,AluminiumSupplAbs);
  aluminium->IsIsotropic(Air_RefrIndex);
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
  // Dimensions of beam cradle inside crystal
  //
  const Double_t crad_dx = 0.2;
  const Double_t crad_dy = 0.2;
  const Double_t crad_dz = 0.2;
  //
  // Start position of beam cradle and step size
  //
  const Double_t frsp  = 1.0; //frsp == free space inside crystal below cradle
  const Double_t stepz = 1.0;
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
  const Double_t glueapd_dz[2] = {0.05,0.5};
  //
  // Width of wrapping is wid
  //
  const Double_t wid[2] = {0.1,0.5};
  Double_t ws2 = 0.5*wid[ns];
  //
  // Dimensions of TGEOBBox of alu around crystal "CRYSTALWS"
  //
  Double_t aluc_dx = crys_dx + wid[ns];
  Double_t aluc_dy = crys_dy + wid[ns];
  Double_t aluc_dz = crys_dz + wid[ns];
  //
  // Dimensions of TGEOBBox of alu around APD and glue of APD [partially] "APDWS"
  //
  Double_t alua_dx = apd_dx + wid[ns];
  Double_t alua_dy = apd_dy + wid[ns];
  Double_t alua_dz = apd_dz + glueapd_dz[ns];
  //
  // Dimensions of TotAbsorbing wrapping around APD and glue of APD
  //
  Double_t tot_dx = apd_dx + wid[ns];
  Double_t tot_dy = apd_dy + wid[ns];
  Double_t tot_dz = apd_dz + glueapd_dz[ns] + ws2;
  //
  // Dimensions of TOP
  //
  const Double_t top_dx    = 4.0;
  const Double_t top_dy    = 4.0;
  Double_t top_dz = wid[ns] + crys_dz + glueapd_dz[ns] + apd_dz + 1.0;
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
  Double_t t_apd_tot_z = glueapd_dz[ns] - ws2;
  TGeoTranslation *t4 = new TGeoTranslation("t4",0.0,0.0,t_apd_tot_z);
  //  t5 is for positionning TGeoBBox of glue of the APD inside the TGeoBBox of "totabsorbing"
  //  around APD and glue of APD "TOT"
  //
  const Double_t t_glueapd_tot_z = -(apd_dz + ws2);
  TGeoTranslation *t5 = new TGeoTranslation("t5",0.0,0.0,t_glueapd_tot_z);
  //  t6 is for the start position of the beam cradle inside crystal
  TGeoTranslation *t6 = new TGeoTranslation("t6",0.0,0.0,startz);
  //____________________________________________________________________________
  //
  // Volumes and nodes
  //____________________________________________________________________________
  //
  //
  // top box containing everything and defining the WCS, world coordinate system
  //
  TGeoVolume *top = geom->MakeBox("TOP",vacuum,top_dx,top_dy,top_dz);
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
  TGeoVolume *glueapd = geom->MakeBox("GlueAPD",glue,glueapd_dx,glueapd_dy,glueapd_dz[ns]);
  tot->AddNode(glueapd,1,t5);
  //
  // TGeoBBox crystal made of PbWO4 is centered inside "rev"
  //
  TGeoVolume *crystal = geom->MakeBox("CRYSTAL",pbwo4,crys_dx,crys_dy,crys_dz);
  rev->AddNode(crystal,1);
  //
  // TGeoBBox beam cradle made of PbWO4 inside crystal. Since it is a source of photons
  // we have to associate a TLitVolume and to call TLitVolume::SetEmission()
  //
  TGeoVolume *cradle = geom->MakeBox("CRADLE",pbwo4,crad_dx,crad_dy,crad_dz);
  crystal->AddNode(cradle,1,t6);
  TLitVolume *lit_cradle = new TLitVolume(cradle);
  lit_cradle->SetEmission();
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
  crystal->SetLineColor(PbWO4Color);
  crystal->SetLineWidth(1);
  cradle->SetVisibility(kTRUE);
  cradle->SetLineColor(CradleColor);
  cradle->SetLineWidth(1);
  gGeoManager->SetTopVisible(1);
  gGeoManager->SetVisLevel(4);
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Generation of photons
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  //  Notice that the small TGeoBBox "CRADLE" is displaced AFTER the geometry has been closed by a
  // call to gGeoManager->CloseGeometry()! To understand how it is possible [calling
  // TGeoVolume::RemoveNode() and TGeoVolume::AddNode() after having closed the geometry is
  // forbidden!], read § "Representing Misalignments of the Ideal Geometry", p340 of the ROOT
  // user manual. We have to use the TGeoPhysicalNode pn. When this node pn is moving, the associated
  // TGeoVolume startV is deleted and recreated, so that its hexadecimal address changes. It is the
  // reason for the 2 lines:
  //
  //    startV = pn->GetVolume();
  //    lit_cradle->SetGeoVolume(startV);
  //
  Int_t krun;
  TLitSpontan *sp = new TLitSpontan("spontan","spontan","/TOP_1/REV_1/CRYSTAL_1/CRADLE_1",kTRUE,kFALSE);
  TLit::Get()->BookCanvas(5);
  //  TLit::Get()->BookCanvas(600,600);
  gTwoPad->SetStateOfPads(Pad1Only);
  //  gTwoPad->SetAllGrey();
  gTwoPad->CommentTL(comTL);
  //  gStyle->SetCanvasPreferGL(kTRUE);
  // top->Draw("ogl");
  top->Draw("");
  for (krun=1;krun<=nrun;krun++) {
    sp->Gen(krun,10000,startz);
    startz += stepz;
    if (krun != nrun) {
      t6->SetTranslation(0.0,0.0,startz);
      sp->MoveCradle(t6,kTRUE);
    }
  }
  gLitGp->SetTitle("position z along crystal axis");
  gLitGp->Summary();
  gTwoPad->SetStateOfPads(Pad1AndPad2);
  gTwoPad->ChangePad();
  gLitGs->DoStat();
  TLit::Get()->CloseFiles();
}
