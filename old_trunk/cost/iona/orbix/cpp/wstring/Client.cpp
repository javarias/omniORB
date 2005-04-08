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
int
mywcscmp(const wchar_t* ws1, const wchar_t* ws2)
{
  wchar_t wc1, wc2;
 
  do
  {
      wc1 = (wchar_t)*ws1++;
      wc2 = (wchar_t)*ws2++;
      if(!wc1)
	  return wc1 - wc2;
  }
  while(wc1 == wc2);
 
  return wc1 - wc2;
}

static CORBA::Boolean
aliased(
    const CORBA::WChar *lhs,
    const CORBA::WChar *rhs
)
{
    return (lhs == rhs) ? 1 : 0;
}

static CORBA::Boolean
deep_copied(
    const CORBA::WChar *lhs,
    const CORBA::WChar *rhs
)
{
    if (lhs == 0 && rhs == 0) return 1;
    if (lhs == 0 && rhs != 0) return 0;
    if (lhs != 0 && rhs == 0) return 0;
    if (lhs == rhs)           return 0; // aliased, not deep copied

    return (mywcscmp(lhs, rhs) == 0) ? 1 : 0;
}

//-----------------------------------------------------------------------------
// An implementation of the WStringTest interface
//
class Impl : public POA_WStringTest
{
  public:
    virtual CORBA::WChar*
    op(
        const CORBA::WChar *in_string,
        CORBA::WChar       *&inout_string,
        CORBA::WString_out out_string
    );
};

CORBA::WChar*
Impl::op(
    const CORBA::WChar *in_string,
    CORBA::WChar       *&inout_string,
    CORBA::WString_out out_string
)
{
    CORBA::wstring_free(inout_string);
    inout_string = CORBA::wstring_dup(in_string);
    out_string = CORBA::wstring_dup(in_string);
    return CORBA::wstring_dup(in_string);
}

//-----------------------------------------------------------------------------
// A class to drive this test
//

class Tester
{
  public:
    Tester(WStringTest_ptr);
    ~Tester() { };

    void
    run();

  private:

    void assign_to_wcharstar();
        void wcharstar_from_wcharstar();
        void wcharstar_from_wstring_var();
        void wcharstar_from_struct_wstring_member();
        void wcharstar_from_exception_wstring_member();
        void wcharstar_from_sequence_wstring_item();
        void wcharstar_from_array_wstring_item();
        void wcharstar_from_any();

    void assign_to_const_wcharstar();
        void const_wcharstar_from_wcharstar();
        void const_wcharstar_from_const_wcharstar();
        void const_wcharstar_from_union_wstring_member();
        void const_wcharstar_from_wstring_var();
        void const_wcharstar_from_struct_wstring_member();
        void const_wcharstar_from_exception_wstring_member();
        void const_wcharstar_from_sequence_wstring_item();
        void const_wcharstar_from_array_wstring_item();

    void assign_to_wstring_var();
        void wstring_var_from_wcharstar();
        void wstring_var_from_const_wcharstar();
        void wstring_var_from_union_wstring_member();
        void wstring_var_from_wstring_var();
        void wstring_var_from_struct_wstring_member();
        void wstring_var_from_exception_wstring_member();
        void wstring_var_from_sequence_wstring_item();
        void wstring_var_from_array_wstring_item();
        void wstring_var_subscript();

    void assign_to_struct_wstring_member();
        void struct_wstring_member_from_wcharstar();
        void struct_wstring_member_from_const_wcharstar();
        void struct_wstring_member_from_union_wstring_member();
        void struct_wstring_member_from_wstring_var();
        void struct_wstring_member_from_struct_wstring_member();
        void struct_wstring_member_from_exception_wstring_member();
        void struct_wstring_member_from_sequence_wstring_item();
        void struct_wstring_member_from_array_wstring_item();
	void struct_wstring_member_subscript();

    void assign_to_exception_wstring_member();
        void exception_wstring_member_from_wcharstar();
        void exception_wstring_member_from_const_wcharstar();
        void exception_wstring_member_from_union_wstring_member();
        void exception_wstring_member_from_wstring_var();
        void exception_wstring_member_from_struct_wstring_member();
        void exception_wstring_member_from_exception_wstring_member();
        void exception_wstring_member_from_sequence_wstring_item();
        void exception_wstring_member_from_array_wstring_item();

    void assign_to_union_wstring_member();
        void union_wstring_member_from_wcharstar();
        void union_wstring_member_from_const_wcharstar();
        void union_wstring_member_from_union_wstring_member();
        void union_wstring_member_from_wstring_var();
        void union_wstring_member_from_struct_wstring_member();
        void union_wstring_member_from_exception_wstring_member();
        void union_wstring_member_from_sequence_wstring_item();
        void union_wstring_member_from_array_wstring_item();

