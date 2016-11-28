// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME VisuSLitraniDict

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
#include "TVisuSLit.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TVisuSLit(void *p = 0);
   static void *newArray_TVisuSLit(Long_t size, void *p);
   static void delete_TVisuSLit(void *p);
   static void deleteArray_TVisuSLit(void *p);
   static void destruct_TVisuSLit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TVisuSLit*)
   {
      ::TVisuSLit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TVisuSLit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TVisuSLit", ::TVisuSLit::Class_Version(), "TVisuSLit.h", 31,
                  typeid(::TVisuSLit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TVisuSLit::Dictionary, isa_proxy, 4,
                  sizeof(::TVisuSLit) );
      instance.SetNew(&new_TVisuSLit);
      instance.SetNewArray(&newArray_TVisuSLit);
      instance.SetDelete(&delete_TVisuSLit);
      instance.SetDeleteArray(&deleteArray_TVisuSLit);
      instance.SetDestructor(&destruct_TVisuSLit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TVisuSLit*)
   {
      return GenerateInitInstanceLocal((::TVisuSLit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TVisuSLit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TVisuSLit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TVisuSLit::Class_Name()
{
   return "TVisuSLit";
}

//______________________________________________________________________________
const char *TVisuSLit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TVisuSLit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TVisuSLit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TVisuSLit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TVisuSLit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TVisuSLit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TVisuSLit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TVisuSLit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TVisuSLit::Streamer(TBuffer &R__b)
{
   // Stream an object of class TVisuSLit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TVisuSLit::Class(),this);
   } else {
      R__b.WriteClassBuffer(TVisuSLit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TVisuSLit(void *p) {
      return  p ? new(p) ::TVisuSLit : new ::TVisuSLit;
   }
   static void *newArray_TVisuSLit(Long_t nElements, void *p) {
      return p ? new(p) ::TVisuSLit[nElements] : new ::TVisuSLit[nElements];
   }
   // Wrapper around operator delete
   static void delete_TVisuSLit(void *p) {
      delete ((::TVisuSLit*)p);
   }
   static void deleteArray_TVisuSLit(void *p) {
      delete [] ((::TVisuSLit*)p);
   }
   static void destruct_TVisuSLit(void *p) {
      typedef ::TVisuSLit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TVisuSLit

namespace {
  void TriggerDictionaryInitialization_VisuSLitraniDict_Impl() {
    static const char* headers[] = {
"TVisuSLit.h",
0
    };
    static const char* includePaths[] = {
"/home/serena/Software/SLITRANI/rootdev/include",
"/home/serena/Software/root-install/include",
"/home/serena/Software/SLITRANI/slitrani-fixed/VisuSLitrani/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "VisuSLitraniDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Visualize the ROOT files produced by SLitrani)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TVisuSLit.h")))  TVisuSLit;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "VisuSLitraniDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TVisuSLit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TVisuSLit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("VisuSLitraniDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_VisuSLitraniDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_VisuSLitraniDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_VisuSLitraniDict() {
  TriggerDictionaryInitialization_VisuSLitraniDict_Impl();
}
