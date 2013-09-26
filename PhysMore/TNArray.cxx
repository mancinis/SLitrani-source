// @(#)root/html:$Id: TNArray.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TNArray.h"

ClassImp(TNArray<Char_t>)
ClassImp(TNArray<Short_t>)
ClassImp(TNArray<Int_t>)
ClassImp(TNArray<Long_t>)
ClassImp(TNArray<Long64_t>)
ClassImp(TNArray<Float_t>)
ClassImp(TNArray<Double_t>)
ClassImp(TNArray<TComplex>)
//______________________________________________________________________________
//
//     TNArray  Multidimensional arrays of any base type
//
//   Up to dimension 6
//   Book one place more for overflow
//   Detects overflow. Overflow has happened if method GetOverFlow() does not
//returns 0.
//   Access to the values using parentheses, like A(i1,i2,i3), not brackets []
//   TNArray uses templates to be able to make multi-dimensional arrays of
//
//     - Char_t    ==> typedef TNArrayC
//     - Short_t   ==> typedef TNArrayS
//     - Int_t     ==> typedef TNArrayI
//     - Long_t    ==> typedef TNArrayL
//     - Long64_t  ==> typedef TNArrayL64
//     - Float_t   ==> typedef TNArrayF
//     - Double_t  ==> typedef TNArrayD
//     - TComplex  ==> typedef TNArrayZ
//
//   TNArray uses typedef in order to hide the template to the user. For instance,
//for booking a 2 dimensional array of double, use:
//     TNArrayD d(2,3);      //simpler than TNArray<Double_t> d(2,3);
//     d(0,1) = 1.2;         //to set values
//     Double_t a = d(0,1);  //to get a value
//
//   Methods "ReSet" redefines the array completely, all old values are lost
//   Methods "Set" allows to change sizes [fN[0],fN[1],...fN[5]], but not the
//dimensions fNd of the array. Preserves the maximum possible from the
//old values.
//   Method Print() to print the array. Operator << can also be used for
//printing the array with cout. It also works within CINT.
//   TNArray derives from TObject. They can be put into collections or written
//to a ROOT file. TNArray are sortable according to their norm, calculated
//as if they were 1-dimensional.
//   TNArray can be added or subtracted. They can be multiplied or divided
//by a number, etc. A scalar product is defined. Look at all the available
//operators.
//
template <typename TT>
TNArray<TT>::TNArray(const TNArray<TT> &orig) {
  //copy constructor
  Short_t k;
  fNd = orig.fNd;
  fN  = new UInt_t [fNd];
  for (k=0;k<fNd;k++) fN[k] = orig.fN[k];
  fNL = orig.fNL;
  fA  = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = orig.fA[i];
}
template <typename TT>
TNArray<TT>::TNArray(Int_t Nd,UInt_t *N) {
  // Constructor for a Nd dimensional array. Size of the Nd dimensions are
  //in array N
  if ((Nd<1) || (Nd>6)) {
    Error("TNArray","Dimension has to be >=1 and <= 6");
    Init();
  }
  else {
    const TT zero = 0;
    Short_t k;
    fNd  = Nd;
    fN   = new UInt_t [fNd];
    fNL  = 1;
    for (k=0;k<fNd;k++) {
      fN[k] = N[k];
      fNL  *= fN[k];
    }
    fNL++;
    fA    = new TT [fNL];
    for (Int_t i=0;i<fNL;i++) fA[i] = zero;
  }
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1) {
  //constructor for a 1 dimensional array of size n1. Array is put to 0
  const TT zero = 0;
  fNd   = 1;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fNL   = n1 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1,Int_t n2,Bool_t allsamesize) {
  // If allsamesize is false, [default], then this is the constructor for a
  //2 dimensional array.
  //  n1 is the size of the 1st dimension
  //  n2 is the size of the 2nd dimension
  // If allsamesize is true, then this is the constructor for a n1 dimensional
  //array, the size of all dimensions being all equal to n2
  //
  // Array is put to 0
  //
  const TT zero = 0;
  if (allsamesize) {
    if ((n1<1) || (n1>6)) {
      Error("TNArray","Dimension has to be >=1 and <= 6");
      Init();
    }
    else {
      Short_t k;
      fNd = n1;
      fN  = new UInt_t [fNd];
      fNL = 1;
      for (k=0;k<fNd;k++) {
        fN[k] = n2;
        fNL  *= n2;
      }
      fNL++;
    }
  }
  else {
    fNd   = 2;
    fN    = new UInt_t [fNd];
    fN[0] = n1;
    fN[1] = n2;
    fNL   = n1*n2 + 1;
  }
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1,Int_t n2,Int_t n3) {
  //constructor 3 dimensional array of sizes n1,n2,n3. Array is put to 0
  const TT zero = 0;
  fNd   = 3;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fNL   = n1*n2*n3 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1,Int_t n2,Int_t n3,Int_t n4) {
  //constructor for a 4 dimensional array of sizes n1,n2,n3,n4. Array is put to 0
  const TT zero = 0;
  fNd   = 4;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fNL   = n1*n2*n3*n4 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5) {
  //constructor for a 5 dimensional array of sizes n1,n2,n3,n4,n5. Array is put to 0
  const TT zero = 0;
  fNd   = 5;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fN[4] = n5;
  fNL   = n1*n2*n3*n4*n5 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::TNArray(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5,Int_t n6) {
  //constructor for a 6 dimensional array of sizes n1,n2,n3,n4,n5,n6. Array is put to 0
  const TT zero = 0;
  fNd   = 6;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fN[4] = n5;
  fN[5] = n6;
  fNL   = n1*n2*n3*n4*n5*n6 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
TNArray<TT>::~TNArray() {
  //destructor
  Clean();
}
template <typename TT>
Double_t TNArray<TT>::Biggest() const {
  // returns the biggest element in absolute value in form of a double
  Int_t i;
  Double_t d;
  Double_t dmax = 0.0;
  TComplex z;
  for (i=0;i<fNL-1;i++) {
    z = fA[i];
    d = TComplex::Abs(z);
    if (d>dmax) dmax = d;
  }
  return dmax;
}
template <typename TT>
void TNArray<TT>::Clean() {
  //
  if (fA) delete [] fA;
  if (fN) delete [] fN;
  Init();
}
template <typename TT>
Int_t TNArray<TT>::Compare(const TObject* obj) const {
  // Compare according to norm
  Int_t k;
  Double_t num1, num2;
  TNArray<TT> *p;
  p = (TNArray<TT> *)obj;
  num1 = Norm();
  num2 = p->Norm();
  if (num1<num2) k=-1;
  else {
    if (num1>num2) k=1;
    else k=0;
  }
  return k;
}
template <typename TT>
void TNArray<TT>::Copy(const TNArray<TT> &orig) {
  //copy orig into this
  Short_t k;
  Clean();
  fNd = orig.fNd;
  fN  = new UInt_t [fNd];
  for (k=0;k<fNd;k++) fN[k] = orig.fN[k];
  fNL = orig.fNL;
  fA  = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = orig.fA[i];
}
template <typename TT>
Bool_t TNArray<TT>::IsEqual(const TObject *obj) const {
  // 2 arrays are equal if they are compatible and if all elements are
  //equal up to 1.0e-10 of the biggest norm
  const Double_t eps = 1.0e-10;
  Bool_t equal = kFALSE;
  Double_t norm,norm1,norm2;
  TNArray<TT> *p;
  p = (TNArray<TT> *)obj;
  if (IsCompatibleWith(*p)) {
    Int_t k;
    Double_t d;
    TComplex z;
    norm1 = Norm();
    norm2 = p->Norm();
    norm  = TMath::Max(norm1,norm2);
    equal = kTRUE;
    k     = 0;
    while ((equal) && (k<fNL-1)) {
      z  = fA[k] - p->fA[k];
      d  = TComplex::Abs(z)/norm;
      if (d>eps) equal = kFALSE;
      k++;
    }
  }
  return equal;
}
template <typename TT>
void TNArray<TT>::Init() {
  //Initialization
  fNd  = 0;
  fNL  = 2;
  fA   = 0;
  fN   = 0;
}
template <typename TT>
Double_t TNArray<TT>::Norm() const {
  // Calculates the norm [scalar product with itself] of the array "as if"
  //it was one dimensional
  Int_t k;
  TComplex z,z1,z2;
  Double_t norm = 0.0;
  for (k=0;k<fNL-1;k++) {
    z1    = fA[k];
    z2    = TComplex::Conjugate(z1);
    z     = z1*z2;
    norm += z.Re();
  }
  return norm;
}
template <typename TT>
void TNArray<TT>::Print() const {
  cout << *this << endl;
}
template <typename TT>
ostream& TNArray<TT>::Print(ostream& os) const {
  // Print the whole n multidimensionnal array
  UInt_t i1,i2,i3,i4,i5,i6;
  Int_t i;
  os << endl;
  os << "Dimension of array : " << fNd << endl;
  os << endl;
  switch (fNd) {
    case 6:
      os << "Nb. of elements in the 6th dimension : " << fN[5] << endl;
    case 5:
      os << "Nb. of elements in the 5th dimension : " << fN[4] << endl;
    case 4:
      os << "Nb. of elements in the 4th dimension : " << fN[3] << endl;
    case 3:
      os << "Nb. of elements in the 3rd dimension : " << fN[2] << endl;
    case 2:
      os << "Nb. of elements in the 2nd dimension : " << fN[1] << endl;
    case 1:
      os << "Nb. of elements in the 1st dimension : " << fN[0] << endl;
      break;
    default:
      break;
  }
  os << endl;
  switch (fNd) {
    case 1:
      for (i1=0;i1<fN[0];i1++) {
        os << "A(";
        os.width(4);
        os << i1 << ") =  ";
        i = OneDim(i1);
        os << fA[i] << endl;
      }
      break;
    case 2:
      for (i1=0;i1<fN[0];i1++)
        for (i2=0;i2<fN[1];i2++) {
          os << "A(";
          os.width(4);
          os << i1 << "  ";
          os.width(4);
          os << i2 << ") =  ";
          i = OneDim(i1,i2);
          os << fA[i] << endl;
        }
        break;
    case 3:
      for (i1=0;i1<fN[0];i1++)
        for (i2=0;i2<fN[1];i2++)
          for (i3=0;i3<fN[2];i3++) {
            os << "A(";
            os.width(4);
            os << i1 << "  ";
            os.width(4);
            os << i2 << "  ";
            os.width(4);
            os << i3 << ") =  ";
            i = OneDim(i1,i2,i3);
            os << fA[i] << endl;
          }
          break;
    case 4:
      for (i1=0;i1<fN[0];i1++)
        for (i2=0;i2<fN[1];i2++)
          for (i3=0;i3<fN[2];i3++)
            for (i4=0;i4<fN[3];i4++) {
              os << "A(";
              os.width(4);
              os << i1 << "  ";
              os.width(4);
              os << i2 << "  ";
              os.width(4);
              os << i3 << "  ";
              os.width(4);
              os << i4 << ") =  ";
              i = OneDim(i1,i2,i3,i4);
              os << fA[i] << endl;
            }
            break;
    case 5:
      for (i1=0;i1<fN[0];i1++)
        for (i2=0;i2<fN[1];i2++)
          for (i3=0;i3<fN[2];i3++)
            for (i4=0;i4<fN[3];i4++)
              for (i5=0;i5<fN[4];i5++) {
                os << "A(";
                os.width(4);
                os << i1 << "  ";
                os.width(4);
                os << i2 << "  ";
                os.width(4);
                os << i3 << "  ";
                os.width(4);
                os << i4 << "  ";
                os.width(4);
                os << i5 << ") =  ";
                i = OneDim(i1,i2,i3,i4,i5);
                os << fA[i] << endl;
              }
              break;
    case 6:
      for (i1=0;i1<fN[0];i1++)
        for (i2=0;i2<fN[1];i2++)
          for (i3=0;i3<fN[2];i3++)
            for (i4=0;i4<fN[3];i4++)
              for (i5=0;i5<fN[4];i5++)
                for (i6=0;i6<fN[5];i6++) {
                  os << "A(";
                  os.width(4);
                  os << i1 << "  ";
                  os.width(4);
                  os << i2 << "  ";
                  os.width(4);
                  os << i3 << "  ";
                  os.width(4);
                  os << i4 << "  ";
                  os.width(4);
                  os << i5 << "  ";
                  os.width(4);
                  os << i6 << ") =  ";
                  i = OneDim(i1,i2,i3,i4,i5,i6);
                  os << fA[i] << endl;
                }
                break;
    default:
      break;
  }
  return os;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1) {
  //Reset this to be 1 dimensional of dimension n1
  const TT zero = 0;
  Clean();
  fNd   = 1;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fNL   = n1 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1,Int_t n2) {
  //Reset this to be 2 dimensional of dimension n1,n2
  const TT zero = 0;
  Clean();
  fNd   = 2;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fNL   = n1*n2 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1,Int_t n2,Int_t n3) {
  //Reset this to be 3 dimensional of dimension n1,n2,n3
  const TT zero = 0;
  Clean();
  fNd   = 3;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fNL   = n1*n2*n3 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1,Int_t n2,Int_t n3,Int_t n4) {
  //Reset this to be 4 dimensional of dimension n1,n2,n3,n4
  const TT zero = 0;
  Clean();
  fNd   = 4;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fNL   = n1*n2*n3*n4 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5) {
  //Reset this to be 5 dimensional of dimension n1,n2,n3,n4,n5
  const TT zero = 0;
  Clean();
  fNd   = 5;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fN[4] = n5;
  fNL   = n1*n2*n3*n4*n5 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::ReSet(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5,Int_t n6) {
  //Reset this to be 6 dimensional of dimension n1,n2,n3,n4,n5,n6
  const TT zero = 0;
  Clean();
  fNd   = 6;
  fN    = new UInt_t [fNd];
  fN[0] = n1;
  fN[1] = n2;
  fN[2] = n3;
  fN[3] = n4;
  fN[4] = n5;
  fN[5] = n6;
  fNL   = n1*n2*n3*n4*n5*n6 + 1;
  fA    = new TT [fNL];
  for (Int_t i=0;i<fNL;i++) fA[i] = zero;
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1) {
  // Gives to the one dimensional array the new size n1. The element of the old 
  //one dimensional array are copied to the new array up to element n1. Other
  //elements are lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 1. It has to remain 1. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,N1;
  if (fA) {
    if (fNd != 1) {
      Error("Set","Dimension of old array not 1");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1);
    N1 = TMath::Min(old.fN[0],fN[0]);
    for (i1=0;i1<N1;i1++) (*this)(i1) = old(i1);
  }
  else ReSet(n1);
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1,Int_t n2) {
  // Gives to the 2-dim array the new size n1*n2. The element of the 2-dim old array
  //are copied to the new array up to available place. Other elements are lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 2. It has to remain 2. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,i2,N1,N2;
  if (fA) {
    if (fNd != 2) {
      Error("Set","Dimension of old array not 2");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1,n2);
    N1 = TMath::Min(old.fN[0],fN[0]);
    N2 = TMath::Min(old.fN[1],fN[1]);
    for (i1=0;i1<N1;i1++)
      for (i2=0;i2<N2;i2++) (*this)(i1,i2) = old(i1,i2);
  }
  else ReSet(n1,n2);
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1,Int_t n2,Int_t n3) {
  // Gives to the 3-dim array the new size n1*n2*n3. The element of the old 3-dim
  //array are copied to the new array up to available place. Other elements are lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 3. It has to remain 3. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,i2,i3,N1,N2,N3;
  if (fA) {
    if (fNd != 3) {
      Error("Set","Dimension of old array not 3");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1,n2,n3);
    N1 = TMath::Min(old.fN[0],fN[0]);
    N2 = TMath::Min(old.fN[1],fN[1]);
    N3 = TMath::Min(old.fN[2],fN[2]);
    for (i1=0;i1<N1;i1++)
      for (i2=0;i2<N2;i2++)
        for (i3=0;i3<N3;i3++) (*this)(i1,i2,i3) = old(i1,i2,i3);
  }
  else ReSet(n1,n2,n3);
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1,Int_t n2,Int_t n3,Int_t n4) {
  // Gives to the 4-dim array the new size n1*n2*n3*n4. The element of the old 4-dim
  //array are copied to the new array up to available place. Other elements are lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 4. It has to remain 4. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,i2,i3,i4,N1,N2,N3,N4;
  if (fA) {
    if (fNd != 4) {
      Error("Set","Dimension of old array not 4");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1,n2,n3,n4);
    N1 = TMath::Min(old.fN[0],fN[0]);
    N2 = TMath::Min(old.fN[1],fN[1]);
    N3 = TMath::Min(old.fN[2],fN[2]);
    N4 = TMath::Min(old.fN[3],fN[3]);
    for (i1=0;i1<N1;i1++)
      for (i2=0;i2<N2;i2++)
        for (i3=0;i3<N3;i3++) 
          for (i4=0;i4<N4;i4++) (*this)(i1,i2,i3,i4) = old(i1,i2,i3,i4);
  }
  else ReSet(n1,n2,n3,n4);
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5) {
  // Gives to the 5-dim array the new size n1*n2*n3*n4*n5. The element of the old 5-dim
  //array are copied to the new array up to available place. Other elements are lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 5. It has to remain 5. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,i2,i3,i4,i5,N1,N2,N3,N4,N5;
  if (fA) {
    if (fNd != 5) {
      Error("Set","Dimension of old array not 5");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1,n2,n3,n4,n5);
    N1 = TMath::Min(old.fN[0],fN[0]);
    N2 = TMath::Min(old.fN[1],fN[1]);
    N3 = TMath::Min(old.fN[2],fN[2]);
    N4 = TMath::Min(old.fN[3],fN[3]);
    N5 = TMath::Min(old.fN[4],fN[4]);
    for (i1=0;i1<N1;i1++)
      for (i2=0;i2<N2;i2++)
        for (i3=0;i3<N3;i3++) 
          for (i4=0;i4<N4;i4++)
            for (i5=0;i5<N5;i5++) (*this)(i1,i2,i3,i4,i5) = old(i1,i2,i3,i4,i5);
  }
  else ReSet(n1,n2,n3,n4,n5);
}
template <typename TT>
void TNArray<TT>::Set(Int_t n1,Int_t n2,Int_t n3,Int_t n4,Int_t n5,Int_t n6) {
  // Gives to the 6-dim array the new size n1*n2*n3*n4*n5*n6. The element of the old
  //6-dim array are copied to the new array up to available place. Other elements are
  //lost.
  // IMPORTANT: you cannot change the dimension [1,2,...6] of the array when calling
  //a Set method. In this case, dimension fNd is 6. It has to remain 6. Except for
  //the case where the old array was not yet reserved.
  Int_t i1,i2,i3,i4,i5,i6,N1,N2,N3,N4,N5,N6;
  if (fA) {
    if (fNd != 6) {
      Error("Set","Dimension of old array not 6");
      return;
    }
    TNArray<TT> old(*this);
    ReSet(n1,n2,n3,n4,n5,n6);
    N1 = TMath::Min(old.fN[0],fN[0]);
    N2 = TMath::Min(old.fN[1],fN[1]);
    N3 = TMath::Min(old.fN[2],fN[2]);
    N4 = TMath::Min(old.fN[3],fN[3]);
    N5 = TMath::Min(old.fN[4],fN[4]);
    N6 = TMath::Min(old.fN[5],fN[5]);
    for (i1=0;i1<N1;i1++)
      for (i2=0;i2<N2;i2++)
        for (i3=0;i3<N3;i3++) 
          for (i4=0;i4<N4;i4++)
            for (i5=0;i5<N5;i5++) 
              for (i6=0;i6<N6;i6++) (*this)(i1,i2,i3,i4,i5,i6) = old(i1,i2,i3,i4,i5,i6);
  }
  else ReSet(n1,n2,n3,n4,n5,n6);
}
template <typename TT>
TNArray<TT> &TNArray<TT>::operator=(const TNArray<TT> &orig) {
  // TNArray assignment operator.
  if (this != &orig) {
    Clean();
    Copy(orig);
  }
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator+=(TT val) {
  // Add val to every element of the array
  Int_t i;
  for (i=0;i<fNL-1;i++) fA[i] += val;
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator+=(const TNArray<TT> &B) {
  // Add array B if B compatible with *this. Else do nothing
  if (IsCompatibleWith(B)) {
    Int_t i;
    for (i=0;i<fNL-1;i++) fA[i] += B.fA[i];
  }
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator-=(TT val) {
  // Subtract val to every element of the array
  Int_t i;
  for (i=0;i<fNL-1;i++) fA[i] -= val;
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator-=(const TNArray<TT> &B) {
  // Subtract array B if B compatible with *this. Else do nothing
  if (IsCompatibleWith(B)) {
    Int_t i;
    for (i=0;i<fNL-1;i++) fA[i] -= B.fA[i];
  }
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator*=(TT val) {
  // Multiply every element of the array with val
  Int_t i;
  for (i=0;i<fNL-1;i++) fA[i] *= val;
  return *this;
}
template<typename TT>
TNArray<TT> &TNArray<TT>::operator/=(TT val) {
  // Divide every element of the array with val
  Int_t i;
  for (i=0;i<fNL-1;i++) fA[i] /= val;
  return *this;
}
template<typename TT>
Bool_t TNArray<TT>::operator<(const TNArray &B) const {
  // Is the norm of *this smaller than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1<num2);
}
template<typename TT>
Bool_t TNArray<TT>::operator<=(const TNArray &B) const {
  // Is the norm of *this smaller or equal than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1<=num2);
}
template<typename TT>
Bool_t TNArray<TT>::operator>(const TNArray &B) const {
  // Is the norm of *this greater than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1>num2);
}
template<typename TT>
Bool_t TNArray<TT>::operator>=(const TNArray &B) const {
  // Is the norm of *this greater or equal than the one of B ?
  Double_t num1,num2;
  num1 = Norm();
  num2 = B.Norm();
  return (num1>=num2);
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1) const {
  Int_t i;
  i = OneDim(i1);
  return fA[i];
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1,Int_t i2) const {
  Int_t i;
  i = OneDim(i1,i2);
  return fA[i];
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3) const {
  Int_t i;
  i = OneDim(i1,i2,i3);
  return fA[i];
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4) const {
  Int_t i;
  i = OneDim(i1,i2,i3,i4);
  return fA[i];
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5) const {
  Int_t i;
  i = OneDim(i1,i2,i3,i4,i5);
  return fA[i];
}
template <typename TT>
const TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5,Int_t i6) const {
  Int_t i;
  i = OneDim(i1,i2,i3,i4,i5,i6);
  return fA[i];
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1) {
  return (TT&)((*(const TNArray<TT> *)this)(i1));
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1,Int_t i2) {
  return (TT&)((*(const TNArray<TT> *)this)(i1,i2));
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3) {
  return (TT&)((*(const TNArray<TT> *)this)(i1,i2,i3));
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4) {
  return (TT&)((*(const TNArray<TT> *)this)(i1,i2,i3,i4));
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5) {
  return (TT&)((*(const TNArray<TT> *)this)(i1,i2,i3,i4,i5));
}
template <typename TT>
TT &TNArray<TT>::operator()(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5,Int_t i6) {
  return (TT&)((*(const TNArray<TT> *)this)(i1,i2,i3,i4,i5,i6));
}
template<typename TT>
TNArray<TT> operator+(const TNArray<TT> &A,const TNArray<TT> &B) {
  // Return A+B if both are compatible. If not compatible, returns A
  TNArray<TT> C(A);
  C += B;
  return C;
}
template<typename TT>
TNArray<TT> operator-(const TNArray<TT> &A,const TNArray<TT> &B) {
  // Return A-B if both are compatible. If not compatible, returns A
  TNArray<TT> C(A);
  C -= B;
  return C;
}
template<typename TT>
TComplex operator*(const TNArray<TT> &A,const TNArray<TT> &B) {
  // Calculates scalar product as if array was one-dimensional. But the
  //2 arrays must be compatible, else returns 0. In case of complex, the conjugate
  //of the 2nd vector is taken.
  TComplex z = 0;
  if (A.IsCompatibleWith(B)) {
    Int_t k;
    TComplex zz,z1,z2;
    for (k=0;k<A.GetSize();k++) {
      z1 = A.GetA(k);
      z2 = B.GetA(k);
      zz = z1*TComplex::Conjugate(z2);
      z += zz;
    }
  }
  return z;
}
template <typename TT>
ostream &operator<<(ostream &os,const TNArray<TT> &c) {
  //Allows to output the array with cout
  return c.Print(os);
}

template class TNArray<Char_t>;
template class TNArray<Short_t>;
template class TNArray<Int_t>;
template class TNArray<Long_t>;
template class TNArray<Long64_t>;
template class TNArray<Float_t>;
template class TNArray<Double_t>;
template class TNArray<TComplex>;

template TNArray<Char_t> operator+(const TNArray<Char_t> &A,const TNArray<Char_t> &B);
template TNArray<Short_t> operator+(const TNArray<Short_t> &A,const TNArray<Short_t> &B);
template TNArray<Int_t> operator+(const TNArray<Int_t> &A,const TNArray<Int_t> &B);
template TNArray<Long_t> operator+(const TNArray<Long_t> &A,const TNArray<Long_t> &B);
template TNArray<Long64_t> operator+(const TNArray<Long64_t> &A,const TNArray<Long64_t> &B);
template TNArray<Float_t> operator+(const TNArray<Float_t> &A,const TNArray<Float_t> &B);
template TNArray<Double_t> operator+(const TNArray<Double_t> &A,const TNArray<Double_t> &B);
template TNArray<TComplex> operator+(const TNArray<TComplex> &A,const TNArray<TComplex> &B);

template TNArray<Char_t> operator-(const TNArray<Char_t> &A,const TNArray<Char_t> &B);
template TNArray<Short_t> operator-(const TNArray<Short_t> &A,const TNArray<Short_t> &B);
template TNArray<Int_t> operator-(const TNArray<Int_t> &A,const TNArray<Int_t> &B);
template TNArray<Long_t> operator-(const TNArray<Long_t> &A,const TNArray<Long_t> &B);
template TNArray<Long64_t> operator-(const TNArray<Long64_t> &A,const TNArray<Long64_t> &B);
template TNArray<Float_t> operator-(const TNArray<Float_t> &A,const TNArray<Float_t> &B);
template TNArray<Double_t> operator-(const TNArray<Double_t> &A,const TNArray<Double_t> &B);
template TNArray<TComplex> operator-(const TNArray<TComplex> &A,const TNArray<TComplex> &B);

template TComplex operator*(const TNArray<Char_t> &A,const TNArray<Char_t> &B);
template TComplex operator*(const TNArray<Short_t> &A,const TNArray<Short_t> &B);
template TComplex operator*(const TNArray<Int_t> &A,const TNArray<Int_t> &B);
template TComplex operator*(const TNArray<Long_t> &A,const TNArray<Long_t> &B);
template TComplex operator*(const TNArray<Long64_t> &A,const TNArray<Long64_t> &B);
template TComplex operator*(const TNArray<Float_t> &A,const TNArray<Float_t> &B);
template TComplex operator*(const TNArray<Double_t> &A,const TNArray<Double_t> &B);
template TComplex operator*(const TNArray<TComplex> &A,const TNArray<TComplex> &B);

template ostream &operator<< <Char_t>(ostream &os,const TNArray<Char_t> &c);
template ostream &operator<< <Short_t>(ostream &os,const TNArray<Short_t> &c);
template ostream &operator<< <Int_t>(ostream &os,const TNArray<Int_t> &c);
template ostream &operator<< <Long_t>(ostream &os,const TNArray<Long_t> &c);
template ostream &operator<< <Long64_t>(ostream &os,const TNArray<Long64_t> &c);
template ostream &operator<< <Float_t>(ostream &os,const TNArray<Float_t> &c);
template ostream &operator<< <Double_t>(ostream &os,const TNArray<Double_t> &c);
template ostream &operator<< <TComplex>(ostream &os,const TNArray<TComplex> &c);
