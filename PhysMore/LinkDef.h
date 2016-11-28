#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TArrayZ+;
#pragma link C++ class TNArray<Char_t>+;
#pragma link C++ class TNArray<Short_t>+;
#pragma link C++ class TNArray<Int_t>+;
#pragma link C++ class TNArray<Long_t>+;
#pragma link C++ class TNArray<Long64_t>+;
#pragma link C++ class TNArray<Float_t>+;
#pragma link C++ class TNArray<Double_t>+;
#pragma link C++ class TNArray<TComplex>+;
#pragma link C++ class TCleanOut+;
#pragma link C++ class TMat33+;
#pragma link C++ class TEuler+;
#pragma link C++ class TVec3C+;
#pragma link C++ class TMat33C+;

#pragma link C++ typedef TNArrayC;
#pragma link C++ typedef TNArrayS;
#pragma link C++ typedef TNArrayI;
#pragma link C++ typedef TNArrayL;
#pragma link C++ typedef TNArrayL64;
#pragma link C++ typedef TNArrayF;
#pragma link C++ typedef TNArrayD;
#pragma link C++ typedef TNArrayZ;

#pragma link C++ enum KindOfMessage;
#pragma link C++ global gCleanOut;

//operators for Arrays
#pragma link C++ function operator+(const TArrayZ&,const TArrayZ&);
#pragma link C++ function operator-(const TArrayZ&,const TArrayZ&);
#pragma link C++ function operator*(const TArrayZ&,const TArrayZ&);
#pragma link C++ function operator<<(ostream&,const TArrayZ&);


#pragma link C++ function operator+(const TNArrayC&,const TNArrayC&);
#pragma link C++ function operator+(const TNArrayS&,const TNArrayS&);
#pragma link C++ function operator+(const TNArrayI&,const TNArrayI&);
#pragma link C++ function operator+(const TNArrayL&,const TNArrayL&);
#pragma link C++ function operator+(const TNArrayL64&,const TNArrayL64&);
#pragma link C++ function operator+(const TNArrayF&,const TNArrayF&);
#pragma link C++ function operator+(const TNArrayD&,const TNArrayD&);
#pragma link C++ function operator+(const TNArrayZ&,const TNArrayZ&);

#pragma link C++ function operator-(const TNArrayC&,const TNArrayC&);
#pragma link C++ function operator-(const TNArrayS&,const TNArrayS&);
#pragma link C++ function operator-(const TNArrayI&,const TNArrayI&);
#pragma link C++ function operator-(const TNArrayL&,const TNArrayL&);
#pragma link C++ function operator-(const TNArrayL64&,const TNArrayL64&);
#pragma link C++ function operator-(const TNArrayF&,const TNArrayF&);
#pragma link C++ function operator-(const TNArrayD&,const TNArrayD&);
#pragma link C++ function operator-(const TNArrayZ&,const TNArrayZ&);

#pragma link C++ function operator*(const TNArrayC&,const TNArrayC&);
#pragma link C++ function operator*(const TNArrayS&,const TNArrayS&);
#pragma link C++ function operator*(const TNArrayI&,const TNArrayI&);
#pragma link C++ function operator*(const TNArrayL&,const TNArrayL&);
#pragma link C++ function operator*(const TNArrayL64&,const TNArrayL64&);
#pragma link C++ function operator*(const TNArrayF&,const TNArrayF&);
#pragma link C++ function operator*(const TNArrayD&,const TNArrayD&);
#pragma link C++ function operator*(const TNArrayZ&,const TNArrayZ&);

#pragma link C++ function operator<<(ostream&,const TNArrayC&);
#pragma link C++ function operator<<(ostream&,const TNArrayS&);
#pragma link C++ function operator<<(ostream&,const TNArrayI&);
#pragma link C++ function operator<<(ostream&,const TNArrayL&);
#pragma link C++ function operator<<(ostream&,const TNArrayL64&);
#pragma link C++ function operator<<(ostream&,const TNArrayF&);
#pragma link C++ function operator<<(ostream&,const TNArrayD&);
#pragma link C++ function operator<<(ostream&,const TNArrayZ&);

//operators for TVector3
#pragma link C++ function operator^(const TVector3&,const TVector3&);
//operators for TMat33
#pragma link C++ function operator+(Double_t,const TMat33&);
#pragma link C++ function operator-(Double_t,const TMat33&);
#pragma link C++ function operator*(Double_t,const TMat33&);
#pragma link C++ function operator*(const TMat33&,const TVector3&);
#pragma link C++ function operator*(const TMat33&,const TVec3C&);
#pragma link C++ function operator*(const TVector3&,const TMat33&);
#pragma link C++ function operator*(const TVec3C&,const TMat33&);
#pragma link C++ function operator&(Double_t,const TMat33&);
//operators for TVec3C
#pragma link C++ function operator+(const TVector3&, const TVec3C&);
#pragma link C++ function operator-(const TVector3&, const TVec3C&);
#pragma link C++ function operator*(Double_t, const TVec3C&);
#pragma link C++ function operator*(const TComplex&, const TVec3C&);
#pragma link C++ function operator*(const TVector3&, const TVec3C&);
#pragma link C++ function operator&(const TVector3&, const TVec3C&);
#pragma link C++ function operator<<(ostream&, const TVec3C&);
#pragma link C++ function operator>>(istream&, TVec3C&);
#pragma link C++ function operator^(const TVector3&, const TVec3C&);
//operators for TMat33C
#pragma link C++ function operator+(Double_t,const TMat33C&);
#pragma link C++ function operator+(TComplex,const TMat33C&);
#pragma link C++ function operator-(Double_t,const TMat33C&);
#pragma link C++ function operator-(TComplex,const TMat33C&);
#pragma link C++ function operator*(Double_t,const TMat33C&);
#pragma link C++ function operator*(TComplex,const TMat33C&);
#pragma link C++ function operator*(const TMat33&,const TMat33C&);
#pragma link C++ function operator*(const TMat33C&,const TVector3&);
#pragma link C++ function operator*(const TMat33C&,const TVec3C&);
#pragma link C++ function operator*(const TVector3&,const TMat33C&);
#pragma link C++ function operator*(const TVec3C&,const TMat33C&);
#pragma link C++ function operator&(Double_t,const TMat33C&);
#pragma link C++ function operator&(TComplex,const TMat33C&);
#pragma link C++ function operator&(const TMat33&,const TMat33C&);

#endif
