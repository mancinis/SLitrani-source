TSplineFit* QuantumEff_Hamamatsu(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   QuantumEff_Hamamatsu
//
//   Quantum efficiency of the photocathode of the Hamamatsu Phototube 
//
//   Source : Hamamatsu
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 37;
  const Int_t m = 5;
  Int_t i;
  Double_t sig[M];
  TSplineFit *PhotoTube;
  Double_t x[M] = {295.02404,
    294.73113,
    296.17699,
    304.42219,
    312.73593,
    326.08529,
    347.89797,
    368.05912,
    393.29327,
    420.24127,
    443.83013,
    465.74252,
    487.67361,
    504.56286,
    523.15349,
    540.06766,
    555.30538,
    567.18394,
    577.36111,
    587.55075,
    599.42931,
    607.96741,
    619.82728,
    623.28615,
    633.50071,
    642.03882,
    650.57069,
    657.4261,
    664.28152,
    672.83832,
    679.6875,
    686.55538,
    691.7281,
    698.59598,
    705.46385,
    708.92895,
    712.40028 };
  Double_t y[M] = { 
    0.0158489,
    0.0353305,
    0.0675507,
    0.1070607,
    0.1406533,
    0.1911965,
    0.2267566,
    0.2469392,
    0.2511886,
    0.2346227,
    0.2154418,
    0.1944867,
    0.1668092,
    0.1406533,
    0.1126808,
    0.0887462,
    0.0687132,
    0.0523022,
    0.0419006,
    0.0324422,
    0.0246939,
    0.0175566,
    0.0140653,
    0.0108903,
    0.0078759,
    0.0055995,
    0.0040496,
    0.0028791,
    0.002047,
    0.0013827,
    0.001,
    0.0006871,
    0.0004885,
    0.0003357,
    0.0002307,
    0.0001756,
    0.0001314
  };
  for (i=0;i<M;i++) sig[i] = 0.005;
  PhotoTube = new TSplineFit("QuantumEff_Hamamatsu","Quantum efficiency | Hamamatsu",
    10,M,m,x,y,sig,kTRUE,0.0,kFALSE,0.281,300.0,725.0);
  PhotoTube->SetSource("Hamamatsu documentation");
  PhotoTube->SetMacro("QuantumEff_Hamamatsu.C");
  PhotoTube->SetXLabel("wavelength [nm]");
  PhotoTube->SetVLabel("quantum efficiency");
  if (todraw) {
    PhotoTube->SetDefaultLabels();
    PhotoTube->DrawFit();
    PhotoTube->Print();
  }
  if (infile) {
    if (firstinfile) PhotoTube->UpdateFile(kTRUE);
    else             PhotoTube->UpdateFile(kFALSE);
  }
  return PhotoTube;
}
