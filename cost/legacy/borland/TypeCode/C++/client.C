#include "TypeCode_c.hh"
USE_STD_NS

extern CORBA::Any Init(CORBA::TypeCode* typeCode);
extern CORBA::Boolean Equal(CORBA::Any& lhs, CORBA::Any& rhs);

void testTypeCode(TypeCodeTest::Echo_ptr echo, CORBA::TypeCode_ptr tc) {

    cout << "Testing TypeCode: " << tc->name() << endl << flush;

    cout << "SENDING" << endl << flush;
    CORBA::TypeCode_var	echo_tc = echo->echo_tc(tc);
    cout << "CHECKING..." << endl << flush;

    //cout << "Got back: " << echo_tc << endl;
    if (!tc->equal(echo_tc))
      cout << "Unequal TypeCodes:	" << tc	<< " and " << echo_tc << endl;
    else
      cout << "OK" << endl;
}

void testAny(TypeCodeTest::Echo_ptr echo, CORBA::TypeCode_ptr tc) {

    cout << "Testing Any with tc name: " << tc->name() << endl << flush;

    CORBA::Any a = Init(tc);

    cout << "SENDING" << endl << flush;
    CORBA::Any_var echo_any(echo->echo_any(a));
    cout << "CHECKING..." << endl << flush;

    //cout << "Got back: " << echo_any << endl;
    if (! Equal(a, echo_any))
      cout << "Unequal Any's:	" << a	<< " and " << echo_any.in()
	   << endl;
    else
      cout << "OK" << endl;
}

int main(int argc, char	**argv)
{
   try {
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

	cout <<	"Binding" << endl;
	TypeCodeTest::Echo_var echoer =	TypeCodeTest::Echo::_bind();

	cout <<	"Testing typecodes" << endl;
	testTypeCode(echoer, TypeCodeTest::_tc_SimpleStructTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_IndirectionStructTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_RecursiveStructTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_StructTestList_get());
	testTypeCode(echoer, TypeCodeTest::_tc_ComplexStructTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_SimpleUnionTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_DefaultUnionTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_EnumTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_ComplexUnionTest_get());
	testTypeCode(echoer, TypeCodeTest::_tc_ComplexUnionTestList_get());
	testTypeCode(echoer, TypeCodeTest::_tc_Echo_get());

	cout << "Testing anys" << endl;
	testAny(echoer, TypeCodeTest::_tc_SimpleStructTest_get());
	testAny(echoer, TypeCodeTest::_tc_IndirectionStructTest_get());
	testAny(echoer, TypeCodeTest::_tc_RecursiveStructTest_get());
	testAny(echoer, TypeCodeTest::_tc_StructTestList_get());
	testAny(echoer, TypeCodeTest::_tc_ComplexStructTest_get());
	testAny(echoer, TypeCodeTest::_tc_SimpleUnionTest_get());
	testAny(echoer, TypeCodeTest::_tc_DefaultUnionTest_get());
	testAny(echoer, TypeCodeTest::_tc_EnumTest_get());
	testAny(echoer, TypeCodeTest::_tc_ComplexUnionTest_get());
	testAny(echoer, TypeCodeTest::_tc_ComplexUnionTestList_get());
	testAny(echoer, TypeCodeTest::_tc_Echo_get());
   }
   catch (CORBA::SystemException& se) {
	cerr <<	se << endl;
	return 1;
   }
   catch (CORBA::Exception& ee)	{
	cerr <<	ee << endl;
	return 1;
   }

   return 0;
}
