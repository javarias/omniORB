include ../../stdmk/stdmk

EXE = 	Client Server 

all: $(EXE)

clean:
	-rm -f core *.o *.hh *.cc $(EXE) 
	-rm -rf Templates.DB

#
# "Bank" specific make rules
#


Demo_c.cc: Demo.idl
	$(ORBCC) Demo.idl

Demo_s.cc: Demo.idl
	$(ORBCC) Demo.idl

Client: Demo_c.o Client.o 
	$(CC) -o Client Client.o \
	Demo_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

Server: Demo_s.o Demo_c.o Server.o
	$(CC) -o Server Server.o \
	Demo_s.o Demo_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)
