{
  char *name    = "AEROGELWithPMs";
  char *listing = "AEROGELWithPMs";
  char *upcom   = "Aerogel Cherenkov counter with phototubes and mirrors";
  char *downcom = "Source of light : beam of pions. Phototube on both ends";
  gROOT->ProcessLine(".x InitSLitrani.C(5,name,listing,upcom,downcom,kFALSE,kFALSE,kFALSE)");
  const char *comTL = "Beam cradle is a TGeoEltu";
  const Color_t  matVacuumColor =  0;
  const Color_t  MilColour      =  kPink;
  const Color_t  AirColour      =  kBlue;
  const Color_t  CradleColour   =  kYellow;
  const Color_t  AerogelColour  =  kGreen;
  const Color_t  TubeColour     =  kOrange;
  const Color_t  TotAColour     =  kRed;
  const Color_t  TotRColour     =  kRed;
  const Color_t  MirrorColour   =  kBlack;
  const Double_t IrrA           = 0.0; //Value used for A   when irrelevant !
  const Double_t IrrZ           = 0.0; //Value used for Z   when irrelevant !
  const Double_t IrrRho         = 0.0; //Value used for Rho when irrelevant !
  Int_t mediumindex = 1;
  gCleanOut->fMaxInfo = 1000;
  /*
  gROOT->ProcessLine(".x AbsorptionLength_Aerogel.C();");
  gROOT->ProcessLine(".x DEdx_Aerogel.C();");
  gROOT->ProcessLine(".x DiffusionLength_Aerogel.C();");
  gROOT->ProcessLine(".x QuantumEff_Hamamatsu.C();");
  */
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Building the geometry
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  TGeoManager *geom = new TGeoManager("setup","Pion detector");
  //____________________________________________________________________________
  //
  // Materials and media
  //____________________________________________________________________________
  //
  // (1) Vacuum for "TOP"
  //
  //  We define here vacuum as a TGeoMedium, not as a TLitMedium, so that photons cannot
  // travel inside it.
  //
  TGeoMaterial *vacuum_mat = new TGeoMaterial("Vacuum",IrrA,IrrZ,IrrRho);
  TGeoMedium *vacuum = new TGeoMedium("Vacuum",mediumindex++,vacuum_mat);
  //
  // (2) Full pedantic definition of Aerogel
  //
  TGeoMaterial *aerogel_mat = new TGeoMaterial("Aerogel",IrrA,IrrZ,IrrRho);
  TLitMedium *aerogel = new TLitMedium("Aerogel",mediumindex++,aerogel_mat,kFALSE,1.0,"AbsorptionLength_Aerogel");
  //setting spectrum of aerogel
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_Aerogel","Emission Spectrum | Aerogel in g0 experiment");
  sp->AddOneComponent(320,40.0,0.75,5.0,-1.0,0.39,15.0,-1.0,0.6,100,-1.0,0.01);
  sp->AddOneComponent(350,50.0,0.25,5.0,-1.0,0.39,15.0,-1.0,0.6,100,-1.0,0.01);
  sp->Normalize();
  aerogel->SetLitSpectrum(sp);
  //setting aerogel
  aerogel->dEdx("DEdx_Aerogel",0.0,kFALSE);
  aerogel->IsIsotropic(1.07);
  aerogel->SetDiffusionLength("DiffusionLength_Aerogel");
  //
  // (3) Full pedantic definition of Air
  //
  const Double_t Air_RefrIndex = 1.0003;   //Refractive index of air
  const Double_t Air_Density   = 0.00129;  //Density of air
  TGeoElementTable *table = gGeoManager->GetElementTable();
  TGeoElement *N  = table->FindElement("NITROGEN");
  TGeoElement *O  = table->FindElement("OXYGEN");
  TGeoMixture *air_mix = new TGeoMixture("Air",2,Air_Density);
  air_mix->AddElement(N,0.7);
  air_mix->AddElement(O,0.3);
  TLitMedium *air = new TLitMedium("Air",mediumindex++,air_mix,kFALSE,1.0,10000.0);
  air->IsIsotropic(Air_RefrIndex);
  //
  // (3) Full pedantic definition of Sodocal
  //
  TGeoMaterial *sodocal_mat =new TGeoMaterial("Sodocal",IrrA,IrrZ,IrrRho);
  TLitMedium *sodocal = new TLitMedium("Sodocal",mediumindex++,sodocal_mat,kFALSE,1.0,1000.0);
  sodocal->IsIsotropic("RefrIndex_Sodocal");
  //____________________________________________________________________________
  //
  // Wrappings
  //____________________________________________________________________________
  //
  //
  // Definition of "TotAbsorbing": a totally absorbing wrapping
  //
  TGeoMaterial *totabs_mat = new TGeoMaterial("TotAbsorbing",IrrA,IrrZ,IrrRho);
  TLitMedium *totabsorbing = new TLitMedium("TotAbsorbing",mediumindex++,totabs_mat,-1.0);
  totabsorbing->SetAsWrapping(0.0,0.0,1.0,1.0,1.0);
  totabsorbing->IsIsotropic(1.0);
  //
  // Definition of "TotReflecting": a totally reflecting wrapping
  //
  TGeoMaterial *totref_mat = new TGeoMaterial("Aluminium",IrrA,IrrZ,IrrRho);
  TLitMedium *totreflecting = new TLitMedium("TotReflecting",mediumindex++,totref_mat,-1.0);
  totreflecting->SetAsWrapping(0.0,0.0,0.9,1.0,0.0,90.0);
  totreflecting->IsIsotropic(Air_RefrIndex);
  //
  // Definition of wrapping medium millipore
  //
  const Double_t millipore_diffus    = 0.5;
  const Double_t milliporeSupplAbs   = 0.0;  /*0.04*/
  const Double_t RIndexRev_millipore = 0.1;
  const Double_t IIndexRev_millipore = 0.1;
  TGeoMaterial *millipore_mat = new TGeoMaterial("Millipore",IrrA,IrrZ,IrrRho);
  TLitMedium *millipore = new TLitMedium("Millipore",mediumindex++,millipore_mat,-1.0);
  millipore->SetAsWrapping(millipore_diffus,"RIndexRev_Tyvek","IIndexRev_Tyvek",1.0, milliporeSupplAbs); 
  millipore->IsIsotropic(Air_RefrIndex);
  //____________________________________________________________________________
  //
  // Dimensions
  //____________________________________________________________________________
  //
  // Dimensions of box of air
  //
  const Double_t air_dx = 30.0;
  const Double_t air_dy = 15.0;
  const Double_t air_dz = 15.0;
  //
  // Dimensions of aerogel
  //
  Double_t aerogel_dx = air_dx;
  Double_t aerogel_dy = air_dy;
  const Double_t aerogel_dz = 1;
  //
  // Dimensions of cradle
  //
  const Double_t crad_dx = 1.0;
  const Double_t crad_dy = 1.0;
  const Double_t crad_dz = 1.0;
  //
  // Width of wrapping is wid
  //
  const Double_t wid =0.01;
  Double_t ws2 = 0.5*wid;
  //
  // Dimensions of TGEOBBox of millipore around box of air and aerogel
  //
  Double_t mill_dx = air_dx + wid;
  Double_t mill_dy = air_dy + wid;
  Double_t mill_dz = air_dz + aerogel_dz + wid;
  //
  // Dimensions of phototubes
  //
  const Double_t tube_rmi = 0.0;
  const Double_t tube_rma = 12.7;
  const Double_t tube_dz = 2.0;
  //
  // Dimensions of millipore wrapping around phototubes
  //
  Double_t milt_rmi = tube_rmi;
  Double_t milt_rma = tube_rma + wid;
  Double_t milt_dz = tube_dz;
  //
  // Dimensions of total abosorbing wrapping around phototubes
  //
  Double_t taw_rmi = tube_rmi;
  Double_t taw_rma = tube_rma + wid;
  Double_t taw_dz = tube_dz + ws2 ;
  //
  // Dimensions of mirrors and wrappings for mirrors
  //
  const Double_t mirr_dx = 0.01;
  const Double_t mw = 0.001;
  Double_t mirr_dy = air_dy;
  Double_t mirr_dz = sqrt(pow(air_dz,2)+pow(air_dx/2,2));
  Double_t totr_dz = mirr_dz + mw;
  Double_t totr_dy = mirr_dy + mw;
  Double_t totr_dx = mirr_dx + mw;
  //
  // Dimensions of TOP
  //
  const Double_t top_dx    = 100.0;
  const Double_t top_dy    = 100.0;
  Double_t top_dz = wid + mill_dz + crad_dz + 100.0;
  //____________________________________________________________________________
  //
  // Positionning (translations)
  //____________________________________________________________________________
  //
  //
  // Start position of aerogel below air box
  //
  Double_t t_aerogel_z = -air_dz;
  TGeoTranslation *ta = new TGeoTranslation("ta",0.0,0.0,t_aerogel_z);
  //
  // Start position of box of air on top of and in contact with aerogel
  //
  Double_t t_air_z = aerogel_dz;
  TGeoTranslation *tb = new TGeoTranslation("tb",0.0,0.0,t_air_z);
  //
  // Start position of beam cradle
  //
  Double_t cradle_z = -mill_dz - 2;
  TGeoTranslation *tc = new TGeoTranslation("tc",0.0,0.0,cradle_z);
  //
  // Start position and rotation of both tubes and wrappers
  //
  TGeoRotation *rot = new TGeoRotation("rot",90,90,0);
  Double_t mwtube1_x = mill_dx + milt_dz;
  Double_t mwtube2_x = -(mill_dx + milt_dz);
  TGeoCombiTrans *cwmt1 = new TGeoCombiTrans("cwmt1",mwtube1_x,0.0,aerogel_dz,rot);
  TGeoCombiTrans *cwmt2 = new TGeoCombiTrans("cwmt2",mwtube2_x,0.0,aerogel_dz,rot);
  cwmt1->RegisterYourself();
  cwmt2->RegisterYourself();
  Double_t tawtube1_x = air_dx + milt_dz + ws2;
  Double_t tawtube2_x = -(air_dx + milt_dz + ws2);
  TGeoCombiTrans *cwtat1 = new TGeoCombiTrans("cwtat1",tawtube1_x,0.0,aerogel_dz,rot);
  TGeoCombiTrans *cwtat2 = new TGeoCombiTrans("cwtat2",tawtube2_x,0.0,aerogel_dz,rot);
  TGeoTranslation *tt1 = new TGeoTranslation("tt1",0.0,0.0,-ws2);
  TGeoTranslation *tt2 = new TGeoTranslation("tt2",0.0,0.0,ws2);
  //
  // Start position and rotation of both mirrors and wrappers
  //
  Double_t alpha = (TMath::ATan(2*(air_dz)/(air_dx)))*180/TMath::Pi();
  Double_t phi,theta,psi;
  TGeoRotation rz1;
  TEuler euler(180.0-alpha,0.0,90.0,90.0);
  euler.GetEulerAngles(phi,theta,psi);
  rz1.SetAngles(phi,theta,psi);
  TGeoTranslation tz1(air_dx/2,0.0,0.0);
  TGeoCombiTrans *cz1 = new TGeoCombiTrans(tz1,rz1);
  TGeoHMatrix *rmz1 = new TGeoHMatrix(*cz1);
  Double_t phi,theta,psi;
  TGeoRotation rz2;
  TEuler euler(alpha,0.0,90.0,90.0);
  euler.GetEulerAngles(phi,theta,psi);
  rz2.SetAngles(phi,theta,psi);

  TGeoTranslation tz2(-air_dx/2,0.0,0.0);
  TGeoCombiTrans *cz2 = new TGeoCombiTrans(tz2,rz2);
  TGeoHMatrix *rmz2 = new TGeoHMatrix(*cz2);

  TGeoTranslation twz1(air_dx/2,0.0,0.0);
  TGeoCombiTrans *cwz1 = new TGeoCombiTrans(twz1,rz1);
  TGeoHMatrix *rmwz1 = new TGeoHMatrix(*cwz1);

  TGeoTranslation twz2(-air_dx/2,0.0,0.0);
  TGeoCombiTrans *cwz2 = new TGeoCombiTrans(twz2,rz2);
  TGeoHMatrix *rmwz2 = new TGeoHMatrix(*cwz2);
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
  // Composite shape of millipore
  //
  TGeoBBox *revmill_shape  = new TGeoBBox("MILLWS",mill_dx,mill_dy,mill_dz);
  TGeoTube *revtube1_shape = new TGeoTube("TUBE1WS",milt_rmi,milt_rma,milt_dz);
  TGeoTube *revtube2_shape = new TGeoTube("TUBE2WS",milt_rmi,milt_rma,milt_dz);
  TGeoCompositeShape *rev_shape = 
    new TGeoCompositeShape("REV","MILLWS+TUBE1WS:cwmt1+TUBE2WS:cwmt2");
  TGeoVolume *rev = new TGeoVolume("REV",rev_shape,millipore);
  top->AddNode(rev,1);
  //
  //  TGeoBBox of air inside "rev"
  //
  TGeoVolume *box_shape = geom->MakeBox("BOX",air,air_dx,air_dy,air_dz);
  rev->AddNode(box_shape,1,tb);
  //
  // TGeoBBox aerogel is centered inside "rev" + half width of wrapping
  //
  TGeoVolume *aerogel_shape = geom->MakeBox("AEROGEL",aerogel,aerogel_dx,aerogel_dy,aerogel_dz);
  rev->AddNode(aerogel_shape,1,ta);
  //
  // TGeoEltu wrapper for tubes made of total absorbing material inside rev
  //
  TGeoVolume *tawtube1 = geom->MakeTube("TAWTUBE1",totabsorbing,taw_rmi,taw_rma,taw_dz);
  rev->AddNode(tawtube1,1,cwtat1);
  TGeoVolume *tawtube2 = geom->MakeTube("TAWTUBE2",totabsorbing,taw_rmi,taw_rma,taw_dz);
  rev->AddNode(tawtube2,1,cwtat2);
  //
  // TGeoTube tubes made of sodocal inside totalabsorbing wrapper
  //
  TGeoVolume *tube1 = geom->MakeTube("TUBE1",sodocal,tube_rmi,tube_rma,tube_dz);
  tawtube1->AddNode(tube1,1,tt1);
  TGeoVolume *tube2 = geom->MakeTube("TUBE2",sodocal,tube_rmi,tube_rma,tube_dz);
  tawtube2->AddNode(tube2,1,tt2);
  //
  // Transforming tubes into phototubes
  //
  TLitVolume *phototube1 = new TLitVolume(tube1);
  //  phototube1->SetPhotTube("QuantumEff_Hamamatsu",1.0); // or
  //  phototube1->SetPhotTube("QuantumEff_Hamamatsu",-1.0);
  phototube1->SetDetector(true,"QuantumEff_Hamamatsu",91.0);
  TLitVolume *phototube2 = new TLitVolume(tube2);
  //  phototube2->SetPhotTube("QuantumEff_Hamamatsu",1.0); // or
  //  phototube2->SetPhotTube("QuantumEff_Hamamatsu",-1.0);
  phototube2->SetDetector(true,"QuantumEff_Hamamatsu",91.0);
  //
  // TGeoBBox mirrors and wrappers centered inside air box inside "rev"
  //
  TGeoBBox *wmirror_shape = new TGeoBBox("WMIRROR",totr_dx,totr_dy,totr_dz);
  TGeoVolume *wmirr1 = new TGeoVolume("WMIRROR1",wmirror_shape,totreflecting);
  TGeoVolume *wmirr2 = new TGeoVolume("WMIRROR2",wmirror_shape,totreflecting);

  //   box_shape->AddNode(wmirr1,1,rmwz1);
  //   box_shape->AddNode(wmirr2,1,rmwz2);

  TGeoBBox *mirror_shape = new TGeoBBox("MIRROR",mirr_dx,mirr_dy,mirr_dz);
  TGeoVolume *mirr1 = new TGeoVolume("MIRROR1",mirror_shape,air);
  TGeoVolume *mirr2 = new TGeoVolume("MIRROR2",mirror_shape,air);

  //   wmirr1->AddNode(mirr1,1);
  //   wmirr2->AddNode(mirr2,1);

  //
  // TGeoEltu beam cradle made of vacuum inside TOP.
  //
  TGeoVolume *cradle = geom->MakeEltu("BEAMCRADLE",vacuum,crad_dx,crad_dy,crad_dz);
  top->AddNode(cradle,1,tc);

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
  rev->SetLineColor(MilColour);
  rev->SetLineWidth(1);
  box_shape->SetVisibility(kTRUE);
  box_shape->SetLineColor(AirColour);
  box_shape->SetLineWidth(1);
  aerogel_shape->SetVisibility(kTRUE);
  aerogel_shape->SetLineColor(AerogelColour);
  aerogel_shape->SetLineWidth(1);
  cradle->SetVisibility(kTRUE);
  cradle->SetLineColor(CradleColour);
  cradle->SetLineWidth(1);
  tube1->SetVisibility(kTRUE);
  tube1->SetLineColor(TubeColour);
  tube1->SetLineWidth(1);
  tube2->SetVisibility(kTRUE);
  tube2->SetLineColor(TubeColour);
  tube2->SetLineWidth(1);
  tawtube1->SetVisibility(kTRUE);
  tawtube1->SetLineColor(TotAColour);
  tawtube1->SetLineWidth(1);
  tawtube2->SetVisibility(kTRUE);
  tawtube2->SetLineColor(TotAColour);
  tawtube2->SetLineWidth(1);
  //   mirr1->SetVisibility(kTRUE);
  //   mirr1->SetLineColor(MirrorColour);
  //   mirr1->SetLineWidth(1);
  //   mirr2->SetVisibility(kTRUE);
  //   mirr2->SetLineColor(MirrorColour);
  //   mirr2->SetLineWidth(1);
  //   wmirr1->SetVisibility(kTRUE);
  //   wmirr1->SetLineColor(TotRColour);
  //   wmirr1->SetLineWidth(1);
  //   wmirr2->SetVisibility(kTRUE);
  //   wmirr2->SetLineColor(TotRColour);
  //   wmirr2->SetLineWidth(1);
  gGeoManager->SetTopVisible(1);
  gGeoManager->SetVisLevel(4);
  TLit::Get()->BookCanvas(5);
  gTwoPad->SetStateOfPads(Pad1Only);
  gTwoPad->SetAllGrey();
  gTwoPad->CommentTL(comTL);
  top->Draw("");
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  // Generation of photons
  //____________________________________________________________________________
  //____________________________________________________________________________
  //
  TLitParticle *particle = new TLitParticle(211,singlevalued,1.0,1.0,1.0,0.5,1);
  TVector3 dir(0.0,0.0,1.0);
  TVector3 dirfce(0.0,0.0,1.0);
  //   particle->SetEmitCerenkov(kTRUE);
  TLitBeam *beam = new TLitBeam("pions","beam of pions","/TOP_1/BEAMCRADLE_1",particle,kTRUE,kFALSE);
  beam->SetEmission(flat,5.0,dir,"",kFALSE,0.0,kTRUE,dirfce);
  //   TLitPhoton::fgNPointsMax = 100000;
  beam->SetCerenkov(kTRUE);
  //   beam->TrackToDraw(1);
  beam->Gen(1,1000);
  gLitGp->SetTitle("position z along air axis");
  gLitGp->Summary();
  gTwoPad->SetStateOfPads(Pad1AndPad2);
  gTwoPad->ChangePad();
  gLitGs->DoStat();
  TLit::Get()->CloseFiles();
}
