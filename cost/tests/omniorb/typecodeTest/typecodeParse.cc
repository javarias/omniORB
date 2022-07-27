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
// typecodeParse.cc
//

// TypeCode Parser

#include <iostream>
#include <stdlib.h>

#include <common/omnitest.h>

#include "typecodeParse.h"

using namespace std;


void typecodeParse::parseCode(CORBA::TypeCode_ptr iTC,CORBA::TypeCode_ptr parentTC)
{
  try
    {
      CORBA::ULong temp = iTC->kind();
    }
  catch(...)
    {
      cerr << "General exception when getting TypeCode kind." << endl;
      exit(-1);
    }


  switch(iTC->kind())
    {
    case CORBA::tk_null:
      cout << "tk_null" << endl;
      break;

    case CORBA::tk_void:
      cout << "tk_void" << endl;
      break;
     
    case CORBA::tk_short:
      cout << "tk_short" << endl;
      break;

    case CORBA::tk_long:
      cout << "tk_long" << endl;
      break;

    case CORBA::tk_ushort:
      cout << "tk_ushort" << endl;
      break;

    case CORBA::tk_ulong:
      cout << "tk_ulong" << endl;
      break;

    case CORBA::tk_float:
      cout << "tk_float" << endl;
      break;

    case CORBA::tk_double:
      cout << "tk_double" << endl;
      break;

    case CORBA::tk_boolean:
      cout << "tk_boolean" << endl;
      break;

    case CORBA::tk_char:
      cout << "tk_char" << endl;
      break;

    case CORBA::tk_octet:
      cout << "tk_octet" << endl;
      break;

    case CORBA::tk_any:
      cout << "tk_any" << endl;
      break;

    case CORBA::tk_TypeCode:
      cout << "tk_TypeCode" << endl;
      break;

    case CORBA::tk_Principal:
      cout << "tk_Principal" << endl;
      break;

    case CORBA::tk_objref:
      cout << "tk_objref" << endl;

      try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID."
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Object Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Object Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting object name." 
	       << endl;
	  exit(-1);
	}

      break;
      
    case CORBA::tk_struct:
      cout << "  tk_struct" << endl;

      try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID." 
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Struct Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Struct Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting struct name." 
	       << endl;
	  exit(-1);
	}

      parseMembers(iTC);

      break;

    case CORBA::tk_union:
      cout << "  tk_union" << endl;

      try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID." 
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Union Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Union Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting union name." 
	       << endl;
	  exit(-1);
	}

      
      try
	{
	  cout << "  Discriminator Type: " << endl;
	  CORBA::TypeCode_ptr memTC = iTC->discriminator_type();

	  if (!CORBA::is_nil(parentTC))
	    {
	      if (memTC->equal(parentTC))
		{
		  cout << "[Parent Type]" << endl;
		}
	      else typecodeParse::parseCode(memTC);
	    }
	  else typecodeParse::parseCode(memTC);

	  CORBA::release(memTC);
	  cout << "\n";
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << 
	    "  <BAD KIND EXCEPTION RAISED> when reading discriminator type." 
	       << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when reading discriminator type." 
	       << endl;
	  exit(-1);
	}

      cout << "Default index: " << iTC->default_index() << endl;
      parseMembers(iTC);

      break;

    case CORBA::tk_enum:


      cout << "  tk_enum" << endl;

      try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID." 
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Enum Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Enum Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting enum name." 
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Number of members: "  << iTC->member_count() << endl;
	  cout << "  Member Names: " << endl;

	  for(CORBA::ULong i = 0; i < iTC->member_count(); i++)
	    {
	      cout << "   " << i << ". ";

	      try
		{
		  cout << (const char*) iTC->member_name(i) << endl;
		}
	      catch(CORBA::TypeCode::BadKind& BKexcept)
		{
		  cerr << "   <BAD KIND EXCEPTION RAISED> when reading "
		       << "member_name()" << endl;

		}
	      catch(CORBA::TypeCode::Bounds& Bndsexcept)
		{
		  cerr << "   <BOUNDS EXCEPTION RAISED> when reading "
		       << "member_name()"<< endl;
		}
	      catch(...)
		{
		  cerr << "   General Exception Raised when reading "
		       << "member_name() " << endl;
		  exit(-1);
		}
	    }
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Number of members: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting number of members." 
	       << endl;
	  exit(-1);
	}
	
      break;


    case CORBA::tk_string:

      cout << "  tk_string" << endl;
      
      try
	{
	  cout << "  Maximum length: " << iTC->length() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Maximum length: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when getting maximum length." 
	       << endl;
	  exit(-1);
	}
      
      break;

      
    case CORBA::tk_sequence:

      cout << "  tk_sequence" << endl;


      try
	{
	  cout << "  Maximum length: " << iTC->length() << endl;
	  if (iTC->length() == 0) cout << "Unbounded Sequence." << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Maximum length: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when getting maximum length." 
	       << endl;
	  exit(-1);
	}

      cout << "  Content Type:" << endl;

      try
	{
	  CORBA::TypeCode_ptr memTC = iTC->content_type();

	  if (!CORBA::is_nil(parentTC))
	    {
	      if (memTC->equal(parentTC))
		{
		  cout << "[Parent Type]" << endl;
		}
	      else typecodeParse::parseCode(memTC);
	    }
	  else typecodeParse::parseCode(memTC);
	  CORBA::release(memTC);
	  cout << "\n";
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  <BAD KIND EXCEPTION RAISED> when reading content type." 
	       << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when reading content type." 
	       << endl;
	  exit(-1);
	}

      break;

    case CORBA::tk_array:

      cout << "  tk_array" << endl;

      try
	{
	  cout << "  Maximum length: " << iTC->length() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Maximum length: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when getting maximum length." 
	       << endl;
	  exit(-1);
	}

      cout << "  Content Type:" << endl;

      try
	{
	  CORBA::TypeCode_ptr memTC = iTC->content_type();

	  if (!CORBA::is_nil(parentTC))
	    {
	      if (memTC->equal(parentTC))
		{
		  cout << "[Parent Type]" << endl;
		}
	      else  typecodeParse::parseCode(memTC);
	    }
	  else typecodeParse::parseCode(memTC);
	  CORBA::release(memTC);

	  cout << "\n";
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  <BAD KIND EXCEPTION RAISED> when reading content type." 
	       << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when reading content type." 
	       << endl;
	  exit(-1);
	}

      break;

    case CORBA::tk_alias:

      cout << "  tk_alias" << endl;

      try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID."
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Alias Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Alias Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting alias name." 
	       << endl;
	  exit(-1);
	}
      

      cout << "  Content Type (alias to):" << endl;

      try
	{
	  CORBA::TypeCode_ptr memTC = iTC->content_type();

	  if (memTC->equal(iTC)) 
	    {
	      cout << "    Member typecode (same type as parent) :" << endl;
	      typecodeParse::parseCode(memTC);
	    }
	  else  typecodeParse::parseCode(memTC);
	  CORBA::release(memTC);
	  cout << "\n";
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  <BAD KIND EXCEPTION RAISED> when reading content type." 
	       << endl;
	}
      catch(...)
	{
	  cerr <<"  General Exception Raised when reading content type." 
	       << endl;
	  exit(-1);
	}

      break;

    case CORBA::tk_except:
      
      cout << "  tk_except" << endl;

            try
	{
	  cout << "  Repository Id: " << (const char*)  iTC->id() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Repository Id: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting repository ID."
	       << endl;
	  exit(-1);
	}

      try
	{
	  cout << "  Exception Name: " << (const char*)  iTC->name() << endl;
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "  Exception Name: <BAD KIND EXCEPTION RAISED>" << endl;
	}
      catch(...)
	{
	  cerr << "  General Exception Raised when getting exception name." 
	       << endl;
	  exit(-1);
	}

      parseMembers(iTC);
      
      break;

    default:

      cerr << "TypeCode not known (Number = " << (CORBA::ULong) iTC->kind()
	   << " )" << endl;
      
    }	  

}