    void assign_to_sequence_wstring_item();
        void sequence_wstring_item_from_wcharstar();
        void sequence_wstring_item_from_const_wcharstar();
        void sequence_wstring_item_from_union_wstring_member();
        void sequence_wstring_item_from_wstring_var();
        void sequence_wstring_item_from_struct_wstring_member();
        void sequence_wstring_item_from_exception_wstring_member();
        void sequence_wstring_item_from_sequence_wstring_item();
        void sequence_wstring_item_from_array_wstring_item();
	void sequence_wstring_item_subscript();

    void assign_to_array_wstring_item();
        void array_wstring_item_from_wcharstar();
        void array_wstring_item_from_const_wcharstar();
        void array_wstring_item_from_union_wstring_member();
        void array_wstring_item_from_wstring_var();
        void array_wstring_item_from_struct_wstring_member();
        void array_wstring_item_from_exception_wstring_member();
        void array_wstring_item_from_sequence_wstring_item();
        void array_wstring_item_from_array_wstring_item();
	void array_wstring_item_subscript();

    void arglist();
        void arglist_from_wcharstar();
        void arglist_from_wstring_var();
        void arglist_from_struct_wstring_member(); 
        void arglist_from_exception_wstring_member();
        void arglist_from_union_wstring_member();
        void arglist_from_array_wstring_item();
        void arglist_from_sequence_wstring_item();

    const CORBA::WChar *initial_value();
    const CORBA::WChar *in_value();
    const CORBA::WChar *initial_inout_value();

    WStringTest_ptr impl_oref();

    WStringTest_ptr m_impl_oref;
};

//-----------------------------------------------------------------------------

Tester::Tester(WStringTest_ptr m_impl)
{
    m_impl_oref = m_impl;
}

const CORBA::WChar*
Tester::initial_value()
{
    return L"initial_value";
}

const CORBA::WChar*
Tester::in_value()
{
    return L"in_value";
}

const CORBA::WChar*
Tester::initial_inout_value()
{
    return L"initial_inout_value";
}

WStringTest_ptr
Tester::impl_oref()
{
    return m_impl_oref;
}

//-----------------------------------------------------------------------------

void
Tester::wcharstar_from_wcharstar()
{
    ITTEST_INFO("wcharstar_from_wcharstar");
    CORBA::WChar *from = (CORBA::WChar*)initial_value();
    CORBA::WChar *to = from;
    TEST(aliased(to, from));
}

void
Tester::wcharstar_from_wstring_var()
{
    ITTEST_INFO("wcharstar_from_wstring_var");
    CORBA::WString_var from = initial_value();
    CORBA::WChar *to = from;
    TEST(aliased(to, from));
}

