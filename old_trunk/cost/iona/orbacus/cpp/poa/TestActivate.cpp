// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <Test_skel.h>

#include <Test_impl.h>
#include <TestUtil.h>

using namespace CORBA;
using namespace PortableServer;

class TestActivator_impl : public POA_PortableServer::ServantActivator,
			   public PortableServer::RefCountServantBase
{
    ObjectId oid_;
    POA_ptr poa_;
    Servant servant_;
    Boolean remaining_;
    bool valid_;

public:

    TestActivator_impl()
    {
        valid_ = false;
    }

    void expect(const ObjectId& oid,
                POA_ptr poa,
                Servant servant,
                Boolean remaining)
    {
        oid_ = oid;
        poa_ = poa;
        servant_ = servant;
        remaining_ = remaining;
        valid_ = false;
    }

    bool isValid() { return valid_; }

    Servant incarnate(const ObjectId& /*oid*/, POA_ptr /*poa*/)
        throw(ForwardRequest, SystemException)
    {
        return 0;
    }

    void etherealize(const ObjectId& oid,
                     POA_ptr poa,
                     Servant servant,
                     Boolean /*cleanup*/,
                     Boolean remaining)
        throw(SystemException)
    {
        TEST(TestUtil_Compare(oid_, oid));
        TEST(poa_ -> _is_equivalent(poa));
        TEST(servant_ == servant);
        TEST(remaining_ == remaining);
        valid_ = true;
    }
};


void
TestServantActivator(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var system, nonretain, multiple, ether;
    ObjectId_var id1, id2, id3;
    PolicyList policies;
    Test_impl* servant1;
    Test_impl* servant2;
    ServantBase_var tmpserv;

    POAManager_var manager = root -> the_POAManager();

    manager -> activate();

    //
    // Create POAs
    //

    policies.length(3);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::SYSTEM_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    system = root -> create_POA("system_id", manager, policies);

    policies.length(5);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies[3] = root ->
        create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);
    policies[4] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    nonretain = root -> create_POA("nonretain", manager, policies);

    policies.length(3);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::SYSTEM_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    multiple = root -> create_POA("multiple_id", manager, policies);

    policies.length(3);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::SYSTEM_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[2] = root ->
        create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

    ether = root -> create_POA("ether", manager, policies);
    TestActivator_impl* activatorImpl = new TestActivator_impl();
    PortableServer::ServantBase_var activatorServant = activatorImpl;
    PortableServer::ServantActivator_var activator = activatorImpl -> _this();

    // 
    // Start tests
    //

    //
    // Test: set_servant_manager with nil argument
    //
    try
    {
        ether -> set_servant_manager(PortableServer::ServantManager::_nil());
        TEST(false); // set_servant_manager should not have succeeded
    }
    catch(const OBJ_ADAPTER&)
    { 
        // expected
    }

    ether -> set_servant_manager(activator);

    servant1 = new Test_impl(orb, "obj1", false);
    servant2 = new Test_impl(orb, "obj2", false);

    //
    // Test: activate_object w/ SYSTEM_ID POA
    //

    id1 = system -> activate_object(servant1);
    id2 = system -> activate_object(servant2);
    TEST(!TestUtil_Compare(id1, id2));
    tmpserv = system -> id_to_servant(id1);
    TEST(tmpserv.in() == servant1);
    tmpserv = system -> id_to_servant(id2);
    TEST(tmpserv.in() == servant2);

    //
    // Test: ServantAlreadyActive exception
    //
    try
    {
        system -> activate_object(servant1);
        TEST(false); // activate_object should not have succeeded
    }
    catch(const POA::ServantAlreadyActive&)
    {
        // expected
    }

    try
    {
        system -> activate_object(servant2);
        TEST(false); // activate_object should not have succeeded
    }
    catch(const POA::ServantAlreadyActive&)
    {
        // expected
    }

    //
    // Test: deactivate_object
    //
    system -> deactivate_object(id2);
    system -> deactivate_object(id1);

    //
    // Test: ObjectNotActive exception
    //
    try
    {
        system -> deactivate_object(id1);
        TEST(false); // deactivate_object should not have succeeded
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    try
    {
        system -> deactivate_object(id2);
        TEST(false); // deactivate_object should not have succeeded
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    //
    // Test: WrongPolicy exception
    //
    try
    {
        nonretain -> activate_object(servant1);
        TEST(false); // activate_object should not have succeeded
    }
    catch(const POA::WrongPolicy&)
    {
        // expected
    }

    try
    {
        ObjectId_var id = string_to_ObjectId("XXX");
        nonretain -> activate_object_with_id(id, servant1);
        TEST(false); // activate_object_with_id should not have succeeded
    }
    catch(const POA::WrongPolicy&)
    {
        // expected
    }

    try
    {
        ObjectId_var id = string_to_ObjectId("XXX");
        nonretain -> deactivate_object(id);
        TEST(false); // deactivate_object should not have succeeded
    }
    catch(const POA::WrongPolicy&)
    {
        // expected
    }

    //
    // Test: activate_object w/ MULTIPLE_ID POA
    //

    id1 = multiple -> activate_object(servant1);
    id2 = multiple -> activate_object(servant1);
    TEST(!TestUtil_Compare(id1, id2));
    tmpserv = multiple -> id_to_servant(id1);
    TEST(tmpserv.in() == servant1);
    tmpserv = multiple -> id_to_servant(id2);
    TEST(tmpserv.in() == servant1);

    //
    // Test: confirm servant1 is no longer active
    //
    multiple -> deactivate_object(id1);
    multiple -> deactivate_object(id2);
    try
    {
        multiple -> id_to_servant(id1);
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    try
    {
        multiple -> id_to_servant(id2);
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    //
    // Test: confirm ServantActivator::etherealize is invoked on deactivate
    //
    id1 = ether -> activate_object(servant1);
    id2 = ether -> activate_object(servant1);
    id3 = ether -> activate_object(servant2);
    activatorImpl -> expect(id1, ether, servant1, true);
    ether -> deactivate_object(id1);
    TEST(activatorImpl -> isValid());
    activatorImpl -> expect(id2, ether, servant1, false);
    ether -> deactivate_object(id2);
    TEST(activatorImpl -> isValid());
    activatorImpl -> expect(id3, ether, servant2, false);
    ether -> deactivate_object(id3);
    TEST(activatorImpl -> isValid());


    system -> destroy(true, true);
    nonretain -> destroy(true, true);
    multiple -> destroy(true, true);
    ether -> destroy(true, true);

    tmpserv = 0;
    delete servant1;
    delete servant2;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;

    ORB_var orb;

    try
    {
        orb = ORB_init(argc, argv);

        POA_var root = TestUtil_GetRootPOA(orb);

        cout << "Testing servant activator... " << flush;
        TestServantActivator(orb, root);
        cout << "Done" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#endif
        status = EXIT_FAILURE;
    }

    if(!is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const Exception& ex)
	{
#ifdef HAVE_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
