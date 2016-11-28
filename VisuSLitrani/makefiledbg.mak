ObjSuf        = obj
SrcSuf        = cpp
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:

# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
CXXFLAGS      = $(cvarsdll) -EHsc  -nologo -GR -DWIN32 \
                -D_WINDOWS -I$(ROOTDEV)/include -I$(ROOTSYS)/include -FIw32pragma.h 
#CXXOPT        = -O2 -MD
CXXOPT        = -Z7 -MDd
LD            = $(link)
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
                $(ROOTSYS)/lib/libNet.lib  $(ROOTSYS)/lib/libRIO.lib \
                $(ROOTSYS)/lib/libSpectrum.lib $(ROOTSYS)/lib/libGui.lib \
                $(ROOTDEV)/lib/libTwoPad.lib $(ROOTDEV)/lib/libSplineFit.lib \
                $(ROOTDEV)/lib/libPhysMore.lib $(ROOTDEV)/lib/libSLitrani.lib
LIBS          = $(ROOTLIBS) $(guilibsdll)
LIBSALL       = $(ROOTLIBS)
LIBNAME       = libVisuSLitrani
PROGRAMLIB    = $(LIBNAME).lib

HDRS          = TVisuSLit.h
SRCS          = main.$(SrcSuf) TVisuSLit.$(SrcSuf) VisuSLitraniDict.$(SrcSuf)
OBJS          = TVisuSLit.$(ObjSuf) VisuSLitraniDict.$(ObjSuf)

PROGRAM    = VisuSLitrani$(ExeSuf)
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
	@del /q $(OBJS) main.$(ObjSuf) VisuSLitraniDict.h VisuSLitraniDict.$(SrcSuf) *~
        @del /q $(PROGRAM) $(PROGRAMSO) $(PROGRAMLIB) $(LIBNAME).def $(LIBNAME).exp
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TVisuSLit.h

install:
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TVisuSLit.h
        @copy $(PROGRAM) $(ROOTDEV)\bin\$(PROGRAM)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).lib $(ROOTDEV)\lib\$(LIBNAME).lib
	@copy $(LIBNAME).def $(ROOTDEV)\lib\$(LIBNAME).def
	@copy $(LIBNAME).exp $(ROOTDEV)\lib\$(LIBNAME).exp
	@copy TVisuSLit.h $(ROOTDEV)\include\TVisuSLit.h
	@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"


###

TVisuSLit.$(ObjSuf):   TVisuSLit.h

VisuSLitraniDict.$(SrcSuf): $(HDRS)
	@echo "Deleting dictionary..."
	@del /q VisuSLitraniDict.h VisuSLitraniDict.$(SrcSuf) VisuSLitraniDict.obj
	@echo "Generating dictionary ..."
	@$(ROOTSYS)\bin\rootcint -f VisuSLitraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h

.$(SrcSuf).$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -c $<
