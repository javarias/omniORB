# **********************************************************************
#
# Copyright (c) 2001
# Object Oriented Concepts, Inc.
# Billerica, MA, USA
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir	= ..

LIB_NAME	= libTestMain$(LIB_EXT)
SERVER_NAME	= server
CLIENT_NAME	= client
COLLOCATED_NAME	= collocated

TARGETS		= $(LIB_NAME) \
		  $(SERVER_NAME) \
		  $(CLIENT_NAME) \
		  $(COLLOCATED_NAME)

include $(top_srcdir)/config/Make.rules

LIB_OBJS	= TestIntf.o \
		  TestIntf_skel.o \
		  TestIntfBasic.o \
		  TestIntfBasic_skel.o \
		  TestIntfContext.o \
		  TestIntfContext_skel.o \
		  TestIntfExceptions.o \
		  TestIntfExceptions_skel.o \
		  TestIntfExceptionsExt_2_0.o \
		  TestIntfExceptionsExt_2_0_skel.o \
		  TestIntfWChar.o \
		  TestIntfWChar_skel.o \
		  TestIntfFixed.o \
		  TestIntfFixed_skel.o \
		  TestIntfLongDouble.o \
		  TestIntfLongDouble_skel.o \
		  TestIntfLongLong.o \
		  TestIntfLongLong_skel.o \
		  TestIntfExceptionsExt_2_3.o \
		  TestIntfExceptionsExt_2_3_skel.o \
		  TestIntf_impl.o \
		  TestIntfBasic_impl.o \
		  TestIntfContext_impl.o \
		  TestIntfExceptions_impl.o \
		  TestIntfExceptionsExt_2_0_impl.o \
		  TestIntfWChar_impl.o \
		  TestIntfFixed_impl.o \
		  TestIntfLongDouble_impl.o \
		  TestIntfLongLong_impl.o \
		  TestIntfExceptionsExt_2_3_impl.o \
		  TestIntfBasicDSI_impl.o \
		  TestIntfContextDSI_impl.o \
		  TestIntfExceptionsDSI_impl.o \
		  TestIntfExceptionsExt_2_0DSI_impl.o \
		  TestIntfWCharDSI_impl.o \
		  TestIntfFixedDSI_impl.o \
		  TestIntfLongDoubleDSI_impl.o \
		  TestIntfLongLongDSI_impl.o \
		  TestIntfExceptionsExt_2_3DSI_impl.o \
		  TestObject.o \
		  TestObjectBasic.o \
		  TestObjectContext.o \
		  TestObjectExceptions.o \
		  TestObjectExceptionsExt_2_0.o \
		  TestObjectWChar.o \
		  TestObjectFixed.o \
		  TestObjectLongDouble.o \
		  TestObjectLongLong.o \
		  TestObjectExceptionsExt_2_3.o \
		  Server.o \
		  Client.o

SERVER_OBJS	= ServerMain.o

CLIENT_OBJS	= ClientMain.o

COLLOCATED_OBJS	= CollocatedMain.o

SRCS		= $(LIB_OBJS:.o=.cpp) \
		  $(SERVER_OBJS:.o=.cpp) \
		  $(CLIENT_OBJS:.o=.cpp) \
		  $(COLLOCATED_OBJS:.o=.cpp)

GENERATED	= TestIntf.cpp \
		  TestIntf.h \
		  TestIntf_skel.cpp \
		  TestIntf_skel.h \
		  TestIntfBasic.cpp \
		  TestIntfBasic.h \
		  TestIntfBasic_skel.cpp \
		  TestIntfBasic_skel.h \
		  TestIntfContext.cpp \
		  TestIntfContext.h \
		  TestIntfContext_skel.cpp \
		  TestIntfContext_skel.h \
		  TestIntfExceptions.cpp \
		  TestIntfExceptions.h \
		  TestIntfExceptions_skel.cpp \
		  TestIntfExceptions_skel.h \
		  TestIntfExceptionsExt_2_0.cpp \
		  TestIntfExceptionsExt_2_0.h \
		  TestIntfExceptionsExt_2_0_skel.cpp \
		  TestIntfExceptionsExt_2_0_skel.h \
		  TestIntfWChar.cpp \
		  TestIntfWChar.h \
		  TestIntfWChar_skel.cpp \
		  TestIntfWChar_skel.h \
		  TestIntfFixed.cpp \
		  TestIntfFixed.h \
		  TestIntfFixed_skel.cpp \
		  TestIntfFixed_skel.h \
		  TestIntfLongDouble.cpp \
		  TestIntfLongDouble.h \
		  TestIntfLongDouble_skel.cpp \
		  TestIntfLongDouble_skel.h \
		  TestIntfLongLong.cpp \
		  TestIntfLongLong.h \
		  TestIntfLongLong_skel.cpp \
		  TestIntfLongLong_skel.h \
		  TestIntfExceptionsExt_2_3.cpp \
		  TestIntfExceptionsExt_2_3.h \
		  TestIntfExceptionsExt_2_3_skel.cpp \
		  TestIntfExceptionsExt_2_3_skel.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS	= $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS	= -lTestMain $(ORB_LIBS) $(LIBS)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS)

