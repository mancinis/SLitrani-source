// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SplineFitDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TPoly3.h"
#include "TBandedLE.h"
#include "TZigZag.h"
#include "TSplineFit.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TPoly3(void *p = 0);
   static void *newArray_TPoly3(Long_t size, void *p);
   static void delete_TPoly3(void *p);
   static void deleteArray_TPoly3(void *p);
   static void destruct_TPoly3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TPoly3*)
   {
      ::TPoly3 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TPoly3 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TPoly3", ::TPoly3::Class_Version(), "TPoly3.h", 16,
                  typeid(::TPoly3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TPoly3::Dictionary, isa_proxy, 4,
                  sizeof(::TPoly3) );
      instance.SetNew(&new_TPoly3);
      instance.SetNewArray(&newArray_TPoly3);
      instance.SetDelete(&delete_TPoly3);
      instance.SetDeleteArray(&deleteArray_TPoly3);
      instance.SetDestructor(&destruct_TPoly3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TPoly3*)
   {
      return GenerateInitInstanceLocal((::TPoly3*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TPoly3*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TZigZag(void *p = 0);
   static void *newArray_TZigZag(Long_t size, void *p);
   static void delete_TZigZag(void *p);
   static void deleteArray_TZigZag(void *p);
   static void destruct_TZigZag(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TZigZag*)
   {
      ::TZigZag *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TZigZag >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TZigZag", ::TZigZag::Class_Version(), "TZigZag.h", 15,
                  typeid(::TZigZag), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TZigZag::Dictionary, isa_proxy, 4,
                  sizeof(::TZigZag) );
      instance.SetNew(&new_TZigZag);
      instance.SetNewArray(&newArray_TZigZag);
      instance.SetDelete(&delete_TZigZag);
      instance.SetDeleteArray(&deleteArray_TZigZag);
      instance.SetDestructor(&destruct_TZigZag);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TZigZag*)
   {
      return GenerateInitInstanceLocal((::TZigZag*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TZigZag*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TBandedLE(void *p = 0);
   static void *newArray_TBandedLE(Long_t size, void *p);
   static void delete_TBandedLE(void *p);
   static void deleteArray_TBandedLE(void *p);
   static void destruct_TBandedLE(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBandedLE*)
   {
      ::TBandedLE *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TBandedLE >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TBandedLE", ::TBandedLE::Class_Version(), "TBandedLE.h", 14,
                  typeid(::TBandedLE), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TBandedLE::Dictionary, isa_proxy, 4,
                  sizeof(::TBandedLE) );
      instance.SetNew(&new_TBandedLE);
      instance.SetNewArray(&newArray_TBandedLE);
      instance.SetDelete(&delete_TBandedLE);
      instance.SetDeleteArray(&deleteArray_TBandedLE);
      instance.SetDestructor(&destruct_TBandedLE);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBandedLE*)
   {
      return GenerateInitInstanceLocal((::TBandedLE*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TBandedLE*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSplineFit(void *p = 0);
   static void *newArray_TSplineFit(Long_t size, void *p);
   static void delete_TSplineFit(void *p);
   static void deleteArray_TSplineFit(void *p);
   static void destruct_TSplineFit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSplineFit*)
   {
      ::TSplineFit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSplineFit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSplineFit", ::TSplineFit::Class_Version(), "TSplineFit.h", 30,
                  typeid(::TSplineFit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSplineFit::Dictionary, isa_proxy, 4,
                  sizeof(::TSplineFit) );
      instance.SetNew(&new_TSplineFit);
      instance.SetNewArray(&newArray_TSplineFit);
      instance.SetDelete(&delete_TSplineFit);
      instance.SetDeleteArray(&deleteArray_TSplineFit);
      instance.SetDestructor(&destruct_TSplineFit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSplineFit*)
   {
      return GenerateInitInstanceLocal((::TSplineFit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TSplineFit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TPoly3::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TPoly3::Class_Name()
{
   return "TPoly3";
}

//______________________________________________________________________________
const char *TPoly3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPoly3*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TPoly3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPoly3*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TPoly3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPoly3*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TPoly3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPoly3*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TZigZag::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TZigZag::Class_Name()
{
   return "TZigZag";
}

//______________________________________________________________________________
const char *TZigZag::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TZigZag*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TZigZag::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TZigZag*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TZigZag::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TZigZag*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TZigZag::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TZigZag*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TBandedLE::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TBandedLE::Class_Name()
{
   return "TBandedLE";
}

//______________________________________________________________________________
const char *TBandedLE::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TBandedLE*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TBandedLE::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TBandedLE*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TBandedLE::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TBandedLE*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TBandedLE::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TBandedLE*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSplineFit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSplineFit::Class_Name()
{
   return "TSplineFit";
}

//______________________________________________________________________________
const char *TSplineFit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSplineFit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSplineFit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSplineFit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSplineFit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSplineFit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSplineFit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSplineFit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TPoly3::Streamer(TBuffer &R__b)
{
   // Stream an object of class TPoly3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TPoly3::Class(),this);
   } else {
      R__b.WriteClassBuffer(TPoly3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TPoly3(void *p) {
      return  p ? new(p) ::TPoly3 : new ::TPoly3;
   }
   static void *newArray_TPoly3(Long_t nElements, void *p) {
      return p ? new(p) ::TPoly3[nElements] : new ::TPoly3[nElements];
   }
   // Wrapper around operator delete
   static void delete_TPoly3(void *p) {
      delete ((::TPoly3*)p);
   }
   static void deleteArray_TPoly3(void *p) {
      delete [] ((::TPoly3*)p);
   }
   static void destruct_TPoly3(void *p) {
      typedef ::TPoly3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TPoly3

//______________________________________________________________________________
void TZigZag::Streamer(TBuffer &R__b)
{
   // Stream an object of class TZigZag.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TZigZag::Class(),this);
   } else {
      R__b.WriteClassBuffer(TZigZag::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TZigZag(void *p) {
      return  p ? new(p) ::TZigZag : new ::TZigZag;
   }
   static void *newArray_TZigZag(Long_t nElements, void *p) {
      return p ? new(p) ::TZigZag[nElements] : new ::TZigZag[nElements];
   }
   // Wrapper around operator delete
   static void delete_TZigZag(void *p) {
      delete ((::TZigZag*)p);
   }
   static void deleteArray_TZigZag(void *p) {
      delete [] ((::TZigZag*)p);
   }
   static void destruct_TZigZag(void *p) {
      typedef ::TZigZag current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TZigZag

//______________________________________________________________________________
void TBandedLE::Streamer(TBuffer &R__b)
{
   // Stream an object of class TBandedLE.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TBandedLE::Class(),this);
   } else {
      R__b.WriteClassBuffer(TBandedLE::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBandedLE(void *p) {
      return  p ? new(p) ::TBandedLE : new ::TBandedLE;
   }
   static void *newArray_TBandedLE(Long_t nElements, void *p) {
      return p ? new(p) ::TBandedLE[nElements] : new ::TBandedLE[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBandedLE(void *p) {
      delete ((::TBandedLE*)p);
   }
   static void deleteArray_TBandedLE(void *p) {
      delete [] ((::TBandedLE*)p);
   }
   static void destruct_TBandedLE(void *p) {
      typedef ::TBandedLE current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TBandedLE

//______________________________________________________________________________
void TSplineFit::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSplineFit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSplineFit::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSplineFit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSplineFit(void *p) {
      return  p ? new(p) ::TSplineFit : new ::TSplineFit;
   }
   static void *newArray_TSplineFit(Long_t nElements, void *p) {
      return p ? new(p) ::TSplineFit[nElements] : new ::TSplineFit[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSplineFit(void *p) {
      delete ((::TSplineFit*)p);
   }
   static void deleteArray_TSplineFit(void *p) {
      delete [] ((::TSplineFit*)p);
   }
   static void destruct_TSplineFit(void *p) {
      typedef ::TSplineFit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSplineFit

namespace {
  void TriggerDictionaryInitialization_SplineFitDict_Impl() {
    static const char* headers[] = {
"TPoly3.h",
"TBandedLE.h",
"TZigZag.h",
"TSplineFit.h",
0
    };
    static const char* includePaths[] = {
"/home/serena/Software/SLITRANI/rootdev/include",
"/home/serena/Software/root-install/include",
"/home/serena/Software/SLITRANI/slitrani-fixed/SplineFit/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SplineFitDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Handling of 3rd order polynoms)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TPoly3.h")))  TPoly3;
class __attribute__((annotate(R"ATTRDUMP(Labelling of points such that point i+1 always near from point i)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TZigZag.h")))  TZigZag;
class __attribute__((annotate(R"ATTRDUMP(CERN program F406 DBEQN translated to C++)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TBandedLE.h")))  TBandedLE;
class __attribute__((annotate(R"ATTRDUMP(General Handling of Spline fits)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TSplineFit.h")))  TSplineFit;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SplineFitDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TPoly3.h"
#include "TBandedLE.h"
#include "TZigZag.h"
#include "TSplineFit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"KindOfFit", payloadCode, "@",
"TBandedLE", payloadCode, "@",
"TPoly3", payloadCode, "@",
"TSplineFit", payloadCode, "@",
"TZigZag", payloadCode, "@",
"gSplineFit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SplineFitDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SplineFitDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SplineFitDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SplineFitDict() {
  TriggerDictionaryInitialization_SplineFitDict_Impl();
}
