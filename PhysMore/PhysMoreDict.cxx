// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PhysMoreDict

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
#include "TArrayZ.h"
#include "TNArray.h"
#include "TCleanOut.h"
#include "TMat33.h"
#include "TEuler.h"
#include "TVec3C.h"
#include "TMat33C.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TArrayZ(void *p = 0);
   static void *newArray_TArrayZ(Long_t size, void *p);
   static void delete_TArrayZ(void *p);
   static void deleteArray_TArrayZ(void *p);
   static void destruct_TArrayZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TArrayZ*)
   {
      ::TArrayZ *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TArrayZ >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TArrayZ", ::TArrayZ::Class_Version(), "TArrayZ.h", 18,
                  typeid(::TArrayZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TArrayZ::Dictionary, isa_proxy, 4,
                  sizeof(::TArrayZ) );
      instance.SetNew(&new_TArrayZ);
      instance.SetNewArray(&newArray_TArrayZ);
      instance.SetDelete(&delete_TArrayZ);
      instance.SetDeleteArray(&deleteArray_TArrayZ);
      instance.SetDestructor(&destruct_TArrayZ);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TArrayZ*)
   {
      return GenerateInitInstanceLocal((::TArrayZ*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TArrayZ*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylEchargR_Dictionary();
   static void TNArraylEchargR_TClassManip(TClass*);
   static void *new_TNArraylEchargR(void *p = 0);
   static void *newArray_TNArraylEchargR(Long_t size, void *p);
   static void delete_TNArraylEchargR(void *p);
   static void deleteArray_TNArraylEchargR(void *p);
   static void destruct_TNArraylEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<char>*)
   {
      ::TNArray<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<char>", ::TNArray<char>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<char>) );
      instance.SetNew(&new_TNArraylEchargR);
      instance.SetNewArray(&newArray_TNArraylEchargR);
      instance.SetDelete(&delete_TNArraylEchargR);
      instance.SetDeleteArray(&deleteArray_TNArraylEchargR);
      instance.SetDestructor(&destruct_TNArraylEchargR);

      ::ROOT::AddClassAlternate("TNArray<char>","TNArray<Char_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<char>*)
   {
      return GenerateInitInstanceLocal((::TNArray<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<char>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<char>*)0x0)->GetClass();
      TNArraylEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylEshortgR_Dictionary();
   static void TNArraylEshortgR_TClassManip(TClass*);
   static void *new_TNArraylEshortgR(void *p = 0);
   static void *newArray_TNArraylEshortgR(Long_t size, void *p);
   static void delete_TNArraylEshortgR(void *p);
   static void deleteArray_TNArraylEshortgR(void *p);
   static void destruct_TNArraylEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<short>*)
   {
      ::TNArray<short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<short>", ::TNArray<short>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<short>) );
      instance.SetNew(&new_TNArraylEshortgR);
      instance.SetNewArray(&newArray_TNArraylEshortgR);
      instance.SetDelete(&delete_TNArraylEshortgR);
      instance.SetDeleteArray(&deleteArray_TNArraylEshortgR);
      instance.SetDestructor(&destruct_TNArraylEshortgR);

      ::ROOT::AddClassAlternate("TNArray<short>","TNArray<Short_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<short>*)
   {
      return GenerateInitInstanceLocal((::TNArray<short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<short>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<short>*)0x0)->GetClass();
      TNArraylEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylEintgR_Dictionary();
   static void TNArraylEintgR_TClassManip(TClass*);
   static void *new_TNArraylEintgR(void *p = 0);
   static void *newArray_TNArraylEintgR(Long_t size, void *p);
   static void delete_TNArraylEintgR(void *p);
   static void deleteArray_TNArraylEintgR(void *p);
   static void destruct_TNArraylEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<int>*)
   {
      ::TNArray<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<int>", ::TNArray<int>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<int>) );
      instance.SetNew(&new_TNArraylEintgR);
      instance.SetNewArray(&newArray_TNArraylEintgR);
      instance.SetDelete(&delete_TNArraylEintgR);
      instance.SetDeleteArray(&deleteArray_TNArraylEintgR);
      instance.SetDestructor(&destruct_TNArraylEintgR);

      ::ROOT::AddClassAlternate("TNArray<int>","TNArray<Int_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<int>*)
   {
      return GenerateInitInstanceLocal((::TNArray<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<int>*)0x0)->GetClass();
      TNArraylEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylElonggR_Dictionary();
   static void TNArraylElonggR_TClassManip(TClass*);
   static void *new_TNArraylElonggR(void *p = 0);
   static void *newArray_TNArraylElonggR(Long_t size, void *p);
   static void delete_TNArraylElonggR(void *p);
   static void deleteArray_TNArraylElonggR(void *p);
   static void destruct_TNArraylElonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<long>*)
   {
      ::TNArray<long> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<long> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<long>", ::TNArray<long>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylElonggR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<long>) );
      instance.SetNew(&new_TNArraylElonggR);
      instance.SetNewArray(&newArray_TNArraylElonggR);
      instance.SetDelete(&delete_TNArraylElonggR);
      instance.SetDeleteArray(&deleteArray_TNArraylElonggR);
      instance.SetDestructor(&destruct_TNArraylElonggR);

      ::ROOT::AddClassAlternate("TNArray<long>","TNArray<Long_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<long>*)
   {
      return GenerateInitInstanceLocal((::TNArray<long>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<long>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylElonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<long>*)0x0)->GetClass();
      TNArraylElonggR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylElonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylELong64_tgR_Dictionary();
   static void TNArraylELong64_tgR_TClassManip(TClass*);
   static void *new_TNArraylELong64_tgR(void *p = 0);
   static void *newArray_TNArraylELong64_tgR(Long_t size, void *p);
   static void delete_TNArraylELong64_tgR(void *p);
   static void deleteArray_TNArraylELong64_tgR(void *p);
   static void destruct_TNArraylELong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<Long64_t>*)
   {
      ::TNArray<Long64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<Long64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<Long64_t>", ::TNArray<Long64_t>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<Long64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylELong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<Long64_t>) );
      instance.SetNew(&new_TNArraylELong64_tgR);
      instance.SetNewArray(&newArray_TNArraylELong64_tgR);
      instance.SetDelete(&delete_TNArraylELong64_tgR);
      instance.SetDeleteArray(&deleteArray_TNArraylELong64_tgR);
      instance.SetDestructor(&destruct_TNArraylELong64_tgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<Long64_t>*)
   {
      return GenerateInitInstanceLocal((::TNArray<Long64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylELong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0)->GetClass();
      TNArraylELong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylELong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylEfloatgR_Dictionary();
   static void TNArraylEfloatgR_TClassManip(TClass*);
   static void *new_TNArraylEfloatgR(void *p = 0);
   static void *newArray_TNArraylEfloatgR(Long_t size, void *p);
   static void delete_TNArraylEfloatgR(void *p);
   static void deleteArray_TNArraylEfloatgR(void *p);
   static void destruct_TNArraylEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<float>*)
   {
      ::TNArray<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<float>", ::TNArray<float>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<float>) );
      instance.SetNew(&new_TNArraylEfloatgR);
      instance.SetNewArray(&newArray_TNArraylEfloatgR);
      instance.SetDelete(&delete_TNArraylEfloatgR);
      instance.SetDeleteArray(&deleteArray_TNArraylEfloatgR);
      instance.SetDestructor(&destruct_TNArraylEfloatgR);

      ::ROOT::AddClassAlternate("TNArray<float>","TNArray<Float_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<float>*)
   {
      return GenerateInitInstanceLocal((::TNArray<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<float>*)0x0)->GetClass();
      TNArraylEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylEdoublegR_Dictionary();
   static void TNArraylEdoublegR_TClassManip(TClass*);
   static void *new_TNArraylEdoublegR(void *p = 0);
   static void *newArray_TNArraylEdoublegR(Long_t size, void *p);
   static void delete_TNArraylEdoublegR(void *p);
   static void deleteArray_TNArraylEdoublegR(void *p);
   static void destruct_TNArraylEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<double>*)
   {
      ::TNArray<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<double>", ::TNArray<double>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<double>) );
      instance.SetNew(&new_TNArraylEdoublegR);
      instance.SetNewArray(&newArray_TNArraylEdoublegR);
      instance.SetDelete(&delete_TNArraylEdoublegR);
      instance.SetDeleteArray(&deleteArray_TNArraylEdoublegR);
      instance.SetDestructor(&destruct_TNArraylEdoublegR);

      ::ROOT::AddClassAlternate("TNArray<double>","TNArray<Double_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<double>*)
   {
      return GenerateInitInstanceLocal((::TNArray<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<double>*)0x0)->GetClass();
      TNArraylEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TNArraylETComplexgR_Dictionary();
   static void TNArraylETComplexgR_TClassManip(TClass*);
   static void *new_TNArraylETComplexgR(void *p = 0);
   static void *newArray_TNArraylETComplexgR(Long_t size, void *p);
   static void delete_TNArraylETComplexgR(void *p);
   static void deleteArray_TNArraylETComplexgR(void *p);
   static void destruct_TNArraylETComplexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TNArray<TComplex>*)
   {
      ::TNArray<TComplex> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TNArray<TComplex> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TNArray<TComplex>", ::TNArray<TComplex>::Class_Version(), "TNArray.h", 17,
                  typeid(::TNArray<TComplex>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TNArraylETComplexgR_Dictionary, isa_proxy, 4,
                  sizeof(::TNArray<TComplex>) );
      instance.SetNew(&new_TNArraylETComplexgR);
      instance.SetNewArray(&newArray_TNArraylETComplexgR);
      instance.SetDelete(&delete_TNArraylETComplexgR);
      instance.SetDeleteArray(&deleteArray_TNArraylETComplexgR);
      instance.SetDestructor(&destruct_TNArraylETComplexgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TNArray<TComplex>*)
   {
      return GenerateInitInstanceLocal((::TNArray<TComplex>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TNArraylETComplexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0)->GetClass();
      TNArraylETComplexgR_TClassManip(theClass);
   return theClass;
   }

   static void TNArraylETComplexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TCleanOut(void *p = 0);
   static void *newArray_TCleanOut(Long_t size, void *p);
   static void delete_TCleanOut(void *p);
   static void deleteArray_TCleanOut(void *p);
   static void destruct_TCleanOut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TCleanOut*)
   {
      ::TCleanOut *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TCleanOut >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TCleanOut", ::TCleanOut::Class_Version(), "TCleanOut.h", 22,
                  typeid(::TCleanOut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TCleanOut::Dictionary, isa_proxy, 4,
                  sizeof(::TCleanOut) );
      instance.SetNew(&new_TCleanOut);
      instance.SetNewArray(&newArray_TCleanOut);
      instance.SetDelete(&delete_TCleanOut);
      instance.SetDeleteArray(&deleteArray_TCleanOut);
      instance.SetDestructor(&destruct_TCleanOut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TCleanOut*)
   {
      return GenerateInitInstanceLocal((::TCleanOut*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TCleanOut*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TMat33(void *p = 0);
   static void *newArray_TMat33(Long_t size, void *p);
   static void delete_TMat33(void *p);
   static void deleteArray_TMat33(void *p);
   static void destruct_TMat33(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMat33*)
   {
      ::TMat33 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMat33 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TMat33", ::TMat33::Class_Version(), "TMat33.h", 18,
                  typeid(::TMat33), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMat33::Dictionary, isa_proxy, 4,
                  sizeof(::TMat33) );
      instance.SetNew(&new_TMat33);
      instance.SetNewArray(&newArray_TMat33);
      instance.SetDelete(&delete_TMat33);
      instance.SetDeleteArray(&deleteArray_TMat33);
      instance.SetDestructor(&destruct_TMat33);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMat33*)
   {
      return GenerateInitInstanceLocal((::TMat33*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TMat33*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TEuler(void *p = 0);
   static void *newArray_TEuler(Long_t size, void *p);
   static void delete_TEuler(void *p);
   static void deleteArray_TEuler(void *p);
   static void destruct_TEuler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TEuler*)
   {
      ::TEuler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TEuler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TEuler", ::TEuler::Class_Version(), "TEuler.h", 17,
                  typeid(::TEuler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TEuler::Dictionary, isa_proxy, 4,
                  sizeof(::TEuler) );
      instance.SetNew(&new_TEuler);
      instance.SetNewArray(&newArray_TEuler);
      instance.SetDelete(&delete_TEuler);
      instance.SetDeleteArray(&deleteArray_TEuler);
      instance.SetDestructor(&destruct_TEuler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TEuler*)
   {
      return GenerateInitInstanceLocal((::TEuler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TEuler*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TVec3C(void *p = 0);
   static void *newArray_TVec3C(Long_t size, void *p);
   static void delete_TVec3C(void *p);
   static void deleteArray_TVec3C(void *p);
   static void destruct_TVec3C(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TVec3C*)
   {
      ::TVec3C *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TVec3C >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TVec3C", ::TVec3C::Class_Version(), "TVec3C.h", 19,
                  typeid(::TVec3C), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TVec3C::Dictionary, isa_proxy, 4,
                  sizeof(::TVec3C) );
      instance.SetNew(&new_TVec3C);
      instance.SetNewArray(&newArray_TVec3C);
      instance.SetDelete(&delete_TVec3C);
      instance.SetDeleteArray(&deleteArray_TVec3C);
      instance.SetDestructor(&destruct_TVec3C);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TVec3C*)
   {
      return GenerateInitInstanceLocal((::TVec3C*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TVec3C*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TMat33C(void *p = 0);
   static void *newArray_TMat33C(Long_t size, void *p);
   static void delete_TMat33C(void *p);
   static void deleteArray_TMat33C(void *p);
   static void destruct_TMat33C(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMat33C*)
   {
      ::TMat33C *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMat33C >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TMat33C", ::TMat33C::Class_Version(), "TMat33C.h", 18,
                  typeid(::TMat33C), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMat33C::Dictionary, isa_proxy, 4,
                  sizeof(::TMat33C) );
      instance.SetNew(&new_TMat33C);
      instance.SetNewArray(&newArray_TMat33C);
      instance.SetDelete(&delete_TMat33C);
      instance.SetDeleteArray(&deleteArray_TMat33C);
      instance.SetDestructor(&destruct_TMat33C);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMat33C*)
   {
      return GenerateInitInstanceLocal((::TMat33C*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TMat33C*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TArrayZ::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TArrayZ::Class_Name()
{
   return "TArrayZ";
}

//______________________________________________________________________________
const char *TArrayZ::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TArrayZ*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TArrayZ::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TArrayZ*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TArrayZ::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TArrayZ*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TArrayZ::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TArrayZ*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<char>::Class_Name()
{
   return "TNArray<char>";
}

//______________________________________________________________________________
template <> const char *TNArray<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<short>::Class_Name()
{
   return "TNArray<short>";
}

//______________________________________________________________________________
template <> const char *TNArray<short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<int>::Class_Name()
{
   return "TNArray<int>";
}

//______________________________________________________________________________
template <> const char *TNArray<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<long>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<long>::Class_Name()
{
   return "TNArray<long>";
}

//______________________________________________________________________________
template <> const char *TNArray<long>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<long>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<long>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<long>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<long>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<long>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<long>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<long>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<Long64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<Long64_t>::Class_Name()
{
   return "TNArray<Long64_t>";
}

//______________________________________________________________________________
template <> const char *TNArray<Long64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<Long64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<Long64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<Long64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<Long64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<float>::Class_Name()
{
   return "TNArray<float>";
}

//______________________________________________________________________________
template <> const char *TNArray<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<double>::Class_Name()
{
   return "TNArray<double>";
}

//______________________________________________________________________________
template <> const char *TNArray<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TNArray<TComplex>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TNArray<TComplex>::Class_Name()
{
   return "TNArray<TComplex>";
}

//______________________________________________________________________________
template <> const char *TNArray<TComplex>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TNArray<TComplex>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TNArray<TComplex>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TNArray<TComplex>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TNArray<TComplex>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TCleanOut::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TCleanOut::Class_Name()
{
   return "TCleanOut";
}

//______________________________________________________________________________
const char *TCleanOut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCleanOut*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TCleanOut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCleanOut*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TCleanOut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCleanOut*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TCleanOut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCleanOut*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TMat33::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TMat33::Class_Name()
{
   return "TMat33";
}

//______________________________________________________________________________
const char *TMat33::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMat33*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TMat33::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMat33*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMat33::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMat33*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMat33::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMat33*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TEuler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TEuler::Class_Name()
{
   return "TEuler";
}

//______________________________________________________________________________
const char *TEuler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TEuler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TEuler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TEuler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TEuler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TEuler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TEuler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TEuler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TVec3C::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TVec3C::Class_Name()
{
   return "TVec3C";
}

//______________________________________________________________________________
const char *TVec3C::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TVec3C*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TVec3C::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TVec3C*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TVec3C::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TVec3C*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TVec3C::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TVec3C*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TMat33C::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TMat33C::Class_Name()
{
   return "TMat33C";
}

//______________________________________________________________________________
const char *TMat33C::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMat33C*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TMat33C::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMat33C*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMat33C::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMat33C*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMat33C::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMat33C*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TArrayZ::Streamer(TBuffer &R__b)
{
   // Stream an object of class TArrayZ.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TArrayZ::Class(),this);
   } else {
      R__b.WriteClassBuffer(TArrayZ::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TArrayZ(void *p) {
      return  p ? new(p) ::TArrayZ : new ::TArrayZ;
   }
   static void *newArray_TArrayZ(Long_t nElements, void *p) {
      return p ? new(p) ::TArrayZ[nElements] : new ::TArrayZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_TArrayZ(void *p) {
      delete ((::TArrayZ*)p);
   }
   static void deleteArray_TArrayZ(void *p) {
      delete [] ((::TArrayZ*)p);
   }
   static void destruct_TArrayZ(void *p) {
      typedef ::TArrayZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TArrayZ

//______________________________________________________________________________
template <> void TNArray<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylEchargR(void *p) {
      return  p ? new(p) ::TNArray<char> : new ::TNArray<char>;
   }
   static void *newArray_TNArraylEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<char>[nElements] : new ::TNArray<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylEchargR(void *p) {
      delete ((::TNArray<char>*)p);
   }
   static void deleteArray_TNArraylEchargR(void *p) {
      delete [] ((::TNArray<char>*)p);
   }
   static void destruct_TNArraylEchargR(void *p) {
      typedef ::TNArray<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<char>

//______________________________________________________________________________
template <> void TNArray<short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylEshortgR(void *p) {
      return  p ? new(p) ::TNArray<short> : new ::TNArray<short>;
   }
   static void *newArray_TNArraylEshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<short>[nElements] : new ::TNArray<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylEshortgR(void *p) {
      delete ((::TNArray<short>*)p);
   }
   static void deleteArray_TNArraylEshortgR(void *p) {
      delete [] ((::TNArray<short>*)p);
   }
   static void destruct_TNArraylEshortgR(void *p) {
      typedef ::TNArray<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<short>

//______________________________________________________________________________
template <> void TNArray<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylEintgR(void *p) {
      return  p ? new(p) ::TNArray<int> : new ::TNArray<int>;
   }
   static void *newArray_TNArraylEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<int>[nElements] : new ::TNArray<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylEintgR(void *p) {
      delete ((::TNArray<int>*)p);
   }
   static void deleteArray_TNArraylEintgR(void *p) {
      delete [] ((::TNArray<int>*)p);
   }
   static void destruct_TNArraylEintgR(void *p) {
      typedef ::TNArray<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<int>

//______________________________________________________________________________
template <> void TNArray<long>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<long>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<long>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<long>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylElonggR(void *p) {
      return  p ? new(p) ::TNArray<long> : new ::TNArray<long>;
   }
   static void *newArray_TNArraylElonggR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<long>[nElements] : new ::TNArray<long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylElonggR(void *p) {
      delete ((::TNArray<long>*)p);
   }
   static void deleteArray_TNArraylElonggR(void *p) {
      delete [] ((::TNArray<long>*)p);
   }
   static void destruct_TNArraylElonggR(void *p) {
      typedef ::TNArray<long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<long>

//______________________________________________________________________________
template <> void TNArray<Long64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<Long64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<Long64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<Long64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylELong64_tgR(void *p) {
      return  p ? new(p) ::TNArray<Long64_t> : new ::TNArray<Long64_t>;
   }
   static void *newArray_TNArraylELong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<Long64_t>[nElements] : new ::TNArray<Long64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylELong64_tgR(void *p) {
      delete ((::TNArray<Long64_t>*)p);
   }
   static void deleteArray_TNArraylELong64_tgR(void *p) {
      delete [] ((::TNArray<Long64_t>*)p);
   }
   static void destruct_TNArraylELong64_tgR(void *p) {
      typedef ::TNArray<Long64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<Long64_t>

//______________________________________________________________________________
template <> void TNArray<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylEfloatgR(void *p) {
      return  p ? new(p) ::TNArray<float> : new ::TNArray<float>;
   }
   static void *newArray_TNArraylEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<float>[nElements] : new ::TNArray<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylEfloatgR(void *p) {
      delete ((::TNArray<float>*)p);
   }
   static void deleteArray_TNArraylEfloatgR(void *p) {
      delete [] ((::TNArray<float>*)p);
   }
   static void destruct_TNArraylEfloatgR(void *p) {
      typedef ::TNArray<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<float>

//______________________________________________________________________________
template <> void TNArray<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylEdoublegR(void *p) {
      return  p ? new(p) ::TNArray<double> : new ::TNArray<double>;
   }
   static void *newArray_TNArraylEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<double>[nElements] : new ::TNArray<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylEdoublegR(void *p) {
      delete ((::TNArray<double>*)p);
   }
   static void deleteArray_TNArraylEdoublegR(void *p) {
      delete [] ((::TNArray<double>*)p);
   }
   static void destruct_TNArraylEdoublegR(void *p) {
      typedef ::TNArray<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<double>

//______________________________________________________________________________
template <> void TNArray<TComplex>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TNArray<TComplex>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TNArray<TComplex>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TNArray<TComplex>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TNArraylETComplexgR(void *p) {
      return  p ? new(p) ::TNArray<TComplex> : new ::TNArray<TComplex>;
   }
   static void *newArray_TNArraylETComplexgR(Long_t nElements, void *p) {
      return p ? new(p) ::TNArray<TComplex>[nElements] : new ::TNArray<TComplex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TNArraylETComplexgR(void *p) {
      delete ((::TNArray<TComplex>*)p);
   }
   static void deleteArray_TNArraylETComplexgR(void *p) {
      delete [] ((::TNArray<TComplex>*)p);
   }
   static void destruct_TNArraylETComplexgR(void *p) {
      typedef ::TNArray<TComplex> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TNArray<TComplex>

//______________________________________________________________________________
void TCleanOut::Streamer(TBuffer &R__b)
{
   // Stream an object of class TCleanOut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TCleanOut::Class(),this);
   } else {
      R__b.WriteClassBuffer(TCleanOut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TCleanOut(void *p) {
      return  p ? new(p) ::TCleanOut : new ::TCleanOut;
   }
   static void *newArray_TCleanOut(Long_t nElements, void *p) {
      return p ? new(p) ::TCleanOut[nElements] : new ::TCleanOut[nElements];
   }
   // Wrapper around operator delete
   static void delete_TCleanOut(void *p) {
      delete ((::TCleanOut*)p);
   }
   static void deleteArray_TCleanOut(void *p) {
      delete [] ((::TCleanOut*)p);
   }
   static void destruct_TCleanOut(void *p) {
      typedef ::TCleanOut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TCleanOut

//______________________________________________________________________________
void TMat33::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMat33.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMat33::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMat33::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TMat33(void *p) {
      return  p ? new(p) ::TMat33 : new ::TMat33;
   }
   static void *newArray_TMat33(Long_t nElements, void *p) {
      return p ? new(p) ::TMat33[nElements] : new ::TMat33[nElements];
   }
   // Wrapper around operator delete
   static void delete_TMat33(void *p) {
      delete ((::TMat33*)p);
   }
   static void deleteArray_TMat33(void *p) {
      delete [] ((::TMat33*)p);
   }
   static void destruct_TMat33(void *p) {
      typedef ::TMat33 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMat33

//______________________________________________________________________________
void TEuler::Streamer(TBuffer &R__b)
{
   // Stream an object of class TEuler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TEuler::Class(),this);
   } else {
      R__b.WriteClassBuffer(TEuler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TEuler(void *p) {
      return  p ? new(p) ::TEuler : new ::TEuler;
   }
   static void *newArray_TEuler(Long_t nElements, void *p) {
      return p ? new(p) ::TEuler[nElements] : new ::TEuler[nElements];
   }
   // Wrapper around operator delete
   static void delete_TEuler(void *p) {
      delete ((::TEuler*)p);
   }
   static void deleteArray_TEuler(void *p) {
      delete [] ((::TEuler*)p);
   }
   static void destruct_TEuler(void *p) {
      typedef ::TEuler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TEuler

//______________________________________________________________________________
void TVec3C::Streamer(TBuffer &R__b)
{
   // Stream an object of class TVec3C.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TVec3C::Class(),this);
   } else {
      R__b.WriteClassBuffer(TVec3C::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TVec3C(void *p) {
      return  p ? new(p) ::TVec3C : new ::TVec3C;
   }
   static void *newArray_TVec3C(Long_t nElements, void *p) {
      return p ? new(p) ::TVec3C[nElements] : new ::TVec3C[nElements];
   }
   // Wrapper around operator delete
   static void delete_TVec3C(void *p) {
      delete ((::TVec3C*)p);
   }
   static void deleteArray_TVec3C(void *p) {
      delete [] ((::TVec3C*)p);
   }
   static void destruct_TVec3C(void *p) {
      typedef ::TVec3C current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TVec3C

//______________________________________________________________________________
void TMat33C::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMat33C.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMat33C::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMat33C::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TMat33C(void *p) {
      return  p ? new(p) ::TMat33C : new ::TMat33C;
   }
   static void *newArray_TMat33C(Long_t nElements, void *p) {
      return p ? new(p) ::TMat33C[nElements] : new ::TMat33C[nElements];
   }
   // Wrapper around operator delete
   static void delete_TMat33C(void *p) {
      delete ((::TMat33C*)p);
   }
   static void deleteArray_TMat33C(void *p) {
      delete [] ((::TMat33C*)p);
   }
   static void destruct_TMat33C(void *p) {
      typedef ::TMat33C current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMat33C

namespace {
  void TriggerDictionaryInitialization_PhysMoreDict_Impl() {
    static const char* headers[] = {
"TArrayZ.h",
"TNArray.h",
"TCleanOut.h",
"TMat33.h",
"TEuler.h",
"TVec3C.h",
"TMat33C.h",
0
    };
    static const char* includePaths[] = {
"/home/serena/Software/root-install/include",
"/home/serena/Software/SLITRANI/slitrani-fixed/PhysMore/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PhysMoreDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(1dim array of complex)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TArrayZ.h")))  TArrayZ;
template <typename TT> class __attribute__((annotate("$clingAutoload$TNArray.h")))  TNArray;

class __attribute__((annotate("$clingAutoload$TArrayZ.h")))  TComplex;
class __attribute__((annotate(R"ATTRDUMP(Class emitting standardized messages onto output and listing)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TCleanOut.h")))  TCleanOut;
class __attribute__((annotate(R"ATTRDUMP(implements 3X3 matrices associated with TVector3 and TVec3C)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TMat33.h")))  TMat33;
class __attribute__((annotate(R"ATTRDUMP(Euler <==> GEANT3 rotation angles)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TEuler.h")))  TEuler;
class __attribute__((annotate(R"ATTRDUMP(Implement complex 3-vectors in the same way as real TVector3)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TMat33.h")))  TVec3C;
class __attribute__((annotate(R"ATTRDUMP(Implements complex 3X3 matrices associated with TVector3 and TVec3C)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TMat33C.h")))  TMat33C;
typedef char Char_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Char_t> TNArrayC __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef short Short_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Short_t> TNArrayS __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef int Int_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Int_t> TNArrayI __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef long Long_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Long_t> TNArrayL __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef long long Long64_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Long64_t> TNArrayL64 __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef float Float_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Float_t> TNArrayF __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef double Double_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef TNArray<Double_t> TNArrayD __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
typedef TNArray<TComplex> TNArrayZ __attribute__((annotate("$clingAutoload$TNArray.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PhysMoreDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TArrayZ.h"
#include "TNArray.h"
#include "TCleanOut.h"
#include "TMat33.h"
#include "TEuler.h"
#include "TVec3C.h"
#include "TMat33C.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"KindOfMessage", payloadCode, "@",
"TArrayZ", payloadCode, "@",
"TCleanOut", payloadCode, "@",
"TEuler", payloadCode, "@",
"TMat33", payloadCode, "@",
"TMat33C", payloadCode, "@",
"TNArray<Char_t>", payloadCode, "@",
"TNArray<Double_t>", payloadCode, "@",
"TNArray<Float_t>", payloadCode, "@",
"TNArray<Int_t>", payloadCode, "@",
"TNArray<Long64_t>", payloadCode, "@",
"TNArray<Long_t>", payloadCode, "@",
"TNArray<Short_t>", payloadCode, "@",
"TNArray<TComplex>", payloadCode, "@",
"TNArray<char>", payloadCode, "@",
"TNArray<double>", payloadCode, "@",
"TNArray<float>", payloadCode, "@",
"TNArray<int>", payloadCode, "@",
"TNArray<long>", payloadCode, "@",
"TNArray<short>", payloadCode, "@",
"TNArrayC", payloadCode, "@",
"TNArrayD", payloadCode, "@",
"TNArrayF", payloadCode, "@",
"TNArrayI", payloadCode, "@",
"TNArrayL", payloadCode, "@",
"TNArrayL64", payloadCode, "@",
"TNArrayS", payloadCode, "@",
"TNArrayZ", payloadCode, "@",
"TVec3C", payloadCode, "@",
"gCleanOut", payloadCode, "@",
"operator&", payloadCode, "@",
"operator*", payloadCode, "@",
"operator+", payloadCode, "@",
"operator-", payloadCode, "@",
"operator^", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PhysMoreDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PhysMoreDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PhysMoreDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PhysMoreDict() {
  TriggerDictionaryInitialization_PhysMoreDict_Impl();
}
