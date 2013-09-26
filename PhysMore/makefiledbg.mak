ObjSuf        = obj
SrcSuf        = cxx
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:

# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
CXXFLAGS      = $(cvarsdll) -EHsc  -nologo -GR -DWIN32 \
                -D_WINDOWS -I$(ROOTSYS)/include -FIw32pragma.h
LD            = $(link)
 
#CXXOPT        = -O2 -MD
CXXOPT        = -Z7 -MDd
#LDOPT         = -opt:ref
LDOPT         = -debug

EXTRAFLAGS    = -D_CRT_SECURE_NO_DEPRECATE
LDFLAGS       = $(LDOPT) $(conlflags) -nologo -include:_G__cpp_setupG__Hist \
                -include:_G__cpp_setupG__Graf -include:_G__cpp_setupG__G3D \
                -include:_G__cpp_setupG__GPad -include:_G__cpp_setupG__Tree \
                -include:_G__cpp_setupG__Rint -include:_G__cpp_setupG__PostScript \
                -include:_G__cpp_setupG__Matrix -include:_G__cpp_setupG__Physics
SOFLAGS       = $(dlllflags:-pdb:none=)
ROOTLIBS      = $(ROOTSYS)/lib/libCore.lib \
                $(ROOTSYS)/lib/libCint.lib $(ROOTSYS)/lib/libHist.lib \
                $(ROOTSYS)/lib/libGraf.lib $(ROOTSYS)/lib/libGraf3d.lib \
                $(ROOTSYS)/lib/libGpad.lib $(ROOTSYS)/lib/libTree.lib \
                $(ROOTSYS)/lib/libRint.lib $(ROOTSYS)/lib/libPostscript.lib \
                $(ROOTSYS)/lib/libMatrix.lib $(ROOTSYS)/lib/libPhysics.lib \
                $(ROOTSYS)/lib/libMathCore.lib \
                $(ROOTSYS)/lib/libNet.lib $(ROOTSYS)/lib/libRIO.lib
#LIBS          = $(ROOTLIBS) $(guilibsdll)
LIBS          = $(ROOTLIBS)
LIBSALL       = $(ROOTLIBS)
LIBNAME       = libPhysMore
PROGRAMLIB    = $(LIBNAME).lib

HDRS          = TArrayZ.h TNArray.h TCleanOut.h \
                TMat33.h TEuler.h TVec3C.h TMat33C.h
SRCS          = main.$(SrcSuf) TArrayZ.$(SrcSuf) TNArray.$(SrcSuf) TCleanOut.$(SrcSuf) \
                TMat33.$(SrcSuf) TEuler.$(SrcSuf) \
                TVec3C.$(SrcSuf) TMat33C.$(SrcSuf) \
                PhysMoreDict.$(SrcSuf)
OBJS          = TArrayZ.$(ObjSuf) TNArray.$(ObjSuf) TCleanOut.$(ObjSuf) \
                TMat33.$(ObjSuf) TEuler.$(ObjSuf) \
                TVec3C.$(ObjSuf) TMat33C.$(ObjSuf) \
                PhysMoreDict.$(ObjSuf)

PROGRAM    = PhysMore$(ExeSuf)
PROGRAMSO  = $(LIBNAME).$(DllSuf)
all:        $(PROGRAMSO) $(PROGRAM)

$(PROGRAMSO): $(OBJS)
	@echo "Linking $(PROGRAMSO) ..."
        BINDEXPLIB  $* $(OBJS) > $*.def
        lib -nologo -MACHINE:IX86 $(OBJS) -def:$*.def $(OutPutOpt)$(PROGRAMLIB)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $*.exp $(LIBS) $(OutPutOpt)$(PROGRAMSO)
	@mt -nologo -manifest $(PROGRAMSO).manifest -outputresource:$(PROGRAMSO);2
        @del /q $(PROGRAMSO).manifest
	@echo "$(PROGRAMSO).manifest included into $(PROGRAMSO)"
	@echo "$(PROGRAMSO) done"

$(PROGRAM):  main.$(ObjSuf) $(OBJS)
	@echo "Linking $(PROGRAM) ..."
        $(LD) $(LDFLAGS) main.$(ObjSuf) $(OBJS) $(LIBS) $(OutPutOpt)$(PROGRAM)
	@mt -nologo -manifest $(PROGRAM).manifest -outputresource:$(PROGRAM);1
        @del /q $(PROGRAM).manifest
	@echo "$(PROGRAM).manifest included into $(PROGRAM)"
        @echo "$(PROGRAM) done"

clean:
	@del /q $(OBJS) main.$(ObjSuf) TNArrayaDict.h PhysMoreDict.$(SrcSuf) *~
        @del /q $(PROGRAM) $(PROGRAMSO) $(PROGRAMLIB) $(LIBNAME).def $(LIBNAME).exp
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).dll
	@del /q $(ROOTDEV)\lib\$(LIBNAME).dll
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TArrayZ.h
	@del /q $(ROOTDEV)\include\TNArray.h
	@del /q $(ROOTDEV)\include\TCleanOut.h
	@del /q $(ROOTDEV)\include\TMat33.h
	@del /q $(ROOTDEV)\include\TEuler.h
	@del /q $(ROOTDEV)\include\TVec3C.h
	@del /q $(ROOTDEV)\include\TMat33C.h

install:
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TArrayZ.h
	@del /q $(ROOTDEV)\include\TNArray.h
	@del /q $(ROOTDEV)\include\TCleanOut.h
	@del /q $(ROOTDEV)\include\TMat33.h
	@del /q $(ROOTDEV)\include\TEuler.h
	@del /q $(ROOTDEV)\include\TVec3C.h
	@del /q $(ROOTDEV)\include\TMat33C.h
	@copy $(PROGRAM) $(ROOTDEV)\bin\$(PROGRAM)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).lib $(ROOTDEV)\lib\$(LIBNAME).lib
	@copy $(LIBNAME).def $(ROOTDEV)\lib\$(LIBNAME).def
	@copy $(LIBNAME).exp $(ROOTDEV)\lib\$(LIBNAME).exp
	@copy TArrayZ.h $(ROOTDEV)\include\TArrayZ.h
	@copy TNArray.h $(ROOTDEV)\include\TNArray.h
	@copy TCleanOut.h $(ROOTDEV)\include\TCleanOut.h
	@copy TMat33.h $(ROOTDEV)\include\TMat33.h
	@copy TEuler.h $(ROOTDEV)\include\TEuler.h
	@copy TVec3C.h $(ROOTDEV)\include\TVec3C.h
	@copy TMat33C.h $(ROOTDEV)\include\TMat33C.h
	@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"
###

TArrayZ.$(ObjSuf):     TArrayZ.h
TNArray.$(ObjSuf):     TNArray.h
TCleanOut.$(ObjSuf):   TCleanOut.h
TVec3C.$(ObjSuf):      TVec3C.h
TMat33.$(ObjSuf):      TVec3C.h TMat33.h
TEuler.$(ObjSuf):      TMat33.h TEuler.h
TMat33C.$(ObjSuf):     TVec3C.h TMat33.h TMat33C.h

PhysMoreDict.$(SrcSuf): $(HDRS)
	@echo "Deleting Dictionary..."
	@del /q PhysMoreDict.h PhysMoreDict.$(SrcSuf) PhysMoreDict.obj
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)\bin\rootcint -f PhysMoreDict.$(SrcSuf) -c $(HDRS) LinkDef.h

.$(SrcSuf).$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -c $<
