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

using namespace CORBA;

void
TestCodec(ORB_ptr orb)
{
    //
    // Test: Resolve CodecFactory
    //
    IOP::CodecFactory_var factory = IOP::CodecFactory::_narrow(
        Object_var(orb -> resolve_initial_references("CodecFactory")));
    TEST(!is_nil(factory));

    IOP::Encoding how;
    how.major_version = 0;
    how.minor_version = 0;

    //
    // Test: Create non-existent codec
    //
    try
    {
        how.format = 1; // Some unknown value
        IOP::Codec_var codec = factory -> create_codec(how);
        TEST(false);
    }
    catch(const IOP::CodecFactory::UnknownEncoding&)
    {
        // Expected
    }

    //
    // Test: CDR Codec
    //
    how.format = IOP::ENCODING_CDR_ENCAPS;
    IOP::Codec_var cdrCodec = factory -> create_codec(how);
    assert(!is_nil(cdrCodec));

    //
    // Test: Encode/decode
    //
    foo f;
    f.l = 10;
    Any any;
    any <<= f;

    OctetSeq_var encoding = cdrCodec -> encode(any);
    Any_var result = cdrCodec -> decode(encoding);
    
    const foo* newf;
    TEST(result >>= newf);
    TEST(newf -> l == 10);

    //
    // Test: Encode/decode
    //
    encoding = cdrCodec -> encode_value(any);
    result = cdrCodec -> decode_value(encoding, _tc_foo);
    
    TEST(result >>= newf);
    TEST(newf -> l == 10);
}
