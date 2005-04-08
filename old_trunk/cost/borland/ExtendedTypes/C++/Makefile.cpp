# Makefile
include ../../stdmk/stdmk

EXE = server client
UCFLAG = -D_VIS_UNICODE
XTRA_FLAGS=-DNO_LONG_DOUBLE

##all: checkenv $(EXE)
all: print_warning $(EXE)

clean: 
	rm -f $(EXE) *.o core *_[cs].hh *_[cs].cc
	rm -rf Templates.DB

print_warning:
	@echo "Please read the following note:"
	@echo "If you are testing between Windows/UNIX"
	@echo "please set environment XTRA_FLAGS=-DNO_LONG_DOUBLE"
	@echo "and use gmake -e option"
	@echo "Windows does not support the longdouble type."

extest_c.cc extest_s.cc extest_c.hh extest_s.hh: extest.idl
	$(ORBCC) -boa -type_code_info $(XTRA_FLAGS) extest.idl

client.o: client.C
	$(CC) $(CCFLAGS) $(UCFLAG) $(XTRA_FLAGS) -c -o $@ $<

server.o: server.C
	$(CC) $(CCFLAGS) $(UCFLAG) $(XTRA_FLAGS) -c -o $@ $<

extest_s.o: extest_s.cc
	$(CC) $(CCFLAGS) $(UCFLAG) $(XTRA_FLAGS) -c -o $@ $<

extest_c.o: extest_c.cc
	$(CC) $(CCFLAGS) $(UCFLAG) $(XTRA_FLAGS) -c -o $@ $<

client: extest_c.o client.o
	$(CC) -o client  client.o extest_c.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)

server: extest_c.o extest_s.o server.o
	$(CC) -o server server.o extest_c.o extest_s.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)