void
Tester::wcharstar_from_struct_wstring_member()
{
    ITTEST_INFO("wcharstar_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    CORBA::WChar *to = from.wstring_member;
    TEST(aliased(to, from.wstring_member));
}

void
Tester::wcharstar_from_exception_wstring_member()
{
    ITTEST_INFO("wcharstar_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    CORBA::WChar *to = from.wstring_member;
    TEST(aliased(to, from.wstring_member));
}

void
Tester::wcharstar_from_sequence_wstring_item()
{
    ITTEST_INFO("wcharstar_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    CORBA::WChar *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::wcharstar_from_array_wstring_item()
{
    ITTEST_INFO("wcharstar_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    CORBA::WChar *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::wcharstar_from_any()
{
    ITTEST_INFO("wcharstar_from_any");
    CORBA::Any from;
    from <<= initial_value();
    const CORBA::WChar *to;
    TEST(from >>= to);
    TEST(deep_copied(to, initial_value()));
}

void
Tester::assign_to_wcharstar()
{
    wcharstar_from_wcharstar();
    wcharstar_from_wstring_var();
    wcharstar_from_struct_wstring_member();
    wcharstar_from_exception_wstring_member();
    wcharstar_from_sequence_wstring_item();
    wcharstar_from_array_wstring_item();
    wcharstar_from_any();
}

//-----------------------------------------------------------------------------

void
Tester::const_wcharstar_from_wcharstar()
{
    ITTEST_INFO("const_wcharstar_from_wcharstar");
    CORBA::WChar *from = (CORBA::WChar*)initial_value();
    CORBA::WChar *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_wcharstar_from_const_wcharstar()
{
    ITTEST_INFO("const_wcharstar_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    const CORBA::WChar *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_wcharstar_from_union_wstring_member()
{
    ITTEST_INFO("const_wcharstar_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* foo1 = L"foo";
    from.wstring_member(foo1);
    from._d(0);
    from.wstring_member(initial_value());
    const CORBA::WChar *to = from.wstring_member();
    TEST(aliased(to, from.wstring_member()));
}

void
Tester::const_wcharstar_from_wstring_var()
{
    ITTEST_INFO("const_wcharstar_from_wstring_var");
    CORBA::WString_var from = initial_value();
    const CORBA::WChar *to = from;
    TEST(aliased(to, from));
}

void
Tester::const_wcharstar_from_struct_wstring_member()
{
    ITTEST_INFO("const_wcharstar_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    const CORBA::WChar *to = from.wstring_member;
    TEST(aliased(to, from.wstring_member));
}

void
Tester::const_wcharstar_from_exception_wstring_member()
{
    ITTEST_INFO("const_wcharstar_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    const CORBA::WChar *to = from.wstring_member;
    TEST(aliased(to, from.wstring_member));
}

void
Tester::const_wcharstar_from_sequence_wstring_item()
{
    ITTEST_INFO("const_wcharstar_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    const CORBA::WChar *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::const_wcharstar_from_array_wstring_item()
{
    ITTEST_INFO("const_wcharstar_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    const CORBA::WChar *to = from[0];
    TEST(aliased(to, from[0]));
}

void
Tester::assign_to_const_wcharstar()
{
    const_wcharstar_from_wcharstar();
    const_wcharstar_from_const_wcharstar();
    const_wcharstar_from_union_wstring_member();
    const_wcharstar_from_wstring_var();
    const_wcharstar_from_struct_wstring_member();
    const_wcharstar_from_exception_wstring_member();
    const_wcharstar_from_sequence_wstring_item();
    const_wcharstar_from_array_wstring_item();
}

//-----------------------------------------------------------------------------

void
Tester::wstring_var_from_wcharstar()
{
    ITTEST_INFO("wstring_var_from_wcharstar");
    // "to" adopts "from"
    {
	CORBA::WChar *from = CORBA::wstring_dup(initial_value());
	CORBA::WString_var to = from;
	TEST(aliased(to, from));
    }
    {
	CORBA::WChar *from = CORBA::wstring_dup(initial_value());
	CORBA::WString_var to;
	to = from;
	TEST(aliased(to, from));
    }
    {
	CORBA::WChar *from = CORBA::wstring_dup(initial_value());
	CORBA::WString_var to = from;
	const CORBA::WChar *addr1 = to;
	to = from;
	TEST(aliased(to, addr1));
    }
}

void
Tester::wstring_var_from_const_wcharstar()
{
    ITTEST_INFO("wstring_var_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    {
	CORBA::WString_var to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::WString_var to;
	to = from;
	TEST(deep_copied(to, from));
    }
}

void
Tester::wstring_var_from_union_wstring_member()
{
    ITTEST_INFO("wstring_var_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* bar1 = L"bar";
    from.wstring_member(bar1);
    from._d(0);
    from.wstring_member(initial_value());
    {
	CORBA::WString_var to = from.wstring_member();
	TEST(deep_copied(to, from.wstring_member()));
    }
    {
	CORBA::WString_var to;
	to = from.wstring_member();
	TEST(deep_copied(to, from.wstring_member()));
    }
}

void
Tester::wstring_var_from_wstring_var()
{
    ITTEST_INFO("wstring_var_from_wstring_var");
    CORBA::WString_var from = initial_value();
    {
	CORBA::WString_var to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::WString_var to;
	to = from;
	TEST(deep_copied(to, from));
    }
    {
	CORBA::WString_var to = from;
	const CORBA::WChar *addr1 = to;
	to = to;
	TEST(aliased(to, addr1));
    }
}

void
Tester::wstring_var_from_struct_wstring_member()
{
    ITTEST_INFO("wstring_var_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    {
	CORBA::WString_var to = from.wstring_member;
	TEST(deep_copied(to, from.wstring_member));
    }
    {
	CORBA::WString_var to;
	to = from.wstring_member;
	TEST(deep_copied(to, from.wstring_member));
    }
}

void
Tester::wstring_var_from_exception_wstring_member()
{
    ITTEST_INFO("wstring_var_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    {
	CORBA::WString_var to = from.wstring_member;
	TEST(deep_copied(to, from.wstring_member));
    }
    {
	CORBA::WString_var to;
	to = from.wstring_member;
	TEST(deep_copied(to, from.wstring_member));
    }
}

void
Tester::wstring_var_from_sequence_wstring_item()
{
    ITTEST_INFO("wstring_var_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    {
	CORBA::WString_var to = from[0];
	TEST(deep_copied(to, from[0]));
    }
    {
	CORBA::WString_var to;
	to = from[0];
	TEST(deep_copied(to, from[0]));
    }
}

void
Tester::wstring_var_from_array_wstring_item()
{
    ITTEST_INFO("wstring_var_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    {
	CORBA::WString_var to = from[0];
	TEST(deep_copied(to, from[0]));
    }
    {
	CORBA::WString_var to;
	to = from[0];
	TEST(deep_copied(to, from[0]));
    }
}

void
Tester::wstring_var_subscript()
{
    ITTEST_INFO("wstring_var_subscript");
    CORBA::WString_var s1 = CORBA::wstring_dup(initial_value());
    s1[0] = L'X';
    TEST(s1[0] == L'X');

    const CORBA::WString_var s2(s1);
    TEST(s2[0] == L'X');
    CORBA::WChar c = s2[0];
    TEST(c == L'X');
}

void
Tester::assign_to_wstring_var()
{
    wstring_var_from_wcharstar();
    wstring_var_from_const_wcharstar();
    wstring_var_from_union_wstring_member();
    wstring_var_from_wstring_var();
    wstring_var_from_struct_wstring_member();
    wstring_var_from_exception_wstring_member();
    wstring_var_from_sequence_wstring_item();
    wstring_var_from_array_wstring_item();
    wstring_var_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::struct_wstring_member_from_wcharstar()
{
    ITTEST_INFO("struct_wstring_member_from_wcharstar");
    CORBA::WChar *from = CORBA::wstring_dup(initial_value());
    WStringStruct to;
    to.wstring_member = from;
    TEST(aliased(to.wstring_member, from));
    const CORBA::WChar *addr1 = to.wstring_member;
    to.wstring_member = from;
    TEST(aliased(to.wstring_member, addr1));
}

void
Tester::struct_wstring_member_from_const_wcharstar()
{
    ITTEST_INFO("struct_wstring_member_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    WStringStruct to;
    to.wstring_member = from;
    TEST(deep_copied(to.wstring_member, from));
}

void
Tester::struct_wstring_member_from_union_wstring_member()
{
    ITTEST_INFO("struct_wstring_member_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* foo1 = L"foo";
    from.wstring_member(foo1);
    from._d(0);
    from.wstring_member(initial_value());
    WStringStruct to;
    to.wstring_member = from.wstring_member();
    TEST(deep_copied(to.wstring_member, from.wstring_member()));
}

void
Tester::struct_wstring_member_from_wstring_var()
{
    ITTEST_INFO("struct_wstring_member_from_wstring_var");
    CORBA::WString_var from = initial_value();
    WStringStruct to;
    to.wstring_member = from;
    TEST(deep_copied(to.wstring_member, from));
}

void
Tester::struct_wstring_member_from_struct_wstring_member()
{
    ITTEST_INFO("struct_wstring_member_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    WStringStruct to;
    to.wstring_member = from.wstring_member;
    TEST(deep_copied(to.wstring_member, from.wstring_member));
    const CORBA::WChar *addr1 = to.wstring_member;
    to.wstring_member = to.wstring_member;
    TEST(aliased(to.wstring_member, addr1));
}

void
Tester::struct_wstring_member_from_exception_wstring_member()
{
    ITTEST_INFO("struct_wstring_member_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    WStringStruct to;
    to.wstring_member = from.wstring_member;
    TEST(deep_copied(to.wstring_member, from.wstring_member));
}

void
Tester::struct_wstring_member_from_sequence_wstring_item()
{
    ITTEST_INFO("struct_wstring_member_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    WStringStruct to;
    to.wstring_member = from[0];
    TEST(deep_copied(to.wstring_member, from[0]));
}

void
Tester::struct_wstring_member_from_array_wstring_item()
{
    ITTEST_INFO("struct_wstring_member_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    WStringStruct to;
    to.wstring_member = from[0];
    TEST(deep_copied(to.wstring_member, from[0]));
}

void
Tester::struct_wstring_member_subscript()
{
    ITTEST_INFO("struct_wstring_member_subscript");
    WStringStruct s1;
    s1.wstring_member = CORBA::wstring_dup(initial_value());
    s1.wstring_member[0] = L'X';
    TEST(s1.wstring_member[0] == L'X');

    const WStringStruct s2(s1);
    TEST(s2.wstring_member[0] == L'X');
    CORBA::WChar c = s2.wstring_member[0];
    TEST(c == L'X');
}

void
Tester::assign_to_struct_wstring_member()
{
    struct_wstring_member_from_wcharstar();
    struct_wstring_member_from_const_wcharstar();
    struct_wstring_member_from_union_wstring_member();
    struct_wstring_member_from_wstring_var();
    struct_wstring_member_from_struct_wstring_member();
    struct_wstring_member_from_exception_wstring_member();
    struct_wstring_member_from_sequence_wstring_item();
    struct_wstring_member_from_array_wstring_item();
    struct_wstring_member_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::exception_wstring_member_from_wcharstar()
{
    ITTEST_INFO("exception_wstring_member_from_wcharstar");
    {
	CORBA::WChar *from = CORBA::wstring_dup(initial_value());
	WStringException to;
	to.wstring_member = from;
	TEST(aliased(to.wstring_member, from));
	const CORBA::WChar *addr1 = to.wstring_member;
	to.wstring_member = from;
	TEST(aliased(to.wstring_member, addr1));
    }
    {
	CORBA::WChar *from = CORBA::wstring_dup(initial_value());
	WStringException to(from); // doesn't adopt from
	TEST(deep_copied(to.wstring_member, from));
	CORBA::wstring_free(from);
    }
}

void
Tester::exception_wstring_member_from_const_wcharstar()
{
    ITTEST_INFO("exception_wstring_member_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    {
	WStringException to;
	to.wstring_member = from;
	TEST(deep_copied(to.wstring_member, from));
    }
    {
	WStringException to(from);
	TEST(deep_copied(to.wstring_member, from));
    }
}

void
Tester::exception_wstring_member_from_union_wstring_member()
{
    ITTEST_INFO("exception_wstring_member_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* bar1 = L"bar";
    from.wstring_member(bar1);
    from._d(0);
    from.wstring_member(initial_value());
    {
        WStringException to;
        to.wstring_member = from.wstring_member();
        TEST(deep_copied(to.wstring_member, from.wstring_member()));
    }
    {
        WStringException to(from.wstring_member());
        TEST(deep_copied(to.wstring_member, from.wstring_member()));
    }
}

void
Tester::exception_wstring_member_from_wstring_var()
{
    ITTEST_INFO("exception_wstring_member_from_wstring_var");
    CORBA::WString_var from = initial_value();
    {
	WStringException to;
	to.wstring_member = from;
	TEST(deep_copied(to.wstring_member, from));
    }
    {
	WStringException to(from);
	TEST(deep_copied(to.wstring_member, from));
    }
}

void
Tester::exception_wstring_member_from_struct_wstring_member()
{
    ITTEST_INFO("exception_wstring_member_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    {
	WStringException to;
	to.wstring_member = from.wstring_member;
	TEST(deep_copied(to.wstring_member, from.wstring_member));
    }
    {
	WStringException to(from.wstring_member);
	TEST(deep_copied(to.wstring_member, from.wstring_member));
    }
}

void
Tester::exception_wstring_member_from_exception_wstring_member()
{
    ITTEST_INFO("exception_wstring_member_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    {
	WStringException to;
	to.wstring_member = from.wstring_member;
	TEST(deep_copied(to.wstring_member, from.wstring_member));
	const CORBA::WChar *addr1 = to.wstring_member;
	to.wstring_member = to.wstring_member;
	TEST(aliased(to.wstring_member, addr1));
    }
    {
	WStringException to(from.wstring_member);
	TEST(deep_copied(to.wstring_member, from.wstring_member));
    }
}

void
Tester::exception_wstring_member_from_sequence_wstring_item()
{
    ITTEST_INFO("exception_wstring_member_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    {
        WStringException to;
        to.wstring_member = from[0];
        TEST(deep_copied(to.wstring_member, from[0]));
    }
    {
        WStringException to(from[0]);
        TEST(deep_copied(to.wstring_member, from[0]));
    }
}

void
Tester::exception_wstring_member_from_array_wstring_item()
{
    ITTEST_INFO("exception_wstring_member_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    {
        WStringException to;
        to.wstring_member = from[0];
        TEST(deep_copied(to.wstring_member, from[0]));
    }
    {
        WStringException to(from[0]);
        TEST(deep_copied(to.wstring_member, from[0]));
    }
}

void
Tester::assign_to_exception_wstring_member()
{
    exception_wstring_member_from_wcharstar();
    exception_wstring_member_from_const_wcharstar();
    exception_wstring_member_from_union_wstring_member();
    exception_wstring_member_from_wstring_var();
    exception_wstring_member_from_struct_wstring_member();
    exception_wstring_member_from_exception_wstring_member();
    exception_wstring_member_from_sequence_wstring_item();
    exception_wstring_member_from_array_wstring_item();
}

//-----------------------------------------------------------------------------

void
Tester::union_wstring_member_from_wcharstar()
{
    ITTEST_INFO("union_wstring_member_from_wcharstar");
    CORBA::WChar *from = CORBA::wstring_dup(initial_value());
    WStringUnion to;
    to.wstring_member(from);
    TEST(aliased(to.wstring_member(), from));
    const CORBA::WChar *addr1 = to.wstring_member();
    to.wstring_member(from);
    TEST(aliased(to.wstring_member(), addr1));
}

void
Tester::union_wstring_member_from_const_wcharstar()
{
    ITTEST_INFO("union_wstring_member_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    WStringUnion to;
    to.wstring_member(from);
    TEST(deep_copied(to.wstring_member(), from));
}

void
Tester::union_wstring_member_from_union_wstring_member()
{
    ITTEST_INFO("union_wstring_member_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* foo1 = L"foo";
    from.wstring_member(foo1);
    from._d(0);
    from.wstring_member(initial_value());
    WStringUnion to;
    to.wstring_member(from.wstring_member());
    TEST(deep_copied(to.wstring_member(), from.wstring_member()));
    const CORBA::WChar *addr1 = to.wstring_member();
    to.wstring_member(to.wstring_member());
    TEST(aliased(to.wstring_member(), addr1));
}

void
Tester::union_wstring_member_from_wstring_var()
{
    ITTEST_INFO("union_wstring_member_from_wstring_var");
    CORBA::WString_var from = initial_value();
    WStringUnion to;
    to.wstring_member(from);
    TEST(deep_copied(to.wstring_member(), from));
}

void
Tester::union_wstring_member_from_struct_wstring_member()
{
    ITTEST_INFO("union_wstring_member_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    WStringUnion to;
    to.wstring_member(from.wstring_member.in());
    TEST(deep_copied(to.wstring_member(), from.wstring_member));
}

void
Tester::union_wstring_member_from_exception_wstring_member()
{
    ITTEST_INFO("union_wstring_member_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    WStringUnion to;
    to.wstring_member(from.wstring_member.in());
    TEST(deep_copied(to.wstring_member(), from.wstring_member));
}

void
Tester::union_wstring_member_from_sequence_wstring_item()
{
    ITTEST_INFO("union_wstring_member_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    WStringUnion to;
    to.wstring_member(from[0].in());
    TEST(deep_copied(to.wstring_member(), from[0]));
}

void
Tester::union_wstring_member_from_array_wstring_item()
{
    ITTEST_INFO("union_wstring_member_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    WStringUnion to;
    to.wstring_member(from[0].in());
    TEST(deep_copied(to.wstring_member(), from[0]));
}

void
Tester::assign_to_union_wstring_member()
{
    union_wstring_member_from_wcharstar();
    union_wstring_member_from_const_wcharstar();
    union_wstring_member_from_union_wstring_member();
    union_wstring_member_from_wstring_var();
    union_wstring_member_from_struct_wstring_member();
    union_wstring_member_from_exception_wstring_member();
    union_wstring_member_from_sequence_wstring_item();
    union_wstring_member_from_array_wstring_item();
}

//-----------------------------------------------------------------------------

void
Tester::sequence_wstring_item_from_wcharstar()
{
    ITTEST_INFO("sequence_wstring_item_from_wcharstar");
    CORBA::WChar *from = CORBA::wstring_dup(initial_value());
    WStringSequence to;
    to.length(1);
    to[0] = from;
    TEST(aliased(to[0], from));
    const CORBA::WChar *addr1 = to[0];
    to[0] = from;
    TEST(aliased(to[0], addr1));
}

void
Tester::sequence_wstring_item_from_const_wcharstar()
{
    ITTEST_INFO("sequence_wstring_item_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::sequence_wstring_item_from_union_wstring_member()
{
    ITTEST_INFO("sequence_wstring_item_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* bar1 = L"bar";
    from.wstring_member(bar1);
    from._d(0);
    from.wstring_member(initial_value());
    WStringSequence to;
    to.length(1);
    to[0] = from.wstring_member();
    TEST(deep_copied(to[0], from.wstring_member()));
}

void
Tester::sequence_wstring_item_from_wstring_var()
{
    ITTEST_INFO("sequence_wstring_item_from_wstring_var");
    CORBA::WString_var from = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::sequence_wstring_item_from_struct_wstring_member()
{
    ITTEST_INFO("sequence_wstring_item_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from.wstring_member;
    TEST(deep_copied(to[0], from.wstring_member));
}

void
Tester::sequence_wstring_item_from_exception_wstring_member()
{
    ITTEST_INFO("sequence_wstring_item_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from.wstring_member;
    TEST(deep_copied(to[0], from.wstring_member));
}

void
Tester::sequence_wstring_item_from_sequence_wstring_item()
{
    ITTEST_INFO("sequence_wstring_item_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
    const CORBA::WChar *addr1 = to[0];
    to[0] = to[0];
    TEST(aliased(to[0], addr1));
}

void
Tester::sequence_wstring_item_from_array_wstring_item()
{
    ITTEST_INFO("sequence_wstring_item_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    WStringSequence to;
    to.length(1);
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
}

void
Tester::sequence_wstring_item_subscript()
{
    ITTEST_INFO("sequence_wstring_item_subscript");
    WStringSequence s1(1);
    s1.length(1);
    s1[0] = CORBA::wstring_dup(initial_value());
    s1[0][0] = L'X';
    TEST(s1[0][0] == L'X');

    const WStringSequence s2(s1);
    TEST(s2[0][0] == L'X');
    CORBA::WChar c = s2[0][0];
    TEST(c == L'X');
}

void
Tester::assign_to_sequence_wstring_item()
{
    sequence_wstring_item_from_wcharstar();
    sequence_wstring_item_from_const_wcharstar();
    sequence_wstring_item_from_union_wstring_member();
    sequence_wstring_item_from_wstring_var();
    sequence_wstring_item_from_struct_wstring_member();
    sequence_wstring_item_from_exception_wstring_member();
    sequence_wstring_item_from_sequence_wstring_item();
    sequence_wstring_item_from_array_wstring_item();
    sequence_wstring_item_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::array_wstring_item_from_wcharstar()
{
    ITTEST_INFO("array_wstring_item_from_wcharstar");
    CORBA::WChar *from = CORBA::wstring_dup(initial_value());
    WStringArray to;
    to[0] = from;
    TEST(aliased(to[0], from));
    const CORBA::WChar *addr1 = to[0];
    to[0] = from;
    TEST(aliased(to[0], addr1));
}

void
Tester::array_wstring_item_from_const_wcharstar()
{
    ITTEST_INFO("array_wstring_item_from_const_wcharstar");
    const CORBA::WChar *from = initial_value();
    WStringArray to;
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::array_wstring_item_from_union_wstring_member()
{
    ITTEST_INFO("array_wstring_item_from_union_wstring_member");
    WStringUnion from;
    const CORBA::WChar* foo1 = L"foo";
    from.wstring_member(foo1);
    from._d(0);
    from.wstring_member(initial_value());
    WStringArray to;
    to[0] = from.wstring_member();
    TEST(deep_copied(to[0], from.wstring_member()));
}

void
Tester::array_wstring_item_from_wstring_var()
{
    ITTEST_INFO("array_wstring_item_from_wstring_var");
    CORBA::WString_var from = initial_value();
    WStringArray to;
    to[0] = from;
    TEST(deep_copied(to[0], from));
}

void
Tester::array_wstring_item_from_struct_wstring_member()
{
    ITTEST_INFO("array_wstring_item_from_struct_wstring_member");
    WStringStruct from;
    from.wstring_member = initial_value();
    WStringArray to;
    to[0] = from.wstring_member;
    TEST(deep_copied(to[0], from.wstring_member));
}

void
Tester::array_wstring_item_from_exception_wstring_member()
{
    ITTEST_INFO("array_wstring_item_from_exception_wstring_member");
    WStringException from;
    from.wstring_member = initial_value();
    WStringArray to;
    to[0] = from.wstring_member;
    TEST(deep_copied(to[0], from.wstring_member));
}

void
Tester::array_wstring_item_from_sequence_wstring_item()
{
    ITTEST_INFO("array_wstring_item_from_sequence_wstring_item");
    WStringSequence from;
    from.length(1);
    from[0] = initial_value();
    WStringArray to;
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
}

void
Tester::array_wstring_item_from_array_wstring_item()
{
    ITTEST_INFO("array_wstring_item_from_array_wstring_item");
    WStringArray from;
    from[0] = initial_value();
    WStringArray to;
    to[0] = from[0];
    TEST(deep_copied(to[0], from[0]));
    const CORBA::WChar *addr1 = to[0];
    to[0] = to[0];
    TEST(aliased(to[0], addr1));
}

void
Tester::array_wstring_item_subscript()
{
    ITTEST_INFO("array_wstring_item_subscript");
    WStringArray s1;
    s1[0] = CORBA::wstring_dup(initial_value());
    s1[0][0] = L'X';
    TEST(s1[0][0] == L'X');
}

void
Tester::assign_to_array_wstring_item()
{
    array_wstring_item_from_wcharstar();
    array_wstring_item_from_const_wcharstar();
    array_wstring_item_from_union_wstring_member();
    array_wstring_item_from_wstring_var();
    array_wstring_item_from_struct_wstring_member();
    array_wstring_item_from_exception_wstring_member();
    array_wstring_item_from_sequence_wstring_item();
    array_wstring_item_from_array_wstring_item();
    array_wstring_item_subscript();
}

//-----------------------------------------------------------------------------

void
Tester::arglist_from_wcharstar()
{
    ITTEST_INFO("arglist_from_wcharstar");

    const CORBA::WChar* in_string = in_value();
    CORBA::WChar* inout_string = CORBA::wstring_dup(initial_inout_value());
    CORBA::WChar  * out_string;

    CORBA::WChar *return_string =
	impl_oref()->op(in_string, inout_string, out_string);
    TEST(deep_copied(in_string,  inout_string));
    TEST(deep_copied(in_string,    out_string));
    TEST(deep_copied(in_string, return_string));

    CORBA::wstring_free(  inout_string );
    CORBA::wstring_free(    out_string );
    CORBA::wstring_free( return_string );
}

void
Tester::arglist_from_wstring_var()
{
    ITTEST_INFO("arglist_from_wstring_var");

    CORBA::WString_var    in_string = in_value();
    CORBA::WString_var inout_string = initial_inout_value();
    CORBA::WString_var   out_string;

    CORBA::WString_var return_string =
	impl_oref()->op(in_string, inout_string, out_string);
    TEST(deep_copied(in_string,  inout_string));
    TEST(deep_copied(in_string,    out_string));
    TEST(deep_copied(in_string, return_string));
}

void
Tester::arglist_from_struct_wstring_member()
{
    ITTEST_INFO("arglist_from_struct_wstring_member");

    WStringStruct     in_struct;
    WStringStruct  inout_struct;
    WStringStruct    out_struct;
    WStringStruct return_struct;

       in_struct.wstring_member = in_value();
    inout_struct.wstring_member = initial_inout_value();

    return_struct.wstring_member =
	impl_oref()->op(
	       in_struct.wstring_member,
	    inout_struct.wstring_member,
	      out_struct.wstring_member
	);
    TEST(deep_copied(in_struct.wstring_member,  inout_struct.wstring_member));
    TEST(deep_copied(in_struct.wstring_member,    out_struct.wstring_member));
    TEST(deep_copied(in_struct.wstring_member, return_struct.wstring_member));
}

void
Tester::arglist_from_exception_wstring_member()
{
    ITTEST_INFO("arglist_from_exception_wstring_member");

    WStringException     in_exception;
    WStringException  inout_exception;
    WStringException    out_exception;
    WStringException return_exception;

       in_exception.wstring_member = in_value();
    inout_exception.wstring_member = initial_inout_value();

    return_exception.wstring_member =
	impl_oref()->op(
	       in_exception.wstring_member,
	    inout_exception.wstring_member,
	      out_exception.wstring_member
	);
    TEST(deep_copied(in_exception.wstring_member,  inout_exception.wstring_member));
    TEST(deep_copied(in_exception.wstring_member,    out_exception.wstring_member));
    TEST(deep_copied(in_exception.wstring_member, return_exception.wstring_member));
}

void
Tester::arglist_from_union_wstring_member()
{
    ITTEST_INFO("arglist_from_union_wstring_member");

    WStringUnion          in_union;
    CORBA::WString_var inout_var = initial_inout_value();
    CORBA::WString_var   out_var;

    in_union.wstring_member(in_value());

    CORBA::WString_var return_var =
	impl_oref()->op(
	       in_union.wstring_member(),
	    inout_var,
	      out_var
	);
    TEST(deep_copied(in_union.wstring_member(), inout_var));
}

void
Tester::arglist_from_array_wstring_item()
{
    ITTEST_INFO("arglist_from_array_wstring_item");

    WStringArray     in_array;
    WStringArray  inout_array;
    WStringArray    out_array;
    WStringArray return_array;

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
Tester::arglist_from_sequence_wstring_item()
{
    ITTEST_INFO("arglist_from_sequence_wstring_item");

    WStringSequence     in_sequence;
    WStringSequence  inout_sequence;
    WStringSequence    out_sequence;
    WStringSequence return_sequence;

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
    arglist_from_wcharstar();
    arglist_from_wstring_var();
    arglist_from_struct_wstring_member(); 
    arglist_from_exception_wstring_member();
    arglist_from_union_wstring_member();
    arglist_from_array_wstring_item();
    arglist_from_sequence_wstring_item();
}

//-----------------------------------------------------------------------------

void
Tester::run()
{
    assign_to_wcharstar();
    assign_to_const_wcharstar();
    assign_to_wstring_var();
    assign_to_struct_wstring_member();
    assign_to_exception_wstring_member();
    assign_to_union_wstring_member();
    assign_to_sequence_wstring_item();
    assign_to_array_wstring_item();
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
        WStringTest_var server = i._this();

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
