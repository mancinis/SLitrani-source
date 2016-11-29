#include "Riostream.h"
#include <unistd.h>

int Simul_Nucifer2()

{

/*------------------------------------------------------------------------------------------------------------*/
/* Affichage des entrées :                                                                                    */
/*------------------------------------------------------------------------------------------------------------*/


  cout << endl;
  cout << "Start from Litrani, all libs loaded" << endl;
  TLit::Kill();


/*------------------------------------------------------------------------------------------------------------*/
/* The 7 arguments of the static method TLit::SetParameters() are the following :                             */
/*------------------------------------------------------------------------------------------------------------*/


//    size    : size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
//    nbbut   : Number of buttons wanted in TwoPadDisplay. Bigger than 10. [default 24]
//    xminC   : x coordinate, in pixel, of top left corner of main canvas  [default 2]
//    yminC   : y coordinate, in pixel, of top left corner of main canvas  [default 2]
//    WithDate: if true, date and time are appended to the name of the ROOT file
//              and of the listing, in order not to loose them when starting again
//              the job.                                                             [default false]     
//    name    : the name of the .root file that Litrani produces, containing
//              statistics and histograms of all runs. Do not give the suffix .root.
//              It will be appended.                                                 [default "litrani"]
//    listing : the name of the .lis file that Litrani produces, containing
//              messages issued by litrani. Do not give the suffix .lis . It will
//              be appended.
//              [default "litrani"] 
//    ls:     : number of characters in a line of output or listing [default 119]

  cout << "Booking TLit" << endl;
  TLit::SetParameters(5,24,20,2,kFALSE,"Simul_Nucifer2","Simul_Nucifer2");


/*------------------------------------------------------------------------------------------------------------*/
/* Parameters of SLitrani :                                                                                   */
/*------------------------------------------------------------------------------------------------------------*/


  // (1) TLit::Get()->SetTesTCalc(kTRUE) asks for many many checks in the computations.
  TLit::Get()->SetTestCalc(kTRUE);


  // (2) Maximum number of output messages
  gCleanOut->fMaxInfo = 10000;


/*------------------------------------------------------------------------------------------------------------*/
/* Simulation of Detector Nucifer :                                                                           */
/*------------------------------------------------------------------------------------------------------------*/


  //  Simulation of PSD in the detector "Nucifer" :
  // We try to determinate the quality of PSD in fonction of wrapping between the tank and the scintillator.
  // So, we create in SLitrani the detector (tank, scintillator, buffer and photodetectors), then we 
  // generate into the Scintillator two signal. They discribe signals generate by neutrinos and fast 
  // neutrons (induice by muons). [...]


/*------------------------------------------------------------------------------------------------------------*/
/* What kind of Simulation?                                                                                 */
/*------------------------------------------------------------------------------------------------------------*/


  int k = 0;
  while ((k<1) || (k>2)) {
    cout << "Do you want a simulation of :" << endl;
    cout << " (1) - mesure of detection efficiency" << endl;
    cout << " (2) - Pulse Shape Discrimination" << endl;
    cout << "Answer 1 or 2 : ";
    cin  >> k; }
  cout << endl;


  int l = 0;
  while ((l<1) || (l>5)) {
    cout << "What kind of wrapping do you want :" << endl;
    cout << " (1) - total absorbing" << endl;
    cout << " (2) - total teflon " << "  Warning : undo " << endl;
    cout << " (3) - Fond absorbant et cotés diffusants " << "  Warning : undo " << endl;
    cout << " (4) - Fond absorband et cotés réfléchissant à 90% " << "  Warning : undo " << endl;
    cout << " (5) - Fond absorband et cotés en acier inox " << "  Warning : undo " << endl;
    cout << "Answer : ";
    cin >> l; }
  cout << endl;


/*------------------------------------------------------------------------------------------------------------*/
/* Building the geometry :                                                                                    */
/*------------------------------------------------------------------------------------------------------------*/


  TGeoManager *geom = new TGeoManager("","");


/*------------------------------------------------------------------------------------------------------------*/
/* Materials and media :                                                                                      */
/*------------------------------------------------------------------------------------------------------------*/


  // (0) Definition of atoms
  
  TGeoElementTable *table = gGeoManager->GetElementTable();
  TGeoElement *H = table->FindElement("HYDROGEN");
  TGeoElement *C = table->FindElement("CARBON");
  TGeoElement *N = table->FindElement("NITROGEN");
  TGeoElement *O = table->FindElement("OXYGEN");

  int mediumindex = 1;
  const double IrrA = 0.0, IrrZ = 0.0, IrrRho = 0.0;	// A, Z and Rho are needed only if we use TLitGamma


  // (1) Nitrogen gaz for "TOP"
  // We define the environnement of Nucifer as a medium which is composed only of N2

  const double La_N2 = 350.0;
  const double n_n2  =   1.2;
  TGeoMaterial *n2_mat = new TGeoMaterial("N2",IrrA,IrrZ,IrrRho);
  TLitMedium *n2 = new TLitMedium("N2",mediumindex++,n2_mat,kFALSE,1.0,La_N2);
  n2->IsIsotropic(n_n2);

    /* if you have the refraction index in fonction of wavelength, use n2->IsIsotropic("namefile")   */
    /* if you have the absorption length in fonction of wavelength, use n2->FindSpectrum("namefile") */


  // (2) Full pedantic definition of media of Scintillator
 
  const double La_liquid = 400.0;	//To be replaced later by true value, depending upon wavelength
  const double n_liquid  =  1.49;
  TGeoMaterial *liquid_mat = new TGeoMaterial("liquid",IrrA,IrrZ,IrrRho);
  TLitMedium *liquid = new TLitMedium("liquid",mediumindex++,liquid_mat,kFALSE,1.0,La_liquid);
  liquid->IsIsotropic(n_liquid);


  // (3) Definition of the Spectrum of Scintillator

      // (3.1) Component of Gaussian Distribution of Wavelenght generate by the Scintillator (in nm)
  const double mean_wl  = 424.0;
  const double sigma_wl = 100.0;

      // (3.2) Component of Time Decay Distribution of the Scintillator
  const double q1 = 0.936, tau1 =   2.1;
  const double q2 = 0.053, tau2 =  14.2;
  const double q3 = 0.011, tau3 = 106.0;

      // (3.3) Definition of the Spectrum
  TLitSpectrum *spectrum = new TLitSpectrum("Spectrum of BC 505","Emission Spectrum | BC 505");
  spectrum->AddOneComponent(mean_wl,sigma_wl,1.0);
  spectrum->AddTimeComponent(mean_wl,q1,0.0,tau1);
  spectrum->AddTimeComponent(mean_wl,q2,0.0,tau2);
  spectrum->AddTimeComponent(mean_wl,q3,0.0,tau3);
  spectrum->Normalize();

      // (3.4) Association of Scintillator and its Spectrum
  liquid->SetLitSpectrum(spectrum);


  // (4) Definition of media "acrylic" of buffer
  // We use as example Polymethyl mathacrylate

  const double La_acrylic = 550.0; 
  const double n_acrylic  =  1.49;
  TGeoMaterial *acrylic_mat = new TGeoMaterial("Acrylic",IrrA,IrrZ,IrrRho);
  TLitMedium *acrylic = new TLitMedium("Acrylic",mediumindex++,acrylic_mat,kFALSE,1.0,La_acrylic);
  acrylic->IsIsotropic(n_acrylic);


  // (5) Definition of medium in PMs

    // (5.1) Definition of media "sodocal" of window of PMs  
  const double La_sodocal = 1000.0;
  TGeoMaterial *sodocal_mat = new TGeoMaterial("sodocal",IrrA,IrrZ,IrrRho);
  TLitMedium *sodocal = new TLitMedium("sodocal",mediumindex++,sodocal_mat,kFALSE,1.0,La_sodocal);
  sodocal->IsIsotropic("RefrIndex_Sodocal");
  
  /* As photons do not enter inside of the volume of the PM, but immediatly stop on the photocathode,
  the material of the PM does not matter. So we declare it identical with the material of the entrance
  window of the PM, so that the transition (window of PM) => (photocathode) is immediate.
     However, we are oblige to give an other name [here "cathode" instead of sodocal] to the TLitMedium
  of the photocathode, it has to be declared as "sensible", contrary to the TLitMedium of the window
  of the PM. */

    // (5.2) Definition of "Cathode"
  TLitMedium *cathode = new TLitMedium("cathode",mediumindex++,sodocal_mat,kTRUE,1.0,La_sodocal);
  cathode->IsIsotropic("RefrIndex_Sodocal");

    // (5.3) Definition of "WrapPM" : a strongly absorbing wrapping around the PMs
    // We can also use "TotAbsorbing" if you want a totally absorbing wrapping
  const double wrapPM_diffus   = 0.9;
  const double wrapPM_Rindex   = 2.5;
  const double wrapPM_Iindex   = 1.5;
  const double wrapPM_supplAbs = 0.9;
  TGeoMaterial *wrapPM_mat = new TGeoMaterial("WrapPM",IrrA,IrrZ,IrrRho);
  TLitMedium *wrappm = new TLitMedium("WrapPM",mediumindex++,wrapPM_mat,-1.0);
  wrappm->SetAsWrapping(wrapPM_diffus,wrapPM_Rindex,wrapPM_Iindex,1.0,wrapPM_supplAbs);
  wrappm->IsIsotropic(1.0);  // Only useful if last arg of constructor changed to +1.0


  // (6) Definitions of Wrapping of the Tank

    // (6.1) Definition of "TotAbsorbingTank" : a totally absorbing wrapping
  const double totabs_diffus   = 0.0;
  const double totabs_Rindex   = 0.0;
  const double totabs_Iindex   = 1.0;
  const double totabs_supplAbs = 1.0;
  TGeoMaterial *totabs_mat = new TGeoMaterial("TotAbsorbing_Tank",IrrA,IrrZ,IrrRho);
  TLitMedium *totabsorbing = new TLitMedium("TotAbsorbing_Tank",mediumindex++,totabs_mat,-1.0);
  totabsorbing->SetAsWrapping(totabs_diffus,totabs_Rindex,totabs_Iindex,1.0,totabs_supplAbs);
  totabsorbing->IsIsotropic(1.0);  // Only useful if last arg of constructor changed to +1.0

    //  (6.2) Definition of wrapping medium teflon
  const double teflon_diffus =  0.0;
  const double teflon_Rindex =  0.0;
  const double teflon_Iindex =  1.0;
  const double teflon_Abs    =  1.0;
  TGeoMaterial *teflon_mat = new TGeoMaterial("Teflon",IrrA,IrrZ,IrrRho);
  TLitMedium *teflon = new TLitMedium("Teflon",mediumindex++,teflon_mat,-1.0);
  teflon->SetAsWrapping(teflon_diffus,teflon_Rindex,teflon_Iindex,1.0,teflon_Abs);
  teflon->IsIsotropic(1.0);

    // (6.3) Definition of wrapping medium polished stainless steel
  const double steel_diffus  = 0.5;
  const double steel_Rindex  = 0.0;
  const double steel_Iindex  = 1.0;
  const double steel_Abs     = 0.6;
  TGeoMaterial *steel_mat = new TGeoMaterial("Steel",IrrA,IrrZ,IrrRho);
  TLitMedium *steel = new TLitMedium("Steel",mediumindex++,steel_mat,1.0);
  steel->SetAsWrapping(steel_diffus,steel_Rindex,steel_Iindex,1.0,steel_Abs);
  steel->IsIsotropic(1.0);

    // (6.4) Definition of wrapping medium VM2000which is at 90% mirror
  const double vm_diffus = 0.0;
  const double vm_Rindex = 0.0;
  const double vm_Iindex = 1.0;
  const double vm_Abs    = 0.1;
  TGeoMaterial *vm_mat = new TGeoMaterial("Mirror",IrrA,IrrZ,IrrRho);
  TLitMedium *mirror = new TLitMedium("Mirror",mediumindex++,vm_mat,1.0);
  mirror->SetAsWrapping(vm_diffus,vm_Rindex,vm_Iindex,1.0,vm_Abs);
  mirror->IsIsotropic(1.0);


/*------------------------------------------------------------------------------------------------------------*/
/* Dimensions for the User (in cm) :                                                                          */
/*------------------------------------------------------------------------------------------------------------*/


  const double Tank_h = 150.0;
  const double Tank_d = 132.0;
  const double Tank_w =   1.0;

  const double Buffer_h =  25.0;
  const double Buffer_d = 126.0;

  const double Scint_h       = 70.0;
  const double Hbuffer_scint = 60.0;

  const double PM_h = 25.00;	// Warning : PM_h > 0.8
  const double PM_d = 20.32;	// Warning : PM_d > 0.4


/*------------------------------------------------------------------------------------------------------------*/
/* Dimensions used by SLitrani :                                                                              */
/*------------------------------------------------------------------------------------------------------------*/

  
  // (1) Dimensions of Tank

  const double tank_dz = Tank_h / 2.0;
  const double tank_r  = Tank_d / 2.0;
  const double base_dz = Tank_w / 4.0;
  const double base_r  = Tank_h / 2.0;


  // (2) Dimensions of atmosphere in the Detector

  const double atm_dz = Tank_h / 2.0 - Tank_w;
  const double atm_r  = Tank_d / 2.0 - Tank_w;


  // (3) Dimensions of the Scintillator

  const double scintdown_dz =               Hbuffer_scint / 2.0;
  const double scintup_dz   = ( Scint_h - Hbuffer_scint ) / 2.0;
  const double scintdown_r  =   ( Tank_d - 2.0 * Tank_w ) / 2.0;
  const double scintup_rmin =                    Buffer_d / 2.0;
  const double scintup_rmax =   ( Tank_d - 2.0 * Tank_w ) / 2.0;

  
  // (4) Dimensions of buffer

  const double buffer_dz = Buffer_h / 2.0;
  const double buffer_r  = Buffer_d / 2.0;


  // (5) Dimmensions of PM ans its wrapping and window entrance

  const double pmwrap_dz =              PM_h / 2.0;
  const double pmwrap_r  =              PM_d / 2.0;
  const double pmwin_dz  =               0.4 / 2.0;
  const double pmwin_r   = ( PM_d - 2.0*0.2) / 2.0; // 0.2 is width of wrapping around PM
  const double pm_dz     = ( PM_h - 2.0*0.4) / 2.0; // thickness of window of PM is 0.4
  const double pm_r      = ( PM_d - 2.0*0.2) / 2.0;


  // (6) Dimensions of World

  const double top_dx = Tank_d;
  const double top_dy = Tank_d;
  const double top_dz = Tank_h;


/*------------------------------------------------------------------------------------------------------------*/
/* Positionning (translations) :                                                                              */
/*------------------------------------------------------------------------------------------------------------*/


  const double z0 = - ( Tank_h - 2.0*Tank_w ) / 2.0;


  // (1) Positionning of Base under the Scintillator

  const double t_base = z0 - base_dz;
  TGeoTranslation *t1 = new TGeoTranslation("t1",0.0,0.0,t_base);
  t1->RegisterYourself();


  // (2) Positionning of Buffer into the Atmosphere of Tank

  const double t_buffer = z0 + Hbuffer_scint + buffer_dz;
  TGeoTranslation *t2 = new TGeoTranslation("t2",0.0,0.0,t_buffer);
  t2->RegisterYourself();


  // (3) Positionning of Scintillator into the Tank

  const double t_scint_down = z0 + scintdown_dz;
  TGeoTranslation *t3 = new TGeoTranslation("t3",0.0,0.0,t_scint_down);
  t3->RegisterYourself();

  const double t_scint_up = z0 + 2.0*scintdown_dz + scintup_dz;
  TGeoTranslation *t4 = new TGeoTranslation("t4",0.0,0.0,t_scint_up);
  t4->RegisterYourself();


  // (1) Positionning of PM and PMwin inside PMrev

  const double t_PM = - pmwrap_dz + 2.0*pmwin_dz + pm_dz;
  TGeoTranslation *t5 = new TGeoTranslation("t5",0.0,0.0,t_PM);
  t5->RegisterYourself();

  const double t_PMwin = - pmwrap_dz + pmwin_dz;
  TGeoTranslation *t6 = new TGeoTranslation("t6",0.0,0.0,t_PMwin);
  t6->RegisterYourself();


  // (5) Positionning of PMs above the Buffer

  const double t_PM_z = z0 + Hbuffer_scint + 2.0*buffer_dz + pmwrap_dz;
  const double t_PM_x[16] = { 15.37,- 5.87,-19.00,- 5.87, 15.37, 44.00, 37.02, 18.28,- 6.26,-28.81,-42.22,-42.22,-28.81,- 6.26, 18.28, 37.02};
  const double t_PM_y[16] = { 11.17, 18.07,   0.0,-18.07,-11.17,   0.0, 23.79, 40.02, 43.55, 33.25, 12.40,-12.40,-33.25,-43.55,-40.02,-23.79};

  TGeoTranslation *tPM[16];
  for (int i=0; i<16; i++) {
    tPM[i] = new TGeoTranslation(Form("tPM %d",i),t_PM_x[i],t_PM_y[i],t_PM_z);
    tPM[i]->RegisterYourself();
  }


/*------------------------------------------------------------------------------------------------------------*/
/* Volumes and Nodes :                                                                                        */
/*------------------------------------------------------------------------------------------------------------*/


  // (1) top box containing everything and defining the WCS, world coordinate system

  TGeoVolume *top = geom->MakeBox("Top",n2,top_dx,top_dy,top_dz);
  geom->SetTopVolume(top);


  // (2) TGeoTube tank (is a wrapping)

  if (l==1) {
    TGeoVolume *Tank = geom->MakeTube("Tank",totabsorbing,0.0,tank_r,tank_dz);
    top->AddNode(Tank,1);
  }

  if (l==2) {
    TGeoVolume *Tank = geom->MakeTube("Tank",totabsorbing,0.0,tank_r,tank_dz);
    top->AddNode(Tank,1);
    TGeoVolume *Tank2 = geom->MakeTube("Tank2",teflon,atm_r,(tank_r+atm_r)/2.0,tank_dz);
    Tank->AddNode(Tank2,1);
  }

  if (l==3) {
    TGeoVolume *Tank = geom->MakeTube("Tank",mirror,0.0,tank_r,tank_dz);
    top->AddNode(Tank,1);
    TGeoVolume *Base = geom->MakeTube("Base",teflon,0.0,tank_r,base_dz);
    Tank->AddNode(Base,1,t0);
  }

  if (l==4) {
    TGeoVolume *Tank = geom->MakeTube("Tank",mirror,0.0,tank_r,tank_dz);
    top->AddNode(Tank,1);
    TGeoVolume *Base = geom->MakeTube("Base",teflon,0.0,tank_r,base_dz);
    Tank->AddNode(Base,1,t0);
  }

  if (l==5) {
    TGeoVolume *Tank = geom->MakeTube("Tank",steel,0.0,tank_r,tank_dz);
    top->AddNode(Tank,1);
    TGeoVolume *Base = geom->MakeTube("Base",teflon,0.0,tank_r,base_dz);
    Tank->AddNode(Base,1,t0);
  }


  // (3) TGeoTube Atmosphere of Nitrogen in the Tank

  TGeoVolume *Atm =geom->MakeTube("Atm",n2,0.0,atm_r,atm_dz);
  Tank->AddNode(Atm,1);

 
  // (4) TGeoTube buffer
  
  TGeoVolume *Buffer = geom->MakeTube("Buffer",acrylic,0.0,buffer_r,buffer_dz);
  Atm->AddNode(Buffer,1,t2);


  // (5) TGeoTube Scintillator

  TGeoVolume *Scintdown =geom->MakeTube("Scintdown",liquid,0.0,scintdown_r,scintdown_dz);
  Atm->AddNode(Scintdown,1,t3);

  TGeoVolume *Scintup =geom->MakeTube("Scintup",liquid,scintup_rmin,scintup_rmax,scintup_dz);
  Atm->AddNode(Scintup,1,t4);

  TLitVolume *Lit_Scintdown = new TLitVolume(Scintdown);
  TLitVolume *Lit_Scintup = new TLitVolume(Scintup);
  

  // (6) TGeoTube PhotoTube

  TGeoVolume *PMwrap = geom->MakeTube("PMwrap",wrappm,0.0,pmwrap_r,pmwrap_dz);
  TGeoVolume *PMwin  = geom->MakeTube("PMwin",sodocal,0.0,pmwin_r,pmwin_dz);
  TGeoVolume *PM     = geom->MakeTube("PM",cathode,0.0,pm_r,pm_dz);
  PMwrap->AddNode(PM,1,t5);
  PMwrap->AddNode(PMwin,1,t6);
  for (int i=0; i<16; i++) { Atm->AddNode(PMwrap,(i+1),tPM[i]); }

  TLitVolume *Lit_PM = new TLitVolume(PM);
  Lit_PM->SetPhotoTube("QuantumEff_XP2020",1.0);


/*------------------------------------------------------------------------------------------------------------*/
/* End of Builing the Geometry                                                                                */
/*------------------------------------------------------------------------------------------------------------*/


  geom->CloseGeometry();
  geom->CheckOverlaps(0.01);


/*------------------------------------------------------------------------------------------------------------*/
/* Color and Drawing                                                                                          */
/*------------------------------------------------------------------------------------------------------------*/


  const Color_t color_top    =  0;
  const Color_t color_tank   =  1;
  const Color_t color_atm    = 10;
  const Color_t color_scint  =  7;
  const Color_t color_buffer =  5;
  const Color_t color_pm     =  4;
  const Color_t color_pm_win =  4;
  const Color_t color_pm_rev =  4;

  top->SetVisibility(kFALSE);
  top->SetVisContainers();
  top->SetLineColor(color_top);
  top->SetLineWidth(1);

  Tank->SetVisibility(kTRUE);
  Tank->SetFillColor(color_tank);
  Tank->SetLineColor(color_tank);
  Tank->SetLineWidth(1);

  Atm->SetVisibility(kTRUE);
  Atm->SetFillColor(color_atm);
  Atm->SetLineColor(color_atm);
  Atm->SetLineWidth(1);

  Buffer->SetVisibility(kTRUE);
  Buffer->SetFillColor(color_buffer);
  Buffer->SetLineColor(color_buffer);
  Buffer->SetLineWidth(1);

  Scintdown->SetVisibility(kTRUE);
  Scintdown->SetFillColor(color_scint);
  Scintdown->SetLineColor(color_scint);
  Scintdown->SetLineWidth(1);

  Scintup->SetVisibility(kTRUE);
  Scintup->SetFillColor(color_scint);
  Scintup->SetLineColor(color_scint);
  Scintup->SetLineWidth(1);

  PMwrap->SetVisibility(kTRUE);
  PMwrap->SetFillColor(color_pm_rev);
  PMwrap->SetLineColor(color_pm_rev);
  PMwrap->SetLineWidth(1);

  PMwin->SetVisibility(kTRUE);
  PMwin->SetFillColor(color_pm_win);
  PMwin->SetLineColor(color_pm_win);
  PMwin->SetLineWidth(1);

  PM->SetVisibility(kTRUE);
  PM->SetFillColor(color_pm);
  PM->SetLineColor(color_pm);
  PM->SetLineWidth(1);

  geom->SetTopVisible(0);
  geom->SetVisLevel(4);
//  geom->SetVisOption(3);


/*------------------------------------------------------------------------------------------------------------*/
/* Mesure of Efficiency                                                                                       */
/*------------------------------------------------------------------------------------------------------------*/


  if (k==1) {

  // (1) Initialisation des variables

    // Variables of emission
  double nb_photon = 5000.0;
  double Xsource = 0.0, Zsource = 0.0;
  int run = 0, i = 0, j = 0;
    // Variables to find and save Efficiency and its Error
  double a = 0.0, nb_seen   = 0.0;
  double b = 0.0, nb_gener  = 0.0;
  double c = 0.0, nb_abnorm = 0.0;
  double d = 0.0, nb_l0     = 0.0;
  double eff = 0.0, err = 0.0, max_err = 0.0;
  int binsX = 10, binsZ = 10;
  double Xlow = - scintdown_r, Xup = scintdown_r, Zlow = - Hbuffer_scint / 2.0, Zup = Hbuffer_scint / 2.0;
  TH2D *Eff = new TH2D("Efficiency","Efficiency",binsX,Xlow,Xup,binsZ,Zlow,Zup);


  // (2) Computation of Efficiency

  for (i=0; i<binsX; i++) 
  {
    for (j=0; j<binsZ; j++) 
    {
      // Parameters
      Xsource = Xlow + (Xup-Xlow) * (2*i+1) / (2*binsX);
      Zsource = Zlow + (Zup-Zlow) * (2*j+1) / (2*binsZ);
      TVector3 emisdir(0.0,0.0,1.0);
      TVector3 facedir(0.0,0.0,0.0);
      TVector3 source(Xsource,0.0,Zsource);

      // Generation of photons in position(i;j)
      run++;
      TLitSpontan *emission = new TLitSpontan("Emission","Emission","/Top_1/Tank_1/Atm_1/Scintdown_1");
      emission->SetEmission(flat,180.0,emisdir,"",kTRUE,source,kFALSE,facedir);
      emission->Gen(run,nb_photon);
      emission->TrackToDraw(1);		// Why is it used?

      // Look for Results to calculate Efficiency and its Error
      // Fonctions are described in <path>/SLitrani/SLitrani/TLitResults.h and .cpp
      // We had to close file in order to open "Simul_Nucifer.root"
      TLit::Get()->CloseFiles();
      TFile *f = new TFile("Simul_Nucifer2.root");
      gLitGs = (TLitResults *)f->Get("GlobStat;1");

      // Calculation of Efficiency
      nb_seen   = gLitGs->fNpSeen  ;	nb_seen   =   nb_seen - a;	a = gLitGs->fNpSeen  ;
      nb_gener  = gLitGs->fNpGener ;	nb_gener  =  nb_gener - b;	b = gLitGs->fNpGener ;
      nb_abnorm = gLitGs->fNpAbnorm;	nb_abnorm = nb_abnorm - c;	c = gLitGs->fNpAbnorm;
      nb_l0     = gLitGs->fNpAbsL0 ;	nb_l0     =     nb_l0 - d;	d = gLitGs->fNpAbsL0 ; 
      eff = nb_seen / (nb_gener - nb_abnorm - nb_l0);
      err = TMath::Sqrt( eff * (1.0 - eff) / (nb_gener - nb_abnorm - nb_l0) );
      if ( max_err<err ) { max_err = err; }
      Eff->SetBinContent(i+1,j+1,100*eff);
      f->Close();
    } 
  }


  // (3) Data Saving

  TFile *Efficiency = new TFile("/home/cyril/Code_SLitrani/Histograms/Efficiency_totabs_20x20.root","RECREATE");
  Eff->Write();
  Efficiency->Close();


  // (4) Drawing Results

  TLit::Get()->BookCanvas();
  gTwoPad->SetAllGrey();
  gTwoPad->CommentTL("Simulation of PSD in Nucifer");
  top->Draw("");
  top->Draw("x3d");
  gLitGp->Summary();
  gTwoPad->ChangePad();
  gLitGs->DoStat();
  TLit::Get()->CloseFiles();
  gStyle->SetPalette(1);
  TCanvas *c1 = new TCanvas("c1","Efficiency",10,10,700,500);
  Eff->SetTitle(Form("Efficiency ( % ), Error Max : %.3lf %",100*max_err));
  Eff->Draw("colz");

  }


/*------------------------------------------------------------------------------------------------------------*/
/* Pulse Shape Discrimination                                                                                 */
/*------------------------------------------------------------------------------------------------------------*/


  if (k==2) {

  // (1) Initialisation des variables

  const double Rmax = scintdown_r;
  const double Zlow = - Hbuffer_scint / 2.0, Zup = Hbuffer_scint / 2.0;
  
  TRandom3 *aleaE = new TRandom3(); 
  TRandom3 *aleaN = new TRandom3();
  TRandom3 *aleaR = new TRandom3(); 
  TRandom3 *aleaT = new TRandom3();
  TRandom3 *aleaZ = new TRandom3();

  int run = 0;
//   TH1D *hE = new TH1D("","",100,0.,10.);
//   TH1D *hN = new TH1D("","",1000,15000.,8000.);
//   TH1D *hR = new TH1D("","",100,-Rmax,2*Rmax);
//   TH1D *hT = new TH1D("","",400,0.,400.);	hT->Fill(T);

  // (2) Generation of Events

  for (int i=0; i<10; i++) {

    // Creation of Energy, Number of Photon created and Position of Event
    double E         = aleaE->Uniform(1.0,8.0);
    double nb_photon = aleaN->Gaus(E*6000,sqrt(E*6000));
    double R         = aleaR->Uniform(0.0,Rmax);
    double Theta     = aleaT->Uniform(0.0,360.0);
    double Z         = aleaZ->Uniform(Zlow,Zup);
    double X = R * TMath::Cos(Theta);
    double Y = R * TMath::Sin(Theta);
    TVector3 emisdir(0.0,0.0,1.0);
    TVector3 facedir(0.0,0.0,0.0);
    TVector3 source(X,Y,Z);

    // Generation of photons in position(X;Y;Z)
    TLitSpontan *emission = new TLitSpontan("Emission","Emission","/Top_1/Tank_1/Atm_1/Scintdown_1");
    emission->SetEmission(flat,180.0,emisdir,"",kTRUE,source,kFALSE,facedir);
    run++;
    emission->Gen(run,nb_photon);
    emission->TrackToDraw(1);		// Why is it used?

    }
  

  // (3) Search for Histogram of Time of Arrival in PM and Save it

  TLit::Get()->CloseFiles();
  TFile *f = new TFile("Simul_Nucifer2.root");
  TLit::Get()->CloseFiles();
  TH1D *TimeSeen = new TH1D;
  TimeSeen = gLitGs->fHTimeSeen;
  TCanvas *c0 = new TCanvas("c0","Photon Detected",10,10,700,500);
  TimeSeen->Draw("");


  // (4) Reconstruction of Signal of PMs



  }


/*------------------------------------------------------------------------------------------------------------*/
/* End of Macro                                                                                               */
/*------------------------------------------------------------------------------------------------------------*/


  return 0;


}



