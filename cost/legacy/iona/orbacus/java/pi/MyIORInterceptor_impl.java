// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.CORBA.*;
import org.omg.PortableInterceptor.*;

final class MyIORInterceptor_impl extends org.omg.CORBA.LocalObject
    implements IORInterceptor
{
    private org.omg.IOP.Codec cdrCodec_;

    MyIORInterceptor_impl(ORBInitInfo info)
    {
	org.omg.IOP.CodecFactory factory = info.codec_factory();

	org.omg.IOP.Encoding how = new org.omg.IOP.Encoding(
	    (byte)org.omg.IOP.ENCODING_CDR_ENCAPS.value, (byte)0,
	    (byte)0);

	try
	{
	    cdrCodec_ = factory.create_codec(how);
	}
	catch(org.omg.IOP.CodecFactoryPackage.UnknownEncoding ex)
	{
	    throw new RuntimeException();
	}
    }

    //
    // IDL to Java Mapping
    //

    public String
    name()
    {
	return "";
    }

    public void
    destroy()
    {
    }

    public void
    establish_components(IORInfo info)
    {
	try
	{
	    Policy p = info.get_effective_policy(MY_SERVER_POLICY_ID.value);
	    MyServerPolicy policy = MyServerPolicyHelper.narrow(p);

	    MyComponent content = new MyComponent();
	    content.val = policy.value();
	    Any any = ORB.init().create_any();
	    MyComponentHelper.insert(any, content);

	    byte[] encoding = null;
	    try
	    {
		encoding = cdrCodec_.encode_value(any);
	    }
	    catch(org.omg.IOP.CodecPackage.InvalidTypeForEncoding ex)
	    {
		throw new RuntimeException();
	    }

	    org.omg.IOP.TaggedComponent component =
		new org.omg.IOP.TaggedComponent();
	    component.tag = MY_COMPONENT_ID.value;
	    component.component_data = new byte[encoding.length];
	    System.arraycopy(encoding, 0, component.component_data, 0,
			     encoding.length);

	    info.add_ior_component(component);
	}
	catch(INV_POLICY ex)
	{
	    return;
	}
    }

    public void
    components_established(IORInfo info)
    {
    }

    public void
    adapter_state_change(ObjectReferenceTemplate[] templates, short state)
    {
    }

    public void
    adapter_manager_state_change(int id, short state)
    {
    }
}
