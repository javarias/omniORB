# Check for Python, and complain early.
ifndef PYTHON
all::
	@$(NoPythonError)
export::
	@$(NoPythonError)
endif

SUBDIRS += tool

SUBDIRS += lib appl

all::
	@echo
	@echo 'No "all" rule here.  If you want to build and export everything'
	@echo 'use "gnumake export".  Otherwise build subdirectories separately.'
	@echo

export::
	@$(MakeSubdirs)

clean::
	rm -rf stub
