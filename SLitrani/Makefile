#
#  make clean and make install of this makefile imply that you define the system variable
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
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = g++
#CXXFLAGS      = -g -m64 -Wall -fPIC -I$(ROOTDEV)/include 
CXXFLAGS      = -O -m64 -Wall -fPIC -I$(ROOTDEV)/include 
LD            = g++
#LDFLAGS       = -g -m64
LDFLAGS       = -O -m64

LIBNAME       = libSLitrani
PROGRAMSO     = $(LIBNAME).$(DllSuf)
PROGRAM       = SLitrani$(ExeSuf)
SOFLAGS       = -shared 
CXXFLAGS     += $(ROOTCFLAGS)
ROOTLIBS     += -lGui
ROOTLIBS     += -lHtml
ROOTLIBS     += -lGuiHtml
ROOTLIBS     += -lEG
ROOTLIBS     += -lGeom
ROOTLIBS     += -lMathMore
ROOTLIBS     += -lSpectrum
LIBS          = $(SYSLIBS)
LIBS         += -L$(ROOTDEV)/lib -lTwoPad -lSplineFit -lPhysMore $(ROOTLIBS)

#------------------------------------------------------------------------------


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
                TLitCascade.$(ObjSuf) TLitGamma.$(ObjSuf) TLitAlbedo.$(ObjSuf) \
                SLitraniDict.$(ObjSuf)


all:        $(PROGRAMSO) $(PROGRAM)

$(PROGRAMSO): $(OBJS)
		@echo "Creating library $(PROGRAMSO) ..."
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@
		@echo "$(PROGRAMSO) done"

$(PROGRAM): main.$(ObjSuf) $(OBJS)
		@echo "Linking $(PROGRAM) ..."
		$(LD) $(LDFLAGS) main.$(ObjSuf) $(OBJS) $(LIBS) $(OutPutOpt)$(PROGRAM)
		@echo "$(PROGRAM) done"

