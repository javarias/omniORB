// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//    02111-1307, USA
//
//
// Testing code: client of attrObjRef objects
//
// interface attrObjRef {
//   attribute Object obj;
// };
//

// usage: attrObjRefclt -i <stringified IOR>
//        attrObjRefclt -i <stringified IOR> <new attribute: stringified IOR>
//        attrObjRefclt <attribute object name>
//        attrObjRefclt <attribute object name> <new attribute: read from this name>
//               Names are COSNaming compound names (e.g. x/y/test.obj
// 

#include <iostream>
#include <common/omnitest.h>
#include <common/common.h>
#include <attrObjRef.hh>

using namespace std;


const char* x = "IOR:000000000000001349444c3a617474724f626a5265663a312e300001000000010000000000000028000100000000000d3139322e352e3233392e3631002089900000000c000000000000000000000009";


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(attrObjRef, e);


  CORBA::Boolean setobj = 0;
  CORBA::Object_var newvalue;

#if 0
    cerr << "usage: attrObjRefclt -i <stringified IOR>\n"
	 << "       attrObjRefclt -i <stringified IOR> <new attribute: stringified IOR>\n"
	 << "       attrObjRefclt <attribute object name>\n"
	 << "       attrObjRefclt <attribute object name> <new attribute: read from this name>\n"
	 << "       Names are COSNaming compound names (e.g. x/y/test.obj)"
	 << endl;
#endif

  if (argc > 1)
  {
    if (!strncmp("IOR:", argv[1], 4))
    {
      newvalue = orb()->string_to_object(argv[1]);
    }
    else
    {
      CosNaming::Name value_name = string_to_name(argv[1]);
      newvalue = getObjectReference(orb(), value_name);
    }
    setobj = 1;
  }


  if (setobj)
  {
    e->obj(newvalue);
  }

  CORBA::Object_var retvalue = e->obj();

  CORBA::String_var objstr;

  if (setobj)
  {
    if (!newvalue->_is_equivalent(retvalue))
    {
      cerr << "Error: get attribute returns a different object reference." << endl;
      objstr = orb()->object_to_string(newvalue);
      cerr << "       set value = '" << (char *)objstr << "'" << endl;
      objstr = orb()->object_to_string(retvalue);
      cerr << "    return value = '" << (char *)objstr << "'" << endl;
      OMNI_FAILED("attribute value differs");
    }
  }
  objstr = orb()->object_to_string(retvalue);
  cerr << "'" << (char*)objstr << "'" << endl;
  

  test_complete();
  return 1;
}
