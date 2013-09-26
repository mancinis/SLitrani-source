#
#  nmake clean and nmake install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like litrani, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of litrani.
#  If you have write access to $(ROOTSYS), you can choose ROOTDEV=ROOTSYS, but this mixing
#of your code with the code of ROOT is to my mind inelegant and the choice of a separate
#ROOTDEV is surely better.
# $(ROOTDEV)/bin  has to be added to PATH
# $(ROOTDEV)/lib  has to be added to LD_LIBRARY_PATH
#
ObjSuf        = obj
SrcSuf        = cpp
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:

# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
#CXXFLAGS      = $(cvarsdll) -EHsc  -nologo -wd4273 -wd4786 -GR -DWIN32 \
CXXFLAGS      = $(cvarsdll) -EHsc  -nologo -GR -DWIN32 \
                -D_WINDOWS -I$(ROOTDEV)/include -I$(ROOTSYS)/include -FIw32pragma.h 
LD            = $(link)

#CXXOPT       = -O2 -MD
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
                $(ROOTSYS)/lib/libNet.lib $(ROOTSYS)/lib/libRIO.lib \
                $(ROOTSYS)/lib/libGui.lib $(ROOTSYS)/lib/libEG.lib \
                $(ROOTSYS)/lib/libSpectrum.lib $(ROOTSYS)/lib/libGeom.lib \
                $(ROOTSYS)/lib/libMathMore.lib \
                $(ROOTDEV)/lib/libTwoPad.lib $(ROOTDEV)/lib/libSplineFit.lib \
                $(ROOTDEV)/lib/libPhysMore.lib 
#LIBS          = $(ROOTLIBS)
LIBS          = $(ROOTLIBS) $(guilibsdll)
LIBSALL       = $(ROOTLIBS)
LIBNAME       = libSLitrani
PROGRAMLIB    = $(LIBNAME).lib

HDRS          = TLitPol3.h TLitPol4.h TLitEqIndex.h TLitWave.h TLitHistParams.h TLitPhys.h TLitTrap.h \
                TLitSpectrumCp.h TLitSpectrum.h TLitDetector.h TLitSellmeier.h \
                TLitMedium.h TLitVolume.h TLit.h TLitPhoton.h TLitSpontan.h \
                TLitranino.h TLitResults.h TLitMarrow.h \
                TLitParticle.h TLitBeam.h TLitCascade.h TLitGamma.h TLitAlbedo.h
SRCS          = main.$(SrcSuf) TLitPol3.$(SrcSuf) TLitPol4.$(SrcSuf) \
                TLitEqIndex.$(SrsSuf) TLitWave.$(SrcSuf) \
                TLitHistParams.$(SrcSuf) TLitPhys.$(SrcSuf) TLitTrap.$(SrcSuf) \
                TLitSpectrumCp.$(SrcSuf) TLitSpectrum.$(SrcSuf) TLitDetector.$(SrcSuf) \
                TLitSellmeier.$(SrcSuf) TLitMedium.$(SrcSuf) TLitVolume.$(SrcSuf) \
                TLit.$(SrcSuf) TLitPhoton.$(SrcSuf) TLitSpontan.$(SrcSuf) \
                TLitranino.$(SrcSuf) TLitResults.$(SrcSuf) TLitMarrow.$(SrcSuf) \
                TLitParticle.$(SrcSuf) TLitBeam.$(SrcSuf) \
                TLitCascade.$(SrcSuf) TLitGamma.$(SrcSuf) TLitAlbedo.$(SrcSuf) \
                SLitraniDict.$(SrcSuf)
OBJS          = TLitPol3.$(ObjSuf) TLitPol4.$(ObjSuf) TLitEqIndex.$(ObjSuf) \
                TLitWave.$(ObjSuf) TLitHistParams.$(ObjSuf) TLitPhys.$(ObjSuf) TLitTrap.$(ObjSuf) \
                TLitSpectrumCp.$(ObjSuf) TLitSpectrum.$(ObjSuf) TLitDetector.$(ObjSuf) \
                TLitSellmeier.$(ObjSuf) TLitMedium.$(ObjSuf) TLitVolume.$(ObjSuf) \
                TLit.$(ObjSuf) TLitPhoton.$(ObjSuf) TLitSpontan.$(ObjSuf) \
                TLitranino.$(ObjSuf) TLitResults.$(ObjSuf) TLitMarrow.$(ObjSuf) \
                TLitParticle.$(ObjSuf) TLitBeam.$(ObjSuf) \
                TLitCascade.$(ObjSuf) TLitGamma.$(ObjSuf)  TLitAlbedo.$(ObjSuf) \
                SLitraniDict.$(ObjSuf)

