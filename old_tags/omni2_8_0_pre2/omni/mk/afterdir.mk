#
# afterdir.mk - standard make stuff to go after dir.mk
#


#############################################################################
#
# Rules for cleaning
#

#
# "omake clean" should remove all objects, executables and libraries from the
# current directory.  "omake veryclean" additionally should remove .d files
# and any CORBA stubs that this directory has generated.
#

clean::
	$(CleanRule)

ifneq ($(OMAKE_TARGET),veryclean)
clean::
	@$(MakeSubdirs)
endif

veryclean:: clean
	$(VeryCleanRule)

veryclean::
	@$(MakeSubdirs)


#############################################################################
#
# CORBA stuff
#

#
# Tell make not to delete stub headers and source files (it usually removes
# intermediate files like these).
#

.PRECIOUS: $(CORBA_STUB_HDR_PATTERN) $(CORBA_STUB_SRC_PATTERN) $(CORBA_DYN_STUB_SRC_PATTERN)


#
# Rule to invoke the IDL compiler.
#

$(CORBA_STUB_HDR_PATTERN) $(CORBA_STUB_SRC_PATTERN) $(CORBA_DYN_STUB_SRC_PATTERN): %.idl
	$(MKDIRHIER) $(CORBA_STUB_DIR)
	$(RM) $(CORBA_STUB_DIR)/$(notdir $^)
	$(CP) $^ $(CORBA_STUB_DIR)
	chmod +w $(CORBA_STUB_DIR)/$(notdir $^)
	( cd $(CORBA_STUB_DIR); \
	  $(CORBA_STUB_DIR_TO_TOP)/$(BINDIR)/$(CORBA_IDL) $(notdir $^); \
	)

$(CORBA_STUB_OBJ_PATTERN): $(CORBA_STUB_SRC_PATTERN) $(CORBA_STUB_HDR_PATTERN)
	$(CXX) $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(CORBA_CPPFLAGS) \
		$(IMPORT_CPPFLAGS) -c $< -o $@

$(CORBA_DYN_STUB_OBJ_PATTERN): $(CORBA_DYN_STUB_SRC_PATTERN) $(CORBA_STUB_HDR_PATTERN)
	$(CXX) $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(CORBA_CPPFLAGS) \
		$(IMPORT_CPPFLAGS) -c $< -o $@


#
# This rule prints an error message if an IDL file couldn't be found (the
# error message printed without this rule is a rather cryptic
# "*** No rule to make target `xxx.d'".  However when an IDL file is to be
# generated by some other rule, we don't want this rule to get in the way.
# In this case, the dir.mk defines 'CorbaIDLFilesGenerated' to stop this
# rule being included.
#

ifndef CorbaIDLFilesGenerated
$(CORBA_IDL_FILES):
	@echo "ERROR: couldn't find idl file $@"
	@exit 1
endif


#############################################################################
#
# C / C++ header dependency stuff
#

#
# The recommended method of doing dependency analysis in the GNU make manual
# turns out to be painfully slow.  This method is similar but it's
# substantially faster and retains the desirable property that the user doesn't
# need to manually invoke a "make depend" step.
#
# As with the method described in the manual, we generate a separate dependency
# (.d) file for each source file.  The .d file records the header files that
# each C or C++ source file includes.  Any source file recorded in SRCS or
# CXXSRCS will cause us to try and include the corresponding .d file and GNU
# make then treats each .d file as a target to be remade.
#
# Unlike the manual's method, the rule we provide for making the .d file is
# actually a fake.  All it does is record in a temporary file that the .d file
# needs to be remade.  But as well as all the .d files, we also try to include
# a file called "depend.phony".  This file never exists, but it causes GNU make
# to try and make the target "depend.phony".  The rule for depend.phony then
# looks at the temporary files generated by the .d rules and then invokes the
# "omkdepend" program on all of the source files in one go.
#

#
# The "redepend" target is also a bit tricky.  We force the removal of .d files
# at the time make _parses_ this bit by assignment to a simple variable.  Then
# when it gets to parsing "include $(SRCS:.c=.d)" it realises it needs to
# regenerate the .d files.  All this happens without even needing a rule for
# "redepend"!  To get it to work over subdirectories too, we just have a simple
# MakeSubdirs rule for "redepend".
#

ifeq ($(OMAKE_TARGET),redepend)
dummyvariable := $(shell $(RM) *.d)
endif

redepend::
	@$(MakeSubdirs)

#
# Again we use simple assignment here to remove any of the depend.tmp files
# at the time make parses this bit.
#

dummyvariable := $(shell $(RM) cdepend.tmp cxxdepend.tmp asdepend.tmp)

#
# Now the "fake" rules for generating .d files.
#

%.d: %.c $(GENERATED_C_HDRS)
	@echo "$<" >> cdepend.tmp

%.d: %.cc $(GENERATED_CXX_HDRS)
	@echo "$<" >> cxxdepend.tmp

%.d: %.s $(GENERATED_AS_HDRS)
	@echo "$<" >> asdepend.tmp

#
# The depend.phony rule which actually runs omkdepend.
#

depend.phony:
	@if [ -f cdepend.tmp ]; then \
	   echo $(CMAKEDEPEND) $(CPPFLAGS) `cat cdepend.tmp`; \
	   $(CMAKEDEPEND) $(CPPFLAGS) `cat cdepend.tmp`; \
	   rm -f cdepend.tmp; \
	 fi; \
	 if [ -f cxxdepend.tmp ]; then \
	   echo $(CXXMAKEDEPEND) $(CPPFLAGS) `cat cxxdepend.tmp`; \
	   $(CXXMAKEDEPEND) $(CPPFLAGS) `cat cxxdepend.tmp`; \
	   rm -f cxxdepend.tmp; \
	 fi; \
	 if [ -f asdepend.tmp ]; then \
	   echo $(ASMAKEDEPEND) $(CPPFLAGS) `cat asdepend.tmp`; \
	   $(ASMAKEDEPEND) $(CPPFLAGS) `cat asdepend.tmp`; \
	   rm -f asdepend.tmp; \
	 fi

#
# Now include the .d files and the "depend.phony" file which never exists.
# For some reason GNU make evaluates the targets in reverse order, so we need
# to include depend.phony first.  The "-" tells make not to complain that it
# can't find the file.
#
# Don't do any includes for targets "clean" and "veryclean" - so if you do
# an "omake veryclean" or "omake clean" in an empty directory the .d files
# won't get generated.
#

ifneq ($(OMAKE_TARGET),clean)
ifneq ($(OMAKE_TARGET),veryclean)

-include depend.phony

ifdef SRCS
-include $(SRCS:.c=.d)
endif

ifdef CXXSRCS
-include $(CXXSRCS:.cc=.d)
endif

ifdef ASSRCS
-include $(ASSRCS:.s=.d)
endif
endif
endif