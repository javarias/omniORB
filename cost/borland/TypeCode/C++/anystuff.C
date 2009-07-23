// anystuff.C

/* 
   This module provides two functions:
   1) Init fills an any object with data
   2) Equal compares two any objects
*/

#include "corba.h"
#include <assert.h>
USE_STD_NS

static CORBA::TypeCode*
GetTypeCode()
{
  const int count = 4;
  static CORBA::TypeCode* typeCodes[count] = 
  {
    CORBA::_tc_short,
    CORBA::_tc_float,
    CORBA::_tc_string,
    CORBA::_tc_any
  };
  static int index = 0;
  return typeCodes[index++ % count];
}

static const char*
GetString()
{
  const int count = 4;
  static const char* strings[count] = { "this", "is", "a", "test" };
  static int index = 0;
  return strings[index++ % count];
}
  
CORBA::Any
Init(CORBA::TypeCode* typeCode);

CORBA::Any
RecursiveInit(CORBA::TypeCode* typeCode, 
	      LIST(CORBA::TypeCode*)& list)
{
  CORBA::Any result;

  static CORBA::Short    theShort    = 100;
  static CORBA::Long     theLong     = 200;
  static CORBA::UShort   theUShort   = 300;
  static CORBA::ULong    theULong    = 400;
  static CORBA::Float    theFloat    = 500;
  static CORBA::Double   theDouble   = 600;
  static CORBA::Boolean  theBoolean  = 1;
  static CORBA::Char     theChar     = 'a';
  static CORBA::Octet    theOctet    = 0;
  static CORBA::ULong    theEnum     = 10;
  static CORBA::ULong    theUnion    = 20;
  static CORBA::ULong    theSequence = 30;

  switch(typeCode->kind())
  {
  case CORBA::tk_null:
  case CORBA::tk_void:
    assert(!"Invalid type kind");
    break;
  case CORBA::tk_short:
    result <<= theShort++;
    break;
  case CORBA::tk_long:
    result <<= theLong++;
    break;
  case CORBA::tk_ushort:
    result <<= theUShort++;
    break;
  case CORBA::tk_ulong:
    result <<= theULong++;
    break;
  case CORBA::tk_float:
    result <<= theFloat;
    theFloat = theFloat + 1;
    break;
  case CORBA::tk_double:
    result <<= theDouble;
    theDouble = theDouble + 1;
    break;
  case CORBA::tk_boolean:
    result <<= CORBA::Any::from_boolean(theBoolean);
    theBoolean = !theBoolean;
    break;
  case CORBA::tk_char:
    result <<= CORBA::Any::from_char(theChar);
    theChar = theChar < 'z' ? theChar + 1 : 'a';
    break;
  case CORBA::tk_octet:
    result <<= CORBA::Any::from_octet(theOctet++);
    break;
  case CORBA::tk_any:
    {
      CORBA::Any any = Init(GetTypeCode());
      result <<= any;
    }
    break;
  case CORBA::tk_TypeCode:
    result.replace(CORBA::_tc_TypeCode, GetTypeCode());
    break;
  case CORBA::tk_Principal:
    assert(!"Invalid type kind");
    break;
  case CORBA::tk_objref:
    {
      CORBA::String_var repid = typeCode->id();
      CORBA::ORB_var orb(CORBA::ORB_init());
      CORBA::Object_var obj(orb->bind(repid));
      result <<= obj;
    }
    break;
  case CORBA::tk_struct:
  case CORBA::tk_except:
    {
      list.add(typeCode);
      CORBA::MarshalOutBuffer buf;
      if(typeCode->kind() == CORBA::tk_except)
      {
	CORBA::String_var id = typeCode->id();
	//operator << (buf, id);
	buf << id;  // cast op. for String_var didn't work
      }
      CORBA::ULong memberCount = typeCode->member_count();
      for(CORBA::ULong i = 0; i < memberCount; i++)
      {
	CORBA::TypeCode_var memberType = typeCode->member_type(i);
	RecursiveInit(memberType, list).write_value(buf);
      }
      result.replace(typeCode, buf);
      list.remove(typeCode);
    }
    break;
  case CORBA::tk_union:
    {
      list.add(typeCode);
      CORBA::MarshalOutBuffer buf;
      CORBA::ULong memberCount = typeCode->member_count();
      CORBA::ULong disc = theUnion++ % memberCount;
      CORBA::Any_var memberLabel = typeCode->member_label(disc);
      // check if the memberLabel is tk_octet which means it is the default.
      // reset 'theUnion' to 0, defaults are hard to test...
      CORBA::TypeCode_var memberLabelType(memberLabel->type());
      CORBA::TypeCode_var memberType;
      memberType = typeCode->member_type(disc);
      if (memberLabelType->equal(CORBA::_tc_octet)) {
        theUnion = 0;
        memberLabel = typeCode->member_label(0);
        memberType = typeCode->member_type(0);
      }
      memberLabel->write_value(buf);
 
      Init(memberType).write_value(buf);
      result.replace(typeCode, buf);
      list.remove(typeCode);
    }
    break;
  case CORBA::tk_enum:
    {
      CORBA::ULong enumCount = typeCode->member_count();
      CORBA::ULong *enumValue = CORBA::ULong_alloc();
      *enumValue = CORBA::ULong(theEnum++ % enumCount);
      result.replace(typeCode, enumValue, 1);
    }
    break;
  case CORBA::tk_string:
    result <<= GetString();
    break;
  case CORBA::tk_sequence:
    {
      CORBA::MarshalOutBuffer buf;
      CORBA::ULong length = typeCode->length();
      if(!length) {
	length = theSequence++ % 3 + 2;
      }
      CORBA::TypeCode_var contentType = typeCode->content_type();

      for (CORBA::ULong numtcs=0; numtcs < list.size(); numtcs++) {
	if (contentType->equal(list[numtcs])) {
	  length = 0;
	  break;
	}
      }
      buf << length;
      for(CORBA::ULong i = 0; i < length; i++)
      {
	Init(contentType).write_value(buf);
      }
      result.replace(typeCode, buf);
    }
    break;
  case CORBA::tk_array:
    {
      CORBA::MarshalOutBuffer buf;
      CORBA::ULong length = typeCode->length();
      CORBA::TypeCode_var contentType = typeCode->content_type();
      for(CORBA::ULong i = 0; i < length; i++)
      {
	Init(contentType).write_value(buf);
      }
      result.replace(typeCode, buf);
    }
    break;
  case CORBA::tk_alias:
    {
      CORBA::MarshalOutBuffer buf;
      CORBA::TypeCode_var contentType = typeCode->content_type();
      Init(contentType).write_value(buf);
      result.replace(typeCode, buf);
    }
    break;
  default:
    assert(!"Invalid type kind");
    break;
  }
  return result;
}

CORBA::Boolean 
Equal(CORBA::Any& lhs, CORBA::Any& rhs)
{
  CORBA::TypeCode_var lhsType = lhs.type();
  CORBA::TypeCode_var rhsType = rhs.type();
  if (!lhsType->equal(rhsType))
    return 0;
  
  CORBA::MarshalOutBuffer lhsBuf;
  CORBA::MarshalOutBuffer rhsBuf;
  lhsBuf.release_flag(1UL);
  rhsBuf.release_flag(1UL);

  lhs.write_value(lhsBuf);
  rhs.write_value(rhsBuf);
  return lhsBuf.curoff() == rhsBuf.curoff() && 
    !memcmp(lhsBuf.buffer(), rhsBuf.buffer(), size_t(lhsBuf.curoff()));
}

CORBA::Any
Init(CORBA::TypeCode* typeCode)
{
  LIST(CORBA::TypeCode*) tcs;

  return RecursiveInit(typeCode, tcs);
  for (CORBA::ULong i=0; i < tcs.size(); i++) {
    CORBA::release(tcs[i]);
  }
}
