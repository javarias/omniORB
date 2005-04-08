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

void
TestCreateReference(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var user, system;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;

    POAManager_var manager = root -> the_POAManager();

    policies.length(1);
    policies[0] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    user = root -> create_POA("user_id", manager, policies);

    policies.length(1);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::SYSTEM_ID);
    system = root -> create_POA("system_id", manager, policies);

    //
    // Test: create_reference with wrong POA policies
    //
    try
    {
        obj = user -> create_reference("IDL:Test:1.0");
        TEST(false); // create_reference should not have succeeded
    }
    catch(const POA::WrongPolicy&)
    {
        // expected
    }

    //
    // Test: create_reference - should get a new ID for each invocation
    //       on POA w/ SYSTEM_ID policy
    //
    obj = system -> create_reference("IDL:Test:1.0");
    TEST(!is_nil(obj));
    id1 = system -> reference_to_id(obj);
    obj = system -> create_reference("IDL:Test:1.0");
    TEST(!is_nil(obj));
    id2 = system -> reference_to_id(obj);
    TEST(!TestUtil_Compare(id1, id2));

    //
    // Test: create_reference_with_id using a system-generated ID
    //
    try
    {
        obj = system -> create_reference_with_id(id1, "IDL:Test:1.0");
    }
    catch(const BAD_PARAM&)
    {
        TEST(false); // create_reference_with_id should have succeeded
    }

    id1 = string_to_ObjectId("id1");

    //
    // Test: create_reference_with_id
    //
    obj = user -> create_reference_with_id(id1, "IDL:Test:1.0");
    TEST(!is_nil(obj));
    tmpid = user -> reference_to_id(obj);
    TEST(TestUtil_Compare(id1, tmpid));
    id2 = string_to_ObjectId("id2");
    obj = user -> create_reference_with_id(id2, "IDL:Test:1.0");
    TEST(!is_nil(obj));
    tmpid = user -> reference_to_id(obj);
    TEST(TestUtil_Compare(id2, tmpid));

    user -> destroy(true, true);
    system -> destroy(true, true);
}

void
TestServantToId(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var unique, implicit, multiple;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;
    Test_impl* servant1;
    Test_impl* servant2;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA w/ UNIQUE_ID, NO_IMPLICIT_ACTIVATION
    //
    policies.length(4);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[1] =
        root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[3] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    unique = root -> create_POA("unique_id", manager, policies);

    //
    // Create POA w/ UNIQUE_ID, IMPLICIT_ACTIVATION
    //
    policies.length(3);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
    implicit = root -> create_POA("implicit", manager, policies);

    //
    // Create POA w/ MULTIPLE_ID, IMPLICIT_ACTIVATION
    //
    policies.length(3);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
    multiple = root -> create_POA("multiple", manager, policies);

    servant1 = new Test_impl(orb, "test1", false);
    servant2 = new Test_impl(orb, "test2", false);

    //
    // Test: ServantNotActive exception
    //
    try
    {
        unique -> servant_to_id(servant1);
        TEST(false); // servant_to_id should not have succeeded
    }
    catch(const POA::ServantNotActive&)
    {
        // expected
    }

    id1 = string_to_ObjectId("test1");
    unique -> activate_object_with_id(id1, servant1);

    //
    // Test: servant_to_id (UNIQUE_ID policy)
    //
    tmpid = unique -> servant_to_id(servant1);
    TEST(TestUtil_Compare(id1, tmpid));

    //
    // Test: servant_to_id (IMPLICIT_ACTIVATION) - servant1 should
    //       be automatically activated
    //
    id1 = implicit -> servant_to_id(servant1);

    //
    // Test: Now that servant1 is activated, and since we have UNIQUE_ID,
    //       we should get the same ID back
    //
    tmpid = implicit -> servant_to_id(servant1);
    TEST(TestUtil_Compare(id1, tmpid));

    //
    // Test: Implicitly activating servant2 should produce a new ID
    //
    id2 = implicit -> servant_to_id(servant2);
    TEST(!TestUtil_Compare(id1, id2));

    //
    // Test: servant_to_id (IMPLICIT_ACTIVATION, MULTIPLE_ID) - servant1
    //       should be automatically activated
    //
    id1 = multiple -> servant_to_id(servant1);

    //
    // Test: Since we have MULTIPLE_ID, we should get a new ID
    //
    tmpid = multiple -> servant_to_id(servant1);
    TEST(!TestUtil_Compare(id1, tmpid));

    unique -> destroy(true, true);
    implicit -> destroy(true, true);
    multiple -> destroy(true, true);

    delete servant1;
    delete servant2;
}

