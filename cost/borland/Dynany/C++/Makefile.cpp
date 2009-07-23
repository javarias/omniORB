include ../../stdmk/stdmk

EXE = 	Client Server 

all: $(EXE)

clean:
	-rm -f core *.o *.hh *.cc $(EXE) 
	-rm -rf Templates.DB

#
# "Bank" specific make rules
#


Printer_c.cc: Printer.idl
	$(ORBCC) -type_code_info Printer.idl

Printer_s.cc: Printer.idl
	$(ORBCC) -type_code_info Printer.idl

Client: Printer_c.o Client.o 
	$(CC) -o Client Client.o \
	Printer_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

Server: Printer_s.o Printer_c.o Server.o
	$(CC) -o Server Server.o \
	Printer_s.o Printer_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)
