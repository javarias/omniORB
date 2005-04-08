# Makefile
include ../../stdmk/stdmk

EXE = client server

all: $(EXE)

clean: 
	rm -f $(EXE) *.o core *_[cs].*

CLIENT_OBJS = TypeCode_c.o client.o anystuff.o

SERVER_OBJS = TypeCode_c.o TypeCode_s.o server.o

TypeCode_c.cc TypeCode_s.cc TypeCode_c.hh:	TypeCode.idl
	$(ORBCC) -type_code_info -boa TypeCode.idl

client: $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

server: $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LIBPATH) $(LIBORB) $(STDCC_LIBS)
