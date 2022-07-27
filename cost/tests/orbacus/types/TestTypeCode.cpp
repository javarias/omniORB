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

#include <Types.h>

using namespace CORBA;

void
TestTypeCode()
{
    {
	TCKind kind = _tc_TestStruct1 -> kind();
	CHECK(kind == tk_struct);

	String_var name = _tc_TestStruct1 -> name();
	CHECK(strcmp(name, "TestStruct1") == 0);

	ULong count = _tc_TestStruct1 -> member_count();
	CHECK(count == 7);

	String_var name0 = _tc_TestStruct1 -> member_name(0);
	CHECK(strcmp(name0, "s") == 0);

	String_var name1 = _tc_TestStruct1 -> member_name(1);
	CHECK(strcmp(name1, "l") == 0);

	String_var name2 = _tc_TestStruct1 -> member_name(2);
	CHECK(strcmp(name2, "d") == 0);

	String_var name3 = _tc_TestStruct1 -> member_name(3);
	CHECK(strcmp(name3, "b") == 0);

	String_var name4 = _tc_TestStruct1 -> member_name(4);
	CHECK(strcmp(name4, "c") == 0);

	String_var name5 = _tc_TestStruct1 -> member_name(5);
	CHECK(strcmp(name5, "o") == 0);

	String_var name6 = _tc_TestStruct1 -> member_name(6);
	CHECK(strcmp(name6, "str") == 0);

	TypeCode_var type0 = _tc_TestStruct1 -> member_type(0);
	CHECK(type0 -> equal(_tc_short));

	TypeCode_var type1 = _tc_TestStruct1 -> member_type(1);
	CHECK(type1 -> equal(_tc_long));

	TypeCode_var type2 = _tc_TestStruct1 -> member_type(2);
	CHECK(type2 -> equal(_tc_double));

	TypeCode_var type3 = _tc_TestStruct1 -> member_type(3);
	CHECK(type3 -> equal(_tc_boolean));

	TypeCode_var type4 = _tc_TestStruct1 -> member_type(4);
	CHECK(type4 -> equal(_tc_char));

	TypeCode_var type5 = _tc_TestStruct1 -> member_type(5);
	CHECK(type5 -> equal(_tc_octet));

	TypeCode_var type6 = _tc_TestStruct1 -> member_type(6);
	CHECK(type6 -> equal(_tc_string));
    }

    {
	TCKind kind = _tc_TestStruct2 -> kind();
	CHECK(kind == tk_struct);

	String_var name = _tc_TestStruct2 -> name();
	CHECK(strcmp(name, "TestStruct2") == 0);

	ULong count = _tc_TestStruct2 -> member_count();
	CHECK(count == 4);

	String_var name0 = _tc_TestStruct2 -> member_name(0);
	CHECK(strcmp(name0, "s") == 0);

	TypeCode_var type0 = _tc_TestStruct2 -> member_type(0);
	CHECK(type0 -> equal(_tc_TestStruct1));

	String_var name1 = _tc_TestStruct2 -> member_name(1);
	CHECK(strcmp(name1, "a") == 0);

	TypeCode_var type1 = _tc_TestStruct2 -> member_type(1);
	CHECK(type1 -> equal(_tc_any));

	String_var name2 = _tc_TestStruct2 -> member_name(2);
	CHECK(strcmp(name2, "da") == 0);

	TypeCode_var type2 = _tc_TestStruct2 -> member_type(2);
	CHECK(type2 -> equal(_tc_DoubleArray));

	String_var name3 = _tc_TestStruct2 -> member_name(3);
	CHECK(strcmp(name3, "sa") == 0);

	TypeCode_var type3 = _tc_TestStruct2 -> member_type(3);
	//CHECK(type3 -> equal(_tc_...));
    }

    {
	TCKind kind = _tc_TestStruct3 -> kind();
	CHECK(kind == tk_struct);

	String_var name = _tc_TestStruct3 -> name();
	CHECK(strcmp(name, "TestStruct3") == 0);

	ULong count = _tc_TestStruct3 -> member_count();
	CHECK(count == 2);

	String_var name0 = _tc_TestStruct3 -> member_name(0);
	CHECK(strcmp(name0, "l") == 0);

	TypeCode_var type0 = _tc_TestStruct3 -> member_type(0);
	CHECK(type0 -> equal(_tc_long));

	String_var name1 = _tc_TestStruct3 -> member_name(1);
	CHECK(strcmp(name1, "seq") == 0);

	TypeCode_var type1 = _tc_TestStruct3 -> member_type(1);
	TypeCode_var contentType = type1 -> content_type();
	CHECK(contentType -> equal(_tc_TestStruct3));
    }

    {
	TCKind kind = _tc_TestStruct4 -> kind();
	CHECK(kind == tk_struct);

	String_var name = _tc_TestStruct4 -> name();
	CHECK(strcmp(name, "TestStruct4") == 0);

	ULong count = _tc_TestStruct4 -> member_count();
	CHECK(count == 2);

	String_var name0 = _tc_TestStruct4 -> member_name(0);
	CHECK(strcmp(name0, "a") == 0);

	TypeCode_var type0 = _tc_TestStruct4 -> member_type(0);
	CHECK(type0 -> equal(_tc_TestStruct3));

	String_var name1 = _tc_TestStruct4 -> member_name(1);
	CHECK(strcmp(name1, "b") == 0);

	TypeCode_var type1 = _tc_TestStruct4 -> member_type(1);
	TypeCode_var contentType = type1 -> content_type();
	CHECK(contentType -> equal(_tc_TestStruct3));
    }

    {
	TCKind kind = _tc_TestUnion4 -> kind();
	CHECK(kind == tk_union);

	String_var name = _tc_TestUnion4 -> name();
	CHECK(strcmp(name, "TestUnion4") == 0);

	ULong count = _tc_TestUnion4 -> member_count();
	CHECK(count == 2);

	String_var name0 = _tc_TestUnion4 -> member_name(0);
	CHECK(strcmp(name0, "seq") == 0);

	TypeCode_var type0 = _tc_TestUnion4 -> member_type(0);
	TypeCode_var contentType = type0 -> content_type();
	CHECK(contentType -> equal(_tc_TestUnion4));

	String_var name1 = _tc_TestUnion4 -> member_name(1);
	CHECK(strcmp(name1, "c") == 0);

	TypeCode_var type1 = _tc_TestUnion4 -> member_type(1);
	CHECK(type1 -> equal(_tc_char));
    }

    {
	TypeCode_ptr p;

	p = _tc_RepositoryId;
	CHECK(strcmp(p -> name(), "RepositoryId") == 0);

	p = _tc_ScopedName;
	CHECK(strcmp(p -> name(), "ScopedName") == 0);

	p = _tc_Identifier;
	CHECK(strcmp(p -> name(), "Identifier") == 0);

	p = _tc_DefinitionKind;
	CHECK(strcmp(p -> name(), "DefinitionKind") == 0);

	p = _tc_IRObject;
	CHECK(strcmp(p -> name(), "IRObject") == 0);

	p = _tc_VersionSpec;
	CHECK(strcmp(p -> name(), "VersionSpec") == 0);

	p = _tc_Contained;
	CHECK(strcmp(p -> name(), "Contained") == 0);

	p = Contained::_tc_Description;
	CHECK(strcmp(p -> name(), "Description") == 0);

	p = _tc_InterfaceDefSeq;
	CHECK(strcmp(p -> name(), "InterfaceDefSeq") == 0);

	p = _tc_ContainedSeq;
	CHECK(strcmp(p -> name(), "ContainedSeq") == 0);

	p = _tc_StructMember;
	CHECK(strcmp(p -> name(), "StructMember") == 0);

	p = _tc_StructMemberSeq;
	CHECK(strcmp(p -> name(), "StructMemberSeq") == 0);

	p = _tc_UnionMember;
	CHECK(strcmp(p -> name(), "UnionMember") == 0);

	p = _tc_UnionMemberSeq;
	CHECK(strcmp(p -> name(), "UnionMemberSeq") == 0);

	p = _tc_EnumMemberSeq;
	CHECK(strcmp(p -> name(), "EnumMemberSeq") == 0);

	p = _tc_Container;
	CHECK(strcmp(p -> name(), "Container") == 0);

	p = Container::_tc_Description;
	CHECK(strcmp(p -> name(), "Description") == 0);

	p = Container::_tc_DescriptionSeq;
	CHECK(strcmp(p -> name(), "DescriptionSeq") == 0);

	p = _tc_IDLType;
	CHECK(strcmp(p -> name(), "IDLType") == 0);

	p = _tc_PrimitiveKind;
	CHECK(strcmp(p -> name(), "PrimitiveKind") == 0);

	p = _tc_Repository;
	CHECK(strcmp(p -> name(), "Repository") == 0);

	p = _tc_ModuleDef;
	CHECK(strcmp(p -> name(), "ModuleDef") == 0);

	p = _tc_ModuleDescription;
	CHECK(strcmp(p -> name(), "ModuleDescription") == 0);

	p = _tc_ConstantDef;
	CHECK(strcmp(p -> name(), "ConstantDef") == 0);

	p = _tc_ConstantDescription;
	CHECK(strcmp(p -> name(), "ConstantDescription") == 0);

	p = _tc_TypedefDef;
	CHECK(strcmp(p -> name(), "TypedefDef") == 0);

	p = _tc_TypeDescription;
	CHECK(strcmp(p -> name(), "TypeDescription") == 0);

	p = _tc_StructDef;
	CHECK(strcmp(p -> name(), "StructDef") == 0);

	p = _tc_UnionDef;
	CHECK(strcmp(p -> name(), "UnionDef") == 0);

	p = _tc_EnumDef;
	CHECK(strcmp(p -> name(), "EnumDef") == 0);

	p = _tc_AliasDef;
	CHECK(strcmp(p -> name(), "AliasDef") == 0);

	p = _tc_PrimitiveDef;
	CHECK(strcmp(p -> name(), "PrimitiveDef") == 0);

	p = _tc_StringDef;
	CHECK(strcmp(p -> name(), "StringDef") == 0);

	p = _tc_SequenceDef;
	CHECK(strcmp(p -> name(), "SequenceDef") == 0);

	p = _tc_ArrayDef;
	CHECK(strcmp(p -> name(), "ArrayDef") == 0);

	p = _tc_ExceptionDef;
	CHECK(strcmp(p -> name(), "ExceptionDef") == 0);

	p = _tc_ExceptionDescription;
	CHECK(strcmp(p -> name(), "ExceptionDescription") == 0);

	p = _tc_AttributeMode;
	CHECK(strcmp(p -> name(), "AttributeMode") == 0);

	p = _tc_AttributeDef;
	CHECK(strcmp(p -> name(), "AttributeDef") == 0);

	p = _tc_AttributeDescription;
	CHECK(strcmp(p -> name(), "AttributeDescription") == 0);

	p = _tc_OperationMode;
	CHECK(strcmp(p -> name(), "OperationMode") == 0);

	p = _tc_ParameterMode;
	CHECK(strcmp(p -> name(), "ParameterMode") == 0);

	p = _tc_ParameterDescription;
	CHECK(strcmp(p -> name(), "ParameterDescription") == 0);

	p = _tc_ParDescriptionSeq;
	CHECK(strcmp(p -> name(), "ParDescriptionSeq") == 0);

	p = _tc_ContextIdentifier;
	CHECK(strcmp(p -> name(), "ContextIdentifier") == 0);

	p = _tc_ContextIdSeq;
	CHECK(strcmp(p -> name(), "ContextIdSeq") == 0);

	p = _tc_ExceptionDefSeq;
	CHECK(strcmp(p -> name(), "ExceptionDefSeq") == 0);

	p = _tc_ExcDescriptionSeq;
	CHECK(strcmp(p -> name(), "ExcDescriptionSeq") == 0);

	p = _tc_OperationDef;
	CHECK(strcmp(p -> name(), "OperationDef") == 0);

	p = _tc_OperationDescription;
	CHECK(strcmp(p -> name(), "OperationDescription") == 0);

	p = _tc_RepositoryIdSeq;
	CHECK(strcmp(p -> name(), "RepositoryIdSeq") == 0);

	p = _tc_OpDescriptionSeq;
	CHECK(strcmp(p -> name(), "OpDescriptionSeq") == 0);

	p = _tc_AttrDescriptionSeq;
	CHECK(strcmp(p -> name(), "AttrDescriptionSeq") == 0);

	p = _tc_InterfaceDef;
	CHECK(strcmp(p -> name(), "InterfaceDef") == 0);

	p = InterfaceDef::_tc_FullInterfaceDescription;
	CHECK(strcmp(p -> name(), "FullInterfaceDescription") == 0);

	p = _tc_InterfaceDescription;
	CHECK(strcmp(p -> name(), "InterfaceDescription") == 0);
    }
}

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        //
        // Create ORB
        //
        orb = ORB_init(argc, argv);

	//
	// Run the test
	//
        cout << "Testing TypeCode type... " << flush;
        TestTypeCode();
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
#endif
        status = EXIT_SUCCESS;
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