PROGRAM    = SLitrani$(ExeSuf)
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
	@del /q $(OBJS) main.$(ObjSuf) SLitraniDict.h SLitraniDict.$(SrcSuf) *~
        @del /q $(PROGRAM) $(PROGRAMSO) $(PROGRAMLIB) $(LIBNAME).def $(LIBNAME).exp
        @del /q *.lis *.root
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TLitPol3.h
	@del /q $(ROOTDEV)\include\TLitPol4.h
	@del /q $(ROOTDEV)\include\TLitEqIndex.h
	@del /q $(ROOTDEV)\include\TLitWave.h
	@del /q $(ROOTDEV)\include\TLitHistParams.h
	@del /q $(ROOTDEV)\include\TLitPhys.h
	@del /q $(ROOTDEV)\include\TLitTrap.h
	@del /q $(ROOTDEV)\include\TLitSpectrumCp.h
	@del /q $(ROOTDEV)\include\TLitSpectrum.h
	@del /q $(ROOTDEV)\include\TLitDetector.h
	@del /q $(ROOTDEV)\include\TLitSellmeier.h
	@del /q $(ROOTDEV)\include\TLitMedium.h
	@del /q $(ROOTDEV)\include\TLitVolume.h
	@del /q $(ROOTDEV)\include\TLit.h
	@del /q $(ROOTDEV)\include\TLitPhoton.h
	@del /q $(ROOTDEV)\include\TLitSpontan.h
	@del /q $(ROOTDEV)\include\TLitranino.h
	@del /q $(ROOTDEV)\include\TLitResults.h
	@del /q $(ROOTDEV)\include\TLitMarrow.h
	@del /q $(ROOTDEV)\include\TLitParticle.h
	@del /q $(ROOTDEV)\include\TLitBeam.h
	@del /q $(ROOTDEV)\include\TLitCascade.h
	@del /q $(ROOTDEV)\include\TLitGamma.h
	@del /q $(ROOTDEV)\include\TLitAlbedo.h

install:
	@del /q $(ROOTDEV)\bin\$(PROGRAM)
	@del /q $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@del /q $(ROOTDEV)\lib\$(LIBNAME).lib
	@del /q $(ROOTDEV)\lib\$(LIBNAME).def
	@del /q $(ROOTDEV)\lib\$(LIBNAME).exp
	@del /q $(ROOTDEV)\include\TLitPol3.h
	@del /q $(ROOTDEV)\include\TLitPol4.h
	@del /q $(ROOTDEV)\include\TLitEqIndex.h
	@del /q $(ROOTDEV)\include\TLitWave.h
	@del /q $(ROOTDEV)\include\TLitHistParams.h
	@del /q $(ROOTDEV)\include\TLitPhys.h
	@del /q $(ROOTDEV)\include\TLitTrap.h
	@del /q $(ROOTDEV)\include\TLitSpectrumCp.h
	@del /q $(ROOTDEV)\include\TLitSpectrum.h
	@del /q $(ROOTDEV)\include\TLitDetector.h
	@del /q $(ROOTDEV)\include\TLitSellmeier.h
	@del /q $(ROOTDEV)\include\TLitMedium.h
	@del /q $(ROOTDEV)\include\TLitVolume.h
	@del /q $(ROOTDEV)\include\TLit.h
	@del /q $(ROOTDEV)\include\TLitPhoton.h
	@del /q $(ROOTDEV)\include\TLitSpontan.h
	@del /q $(ROOTDEV)\include\TLitranino.h
	@del /q $(ROOTDEV)\include\TLitResults.h
	@del /q $(ROOTDEV)\include\TLitMarrow.h
	@del /q $(ROOTDEV)\include\TLitParticle.h
	@del /q $(ROOTDEV)\include\TLitBeam.h
	@del /q $(ROOTDEV)\include\TLitCascade.h
	@del /q $(ROOTDEV)\include\TLitGamma.h
	@del /q $(ROOTDEV)\include\TLitAlbedo.h
	@copy $(PROGRAM) $(ROOTDEV)\bin\$(PROGRAM)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\bin\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).$(DllSuf) $(ROOTDEV)\lib\$(LIBNAME).$(DllSuf)
	@copy $(LIBNAME).lib $(ROOTDEV)\lib\$(LIBNAME).lib
	@copy $(LIBNAME).def $(ROOTDEV)\lib\$(LIBNAME).def
	@copy $(LIBNAME).exp $(ROOTDEV)\lib\$(LIBNAME).exp
	@copy TLitPol3.h  $(ROOTDEV)\include\TLitPol3.h 
	@copy TLitPol4.h  $(ROOTDEV)\include\TLitPol4.h 
	@copy TLitEqIndex.h  $(ROOTDEV)\include\TLitEqIndex.h 
	@copy TLitWave.h  $(ROOTDEV)\include\TLitWave.h 
	@copy TLitHistParams.h  $(ROOTDEV)\include\TLitHistParams.h 
	@copy TLitPhys.h  $(ROOTDEV)\include\TLitPhys.h 
	@copy TLitTrap.h  $(ROOTDEV)\include\TLitTrap.h 
	@copy TLitSpectrumCp.h  $(ROOTDEV)\include\TLitSpectrumCp.h 
	@copy TLitSpectrum.h  $(ROOTDEV)\include\TLitSpectrum.h 
	@copy TLitDetector.h  $(ROOTDEV)\include\TLitDetector.h 
	@copy TLitSellmeier.h  $(ROOTDEV)\include\TLitSellmeier.h 
	@copy TLitMedium.h $(ROOTDEV)\include\TLitMedium.h 
	@copy TLitVolume.h $(ROOTDEV)\include\TLitVolume.h 
	@copy TLit.h $(ROOTDEV)\include\TLit.h 
	@copy TLitPhoton.h $(ROOTDEV)\include\TLitPhoton.h 
	@copy TLitSpontan.h $(ROOTDEV)\include\TLitSpontan.h 
	@copy TLitranino.h $(ROOTDEV)\include\TLitranino.h 
	@copy TLitResults.h $(ROOTDEV)\include\TLitResults.h 
	@copy TLitMarrow.h $(ROOTDEV)\include\TLitMarrow.h 
	@copy TLitParticle.h $(ROOTDEV)\include\TLitParticle.h 
	@copy TLitBeam.h $(ROOTDEV)\include\TLitBeam.h 
	@copy TLitCascade.h $(ROOTDEV)\include\TLitCascade.h 
	@copy TLitGamma.h $(ROOTDEV)\include\TLitGamma.h 
	@copy TLitAlbedo.h $(ROOTDEV)\include\TLitAlbedo.h 
	@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###