void
TestIdToServant(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var retain, defaultPOA;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;
    Test_impl* def;
    Test_impl* servant1;
    Test_impl* servant2;
    ServantBase_var tmpservant;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA w/ RETAIN
    //
    policies.length(3);
    policies[0] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[1] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[2] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    retain = root -> create_POA("retain", manager, policies);

    //
    // Create POA w/ USE_DEFAULT_SERVANT
    //
    policies.length(5);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);
    policies[3] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[4] = root -> create_id_assignment_policy(PortableServer::USER_ID);

    defaultPOA = root -> create_POA("default", manager, policies);
    def = new Test_impl(orb, "default", false);
    defaultPOA -> set_servant(def);

    servant1 = new Test_impl(orb, "test1", false);
    servant2 = new Test_impl(orb, "test2", false);

    //
    // Test: ObjectNotActive exception
    //
    try
    {
        tmpid = string_to_ObjectId("bad_id");
        retain -> id_to_servant(tmpid);
        TEST(false); // id_to_servant should not have succeeded
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    id1 = string_to_ObjectId("test1");
    id2 = string_to_ObjectId("test2");
    retain -> activate_object_with_id(id1, servant1);
    retain -> activate_object_with_id(id2, servant2);

    //
    // Test: servant_to_id (RETAIN policy)
    //
    tmpservant = retain -> id_to_servant(id1);
    TEST(servant1 == tmpservant.in());
    tmpservant = retain -> id_to_servant(id2);
    TEST(servant2 == tmpservant.in());

    //
    // Test: id_to_servant (USE_DEFAULT_SERVANT)
    //
    defaultPOA -> activate_object_with_id(id1, servant1);
    defaultPOA -> activate_object_with_id(id2, servant2);
    tmpservant = defaultPOA -> id_to_servant(id1);
    TEST(servant1 == tmpservant.in());
    tmpservant = defaultPOA -> id_to_servant(id2);
    TEST(servant2 == tmpservant.in());

    //
    // Test: id_to_servant (USE_DEFAULT_SERVANT) - should return
    //       default servant for all unknown IDs
    //
    tmpid = string_to_ObjectId("test99");
    tmpservant = defaultPOA -> id_to_servant(tmpid);
    TEST(tmpservant.in() == def);
    tmpservant = (Servant)0;

    retain -> destroy(true, true);
    defaultPOA -> destroy(true, true);

    delete servant1;
    delete servant2;
    delete def;
}

void
TestServantToReference(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var unique, implicit, multiple;
    ObjectId_var id1, id2, tmpid1, tmpid2;
    PolicyList policies;
    Test_impl* servant1;
    Test_impl* servant2;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA w/ UNIQUE_ID, NO_IMPLICIT_ACTIVATION
    //
    policies.length(4);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[1] =
        root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[3] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    unique = root -> create_POA("unique_id", manager, policies);

    //
    // Create POA w/ UNIQUE_ID, IMPLICIT_ACTIVATION
    //
    policies.length(3);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
    implicit = root -> create_POA("implicit", manager, policies);

    //
    // Create POA w/ MULTIPLE_ID, IMPLICIT_ACTIVATION
    //
    policies.length(3);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
    multiple = root -> create_POA("multiple", manager, policies);

    servant1 = new Test_impl(orb, "test1", false);
    servant2 = new Test_impl(orb, "test2", false);

    //
    // Test: ServantNotActive exception
    //
    try
    {
        unique -> servant_to_reference(servant1);
        TEST(false); // servant_to_reference should not have succeeded
    }
    catch(const POA::ServantNotActive&)
    {
        // expected
    }

    id1 = string_to_ObjectId("test1");
    unique -> activate_object_with_id(id1, servant1);

    //
    // Test: servant_to_reference (UNIQUE_ID policy)
    //
    obj = unique -> servant_to_reference(servant1);
    TEST(!is_nil(obj));
    tmpid1 = unique -> reference_to_id(obj);
    TEST(TestUtil_Compare(id1, tmpid1));

    //
    // Test: servant_to_reference (IMPLICIT_ACTIVATION) - servant1 should
    //       be automatically activated
    //
    obj = implicit -> servant_to_reference(servant1);
    TEST(!is_nil(obj));
    tmpid1 = implicit -> reference_to_id(obj);

    //
    // Test: Now that servant1 is activated, and since we have UNIQUE_ID,
    //       we should get the same ID back
    //
    obj = implicit -> servant_to_reference(servant1);
    TEST(!is_nil(obj));
    tmpid2 = implicit -> reference_to_id(obj);
    TEST(TestUtil_Compare(tmpid1, tmpid2));

    //
    // Test: Implicitly activating servant2 should produce a new ID
    //
    obj = implicit -> servant_to_reference(servant2);
    TEST(!is_nil(obj));
    tmpid2 = implicit -> reference_to_id(obj);
    TEST(!TestUtil_Compare(tmpid1, tmpid2));

    //
    // Test: servant_to_reference (IMPLICIT_ACTIVATION, MULTIPLE_ID) -
    //       servant1 should be automatically activated
    //
    obj = multiple -> servant_to_reference(servant1);
    TEST(!is_nil(obj));
    tmpid1 = multiple -> reference_to_id(obj);

    //
    // Test: Since we have MULTIPLE_ID, we should get a new ID
    //
    obj = multiple -> servant_to_reference(servant1);
    TEST(!is_nil(obj));
    tmpid2 = multiple -> reference_to_id(obj);
    TEST(!TestUtil_Compare(tmpid1, tmpid2));

    unique -> destroy(true, true);
    implicit -> destroy(true, true);
    multiple -> destroy(true, true);

    delete servant1;
    delete servant2;
}

