// This is the client code to invoke the 
// methods implemented.
#if !defined(_VIS_STD)
#include <fstream.h>
#endif
#include <wchar.h>
#include "extest_c.hh"
USE_STD_NS


class TESTM : public extest::Test_var {
private:
public :
  void test_wchar (extest::Test_var& testobj) {
    // invoke the t_wc method
    cout << "Testing wchar....";
    CORBA::WChar c;
    try{
      c=testobj->t_wc();
      if ( c == L'A')
        cout << "passed"<< endl;
      else 
        cout << "failed" <<endl;
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;	
    }
  }

  void test_wstring (extest::Test_var& testobj) {
    // invoke the t_wstr method
    try{
      cout << "Testing wstring....";
      CORBA::WString_var s,tmp;
      s=CORBA::wstring_alloc(100);
      tmp = testobj->t_wstr();
      wcscpy(s,tmp);
      #ifdef LINUX
      if (wcscmp(s,L"This is my special string")==0)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

     #else
      if (wcscmp(s,L"This is my special string")==NULL)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;
     #endif
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }		
  }

  void test_longlong(extest::Test_var& testobj) {
    // invoke the t_ll_inout method
    try{
      cout << "Testing longlong....";
      CORBA::LongLong l;
      l=testobj->t_ll();
	#ifdef LINUX
      if (l==-2147483648LL)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;
 
      #else
      if (l==-2147483648)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

     #endif
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }        
  }

#if !defined(NO_LONG_DOUBLE)
  void test_longdouble(Test_var& testobj) {
    // invoke the t_ld method
    try{    
      cout << "Testing longdouble....";
      CORBA::LongDouble ld;
      ld=testobj->t_ld();
	
	#ifdef LINUX

      if (ld==4294967295787987ULL)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;
	#else

      if (ld==4294967295787987)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

	#endif
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }  
  }
#endif

