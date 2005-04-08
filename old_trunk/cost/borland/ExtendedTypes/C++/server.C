#include <wchar.h>
#include "extest_s.hh"
USE_STD_NS


class TestImpl : public _sk_extest::_sk_Test {
    
private:
  CORBA::WChar c;
  CORBA::WChar *str;	
        
public:
  TestImpl ();
  TestImpl ( const char * object_name = (const char *) NULL):
      _sk_extest::_sk_Test (object_name) { 
    str = CORBA::wstring_alloc(100);
  }

  CORBA::WChar t_wc() {
    cout << "In method t_wc" << endl;
    c = L'A';
    return c;
  }
	
  CORBA::WChar* t_wstr () {
    cout << "In method t_wstr" << endl;
    str = CORBA::wstring_alloc(100);
    wcscpy(str,L"This is my special string");
    return str ;
  }

  CORBA::LongLong t_ll() {
    cout << "In method t_lll" << endl;
    #ifdef LINUX
	return  -2147483648LL;
    #else

    return -2147483648;
	
    #endif
  }
	
  CORBA::ULongLong t_ull() {
    cout << "In method t_tull" << endl;
    #ifdef LINUX
    return 4294967295LL;
    #else
    return 4294967295;
    #endif
  }

#if !defined(NO_LONG_DOUBLE)
  CORBA::LongDouble t_ld() {
    cout << "In method t_ld" << endl;
    return 4294967295787987e0;
  }
#endif

  void t_wc_inout (CORBA::WChar& twc) {
    cout << "In method t_wc_inout" << endl;
    if (twc == L'A')
      twc = L'X';
    else 
      twc =L'Z';
  }

  void t_wstr_inout (CORBA::WChar*& twstr) {
    cout << "In method t_wstr_inout" << endl;
    int i;
    i= wcslen(twstr);
    CORBA::WChar *s;
    s=CORBA::wstring_alloc(100);
    #ifdef LINUX 
	if ( wcscmp (twstr,L"This is my special string")==0)
      wcscpy (s, L"hello to you");
    else 
      wcscpy (s, L"bad string ");
    CORBA::wstring_free(twstr);
    twstr=s;
   #else
if ( wcscmp (twstr,L"This is my special string")==NULL)
      wcscpy (s, L"hello to you");
    else 
      wcscpy (s, L"bad string ");
    CORBA::wstring_free(twstr);
    twstr=s;
   #endif
  }

  void t_ll_inout ( CORBA::LongLong& tll) {
    cout << "In method t_ll_inout" << endl;
    #ifdef LINUX
    if (tll == -2147483648LL)
      tll = 2147483647;
    else 
      tll = 123;
    #else
    if (tll == -2147483648)
      tll = 2147483647;
    else 
      tll = 123;
    #endif
  }

  void t_ull_inout ( CORBA::ULongLong& tull) {
    cout << "In method t_tull_inout" << endl;
 
    #ifdef LINUX
	if (tull == 4294967295ULL)
      tull = 0;
    else
      tull = 123;
   #else
    if (tull == 4294967295)
      tull = 0;
    else 
      tull = 123;
  #endif
  }

#if !defined(NO_LONG_DOUBLE)
  void t_ld_inout ( CORBA::LongDouble& tld) {
    cout << "In method t_ld_inout" << endl;
    cout << tld << endl;
    if (tld == 42949672957879e0)
      tld = 0;
    else 
      tld = 123;
    cout << tld << endl;
  }
#endif

  extest::Test::extS *t_struct() {
    cout << "In method t_struct" << endl;
    extest::Test::extS  *p = new extest::Test::extS();
    int i;
	   
    p->twc = L'c';
    p->twstr=CORBA::wstring_alloc(100);
    wcscpy(p->twstr,L"Hello World");
    p->tll = 876543;
    p->tull = 429497295;
#if !defined(NO_LONG_DOUBLE)
    p->tld =  4294967295787987e0;   
#endif

    return (p);
  }

