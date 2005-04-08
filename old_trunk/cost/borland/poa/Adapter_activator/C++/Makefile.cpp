include ../../../stdmk/stdmk

EXE = 	Client Server 

all: $(EXE)

clean:
	-rm -f core *.o *.hh *.cc $(EXE) 
	-rm -rf Templates.DB

#
# "Bank" specific make rules
#


Bank_c.cc: Bank.idl
	$(ORBCC) Bank.idl

Bank_s.cc: Bank.idl
	$(ORBCC) Bank.idl

Client: Bank_c.o Client.o 
	$(CC) -o Client Client.o \
	Bank_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

Server: Bank_s.o Bank_c.o Server.o
	$(CC) -o Server Server.o \
	Bank_s.o Bank_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)