void
TestIdToReference(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var retain, defaultPOA;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;
    Test_impl* servant1;
    Test_impl* servant2;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA w/ RETAIN
    //
    policies.length(3);
    policies[0] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[1] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[2] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    retain = root -> create_POA("retain", manager, policies);

    servant1 = new Test_impl(orb, "test1", false);
    servant2 = new Test_impl(orb, "test2", false);

    //
    // Test: ObjectNotActive exception
    //
    try
    {
        tmpid = string_to_ObjectId("bad_id");
        retain -> id_to_reference(tmpid);
        TEST(false); // id_to_reference should not have succeeded
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    id1 = string_to_ObjectId("test1");
    id2 = string_to_ObjectId("test2");
    retain -> activate_object_with_id(id1, servant1);
    retain -> activate_object_with_id(id2, servant2);

    //
    // Test: servant_to_reference
    //
    obj = retain -> id_to_reference(id1);
    TEST(!is_nil(obj));
    tmpid = retain -> reference_to_id(obj);
    TEST(TestUtil_Compare(id1, tmpid));

    //
    // Test: servant_to_reference
    //
    obj = retain -> id_to_reference(id2);
    TEST(!is_nil(obj));
    tmpid = retain -> reference_to_id(obj);
    TEST(TestUtil_Compare(id2, tmpid));

    retain -> destroy(true, true);

    delete servant1;
    delete servant2;
}

void
TestReferenceToServant(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var retain, defaultPOA;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;
    Test_impl* def;
    Test_impl* servant1;
    Test_impl* servant2;
    ServantBase_var tmpservant;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA w/ RETAIN
    //
    policies.length(3);
    policies[0] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[1] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[2] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    retain = root -> create_POA("retain", manager, policies);

    //
    // Create POA w/ USE_DEFAULT_SERVANT
    //
    policies.length(5);
    policies[0] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[1] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[2] = root ->
        create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);
    policies[3] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[4] = root -> create_id_assignment_policy(PortableServer::USER_ID);

    defaultPOA = root -> create_POA("default", manager, policies);
    def = new Test_impl(orb, "default", false);
    defaultPOA -> set_servant(def);

    servant1 = new Test_impl(orb, "test1", false);
    servant2 = new Test_impl(orb, "test2", false);

    //
    // Test: ObjectNotActive exception
    //
    try
    {
        tmpid = string_to_ObjectId("bad_id");
        obj = retain -> create_reference_with_id(tmpid, "IDL:Test:1.0");
        retain -> reference_to_servant(obj);
        TEST(false); // reference_to_servant should not have succeeded
    }
    catch(const POA::ObjectNotActive&)
    {
        // expected
    }

    id1 = string_to_ObjectId("test1");
    id2 = string_to_ObjectId("test2");
    retain -> activate_object_with_id(id1, servant1);
    retain -> activate_object_with_id(id2, servant2);

    //
    // Test: reference_to_servant (RETAIN policy)
    //
    obj = retain -> create_reference_with_id(id1, "IDL:Test:1.0");
    tmpservant = retain -> reference_to_servant(obj);
    TEST(servant1 == tmpservant.in());
    obj = retain -> create_reference_with_id(id2, "IDL:Test:1.0");
    tmpservant = retain -> reference_to_servant(obj);
    TEST(servant2 == tmpservant.in());

    //
    // Test: WrongAdapter exception
    //
    try
    {
        obj = retain -> create_reference_with_id(id1, "IDL:Test:1.0");
        defaultPOA -> reference_to_servant(obj);
        TEST(false); // reference_to_servant should not have succeeded
    }
    catch(const POA::WrongAdapter&)
    {
        // expected
    }

    //
    // Test: reference_to_servant (USE_DEFAULT_SERVANT)
    //
    defaultPOA -> activate_object_with_id(id1, servant1);
    defaultPOA -> activate_object_with_id(id2, servant2);
    obj = defaultPOA -> create_reference_with_id(id1, "IDL:Test:1.0");
    tmpservant = defaultPOA -> reference_to_servant(obj);
    TEST(servant1 == tmpservant.in());
    obj = defaultPOA -> create_reference_with_id(id2, "IDL:Test:1.0");
    tmpservant = defaultPOA -> reference_to_servant(obj);
    TEST(servant2 == tmpservant.in());

    //
    // Test: reference_to_servant (USE_DEFAULT_SERVANT) - should return
    //       default servant for all unknown IDs
    //
    tmpid = string_to_ObjectId("test99");
    obj = defaultPOA -> create_reference_with_id(tmpid, "IDL:Test:1.0");
    tmpservant = defaultPOA -> reference_to_servant(obj);
    TEST(tmpservant.in() == def);
    tmpservant = (Servant)0;

    retain -> destroy(true, true);
    defaultPOA -> destroy(true, true);

    delete servant1;
    delete servant2;
    delete def;
}

