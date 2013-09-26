#
#  make clean and make install of this makefile imply that you define the system variable
#ROOTDEV. Exactly as ROOTSYS is a pointer towards the directory containing the include
#files, the libraries and the shared libraries of ROOT, ROOTDEV points towards a directory intended
#to contain the include files, the libraries and the shared libraries of all developments made
#above ROOT, like PhysMore, or the programs you may have developed yourself.
#  $(ROOTDEV) must contain at least 3 subdirectories: bin, lib and include.
#  Only by this way will you be able to write modular code, allowing one of your module
#to call entries of an other of your modules or entries of PhysMore.
#  If you have write access to $(ROOTSYS), you can choose ROOTDEV=ROOTSYS, but this mixing
#of your code with the code of ROOT is to my mind inelegant and the choice of a separate
#ROOTDEV is surely better.
# $(ROOTDEV)/bin  has to be added to PATH
# $(ROOTDEV)/lib  has to be added to LD_LIBRARY_PATH
#
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -O -m64 -Wall -fPIC
LD            = g++
LDFLAGS       = -O -m64

LIBNAME       = libPhysMore
PROGRAMSO     = $(LIBNAME).$(DllSuf)
PROGRAM       = PhysMore$(ExeSuf)
SOFLAGS       = -shared 
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LIBS         += -lGui
LIBS         += -lHtml
LIBS         += -lGuiHtml

#------------------------------------------------------------------------------

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
		@rm -f $(OBJS) main.$(ObjSuf) *\~ PhysMoreDict.h PhysMoreDict.$(SrcSuf) core
		@rm -f $(PROGRAM) $(PROGRAMSO)
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(PROGRAMSO)
		@rm -f $(ROOTDEV)/include/TArrayZ.h
		@rm -f $(ROOTDEV)/include/TNArray.h
		@rm -f $(ROOTDEV)/include/TCleanOut.h
		@rm -f $(ROOTDEV)/include/TMat33.h
		@rm -f $(ROOTDEV)/include/TEuler.h
		@rm -f $(ROOTDEV)/include/TVec3C.h
		@rm -f $(ROOTDEV)/include/TMat33C.h

install:
		@rm -f $(ROOTDEV)/bin/$(PROGRAM)
		@rm -f $(ROOTDEV)/lib/$(LIBNAME).$(DllSuf)
		@rm -f $(ROOTDEV)/include/TArrayZ.h
		@rm -f $(ROOTDEV)/include/TNArray.h
		@rm -f $(ROOTDEV)/include/TCleanOut.h
		@rm -f $(ROOTDEV)/include/TMat33.h
		@rm -f $(ROOTDEV)/include/TEuler.h
		@rm -f $(ROOTDEV)/include/TVec3C.h
		@rm -f $(ROOTDEV)/include/TMat33C.h
		@cp $(PROGRAM) $(ROOTDEV)/bin/$(PROGRAM)
		@cp $(PROGRAMSO) $(ROOTDEV)/lib/$(PROGRAMSO)
		@cp TArrayZ.h $(ROOTDEV)/include/TArrayZ.h
		@cp TNArray.h $(ROOTDEV)/include/TNArray.h
		@cp TCleanOut.h $(ROOTDEV)/include/TCleanOut.h
		@cp TMat33.h $(ROOTDEV)/include/TMat33.h
		@cp TEuler.h $(ROOTDEV)/include/TEuler.h
		@cp TVec3C.h $(ROOTDEV)/include/TVec3C.h
		@cp TMat33C.h $(ROOTDEV)/include/TMat33C.h
		@echo "libraries, shared libraries and includes copied to $(ROOTDEV)"

###

TArrayZ.$(ObjSuf):     TArrayZ.h
TNArray.$(ObjSuf):     TNArray.h
TCleanOut.$(ObjSuf):   TCleanOut.h
TVec3C.$(ObjSuf):      TVec3C.h
TMat33.$(ObjSuf):      TVec3C.h TMat33.h
TEuler.$(ObjSuf):      TMat33.h TEuler.h
TMat33C.$(ObjSuf):     TVec3C.h TMat33.h TMat33C.h

.SUFFIXES: .$(SrcSuf)

###
PhysMoreDict.$(SrcSuf): $(HDRS)
	@echo "Generating Dictionary ..."
	@$(ROOTSYS)/bin/rootcint -f PhysMoreDict.$(SrcSuf) -c $(HDRS) LinkDef.h


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

