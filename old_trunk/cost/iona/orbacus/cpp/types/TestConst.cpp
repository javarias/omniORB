// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <stdio.h>

#include <includes.h>

#include <Constants.h>

using namespace CORBA;

int
wcscmp(const wchar_t* ws1, const wchar_t* ws2)
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

void
TestConst()
{
    {
        const Measurement constEnum = FURLONGS;
	CHECK(constEnum == ConstEnum);

	const Long constLong = -1234;
	CHECK(constLong == ConstLong);
	
	const LongLong constLongLong = constLong * constLong;
	CHECK(constLongLong == ConstLongLong);
	
	const LongLong constLongLongMin = -9223372036854775808LL;
	CHECK(constLongLongMin == ConstLongLongMin);

	const LongLong constLongLongMax = 9223372036854775807LL;
	CHECK(constLongLongMax == ConstLongLongMax);
	
	const ULong constULong = (5432 + constLong ) * 100 % 124;
	CHECK(constULong == ConstULong);
	
	const ULongLong constULongLong = constULong + (constULong << 20);
	CHECK(constULongLong == ConstULongLong);
	
	const ULongLong constULongLongMax = 18446744073709551615ULL;
	CHECK(constULongLongMax == ConstULongLongMax);
	
	const Double constDouble = 50.23 - 1532.718 * 0.029;
	CHECK(constDouble == ConstDouble);
	
	const Float constFloat = (Float)(constDouble * 1.11);
	CHECK(constFloat == ConstFloat);
	
	const Short constShort = -23 % 10;
	CHECK(constShort == ConstShort);
	
	const Boolean constBoolean = true;
	CHECK(constBoolean == ConstBoolean);
	
	const Octet constOctet = 254;
	CHECK(constOctet == ConstOctet);

#ifndef HAVE_NO_FIXED_TYPE
        const Fixed constFixed = Fixed("275.189") * Fixed("1.163") +
                                 Fixed("48.0093");
        CHECK(constFixed == ConstFixed);
#endif

	const char* constString = "This is ConstString";
	CHECK(strcmp(constString, ConstString) == 0);

	const char* constEmptyString = "";
	CHECK(strcmp(constEmptyString, ConstEmptyString) == 0);

	const wchar_t* constWString = L"This is ConstWString";
	CHECK(wcscmp(constWString, ConstWString) == 0);

	const wchar_t* constEmptyWString = L"";
	CHECK(wcscmp(constEmptyWString, ConstEmptyWString) == 0);
    }

    {
	const Long constLong = 12345678;
	CHECK(constLong == TestConstModule::ConstLong);
	
	const Double constDouble = ConstDouble / 2.0;
	CHECK(constDouble == TestConstModule::ConstDouble);
	
	const Boolean constBoolean = false;
	CHECK(constBoolean == TestConstModule::ConstBoolean);
	
	const char* constString = "This is ConstString in a module";
	CHECK(strcmp(constString, TestConstModule::ConstString) == 0);

	const wchar_t* constWString = L"This is ConstWString in a module";
	CHECK(wcscmp(constWString, TestConstModule::ConstWString) == 0);

        const Measurement constEnum = METERS;
	CHECK(constEnum == TestConstModule::ConstEnum);
    }

    {
	const Long constLong = (0xf | 0xf000) & 0xfffe;
	CHECK(constLong == TestConstInterface::ConstLong);
	
	const ULong constULong = 0xFFFFFFFFU;
	CHECK(constULong == TestConstInterface::ConstULong);
	
	const Char constChar0 = (Char)0;
	CHECK(constChar0 == TestConstInterface::ConstChar0);
	
	const Char constChar1 = (Char)'c';
	CHECK(constChar1 == TestConstInterface::ConstChar1);
	
	const Char constChar2 = (Char)'\n';
	CHECK(constChar2 == TestConstInterface::ConstChar2);
	
	const Char constChar3 = (Char)'\377';
	CHECK(constChar3 == TestConstInterface::ConstChar3);
	
	const Char constChar4 = (Char)'\x88';
	CHECK(constChar4 == TestConstInterface::ConstChar4);
	
	const Char constChar5 = (Char)'\'';
	CHECK(constChar5 == TestConstInterface::ConstChar5);
	
	const WChar constWChar = L'Z';
	CHECK(constWChar == TestConstInterface::ConstWChar);

	const char* constString = "\n\t\v\b\r\f\a\\\?\'\"\xff\377\7";
	CHECK(strcmp(constString, TestConstInterface::ConstString) == 0);

	const wchar_t* constWString = L"\n\t\v\b\r\f\a\\\?\'\"\xff\377\7";
	CHECK(wcscmp(constWString, TestConstInterface::ConstWString) == 0);

        const Measurement constEnum = FEET;
	CHECK(constEnum == TestConstInterface::ConstEnum);
    }
}


int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;

    try
    {
	//
	// Run the test
	//
        cout << "Testing constants... " << flush;
        TestConst();
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        OB_ERROR(ex);
#endif
        status = EXIT_FAILURE;
    }

    return status;
}