void
TestReferenceToId(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var poa;
    ObjectId_var id1, id2, tmpid;
    PolicyList policies;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POA
    //
    policies.length(3);
    policies[0] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[1] = root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[2] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    poa = root -> create_POA("poa", manager, policies);

    id1 = string_to_ObjectId("test1");
    id2 = string_to_ObjectId("test2");

    //
    // Test: reference_to_id
    //
    obj = poa -> create_reference_with_id(id1, "IDL:Test:1.0");
    tmpid = poa -> reference_to_id(obj);
    TEST(TestUtil_Compare(tmpid, id1));
    obj = poa -> create_reference_with_id(id2, "IDL:Test:1.0");
    tmpid = poa -> reference_to_id(obj);
    TEST(TestUtil_Compare(tmpid, id2));

    //
    // Test: WrongAdapter exception
    //
    try
    {
        obj = poa -> create_reference_with_id(id1, "IDL:Test:1.0");
        root -> reference_to_id(obj);
        TEST(false); // reference_to_id should not have succeeded
    }
    catch(const POA::WrongAdapter&)
    {
        // expected
    }

    poa -> destroy(true, true);
}

void
TestToString()
{
    PortableServer::ObjectId_var oid1 = new PortableServer::ObjectId(10);
    oid1 -> length(10);
    memset(oid1 -> get_buffer(), 0, 10);

    //
    // Shouldn't succeed - embedded 0's
    //
    try
    {
	CORBA::String_var s1 = PortableServer::ObjectId_to_string(oid1);
	TEST(false); // ObjectId_to_string should not have succeeded
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }

    //
    // Shouldn't succeed - embedded 0's, wrong length
    //
    try
    {
	CORBA::WString_var s1 = PortableServer::ObjectId_to_wstring(oid1);
	TEST(false); // ObjectId_to_string should not have succeeded
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }

    oid1 -> length(12);
    memset(oid1 -> get_buffer(), 0, 10);

    //
    // Shouldn't succeed - embedded 0's.
    //
    try
    {
	CORBA::WString_var s1 = PortableServer::ObjectId_to_wstring(oid1);
	TEST(false); // ObjectId_to_string should not have succeeded
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }

    CORBA::Octet bytes[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			     '0', '1' };
    for(CORBA::ULong i = 0; i < sizeof(bytes)/sizeof(bytes[0]); ++i)
	oid1[i] = bytes[i];

    CORBA::String_var s1 = PortableServer::ObjectId_to_string(oid1);
    PortableServer::ObjectId_var oid2 = PortableServer::string_to_ObjectId(s1);
    TEST(oid2 -> length() == oid1 -> length());
    TEST(memcmp(oid1 -> get_buffer(), oid2 -> get_buffer(),
		oid1 -> length()) == 0);

    CORBA::WString_var s2 = PortableServer::ObjectId_to_wstring(oid1);
    oid2 = PortableServer::wstring_to_ObjectId(s2);
    TEST(oid2 -> length() == oid1 -> length());
    TEST(memcmp(oid1 -> get_buffer(), oid2 -> get_buffer(),
		oid1 -> length()) == 0);
}

void
runtests(ORB_ptr orb, POA_ptr root)
{
    TestToString();
    TestCreateReference(orb, root);
    TestServantToId(orb, root);
    TestIdToServant(orb, root);
    TestServantToReference(orb, root);
    TestIdToReference(orb, root);
    TestReferenceToServant(orb, root);
    TestReferenceToId(orb, root);
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

        cout << "Testing miscellaneous POA functions... " << flush;

        //
        // Run the tests using the root POA
        //
        runtests(orb, root);

        //
        // Create a child POA and run the tests again using the
        // child as the root
        //
        PolicyList policies;
        POAManager_var manager = root -> the_POAManager();
        POA_var child = root -> create_POA("child", manager, policies);
        runtests(orb, child);

        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
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
#else
	    cerr << "Exception: " << ex._rep_id() << endl;
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
