// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestMacro.h>

#ifndef HAVE_NO_SKEL_HEADER
#include <Test_skel.h>
#else
#include <Test.h>
#endif

#include <stdlib.h>
#include <errno.h>

#include <fstream>

using namespace CORBA;

//-----------------------------------------------------------------------------
// a few useful tools
//
static CORBA::Boolean
aliased(
    const char *lhs,
    const char *rhs
)
{
    return (lhs == rhs) ? 1 : 0;
}

static CORBA::Boolean
deep_copied(
    const char *lhs,
    const char *rhs
)
{
    if (lhs == 0 && rhs == 0) return 1;
    if (lhs == 0 && rhs != 0) return 0;
    if (lhs != 0 && rhs == 0) return 0;
    if (lhs == rhs)           return 0; // aliased, not deep copied

    return (strcmp(lhs, rhs) == 0) ? 1 : 0;
}

//-----------------------------------------------------------------------------
// An implementation of the StringTest interface
//
class Impl : public POA_StringTest
{
  public:
    virtual char*
    op(
        const char        *in_string,
        char              *&inout_string,
        CORBA::String_out out_string
    );
};

char*
Impl::op(
    const char        *in_string,
    char              *&inout_string,
    CORBA::String_out out_string
    )
{
    CORBA::string_free(inout_string);
    inout_string = CORBA::string_dup(in_string);
    out_string = CORBA::string_dup(in_string);
    return CORBA::string_dup(in_string);
}

//-----------------------------------------------------------------------------
// A class to drive this test
//

class Tester
{
  public:
    Tester(StringTest_ptr);
    ~Tester() { };

    void
    run();

  private:

    void assign_to_charstar();
        void charstar_from_charstar();
        void charstar_from_string_var();
        void charstar_from_struct_string_member();
        void charstar_from_exception_string_member();
        void charstar_from_sequence_string_item();
        void charstar_from_array_string_item();
        void charstar_from_any();

    void assign_to_const_charstar();
        void const_charstar_from_charstar();
        void const_charstar_from_const_charstar();
        void const_charstar_from_union_string_member();
        void const_charstar_from_string_var();
        void const_charstar_from_struct_string_member();
        void const_charstar_from_exception_string_member();
        void const_charstar_from_sequence_string_item();
        void const_charstar_from_array_string_item();

    void assign_to_string_var();
        void string_var_from_charstar();
        void string_var_from_const_charstar();
        void string_var_from_union_string_member();
        void string_var_from_string_var();
        void string_var_from_struct_string_member();
        void string_var_from_exception_string_member();
        void string_var_from_sequence_string_item();
        void string_var_from_array_string_item();
        void string_var_subscript();

    void assign_to_struct_string_member();
        void struct_string_member_from_charstar();
        void struct_string_member_from_const_charstar();
        void struct_string_member_from_union_string_member();
        void struct_string_member_from_string_var();
        void struct_string_member_from_struct_string_member();
        void struct_string_member_from_exception_string_member();
        void struct_string_member_from_sequence_string_item();
        void struct_string_member_from_array_string_item();
        void struct_string_member_subscript();

    void assign_to_exception_string_member();
        void exception_string_member_from_charstar();
        void exception_string_member_from_const_charstar();
        void exception_string_member_from_union_string_member();
        void exception_string_member_from_string_var();
        void exception_string_member_from_struct_string_member();
        void exception_string_member_from_exception_string_member();
        void exception_string_member_from_sequence_string_item();
        void exception_string_member_from_array_string_item();

    void assign_to_union_string_member();
        void union_string_member_from_charstar();
        void union_string_member_from_const_charstar();
        void union_string_member_from_union_string_member();
        void union_string_member_from_string_var();
        void union_string_member_from_struct_string_member();
        void union_string_member_from_exception_string_member();
        void union_string_member_from_sequence_string_item();
        void union_string_member_from_array_string_item();

    void assign_to_sequence_string_item();
        void sequence_string_item_from_charstar();
        void sequence_string_item_from_const_charstar();
        void sequence_string_item_from_union_string_member();
        void sequence_string_item_from_string_var();
        void sequence_string_item_from_struct_string_member();
        void sequence_string_item_from_exception_string_member();
        void sequence_string_item_from_sequence_string_item();
        void sequence_string_item_from_array_string_item();
        void sequence_string_item_subscript();