TLitPhys.$(ObjSuf):       TLitPhys.h
TLitPol3.$(ObjSuf):       TLitPol3.h
TLitPol4.$(ObjSuf):       TLitPol3.h TLitPol4.h
TLitEqIndex.$(ObjSuf):    TLit.h TLitPol4.h TLitEqIndex.h
TLitParticle.$(ObjSuf):   TLitPhys.h TLitParticle.h
TLitWave.$(ObjSuf):       TLitWave.h
TLitHistParams.$(ObjSuf): TLitPhys.h TLitHistParams.h
TLitTrap.$(ObjSuf):       TLitTrap.h
TLitSpectrumCp.$(ObjSuf): TLitPhys.h TLitSpectrumCp.h
TLitSpectrum.$(ObjSuf):   TLit.h TLitPhys.h TLitSpectrumCp.h TLitSpectrum.h
TLitDetector.$(ObjSuf):   TLitPhys.h TLitHistParams.h TLitDetector.h
TLitSellmeier.$(ObjSuf):  TLitSellmeier.h
TLitMedium.$(ObjSuf):     TLitPhys.h TLit.h TLitSellmeier.h TLitSpectrum.h TLitVolume.h TLitMedium.h
TLitResults.$(ObjSuf):    TLit.h TLitPhys.h TLitHistParams.h TLitMedium.h TLitDetector.h TLitResults.h
TLitMarrow.$(ObjSuf):     TLitResults.h TLitDetector.h TLitMarrow.h
TLitVolume.$(ObjSuf):     TLitPhys.h TLit.h TLitDetector.h TLitMedium.h TLitVolume.h
TLit.$(ObjSuf):           TLitHistParams.h TLitEqIndex.h TLitPhys.h TLitSpectrum.h \
                          TLitResults.h TLitMarrow.h TLit.h
TLitPhoton.$(ObjSuf):     TLitPhys.h TLit.h TLitDetector.h TLitResults.h TLitEqIndex.h TLitMedium.h \
                          TLitVolume.h TLitWave.h TLitPhoton.h
TLitranino.$(ObjSuf):     TLitPhys.h TLit.h TLitMedium.h TLitVolume.h TLitDetector.h TLitResults.h \
                          TLitMarrow.h TLitranino.h
TLitSpontan.$(ObjSuf):    TLit.h TLitPhys.h TLitMedium.h TLitVolume.h TLitDetector.h TLitResults.h \
                          TLitMarrow.h TLitPhoton.h TLitSpontan.h
TLitBeam.$(ObjSuf):       TLitPhys.h TLitMedium.h TLitVolume.h TLitPhoton.h TLitParticle.h \
                          TLitDetector.h TLitResults.h TLitMarrow.h TLit.h TLitBeam.h 
TLitCascade.$(ObjSuf):    TLit.h TLitPhys.h TLitMedium.h TLitVolume.h TLitDetector.h TLitResults.h \
                          TLitMarrow.h TLitPhoton.h TLitCascade.h
TLitGamma.$(ObjSuf):      TLitParticle.h TLit.h TLitPhys.h TLitMedium.h TLitVolume.h TLitDetector.h \
                          TLitResults.h TLitMarrow.h TLitPhoton.h TLitGamma.h
TLitAlbedo.$(ObjSuf):     TLit.h TLitMedium.h TLitVolume.h TLitSpontan.h TLitAlbedo.h

SLitraniDict.$(SrcSuf): $(HDRS)
	@echo "Deleting dictionary..."
	@del /q SLitraniDict.h SLitraniDict.$(SrcSuf) SLitraniDict.$(ObjSuf)
	@echo "Generating dictionary ..."
	@$(ROOTSYS)\bin\rootcint -f SLitraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include  $(HDRS) LinkDef.h

.$(SrcSuf).$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -c $<
