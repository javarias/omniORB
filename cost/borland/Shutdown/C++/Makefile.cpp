include ../../stdmk/stdmk

EXE = 	account_client account_server shutdown 

##all: checkenv $(EXE)
all: $(EXE)

clean:
	rm -f *.o *.hh *_c.cc *_s.cc $(EXE) *.log

#
# "Account" specific make rules
#

account_c.cc account_s.cc: account.idl
	$(ORBCC) -boa account.idl

account_client: account_c.o account_clnt.o 
	$(CC) -o account_client account_clnt.o account_c.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)

shutdown: account_c.o shutdown.o 
	$(CC) -o shutdown shutdown.o account_c.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)

account_server: account_s.o account_c.o account_srvr.o
	$(CC) -o account_server account_srvr.o account_s.o \
	account_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

