TSplineFit* DEdx_MusInPbWO4NoL(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  DEdx_MusInPbWO4NoL
//
//  Interpolation giving the value of dEdx [Mev] in 1 cm of PbWO4, WITHOUT Landau fluctuation
//as a function of the energy of the muon E in Gev.
//
//  Old f_dedx.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 8;
  TString ss1,ss2,ss3,ss4;
  Int_t i;
  TSplineFit::fgNChanRand      = 100000;
  TSplineFit *DEdx;
  Double_t x[M]= {    0.01,    0.05,   0.08,   0.1,    1.0,   10.0,  100.0, 1000.0};
  Double_t y[M]= {    10.0,   50.0,    8.0,    6.0,    8.0,    8.25,   8.5,    8.6};
  DEdx = new TSplineFit("DEdx_MusInPbWO4NoL","Energy deposited by crossing | Muons in PbWO4 No Landau",
    11,M,x,y,0.01,1000.0);
  DEdx->SetSource("Calculated with GEANT by A.Givernaud DSM/DAPNIS/SPP CEA Saclay");
  DEdx->SetMacro("DEdx_MusInPbWO4NoL.C");
  DEdx->SetXLabel("deposited energy [Mev]");
  DEdx->SetVLabel("Nb. of hits according to GEANT 3");
  DEdx->UseForRandom();
  if (todraw) {
    DEdx->SetDefaultLabels();
    DEdx->DrawFit();
    DEdx->Print();
  }
  if (infile) {
    if (firstinfile) DEdx->UpdateFile(kTRUE);
    else             DEdx->UpdateFile(kFALSE);
  }
  return DEdx;
}
