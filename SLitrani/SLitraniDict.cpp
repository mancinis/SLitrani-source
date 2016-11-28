// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SLitraniDict

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
#include "TLitPol3.h"
#include "TLitPol4.h"
#include "TLitEqIndex.h"
#include "TLitWave.h"
#include "TLitHistParams.h"
#include "TLitPhys.h"
#include "TLitTrap.h"
#include "TLitSpectrumCp.h"
#include "TLitSpectrum.h"
#include "TLitDetector.h"
#include "TLitSellmeier.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLit.h"
#include "TLitPhoton.h"
#include "TLitSpontan.h"
#include "TLitranino.h"
#include "TLitResults.h"
#include "TLitMarrow.h"
#include "TLitParticle.h"
#include "TLitBeam.h"
#include "TLitCascade.h"
#include "TLitGamma.h"
#include "TLitAlbedo.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TLitPol3(void *p = 0);
   static void *newArray_TLitPol3(Long_t size, void *p);
   static void delete_TLitPol3(void *p);
   static void deleteArray_TLitPol3(void *p);
   static void destruct_TLitPol3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitPol3*)
   {
      ::TLitPol3 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitPol3 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitPol3", ::TLitPol3::Class_Version(), "TLitPol3.h", 17,
                  typeid(::TLitPol3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitPol3::Dictionary, isa_proxy, 4,
                  sizeof(::TLitPol3) );
      instance.SetNew(&new_TLitPol3);
      instance.SetNewArray(&newArray_TLitPol3);
      instance.SetDelete(&delete_TLitPol3);
      instance.SetDeleteArray(&deleteArray_TLitPol3);
      instance.SetDestructor(&destruct_TLitPol3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitPol3*)
   {
      return GenerateInitInstanceLocal((::TLitPol3*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitPol3*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitPol4(void *p = 0);
   static void *newArray_TLitPol4(Long_t size, void *p);
   static void delete_TLitPol4(void *p);
   static void deleteArray_TLitPol4(void *p);
   static void destruct_TLitPol4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitPol4*)
   {
      ::TLitPol4 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitPol4 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitPol4", ::TLitPol4::Class_Version(), "TLitPol4.h", 26,
                  typeid(::TLitPol4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitPol4::Dictionary, isa_proxy, 4,
                  sizeof(::TLitPol4) );
      instance.SetNew(&new_TLitPol4);
      instance.SetNewArray(&newArray_TLitPol4);
      instance.SetDelete(&delete_TLitPol4);
      instance.SetDeleteArray(&deleteArray_TLitPol4);
      instance.SetDestructor(&destruct_TLitPol4);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitPol4*)
   {
      return GenerateInitInstanceLocal((::TLitPol4*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitPol4*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitHistParams(void *p = 0);
   static void *newArray_TLitHistParams(Long_t size, void *p);
   static void delete_TLitHistParams(void *p);
   static void deleteArray_TLitHistParams(void *p);
   static void destruct_TLitHistParams(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitHistParams*)
   {
      ::TLitHistParams *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitHistParams >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitHistParams", ::TLitHistParams::Class_Version(), "TLitHistParams.h", 20,
                  typeid(::TLitHistParams), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitHistParams::Dictionary, isa_proxy, 4,
                  sizeof(::TLitHistParams) );
      instance.SetNew(&new_TLitHistParams);
      instance.SetNewArray(&newArray_TLitHistParams);
      instance.SetDelete(&delete_TLitHistParams);
      instance.SetDeleteArray(&deleteArray_TLitHistParams);
      instance.SetDestructor(&destruct_TLitHistParams);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitHistParams*)
   {
      return GenerateInitInstanceLocal((::TLitHistParams*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitHistParams*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitPhys*)
   {
      ::TLitPhys *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitPhys >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitPhys", ::TLitPhys::Class_Version(), "TLitPhys.h", 25,
                  typeid(::TLitPhys), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitPhys::Dictionary, isa_proxy, 4,
                  sizeof(::TLitPhys) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitPhys*)
   {
      return GenerateInitInstanceLocal((::TLitPhys*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitPhys*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitWave(void *p = 0);
   static void *newArray_TLitWave(Long_t size, void *p);
   static void delete_TLitWave(void *p);
   static void deleteArray_TLitWave(void *p);
   static void destruct_TLitWave(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitWave*)
   {
      ::TLitWave *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitWave >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitWave", ::TLitWave::Class_Version(), "TLitWave.h", 22,
                  typeid(::TLitWave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitWave::Dictionary, isa_proxy, 4,
                  sizeof(::TLitWave) );
      instance.SetNew(&new_TLitWave);
      instance.SetNewArray(&newArray_TLitWave);
      instance.SetDelete(&delete_TLitWave);
      instance.SetDeleteArray(&deleteArray_TLitWave);
      instance.SetDestructor(&destruct_TLitWave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitWave*)
   {
      return GenerateInitInstanceLocal((::TLitWave*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitWave*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitEqIndex(void *p = 0);
   static void *newArray_TLitEqIndex(Long_t size, void *p);
   static void delete_TLitEqIndex(void *p);
   static void deleteArray_TLitEqIndex(void *p);
   static void destruct_TLitEqIndex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitEqIndex*)
   {
      ::TLitEqIndex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitEqIndex >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitEqIndex", ::TLitEqIndex::Class_Version(), "TLitEqIndex.h", 24,
                  typeid(::TLitEqIndex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitEqIndex::Dictionary, isa_proxy, 4,
                  sizeof(::TLitEqIndex) );
      instance.SetNew(&new_TLitEqIndex);
      instance.SetNewArray(&newArray_TLitEqIndex);
      instance.SetDelete(&delete_TLitEqIndex);
      instance.SetDeleteArray(&deleteArray_TLitEqIndex);
      instance.SetDestructor(&destruct_TLitEqIndex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitEqIndex*)
   {
      return GenerateInitInstanceLocal((::TLitEqIndex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitEqIndex*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitTrap(void *p = 0);
   static void *newArray_TLitTrap(Long_t size, void *p);
   static void delete_TLitTrap(void *p);
   static void deleteArray_TLitTrap(void *p);
   static void destruct_TLitTrap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitTrap*)
   {
      ::TLitTrap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitTrap >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitTrap", ::TLitTrap::Class_Version(), "TLitTrap.h", 18,
                  typeid(::TLitTrap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitTrap::Dictionary, isa_proxy, 4,
                  sizeof(::TLitTrap) );
      instance.SetNew(&new_TLitTrap);
      instance.SetNewArray(&newArray_TLitTrap);
      instance.SetDelete(&delete_TLitTrap);
      instance.SetDeleteArray(&deleteArray_TLitTrap);
      instance.SetDestructor(&destruct_TLitTrap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitTrap*)
   {
      return GenerateInitInstanceLocal((::TLitTrap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitTrap*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitSpectrumCp(void *p = 0);
   static void *newArray_TLitSpectrumCp(Long_t size, void *p);
   static void delete_TLitSpectrumCp(void *p);
   static void deleteArray_TLitSpectrumCp(void *p);
   static void destruct_TLitSpectrumCp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitSpectrumCp*)
   {
      ::TLitSpectrumCp *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitSpectrumCp >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitSpectrumCp", ::TLitSpectrumCp::Class_Version(), "TLitSpectrumCp.h", 17,
                  typeid(::TLitSpectrumCp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitSpectrumCp::Dictionary, isa_proxy, 4,
                  sizeof(::TLitSpectrumCp) );
      instance.SetNew(&new_TLitSpectrumCp);
      instance.SetNewArray(&newArray_TLitSpectrumCp);
      instance.SetDelete(&delete_TLitSpectrumCp);
      instance.SetDeleteArray(&deleteArray_TLitSpectrumCp);
      instance.SetDestructor(&destruct_TLitSpectrumCp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitSpectrumCp*)
   {
      return GenerateInitInstanceLocal((::TLitSpectrumCp*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitSpectrumCp*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitSpectrum(void *p = 0);
   static void *newArray_TLitSpectrum(Long_t size, void *p);
   static void delete_TLitSpectrum(void *p);
   static void deleteArray_TLitSpectrum(void *p);
   static void destruct_TLitSpectrum(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitSpectrum*)
   {
      ::TLitSpectrum *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitSpectrum >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitSpectrum", ::TLitSpectrum::Class_Version(), "TLitSpectrum.h", 28,
                  typeid(::TLitSpectrum), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitSpectrum::Dictionary, isa_proxy, 4,
                  sizeof(::TLitSpectrum) );
      instance.SetNew(&new_TLitSpectrum);
      instance.SetNewArray(&newArray_TLitSpectrum);
      instance.SetDelete(&delete_TLitSpectrum);
      instance.SetDeleteArray(&deleteArray_TLitSpectrum);
      instance.SetDestructor(&destruct_TLitSpectrum);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitSpectrum*)
   {
      return GenerateInitInstanceLocal((::TLitSpectrum*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitSpectrum*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitDetector(void *p = 0);
   static void *newArray_TLitDetector(Long_t size, void *p);
   static void delete_TLitDetector(void *p);
   static void deleteArray_TLitDetector(void *p);
   static void destruct_TLitDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitDetector*)
   {
      ::TLitDetector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitDetector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitDetector", ::TLitDetector::Class_Version(), "TLitDetector.h", 21,
                  typeid(::TLitDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitDetector::Dictionary, isa_proxy, 4,
                  sizeof(::TLitDetector) );
      instance.SetNew(&new_TLitDetector);
      instance.SetNewArray(&newArray_TLitDetector);
      instance.SetDelete(&delete_TLitDetector);
      instance.SetDeleteArray(&deleteArray_TLitDetector);
      instance.SetDestructor(&destruct_TLitDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitDetector*)
   {
      return GenerateInitInstanceLocal((::TLitDetector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitDetector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitSellmeier(void *p = 0);
   static void *newArray_TLitSellmeier(Long_t size, void *p);
   static void delete_TLitSellmeier(void *p);
   static void deleteArray_TLitSellmeier(void *p);
   static void destruct_TLitSellmeier(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitSellmeier*)
   {
      ::TLitSellmeier *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitSellmeier >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitSellmeier", ::TLitSellmeier::Class_Version(), "TLitSellmeier.h", 19,
                  typeid(::TLitSellmeier), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitSellmeier::Dictionary, isa_proxy, 4,
                  sizeof(::TLitSellmeier) );
      instance.SetNew(&new_TLitSellmeier);
      instance.SetNewArray(&newArray_TLitSellmeier);
      instance.SetDelete(&delete_TLitSellmeier);
      instance.SetDeleteArray(&deleteArray_TLitSellmeier);
      instance.SetDestructor(&destruct_TLitSellmeier);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitSellmeier*)
   {
      return GenerateInitInstanceLocal((::TLitSellmeier*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitSellmeier*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitMedium(void *p = 0);
   static void *newArray_TLitMedium(Long_t size, void *p);
   static void delete_TLitMedium(void *p);
   static void deleteArray_TLitMedium(void *p);
   static void destruct_TLitMedium(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitMedium*)
   {
      ::TLitMedium *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitMedium >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitMedium", ::TLitMedium::Class_Version(), "TLitMedium.h", 28,
                  typeid(::TLitMedium), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitMedium::Dictionary, isa_proxy, 4,
                  sizeof(::TLitMedium) );
      instance.SetNew(&new_TLitMedium);
      instance.SetNewArray(&newArray_TLitMedium);
      instance.SetDelete(&delete_TLitMedium);
      instance.SetDeleteArray(&deleteArray_TLitMedium);
      instance.SetDestructor(&destruct_TLitMedium);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitMedium*)
   {
      return GenerateInitInstanceLocal((::TLitMedium*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitMedium*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitVolume(void *p = 0);
   static void *newArray_TLitVolume(Long_t size, void *p);
   static void delete_TLitVolume(void *p);
   static void deleteArray_TLitVolume(void *p);
   static void destruct_TLitVolume(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitVolume*)
   {
      ::TLitVolume *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitVolume >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitVolume", ::TLitVolume::Class_Version(), "TLitVolume.h", 26,
                  typeid(::TLitVolume), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitVolume::Dictionary, isa_proxy, 4,
                  sizeof(::TLitVolume) );
      instance.SetNew(&new_TLitVolume);
      instance.SetNewArray(&newArray_TLitVolume);
      instance.SetDelete(&delete_TLitVolume);
      instance.SetDeleteArray(&deleteArray_TLitVolume);
      instance.SetDestructor(&destruct_TLitVolume);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitVolume*)
   {
      return GenerateInitInstanceLocal((::TLitVolume*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitVolume*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLit*)
   {
      ::TLit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLit", ::TLit::Class_Version(), "TLit.h", 23,
                  typeid(::TLit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLit::Dictionary, isa_proxy, 4,
                  sizeof(::TLit) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLit*)
   {
      return GenerateInitInstanceLocal((::TLit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitPhoton(void *p = 0);
   static void *newArray_TLitPhoton(Long_t size, void *p);
   static void delete_TLitPhoton(void *p);
   static void deleteArray_TLitPhoton(void *p);
   static void destruct_TLitPhoton(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitPhoton*)
   {
      ::TLitPhoton *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitPhoton >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitPhoton", ::TLitPhoton::Class_Version(), "TLitPhoton.h", 29,
                  typeid(::TLitPhoton), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitPhoton::Dictionary, isa_proxy, 4,
                  sizeof(::TLitPhoton) );
      instance.SetNew(&new_TLitPhoton);
      instance.SetNewArray(&newArray_TLitPhoton);
      instance.SetDelete(&delete_TLitPhoton);
      instance.SetDeleteArray(&deleteArray_TLitPhoton);
      instance.SetDestructor(&destruct_TLitPhoton);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitPhoton*)
   {
      return GenerateInitInstanceLocal((::TLitPhoton*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitPhoton*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitSpontan(void *p = 0);
   static void *newArray_TLitSpontan(Long_t size, void *p);
   static void delete_TLitSpontan(void *p);
   static void deleteArray_TLitSpontan(void *p);
   static void destruct_TLitSpontan(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitSpontan*)
   {
      ::TLitSpontan *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitSpontan >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitSpontan", ::TLitSpontan::Class_Version(), "TLitSpontan.h", 23,
                  typeid(::TLitSpontan), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitSpontan::Dictionary, isa_proxy, 4,
                  sizeof(::TLitSpontan) );
      instance.SetNew(&new_TLitSpontan);
      instance.SetNewArray(&newArray_TLitSpontan);
      instance.SetDelete(&delete_TLitSpontan);
      instance.SetDeleteArray(&deleteArray_TLitSpontan);
      instance.SetDestructor(&destruct_TLitSpontan);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitSpontan*)
   {
      return GenerateInitInstanceLocal((::TLitSpontan*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitSpontan*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitranino(void *p = 0);
   static void *newArray_TLitranino(Long_t size, void *p);
   static void delete_TLitranino(void *p);
   static void deleteArray_TLitranino(void *p);
   static void destruct_TLitranino(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitranino*)
   {
      ::TLitranino *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitranino >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitranino", ::TLitranino::Class_Version(), "TLitranino.h", 24,
                  typeid(::TLitranino), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitranino::Dictionary, isa_proxy, 4,
                  sizeof(::TLitranino) );
      instance.SetNew(&new_TLitranino);
      instance.SetNewArray(&newArray_TLitranino);
      instance.SetDelete(&delete_TLitranino);
      instance.SetDeleteArray(&deleteArray_TLitranino);
      instance.SetDestructor(&destruct_TLitranino);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitranino*)
   {
      return GenerateInitInstanceLocal((::TLitranino*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitranino*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitParticle(void *p = 0);
   static void *newArray_TLitParticle(Long_t size, void *p);
   static void delete_TLitParticle(void *p);
   static void deleteArray_TLitParticle(void *p);
   static void destruct_TLitParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitParticle*)
   {
      ::TLitParticle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitParticle >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitParticle", ::TLitParticle::Class_Version(), "TLitParticle.h", 20,
                  typeid(::TLitParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitParticle::Dictionary, isa_proxy, 4,
                  sizeof(::TLitParticle) );
      instance.SetNew(&new_TLitParticle);
      instance.SetNewArray(&newArray_TLitParticle);
      instance.SetDelete(&delete_TLitParticle);
      instance.SetDeleteArray(&deleteArray_TLitParticle);
      instance.SetDestructor(&destruct_TLitParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitParticle*)
   {
      return GenerateInitInstanceLocal((::TLitParticle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitParticle*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitBeam(void *p = 0);
   static void *newArray_TLitBeam(Long_t size, void *p);
   static void delete_TLitBeam(void *p);
   static void deleteArray_TLitBeam(void *p);
   static void destruct_TLitBeam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitBeam*)
   {
      ::TLitBeam *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitBeam >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitBeam", ::TLitBeam::Class_Version(), "TLitBeam.h", 26,
                  typeid(::TLitBeam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitBeam::Dictionary, isa_proxy, 4,
                  sizeof(::TLitBeam) );
      instance.SetNew(&new_TLitBeam);
      instance.SetNewArray(&newArray_TLitBeam);
      instance.SetDelete(&delete_TLitBeam);
      instance.SetDeleteArray(&deleteArray_TLitBeam);
      instance.SetDestructor(&destruct_TLitBeam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitBeam*)
   {
      return GenerateInitInstanceLocal((::TLitBeam*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitBeam*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitResults(void *p = 0);
   static void *newArray_TLitResults(Long_t size, void *p);
   static void delete_TLitResults(void *p);
   static void deleteArray_TLitResults(void *p);
   static void destruct_TLitResults(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitResults*)
   {
      ::TLitResults *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitResults >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitResults", ::TLitResults::Class_Version(), "TLitResults.h", 21,
                  typeid(::TLitResults), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitResults::Dictionary, isa_proxy, 4,
                  sizeof(::TLitResults) );
      instance.SetNew(&new_TLitResults);
      instance.SetNewArray(&newArray_TLitResults);
      instance.SetDelete(&delete_TLitResults);
      instance.SetDeleteArray(&deleteArray_TLitResults);
      instance.SetDestructor(&destruct_TLitResults);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitResults*)
   {
      return GenerateInitInstanceLocal((::TLitResults*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitResults*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitMarrow(void *p = 0);
   static void *newArray_TLitMarrow(Long_t size, void *p);
   static void delete_TLitMarrow(void *p);
   static void deleteArray_TLitMarrow(void *p);
   static void destruct_TLitMarrow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitMarrow*)
   {
      ::TLitMarrow *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitMarrow >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitMarrow", ::TLitMarrow::Class_Version(), "TLitMarrow.h", 20,
                  typeid(::TLitMarrow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitMarrow::Dictionary, isa_proxy, 4,
                  sizeof(::TLitMarrow) );
      instance.SetNew(&new_TLitMarrow);
      instance.SetNewArray(&newArray_TLitMarrow);
      instance.SetDelete(&delete_TLitMarrow);
      instance.SetDeleteArray(&deleteArray_TLitMarrow);
      instance.SetDestructor(&destruct_TLitMarrow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitMarrow*)
   {
      return GenerateInitInstanceLocal((::TLitMarrow*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitMarrow*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitCascade(void *p = 0);
   static void *newArray_TLitCascade(Long_t size, void *p);
   static void delete_TLitCascade(void *p);
   static void deleteArray_TLitCascade(void *p);
   static void destruct_TLitCascade(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitCascade*)
   {
      ::TLitCascade *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitCascade >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitCascade", ::TLitCascade::Class_Version(), "TLitCascade.h", 23,
                  typeid(::TLitCascade), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitCascade::Dictionary, isa_proxy, 4,
                  sizeof(::TLitCascade) );
      instance.SetNew(&new_TLitCascade);
      instance.SetNewArray(&newArray_TLitCascade);
      instance.SetDelete(&delete_TLitCascade);
      instance.SetDeleteArray(&deleteArray_TLitCascade);
      instance.SetDestructor(&destruct_TLitCascade);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitCascade*)
   {
      return GenerateInitInstanceLocal((::TLitCascade*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitCascade*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitGamma(void *p = 0);
   static void *newArray_TLitGamma(Long_t size, void *p);
   static void delete_TLitGamma(void *p);
   static void deleteArray_TLitGamma(void *p);
   static void destruct_TLitGamma(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitGamma*)
   {
      ::TLitGamma *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitGamma >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitGamma", ::TLitGamma::Class_Version(), "TLitGamma.h", 24,
                  typeid(::TLitGamma), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitGamma::Dictionary, isa_proxy, 4,
                  sizeof(::TLitGamma) );
      instance.SetNew(&new_TLitGamma);
      instance.SetNewArray(&newArray_TLitGamma);
      instance.SetDelete(&delete_TLitGamma);
      instance.SetDeleteArray(&deleteArray_TLitGamma);
      instance.SetDestructor(&destruct_TLitGamma);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitGamma*)
   {
      return GenerateInitInstanceLocal((::TLitGamma*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitGamma*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLitAlbedo(void *p = 0);
   static void *newArray_TLitAlbedo(Long_t size, void *p);
   static void delete_TLitAlbedo(void *p);
   static void deleteArray_TLitAlbedo(void *p);
   static void destruct_TLitAlbedo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLitAlbedo*)
   {
      ::TLitAlbedo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLitAlbedo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLitAlbedo", ::TLitAlbedo::Class_Version(), "TLitAlbedo.h", 26,
                  typeid(::TLitAlbedo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLitAlbedo::Dictionary, isa_proxy, 4,
                  sizeof(::TLitAlbedo) );
      instance.SetNew(&new_TLitAlbedo);
      instance.SetNewArray(&newArray_TLitAlbedo);
      instance.SetDelete(&delete_TLitAlbedo);
      instance.SetDeleteArray(&deleteArray_TLitAlbedo);
      instance.SetDestructor(&destruct_TLitAlbedo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLitAlbedo*)
   {
      return GenerateInitInstanceLocal((::TLitAlbedo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLitAlbedo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TLitPol3::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitPol3::Class_Name()
{
   return "TLitPol3";
}

//______________________________________________________________________________
const char *TLitPol3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPol3*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitPol3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPol3*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitPol3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPol3*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitPol3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPol3*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitPol4::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitPol4::Class_Name()
{
   return "TLitPol4";
}

//______________________________________________________________________________
const char *TLitPol4::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPol4*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitPol4::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPol4*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitPol4::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPol4*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitPol4::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPol4*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitHistParams::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitHistParams::Class_Name()
{
   return "TLitHistParams";
}

//______________________________________________________________________________
const char *TLitHistParams::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitHistParams*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitHistParams::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitHistParams*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitHistParams::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitHistParams*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitHistParams::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitHistParams*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitPhys::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitPhys::Class_Name()
{
   return "TLitPhys";
}

//______________________________________________________________________________
const char *TLitPhys::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPhys*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitPhys::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPhys*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitPhys::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPhys*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitPhys::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPhys*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitWave::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitWave::Class_Name()
{
   return "TLitWave";
}

//______________________________________________________________________________
const char *TLitWave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitWave*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitWave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitWave*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitWave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitWave*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitWave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitWave*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitEqIndex::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitEqIndex::Class_Name()
{
   return "TLitEqIndex";
}

//______________________________________________________________________________
const char *TLitEqIndex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitEqIndex*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitEqIndex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitEqIndex*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitEqIndex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitEqIndex*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitEqIndex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitEqIndex*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitTrap::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitTrap::Class_Name()
{
   return "TLitTrap";
}

//______________________________________________________________________________
const char *TLitTrap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitTrap*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitTrap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitTrap*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitTrap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitTrap*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitTrap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitTrap*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitSpectrumCp::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitSpectrumCp::Class_Name()
{
   return "TLitSpectrumCp";
}

//______________________________________________________________________________
const char *TLitSpectrumCp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrumCp*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitSpectrumCp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrumCp*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitSpectrumCp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrumCp*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitSpectrumCp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrumCp*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitSpectrum::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitSpectrum::Class_Name()
{
   return "TLitSpectrum";
}

//______________________________________________________________________________
const char *TLitSpectrum::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrum*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitSpectrum::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrum*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitSpectrum::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrum*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitSpectrum::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpectrum*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitDetector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitDetector::Class_Name()
{
   return "TLitDetector";
}

//______________________________________________________________________________
const char *TLitDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitDetector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitDetector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitDetector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitDetector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitSellmeier::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitSellmeier::Class_Name()
{
   return "TLitSellmeier";
}

//______________________________________________________________________________
const char *TLitSellmeier::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSellmeier*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitSellmeier::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSellmeier*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitSellmeier::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSellmeier*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitSellmeier::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSellmeier*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitMedium::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitMedium::Class_Name()
{
   return "TLitMedium";
}

//______________________________________________________________________________
const char *TLitMedium::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitMedium*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitMedium::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitMedium*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitMedium::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitMedium*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitMedium::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitMedium*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitVolume::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitVolume::Class_Name()
{
   return "TLitVolume";
}

//______________________________________________________________________________
const char *TLitVolume::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitVolume*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitVolume::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitVolume*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitVolume::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitVolume*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitVolume::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitVolume*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLit::Class_Name()
{
   return "TLit";
}

//______________________________________________________________________________
const char *TLit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitPhoton::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitPhoton::Class_Name()
{
   return "TLitPhoton";
}

//______________________________________________________________________________
const char *TLitPhoton::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPhoton*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitPhoton::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitPhoton*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitPhoton::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPhoton*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitPhoton::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitPhoton*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitSpontan::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitSpontan::Class_Name()
{
   return "TLitSpontan";
}

//______________________________________________________________________________
const char *TLitSpontan::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpontan*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitSpontan::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitSpontan*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitSpontan::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpontan*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitSpontan::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitSpontan*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitranino::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitranino::Class_Name()
{
   return "TLitranino";
}

//______________________________________________________________________________
const char *TLitranino::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitranino*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitranino::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitranino*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitranino::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitranino*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitranino::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitranino*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitParticle::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitParticle::Class_Name()
{
   return "TLitParticle";
}

//______________________________________________________________________________
const char *TLitParticle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitParticle*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitParticle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitParticle*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitParticle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitParticle*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitParticle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitParticle*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitBeam::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitBeam::Class_Name()
{
   return "TLitBeam";
}

//______________________________________________________________________________
const char *TLitBeam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitBeam*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitBeam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitBeam*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitBeam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitBeam*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitBeam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitBeam*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitResults::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitResults::Class_Name()
{
   return "TLitResults";
}

//______________________________________________________________________________
const char *TLitResults::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitResults*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitResults::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitResults*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitResults::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitResults*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitResults::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitResults*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitMarrow::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitMarrow::Class_Name()
{
   return "TLitMarrow";
}

//______________________________________________________________________________
const char *TLitMarrow::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitMarrow*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitMarrow::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitMarrow*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitMarrow::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitMarrow*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitMarrow::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitMarrow*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitCascade::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitCascade::Class_Name()
{
   return "TLitCascade";
}

//______________________________________________________________________________
const char *TLitCascade::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitCascade*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitCascade::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitCascade*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitCascade::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitCascade*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitCascade::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitCascade*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitGamma::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitGamma::Class_Name()
{
   return "TLitGamma";
}

//______________________________________________________________________________
const char *TLitGamma::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitGamma*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitGamma::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitGamma*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitGamma::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitGamma*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitGamma::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitGamma*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLitAlbedo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLitAlbedo::Class_Name()
{
   return "TLitAlbedo";
}

//______________________________________________________________________________
const char *TLitAlbedo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitAlbedo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLitAlbedo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLitAlbedo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLitAlbedo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitAlbedo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLitAlbedo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLitAlbedo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TLitPol3::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitPol3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitPol3::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitPol3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitPol3(void *p) {
      return  p ? new(p) ::TLitPol3 : new ::TLitPol3;
   }
   static void *newArray_TLitPol3(Long_t nElements, void *p) {
      return p ? new(p) ::TLitPol3[nElements] : new ::TLitPol3[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitPol3(void *p) {
      delete ((::TLitPol3*)p);
   }
   static void deleteArray_TLitPol3(void *p) {
      delete [] ((::TLitPol3*)p);
   }
   static void destruct_TLitPol3(void *p) {
      typedef ::TLitPol3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitPol3

//______________________________________________________________________________
void TLitPol4::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitPol4.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitPol4::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitPol4::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitPol4(void *p) {
      return  p ? new(p) ::TLitPol4 : new ::TLitPol4;
   }
   static void *newArray_TLitPol4(Long_t nElements, void *p) {
      return p ? new(p) ::TLitPol4[nElements] : new ::TLitPol4[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitPol4(void *p) {
      delete ((::TLitPol4*)p);
   }
   static void deleteArray_TLitPol4(void *p) {
      delete [] ((::TLitPol4*)p);
   }
   static void destruct_TLitPol4(void *p) {
      typedef ::TLitPol4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitPol4

//______________________________________________________________________________
void TLitHistParams::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitHistParams.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitHistParams::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitHistParams::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitHistParams(void *p) {
      return  p ? new(p) ::TLitHistParams : new ::TLitHistParams;
   }
   static void *newArray_TLitHistParams(Long_t nElements, void *p) {
      return p ? new(p) ::TLitHistParams[nElements] : new ::TLitHistParams[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitHistParams(void *p) {
      delete ((::TLitHistParams*)p);
   }
   static void deleteArray_TLitHistParams(void *p) {
      delete [] ((::TLitHistParams*)p);
   }
   static void destruct_TLitHistParams(void *p) {
      typedef ::TLitHistParams current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitHistParams

//______________________________________________________________________________
void TLitPhys::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitPhys.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitPhys::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitPhys::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::TLitPhys

//______________________________________________________________________________
void TLitWave::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitWave.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitWave::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitWave::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitWave(void *p) {
      return  p ? new(p) ::TLitWave : new ::TLitWave;
   }
   static void *newArray_TLitWave(Long_t nElements, void *p) {
      return p ? new(p) ::TLitWave[nElements] : new ::TLitWave[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitWave(void *p) {
      delete ((::TLitWave*)p);
   }
   static void deleteArray_TLitWave(void *p) {
      delete [] ((::TLitWave*)p);
   }
   static void destruct_TLitWave(void *p) {
      typedef ::TLitWave current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitWave

//______________________________________________________________________________
void TLitEqIndex::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitEqIndex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitEqIndex::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitEqIndex::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitEqIndex(void *p) {
      return  p ? new(p) ::TLitEqIndex : new ::TLitEqIndex;
   }
   static void *newArray_TLitEqIndex(Long_t nElements, void *p) {
      return p ? new(p) ::TLitEqIndex[nElements] : new ::TLitEqIndex[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitEqIndex(void *p) {
      delete ((::TLitEqIndex*)p);
   }
   static void deleteArray_TLitEqIndex(void *p) {
      delete [] ((::TLitEqIndex*)p);
   }
   static void destruct_TLitEqIndex(void *p) {
      typedef ::TLitEqIndex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitEqIndex

//______________________________________________________________________________
void TLitTrap::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitTrap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitTrap::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitTrap::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitTrap(void *p) {
      return  p ? new(p) ::TLitTrap : new ::TLitTrap;
   }
   static void *newArray_TLitTrap(Long_t nElements, void *p) {
      return p ? new(p) ::TLitTrap[nElements] : new ::TLitTrap[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitTrap(void *p) {
      delete ((::TLitTrap*)p);
   }
   static void deleteArray_TLitTrap(void *p) {
      delete [] ((::TLitTrap*)p);
   }
   static void destruct_TLitTrap(void *p) {
      typedef ::TLitTrap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitTrap

//______________________________________________________________________________
void TLitSpectrumCp::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitSpectrumCp.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitSpectrumCp::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitSpectrumCp::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitSpectrumCp(void *p) {
      return  p ? new(p) ::TLitSpectrumCp : new ::TLitSpectrumCp;
   }
   static void *newArray_TLitSpectrumCp(Long_t nElements, void *p) {
      return p ? new(p) ::TLitSpectrumCp[nElements] : new ::TLitSpectrumCp[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitSpectrumCp(void *p) {
      delete ((::TLitSpectrumCp*)p);
   }
   static void deleteArray_TLitSpectrumCp(void *p) {
      delete [] ((::TLitSpectrumCp*)p);
   }
   static void destruct_TLitSpectrumCp(void *p) {
      typedef ::TLitSpectrumCp current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitSpectrumCp

//______________________________________________________________________________
void TLitSpectrum::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitSpectrum.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitSpectrum::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitSpectrum::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitSpectrum(void *p) {
      return  p ? new(p) ::TLitSpectrum : new ::TLitSpectrum;
   }
   static void *newArray_TLitSpectrum(Long_t nElements, void *p) {
      return p ? new(p) ::TLitSpectrum[nElements] : new ::TLitSpectrum[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitSpectrum(void *p) {
      delete ((::TLitSpectrum*)p);
   }
   static void deleteArray_TLitSpectrum(void *p) {
      delete [] ((::TLitSpectrum*)p);
   }
   static void destruct_TLitSpectrum(void *p) {
      typedef ::TLitSpectrum current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitSpectrum

//______________________________________________________________________________
void TLitDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitDetector(void *p) {
      return  p ? new(p) ::TLitDetector : new ::TLitDetector;
   }
   static void *newArray_TLitDetector(Long_t nElements, void *p) {
      return p ? new(p) ::TLitDetector[nElements] : new ::TLitDetector[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitDetector(void *p) {
      delete ((::TLitDetector*)p);
   }
   static void deleteArray_TLitDetector(void *p) {
      delete [] ((::TLitDetector*)p);
   }
   static void destruct_TLitDetector(void *p) {
      typedef ::TLitDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitDetector

//______________________________________________________________________________
void TLitSellmeier::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitSellmeier.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitSellmeier::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitSellmeier::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitSellmeier(void *p) {
      return  p ? new(p) ::TLitSellmeier : new ::TLitSellmeier;
   }
   static void *newArray_TLitSellmeier(Long_t nElements, void *p) {
      return p ? new(p) ::TLitSellmeier[nElements] : new ::TLitSellmeier[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitSellmeier(void *p) {
      delete ((::TLitSellmeier*)p);
   }
   static void deleteArray_TLitSellmeier(void *p) {
      delete [] ((::TLitSellmeier*)p);
   }
   static void destruct_TLitSellmeier(void *p) {
      typedef ::TLitSellmeier current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitSellmeier

//______________________________________________________________________________
void TLitMedium::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitMedium.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitMedium::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitMedium::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitMedium(void *p) {
      return  p ? new(p) ::TLitMedium : new ::TLitMedium;
   }
   static void *newArray_TLitMedium(Long_t nElements, void *p) {
      return p ? new(p) ::TLitMedium[nElements] : new ::TLitMedium[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitMedium(void *p) {
      delete ((::TLitMedium*)p);
   }
   static void deleteArray_TLitMedium(void *p) {
      delete [] ((::TLitMedium*)p);
   }
   static void destruct_TLitMedium(void *p) {
      typedef ::TLitMedium current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitMedium

//______________________________________________________________________________
void TLitVolume::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitVolume.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitVolume::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitVolume::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitVolume(void *p) {
      return  p ? new(p) ::TLitVolume : new ::TLitVolume;
   }
   static void *newArray_TLitVolume(Long_t nElements, void *p) {
      return p ? new(p) ::TLitVolume[nElements] : new ::TLitVolume[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitVolume(void *p) {
      delete ((::TLitVolume*)p);
   }
   static void deleteArray_TLitVolume(void *p) {
      delete [] ((::TLitVolume*)p);
   }
   static void destruct_TLitVolume(void *p) {
      typedef ::TLitVolume current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitVolume

//______________________________________________________________________________
void TLit::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLit::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLit::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::TLit

//______________________________________________________________________________
void TLitPhoton::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitPhoton.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitPhoton::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitPhoton::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitPhoton(void *p) {
      return  p ? new(p) ::TLitPhoton : new ::TLitPhoton;
   }
   static void *newArray_TLitPhoton(Long_t nElements, void *p) {
      return p ? new(p) ::TLitPhoton[nElements] : new ::TLitPhoton[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitPhoton(void *p) {
      delete ((::TLitPhoton*)p);
   }
   static void deleteArray_TLitPhoton(void *p) {
      delete [] ((::TLitPhoton*)p);
   }
   static void destruct_TLitPhoton(void *p) {
      typedef ::TLitPhoton current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitPhoton

//______________________________________________________________________________
void TLitSpontan::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitSpontan.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitSpontan::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitSpontan::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitSpontan(void *p) {
      return  p ? new(p) ::TLitSpontan : new ::TLitSpontan;
   }
   static void *newArray_TLitSpontan(Long_t nElements, void *p) {
      return p ? new(p) ::TLitSpontan[nElements] : new ::TLitSpontan[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitSpontan(void *p) {
      delete ((::TLitSpontan*)p);
   }
   static void deleteArray_TLitSpontan(void *p) {
      delete [] ((::TLitSpontan*)p);
   }
   static void destruct_TLitSpontan(void *p) {
      typedef ::TLitSpontan current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitSpontan

//______________________________________________________________________________
void TLitranino::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitranino.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitranino::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitranino::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitranino(void *p) {
      return  p ? new(p) ::TLitranino : new ::TLitranino;
   }
   static void *newArray_TLitranino(Long_t nElements, void *p) {
      return p ? new(p) ::TLitranino[nElements] : new ::TLitranino[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitranino(void *p) {
      delete ((::TLitranino*)p);
   }
   static void deleteArray_TLitranino(void *p) {
      delete [] ((::TLitranino*)p);
   }
   static void destruct_TLitranino(void *p) {
      typedef ::TLitranino current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitranino

//______________________________________________________________________________
void TLitParticle::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitParticle.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitParticle::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitParticle::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitParticle(void *p) {
      return  p ? new(p) ::TLitParticle : new ::TLitParticle;
   }
   static void *newArray_TLitParticle(Long_t nElements, void *p) {
      return p ? new(p) ::TLitParticle[nElements] : new ::TLitParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitParticle(void *p) {
      delete ((::TLitParticle*)p);
   }
   static void deleteArray_TLitParticle(void *p) {
      delete [] ((::TLitParticle*)p);
   }
   static void destruct_TLitParticle(void *p) {
      typedef ::TLitParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitParticle

//______________________________________________________________________________
void TLitBeam::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitBeam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitBeam::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitBeam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitBeam(void *p) {
      return  p ? new(p) ::TLitBeam : new ::TLitBeam;
   }
   static void *newArray_TLitBeam(Long_t nElements, void *p) {
      return p ? new(p) ::TLitBeam[nElements] : new ::TLitBeam[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitBeam(void *p) {
      delete ((::TLitBeam*)p);
   }
   static void deleteArray_TLitBeam(void *p) {
      delete [] ((::TLitBeam*)p);
   }
   static void destruct_TLitBeam(void *p) {
      typedef ::TLitBeam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitBeam

//______________________________________________________________________________
void TLitResults::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitResults.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitResults::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitResults::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitResults(void *p) {
      return  p ? new(p) ::TLitResults : new ::TLitResults;
   }
   static void *newArray_TLitResults(Long_t nElements, void *p) {
      return p ? new(p) ::TLitResults[nElements] : new ::TLitResults[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitResults(void *p) {
      delete ((::TLitResults*)p);
   }
   static void deleteArray_TLitResults(void *p) {
      delete [] ((::TLitResults*)p);
   }
   static void destruct_TLitResults(void *p) {
      typedef ::TLitResults current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitResults

//______________________________________________________________________________
void TLitMarrow::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitMarrow.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitMarrow::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitMarrow::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitMarrow(void *p) {
      return  p ? new(p) ::TLitMarrow : new ::TLitMarrow;
   }
   static void *newArray_TLitMarrow(Long_t nElements, void *p) {
      return p ? new(p) ::TLitMarrow[nElements] : new ::TLitMarrow[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitMarrow(void *p) {
      delete ((::TLitMarrow*)p);
   }
   static void deleteArray_TLitMarrow(void *p) {
      delete [] ((::TLitMarrow*)p);
   }
   static void destruct_TLitMarrow(void *p) {
      typedef ::TLitMarrow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitMarrow

//______________________________________________________________________________
void TLitCascade::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitCascade.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitCascade::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitCascade::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitCascade(void *p) {
      return  p ? new(p) ::TLitCascade : new ::TLitCascade;
   }
   static void *newArray_TLitCascade(Long_t nElements, void *p) {
      return p ? new(p) ::TLitCascade[nElements] : new ::TLitCascade[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitCascade(void *p) {
      delete ((::TLitCascade*)p);
   }
   static void deleteArray_TLitCascade(void *p) {
      delete [] ((::TLitCascade*)p);
   }
   static void destruct_TLitCascade(void *p) {
      typedef ::TLitCascade current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitCascade

//______________________________________________________________________________
void TLitGamma::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitGamma.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitGamma::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitGamma::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitGamma(void *p) {
      return  p ? new(p) ::TLitGamma : new ::TLitGamma;
   }
   static void *newArray_TLitGamma(Long_t nElements, void *p) {
      return p ? new(p) ::TLitGamma[nElements] : new ::TLitGamma[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitGamma(void *p) {
      delete ((::TLitGamma*)p);
   }
   static void deleteArray_TLitGamma(void *p) {
      delete [] ((::TLitGamma*)p);
   }
   static void destruct_TLitGamma(void *p) {
      typedef ::TLitGamma current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitGamma

//______________________________________________________________________________
void TLitAlbedo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLitAlbedo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLitAlbedo::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLitAlbedo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLitAlbedo(void *p) {
      return  p ? new(p) ::TLitAlbedo : new ::TLitAlbedo;
   }
   static void *newArray_TLitAlbedo(Long_t nElements, void *p) {
      return p ? new(p) ::TLitAlbedo[nElements] : new ::TLitAlbedo[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLitAlbedo(void *p) {
      delete ((::TLitAlbedo*)p);
   }
   static void deleteArray_TLitAlbedo(void *p) {
      delete [] ((::TLitAlbedo*)p);
   }
   static void destruct_TLitAlbedo(void *p) {
      typedef ::TLitAlbedo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLitAlbedo

namespace {
  void TriggerDictionaryInitialization_SLitraniDict_Impl() {
    static const char* headers[] = {
"TLitPol3.h",
"TLitPol4.h",
"TLitEqIndex.h",
"TLitWave.h",
"TLitHistParams.h",
"TLitPhys.h",
"TLitTrap.h",
"TLitSpectrumCp.h",
"TLitSpectrum.h",
"TLitDetector.h",
"TLitSellmeier.h",
"TLitMedium.h",
"TLitVolume.h",
"TLit.h",
"TLitPhoton.h",
"TLitSpontan.h",
"TLitranino.h",
"TLitResults.h",
"TLitMarrow.h",
"TLitParticle.h",
"TLitBeam.h",
"TLitCascade.h",
"TLitGamma.h",
"TLitAlbedo.h",
0
    };
    static const char* includePaths[] = {
"/home/serena/Software/SLITRANI/rootdev/include",
"/home/serena/Software/root-install/include",
"/home/serena/Software/SLITRANI/slitrani-fixed/SLitrani/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SLitraniDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Handling of 3rd degree polynomials)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitPol3.h")))  TLitPol3;
class __attribute__((annotate(R"ATTRDUMP(Handle polynomials of 4th degree)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitPol4.h")))  TLitPol4;
class __attribute__((annotate(R"ATTRDUMP(Histogram parameters for SLitrani into xml file)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitHistParams.h")))  TLitHistParams;
class __attribute__((annotate(R"ATTRDUMP(All physical constants used by SLitrani.)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitPhys.h")))  TLitPhys;
class __attribute__((annotate(R"ATTRDUMP(characteristics of el-magn. wave)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitWave.h")))  TLitWave;
class __attribute__((annotate(R"ATTRDUMP(Equation giving the 2 indices in case of anisotropy)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitEqIndex.h")))  TLitEqIndex;
class __attribute__((annotate(R"ATTRDUMP(Help for defining TGeoTrap)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitTrap.h")))  TLitTrap;
class __attribute__((annotate(R"ATTRDUMP(One gaussian component of the emission spectrum TLitSpectrum of an optical material)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitSpectrumCp.h")))  TLitSpectrumCp;
class __attribute__((annotate(R"ATTRDUMP(Emission spectrum of an optical material)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitSpectrum.h")))  TLitSpectrum;
class __attribute__((annotate(R"ATTRDUMP(Contains all statistics and histograms for a detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitDetector.h")))  TLitDetector;
class __attribute__((annotate(R"ATTRDUMP(Defines LitSellmeier law)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitSellmeier.h")))  TLitSellmeier;
class __attribute__((annotate(R"ATTRDUMP(Add optical properties to a TGeoMedium)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitMedium.h")))  TLitMedium;
class __attribute__((annotate(R"ATTRDUMP(Addition to TGeoVolume needed by SLitrani)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitVolume.h")))  TLitVolume;
class __attribute__((annotate(R"ATTRDUMP(Steering class of SLitrani)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLit.h")))  TLit;
class __attribute__((annotate(R"ATTRDUMP(Full description of a photon inside the set-up)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitPhoton.h")))  TLitPhoton;
class __attribute__((annotate(R"ATTRDUMP(Spontaneous source of photon inside a TGeoVolume)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitSpontan.h")))  TLitSpontan;
class __attribute__((annotate(R"ATTRDUMP(Simplest possible particle to test SLitrani)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitranino.h")))  TLitranino;
class __attribute__((annotate(R"ATTRDUMP(Define a particle which will generate photons)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitParticle.h")))  TLitParticle;
class __attribute__((annotate(R"ATTRDUMP(Defines a beam of particles)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitBeam.h")))  TLitBeam;
class __attribute__((annotate(R"ATTRDUMP(Summary of all statistics and histograms for one run)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitResults.h")))  TLitResults;
class __attribute__((annotate(R"ATTRDUMP(Make the final analysis.)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitMarrow.h")))  TLitMarrow;
class __attribute__((annotate(R"ATTRDUMP(Generation of electromagnetic showers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitCascade.h")))  TLitCascade;
class __attribute__((annotate(R"ATTRDUMP(Generation of gamma rays of energy less than ~1 Mev)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitGamma.h")))  TLitGamma;
class __attribute__((annotate(R"ATTRDUMP(Complex index of wrapping from albedo)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TLitAlbedo.h")))  TLitAlbedo;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SLitraniDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TLitPol3.h"
#include "TLitPol4.h"
#include "TLitEqIndex.h"
#include "TLitWave.h"
#include "TLitHistParams.h"
#include "TLitPhys.h"
#include "TLitTrap.h"
#include "TLitSpectrumCp.h"
#include "TLitSpectrum.h"
#include "TLitDetector.h"
#include "TLitSellmeier.h"
#include "TLitMedium.h"
#include "TLitVolume.h"
#include "TLit.h"
#include "TLitPhoton.h"
#include "TLitSpontan.h"
#include "TLitranino.h"
#include "TLitResults.h"
#include "TLitMarrow.h"
#include "TLitParticle.h"
#include "TLitBeam.h"
#include "TLitCascade.h"
#include "TLitGamma.h"
#include "TLitAlbedo.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"KindOfDist", payloadCode, "@",
"LitMemForDet", payloadCode, "@",
"PDistribution", payloadCode, "@",
"TLit", payloadCode, "@",
"TLitAlbedo", payloadCode, "@",
"TLitBeam", payloadCode, "@",
"TLitCascade", payloadCode, "@",
"TLitDetector", payloadCode, "@",
"TLitEqIndex", payloadCode, "@",
"TLitGamma", payloadCode, "@",
"TLitHistParams", payloadCode, "@",
"TLitMarrow", payloadCode, "@",
"TLitMedium", payloadCode, "@",
"TLitParticle", payloadCode, "@",
"TLitPhoton", payloadCode, "@",
"TLitPhys", payloadCode, "@",
"TLitPol3", payloadCode, "@",
"TLitPol4", payloadCode, "@",
"TLitResults", payloadCode, "@",
"TLitSellmeier", payloadCode, "@",
"TLitSpectrum", payloadCode, "@",
"TLitSpectrumCp", payloadCode, "@",
"TLitSpontan", payloadCode, "@",
"TLitTrap", payloadCode, "@",
"TLitVolume", payloadCode, "@",
"TLitWave", payloadCode, "@",
"TLitranino", payloadCode, "@",
"gLitCs", payloadCode, "@",
"gLitGp", payloadCode, "@",
"gLitGs", payloadCode, "@",
"gLitIdx", payloadCode, "@",
"gRandom3", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SLitraniDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SLitraniDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SLitraniDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SLitraniDict() {
  TriggerDictionaryInitialization_SLitraniDict_Impl();
}