  void test_ulonglong(extest::Test_var& testobj) {
    // invoke the t_ull method
    try{
      cout << "Testing ulonglong....";
      CORBA::ULongLong u;
      u=testobj->t_ull();

      #ifdef LINUX

      if (u== 4294967295ULL)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

	#else
      if (u== 4294967295)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

	#endif
    } 
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }
  }

  void test_struct (extest::Test_var& testobj) {
    //invoke method t_struct
    try{
      cout << "Testing struct....";
      extest::Test::extS_var st;
     
      st= testobj->t_struct();

      #ifdef LINUX
      if ((st->twc == L'c')
	  && (wcscmp(st->twstr,L"Hello World")==0)
	  && (st->tll==876543)
	  && (st->tull==429497295)

      #else

      if ((st->twc == L'c')
	  && (wcscmp(st->twstr,L"Hello World")==NULL)
	  && (st->tll==876543)
      
	#endif

#if !defined(NO_LONG_DOUBLE)
          && (st->tld== 4294967295787987))
#else
	)
#endif
        cout << "passed" << endl;
      else 
        cout << "failed" <<endl;
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }	 
  }

  void test_extS(extest::Test_var& testobj) {
    //invoke method t_extTS_inout
    cout << "Testing sequence ......."; 
    CORBA::ULong i , n=10;
  			 
    extest::Test::extTestSeq_var b = new extest::Test::extTestSeq();
    b->length(n);
    try{
      for (i=0;i<n;i++){
        b[i].twc=L'x';
        b[i].twstr=CORBA::wstring_alloc(100);
        wcscpy(b[i].twstr,L"My Hello to you too");
        b[i].tll=0;
        b[i].tull = 42949729;
#if !defined(NO_LONG_DOUBLE)
        b[i].tld=0;
#endif
      }
      testobj->t_extTS_inout(b);

	#ifdef LINUX

      for (i=0; i<n;i++){
        if ((b[i].twc == L'c')
	    && (wcscmp(b[i].twstr,L"Hello World")==0)
	    && (b[i].tll==564) 
	    && (b[i].tull==729)

	#else
      for (i=0; i<n;i++){
        if ((b[i].twc == L'c')
	    && (wcscmp(b[i].twstr,L"Hello World")==NULL)
	    && (b[i].tll==564) 
	    && (b[i].tull==729)

	#endif

#if !defined(NO_LONG_DOUBLE)
	    &&(b[i].tld==4294967295))
#else
	   )
#endif
          cout <<i<< " passed " << endl;
        else 
          cout<<i<< " failed" <<endl;
        // CORBA::wstring_free (b[i].twstr);
      }
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }  
  }

  void test_wchar_inout (extest::Test_var& testobj) {
    // invoke method t_wc_inout
    try{
      cout << "Testing wchar inout....";
      CORBA::WChar c=L'A';
      testobj->t_wc_inout(c);
      if (c == L'X')
        cout << "passed"<< endl;
      else 
        cout << "failed" <<endl; 
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }
  }


  void test_wstring_inout(extest::Test_var& testobj) {
    // invoke method t_wstr_inout
    try{
      cout << "Testing wstring inout....";
      CORBA::WChar *s ;
      s=CORBA::wstring_alloc(100);
      wcscpy(s , L"This is my special string");
      int i;
      i= wcslen(s);
      testobj->t_wstr_inout(s);

      #ifdef LINUX
      if (wcscmp(s,L"hello to you")==0)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

     #else

      if (wcscmp(s,L"hello to you")==NULL)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;

     #endif

      CORBA::wstring_free(s); 
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }
  }


  void test_longlong_inout(extest::Test_var& testobj) {
    // invoke the t_ll_inout method
    try{
      cout << "Testing longlong inout....";
      CORBA::LongLong l;

      #ifdef LINUX
      l = -2147483648LL;

      #else 

      l = -2147483648;

      #endif

      testobj->t_ll_inout(l);
      if (l==2147483647)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl; 
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }
  }

#if !defined(NO_LONG_DOUBLE)
  void test_longdouble_inout(extest::Test_var& testobj) {
    // invoke the t_ld_inout method
    try{
      cout << "Testing longdouble inout....";
      CORBA::LongDouble ld;
      ld = 42949672957879;
      testobj->t_ld_inout(ld);
      if (ld==0)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }
  }
#endif

  void test_ulonglong_inout(extest::Test_var& testobj) {
    // invoke the t_ull_inout method
    try{
      cout << "Testing ulonglong inout....";
      CORBA::ULongLong u;

      #ifdef LINUX
      u = 4294967295ULL;

     #else
      u = 4294967295;

     #endif

      testobj->t_ull_inout(u);
      if (u== 0)
        cout << "passed"<<endl;
      else 
        cout << "failed" <<endl;
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }      
  }

  void test_struct_inout (extest::Test_var& testobj) {
    //invoke method t_struct_inout
    try{
      cout << "Testing struct inout....";
      extest::Test::extS_var a = new extest::Test::extS ;	
      a->twstr=CORBA::wstring_alloc(100);
      a->twc =L'c';
      wcscpy(a->twstr ,L"Hello World");
      a->tll=876543;
      a->tull=429497295;
#if !defined(NO_LONG_DOUBLE)
      a->tld = 4294967295787987;
#endif
      testobj->t_struct_inout(a);

     #ifdef LINUX
 
      if ((a->twc == L'x')
	  && (wcscmp(a->twstr,L"My Hello to you too")==0)
	  && (a->tll==0)
	  && (a->tull==42949729)

     #else 
      if ((a->twc == L'x')
	  && (wcscmp(a->twstr,L"My Hello to you too")==NULL)
	  && (a->tll==0)
	  && (a->tull==42949729)

     #endif

#if !defined(NO_LONG_DOUBLE)
          && (a->tld==0))
#else
	)
#endif
        cout << "passed " << endl;
      else 
        cout << "failed" <<endl; 
    }
    catch (CORBA::Exception& e) {
      cerr << "Caught CORBA Exception: " << e << endl;
    }	  
  }
};

