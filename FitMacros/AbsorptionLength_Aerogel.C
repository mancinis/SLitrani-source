TSplineFit* AbsorptionLength_Aerogel(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_Aerogel
//
//  Source : Knez_Luka for his CINT macro Luka_detector.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 61;
  const Int_t m = 2;
  Int_t i;
  Double_t sig[M];
  TSplineFit *LAbso;
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
    0.2133333333333,
    0.315190494675,
    0.4572989461333,
    0.6525915440083,
    0.9172942848,
    1.271565755208,
    1.740225538133,
    2.353579470675,
    3.148349986133,
    4.168720108008,
    5.4675,
    7.107425312008,
    9.162596898133,
    11.72007181868,
    14.88161587413,
    18.76562825521,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69,
    18.69
  };

  for (i=0;i<M;i++) sig[i] = 0.005;
  LAbso = new TSplineFit("AbsorptionLength_Aerogel","Absorption Length | Aerogel of Kaos Spectrometer",
    5,M,m,x,y,sig,kTRUE,0.0,kFALSE,3500,299.0,800.01);
  LAbso->SetSource("Knez Luka for Kaos Spectrometer");
  LAbso->SetMacro("AbsorptionLength_Aerogel.C");
  LAbso->SetXLabel("wavelength [nm]");
  LAbso->SetVLabel("absorbtion length [cm]");
  if (todraw) {
    LAbso->SetDefaultLabels();
    LAbso->DrawFit();
    LAbso->Print();
  }
  if (infile) {
    if (firstinfile) LAbso->UpdateFile(kTRUE);
    else             LAbso->UpdateFile(kFALSE);
  }
  return LAbso;
}
