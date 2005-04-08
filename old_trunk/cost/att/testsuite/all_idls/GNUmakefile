TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

BASIC = \
       array1 \
       attrEcho \
       attrObjRef \
       bool \
       child \
       const1 \
       testecho \
       derivedEcho \
       enums \
       foo \
       inheritance_1 \
       inheritance_2 \
       intf1 \
       intf2 \
       intf3 \
       intf4 \
       manager \
       mrBook \
       multiEcho \
       nestnest \
       nestclash \
       passref \
       refp \
       renderman \
       seqEcho \
       seqtest \
       shutdown \
       struct1 \
       struct2 \
       tstexcept \
       ttcp \
       typedef1 \
       typedef2 \
       typedef3 \
       union1 \
       union2 \
       union3 \
       varTest \
       varTest2 \
       scopetest \
       intf_fwd1 \
       enum1 \
       struct1a \
       struct1b \
       struct1c \
       structNest \
       except1a \
       except1b \
       except1c \
       except2 \
       union1a \
       union1b \
       union1c \
       union1b1 \
       union1c1 \
       typedef1a \
       typedef1b \
       typedef1c \
       array1a \
       array1b \
       array1c \
       recTest \
       recTest2 \
       recUnion \
       recUnion2 \
       wstringtest

ANY   = anyTest \
	anySeq2 \
	testAnyUnion1 \
	testAnyUnion2 \
	any_sequence \
	any_array \
	any_struct \
	any_except \
	any_union \
	testAnyObjEnum \
	anyStructTest \
	anyUnionTest

SEQ  = seq2 \
       seq3 \
       seq4 \
       seq5 \
       seq6 \
       seq7 \
       seq8 \
       seq9 \
       seq10 \
       seq1 \
       array2a \
       sequence1a \
       sequence1b \
       sequence1c \
       seqAlias1 \
       seqArray1 \
       seqArray2 \
       seqArray3 \
       structforward


TYPECODE = \
	typecode \
	tcSequence \
	tcArray \
	tcStruct \
	tcExcept \
	tcUnion \
	typecodeTest

BUGS = \
       bug1 \
       bug2 \
       bugReport \
       virtualbug \
       bug971222 \
       bug_raykov_990208 \
       bug990923 \
       bug200823


INTFS = $(BASIC) $(ANY) $(SEQ) $(TYPECODE) $(BUGS)

all:: $(INTF_OBJS)

include ../mk/afterdir.mk