    void assign_to_array_string_item();
        void array_string_item_from_charstar();
        void array_string_item_from_const_charstar();
        void array_string_item_from_union_string_member();
        void array_string_item_from_string_var();
        void array_string_item_from_struct_string_member();
        void array_string_item_from_exception_string_member();
        void array_string_item_from_sequence_string_item();
        void array_string_item_from_array_string_item();
        void array_string_item_subscript();

    void arglist();
        void arglist_from_charstar();
        void arglist_from_string_var();
        void arglist_from_struct_string_member(); 
        void arglist_from_exception_string_member();
        void arglist_from_union_string_member();
        void arglist_from_array_string_item();
        void arglist_from_sequence_string_item();

    const char *initial_value();
    const char *in_value();
    const char *initial_inout_value();

    StringTest_ptr impl_oref();

    StringTest_ptr m_impl_oref;
};

//-----------------------------------------------------------------------------

Tester::Tester(StringTest_ptr m_impl)
{
    m_impl_oref = m_impl;
}

const char*
Tester::initial_value()
{
    return "initial_value";
}

const char*
Tester::in_value()
{
    return "in_value";
}

const char*
Tester::initial_inout_value()
{
    return "initial_inout_value";
}

StringTest_ptr
Tester::impl_oref()
{
    return m_impl_oref;
}

//-----------------------------------------------------------------------------

void
Tester::charstar_from_charstar()
{
    ITTEST_INFO("charstar_from_charstar");
    char *from = (char *)initial_value();
    char *to = from;
    TEST(aliased(to, from));
}

void
Tester::charstar_from_string_var()
{
    ITTEST_INFO("charstar_from_string_var");
    CORBA::String_var from = initial_value();
    char *to = from;
    TEST(aliased(to, from));
}

void
Tester::charstar_from_struct_string_member()
{
    ITTEST_INFO("charstar_from_struct_string_member");
    StringStruct from;
    from.string_member = initial_value();
    char *to = from.string_member;
    TEST(aliased(to, from.string_member));
}

void
Tester::charstar_from_exception_string_member()
{
    ITTEST_INFO("charstar_from_exception_string_member");
    StringException from;
    from.string_member = initial_value();
    char *to = from.string_member;
    TEST(aliased(to, from.string_member));
}

