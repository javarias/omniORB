PYLIBROOT= $(EXPORT_TREE)/lib/python
PYLIBDIR = $(PYLIBROOT)/CosNaming__POA
INSTALLPYLIBDIR = $(INSTALLPYTHONDIR)/CosNaming__POA

FILES = __init__.py

export:: $(FILES)
	@(dir="$(PYLIBDIR)"; \
          for file in $^; do \
            $(ExportFileToDir) \
          done; \
          cd $(PYLIBDIR); \
	  $(PYTHON) -c "import compileall; compileall.compile_dir('.')"; \
	 )

ifdef INSTALLTARGET
install:: $(FILES)
	@(dir="$(INSTALLPYLIBDIR)"; \
          for file in $^; do \
            $(ExportFileToDir) \
          done; \
          cd $(INSTALLPYLIBDIR); \
	  $(PYTHON) -c "import compileall; compileall.compile_dir('.')"; \
	 )
endif