  void t_struct_inout (extest::Test::extS& teststruct) {
    cout << "In method t_struct_inout" << endl;
    CORBA::WString_var x;
    x=CORBA::wstring_alloc(100);
    cout << teststruct.twc << " " << wcslen(teststruct.twstr);
    cout << " " << teststruct.twstr << " " << teststruct.tll;
    cout << " " << teststruct.tull;
#if !defined(NO_LONG_DOUBLE)
    cout << " " << teststruct.tld << endl;
#endif
    if (teststruct.twc == L'c')
      teststruct.twc = L'x';
    else 
      teststruct.twc = L'y';
   #ifdef LINUX
	if (wcscmp(teststruct.twstr, L"Hello World") ==0) {
      wcscpy(x, L"My Hello to you too");
    }
 	else {
      wcscpy(x, L"Bad String");
    }
   #else
 
	if (wcscmp(teststruct.twstr, L"Hello World") ==NULL) {
      wcscpy(x, L"My Hello to you too");
    }
    else {
      wcscpy(x, L"Bad String");
    }

   #endif

    if (teststruct.tll == 876543)
      teststruct.tll = 0;
    else teststruct.tll = 123;
    if (teststruct.tull == 429497295 )
      teststruct.tull = 42949729;
    else teststruct.tull =123;
#if !defined(NO_LONG_DOUBLE)
    if (teststruct.tld ==  4294967295787987e0)
      teststruct.tld =  0;
    else teststruct.tld=123;
#endif

    teststruct.twstr=x; 
    cout << teststruct.twc << " " << wcslen(teststruct.twstr);
    cout << " " << x << " " << teststruct.tll << " " << teststruct.tull;
#if !defined(NO_LONG_DOUBLE)
    cout << " " << teststruct.tld << endl;	 
#endif 
  }

  void t_extTS_inout (extest::Test::extTestSeq& testseq) {
    CORBA::ULong i=0;
    cout << "In method t_extS_inout" << endl;
    CORBA::ULong l = testseq.length();
    CORBA::WString_var a[100];
		
    if (l>0) {
      for (i=0;i<l;i++) {
        if (testseq[i].twc == L'x')
          testseq[i].twc = L'c';
        else 
          testseq[i].twc = L'y';
        a[i]=CORBA::wstring_alloc(100);
	#ifdef LINUX
        if (wcscmp(testseq[i].twstr,L"My Hello to you too") == 0) 
          wcscpy(a[i] ,  L"Hello World");
        else
          wcscpy(a[i],  L"Bad String");
	#else

        if (wcscmp(testseq[i].twstr,L"My Hello to you too") == NULL) 
          wcscpy(a[i] ,  L"Hello World");
        else
          wcscpy(a[i],  L"Bad String");

	#endif

        if (testseq[i].tll == 0)
          testseq[i].tll = 564;
        else 
          testseq[i].tll = 123;
        if (testseq[i].tull == 42949729 )
          testseq[i].tull = 729;
        else 
          testseq[i].tull =123;
#if !defined(NO_LONG_DOUBLE)
        if (testseq[i].tld == 0)
          testseq[i].tld = 4294967295;
        else 
          testseq[i].tld = 123;
#endif
        testseq[i].twstr=a[i];			 
      } //end for 			
    } //end if
  } 
};

int main(int argc, char* const* argv)
{
  try {
    // Initialize the ORB and BOA
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::BOA_var boa = orb->BOA_init(argc, argv);

    // Create a new Test object.
    cout << "Instantiating a TestImpl" << endl;
    TestImpl b ("VISIGENIC");

    // Export the newly created object.
    boa->obj_is_ready(&b);
    cout << "Test object is ready." << endl;

    // Wait for incoming requests
    boa->impl_is_ready();

  } catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return(1);
  }
  return(0);
}
