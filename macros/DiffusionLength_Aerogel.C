TSplineFit* DiffusionLength_Aerogel(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//
// Arguments:
//
//  todraw       : true if the fit is to be drawn
//  infile       : true if the fit is to be put in file SplineFitDB.rdb
//  firstinfile  : if true, [BE CAREFUL: RISK OF LOOSING FILE SplineFitDB.rdb ]
//                 delete all fits present in file SplineFitDB.rdb and place
//                 this fit as the first fit in a new file SplineFitDB.rdb
//                 default false !!!
// All defaults for arguments correspond to the case where one calls this CINT
//macro from within a Litrani CINT macro to define a needed fit, instead of trying
//to find it in the database file SplineFitDB.rdb using TSplineFit::FindFit()
//
//
//  DiffusionLength_Aerogel
//
//  Source : Knez_Luka for his CINT macro Luka_detector.C
//  pofitan T(lambda) --->parametra a in b
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 61;
  const Int_t m = 2;
  Int_t i;
  Double_t sig[M];
  TSplineFit *DiffLAero;
  Double_t x[M] = {
    200,
    210,
    220,
    230,
    240,
    250,
    260,
    270,
    280,
    290,
    300,
    310,
    320,
    330,
    340,
    350,
    360,
    370,
    380,
    390,
    400,
    410,
    420,
    430,
    440,
    450,
    460,
    470,
    480,
    490,
    500,
    510,
    520,
    530,
    540,
    550,
    560,
    570,
    580,
    590,
    600,
    610,
    620,
    630,
    640,
    650,
    660,
    670,
    680,
    690,
    700,
    710,
    720,
    730,
    740,
    750,
    760,
    770,
    780,
    790,
    800
  };
  Double_t y[M] = { 
    0.1680672268908,
    0.2042867647059,
    0.2460672268908,
    0.2939506302521,
    0.3485042016807,
    0.4103203781513,
    0.4800168067227,
    0.5582363445378,
    0.6456470588235,
    0.7429422268908,
    0.8508403361345,
    0.9700850840336,
    1.101445378151,
    1.245715336134,
    1.403714285714,
    1.576286764706,
    1.764302521008,
    1.968656512605,
    2.190268907563,
    2.430085084034,
    2.689075630252,
    2.968236344538,
    3.268588235294,
    3.591177521008,
    3.937075630252,
    4.307379201681,
    4.703210084034,
    5.125715336134,
    5.576067226891,
    6.055463235294,
    6.56512605042,
    7.106303571429,
    7.680268907563,
    8.288320378151,
    8.931781512605,
    9.61200105042,
    10.33035294118,
    11.08823634454,
    11.88707563025,
    12.72832037815,
    13.61344537815,
    14.54395063025,
    15.52136134454,
    16.54722794118,
    17.62312605042,
    18.75065651261,
    19.93144537815,
    21.16714390756,
    22.45942857143,
    23.81000105042,
    25.22058823529,
    26.69294222689,
    28.22884033613,
    29.83008508403,
    31.49850420168,
    33.23595063025,
    35.04430252101,
    36.92546323529,
    38.88136134454,
    40.91395063025,
    43.02521008403
  };

  for (i=0;i<M;i++) sig[i] = 0.005;
  DiffLAero = new TSplineFit("DiffusionLength_Aerogel","Diffusion Length | Aerogel of Kaos Spectrometer",
    7,M,m,x,y,sig,kTRUE,0.0,kFALSE,3500,299.0,800.01);
  DiffLAero->SetSource("Knez Luka for Kaos Spectrometer");
  DiffLAero->SetMacro("DiffusionLength_Aerogel.C");
  DiffLAero->SetXLabel("wavelength [nm]");
  DiffLAero->SetVLabel("Diffusion length [cm]");
  if (todraw) {
    DiffLAero->SetDefaultLabels();
    DiffLAero->DrawFit();
    DiffLAero->Print();
  }
  if (infile) {
    if (firstinfile) DiffLAero->UpdateFile(kTRUE);
    else             DiffLAero->UpdateFile(kFALSE);
  }
  return DiffLAero;
}