clean:
		@rm -f $(OBJS) main.o *\~ SLitraniDict.h SLitraniDict.cpp core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f *.lis *.root
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TLitPol3.h
		@rm -f $(ROOTDEV)/include/TLitPol4.h
		@rm -f $(ROOTDEV)/include/TLitEqIndex.h
		@rm -f $(ROOTDEV)/include/TLitWave.h
		@rm -f $(ROOTDEV)/include/TLitHistParams.h
		@rm -f $(ROOTDEV)/include/TLitPhys.h
		@rm -f $(ROOTDEV)/include/TLitTrap.h
		@rm -f $(ROOTDEV)/include/TLitLitSpectrumCp.h
		@rm -f $(ROOTDEV)/include/TLitLitSpectrum.h
		@rm -f $(ROOTDEV)/include/TLitDetector.h
		@rm -f $(ROOTDEV)/include/TLitSellmeier.h
		@rm -f $(ROOTDEV)/include/TLitMedium.h
		@rm -f $(ROOTDEV)/include/TLitVolume.h
		@rm -f $(ROOTDEV)/include/TLit.h
		@rm -f $(ROOTDEV)/include/TLitPhoton.h
		@rm -f $(ROOTDEV)/include/TLitSpontan.h
		@rm -f $(ROOTDEV)/include/TLitranino.h
		@rm -f $(ROOTDEV)/include/TLitResults.h
		@rm -f $(ROOTDEV)/include/TLitMarrow.h
		@rm -f $(ROOTDEV)/include/TLitParticle.h
		@rm -f $(ROOTDEV)/include/TLitBeam.h
		@rm -f $(ROOTDEV)/include/TLitCascade.h
		@rm -f $(ROOTDEV)/include/TLitGamma.h
		@rm -f $(ROOTDEV)/include/TLitAlbedo.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TLitPol3.h
		@rm -f $(ROOTDEV)/include/TLitPol4.h
		@rm -f $(ROOTDEV)/include/TLitEqIndex.h
		@rm -f $(ROOTDEV)/include/TLitWave.h
		@rm -f $(ROOTDEV)/include/TLitHistParams.h
		@rm -f $(ROOTDEV)/include/TLitPhys.h
		@rm -f $(ROOTDEV)/include/TLitTrap.h
		@rm -f $(ROOTDEV)/include/TLitLitSpectrumCp.h
		@rm -f $(ROOTDEV)/include/TLitLitSpectrum.h
		@rm -f $(ROOTDEV)/include/TLitDetector.h
		@rm -f $(ROOTDEV)/include/TLitSellmeier.h
		@rm -f $(ROOTDEV)/include/TLitMedium.h
		@rm -f $(ROOTDEV)/include/TLitVolume.h
		@rm -f $(ROOTDEV)/include/TLitPhoton.h
		@rm -f $(ROOTDEV)/include/TLitSpontan.h
		@rm -f $(ROOTDEV)/include/TLit.h
		@rm -f $(ROOTDEV)/include/TLitranino.h
		@rm -f $(ROOTDEV)/include/TLitResults.h
		@rm -f $(ROOTDEV)/include/TLitMarrow.h
		@rm -f $(ROOTDEV)/include/TLitParticle.h
		@rm -f $(ROOTDEV)/include/TLitBeam.h
		@rm -f $(ROOTDEV)/include/TLitCascade.h
		@rm -f $(ROOTDEV)/include/TLitGamma.h
		@rm -f $(ROOTDEV)/include/TLitAlbedo.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(LIBNAME).$(DllSuf) $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@cp TLitPol3.h $(ROOTDEV)/include/TLitPol3.h
		@cp TLitPol4.h $(ROOTDEV)/include/TLitPol4.h
		@cp TLitEqIndex.h $(ROOTDEV)/include/TLitEqIndex.h
		@cp TLitWave.h $(ROOTDEV)/include/TLitWave.h
		@cp TLitHistParams.h $(ROOTDEV)/include/TLitHistParams.h
		@cp TLitPhys.h $(ROOTDEV)/include/TLitPhys.h
		@cp TLitTrap.h $(ROOTDEV)/include/TLitTrap.h
		@cp TLitSpectrumCp.h $(ROOTDEV)/include/TLitSpectrumCp.h
		@cp TLitSpectrum.h $(ROOTDEV)/include/TLitSpectrum.h
		@cp TLitDetector.h $(ROOTDEV)/include/TLitDetector.h
		@cp TLitSellmeier.h $(ROOTDEV)/include/TLitSellmeier.h
		@cp TLitMedium.h $(ROOTDEV)/include/TLitMedium.h
		@cp TLitVolume.h $(ROOTDEV)/include/TLitVolume.h
		@cp TLit.h $(ROOTDEV)/include/TLit.h
		@cp TLitPhoton.h $(ROOTDEV)/include/TLitPhoton.h
		@cp TLitSpontan.h $(ROOTDEV)/include/TLitSpontan.h
		@cp TLitranino.h $(ROOTDEV)/include/TLitranino.h
		@cp TLitResults.h $(ROOTDEV)/include/TLitResults.h
		@cp TLitMarrow.h $(ROOTDEV)/include/TLitMarrow.h
		@cp TLitParticle.h $(ROOTDEV)/include/TLitParticle.h
		@cp TLitBeam.h $(ROOTDEV)/include/TLitBeam.h
		@cp TLitCascade.h $(ROOTDEV)/include/TLitCascade.h
		@cp TLitGamma.h $(ROOTDEV)/include/TLitGamma.h
		@cp TLitAlbedo.h $(ROOTDEV)/include/TLitAlbedo.h
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


.SUFFIXES: .$(SrcSuf)

###
SLitraniDict.$(SrcSuf): $(HDRS)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f SLitraniDict.$(SrcSuf) -c -I$(ROOTDEV)/include $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

