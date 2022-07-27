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

#include <TestIntfContext_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfContext_impl::TestIntfContext_impl()
{
}

ORBTest_Context::StringSequence*
TestIntfContext_impl::opContext(
    const char* pattern,
    Context_ptr ctx
)
{
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
    volatile int dummy = 1;
    dummy++;
#endif

    NVList_var values;
    ctx->get_values("", 0, pattern, values.out());

    ULong len = values->count();
    ORBTest_Context::StringSequence* result = (
        new ORBTest_Context::StringSequence(len * 2)
    );
    result->length(len * 2);
    for (ULong i = 0; i < len; i++)
    {
	const char* s;
	*(values->item(i)->value()) >>= s;
	(*result)[i * 2] = values->item(i)->name();
	(*result)[i * 2 + 1] = string_dup(s);
    }

    return result;
}
