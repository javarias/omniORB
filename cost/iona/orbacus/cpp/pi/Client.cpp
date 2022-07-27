// **********************************************************************
//
// Copyright (c) 2002
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestInterface.h>
#include <ClientCommon.h>
#include <TestPolicy.h>

#include <fstream>

using namespace CORBA;
using namespace PortableInterceptor;

class MyClientPolicy_impl : public MyClientPolicy,
			    public OBCORBA::RefCountLocalObject
{
    //
    // Hide copy-constructor and assignment operator
    //
    MyClientPolicy_impl(const MyClientPolicy_impl&);
    void operator=(const MyClientPolicy_impl&);
    
    Long value_;
    
public:
    
    MyClientPolicy_impl(Long value)
        : value_(value)
    {
    }

    ~MyClientPolicy_impl()
    {
    }

    //
    // Standard IDL to C++ Mapping
    //
    virtual Long value()
    {
        return value_;
    }

    virtual CORBA::PolicyType policy_type()
    {
        return MY_CLIENT_POLICY_ID;
    }

    virtual CORBA::Policy_ptr copy()
    {
        return Policy::_duplicate(this);
    }

    virtual void destroy()
    {
    }
};

class MyClientPolicyFactory_impl : public PortableInterceptor::PolicyFactory,
				   public OBCORBA::RefCountLocalObject
{
public:

    //
    // IDL to C++ Mapping
    //
    virtual CORBA::Policy_ptr create_policy(CORBA::PolicyType type,
                                            const CORBA::Any& any)
    {
        if(type == MY_CLIENT_POLICY_ID)
        {
            Long val;
            if(any >>= val)
                return new MyClientPolicy_impl(val);
            throw PolicyError(BAD_POLICY_TYPE);
        }
        throw PolicyError(BAD_POLICY);
        return Policy::_nil(); // Some compilers need this
    }
};

static ClientProxyManager* clientProxyManager = 0;

class ClientORBInitializer_impl : public PortableInterceptor::ORBInitializer,
				  public OBCORBA::RefCountLocalObject
{
    static bool local_;

public:

    ClientORBInitializer_impl()
    {
    }

    static void
    _OB_setLocal(bool l)
    {
	local_ = l;
    }

    //
    // IDL to C++ Mapping
    //
    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
        //
        // Test: PICurrent::allocate_state_slot
        //
        if(!local_)
        {
            PortableInterceptor::SlotId id =
                info -> allocate_slot_id();
            TEST(id == 0);
        }
        
        //
        // Test: Register a policy factory
        //
        PolicyFactory_var pf = new MyClientPolicyFactory_impl;
        info -> register_policy_factory(MY_CLIENT_POLICY_ID, pf);

        cout << "Registering client request interceptors... "
	     << flush;
        clientProxyManager = new ClientProxyManager(info);
        cout << "Done!" << endl;
    }

    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    }
};

bool ClientORBInitializer_impl::local_ = false;

void
ClientRegisterInterceptors(bool local)
{
    ORBInitializer_var initializer = new ClientORBInitializer_impl;
    ClientORBInitializer_impl::_OB_setLocal(local);
    PortableInterceptor::register_orb_initializer(initializer);
}

int
ClientRun(ORB_ptr orb, bool local, int argc, char* argv[])
{
    //
    // Get TestInterface
    //
    const char* refFile = "TestInterface.ref";
    ifstream in; // Must use open(name), not ifstream in(name) (VC++ bug)
    in.open(refFile);
    if(in.fail())
    {
	OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
	return 1;
    }
    
    char impl[1000];
    char dsiImpl[1000];
    in >> impl;
    in >> dsiImpl;

    in.close(); // The explicit close is necessary to satisfy the Windows NT
                // file locking

    cout << "Testing string_to_object()... " << flush;
    Object_var obj = orb -> string_to_object(impl);
    Object_var dsiObj = orb -> string_to_object(dsiImpl);
    TEST(!is_nil(obj));
    TEST(!is_nil(dsiObj));
    cout << " Done!" << endl;

    //
    // Test: Create a policy set on the object-reference
    //
    Any any;
    any <<= (Long)10;
    PolicyList pl(1);
    pl.length(1);
    pl[0] = orb -> create_policy(MY_CLIENT_POLICY_ID, any);

    cout << "Testing _narrow()... " << flush;
    obj = obj -> _set_policy_overrides(pl, ADD_OVERRIDE);
    TestInterface_var ti = TestInterface::_narrow(obj);
    dsiObj = dsiObj -> _set_policy_overrides(pl, ADD_OVERRIDE);
    TestInterface_var tiDSI = TestInterface::_narrow(dsiObj);
    TEST(!is_nil(ti));
    cout << " Done!" << endl;

    //
    // Test: Codec
    //
    cout << "Testing Codec... " << flush;
    extern void TestCodec(ORB_ptr);
    TestCodec(orb);
    cout << "Done!" << endl;

    //
    // Test: Exception translation
    //
    cout << "Testing client side exception translation..."
		 << flush;
    extern void TestTranslation(ORB_ptr, ClientProxyManager&,
				TestInterface_ptr);
    TestTranslation(orb, *clientProxyManager, ti);
    cout << "Done!" << endl;

    //
    // Run tests
    //
    cout << "Testing standard method calls with static stubs... "
		 << flush;
    extern void TestCalls(ORB_ptr, ClientProxyManager&, TestInterface_ptr);
    TestCalls(orb, *clientProxyManager, ti);
    cout << "Done!" << endl;

    cout << "Ditto, but with the DSI implementation... "
		 << flush;
    TestCalls(orb, *clientProxyManager, tiDSI);
    cout << "Done!" << endl;

    cout << "Testing standard method calls with the DII... "
		 << flush;
    extern void TestDIICalls(ORB_ptr, ClientProxyManager&, TestInterface_ptr);
    TestDIICalls(orb, *clientProxyManager, ti);
    cout << "Done!" << endl;

    cout << "Ditto, but with the DSI implementation... "
		 << flush;
    TestDIICalls(orb, *clientProxyManager, tiDSI);
    cout << "Done!" << endl;

    ti -> deactivate();

    return(0);
}

void
ClientCleanup()
{
    delete clientProxyManager;
}