$(SERVER_NAME): $(SERVER_OBJS) $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(SERVER_OBJS) $(ALL_LIBS)
 
$(CLIENT_NAME): $(CLIENT_OBJS) $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(CLIENT_OBJS) $(ALL_LIBS)

$(COLLOCATED_NAME): $(COLLOCATED_OBJS) $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(COLLOCATED_OBJS) \
        $(ALL_LIBS)


TestIntfExceptionsExt_2_3.cpp TestIntfExceptionsExt_2_3.h \
TestIntfExceptionsExt_2_3_skel.cpp TestIntfExceptionsExt_2_3_skel.h: \
	TestIntfExceptionsExt_2_3.idl
	rm -f TestIntfExceptionsExt_2_3.cpp TestIntfExceptionsExt_2_3.h \
	TestIntfExceptionsExt_2_3_skel.cpp TestIntfExceptionsExt_2_3_skel.
	$(IDL) $(IDLFLAGS) TestIntfExceptionsExt_2_3.idl
	@if test ! -f TestIntfExceptionsExt_2_3_skel.h ; \
	then \
	    ln -s TestIntfExceptionsExt_2_3.h TestIntfExceptionsExt_2_3_skel.h ; \
	fi
	@if test ! -f TestIntfExceptionsExt_2_3_skel.cpp ; \
	then \
	    ln -s TestIntfExceptionsExt_2_3.cpp TestIntfExceptionsExt_2_3_skel.cpp ; \
	fi

TestIntfLongLong.cpp TestIntfLongLong.h \
TestIntfLongLong_skel.cpp TestIntfLongLong_skel.h: \
	TestIntfLongLong.idl
	rm -f TestIntfLongLong.cpp TestIntfLongLong.h \
	TestIntfLongLong_skel.cpp TestIntfLongLong_skel.h
	$(IDL) $(IDLFLAGS) TestIntfLongLong.idl
	@if test ! -f TestIntfLongLong_skel.h ; \
	then \
	    ln -s TestIntfLongLong.h TestIntfLongLong_skel.h ; \
	fi
	@if test ! -f TestIntfLongLong_skel.cpp ; \
	then \
	    ln -s TestIntfLongLong.cpp TestIntfLongLong_skel.cpp ; \
	fi

TestIntfLongDouble.cpp TestIntfLongDouble.h \
TestIntfLongDouble_skel.cpp TestIntfLongDouble_skel.h: \
	TestIntfLongDouble.idl
	rm -f TestIntfLongDouble.cpp TestIntfLongDouble.h \
	TestIntfLongDouble_skel.cpp TestIntfLongDouble_skel.h
	$(IDL) $(IDLFLAGS) TestIntfLongDouble.idl
	@if test ! -f TestIntfLongDouble_skel.h ; \
	then \
	    ln -s TestIntfLongDouble.h TestIntfLongDouble_skel.h ; \
	fi
	@if test ! -f TestIntfLongDouble_skel.cpp ; \
	then \
	    ln -s TestIntfLongDouble.cpp TestIntfLongDouble_skel.cpp ; \
	fi

TestIntfFixed.cpp TestIntfFixed.h \
TestIntfFixed_skel.cpp TestIntfFixed_skel.h: \
	TestIntfFixed.idl
	rm -f TestIntfFixed.cpp TestIntfFixed.h \
	TestIntfFixed_skel.cpp TestIntfFixed_skel.h
	$(IDL) $(IDLFLAGS) TestIntfFixed.idl
	@if test ! -f TestIntfFixed_skel.h ; \
	then \
	    ln -s TestIntfFixed.h TestIntfFixed_skel.h ; \
	fi
	@if test ! -f TestIntfFixed_skel.cpp ; \
	then \
	    ln -s TestIntfFixed.cpp TestIntfFixed_skel.cpp ; \
	fi