int main(int argc, char* const* argv) {

  try{
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // binding to the ORB
    cout << "Binding..." << endl;
    extest::Test_var testobj = extest::Test::_bind();
    TESTM newobj;	
	
    CORBA::String_var test_name;
    int testnum;
    int result;
	
    if (argc == 2)
      test_name = (const char *) argv[1];
    else
      test_name = (const char *) "Default";

    #ifdef LINUX

    if (strcmp(test_name,"wchar")== 0)
      testnum=1;
    else if (strcmp(test_name,"wstring")== 0)
      testnum=2;
    else if (strcmp(test_name,"longlong")== 0)
      testnum=3;
    else if (strcmp(test_name,"longdouble")==0)
      testnum=4;
    else if (strcmp(test_name,"ulonglong")==0)
      testnum=5;
    else if (strcmp(test_name,"struct")==0)
      testnum=6;
    else if (strcmp(test_name,"sequence")== 0 )
      testnum=7;
    else if (strcmp(test_name,"wchar_inout")== 0)
      testnum=8;
    else if (strcmp(test_name,"wstring_inout")== 0)
      testnum=9;
    else if (strcmp(test_name,"longlong_inout")== 0)
      testnum=10;
    else if (strcmp(test_name,"longdouble_inout")==0)
      testnum=11;
    else if (strcmp(test_name,"ulonglong_inout")==0)
      testnum=12;
    else if (strcmp(test_name,"struct_inout")==0)
      testnum=13;
    else if (strcmp(test_name,"?")==0) {
      cout << "The useage for the test is "<<endl;
      cout << "client with no argument for all the test." <<endl;
      cout << "client with one of the following argument "<<endl;
      cout << "wchar, longlong, ulonglong, longdouble, struct, sequence" <<endl;
      cout << "wchar_inout, longlong_inout, ulonglong_inout, longdouble_inout, struct_inout" <<endl;
    }
    else 
      testnum=0;
    
	#else

    if (strcmp(test_name,"wchar")==NULL)
      testnum=1;
    else if (strcmp(test_name,"wstring")==NULL)
      testnum=2;
    else if (strcmp(test_name,"longlong")==NULL)
      testnum=3;
    else if (strcmp(test_name,"longdouble")==NULL)
      testnum=4;
    else if (strcmp(test_name,"ulonglong")==NULL)
      testnum=5;
    else if (strcmp(test_name,"struct")==NULL)
      testnum=6;
    else if (strcmp(test_name,"sequence")==NULL)
      testnum=7;
    else if (strcmp(test_name,"wchar_inout")==NULL)
      testnum=8;
    else if (strcmp(test_name,"wstring_inout")==NULL)
      testnum=9;
    else if (strcmp(test_name,"longlong_inout")==NULL)
      testnum=10;
    else if (strcmp(test_name,"longdouble_inout")==NULL)
      testnum=11;
    else if (strcmp(test_name,"ulonglong_inout")==NULL)
      testnum=12;
    else if (strcmp(test_name,"struct_inout")==NULL)
      testnum=13;
    else if (strcmp(test_name,"?")==NULL) {
      cout << "The useage for the test is "<<endl;
      cout << "client with no argument for all the test." <<endl;
      cout << "client with one of the following argument "<<endl;
      cout << "wchar, longlong, ulonglong, longdouble, struct, sequence" <<endl;
      cout << "wchar_inout, longlong_inout, ulonglong_inout, longdouble_inout, struct_inout" <<endl;
      return(0);
    }
    else 
      testnum=0;

   #endif
 
    switch (testnum) {
      case 1:   
        newobj.test_wchar(testobj);
        break;
      case 2:   
        newobj.test_wstring(testobj);	
        break;
      case 3:   
        newobj.test_longlong(testobj);
        break;
      case 4:   
#if !defined(NO_LONG_DOUBLE)
        newobj.test_longdouble(testobj);
#endif
        break;
      case 5:    
        newobj.test_ulonglong(testobj);
        break;
      case 6:    
        newobj.test_struct(testobj);
        break;           
      case 7:	   
        newobj.test_extS(testobj);
        break;
      case 8:    
        newobj.test_wchar_inout(testobj);
        break;
      case 9:   
        newobj.test_wstring_inout(testobj);
        break;
      case 10:  
        newobj.test_longlong_inout(testobj);
        break;
      case 11:  
#if !defined(NO_LONG_DOUBLE)
        newobj.test_longdouble_inout(testobj);
#endif
        break;
      case 12:   
        newobj.test_ulonglong_inout(testobj);
        break;
      case 13:   
        newobj.test_struct_inout(testobj);
        break;
      default:   
        newobj.test_wchar(testobj);
        newobj.test_wstring(testobj);
        newobj.test_longlong(testobj);
#if !defined(NO_LONG_DOUBLE)
        newobj.test_longdouble(testobj);
#endif
        newobj.test_ulonglong(testobj);
        newobj.test_struct(testobj);
        newobj.test_extS(testobj);
        newobj.test_wchar_inout(testobj);
        newobj.test_wstring_inout(testobj);
        newobj.test_longlong_inout(testobj);
#if !defined(NO_LONG_DOUBLE)
        newobj.test_longdouble_inout(testobj);
#endif
        newobj.test_ulonglong_inout(testobj);
        newobj.test_struct_inout(testobj);
        break;		   
      }
  }
  catch (CORBA::Exception& e) {
    cerr << "Caught CORBA Exception: " << e << endl;
    return (1);
  }
  return (0);
}
	







