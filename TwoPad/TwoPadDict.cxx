// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TwoPadDict

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
#include "TGRootIDE.h"
#include "TAuthor.h"
#include "TReference.h"
#include "TwoPadDisplay.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAuthor(void *p = 0);
   static void *newArray_TAuthor(Long_t size, void *p);
   static void delete_TAuthor(void *p);
   static void deleteArray_TAuthor(void *p);
   static void destruct_TAuthor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAuthor*)
   {
      ::TAuthor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAuthor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAuthor", ::TAuthor::Class_Version(), "TAuthor.h", 19,
                  typeid(::TAuthor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TAuthor::Dictionary, isa_proxy, 4,
                  sizeof(::TAuthor) );
      instance.SetNew(&new_TAuthor);
      instance.SetNewArray(&newArray_TAuthor);
      instance.SetDelete(&delete_TAuthor);
      instance.SetDeleteArray(&deleteArray_TAuthor);
      instance.SetDestructor(&destruct_TAuthor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAuthor*)
   {
      return GenerateInitInstanceLocal((::TAuthor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAuthor*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TReference(void *p = 0);
   static void *newArray_TReference(Long_t size, void *p);
   static void delete_TReference(void *p);
   static void deleteArray_TReference(void *p);
   static void destruct_TReference(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TReference*)
   {
      ::TReference *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TReference >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TReference", ::TReference::Class_Version(), "TReference.h", 19,
                  typeid(::TReference), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TReference::Dictionary, isa_proxy, 4,
                  sizeof(::TReference) );
      instance.SetNew(&new_TReference);
      instance.SetNewArray(&newArray_TReference);
      instance.SetDelete(&delete_TReference);
      instance.SetDeleteArray(&deleteArray_TReference);
      instance.SetDestructor(&destruct_TReference);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TReference*)
   {
      return GenerateInitInstanceLocal((::TReference*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TReference*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TwoPadDisplay(void *p = 0);
   static void *newArray_TwoPadDisplay(Long_t size, void *p);
   static void delete_TwoPadDisplay(void *p);
   static void deleteArray_TwoPadDisplay(void *p);
   static void destruct_TwoPadDisplay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TwoPadDisplay*)
   {
      ::TwoPadDisplay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TwoPadDisplay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TwoPadDisplay", ::TwoPadDisplay::Class_Version(), "TwoPadDisplay.h", 26,
                  typeid(::TwoPadDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TwoPadDisplay::Dictionary, isa_proxy, 4,
                  sizeof(::TwoPadDisplay) );
      instance.SetNew(&new_TwoPadDisplay);
      instance.SetNewArray(&newArray_TwoPadDisplay);
      instance.SetDelete(&delete_TwoPadDisplay);
      instance.SetDeleteArray(&deleteArray_TwoPadDisplay);
      instance.SetDestructor(&destruct_TwoPadDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TwoPadDisplay*)
   {
      return GenerateInitInstanceLocal((::TwoPadDisplay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TwoPadDisplay*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TGRootIDE(void *p = 0);
   static void *newArray_TGRootIDE(Long_t size, void *p);
   static void delete_TGRootIDE(void *p);
   static void deleteArray_TGRootIDE(void *p);
   static void destruct_TGRootIDE(void *p);
   static void streamer_TGRootIDE(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TGRootIDE*)
   {
      ::TGRootIDE *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TGRootIDE >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TGRootIDE", ::TGRootIDE::Class_Version(), "TGRootIDE.h", 86,
                  typeid(::TGRootIDE), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TGRootIDE::Dictionary, isa_proxy, 16,
                  sizeof(::TGRootIDE) );
      instance.SetNew(&new_TGRootIDE);
      instance.SetNewArray(&newArray_TGRootIDE);
      instance.SetDelete(&delete_TGRootIDE);
      instance.SetDeleteArray(&deleteArray_TGRootIDE);
      instance.SetDestructor(&destruct_TGRootIDE);
      instance.SetStreamerFunc(&streamer_TGRootIDE);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TGRootIDE*)
   {
      return GenerateInitInstanceLocal((::TGRootIDE*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TGRootIDE*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TGDocument(void *p = 0);
   static void *newArray_TGDocument(Long_t size, void *p);
   static void delete_TGDocument(void *p);
   static void deleteArray_TGDocument(void *p);
   static void destruct_TGDocument(void *p);
   static void streamer_TGDocument(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TGDocument*)
   {
      ::TGDocument *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TGDocument >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TGDocument", ::TGDocument::Class_Version(), "TGRootIDE.h", 54,
                  typeid(::TGDocument), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TGDocument::Dictionary, isa_proxy, 16,
                  sizeof(::TGDocument) );
      instance.SetNew(&new_TGDocument);
      instance.SetNewArray(&newArray_TGDocument);
      instance.SetDelete(&delete_TGDocument);
      instance.SetDeleteArray(&deleteArray_TGDocument);
      instance.SetDestructor(&destruct_TGDocument);
      instance.SetStreamerFunc(&streamer_TGDocument);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TGDocument*)
   {
      return GenerateInitInstanceLocal((::TGDocument*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TGDocument*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAuthor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAuthor::Class_Name()
{
   return "TAuthor";
}

//______________________________________________________________________________
const char *TAuthor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAuthor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAuthor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAuthor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAuthor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAuthor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAuthor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAuthor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TReference::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TReference::Class_Name()
{
   return "TReference";
}

//______________________________________________________________________________
const char *TReference::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TReference*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TReference::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TReference*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TReference::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TReference*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TReference::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TReference*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TwoPadDisplay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TwoPadDisplay::Class_Name()
{
   return "TwoPadDisplay";
}

//______________________________________________________________________________
const char *TwoPadDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoPadDisplay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TwoPadDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoPadDisplay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TwoPadDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoPadDisplay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TwoPadDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoPadDisplay*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TGRootIDE::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TGRootIDE::Class_Name()
{
   return "TGRootIDE";
}

//______________________________________________________________________________
const char *TGRootIDE::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGRootIDE*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TGRootIDE::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGRootIDE*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TGRootIDE::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGRootIDE*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TGRootIDE::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGRootIDE*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TGDocument::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TGDocument::Class_Name()
{
   return "TGDocument";
}

//______________________________________________________________________________
const char *TGDocument::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGDocument*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TGDocument::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGDocument*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TGDocument::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGDocument*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TGDocument::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGDocument*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAuthor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAuthor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TAuthor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TAuthor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAuthor(void *p) {
      return  p ? new(p) ::TAuthor : new ::TAuthor;
   }
   static void *newArray_TAuthor(Long_t nElements, void *p) {
      return p ? new(p) ::TAuthor[nElements] : new ::TAuthor[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAuthor(void *p) {
      delete ((::TAuthor*)p);
   }
   static void deleteArray_TAuthor(void *p) {
      delete [] ((::TAuthor*)p);
   }
   static void destruct_TAuthor(void *p) {
      typedef ::TAuthor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TAuthor

//______________________________________________________________________________
void TReference::Streamer(TBuffer &R__b)
{
   // Stream an object of class TReference.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TReference::Class(),this);
   } else {
      R__b.WriteClassBuffer(TReference::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TReference(void *p) {
      return  p ? new(p) ::TReference : new ::TReference;
   }
   static void *newArray_TReference(Long_t nElements, void *p) {
      return p ? new(p) ::TReference[nElements] : new ::TReference[nElements];
   }
   // Wrapper around operator delete
   static void delete_TReference(void *p) {
      delete ((::TReference*)p);
   }
   static void deleteArray_TReference(void *p) {
      delete [] ((::TReference*)p);
   }
   static void destruct_TReference(void *p) {
      typedef ::TReference current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TReference

//______________________________________________________________________________
void TwoPadDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class TwoPadDisplay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TwoPadDisplay::Class(),this);
   } else {
      R__b.WriteClassBuffer(TwoPadDisplay::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TwoPadDisplay(void *p) {
      return  p ? new(p) ::TwoPadDisplay : new ::TwoPadDisplay;
   }
   static void *newArray_TwoPadDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::TwoPadDisplay[nElements] : new ::TwoPadDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_TwoPadDisplay(void *p) {
      delete ((::TwoPadDisplay*)p);
   }
   static void deleteArray_TwoPadDisplay(void *p) {
      delete [] ((::TwoPadDisplay*)p);
   }
   static void destruct_TwoPadDisplay(void *p) {
      typedef ::TwoPadDisplay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TwoPadDisplay

//______________________________________________________________________________
void TGRootIDE::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGRootIDE.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TGRootIDE(void *p) {
      return  p ? new(p) ::TGRootIDE : new ::TGRootIDE;
   }
   static void *newArray_TGRootIDE(Long_t nElements, void *p) {
      return p ? new(p) ::TGRootIDE[nElements] : new ::TGRootIDE[nElements];
   }
   // Wrapper around operator delete
   static void delete_TGRootIDE(void *p) {
      delete ((::TGRootIDE*)p);
   }
   static void deleteArray_TGRootIDE(void *p) {
      delete [] ((::TGRootIDE*)p);
   }
   static void destruct_TGRootIDE(void *p) {
      typedef ::TGRootIDE current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TGRootIDE(TBuffer &buf, void *obj) {
      ((::TGRootIDE*)obj)->::TGRootIDE::Streamer(buf);
   }
} // end of namespace ROOT for class ::TGRootIDE

//______________________________________________________________________________
void TGDocument::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGDocument.

   TNamed::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TGDocument(void *p) {
      return  p ? new(p) ::TGDocument : new ::TGDocument;
   }
   static void *newArray_TGDocument(Long_t nElements, void *p) {
      return p ? new(p) ::TGDocument[nElements] : new ::TGDocument[nElements];
   }
   // Wrapper around operator delete
   static void delete_TGDocument(void *p) {
      delete ((::TGDocument*)p);
   }
   static void deleteArray_TGDocument(void *p) {
      delete [] ((::TGDocument*)p);
   }
   static void destruct_TGDocument(void *p) {
      typedef ::TGDocument current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TGDocument(TBuffer &buf, void *obj) {
      ((::TGDocument*)obj)->::TGDocument::Streamer(buf);
   }
} // end of namespace ROOT for class ::TGDocument

namespace {
  void TriggerDictionaryInitialization_TwoPadDict_Impl() {
    static const char* headers[] = {
"TGRootIDE.h",
"TAuthor.h",
"TReference.h",
"TwoPadDisplay.h",
0
    };
    static const char* includePaths[] = {
"/home/serena/Software/root-install/include",
"/home/serena/Software/SLITRANI/slitrani-fixed/TwoPad/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TwoPadDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Class giving info on one author of program)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TAuthor.h")))  TAuthor;
class __attribute__((annotate(R"ATTRDUMP(Class to specify references for a program)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TReference.h")))  TReference;
class __attribute__((annotate(R"ATTRDUMP(Useful to show drawings or histograms into 2 pads)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TwoPadDisplay.h")))  TwoPadDisplay;
class __attribute__((annotate(R"ATTRDUMP(Simple IDE using TGTextEdit and TGHtml widgets)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TGRootIDE.h")))  TGRootIDE;
class __attribute__((annotate(R"ATTRDUMP(Simple class describing document used in TGRootIDE)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TGRootIDE.h")))  TGDocument;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TwoPadDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TGRootIDE.h"
#include "TAuthor.h"
#include "TReference.h"
#include "TwoPadDisplay.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"StateOfPads", payloadCode, "@",
"TAuthor", payloadCode, "@",
"TGDocument", payloadCode, "@",
"TGRootIDE", payloadCode, "@",
"TReference", payloadCode, "@",
"TwoPadDisplay", payloadCode, "@",
"gRootIDE", payloadCode, "@",
"gTwoPad", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TwoPadDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TwoPadDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TwoPadDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TwoPadDict() {
  TriggerDictionaryInitialization_TwoPadDict_Impl();
}