void
Tester::charstar_from_sequence_string_item()
{
    ITTEST_INFO("charstar_from_sequence_string_item");
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    char *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::charstar_from_array_string_item()
{
    ITTEST_INFO("charstar_from_array_string_item");
    StringArray from;
    from[0] = initial_value();
    char *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::charstar_from_any()
{
    ITTEST_INFO("charstar_from_any");
    CORBA::Any from;
    from <<= initial_value();
    const char *to;
    TEST(from >>= to);
    TEST(deep_copied(to, initial_value()));
}

void
Tester::assign_to_charstar()
{
    charstar_from_charstar();
    charstar_from_string_var();
    charstar_from_struct_string_member();
    charstar_from_exception_string_member();
    charstar_from_sequence_string_item();
    charstar_from_array_string_item();
    charstar_from_any();
}

//-----------------------------------------------------------------------------

void
Tester::const_charstar_from_charstar()
{
    ITTEST_INFO("const_charstar_from_charstar");
    char *from = (char*)initial_value();
    char *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_charstar_from_const_charstar()
{
    ITTEST_INFO("const_charstar_from_const_charstar");
    const char *from = initial_value();
    const char *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_charstar_from_union_string_member()
{
    ITTEST_INFO("const_charstar_from_union_string_member");
    StringUnion from;
    const char* foo1 = "foo";
    from.string_member(foo1);
    from._d(0);
    from.string_member(initial_value());
    const char *to = from.string_member();
    TEST(aliased(to, from.string_member()));
}

void
Tester::const_charstar_from_string_var()
{
    ITTEST_INFO("const_charstar_from_string_var");
    CORBA::String_var from = initial_value();
    const char *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_charstar_from_struct_string_member()
{
    ITTEST_INFO("const_charstar_from_struct_string_member");
    StringStruct from;
    from.string_member = initial_value();
    const char *to = from.string_member;
    TEST(aliased(to, from.string_member));
}

void
Tester::const_charstar_from_exception_string_member()
{
    ITTEST_INFO("const_charstar_from_exception_string_member");
    StringException from;
    from.string_member = initial_value();
    const char *to = from.string_member;
    TEST(aliased(to, from.string_member));
}

void
Tester::const_charstar_from_sequence_string_item()
{
    ITTEST_INFO("const_charstar_from_sequence_string_item");
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    const char *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::const_charstar_from_array_string_item()
{
    ITTEST_INFO("const_charstar_from_array_string_item");
    StringArray from;
    from[0] = initial_value();
    const char *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::assign_to_const_charstar()
{
    const_charstar_from_charstar();
    const_charstar_from_const_charstar();
    const_charstar_from_union_string_member();
    const_charstar_from_string_var();
    const_charstar_from_struct_string_member();
    const_charstar_from_exception_string_member();
    const_charstar_from_sequence_string_item();
    const_charstar_from_array_string_item();
}

//-----------------------------------------------------------------------------

void
Tester::string_var_from_charstar()
{
    {
	char *from = CORBA::string_dup(initial_value());
	CORBA::String_var to = from;
	TEST(aliased(to, from));
    }
    {
	char *from = CORBA::string_dup(initial_value());
	CORBA::String_var to;
	to = from;
	TEST(aliased(to, from));
    }
    {
	char *from = CORBA::string_dup(initial_value());
	CORBA::String_var to = from;
	const char *addr1 = to;
	to = from;
	TEST(aliased(to, addr1));
    }
}

void
Tester::string_var_from_const_charstar()
{
    const char *from = initial_value();
    {
	CORBA::String_var to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::String_var to;
	to = from;
	TEST(deep_copied(to, from));
    }
}

void
Tester::string_var_from_union_string_member()
{
    StringUnion from;
    const char* bar1 = "bar";
    from.string_member(bar1);
    from._d(0);
    from.string_member(initial_value());
    {
	CORBA::String_var to = from.string_member();
	TEST(deep_copied(to, from.string_member()));
    }
    {
	CORBA::String_var to;
	to = from.string_member();
	TEST(deep_copied(to, from.string_member()));
    }
}

void
Tester::string_var_from_string_var()
{
    CORBA::String_var from = initial_value();
    {
	CORBA::String_var to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::String_var to;
	to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::String_var to = from;
	const char *addr1 = to;
	to = to;
	TEST(aliased(to, addr1));
    }
}

void
Tester::string_var_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    {
	CORBA::String_var to = from.string_member;
	TEST(deep_copied(to, from.string_member));
    }
    {
	CORBA::String_var to;
	to = from.string_member;
	TEST(deep_copied(to, from.string_member));
    }
}

void
Tester::string_var_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    {
	CORBA::String_var to = from.string_member;
	TEST(deep_copied(to, from.string_member));
    }
    {
	CORBA::String_var to;
	to = from.string_member;
	TEST(deep_copied(to, from.string_member));
    }
}

void
Tester::string_var_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    {
	CORBA::String_var to = from[0];
	TEST(deep_copied(to, from[0]));
    }
    {
	CORBA::String_var to;
	to = from[0];
	TEST(deep_copied(to, from[0]));
    }
}

void
Tester::string_var_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    {
	CORBA::String_var to = from[0];
	TEST(deep_copied(to, from[0]));
    }
    {
	CORBA::String_var to;
	to = from[0];
	TEST(deep_copied(to, from[0]));
    }
}

void
Tester::string_var_subscript()
{
    CORBA::String_var s1 = CORBA::string_dup(initial_value());
    s1[0] = 'X';
    TEST(s1[0] == 'X');

    const CORBA::String_var s2(s1);
    TEST(s2[0] == 'X');
    char c = s2[0];
    TEST(c == 'X');
}

void
Tester::assign_to_string_var()
{
    string_var_from_charstar();
    string_var_from_const_charstar();
    string_var_from_union_string_member();
    string_var_from_string_var();
    string_var_from_struct_string_member();
    string_var_from_exception_string_member();
    string_var_from_sequence_string_item();
    string_var_from_array_string_item();
    string_var_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::struct_string_member_from_charstar()
{
    char *from = CORBA::string_dup(initial_value());
    StringStruct to;
    to.string_member = from;
    TEST(aliased(to.string_member, from));
    const char *addr1 = to.string_member;
    to.string_member = from;
    TEST(aliased(to.string_member, addr1));
}

void
Tester::struct_string_member_from_const_charstar()
{
    const char *from = initial_value();
    StringStruct to;
    to.string_member = from;
    TEST(deep_copied(to.string_member, from));
}

void
Tester::struct_string_member_from_union_string_member()
{
    StringUnion from;
    const char* foo1 = "foo";
    from.string_member(foo1);
    from._d(0);
    from.string_member(initial_value());
    StringStruct to;
    to.string_member = from.string_member();
    TEST(deep_copied(to.string_member, from.string_member()));
}

void
Tester::struct_string_member_from_string_var()
{
    CORBA::String_var from = initial_value();
    StringStruct to;
    to.string_member = from;
    TEST(deep_copied(to.string_member, from));
}

void
Tester::struct_string_member_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    StringStruct to;
    to.string_member = from.string_member;
    TEST(deep_copied(to.string_member, from.string_member));
    const char *addr1 = to.string_member;
    to.string_member = to.string_member;
    TEST(aliased(to.string_member, addr1));
}

void
Tester::struct_string_member_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    StringStruct to;
    to.string_member = from.string_member;
    TEST(deep_copied(to.string_member, from.string_member));
}

void
Tester::struct_string_member_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    StringStruct to;
    to.string_member = from[0];
    TEST(deep_copied(to.string_member, from[0]));
}

void
Tester::struct_string_member_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    StringStruct to;
    to.string_member = from[0];
    TEST(deep_copied(to.string_member, from[0]));
}

