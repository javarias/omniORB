// Testing code: implementation of multiEcho objects
//
//   interface Echo {
//        string echoString(in string mesg);
//   };
//
//   interface attrEcho {
//     attribute string a1;
//     readonly attribute string a2;
//     readonly attribute long a3;
//     attribute long a4;
//   };
//
// Implement multiple interfaces using tie template

#include <iostream>
#include <string.h>
#include <multiEcho.hh>
#include <common/common.h>

using namespace std;


// implementation using tie template
class multiEcho_i {
public:
  multiEcho_i() {}
  virtual char * echoString(const char *mesg) {
    char *p = CORBA::string_alloc(strlen((const char*)mesg));
    strcpy((char *)p,(const char *)mesg);
    return p;
  }
  virtual char *  a1 () {
    char *_result;
    if (pd_string) {
      _result = CORBA::string_alloc(strlen((const char*)pd_string));
      strcpy(_result,(const char*)pd_string);
    }
    else {
      _result = CORBA::string_alloc(0);
      _result[0] = '\0';
    }
    return _result;
  }
  virtual void a1 (const char *  _value) {
    if (pd_string) {
      CORBA::string_free(pd_string);
      pd_string = 0;
    }
    pd_string = CORBA::string_alloc(strlen((const char*)_value));
    strcpy((char *)pd_string,(const char *)_value);
  }
  virtual char *  a2 () {
    char *_result = CORBA::string_alloc(0);
    _result[0] = '\0';
    return _result;
  }
  virtual CORBA::Long  a3 () { return 0; }
  virtual CORBA::Long  a4 () { return 1; }
  virtual void a4 (CORBA::Long  _value) { return; }

private:
  char * pd_string;
};


int
main(int argc, char **argv) {

  try {

    CORBA::ORB_ptr orb = CORBA::ORB_init(argc,argv,"omniORB2");
    CORBA::BOA_ptr boa = orb->BOA_init(argc,argv,"omniORB2_BOA");

    CosNaming::Name use_name;
  
    if (argc > 1) {
      cerr << "Usage: multitiesrv" << endl;
      return 1;
    }

    multiEcho_i *myobj = new multiEcho_i();
    _tie_Echo<multiEcho_i,1> *myEcho = new _tie_Echo<multiEcho_i,1>(myobj);
    _tie_attrEcho<multiEcho_i,0> *myattEcho = new _tie_attrEcho<mutliEcho_i,0>(myobj);
    myEcho->_obj_is_ready(boa);
    myattrEcho->_obj_is_ready(boa);
    {
      Echo_var myEchoRef = myEcho->_this();
      CORBA::String_var p = orb->object_to_string(myobjRef);
      cerr << (char*)p << endl;

      attrEcho_var myattrRef = myattEcho->_this();
      p = orb->object_to_string(myattRef);
      cerr << (char*)p << endl;
    }

    boa->impl_is_ready();
  }
  catch (const omniORB::fatalException &ex) {
      cerr << "#### You have caught an omniORB2 bug, details are as follows:" << endl;
      cerr << ex.file() << " " << ex.line() << ":" << ex.errmsg() << endl; 
      cerr << "main thread exited." << endl;
      return 1;
    }
  return 0;
}
