#include <iostream.h>
#include <common/omnitest.h>

#include "attrObjRef.hh"


OMNI_SIMPLE_SERVER_DECL(MyApp);


class Forwarding_ServantLocator_i
  : public POA_PortableServer::ServantLocator,
    public PortableServer::RefCountServantBase
{
 public:
  Forwarding_ServantLocator_i(const CORBA::Object_ptr forw_obj);

  virtual ~Forwarding_ServantLocator_i();


  virtual PortableServer::Servant
  preinvoke(const PortableServer::ObjectId& oid,
	    PortableServer::POA_ptr adapter,
	    const char* operation,
	    PortableServer::ServantLocator::Cookie& the_cookie);

  virtual void
  postinvoke(const PortableServer::ObjectId& oid,
	     PortableServer::POA_ptr adapter,
	     const char* operation,
	     PortableServer::ServantLocator::Cookie the_cookie,
	     PortableServer::Servant the_servant);


  inline CORBA::Object_ptr get_forw_obj()
  {
    return CORBA::Object::_duplicate(forw_obj);
  }

  inline void set_forw_obj(CORBA::Object_ptr o)
  {
    forw_obj = CORBA::Object::_duplicate(o);
  }


 private:
  CORBA::Object_var forw_obj;
};


Forwarding_ServantLocator_i::Forwarding_ServantLocator_i(const CORBA::Object_ptr forw_obj)
  : forw_obj(CORBA::Object::_duplicate(forw_obj))
{
}


Forwarding_ServantLocator_i::~Forwarding_ServantLocator_i()
{
}


PortableServer::Servant
Forwarding_ServantLocator_i::preinvoke(const PortableServer::ObjectId& oid,
				       PortableServer::POA_ptr adapter,
				       const char* operation,
				       PortableServer::ServantLocator::Cookie& the_cookie)
{
  throw PortableServer::ForwardRequest(forw_obj);
}


void
Forwarding_ServantLocator_i::postinvoke(const PortableServer::ObjectId& oid,
					PortableServer::POA_ptr adapter,
					const char* operation,
					PortableServer::ServantLocator::Cookie the_cookie,
					PortableServer::Servant the_servant)
{
  // nothing to do here - should never be invoked...
}


class attrObjRef_i
  : public POA_attrObjRef,
    public PortableServer::RefCountServantBase
{
 public:
  attrObjRef_i(Forwarding_ServantLocator_i *forw_loc, CORBA::Object_ptr o);

  virtual ~attrObjRef_i();
  

  CORBA::Object_ptr obj();

  void obj(CORBA::Object_ptr o);


 private:
  Forwarding_ServantLocator_i *forw_loc;
  CORBA::Object_var forw_obj;
};


attrObjRef_i::attrObjRef_i(Forwarding_ServantLocator_i *forw_loc,
			   CORBA::Object_ptr o)
  : forw_loc(forw_loc),
    forw_obj(CORBA::Object::_duplicate(o))
{
  attrObjRef_i::forw_loc = forw_loc;
  forw_loc->_add_ref();
}


attrObjRef_i::~attrObjRef_i()
{
  forw_loc->_remove_ref();
}


CORBA::Object_ptr attrObjRef_i::obj()
{
  return CORBA::Object::_duplicate(forw_obj);
}


void attrObjRef_i::obj(CORBA::Object_ptr o)
{
  forw_loc->set_forw_obj(o);
}


int
MyApp::initServer(int argc, char** argv)
{
  CORBA::Object_var forw_obj = get_server_object();
  Forwarding_ServantLocator_i *forw_loc_impl = new Forwarding_ServantLocator_i(forw_obj);
  PortableServer::ServantLocator_var forw_loc = forw_loc_impl->_this();


  // set up the policies for our forwarding POA
  CORBA::PolicyList policies;

  policies.length(3);
  policies[0] = poa()->create_servant_retention_policy(PortableServer::NON_RETAIN);
  policies[1] = poa()->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
  policies[2] = poa()->create_thread_policy(PortableServer::ORB_CTRL_MODEL);

  PortableServer::POAManager_var pman = poa()->the_POAManager();
  PortableServer::POA_var forwpoa =
    poa()->create_POA("fwd", pman, policies);

  forwpoa->set_servant_manager(forw_loc);


  //CORBA::Object_var server_obj = forwpoa->create_reference("IDL:omg.org/CORBA/Object:1.0");
  CORBA::Object_var server_obj = forwpoa->create_reference("");


  attrObjRef_i *attr_i = new attrObjRef_i(forw_loc_impl, server_obj);
  CORBA::Object_var attr_obj = attr_i->_this();
  attr_i->_remove_ref();
  forw_loc_impl->_remove_ref();


  // publish the attrObjRef - a client can get the forwarding obj-ref. from
  // there...
  server_object(attr_obj);
  return 0;
}
