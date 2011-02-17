// usage: client -i <stringified IOR>
//        client -i <stringified IOR> <new attribute: stringified IOR>
//        client <attribute object name>
//        client <attribute object name> <new attribute: read from this name>
//               Names are COSNaming compound names (e.g. x/y/test.obj
// 


#include <iostream>
#include <common/common.h>
#include <common/omnitest.h>
#include <attrObjRef.hh>

using namespace std;


OMNI_SIMPLE_CLIENT(MyApp, attrObjRef)


void MyApp::test(attrObjRef_ptr e)
{
  CORBA::Object_var obj = e->obj();

  e->obj(obj);
  OMNI_ASSERT(CORBA::is_nil(e->obj()));

  e->obj(obj);
  OMNI_ASSERT(!CORBA::is_nil(e->obj()));
}