void
Tester::struct_string_member_subscript()
{
    StringStruct s1;
    s1.string_member = CORBA::string_dup(initial_value());
    s1.string_member[0] = 'X';
    TEST(s1.string_member[0] == 'X');

    const StringStruct s2(s1);
    TEST(s2.string_member[0] == 'X');
    char c = s2.string_member[0];
    TEST(c == 'X');
}

void
Tester::assign_to_struct_string_member()
{
    struct_string_member_from_charstar();
    struct_string_member_from_const_charstar();
    struct_string_member_from_union_string_member();
    struct_string_member_from_string_var();
    struct_string_member_from_struct_string_member();
    struct_string_member_from_exception_string_member();
    struct_string_member_from_sequence_string_item();
    struct_string_member_from_array_string_item();
    struct_string_member_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::exception_string_member_from_charstar()
{
    {
	char *from = CORBA::string_dup(initial_value());
	StringException to;
	to.string_member = from;
	TEST(aliased(to.string_member, from));
	const char *addr1 = to.string_member;
	to.string_member = from;
	TEST(aliased(to.string_member, addr1));
    }
    {
	char *from = CORBA::string_dup(initial_value());
	StringException to(from); // doesn't adopt from
	TEST(deep_copied(to.string_member, from));
	CORBA::string_free(from);
    }
}

void
Tester::exception_string_member_from_const_charstar()
{
    const char *from = initial_value();
    {
	StringException to;
	to.string_member = from;
	TEST(deep_copied(to.string_member, from));
    }
    {
	StringException to(from);
	TEST(deep_copied(to.string_member, from));
    }
}

void
Tester::exception_string_member_from_union_string_member()
{
    StringUnion from;
    const char* bar1 = "bar";
    from.string_member(bar1);
    from._d(0);
    from.string_member(initial_value());
    {
        StringException to;
        to.string_member = from.string_member();
        TEST(deep_copied(to.string_member, from.string_member()));
    }
    {
        StringException to(from.string_member());
        TEST(deep_copied(to.string_member, from.string_member()));
    }
}

void
Tester::exception_string_member_from_string_var()
{
    CORBA::String_var from = initial_value();
    {
	StringException to;
	to.string_member = from;
	TEST(deep_copied(to.string_member, from));
    }
    {
	StringException to(from);
	TEST(deep_copied(to.string_member, from));
    }
}

void
Tester::exception_string_member_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    {
	StringException to;
	to.string_member = from.string_member;
	TEST(deep_copied(to.string_member, from.string_member));
    }
    {
	StringException to(from.string_member);
	TEST(deep_copied(to.string_member, from.string_member));
    }
}

void
Tester::exception_string_member_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    {
	StringException to;
	to.string_member = from.string_member;
	TEST(deep_copied(to.string_member, from.string_member));
	const char *addr1 = to.string_member;
	to.string_member = to.string_member;
	TEST(aliased(to.string_member, addr1));
    }
    {
	StringException to(from.string_member);
	TEST(deep_copied(to.string_member, from.string_member));
    }
}