TestIntfWChar.cpp TestIntfWChar.h \
TestIntfWChar_skel.cpp TestIntfWChar_skel.h: \
	TestIntfWChar.idl
	rm -f TestIntfWChar.cpp TestIntfWChar.h \
	TestIntfWChar_skel.cpp TestIntfWChar_skel.h
	$(IDL) $(IDLFLAGS) TestIntfWChar.idl
	@if test ! -f TestIntfWChar_skel.h ; \
	then \
	    ln -s TestIntfWChar.h TestIntfWChar_skel.h ; \
	fi
	@if test ! -f TestIntfWChar_skel.cpp ; \
	then \
	    ln -s TestIntfWChar.cpp TestIntfWChar_skel.cpp ; \
	fi

TestIntfExceptionsExt_2_0.cpp TestIntfExceptionsExt_2_0.h \
TestIntfExceptionsExt_2_0_skel.cpp TestIntfExceptionsExt_2_0_skel.h: \
	TestIntfExceptionsExt_2_0.idl
	rm -f TestIntfExceptionsExt_2_0.cpp TestIntfExceptionsExt_2_0.h \
	TestIntfExceptionsExt_2_0_skel.cpp TestIntfExceptionsExt_2_0_skel.h
	$(IDL) $(IDLFLAGS) TestIntfExceptionsExt_2_0.idl
	@if test ! -f TestIntfExceptionsExt_2_0_skel.h ; \
	then \
	    ln -s TestIntfExceptionsExt_2_0.h TestIntfExceptionsExt_2_0_skel.h ; \
	fi
	@if test ! -f TestIntfExceptionsExt_2_0_skel.cpp ; \
	then \
	    ln -s TestIntfExceptionsExt_2_0.cpp TestIntfExceptionsExt_2_0_skel.cpp ; \
	fi

TestIntfExceptions.cpp TestIntfExceptions.h \
TestIntfExceptions_skel.cpp TestIntfExceptions_skel.h: \
	TestIntfExceptions.idl
	rm -f TestIntfExceptions.cpp TestIntfExceptions.h \
	TestIntfExceptions_skel.cpp TestIntfExceptions_skel.h
	$(IDL) $(IDLFLAGS) TestIntfExceptions.idl
	@if test ! -f TestIntfExceptions_skel.h ; \
	then \
	    ln -s TestIntfExceptions.h TestIntfExceptions_skel.h ; \
	fi
	@if test ! -f TestIntfExceptions_skel.cpp ; \
	then \
	    ln -s TestIntfExceptions.cpp TestIntfExceptions_skel.cpp ; \
	fi

TestIntfContext.cpp TestIntfContext.h \
TestIntfContext_skel.cpp TestIntContext_skel.h: \
	TestIntfContext.idl
	rm -f TestIntfContext.cpp TestIntfContext.h \
	TestIntfContext_skel.cpp TestIntfContext_skel.h
	$(IDL) $(IDLFLAGS) TestIntfContext.idl
	@if test ! -f TestIntfContext_skel.h ; \
	then \
	    ln -s TestIntfContext.h TestIntfContext_skel.h ; \
	fi
	@if test ! -f TestIntfContext_skel.cpp ; \
	then \
	    ln -s TestIntfContext.cpp TestIntfContext_skel.cpp ; \
	fi

TestIntfBasic.cpp TestIntfBasic.h \
TestIntfBasic_skel.cpp TestIntfBasic_skel.h: \
	TestIntfBasic.idl
	rm -f TestIntfBasic.cpp TestIntfBasic.h \
	TestIntfBasic_skel.cpp TestIntfBasic_skel.h
	$(IDL) $(IDLFLAGS) TestIntfBasic.idl
	@if test ! -f TestIntfBasic_skel.h ; \
	then \
	    ln -s TestIntfBasic.h TestIntfBasic_skel.h ; \
	fi
	@if test ! -f TestIntfBasic_skel.cpp ; \
	then \
	    ln -s TestIntfBasic.cpp TestIntfBasic_skel.cpp ; \
	fi

TestIntf.cpp TestIntf.h \
TestIntf_skel.cpp TestIntf_skel.h: \
	TestIntf.idl
	rm -f TestIntf.cpp TestIntf.h \
	TestIntf_skel.cpp TestIntf_skel.h
	$(IDL) $(IDLFLAGS) TestIntf.idl
	@if test ! -f TestIntf_skel.h ; \
	then \
	    ln -s TestIntf.h TestIntf_skel.h ; \
	fi
	@if test ! -f TestIntf_skel.cpp ; \
	then \
	    ln -s TestIntf.cpp TestIntf_skel.cpp ; \
	fi

clean::
	rm -f $(LIB_NAME)
	rm -f TestIntf.ref
