include rules.mk

# INCs are all the folders to be searched for header files to include
INCS := $(foreach d, $(INCDIR), -I$d)

# OBJs are all the sources, with .cpp replaced by .o in the proper folder
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))

# TSTOBJs are all the cppunit test OBJs
TSTOBJS := $(addprefix $(OBJDIR)/,$(notdir $(TSTSRCS:.cpp=.o)))


CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-unknown-pragmas -Wno-format -O3 $(INCS) -DLINUX $(JENKINS)

all : $(APP) $(TSTAPP)

$(APP): $(OUTDIR)/$(APP)

$(TSTAPP): $(OUTDIR)/$(TSTAPP)

$(OUTDIR)/$(APP): $(OBJDIR)/.depend $(OBJS)
	@ [ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(OUTDIR)/$(TSTAPP): $(OBJDIR)/.depend $(TSTOBJS)
	@ [ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)
	$(CXX) -static $(CXXFLAGS) $(TSTLDFLAGS) -o $@ $(TSTOBJS) $(TSTLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo Compiling \=\> $< && \
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(TSTDIR)/%.cpp
	@echo Compiling \=\> $< && \
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@



################################################


#
# Phony rules
#   Because these targets are not files and there are no prerequisites, it can
#   only be invoked by typing 'make <<target>>'. It is called a phony rule.
#
.PHONY: $(APP) $(TSTAPP) all clean clobber distclean run runtest depend


# build target and execute it
run: $(OUTDIR)/$(APP)
	@echo
	$<

# build target and execute it
runtest: $(OUTDIR)/$(TSTAPP)
	@echo
	$<
	@echo Igniting Firefox
	@cp $(TSTDIR)/testreport.xsl .
	@firefox Test.xml 2> /dev/null &


# remove all generated binary output
clean:
	-rm -f $(OBJS) $(TSTOBJS)


# remove all generated binary output, including dependency file
clobber:
	-rm -rf $(OBJDIR)


# remove all generated binary output, including dependency file and folders
distclean:
	-rm -rf $(OBJDIR) $(OUTDIR) ./testreport.xsl ./Test.xml

# generate dependency
depend: $(SRCS) $(TSTSRCS)
	@ [ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@ > $(OBJDIR)/.depend
	@ echo \=\=\> dependencies for `pwd`&& \
	$(CXX) -MM -I$(OBJDIR) $(CFLAGS) $(CXXFLAGS) $(SRCS) $(TSTSRCS) | \
	sed -e 's/^\(.*\.o:\)/$(OBJDIR)\/\1/' >> $(OBJDIR)/.depend


# include dependencies, if they exist
ifeq ($(OBJDIR)/.depend,$(wildcard $(OBJDIR)/.depend))
include $(OBJDIR)/.depend
else
$(OBJDIR)/.depend:
	@ [ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@ touch $(OBJDIR)/.depend
	@ $(MAKE) depend
endif