void
Tester::exception_string_member_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    {
        StringException to;
        to.string_member = from[0];
        TEST(deep_copied(to.string_member, from[0]));
    }
    {
        StringException to(from[0]);
        TEST(deep_copied(to.string_member, from[0]));
    }
}

void
Tester::exception_string_member_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    {
        StringException to;
        to.string_member = from[0];
        TEST(deep_copied(to.string_member, from[0]));
    }
    {
        StringException to(from[0]);
        TEST(deep_copied(to.string_member, from[0]));
    }
}

void
Tester::assign_to_exception_string_member()
{
    exception_string_member_from_charstar();
    exception_string_member_from_const_charstar();
    exception_string_member_from_union_string_member();
    exception_string_member_from_string_var();
    exception_string_member_from_struct_string_member();
    exception_string_member_from_exception_string_member();
    exception_string_member_from_sequence_string_item();
    exception_string_member_from_array_string_item();
}

//-----------------------------------------------------------------------------

void
Tester::union_string_member_from_charstar()
{
    char *from = CORBA::string_dup(initial_value());
    StringUnion to;
    to.string_member(from);
    TEST(aliased(to.string_member(), from));
    const char *addr1 = to.string_member();
    to.string_member(from);
    TEST(aliased(to.string_member(), addr1));
}

void
Tester::union_string_member_from_const_charstar()
{
    const char *from = initial_value();
    StringUnion to;
    to.string_member(from);
    TEST(deep_copied(to.string_member(), from));
}

void
Tester::union_string_member_from_union_string_member()
{
    StringUnion from;
    const char* foo1 = "foo";
    from.string_member(foo1);
    from._d(0);
    from.string_member(initial_value());
    StringUnion to;
    to.string_member(from.string_member());
    TEST(deep_copied(to.string_member(), from.string_member()));
    const char *addr1 = to.string_member();
    to.string_member(to.string_member());
    TEST(aliased(to.string_member(), addr1));
}

void
Tester::union_string_member_from_string_var()
{
    CORBA::String_var from = initial_value();
    StringUnion to;
    to.string_member(from);
    TEST(deep_copied(to.string_member(), from));
}

void
Tester::union_string_member_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    StringUnion to;
    to.string_member(from.string_member.in());
    TEST(deep_copied(to.string_member(), from.string_member));
}

void
Tester::union_string_member_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    StringUnion to;
    to.string_member(from.string_member.in());
    TEST(deep_copied(to.string_member(), from.string_member));
}

void
Tester::union_string_member_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    StringUnion to;
    to.string_member(from[0].in());
    TEST(deep_copied(to.string_member(), from[0]));
}

void
Tester::union_string_member_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    StringUnion to;
    to.string_member(from[0].in());
    TEST(deep_copied(to.string_member(), from[0]));
}

void
Tester::assign_to_union_string_member()
{
    union_string_member_from_charstar();
    union_string_member_from_const_charstar();
    union_string_member_from_union_string_member();
    union_string_member_from_string_var();
    union_string_member_from_struct_string_member();
    union_string_member_from_exception_string_member();
    union_string_member_from_sequence_string_item();
    union_string_member_from_array_string_item();
}

//-----------------------------------------------------------------------------

void
Tester::sequence_string_item_from_charstar()
{
    char *from = CORBA::string_dup(initial_value());
    StringSequence to;
    to.length(1);
    to[0] = from;
    TEST(aliased(to[0], from));
    const char *addr1 = to[0];
    to[0] = from;
    TEST(aliased(to[0], addr1));
}

void
Tester::sequence_string_item_from_const_charstar()
{
    const char *from = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::sequence_string_item_from_union_string_member()
{
    StringUnion from;
    const char* bar1 = "bar";
    from.string_member(bar1);
    from._d(0);
    from.string_member(initial_value());
    StringSequence to;
    to.length(1);
    to[0] = from.string_member();
    TEST(deep_copied(to[0], from.string_member()));
}

void
Tester::sequence_string_item_from_string_var()
{
    CORBA::String_var from = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::sequence_string_item_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from.string_member;
    TEST(deep_copied(to[0], from.string_member));
}

void
Tester::sequence_string_item_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from.string_member;
    TEST(deep_copied(to[0], from.string_member));
}