void typecodeParse::parseMembers(CORBA::TypeCode_ptr iTC)
{

  try
    {
      cout << "  Number of members: "  << iTC->member_count() << endl;
    }
  catch(CORBA::TypeCode::BadKind& BKexcept)
    {
      cerr << "  Number of members: <BAD KIND EXCEPTION RAISED>" << endl;
      return;
    }
  catch(...)
    {
      cerr << "  General Exception Raised when getting number of members." 
	   << endl;
      exit(-1);
    }


  for(CORBA::ULong i = 0; i < iTC->member_count(); i++)
    {
      cout << "   Member index: " << i << "." << endl;
		    
      try
	{
	  cout << "   Member name: " << iTC->member_name(i) << "\n" << endl;

	  cout << "   Member typecode: " << endl;
	  CORBA::TypeCode_ptr memTC = iTC->member_type(i);

	  if (memTC->equal(iTC)) 
	    {
	      cout << "    Member typecode: [Same type as parent]" << endl;
	    }
	  else typecodeParse::parseCode(memTC,iTC);
	  CORBA::release(memTC);
	  cout << "\n";

	  if (iTC->kind() == CORBA::tk_union)
	    {
	      CORBA::Any* a;
	      a = iTC->member_label(i);
	      cout << "Union Label: " << endl;
	      CORBA::TypeCode_ptr aTC = a->type();
	      if (aTC->equal(iTC))
		{
		  cout << "    Label TypeCode: [Same type as parent]" << endl;
		}
	      else
		{	      
		  cout << "    Label TypeCode: " << endl;
		  typecodeParse::parseCode(aTC,iTC);
		}
	      CORBA::release(aTC);
	      cout << "\nLabel Value:" << endl;

	      CORBA::Char _ch;
	      CORBA::Boolean _boo;
	      CORBA::Octet _anoct;
	      CORBA::Short _sh;
	      CORBA::UShort _ush;
	      CORBA::Long _l;
	      CORBA::ULong _ul;
	      
	      if (*a >>= CORBA::Any::to_char(_ch))
		{
		  cout << "Char: " << _ch << endl;
		}
	      else if (*a >>= CORBA::Any::to_boolean(_boo))
		{
		  cout << "Boolean: " << _boo << endl;
		}
	      else if (*a >>= CORBA::Any::to_octet(_anoct))
		{
		  cout << "Octet: _" << (CORBA::UShort) _anoct << "_" << endl;
		}
	      else if (*a >>= _sh)
		{
		  cout << "Short: " << _sh << endl;
		}
	      else if (*a >>= _ush)
		{
		  cout << "UShort: " << _ush << endl;
		}
	      else if (*a >>= _l)
		{
		  cout << "Long: " << _l << endl;
		}
	      else if (*a >>= _ul)
		{
		  cout << "ULong: " << _ul << endl;
		}
	      else
		{
		  cout << "Unknown value." << endl;
		}

	      delete a;
	    }
	}
      catch(CORBA::TypeCode::BadKind& BKexcept)
	{
	  cerr << "   <BAD KIND EXCEPTION RAISED> when reading member_name() "
	       << "or member_type()." << endl;
	}
      catch(CORBA::TypeCode::Bounds& Bndsexcept)
	{
	  cerr << "   <BOUNDS EXCEPTION RAISED> when reading member_name() "
	       << "or member_type()." << endl;
	}
      catch(...)
	{
	  cerr << "   General Exception Raised when reading member_name() "
	       << "or member_type()." << endl;
	  exit(-1);
	}
    }
}