void
Tester::sequence_string_item_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
    const char *addr1 = to[0];
    to[0] = to[0];
    TEST(aliased(to[0], addr1));
}

void
Tester::sequence_string_item_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    StringSequence to;
    to.length(1);
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
}

void
Tester::sequence_string_item_subscript()
{
    StringSequence s1(1);
    s1.length(1);
    s1[0] = CORBA::string_dup(initial_value());
    s1[0][0] = 'X';
    TEST(s1[0][0] == 'X');

    const StringSequence s2(s1);
    TEST(s2[0][0] == 'X');
    char c = s2[0][0];
    TEST(c == 'X');
}

void
Tester::assign_to_sequence_string_item()
{
    sequence_string_item_from_charstar();
    sequence_string_item_from_const_charstar();
    sequence_string_item_from_union_string_member();
    sequence_string_item_from_string_var();
    sequence_string_item_from_struct_string_member();
    sequence_string_item_from_exception_string_member();
    sequence_string_item_from_sequence_string_item();
    sequence_string_item_from_array_string_item();
    sequence_string_item_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::array_string_item_from_charstar()
{
    char *from = CORBA::string_dup(initial_value());
    StringArray to;
    to[0] = from;
    TEST(aliased(to[0], from));
    const char *addr1 = to[0];
    to[0] = from;
    TEST(aliased(to[0], addr1));
}

void
Tester::array_string_item_from_const_charstar()
{
    const char *from = initial_value();
    StringArray to;
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::array_string_item_from_union_string_member()
{
    StringUnion from;
    const char* foo1 = "foo";
    from.string_member(foo1);
    from._d(0);
    from.string_member(initial_value());
    StringArray to;
    to[0] = from.string_member();
    TEST(deep_copied(to[0], from.string_member()));
}

void
Tester::array_string_item_from_string_var()
{
    CORBA::String_var from = initial_value();
    StringArray to;
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::array_string_item_from_struct_string_member()
{
    StringStruct from;
    from.string_member = initial_value();
    StringArray to;
    to[0] = from.string_member;
    TEST(deep_copied(to[0], from.string_member));
}

void
Tester::array_string_item_from_exception_string_member()
{
    StringException from;
    from.string_member = initial_value();
    StringArray to;
    to[0] = from.string_member;
    TEST(deep_copied(to[0], from.string_member));
}

void
Tester::array_string_item_from_sequence_string_item()
{
    StringSequence from;
    from.length(1);
    from[0] = initial_value();
    StringArray to;
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
}

void
Tester::array_string_item_from_array_string_item()
{
    StringArray from;
    from[0] = initial_value();
    StringArray to;
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
    const char *addr1 = to[0];
    to[0] = to[0];
    TEST(aliased(to[0], addr1));
}

void
Tester::array_string_item_subscript()
{
    StringArray s1;
    s1[0] = CORBA::string_dup(initial_value());
    s1[0][0] = 'X';
    TEST(s1[0][0] == 'X');
}

void
Tester::assign_to_array_string_item()
{
    array_string_item_from_charstar();
    array_string_item_from_const_charstar();
    array_string_item_from_union_string_member();
    array_string_item_from_string_var();
    array_string_item_from_struct_string_member();
    array_string_item_from_exception_string_member();
    array_string_item_from_sequence_string_item();
    array_string_item_from_array_string_item();
    array_string_item_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::arglist_from_charstar()
{
    const char* in_string = in_value();
    char* inout_string = CORBA::string_dup(initial_inout_value());
    char  * out_string;

    char *return_string =
	impl_oref()->op(in_string, inout_string, out_string);
    TEST(deep_copied(in_string,  inout_string));
    TEST(deep_copied(in_string,    out_string));
    TEST(deep_copied(in_string, return_string));

    CORBA::string_free(  inout_string );
    CORBA::string_free(    out_string );
    CORBA::string_free( return_string );
}

void
Tester::arglist_from_string_var()
{
    CORBA::String_var    in_string = in_value();
    CORBA::String_var inout_string = initial_inout_value();
    CORBA::String_var   out_string;

    CORBA::String_var return_string =
	impl_oref()->op(in_string, inout_string, out_string);
    TEST(deep_copied(in_string,  inout_string));
    TEST(deep_copied(in_string,    out_string));
    TEST(deep_copied(in_string, return_string));
}

void
Tester::arglist_from_struct_string_member()
{
    StringStruct     in_struct;
    StringStruct  inout_struct;
    StringStruct    out_struct;
    StringStruct return_struct;

       in_struct.string_member = in_value();
    inout_struct.string_member = initial_inout_value();

    return_struct.string_member =
	impl_oref()->op(
	       in_struct.string_member,
	    inout_struct.string_member,
	      out_struct.string_member
	);
    TEST(deep_copied(in_struct.string_member,  inout_struct.string_member));
    TEST(deep_copied(in_struct.string_member,    out_struct.string_member));
    TEST(deep_copied(in_struct.string_member, return_struct.string_member));
}

void
Tester::arglist_from_exception_string_member()
{
    StringException     in_exception;
    StringException  inout_exception;
    StringException    out_exception;
    StringException return_exception;

       in_exception.string_member = in_value();
    inout_exception.string_member = initial_inout_value();

    return_exception.string_member =
	impl_oref()->op(
	       in_exception.string_member,
	    inout_exception.string_member,
	      out_exception.string_member
	);
    TEST(deep_copied(in_exception.string_member,  inout_exception.string_member));
    TEST(deep_copied(in_exception.string_member,    out_exception.string_member));
    TEST(deep_copied(in_exception.string_member, return_exception.string_member));
}

void
Tester::arglist_from_union_string_member()
{
    StringUnion          in_union;
    CORBA::String_var inout_var = initial_inout_value();
    CORBA::String_var   out_var;

    in_union.string_member(in_value());

    CORBA::String_var return_var =
	impl_oref()->op(
	       in_union.string_member(),
	    inout_var,
	      out_var
	);
    TEST(deep_copied(in_union.string_member(), inout_var));
}

void
Tester::arglist_from_array_string_item()
{
    StringArray     in_array;
    StringArray  inout_array;
    StringArray    out_array;
    StringArray return_array;

       in_array[0] = in_value();
    inout_array[0] = initial_inout_value();

    return_array[0] =
	impl_oref()->op(
	       in_array[0],
	    inout_array[0],
	      out_array[0]
	);
    TEST(deep_copied(in_array[0],  inout_array[0]));
    TEST(deep_copied(in_array[0],    out_array[0]));
    TEST(deep_copied(in_array[0], return_array[0]));
}

void
Tester::arglist_from_sequence_string_item()
{
    StringSequence     in_sequence;
    StringSequence  inout_sequence;
    StringSequence    out_sequence;
    StringSequence return_sequence;

        in_sequence.length(1);
     inout_sequence.length(1);
       out_sequence.length(1);
    return_sequence.length(1);

       in_sequence[0] = in_value();
    inout_sequence[0] = initial_inout_value();

    return_sequence[0] =
	impl_oref()->op(
	       in_sequence[0],
	    inout_sequence[0].inout(),
	      out_sequence[0].out()
	);
    TEST(deep_copied(in_sequence[0],  inout_sequence[0]));
    TEST(deep_copied(in_sequence[0],    out_sequence[0]));
    TEST(deep_copied(in_sequence[0], return_sequence[0]));
}

void
Tester::arglist()
{
    arglist_from_charstar();
    arglist_from_string_var();
    arglist_from_struct_string_member(); 
    arglist_from_exception_string_member();
    arglist_from_union_string_member();
    arglist_from_array_string_item();
    arglist_from_sequence_string_item();
}

//-----------------------------------------------------------------------------

void
Tester::run()
{
    assign_to_charstar();
    assign_to_const_charstar();
    assign_to_string_var();
    assign_to_struct_string_member();
    assign_to_exception_string_member();
    assign_to_union_string_member();
    assign_to_sequence_string_item();
    assign_to_array_string_item();
    arglist();
}

//-----------------------------------------------------------------------------

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        orb = ORB_init(argc, argv);

	Impl i;
        StringTest_var server = i._this();

	Tester tester(server);
	tester.run();
    }
    catch(const CORBA::Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        cerr << ex << endl;
#endif
        status = EXIT_FAILURE;
    }

    if(!is_nil(orb))
    {
        try
        {
            orb -> destroy();
        }
        catch(const CORBA::Exception& ex)
        {
#ifdef HAVE_EXCEPTION_INSERTERS
            cerr << ex << endl;
#endif
            status = EXIT_FAILURE;
        }
    }

    return status;
}
