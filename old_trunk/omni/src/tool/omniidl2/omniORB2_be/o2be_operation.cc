// -*- Mode: C++; -*-
//                          Package   : omniidl2
// o2be_operation.cc        Created on: 23/08/1996
//                          Author    : Sai-Lai Lo (sll)
//
//    Copyright (C) 1996-1999 AT&T Laboratories Cambridge
//
//  This file is part of omniidl2.
//
//  Omniidl2 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// Description:
//
//

/*
  $Log$
  Revision 1.34.2.1  1999/09/20 10:31:42  sll
  MS VC++ 5.0 does not pick up the const char* conversion operator of a
  StringBuf automagically when passed to an iostream. Have to do an explicit
  casting.

  Revision 1.34  1999/09/15 10:30:01  djr
  produce_invoke() did not pass ctxt argument if operation had no
  other arguments.

  Revision 1.33  1999/08/20 11:39:10  djr
  Removed debug output (left in by mistake!).

  Revision 1.32  1999/08/09 12:27:15  sll
  Removed trace message.

  Revision 1.31  1999/07/23 11:27:28  djr
  Implemented efficient marshalling/unmarshalling of array of basic types.

  Revision 1.30  1999/06/28 13:24:25  dpg1
  LifeCycle code updated for proxyCallWrapper and Context support.

  Revision 1.29  1999/06/18 20:47:28  sll
  Updated to use _dyn_attr.

  Revision 1.28  1999/05/26 11:05:24  sll
  Added code to support context.
  Added code to generate typecode constant for anonymous bounded string
  used as arguments.

  Revision 1.27  1999/03/11 16:26:08  djr
  Updated copyright notice

  Revision 1.26  1999/02/10 13:52:19  djr
  Replaced uses of an interface foo with foo_Helper, so that the stubs
  will compile even if the interface is not defined anywhere in the
  translation unit.

  Revision 1.25  1999/01/07 14:12:43  djr
  New implementation of proxy calls to reduce the code size overhead.

  Revision 1.24  1998/10/06 15:15:48  sll
  Removed check for response expected flag in _sk* dispatch function when
  the operation is a request-reply.

  Revision 1.23  1998/08/13 22:43:51  sll
  Added pragma hdrstop to control pre-compile header if the compiler feature
  is available.

  Revision 1.22  1998/04/07 18:49:34  sll
  Stub code modified to accommodate the use of namespace to represent module.
  Use std::fstream instead of fstream.

  Revision 1.21  1998/03/25 14:40:11  sll
  *** empty log message ***

  Revision 1.20  1998/03/25 14:18:57  sll
  Temporary work-around for egcs compiler.

  Revision 1.19  1998/01/27 16:47:25  ewc
  Added support for type Any and TypeCode

// Revision 1.18  1998/01/21  12:14:16  sll
// Now accepts null pointer as marshalling argument. Substituted with a
// proper nil string.  Print a warning if traceLevel > 1.
// Now unmarshal zero size string. Substituted with a proper nil string.
// Print a warning if traceLevel > 1.
//
  Revision 1.17  1997/12/23 19:30:37  sll
  Now generate correct stub for multi-dimensional array of object references.

  Revision 1.16  1997/12/10 11:35:30  sll
  Updated life cycle service stub.

  Revision 1.15  1997/12/09 19:54:13  sll
  *** empty log message ***

// Revision 1.14  1997/09/20  16:46:48  dpg1
// Added LifeCycle code generation.
//
  Revision 1.13  1997/08/22 12:42:20  sll
  Oh well, gcc does not like variable names starting with __, changed
  the prefix to _0RL_.

  Revision 1.12  1997/08/21 21:13:09  sll
  Names of internal variables inside the stub code now all start with the
  prefix __ to avoid potential clash with identifiers defined in IDL.

// Revision 1.11  1997/05/06  14:03:08  sll
// Public release.
//
  */

#include <idl.hh>
#include <idl_extern.hh>
#include <o2be.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <o2be_stringbuf.h>


o2be_operation::o2be_operation(AST_Type* rt, AST_Operation::Flags fl,
			       UTL_ScopedName* n, UTL_StrList* p)
                : AST_Operation(rt, fl, n, p),
		  AST_Decl(AST_Decl::NT_op, n, p),
		  UTL_Scope(AST_Decl::NT_op),
		  o2be_name(AST_Decl::NT_op,n,p)
{
  o2be_interface* intf = o2be_interface::narrow_from_scope(defined_in());
  if (o2be_interface::check_opname_clash(intf,uqname())) {
    idl_global->err()->operation_name_clash(this);
  }
  if (flags() == AST_Operation::OP_oneway) {
    if (!no_user_exception()) {
      idl_global->err()->syntax_error(idl_global->parse_state());
    }
  }

  pd_mangled_signature = 0;
}


void
o2be_operation::produce_decl(std::fstream &s, const char* prefix,
			     const char* alias_prefix, idl_bool/* ignored */,
			     idl_bool use_fully_qualified_names)
{
  // return type
  if (!return_is_void())
    {
      argMapping mapping;
      argType    ntype;

      ntype = ast2ArgMapping(return_type(),wResult,mapping);

      if (ntype == tObjref) {
	AST_Decl* decl = return_type();
	while (decl->node_type() == AST_Decl::NT_typedef) {
	  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	}
	s << o2be_interface::narrow_from_decl(decl)
	  ->unambiguous_objref_name(this, use_fully_qualified_names);
      }
      else if (ntype == tString) {
	s << "char*";
      }
      else if (ntype == tTypeCode) {
	AST_Decl* decl = return_type();
	while (decl->node_type() == AST_Decl::NT_typedef) {
	  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	}
	s << o2be_name::narrow_and_produce_unambiguous_name(decl,
						    this,
						    use_fully_qualified_names);
      }
      else {
	s << o2be_name::narrow_and_produce_unambiguous_name(return_type(),
						    this,
						    use_fully_qualified_names);
      }
      s << ((mapping.is_arrayslice) ? "_slice":"")
	<< " "
	<< ((mapping.is_pointer)    ? "*":"")
	<< ((mapping.is_reference)  ? "&":"");
    }
  else
    {
      s << "void";
    }
  // function name
  s << " ";
  if (prefix)
    s << prefix << "::";
  s << ((alias_prefix)? alias_prefix : "");
  s << uqname() << "(";

  // argument list
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argMapping mapping;
	argType    ntype;

	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	switch(a->direction())
	  {
	  case AST_Argument::dir_IN:
	    ntype = ast2ArgMapping(a->field_type(),wIN,mapping);
	    break;
	  case AST_Argument::dir_OUT:
	    ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	    break;
	  case AST_Argument::dir_INOUT:
	    ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	    break;
	  }
	s << ((mapping.is_const) ? "const ":"");
	if (ntype == tObjref) {
	  AST_Decl* decl = a->field_type();
	  while (decl->node_type() == AST_Decl::NT_typedef) {
	    decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	  }
	  s << o2be_interface::narrow_from_decl(decl)
	    ->unambiguous_objref_name(this, use_fully_qualified_names);
	}
	else if (ntype == tString) {
	  s << "char*";
	}
	else if (ntype == tTypeCode) {
	  AST_Decl *decl = a->field_type();
	  while (decl->node_type() == AST_Decl::NT_typedef) {
	    decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	  }
	  s << o2be_name::narrow_and_produce_unambiguous_name(decl, this,
					      use_fully_qualified_names);
	}
	else {
	  s << o2be_name::narrow_and_produce_unambiguous_name(a->field_type(),
					      this, use_fully_qualified_names);
	}
	s << ((mapping.is_arrayslice) ? "_slice":"")
	  << " "
	  << ((mapping.is_pointer)    ? "*":"")
	  << ((mapping.is_reference)  ? "&":"");
	s << " " << a->uqname();
	i.next();
	s << ((!i.is_done()) ? ", " : (context()?",":""));
      }
  }

  // context
  {
    if (context()) 
    {
      s << "CORBA::Context_ptr ctxt";
    }
  }

  s << ")";
}


void
o2be_operation::produce_invoke(std::fstream &s)
{
  s << uqname() << '(';

  UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
  int first = 1;

  while( !i.is_done() ) {
    o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
    s << (first ? "":", ") << a->uqname();
    first = 0;
    i.next();
  }

  if( context() )   s << (first ? "ctxt" : ", ctxt");
  s << ')';
}


void
o2be_operation::produce_arg_decl(std::fstream& s, o2be_argument* arg,
				 AST_Decl* used_in,
				 const char* argname,
				 const char* argnameprefix)
{
  argMapping mapping;
  argType    ntype;

  switch( arg->direction() ) {
  case AST_Argument::dir_IN:
    ntype = ast2ArgMapping(arg->field_type(), wIN, mapping);
    break;
  case AST_Argument::dir_OUT:
    ntype = ast2ArgMapping(arg->field_type(), wOUT, mapping);
    break;
  case AST_Argument::dir_INOUT:
    ntype = ast2ArgMapping(arg->field_type(), wINOUT, mapping);
    break;
  }
  if( mapping.is_const )  s << "const ";
  switch( ntype ) {
  case tObjref:
    {
      AST_Decl* decl = arg->field_type();
      while( decl->node_type() == AST_Decl::NT_typedef )
	decl = o2be_typedef::narrow_from_decl(decl)->base_type();
      s << o2be_interface::narrow_from_decl(decl)
	->unambiguous_objref_name(used_in, I_TRUE);
      break;
    }
  case tString:
    s << "char*";
    break;
  case tTypeCode:
    {
      AST_Decl* decl = arg->field_type();
      while( decl->node_type() == AST_Decl::NT_typedef ) {
	decl = o2be_typedef::narrow_from_decl(decl)->base_type();
      }
      s << o2be_name::narrow_and_produce_unambiguous_name(decl, used_in,
							  I_TRUE);
      break;
    }
  case tArrayFixed:
  case tArrayVariable:
    // We use pointer to slice for passing arrays to call descriptors ...
    mapping.is_arrayslice = I_TRUE;
    mapping.is_pointer = I_TRUE;
    s << o2be_name::narrow_and_produce_unambiguous_name(arg->field_type(),
							used_in, I_TRUE);
    break;
  default:
    s << o2be_name::narrow_and_produce_unambiguous_name(arg->field_type(),
							used_in, I_TRUE);
    break;
  }
  if( mapping.is_arrayslice )  s << "_slice";
  if( mapping.is_pointer    )  s << "*";
  if( mapping.is_reference  )  s << "&";
  if( argname )  s << ' ' << (argnameprefix ? argnameprefix : "") << argname;
}


void
o2be_operation::produce_return_decl(std::fstream& s, AST_Decl* used_in,
				    const char* varname,
				    const char* varnameprefix)
{
  argMapping mapping;
  argType    ntype;
  AST_Decl* decl = return_type();

  ntype = ast2ArgMapping(decl, wResult, mapping);

  switch( ntype ) {
  case tObjref:
    while( decl->node_type() == AST_Decl::NT_typedef )
      decl = o2be_typedef::narrow_from_decl(decl)->base_type();
    s << o2be_interface::narrow_from_decl(decl)
      ->unambiguous_objref_name(used_in, I_TRUE);
    break;
  case tString:
    s << "char*";
    break;
  case tTypeCode:
    while( decl->node_type() == AST_Decl::NT_typedef )
      decl = o2be_typedef::narrow_from_decl(decl)->base_type();
    s << o2be_name::narrow_and_produce_unambiguous_name(decl, used_in, I_TRUE);
    break;
  default:
    s << o2be_name::narrow_and_produce_unambiguous_name(decl, used_in, I_TRUE);
    break;
  }
  s << (mapping.is_arrayslice ? "_slice" : "")
    << (mapping.is_pointer    ? "*" : "")
    << (mapping.is_reference  ? "&" : "");
  if( varname )
    s << ' ' << (varnameprefix ? varnameprefix : "") << varname;
}


void
o2be_operation::produce_proxy_call_desc(std::fstream& s,
					const char* class_name)
{
  const char* call_desc_base_class;
  if (!context()) {
    if( flags() == AST_Operation::OP_oneway )
      call_desc_base_class = "OmniOWProxyCallDesc";
    else
      call_desc_base_class = "OmniProxyCallDesc";
  }
  else {
    if( flags() == AST_Operation::OP_oneway )
      call_desc_base_class = "OmniOWProxyCallDescWithContext";
    else
      call_desc_base_class = "OmniProxyCallDescWithContext";
  }

  IND(s); s << "// Proxy call descriptor class. Mangled signature:\n";
  IND(s); s << "//  " << mangled_signature() << '\n';
  IND(s); s << "class " << class_name << '\n';
  IND(s); s << "  : public " << call_desc_base_class << '\n';
  IND(s); s << "{\n";
  IND(s); s << "public:\n";
  INC_INDENT_LEVEL();

  unsigned num_arguments = 0;

  // Constructor.
  IND(s); s << "inline " << class_name << "(const char* _op, size_t _op_len";
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while( !i.is_done() ) {
      num_arguments++;
      o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
      s << ", ";
      produce_arg_decl(s, a, o2be_global::root(), a->uqname(), "_a_");
      i.next();
    }
  }
  s << ") :\n";
  INC_INDENT_LEVEL();
  IND(s); s << call_desc_base_class << "(_op, _op_len";
  if( no_user_exception() )  s << ")";
  else                       s << ", 1)";
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while( !i.is_done() ) {
      o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
      s << ",\n";
      IND(s); s << "arg_" << a->uqname() << "(_a_" << a->uqname() << ")";
      i.next();
    }
  }
  s << "  {}\n\n";
  DEC_INDENT_LEVEL();

  // Declaration of methods to implement the call.
  if( has_any_in_args() || has_any_inout_args() ) {
    IND(s); s << "virtual CORBA::ULong alignedSize(CORBA::ULong size_in);\n";
    IND(s); s << "virtual void marshalArguments(GIOP_C&);\n";
  }
  if( has_any_inout_args() || has_any_out_args() || !return_is_void() ) {
    IND(s); s << "virtual void unmarshalReturnedValues(GIOP_C&);\n";
  }
  if( !no_user_exception() ) {
    IND(s); s << "virtual void userException(GIOP_C&, const char*);\n";
  }

  // Result accessor.
  if( !return_is_void() ) {
    IND(s); s << "inline ";
    produce_return_decl(s, o2be_global::root(), 0);
    s << " result() { return pd_result; }\n";
  }

  s << "\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "private:\n";
  INC_INDENT_LEVEL();

  // Private data members - to store the arguments and return value.
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while( !i.is_done() ) {
      o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
      IND(s);
      produce_arg_decl(s, a, o2be_global::root(), a->uqname(), "arg_");
      s << ";\n";
      i.next();
    }
  }
  if( !return_is_void() ) {
    IND(s);
    produce_return_decl(s, o2be_global::root(), "pd_result");
    s << ";\n";
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  if( has_any_in_args() || has_any_inout_args() ) {
    // Method to calculate the size of the arguments.

    IND(s); s << "CORBA::ULong " << class_name
	      << "::alignedSize(CORBA::ULong msgsize)\n";
    IND(s); s << "{\n";
    INC_INDENT_LEVEL();
    {
      UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
      while( !i.is_done() ) {
	argMapping mapping;
	argType ntype;

	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	char* argname = new char[1 + strlen(a->uqname()) + 4];
	strcpy(argname, "arg_");
	strcat(argname, a->uqname());

	switch( a->direction() ) {
	case AST_Argument::dir_IN:
	  ntype = ast2ArgMapping(a->field_type(), wIN, mapping);
	  produceSizeCalculation(s, a->field_type(), o2be_global::root(),
				 "giop_client", "msgsize",
				 argname, ntype, mapping);
	  break;
	case AST_Argument::dir_INOUT:
	  ntype = ast2ArgMapping(a->field_type(), wINOUT, mapping);
	  produceSizeCalculation(s, a->field_type(), o2be_global::root(),
				 "giop_client", "msgsize",
				 argname, ntype, mapping);
	  break;
	case AST_Argument::dir_OUT:
	  break;
	}

	delete[] argname;
	i.next();
      }
    }
    IND(s); s << "return msgsize;\n";
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n\n";

    // Method to marshal the arguments onto the stream.
    IND(s); s << "void " << class_name
	      << "::marshalArguments(GIOP_C& giop_client)\n";
    IND(s); s << "{\n";
    INC_INDENT_LEVEL();
    {
      UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
      while( !i.is_done() ) {
	argMapping mapping;
	argType ntype;

	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	char* argname = new char[1 + strlen(a->uqname()) + 4];
	strcpy(argname, "arg_");
	strcat(argname, a->uqname());

	switch( a->direction() ) {
	case AST_Argument::dir_IN:
	  ntype = ast2ArgMapping(a->field_type(), wIN, mapping);
	  produceMarshalCode(s, a->field_type(),
			     o2be_global::root(), "giop_client",
			     argname, ntype, mapping);
	  break;
	case AST_Argument::dir_INOUT:
	  ntype = ast2ArgMapping(a->field_type(), wINOUT, mapping);
	  produceMarshalCode(s, a->field_type(),
			     o2be_global::root(), "giop_client",
			     argname, ntype, mapping);
	  break;
	case AST_Argument::dir_OUT:
	  break;
	}

	delete[] argname;
	i.next();
      }
    }
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n\n";
  }


  if( has_any_inout_args() || has_any_out_args() || !return_is_void() ) {
    // Method to unmarshal returned values from the stream.

    IND(s); s << "void " << class_name
	      << "::unmarshalReturnedValues(GIOP_C& giop_client)\n";
    IND(s); s << "{\n";
    INC_INDENT_LEVEL();
    // Declare temporary variables and allocate memory for variable
    // length arguments.

    //?? I think we should probably be using _var types here, so that
    // if unmarshalling fails at any point we will recover the
    // memory.
    {
      UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);

      while( !i.is_done() ) {
	argMapping mapping;
	argType ntype;

	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	i.next();
	if( a->direction() != AST_Argument::dir_OUT)  continue;
	ntype = ast2ArgMapping(a->field_type(), wOUT, mapping);

	if( mapping.is_arrayslice ) {
	  IND(s);
	  declareVarType(s, a->field_type(), o2be_global::root(), 0, I_TRUE);
	  s << "* _arg_" << a->uqname() << " = ";
	  AST_Decl* truetype = a->field_type();
	  while( truetype->node_type() == AST_Decl::NT_typedef )
	    truetype = o2be_typedef::narrow_from_decl(truetype)->base_type();
	  s << o2be_array::narrow_from_decl(truetype)->fqname()
	    << "_alloc();\n";
	}
	else if( mapping.is_reference && mapping.is_pointer ) {
	  IND(s);
	  declareVarType(s, a->field_type(), o2be_global::root());
	  s << "* _arg_" << a->uqname() << " = new ";
	  declareVarType(s, a->field_type(), o2be_global::root());
	  s << ";\n";
	}
	else if( ntype == tObjref || ntype == tString || ntype == tTypeCode ) {
	  IND(s);
	  declareVarType(s, a->field_type(), o2be_global::root());
	  s << " _arg_" << a->uqname() << " = ";
	  //?? Shouldn't we be doing something similar for obj refs?
	  if( ntype == tTypeCode )  s << "CORBA::TypeCode::_nil()";
	  else                      s << "0";
	  s << ";\n";
	}
      }
    }

    // Allocate memory for the return value.
    if( !return_is_void() ) {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(),wResult,mapping);

      if (mapping.is_arrayslice) {
	IND(s); s << "pd_result = ";
	AST_Decl* truetype = return_type();
	while( truetype->node_type() == AST_Decl::NT_typedef )
	  truetype = o2be_typedef::narrow_from_decl(truetype)->base_type();
	s << o2be_array::narrow_from_decl(truetype)->fqname()
	  << "_alloc();\n";
      }
      else if (mapping.is_pointer) {
	IND(s); s << "pd_result = new ";
	declareVarType(s, return_type(), o2be_global::root());
	s << ";\n";
      }
    }

    // Unmarshal the result.
    if( !return_is_void() ) {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(), wResult, mapping);
      produceUnMarshalCode(s, return_type(), o2be_global::root(),
			   "giop_client", "pd_result", ntype, mapping);
    }

    // Unmarshal inout/out arguments.
    {
      UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
      while (!i.is_done()) {
	argMapping mapping;
	argType ntype;

	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	char* argname = new char[strlen(a->uqname()) + 5];
	strcpy(argname, "arg_");
	strcat(argname, a->uqname());
	char* _argname = new char[strlen(argname) + 2];
	strcpy(_argname, "_");
	strcat(_argname, argname);

	//?? I am not convinced that all memory & references will be
	// properly cleaned up if we get exceptions at various points
	// in the following code ... (and the original version).

	switch( a->direction() ) {
	case AST_Argument::dir_INOUT:
	  {
	    ntype = ast2ArgMapping(a->field_type(), wINOUT, mapping);
	    switch (ntype) {
	    case tObjref:
	      {
		IND(s);
		AST_Decl* ft = a->field_type();
		while( ft->node_type() == AST_Decl::NT_typedef )
		  ft = o2be_typedef::narrow_from_decl(ft)->base_type();
		declareVarType(s, ft, o2be_global::root(), 0, 0);
		s << " " << _argname << ";\n";
		produceUnMarshalCode(s, ft, o2be_global::root(),
				     "giop_client", _argname,
				     ntype, mapping);
		// Must use the helper class here, not CORBA::release, as
		// if it is a forward declared interface we might not yet
		// know that it is derived from CORBA::Object.
		IND(s); s << o2be_interface::narrow_from_decl(ft)->fqname()
			  << "_Helper::release(" << argname << ");\n";
		IND(s); s << argname << " = " << _argname << ";\n";
		break;
	      }
	    case tString:
	      {
		IND(s);
		declareVarType(s,a->field_type(), o2be_global::root(), 0, 0);
		s << " " << _argname << ";\n";
		produceUnMarshalCode(s, a->field_type(),
				     o2be_global::root(),
				     "giop_client", _argname,
				     ntype, mapping);
		IND(s); s << "CORBA::string_free(" << argname << ");\n";
		IND(s); s << argname << " = " << _argname << ";\n";
		break;
	      }
	    case tTypeCode:
	      {
		IND(s);
		declareVarType(s, a->field_type(), o2be_global::root(), 1, 0);
		s << ' ' << _argname << ";\n";
		produceUnMarshalCode(s, a->field_type(),
				     o2be_global::root(),
				     "giop_client", _argname,
				     ntype, mapping);
		IND(s); s << "CORBA::release(" << argname << ");\n";
		IND(s); s << argname << " = " << _argname
			  << "._retn();\n";
		break;
	      }
	    default:
	      produceUnMarshalCode(s, a->field_type(),
				   o2be_global::root(),
				   "giop_client", argname,
				   ntype, mapping);
	      break;
	    }
	    break;
	  }
	case AST_Argument::dir_OUT:
	  {
	    ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	    if (ntype == tObjref || ntype == tString || ntype == tTypeCode ||
		(mapping.is_arrayslice) ||
		(mapping.is_reference && mapping.is_pointer))
	      {
		produceUnMarshalCode(s, a->field_type(),
				     o2be_global::root(),
				     "giop_client", _argname,
				     ntype, mapping);
	      }
	    else
	      {
		produceUnMarshalCode(s, a->field_type(),
				     o2be_global::root(),
				     "giop_client", argname,
				     ntype, mapping);
	      }
	    break;
	  }
	case AST_Argument::dir_IN:
	  break;
	}

	delete[] argname;
	delete[] _argname;
	i.next();
      }
    }

    // Hand over control of the allocated memory to the arguments.
    {
      UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
      while( !i.is_done() ) {
	argMapping mapping;
	argType ntype;

	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	i.next();
	if( a->direction() != AST_Argument::dir_OUT ) continue;

	ntype = ast2ArgMapping(a->field_type(), wOUT, mapping);
	if( ntype == tObjref || ntype == tString ||
	    ntype == tTypeCode  || (mapping.is_arrayslice) ||
	    (mapping.is_reference && mapping.is_pointer) ) {
	  IND(s); s << "arg_" << a->uqname() << " = _arg_"
		    << a->uqname() << ";\n";
	}
      }
    }

    DEC_INDENT_LEVEL();
    IND(s); s << "}\n\n";
  }


  if( !no_user_exception() ) {
    // Method to unmarshal a user exception.

    IND(s); s << "void " << class_name
	      << "::userException(GIOP_C& giop_client, const char* repoId)\n";
    IND(s); s << "{\n";
    INC_INDENT_LEVEL();
    {
      UTL_ExceptlistActiveIterator i(exceptions());
      idl_bool first = I_TRUE;

      while( !i.is_done() ) {
	o2be_exception* ex = o2be_exception::narrow_from_decl(i.item());
	IND(s); s << (first ? "if" : "else if")
		  << "( strcmp(repoId, "
		  << ex->repoIdConstName()
		  << ") == 0 ) {\n";
	INC_INDENT_LEVEL();
	IND(s); s << ex->fqname() << " _ex;\n";
	argType ntype = tStructVariable;
	argMapping mapping = {I_FALSE,I_TRUE,I_FALSE,I_FALSE};
	produceUnMarshalCode(s, i.item(), o2be_global::root(),
			     "giop_client", "_ex", ntype, mapping);
	IND(s); s << "giop_client.RequestCompleted();\n";
	IND(s); s << "throw _ex;\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
	i.next();
	first = I_FALSE;
      }
    }
    IND(s); s << "else {\n";
    INC_INDENT_LEVEL();
    IND(s); s << "giop_client.RequestCompleted(1);\n";
    IND(s); s << "throw CORBA::MARSHAL(0, CORBA::COMPLETED_MAYBE);\n";
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n";

    DEC_INDENT_LEVEL();
    IND(s); s << "}\n\n";
  }
}


void
o2be_operation::produce_proxy_skel(std::fstream& s, o2be_interface& def_in,
				   const char* alias_prefix)
{
  o2be_call_desc::produce_descriptor(s, *this);
  const char* call_desc_class = o2be_call_desc::descriptor_name(*this);

  char* ctxt_un = 0;
  int   ctxt_len = 0;
  if (context()) {
    ctxt_un = o2be_call_desc::generate_unique_name("_0RL_ctx_");
    IND(s); s << "static const char* " << ctxt_un << "[] = {\n";
    ctxt_len = 0;
    UTL_StrlistActiveIterator iter(context());

    while (!iter.is_done()) {
      String* p = iter.item();
      IND(s); s  << "\"" << p->get_string() << "\",\n";
      ctxt_len++;
      iter.next();
    }
    IND(s); s << " 0 };\n\n";
  }

  IND(s); produce_decl(s, def_in.proxy_fqname(), alias_prefix,
		       I_FALSE, I_TRUE);
  s << "\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << call_desc_class << " _call_desc(\""
	    << local_name()->get_string() << "\", "
	    << (strlen(local_name()->get_string()) + 1);
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while( !i.is_done() ) {
      o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
      s << ", " << a->uqname();
      i.next();
    }
  }
  s << ");\n\n";

  if (context()) {
    IND(s); s << "_call_desc.set_context(ctxt,"
      <<                                 ctxt_un  << ","
      <<                                 ctxt_len << ");\n\n";
  }

  if( flags() == AST_Operation::OP_oneway ) {
    IND(s); s << "OmniProxyCallWrapper::one_way(this, _call_desc);\n";
  } else {
    IND(s); s << "OmniProxyCallWrapper::invoke(this, _call_desc);\n";
  }
  if( !return_is_void() ) {
    IND(s); s << "return _call_desc.result();\n";
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";
}


void
o2be_operation::produce_server_skel(std::fstream &s,o2be_interface &def_in)
{
  if (flags() == AST_Operation::OP_oneway) {
    IND(s); s << "if (_0RL_response_expected) {\n";
    INC_INDENT_LEVEL();
    IND(s); s << "throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_NO);\n";
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n";
  }
  // Even if this is request reply, the caller may send a GIOP request
  // message with no responds specified in the header. Therefore the
  // stub code should not throw an exception.
  // XXX In fact, this end should just dump the result from the upcall.
  //     This is not done at the moment.
#if 0
  else {
    IND(s); s << "if (!_0RL_response_expected) {\n";
    INC_INDENT_LEVEL();
    IND(s); s << "throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_NO);\n";
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n";
  }
#endif
  {
    // unmarshall arguments
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argMapping mapping;
	argType ntype;

	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	IND(s);
	switch(a->direction())
	  {
	  case AST_Argument::dir_IN:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wIN,mapping);
	      if (ntype == tObjref || ntype == tString || ntype == tTypeCode)
		// declare a <type>_var variable to manage the pointer type
		declareVarType(s,a->field_type(),this,1);
	      else
		declareVarType(s,a->field_type(),this);
	      s << " " << a->uqname();
	      s << ";\n";

	      produceUnMarshalCode(s,a->field_type(),
				   (AST_Decl*)&def_in,
				   "_0RL_s",a->uqname(),
				   ntype,mapping);
	      break;
	    }
	  case AST_Argument::dir_OUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	      if (ntype == tObjref || ntype == tString || ntype == tTypeCode ||
		  (mapping.is_arrayslice) ||
		  (mapping.is_reference && mapping.is_pointer))
		{
		  // declare a <type>_var variable to manage the pointer type
		  declareVarType(s,a->field_type(),this,1,mapping.is_arrayslice);
		}
	      else
		declareVarType(s,a->field_type(),this);
	      s << " " << a->uqname();
	      s << ";\n";
	      break;
	    }
	  case AST_Argument::dir_INOUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	      if (ntype == tObjref || ntype == tString || ntype == tTypeCode)
		{
		  // declare a <type>_var variable to manage the pointer type
		  declareVarType(s,a->field_type(),this,1);
		}
	      else
		  declareVarType(s,a->field_type(),this);
	      s << " " << a->uqname();
	      s << ";\n";
	      produceUnMarshalCode(s,a->field_type(),
				   (AST_Decl*)&def_in,
				   "_0RL_s",a->uqname(),
				   ntype,mapping);
	      break;
	    }
	  }
	i.next();
      }
  }
  {
    if (context()) {
      IND(s); s << "CORBA::Context_var ctxt;\n";

      argMapping mapping;
      mapping.is_const = mapping.is_reference = mapping.is_arrayslice = 0;
      mapping.is_pointer = 1;
      produceUnMarshalCode(s,this,
			   (AST_Decl*)&def_in,
			   "_0RL_s","ctxt",
			   tContext,mapping);
    }
  }
  IND(s); s << "_0RL_s.RequestReceived();\n";

  IND(s);
  if (!return_is_void()) {
    argMapping mapping;
    argType ntype = ast2ArgMapping(return_type(),wResult,mapping);
    if (ntype == tObjref || ntype == tString || ntype == tTypeCode ||
	(mapping.is_arrayslice) ||
	(mapping.is_pointer))
      {
	// declare a <type>_var variable to manage the pointer type
	declareVarType(s,return_type(),this,1,mapping.is_arrayslice);
      }
    else
      {
	declareVarType(s,return_type(),this);
      }
    s << " _0RL_result";
    s << ";\n";
  }

  if (!no_user_exception()) {
    IND(s); s << "try {\n";
    INC_INDENT_LEVEL();
  }
  IND(s);
  if (!return_is_void()) {
    s << "_0RL_result = ";
    if (has_variable_out_arg() || has_pointer_inout_arg()) {
      // Use the indirection function in the base class
      s << def_in.uqname() << "::";
    }
    produce_invoke(s);
    s << ";\n";
  }
  else {
    if (has_variable_out_arg() || has_pointer_inout_arg()) {
      // Use the indirection function in the base class
      s << def_in.uqname() << "::";
    }
    produce_invoke(s);
    s << ";\n";
  }

  if (flags() == AST_Operation::OP_oneway) {
    IND(s); s << "_0RL_s.ReplyCompleted();\n";
    IND(s); s << "return 1;\n";
    return;
  }

  if (!no_user_exception()) {
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n";
    // catch and marshal each user exception
    UTL_ExceptlistActiveIterator i(exceptions());
    while (!i.is_done())
      {
	o2be_exception *excpt = o2be_exception::narrow_from_decl(i.item());
	IND(s); s << "catch ( " << excpt->fqname() << " &_0RL_ex) {\n";
	INC_INDENT_LEVEL();

	argType ntype = tStructVariable;
	argMapping mapping = {I_FALSE,I_TRUE,I_FALSE,I_FALSE};

	IND(s); s << "size_t _0RL_msgsize = (size_t) GIOP_S::ReplyHeaderSize();\n";

	produceConstStringSizeCalculation(s,"_0RL_msgsize",excpt->repoIdConstLen());
	produceSizeCalculation(s,i.item(),
			       (AST_Decl*)&def_in,
			       "_0RL_s","_0RL_msgsize","_0RL_ex",ntype,mapping);

	IND(s); s << "_0RL_s.InitialiseReply(GIOP::USER_EXCEPTION,(CORBA::ULong)_0RL_msgsize);\n";
	produceConstStringMarshalCode(s,"_0RL_s",excpt->repoIdConstName(),
				      excpt->repoIdConstLen());
	produceMarshalCode(s,i.item(),
			   (AST_Decl*)&def_in,
			   "_0RL_s","_0RL_ex",ntype,mapping);
	IND(s); s << "_0RL_s.ReplyCompleted();\n";
	IND(s); s << "return 1;\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
	i.next();
      }
  }

  // calculate reply message size
  IND(s); s << "size_t _0RL_msgsize = (size_t) GIOP_S::ReplyHeaderSize();\n";
  if (!return_is_void())
    {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(),wResult,mapping);
      if ((ntype == tObjref || ntype == tString || ntype == tTypeCode ||
	   mapping.is_pointer) && !mapping.is_arrayslice)
	{
	  // These are declared as <type>_var variable
	  if (ntype == tString) {
	    produceSizeCalculation(s,return_type(),
				   (AST_Decl*)&def_in,
				   "_0RL_s","_0RL_msgsize",
				   "_0RL_result",ntype,mapping);
	  }
	  else {
	    // use operator->() to get to the pointer
	    produceSizeCalculation(s,return_type(),
				   (AST_Decl*)&def_in,
				   "_0RL_s","_0RL_msgsize",
				   "(_0RL_result.operator->())",ntype,mapping);
	  }
	}
      else
	{
	  produceSizeCalculation(s,return_type(),
				 (AST_Decl*)&def_in,
				 "_0RL_s","_0RL_msgsize",
				 "_0RL_result",ntype,mapping);
	}

    }
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argMapping mapping;
	argType ntype;

	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	switch(a->direction())
	  {
	  case AST_Argument::dir_OUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	      if ((ntype == tObjref || ntype == tString ||
		   ntype == tTypeCode ||
		   (mapping.is_reference && mapping.is_pointer))
		  && !mapping.is_arrayslice)
		{
		  // These are declared as <type>_var variable
		  if (ntype == tString) {
		    produceSizeCalculation(s,a->field_type(),
					   (AST_Decl*)&def_in,
					   "_0RL_s","_0RL_msgsize",
					   a->uqname(),ntype,mapping);
		  }
		  else {
		    // use operator->() to get to the pointer
		    char *_argname = new char[strlen(a->uqname())+
					     strlen("(.operator->())")+1];
		    strcpy(_argname,"(");
		    strcat(_argname,a->uqname());
		    strcat(_argname,".operator->())");
		    produceSizeCalculation(s,a->field_type(),
					   (AST_Decl*)&def_in,
					   "_0RL_s","_0RL_msgsize",
					   _argname,ntype,mapping);
		    delete [] _argname;
		  }
		}
	      else
		{
		  produceSizeCalculation(s,a->field_type(),
					 (AST_Decl*)&def_in,
					 "_0RL_s","_0RL_msgsize",
					 a->uqname(),ntype,mapping);
		}
	      break;
	    }
	  case AST_Argument::dir_INOUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	      produceSizeCalculation(s,a->field_type(),
				     (AST_Decl*)&def_in,
				     "_0RL_s","_0RL_msgsize",
				     a->uqname(),ntype,mapping);
	      break;
	    }
	  case AST_Argument::dir_IN:
	      break;
	  }
	i.next();
      }
  }

  IND(s); s << "_0RL_s.InitialiseReply(GIOP::NO_EXCEPTION,(CORBA::ULong)_0RL_msgsize);\n";

  // marshall results
  if (!return_is_void())
    {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(),wResult,mapping);
      if ((ntype == tObjref || ntype == tString || ntype == tTypeCode ||
	   mapping.is_pointer)
	  && !mapping.is_arrayslice)
	{
	  // These are declared as <type>_var variable
	  if (ntype == tString)
	    {
	      produceMarshalCode(s,return_type(),
				 (AST_Decl*)&def_in,
				 "_0RL_s",
				 "_0RL_result",ntype,mapping);
	    }
	  else
	    {
	      // use operator->() to get to the pointer
	      produceMarshalCode(s,return_type(),(AST_Decl*)&def_in,
				 "_0RL_s",
				 "(_0RL_result.operator->())",ntype,mapping);
	    }
	}
      else
	{
	  produceMarshalCode(s,return_type(),(AST_Decl*)&def_in,
			     "_0RL_s","_0RL_result",ntype,mapping);
	}
    }
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argMapping mapping;
	argType ntype;

	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	switch(a->direction())
	  {
	  case AST_Argument::dir_OUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	      if ((ntype == tObjref || ntype == tString || ntype == tTypeCode
		   || (mapping.is_reference && mapping.is_pointer))
		  && !mapping.is_arrayslice)
		{
		  // These are declared as <type>_var variable
		  if (ntype == tString)
		    {
		      produceMarshalCode(s,a->field_type(),
					 (AST_Decl*)&def_in,
					 "_0RL_s",
					 a->uqname(),ntype,mapping);
		    }
		  else
		    {
		      // use operator->() to get to the pointer
		      char *_argname = new char[strlen(a->uqname())+
					       strlen("(.operator->())")+1];
		      strcpy(_argname,"(");
		      strcat(_argname,a->uqname());
		      strcat(_argname,".operator->())");
		      produceMarshalCode(s,a->field_type(),
					 (AST_Decl*)&def_in,
					 "_0RL_s",
					 _argname,ntype,mapping);
		      delete [] _argname;
		    }
		}
	      else
		{
		  produceMarshalCode(s,a->field_type(),
				     (AST_Decl*)&def_in,
				     "_0RL_s",a->uqname(),ntype,mapping);
		}
	      break;
	    }
	  case AST_Argument::dir_INOUT:
	    {
	      ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	      produceMarshalCode(s,a->field_type(),
				 (AST_Decl*)&def_in,
				 "_0RL_s",a->uqname(),ntype,mapping);
	      break;
	    }
	  case AST_Argument::dir_IN:
	      break;
	  }
	i.next();
      }
  }

  IND(s); s << "_0RL_s.ReplyCompleted();\n";
  IND(s); s << "return 1;\n";
  return;
}

void
o2be_operation::produce_nil_skel(std::fstream &s,const char* alias_prefix)
{
  IND(s); produce_decl(s,0,alias_prefix);
  s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_NO);\n";
  s << "#ifdef NEED_DUMMY_RETURN\n";
  IND(s); s << "// never reach here! Dummy return to keep some compilers happy.\n";
  if (!return_is_void())
    {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(),wResult,mapping);
      if (ntype == tObjref || ntype == tString || ntype == tTypeCode ||
	  (mapping.is_arrayslice) ||
	  (mapping.is_pointer))
	{
	  IND(s);
	  declareVarType(s,return_type(),this,0,mapping.is_arrayslice);
	  s <<
	((ntype != tObjref && ntype != tString && ntype != tTypeCode)?" *":"")
	    << " _0RL_result" << " = "
	    << ((ntype == tTypeCode) ? "CORBA::TypeCode::_nil()" : "0")
	    << ";\n";
	}
      else
	{
	  IND(s);
	  declareVarType(s,return_type(),this);
	  s << " _0RL_result";
	  switch (ntype)
	    {
	    case tShort:
	    case tLong:
	    case tUShort:
	    case tULong:
	    case tFloat:
	    case tDouble:
	    case tBoolean:
	    case tChar:
	    case tOctet:
	      s << " = 0;\n";
	      break;
	    case tEnum:
	      {
		s << " = ";
		AST_Decl *decl = return_type();
		while (decl->node_type() == AST_Decl::NT_typedef)
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		UTL_ScopeActiveIterator i(o2be_enum::narrow_from_decl(decl),
					  UTL_Scope::IK_decls);
		AST_Decl *eval = i.item();
		s << o2be_name::narrow_and_produce_unambiguous_name(eval,this)
		  << ";\n";
	      }
	      break;
	    case tStructFixed:
	      s << ";\n";
	      s << "memset((void *)&_0RL_result,0,sizeof(_0RL_result));\n";
	      break;
	    default:
	      s << ";\n";
	      break;
	    }
	}
      IND(s); s << "return _0RL_result;\n";
    }
  else
    {
      IND(s); s << "return;\n";
    }
  DEC_INDENT_LEVEL();
  s << "#endif\n";
  IND(s); s << "}\n";
}


void
o2be_operation::produce_lcproxy_skel(std::fstream& s, o2be_interface& def_in,
				     const char* alias_prefix)
{
  o2be_call_desc::produce_descriptor(s, *this);
  const char* call_desc_class = o2be_call_desc::descriptor_name(*this);

  char* ctxt_un = 0;
  int   ctxt_len = 0;
  if (context()) {
    ctxt_un = o2be_call_desc::generate_unique_name("_0RL_ctx_");
    IND(s); s << "static const char* " << ctxt_un << "[] = {\n";
    ctxt_len = 0;
    UTL_StrlistActiveIterator iter(context());

    while (!iter.is_done()) {
      String* p = iter.item();
      IND(s); s  << "\"" << p->get_string() << "\",\n";
      ctxt_len++;
      iter.next();
    }
    IND(s); s << " 0 };\n\n";
  }

  IND(s); produce_decl(s, def_in.lcproxy_fqname(), alias_prefix,
		       I_FALSE, I_TRUE);
  s << "\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << call_desc_class << " _call_desc(\""
	    << local_name()->get_string() << "\", "
	    << (strlen(local_name()->get_string()) + 1);
  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while( !i.is_done() ) {
      o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
      s << ", " << a->uqname();
      i.next();
    }
  }
  s << ");\n\n";

  if (context()) {
    IND(s); s << "_call_desc.set_context(ctxt,"
      <<                                 ctxt_un  << ","
      <<                                 ctxt_len << ");\n\n";
  }

  if (flags() == AST_Operation::OP_oneway) {
    IND(s); s << "if (!OmniLCProxyCallWrapper::one_way(this, _call_desc, "
	      << "_get_wrap_" << def_in._fqname() << "())) {\n";
  }
  else {
    IND(s); s << "if (!OmniLCProxyCallWrapper::invoke(this, _call_desc, "
	      << "_get_wrap_" << def_in._fqname() << "())) {\n";
  }
  INC_INDENT_LEVEL();
  IND(s);
  if (return_is_void()) {
    s << "_get_wrap_" << def_in._fqname() << "()->";
    produce_invoke(s);
    s << ";\n";
    IND(s); s << "return;\n";
  }
  else {
    s << "return _get_wrap_" << def_in._fqname() << "()->";
    produce_invoke(s);
    s << ";\n";
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";

  if( !return_is_void() ) {
    IND(s); s << "return _call_desc.result();\n";
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";
}


void
o2be_operation::produce_dead_skel(std::fstream& s, const char* alias_prefix)
{
  IND(s); produce_decl(s,0,alias_prefix,I_FALSE,I_TRUE);
  s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);\n";
  s << "#ifdef NEED_DUMMY_RETURN\n";
  IND(s); s << "// never reach here! Dummy return to keep some compilers happy.\n";
  if (!return_is_void())
    {
      argMapping mapping;
      argType ntype = ast2ArgMapping(return_type(),wResult,mapping);
      if (ntype == tObjref || ntype == tString || ntype == tTypeCode ||
	  (mapping.is_arrayslice) ||
	  (mapping.is_pointer))
	{
	  IND(s);
	  declareVarType(s,return_type(),this,0,mapping.is_arrayslice);
	  s <<
	 ((ntype != tObjref && ntype != tString && ntype !=tTypeCode)?" *":"")
	    << " _0RL_result = "
	    << ((ntype == tTypeCode) ? "CORBA::TypeCode::_nil()" : "0")
	    << ";\n";
	}
      else
	{
	  IND(s);
	  declareVarType(s,return_type(),this);
	  s << " _0RL_result";
	  switch (ntype)
	    {
	    case tShort:
	    case tLong:
	    case tUShort:
	    case tULong:
	    case tFloat:
	    case tDouble:
	    case tBoolean:
	    case tChar:
	    case tOctet:
	      s << " = 0;\n";
	      break;
	    case tEnum:
	      {
		s << " = ";
		AST_Decl *decl = return_type();
		while (decl->node_type() == AST_Decl::NT_typedef)
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		UTL_ScopeActiveIterator i(o2be_enum::narrow_from_decl(decl),
					  UTL_Scope::IK_decls);
		AST_Decl *eval = i.item();
		s << o2be_name::narrow_and_produce_unambiguous_name(eval,this)
		  << ";\n";
	      }
	      break;
	    case tStructFixed:
	      s << ";\n";
	      s << "memset((void *)&_0RL_result,0,sizeof(_0RL_result));\n";
	      break;
	    default:
	      s << ";\n";
	      break;
	    }
	}
      IND(s); s << "return _0RL_result;\n";
    }
  else
    {
      IND(s); s << "return;\n";
    }
  DEC_INDENT_LEVEL();
  s << "#endif\n";
  IND(s); s << "}\n";
}

void
o2be_operation::produce_home_skel(std::fstream& s, o2be_interface &def_in,
				  const char* alias_prefix)
{
  IND(s); produce_decl(s,0,alias_prefix,I_FALSE,I_TRUE);
  s << "{\n";
  INC_INDENT_LEVEL();
  IND(s);
  if (!return_is_void())
    s << "return ";
  s << "_actual_" << def_in._fqname() << "->";
  produce_invoke(s);
  s << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
}

void
o2be_operation::produce_wrapproxy_skel(std::fstream& s,
				       o2be_interface &def_in,
				       const char* alias_prefix)
{
  IND(s); produce_decl(s,0,alias_prefix,I_FALSE,I_TRUE);
  s << "{\n";
  INC_INDENT_LEVEL();
  IND(s);
  if (!return_is_void())
    s << "return ";
  s << "_actual_" << def_in._fqname() << "->";
  produce_invoke(s);
  s << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
}

void
o2be_operation::produce_mapping_with_indirection(std::fstream& s,
						 const char* alias_prefix)
{
  if (!has_variable_out_arg() && !has_pointer_inout_arg())
    return;

  unsigned int indent_pos = 0;

  IND(s);
  // return type
  if (!return_is_void())
    {
      argMapping mapping;
      argType    ntype;
      const char* str;

      ntype = ast2ArgMapping(return_type(),wResult,mapping);

      if (ntype == tObjref) {
	AST_Decl *decl = return_type();
	while (decl->node_type() == AST_Decl::NT_typedef) {
	  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	}
	str = o2be_interface::narrow_from_decl(decl)->unambiguous_objref_name(this);
	s << str;
	indent_pos += strlen(str);
      }
      else if (ntype == tString) {
	s << "char *";
	indent_pos += 6;
      }
      else if (ntype == tTypeCode) {
	s << "CORBA::TypeCode_ptr";
	indent_pos += 19;
      }
      else {
	str = o2be_name::narrow_and_produce_unambiguous_name(return_type(),this);
	s << str;
	indent_pos += strlen(str);
      }
      if (mapping.is_arrayslice) {
	s << "_slice";
	indent_pos += 6;

      }
      s << " ";
      indent_pos += 1;
      if (mapping.is_pointer) {
	s << "*";
	indent_pos += 1;
      }
      if (mapping.is_reference) {
	s << "&";
	indent_pos += 1;
      }
    }
  else
    {
      s << "void";
      indent_pos += 4;
    }
  // function namees
  s << " " << uqname() << " ( ";
  indent_pos += strlen(alias_prefix) + strlen(uqname()) + 4;

  // argument list
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argMapping mapping;
	argType    ntype;
	idl_bool   outvar;
	idl_bool   inoutptr;

	outvar = I_FALSE;
	inoutptr = I_FALSE;
	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	switch(a->direction())
	  {
	  case AST_Argument::dir_IN:
	    ntype = ast2ArgMapping(a->field_type(),wIN,mapping);
	    break;
	  case AST_Argument::dir_INOUT:
	    ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	    switch(ntype) {
	    case tObjref:
	    case tString:
	    case tTypeCode:
	      inoutptr = I_TRUE;
	      break;
	    default:
	      break;
	    }
	    break;
	  case AST_Argument::dir_OUT:
	    ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	    switch(ntype) {
	    case tObjref:
	    case tStructVariable:
	    case tUnionVariable:
	    case tString:
	    case tSequence:
	    case tArrayVariable:
	    case tAny:
	    case tTypeCode:
	      outvar = I_TRUE;
	      break;
	    default:
	      break;
	    }
	    break;
	  }

	if (!outvar && !inoutptr)
	  {
	    s << ((mapping.is_const) ? "const ":"");
	    if (ntype == tObjref) {
	      AST_Decl *decl = a->field_type();
	      while (decl->node_type() == AST_Decl::NT_typedef) {
		decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	      }
	      s << o2be_interface::narrow_from_decl(decl)->unambiguous_objref_name(this);
	    }
	    else if (ntype == tString) {
	      s << "char *";
	    }
	    else if (ntype == tTypeCode) {
	      AST_Decl *decl = a->field_type();
	      while (decl->node_type() == AST_Decl::NT_typedef) {
		decl = o2be_typedef::narrow_from_decl(decl)->base_type();
	      }
	      s << o2be_name::narrow_and_produce_unambiguous_name(decl,this);
	    }
	    else {
	      s << o2be_name::narrow_and_produce_unambiguous_name(a->field_type(),this);
	    }
	    s << ((mapping.is_arrayslice) ? "_slice":"")
	      << " "
	      << ((mapping.is_pointer)    ? "*":"")
	      << ((mapping.is_reference)  ? "&":"");
	    s << " " << a->uqname();
	  }
	else
	  {
	    switch (ntype) {
	    case tObjref:
	      {
		AST_Decl *decl = a->field_type();
		while (decl->node_type() == AST_Decl::NT_typedef) {
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		}
		o2be_interface* intf = o2be_interface::narrow_from_decl(decl);
		if (a->direction() == AST_Argument::dir_INOUT) {
		  s << intf->inout_adptarg_name(this) << " ";
		}
		else {
		  s << intf->out_adptarg_name(this) << " ";
		}
		break;
	      }
	    case tString:
	      {
		if (a->direction() == AST_Argument::dir_INOUT) {
		  s << "CORBA::String_INOUT_arg ";
		}
		else {
		  s << "CORBA::String_OUT_arg ";
		}
		break;
	      }
	    case tAny:
	      {
		if (a->direction() == AST_Argument::dir_OUT) {
		  s << "CORBA::Any_OUT_arg ";
		}
		break;
	      }
	    case tTypeCode:
	      {
		if (a->direction() == AST_Argument::dir_INOUT) {
		  s << "CORBA::TypeCode_INOUT_arg ";
		}
		else {
		  s << "CORBA::TypeCode_OUT_arg ";
		}
		break;
	      }
	    case tStructVariable:
	      {
		AST_Decl *decl = a->field_type();
		while (decl->node_type() == AST_Decl::NT_typedef) {
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		}
		o2be_structure* p = o2be_structure::narrow_from_decl(decl);
		if (a->direction() == AST_Argument::dir_OUT) {
		  s << p->out_adptarg_name(this) << " ";
		}
		break;
	      }
	    case tUnionVariable:
	      {
		AST_Decl *decl = a->field_type();
		while (decl->node_type() == AST_Decl::NT_typedef) {
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		}
		o2be_union* p = o2be_union::narrow_from_decl(decl);
		if (a->direction() == AST_Argument::dir_OUT) {
		  s << p->out_adptarg_name(this) << " ";
		}
		break;
	      }
	    case tSequence:
	      {
		AST_Decl *decl = a->field_type();
		if (decl->node_type() != AST_Decl::NT_typedef) {
		  throw o2be_internal_error(__FILE__,__LINE__,
					    "Typedef expected");
		}
		o2be_typedef* tp = o2be_typedef::narrow_from_decl(decl);
		while (decl->node_type() == AST_Decl::NT_typedef) {
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		}
		o2be_sequence* p = o2be_sequence::narrow_from_decl(decl);
		if (a->direction() == AST_Argument::dir_OUT) {
		  s << p->out_adptarg_name(tp,this) << " ";
		}
		break;
	      }
	    case tArrayVariable:
	      {
		AST_Decl *decl = a->field_type();
		if (decl->node_type() != AST_Decl::NT_typedef) {
		  throw o2be_internal_error(__FILE__,__LINE__,
					    "Typedef expected");
		}
		o2be_typedef* tp = o2be_typedef::narrow_from_decl(decl);
		while (decl->node_type() == AST_Decl::NT_typedef) {
		  decl = o2be_typedef::narrow_from_decl(decl)->base_type();
		}
		o2be_array* p = o2be_array::narrow_from_decl(decl);
		if (a->direction() == AST_Argument::dir_OUT) {
		  s << p->out_adptarg_name(tp,this) << " ";
		}
		break;
	      }
	    default:
	      break;
	    }
	    s << " " << a->uqname();
	  }
	i.next();
	if (!i.is_done() || context()) {
	  s << ",\n";
	  IND(s);
	  for (unsigned int j=0; j < indent_pos; j++)
	    s << " ";
	}
      }
  }

  {
    if (context()) {
      s << "CORBA::Context_ptr ctxt";
    }
  }

  s << " )\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s);
  if (!return_is_void()) {
    s << "return ";
  }
  s << alias_prefix;
  s << uqname() << " ( ";
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	argType ntype;
	argMapping mapping;
	o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
	s << a->uqname();
	if (a->direction() == AST_Argument::dir_INOUT) {
	  ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	  if (ntype == tObjref || ntype == tString || ntype == tTypeCode) {
	    s << "._data";
	  }
	}
	else if (a->direction() == AST_Argument::dir_OUT) {
	  ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	  switch(ntype) {
	  case tObjref:
	  case tStructVariable:
	  case tUnionVariable:
	  case tString:
	  case tSequence:
	  case tArrayVariable:
	  case tAny:
	  case tTypeCode:
	    s << "._data";
	  default:
	    break;
	  }
	}
	i.next();
	s << ((!i.is_done()) ? ", " :(context()?", ":""));
      }
  }
  if (context())
    s << "ctxt)";
  else
    s << " )";

  s << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
}

void
o2be_operation::check_and_produce_unnamed_argument_tc_decl(std::fstream&s,
							   AST_Decl* d)
{
  // Check the argument <d>, for each unamed, bounded string, produce
  // a typecode constant
  // for instance:
  //       string<4> op();
  // produce:
  //   CORBA::TypeCode_ptr _tc_string_4;

  if (d->node_type() == AST_Decl::NT_string &&
      o2be_string::narrow_from_decl(d)->max_length()) {

    // XXX Check how to avoid name clash with either guards.
    s << "\n#if !defined(__" 
      << o2be_string::narrow_from_decl(d)->tcname()
      << "__) && !defined(DISABLE_Unnamed_Bounded_String_TC)\n";
    s << "#define __" 
      << o2be_string::narrow_from_decl(d)->tcname()
      << "__\n";
    IND(s); s << o2be_global::root()->variable_qualifier()
	      << " _dyn_attr const CORBA::TypeCode_ptr " 
	      << o2be_string::narrow_from_decl(d)->tcname() << ";\n";
    s << "#endif\n\n";
  }
}

void
o2be_operation::produce_decls_at_global_scope_in_hdr(std::fstream& s)
{
  if (!return_is_void()) {
    check_and_produce_unnamed_argument_tc_decl(s,return_type());
  }

  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	check_and_produce_unnamed_argument_tc_decl(s,a->field_type());
	i.next();
      }
  }

}

void
o2be_operation::check_and_produce_unnamed_argument_tc_value(std::fstream& s,
							    AST_Decl* d)
{
  // Check the argument <d>, for each unamed, bounded string, produce
  // a typecode constant
  // for instance:
  //       string<4> op();
  // produce:
  //   CORBA::TypeCode_ptr _tc_string_4 = CORBA::TypeCode::PR_string_tc(4);

  if (d->node_type() == AST_Decl::NT_string &&
      o2be_string::narrow_from_decl(d)->max_length()) {

    s << "\n#if !defined(__" 
      << o2be_string::narrow_from_decl(d)->tcname()
      << "_value__) && !defined(DISABLE_Unnamed_Bounded_String_TC)\n";
    s << "#define __" 
      << o2be_string::narrow_from_decl(d)->tcname()
      << "_value__\n";
    IND(s); s << "const CORBA::TypeCode_ptr " 
	      << o2be_string::narrow_from_decl(d)->tcname()
	      << " = " 
	      << "CORBA::TypeCode::PR_string_tc("
	      << o2be_string::narrow_from_decl(d)->max_length()
	      << ");\n\n";
    s << "#endif\n\n";
  }
}

void
o2be_operation::produce_dynskel(std::fstream& s)
{
  if (!return_is_void()) {
    check_and_produce_unnamed_argument_tc_value(s,return_type());
  }

  {
    UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	o2be_argument* a = o2be_argument::narrow_from_decl(i.item());
	check_and_produce_unnamed_argument_tc_value(s,a->field_type());
	i.next();
      }
  }
}

idl_bool
o2be_operation::has_variable_out_arg()
{
  idl_bool hasvar = I_FALSE;
  UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
  while (!i.is_done())
    {
      argMapping mapping;
      argType    ntype;
      o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
      switch(a->direction())
	{
	case AST_Argument::dir_OUT:
	  ntype = ast2ArgMapping(a->field_type(),wOUT,mapping);
	  switch (ntype) {
	  case tObjref:
	  case tStructVariable:
	  case tUnionVariable:
	  case tString:
	  case tSequence:
	  case tArrayVariable:
	  case tAny:
	  case tTypeCode:
	    hasvar = I_TRUE;
	    break;
	  default:
	    break;
	  }
	  break;
	default:
	  break;
	}
      i.next();
    }
  return hasvar;
}


idl_bool
o2be_operation::has_pointer_inout_arg()
{
  idl_bool hasptr = I_FALSE;
  UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
  while (!i.is_done())
    {
      argMapping mapping;
      argType    ntype;
      o2be_argument *a = o2be_argument::narrow_from_decl(i.item());
      switch(a->direction())
	{
	case AST_Argument::dir_INOUT:
	  ntype = ast2ArgMapping(a->field_type(),wINOUT,mapping);
	  switch (ntype) {
	  case tObjref:
	  case tString:
	  case tTypeCode:
	    hasptr = I_TRUE;
	    break;
	  default:
	    break;
	  }
	  break;
	default:
	  break;
	}
      i.next();
    }
  return hasptr;
}


idl_bool
o2be_operation::return_is_void()
{
  AST_Decl *rtype = return_type();
  if (rtype->node_type() == AST_Decl::NT_pre_defined &&
      AST_PredefinedType::narrow_from_decl(rtype)->pt()
          == AST_PredefinedType::PT_void)
    return I_TRUE;
  else
    return I_FALSE;
}


idl_bool
o2be_operation::has_any_in_args()
{
  UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);

  while( !i.is_done() ) {
    o2be_argument *a = o2be_argument::narrow_from_decl(i.item());

    if( a->direction() == AST_Argument::dir_IN )
      return I_TRUE;

    i.next();
  }
  return I_FALSE;
}


idl_bool
o2be_operation::has_any_inout_args()
{
  UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);

  while( !i.is_done() ) {
    o2be_argument *a = o2be_argument::narrow_from_decl(i.item());

    if( a->direction() == AST_Argument::dir_INOUT )
      return I_TRUE;

    i.next();
  }
  return I_FALSE;
}


idl_bool
o2be_operation::has_any_out_args()
{
  UTL_ScopeActiveIterator i(this, UTL_Scope::IK_decls);

  while( !i.is_done() ) {
    o2be_argument *a = o2be_argument::narrow_from_decl(i.item());

    if( a->direction() == AST_Argument::dir_OUT )
      return I_TRUE;

    i.next();
  }
  return I_FALSE;
}


const char*
o2be_operation::mangled_signature()
{
  if( !pd_mangled_signature )
    pd_mangled_signature =
      o2be_name_mangler::produce_operation_signature(*this);

  return pd_mangled_signature;
}


IMPL_NARROW_METHODS1(o2be_operation, AST_Operation)
IMPL_NARROW_FROM_DECL(o2be_operation)
IMPL_NARROW_FROM_SCOPE(o2be_operation)

static
const o2be_operation::argMapping
      inArgMappingTable[o2be_operation::_tMaxValue] = {
  // IN argument passing rule.
  // Reference: CORBA V2.0 Table 24
  //
  // const, reference, pointer, arrayslice
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tShort
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tLong
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tUShort
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tULong
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tFloat
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tDouble
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tBoolean
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tChar
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tOctet
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tEnum
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tObjref
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tStructFixed
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tStructVariable
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tUnionFixed
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tUnionVariable
  { I_TRUE,  I_FALSE, I_FALSE, I_FALSE }, // tString
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tSequence
  { I_TRUE,  I_FALSE, I_FALSE, I_FALSE }, // tArrayFixed
  { I_TRUE,  I_FALSE, I_FALSE, I_FALSE }, // tArrayVariable
  { I_TRUE,  I_TRUE,  I_FALSE, I_FALSE }, // tAny
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tTypeCode
};

static
const o2be_operation::argMapping
      outArgMappingTable[o2be_operation::_tMaxValue] = {
  // OUT argument passing rule.
  // Reference: CORBA V2.0 Table 24
  //
  // const, reference, pointer, arrayslice
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tShort
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tLong
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tUShort
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tULong
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tFloat
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tDouble
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tBoolean
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tChar
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tOctet
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tEnum
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tObjref
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tStructFixed
  { I_FALSE, I_TRUE,  I_TRUE,  I_FALSE }, // tStructVariable
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tUnionFixed
  { I_FALSE, I_TRUE,  I_TRUE,  I_FALSE }, // tUnionVariable
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tString
  { I_FALSE, I_TRUE,  I_TRUE,  I_FALSE }, // tSequence
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tArrayFixed
  { I_FALSE, I_TRUE,  I_TRUE,  I_TRUE  }, // tArrayVariable
  { I_FALSE, I_TRUE,  I_TRUE,  I_FALSE }, // tAny
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tTypeCode
};

static
const o2be_operation::argMapping
      inOutArgMappingTable[o2be_operation::_tMaxValue] = {
  // INOUT argument passing rule.
  // Reference: CORBA V2.0 Table 24
  //
  // const, reference, pointer, arrayslice
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tShort
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tLong
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tUShort
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tULong
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tFloat
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tDouble
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tBoolean
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tChar
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tOctet
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tEnum
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tObjref
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tStructFixed
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tStructVariable
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tUnionFixed
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tUnionVariable
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tString
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tSequence
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tArrayFixed
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tArrayVariable
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tAny
  { I_FALSE, I_TRUE,  I_FALSE, I_FALSE }, // tTypeCode
};

static
const o2be_operation::argMapping
      retValMappingTable[o2be_operation::_tMaxValue] = {
  // Return Value  passing rule.
  // Reference: CORBA V2.0 Table 24
  //
  // const, reference, pointer, arrayslice
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tShort
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tLong
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tUShort
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tULong
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tFloat
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tDouble
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tBoolean
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tChar
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tOctet
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tEnum
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tObjref
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tStructFixed
  { I_FALSE, I_FALSE, I_TRUE,  I_FALSE }, // tStructVariable
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tUnionFixed
  { I_FALSE, I_FALSE, I_TRUE,  I_FALSE }, // tUnionVariable
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tString
  { I_FALSE, I_FALSE, I_TRUE,  I_FALSE }, // tSequence
  { I_FALSE, I_FALSE, I_TRUE,  I_TRUE  }, // tArrayFixed
  { I_FALSE, I_FALSE, I_TRUE,  I_TRUE  }, // tArrayVariable
  { I_FALSE, I_FALSE, I_TRUE,  I_FALSE }, // tAny
  { I_FALSE, I_FALSE, I_FALSE, I_FALSE }, // tTypeCode
};

o2be_operation::argType
o2be_operation::ast2ArgMapping(AST_Decl *decl,
			       o2be_operation::argWhich dir,
			       o2be_operation::argMapping &mapping)
{
  while( decl->node_type() == AST_Decl::NT_typedef )
    decl = o2be_typedef::narrow_from_decl(decl)->base_type();

  argType ntype = tShort;

  switch(decl->node_type())
    {
    case AST_Decl::NT_pre_defined:
      {
	switch(o2be_predefined_type::narrow_from_decl(decl)->pt())
	  {
	  case AST_PredefinedType::PT_short:
	    ntype = tShort;
	    break;
	  case AST_PredefinedType::PT_long:
	    ntype = tLong;
	    break;
	  case AST_PredefinedType::PT_ushort:
	    ntype = tUShort;
	    break;
	  case AST_PredefinedType::PT_ulong:
	    ntype = tULong;
	    break;
	  case AST_PredefinedType::PT_float:
	    ntype = tFloat;
	    break;
	  case AST_PredefinedType::PT_double:
	    ntype = tDouble;
	    break;
	  case AST_PredefinedType::PT_boolean:
	    ntype = tBoolean;
	    break;
	  case AST_PredefinedType::PT_char:
	    ntype = tChar;
	    break;
	  case AST_PredefinedType::PT_octet:
	    ntype = tOctet;
	    break;
	  case AST_PredefinedType::PT_any:
	    ntype = tAny;
	    break;
	  case AST_PredefinedType::PT_TypeCode:
	    ntype = tTypeCode;
	    break;
	  case AST_PredefinedType::PT_longlong:
	  case AST_PredefinedType::PT_ulonglong:
	  case AST_PredefinedType::PT_longdouble:
	  case AST_PredefinedType::PT_wchar:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unsupported argument type");
	  case AST_PredefinedType::PT_void:
	  case AST_PredefinedType::PT_pseudo:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Invalid argument type");
	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unknown argument type");
	  }
      }
      break;
    case AST_Decl::NT_enum:
      ntype = tEnum;
      break;
    case AST_Decl::NT_interface:
      ntype = tObjref;
      break;
    case AST_Decl::NT_union:
      if (o2be_union::narrow_from_decl(decl)->isVariable())
	ntype = tUnionVariable;
      else
	ntype = tUnionFixed;
      break;
    case AST_Decl::NT_struct:
      if (o2be_structure::narrow_from_decl(decl)->isVariable())
	ntype = tStructVariable;
      else
	ntype = tStructFixed;
      break;
    case AST_Decl::NT_string:
      ntype = tString;
      break;
    case AST_Decl::NT_sequence:
      ntype = tSequence;
      break;
    case AST_Decl::NT_array:
      if (o2be_array::narrow_from_decl(decl)->isVariable())
	ntype = tArrayVariable;
      else
	ntype = tArrayFixed;
      break;
    default:
      throw o2be_internal_error(__FILE__,__LINE__,"Unexpected argument type");
    }

  switch (dir)
    {
    case wIN:
      mapping = inArgMappingTable[ntype];
      break;
    case wOUT:
      mapping = outArgMappingTable[ntype];
      break;
    case wINOUT:
      mapping = inOutArgMappingTable[ntype];
      break;
    case wResult:
      mapping = retValMappingTable[ntype];
      break;
    }
  return ntype;
}


void
o2be_operation::declareVarType(std::fstream& s,AST_Decl* decl,AST_Decl*
			       used_in, idl_bool is_var,
			       idl_bool is_arrayslice)
{
  AST_Decl *truetype = decl;
  while (truetype->node_type() == AST_Decl::NT_typedef) {
    truetype = o2be_typedef::narrow_from_decl(truetype)->base_type();
  }

  if (truetype->node_type() == AST_Decl::NT_interface)
    {
      if (!is_var)
	s << o2be_interface::narrow_from_decl(truetype)
	  ->unambiguous_objref_name(used_in);
      else
	s << o2be_name::narrow_and_produce_unambiguous_name(truetype,used_in)
	  << "_var";
    }
  else if (truetype->node_type() == AST_Decl::NT_string)
    {
      if (!is_var)
	s << "char*";
      else
	s << "CORBA::String_var";
    }
  else if (truetype->node_type() == AST_Decl::NT_pre_defined)
    {
      if (AST_PredefinedType::narrow_from_decl(truetype)->pt()
	  == AST_PredefinedType::PT_TypeCode)
	{
	  if (is_var)  s << "CORBA::TypeCode_var";
	  else s << "CORBA::TypeCode_ptr";
	}
      else
	{
	  s <<
	    o2be_name::narrow_and_produce_unambiguous_name(decl,used_in);
	  if (is_var)
	    s << "_var";
	  else if (is_arrayslice)
	    s << "_slice";
	}
    }
  else
    {
      s << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in);
      if (is_var)
	s << "_var";
      else
	if (is_arrayslice)
	  s << "_slice";
    }
}


void
o2be_operation::produceUnMarshalCode(std::fstream& s, AST_Decl* decl,
				     AST_Decl* used_in,
				     const char* netstream,
				     const char* argname,
				     argType type, argMapping mapping,
				     idl_bool no_size_check)
{
  while (decl->node_type() == AST_Decl::NT_typedef) {
    decl = o2be_typedef::narrow_from_decl(decl)->base_type();
  }
  switch(type)
    {
    case tBoolean:
    case tChar:
    case tOctet:
    case tShort:
    case tUShort:
    case tLong:
    case tULong:
    case tEnum:
    case tFloat:
    case tDouble:
      IND(s); s << argname << " <<= " << netstream << ";\n";
      break;

    case tStructFixed:
    case tUnionFixed:
    case tStructVariable:
    case tUnionVariable:
    case tSequence:
    case tAny:
    case tObjrefMember:
    case tStringMember:
    case tTypeCodeMember:
      IND(s); s << ((mapping.is_pointer)?"*":"") << argname << " <<= "
		<< netstream << ";\n";
      break;

    case tString:
      {
	IND(s); s << "{\n";
	INC_INDENT_LEVEL();
	IND(s); s << "CORBA::String_member _0RL_str_tmp;\n";
	IND(s); s << "_0RL_str_tmp <<= " << netstream << ";\n";
	IND(s); s << argname << " = _0RL_str_tmp._ptr;\n";
	IND(s); s << "_0RL_str_tmp._ptr = 0;\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
      }
      break;

    case tObjref:
      {
	IND(s); s << argname << " = "
		  << o2be_interface::narrow_from_decl(decl)->fqname()
		  << "_Helper::unmarshalObjRef(" << netstream << ");\n";
      }
      break;

    case tTypeCode:
      IND(s); s << argname << " = CORBA::TypeCode::unmarshalTypeCode("
		<< netstream << ");\n";
      break;

    case tArrayFixed:
      {
	o2be_array* array = o2be_array::narrow_from_decl(decl);
	argMapping dummymapping;
	argType atype = ast2ArgMapping(array->getElementType(),
				       wIN,         // dummy argument
				       dummymapping // ignored
				       );

	int total_length = array->getNumOfElements();
	StringBuf ptr_to_first_elm;
	ptr_to_first_elm += "((";
	ptr_to_first_elm += o2be_name::narrow_and_produce_unambiguous_name(
					   array->getElementType(), used_in);
	ptr_to_first_elm += "*) ";
	ptr_to_first_elm += argname;
	{
	  size_t ndim = 0;
	  o2be_array::dim_iterator next(array);
	  while( ndim++ < array->getNumOfDims() - 1 ) {
	    ptr_to_first_elm += '[';
	    ptr_to_first_elm += (int) next();
	    ptr_to_first_elm += ']';
	  }
	}
	ptr_to_first_elm += ")";

	switch (atype)
	  {
	  case tBoolean:
	  case tChar:
	  case tOctet:
	    IND(s); s << netstream << ".get_char_array((_CORBA_Char*) "
		      << (const char*)ptr_to_first_elm 
		      << ", " << total_length << ");\n";
	    break;

	  case tShort:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayShort("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tUShort:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayUShort("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tLong:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayLong("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tULong:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayULong("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tEnum:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayULong("
		      << netstream << ", (_CORBA_ULong*) "
		      << (const char*)ptr_to_first_elm 
		      << ", " << total_length << ");\n";
	    break;

	  case tFloat:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayFloat("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tDouble:
	    IND(s); s << "CdrStreamHelper_unmarshalArrayDouble("
		      << netstream << ", " 
		      << (const char*)ptr_to_first_elm << ", "
		      << total_length << ");\n";
	    break;

	  case tStructFixed:
	  case tUnionFixed:
	    {
	      unsigned int ndim = 0;
	      unsigned int dimval;
	      o2be_array::dim_iterator next(array);
	      IND(s); s << "{\n";
	      INC_INDENT_LEVEL();
	      while (ndim < array->getNumOfDims())
		{
		  dimval = next();
		  IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
			    << "_i" << ndim << " < " << dimval << ";"
			    << "_i" << ndim << "++) {\n";
		  INC_INDENT_LEVEL();
		  ndim++;
		}
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < array->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " <<= " << netstream << ";\n";
	      ndim = 0;
	      while (ndim < array->getNumOfDims())
		{
		  DEC_INDENT_LEVEL();
		  IND(s); s << "}\n";
		  ndim++;
		}
	      DEC_INDENT_LEVEL();
	      IND(s); s << "}\n";
	    }
	  break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
			      "Unexpected element type for fixed array");
	  }
      }

      break;

    case tArrayVariable:
      {
	argType atype;
	argMapping dummymapping;
	atype = ast2ArgMapping(o2be_array::narrow_from_decl(decl)->getElementType(),
			       wIN,         // dummy argument
			       dummymapping // ignored
			       );
	unsigned int ndim = 0;
	unsigned int dimval;
	o2be_array::dim_iterator next(o2be_array::narrow_from_decl(decl));
	IND(s); s << "{\n";
	INC_INDENT_LEVEL();
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    dimval = next();
	    IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
		      << "_i" << ndim << " < " << dimval << ";"
		      << "_i" << ndim << "++) {\n";
	    INC_INDENT_LEVEL();
	    ndim++;
	  }
	switch (atype)
	  {
	  case tStructVariable:
	  case tUnionVariable:
	  case tSequence:
	  case tAny:
	    {
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " <<= " << netstream << ";\n";
	      break;
	    }
	  case tTypeCode:
	    {
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " = CORBA::TypeCode::unmarshalTypeCode(" << netstream << ");\n";
	      break;
	    }
	  case tString:
	    {
	      IND(s); s << "CORBA::ULong _len;\n";
	      IND(s); s << "_len <<= " << netstream << ";\n";
	      IND(s); s << "if (!_len) {\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "if (omniORB::traceLevel > 1)\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "_CORBA_null_string_ptr(1);\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "_len = 1;\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "}\n";
	      if (!no_size_check)
		{
		  IND(s); s << "else if ( " << netstream << ".RdMessageUnRead() < _len)\n";
		  INC_INDENT_LEVEL();
		  IND(s); s << "throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);\n";
		  DEC_INDENT_LEVEL();
		}

	      AST_Decl *sdecl = o2be_array::narrow_from_decl(decl)->getElementType();
	      while (sdecl->node_type() == AST_Decl::NT_typedef) {
		sdecl = o2be_typedef::narrow_from_decl(sdecl)->base_type();
	      }
	      if (o2be_string::narrow_from_decl(sdecl)->max_length()) {
		IND(s); s << "if (_len > "
			  << o2be_string::narrow_from_decl(sdecl)->max_length()
			  << "+1) {\n";
		INC_INDENT_LEVEL();
		IND(s); s << "throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);\n";
		DEC_INDENT_LEVEL();
		IND(s); s << "}\n";
	      }


	      IND(s); s << "if (!(char*)(";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s	<< " = CORBA::string_alloc(_len-1)))\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);\n";
	      DEC_INDENT_LEVEL();

	      IND(s); s << "if (_len > 1)\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << netstream << ".get_char_array((CORBA::Char *)((char *)";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "),_len);\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "else\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "*((CORBA::Char*)((char*) ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ")) <<= " << netstream << " ;\n";
	      DEC_INDENT_LEVEL();
	      break;
	    }
	  case tObjref:
	    {
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}

	      AST_Decl *tdecl = o2be_array::narrow_from_decl(decl)->getElementType();
	      while (tdecl->node_type() == AST_Decl::NT_typedef) {
		tdecl = o2be_typedef::narrow_from_decl(tdecl)->base_type();
	      }
	      s << " = "
		<< o2be_interface::narrow_from_decl(tdecl)->fqname()
		<< "_Helper::unmarshalObjRef(" << netstream << ");\n";
 	    }
	    break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unexpected element type for variable array");
	  }
	ndim = 0;
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	    ndim++;
	  }
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
      }
      break;

    case tContext:
      {
	IND(s); s << argname << " = CORBA::Context::unmarshalContext("
		  << netstream << ");\n";
      }
      break;

    default:
      throw o2be_internal_error(__FILE__,__LINE__,"Unexpected argument type");
      break;
    }
}

void
o2be_operation::produceMarshalCode(std::fstream& s, AST_Decl* decl,
				   AST_Decl* used_in,
				   const char* netstream,
				   const char* argname,
				   argType type, argMapping mapping)
{
  while (decl->node_type() == AST_Decl::NT_typedef)
    decl = o2be_typedef::narrow_from_decl(decl)->base_type();

  switch(type)
    {
    case tBoolean:
    case tChar:
    case tOctet:
    case tShort:
    case tUShort:
    case tLong:
    case tULong:
    case tEnum:
    case tFloat:
    case tDouble:
    case tStructFixed:
    case tUnionFixed:
      IND(s); s << argname << " >>= " << netstream << ";\n";
      break;


    case tStructVariable:
    case tUnionVariable:
    case tSequence:
    case tAny:
    case tStringMember:
    case tObjrefMember:
    case tTypeCodeMember:
      IND(s); s << ((mapping.is_pointer)?"*":"") << argname
		<< " >>= " << netstream << ";\n";
      break;
    case tTypeCode:
      IND(s); s << "CORBA::TypeCode::marshalTypeCode("
		<< argname << ", " << netstream << ");\n";
      break;
    case tString:
      {
	IND(s); s << "{\n";
	INC_INDENT_LEVEL();
	IND(s); s << "CORBA::ULong _len = (((const char*) " << argname
		  << ")? strlen((const char*) " << argname
		  << ") + 1 : 1);\n";
	if (o2be_string::narrow_from_decl(decl)->max_length()) {
	  IND(s); s << "if (_len > "
		    << o2be_string::narrow_from_decl(decl)->max_length()
		    << "+1) {\n";
	  INC_INDENT_LEVEL();
	  IND(s); s << "throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_MAYBE);\n";
	  DEC_INDENT_LEVEL();
	  IND(s); s << "}\n";
	}
	IND(s); s << "_len >>= " << netstream << ";\n";
        IND(s); s << "if (_len > 1)\n";
	INC_INDENT_LEVEL();
	IND(s); s << netstream << ".put_char_array((const CORBA::Char *)((const char*) "
		  << argname << "),_len);\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "else {\n";
	INC_INDENT_LEVEL();
	IND(s); s << "if ((const char*) " << argname << " == 0 && omniORB::traceLevel > 1)\n";
	INC_INDENT_LEVEL();
	IND(s); s << "_CORBA_null_string_ptr(0);\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "CORBA::Char('\\0') >>= " << netstream << ";\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
      }
      break;

    case tObjref:
      {
	IND(s); s << o2be_interface::narrow_from_decl(decl)->fqname()
		  << "_Helper::marshalObjRef(" << argname << ","
		  << netstream << ");\n";
      }
      break;

    case tArrayFixed:
      {
	o2be_array* array = o2be_array::narrow_from_decl(decl);
	argMapping dummymapping;
	argType atype = ast2ArgMapping(array->getElementType(),
				       wIN,         // dummy argument
				       dummymapping // ignored
				       );

	int total_length = array->getNumOfElements();
	StringBuf ptr_to_first_elm;
	ptr_to_first_elm += "((const ";
	ptr_to_first_elm += o2be_name::narrow_and_produce_unambiguous_name(
					   array->getElementType(), used_in);
	ptr_to_first_elm += "*) ";
	ptr_to_first_elm += argname;
	{
	  size_t ndim = 0;
	  o2be_array::dim_iterator next(array);
	  while( ndim++ < array->getNumOfDims() - 1 ) {
	    ptr_to_first_elm += '[';
	    ptr_to_first_elm += (int) next();
	    ptr_to_first_elm += ']';
	  }
	}
	ptr_to_first_elm += ")";

	switch (atype)
	  {
	  case tBoolean:
	  case tChar:
	  case tOctet:
	    IND(s); s << netstream << ".put_char_array((const _CORBA_Char*) "
		      << (const char*)ptr_to_first_elm 	
		      << ", " << total_length << ");\n";
	    break;

	  case tShort:
	  case tUShort:
	    IND(s); s << netstream << ".put_char_array((const _CORBA_Char*) "
		      << (const char*)ptr_to_first_elm 
		      << ", " << (total_length * 2)
		      << ", omni::ALIGN_2);\n";
	    break;

	  case tLong:
	  case tULong:
	  case tEnum:
	  case tFloat:
	    IND(s); s << netstream << ".put_char_array((const _CORBA_Char*) "
		      << (const char*)ptr_to_first_elm 
		      << ", " << (total_length * 4)
		      << ", omni::ALIGN_4);\n";
	    break;

	  case tDouble:
	    IND(s); s << netstream << ".put_char_array((const _CORBA_Char*) "
		      << (const char*)ptr_to_first_elm 
		      << ", " << (total_length * 8)
		      << ", omni::ALIGN_8);\n";
	    break;

	  case tStructFixed:
	  case tUnionFixed:
	    {
	      unsigned int ndim = 0;
	      unsigned int dimval;
	      o2be_array::dim_iterator next(array);
	      IND(s); s << "{\n";
	      INC_INDENT_LEVEL();
	      while (ndim < array->getNumOfDims())
		{
		  dimval = next();
		  IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
			    << "_i" << ndim << " < " << dimval << ";"
			    << "_i" << ndim << "++) {\n";
		  INC_INDENT_LEVEL();
		  ndim++;
		}
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < array->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " >>= " << netstream << ";\n";
	      ndim = 0;
	      while (ndim < array->getNumOfDims())
		{
		  DEC_INDENT_LEVEL();
		  IND(s); s << "}\n";
		  ndim++;
		}
	      DEC_INDENT_LEVEL();
	      IND(s); s << "}\n";
	    }
	  break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
			      "Unexpected element type for fixed array");
	  }
      }

      break;

    case tArrayVariable:
      {
	argType atype;
	argMapping dummymapping;
	atype = ast2ArgMapping(o2be_array::narrow_from_decl(decl)->getElementType(),
			       wIN,         // dummy argument
			       dummymapping // ignored
			       );
	unsigned int ndim = 0;
	unsigned int dimval;
	o2be_array::dim_iterator next(o2be_array::narrow_from_decl(decl));
	IND(s); s << "{\n";
	INC_INDENT_LEVEL();
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    dimval = next();
	    IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
		      << "_i" << ndim << " < " << dimval << ";"
		      << "_i" << ndim << "++) {\n";
	    INC_INDENT_LEVEL();
	    ndim++;
	  }
	switch (atype)
	  {
	  case tStructVariable:
	  case tUnionVariable:
	  case tSequence:
	  case tAny:
	    {
	      if (!mapping.is_arrayslice) {
		IND(s); s << argname;
	      }
	      else {
		IND(s); s << "(("
			  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
			  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " >>= " << netstream << ";\n";
	      break;
	    }
	  case tString:
	    {
	      IND(s); s << "CORBA::ULong _len = (((const char*) ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ")? strlen((const char*) ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ") + 1 : 1);\n";

	      AST_Decl *sdecl = o2be_array::narrow_from_decl(decl)->getElementType();
	      while (sdecl->node_type() == AST_Decl::NT_typedef) {
		sdecl = o2be_typedef::narrow_from_decl(sdecl)->base_type();
	      }
	      if (o2be_string::narrow_from_decl(sdecl)->max_length()) {
		IND(s); s << "if (_len > "
			  << o2be_string::narrow_from_decl(sdecl)->max_length()
			  << "+1) {\n";
		INC_INDENT_LEVEL();
		IND(s); s << "throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);\n";
		DEC_INDENT_LEVEL();
		IND(s); s << "}\n";
	      }

	      IND(s); s << "_len >>= " << netstream << ";\n";

	      IND(s); s << "if (_len > 1)\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << netstream << ".put_char_array((const CORBA::Char *)((const char*)";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "),_len);\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "else {\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "if ((const char*) ";

	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << " == 0 && omniORB::traceLevel > 1)\n";
	      INC_INDENT_LEVEL();
	      IND(s); s << "_CORBA_null_string_ptr(0);\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "CORBA::Char('\\0') >>= " << netstream << ";\n";
	      DEC_INDENT_LEVEL();
	      IND(s); s << "}\n";
	      break;
	    }
	  case tObjref:
	    {
	      AST_Decl *tdecl = o2be_array::narrow_from_decl(decl)->getElementType();
	      while (tdecl->node_type() == AST_Decl::NT_typedef) {
		tdecl = o2be_typedef::narrow_from_decl(tdecl)->base_type();
	      }
	      IND(s); s << o2be_interface::narrow_from_decl(tdecl)->fqname()
			<< "_Helper::marshalObjRef(";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "._ptr," << netstream << ");\n";
 	    }
	    break;

	  case tTypeCode:
	    {
	      IND(s); s << "CORBA::TypeCode::marshalTypeCode(" << argname;
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "._ptr, " << netstream << ");\n";
	    }
	    break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unexpected element type for variable array");
	  }
	ndim = 0;
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	    ndim++;
	  }
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
      }
      break;

    default:
      throw o2be_internal_error(__FILE__,__LINE__,"Unexpected argument type");
      break;
    }
}

void
o2be_operation::produceSizeCalculation(std::fstream& s, AST_Decl* decl,
				       AST_Decl* used_in,
				       const char* netstream,
				       const char* sizevar,
				       const char* argname,
				       argType type, argMapping mapping)
{
  while (decl->node_type() == AST_Decl::NT_typedef) {
    decl = o2be_typedef::narrow_from_decl(decl)->base_type();
  }
  switch(type)
    {
    case tBoolean:
    case tChar:
    case tOctet:
      IND(s); s << sizevar << " += 1;\n";
      break;

    case tShort:
    case tUShort:
      IND(s); s << sizevar << " = omni::align_to(" << sizevar
		<< ", omni::ALIGN_2) + 2;\n";
      break;

    case tLong:
    case tULong:
    case tEnum:
    case tFloat:
      IND(s); s << sizevar << " = omni::align_to(" << sizevar
		<< ", omni::ALIGN_4) + 4;\n";
      break;

    case tDouble:
      IND(s); s << sizevar << " = omni::align_to(" << sizevar
		<< ", omni::ALIGN_8) + 8;\n";
      break;

    case tStructFixed:
    case tUnionFixed:
    case tStructVariable:
    case tUnionVariable:
    case tSequence:
    case tAny:
    case tObjrefMember:
    case tStringMember:
    case tTypeCodeMember:
      IND(s); s << sizevar << " = "
		<< argname << ((mapping.is_pointer)?"->":".")
		<< "NP_alignedSize(" << sizevar << ");\n";
      break;

    case tTypeCode:
      IND(s); s << sizevar << " = "
		<< argname << "->NP_alignedSize(" << sizevar << ");\n";
      break;

    case tString:
      IND(s); s << sizevar << " = omni::align_to(" << sizevar
		<< ", omni::ALIGN_4) + 4\n";
      IND(s); s << "  + (((const char*) " << argname
		  << ") ? strlen((const char*) " << argname
		  << ") + 1 : 1);\n";
      break;

    case tObjref:
      {
	IND(s); s << sizevar << " = "
		  << o2be_interface::narrow_from_decl(decl)->fqname()
		  << "_Helper::NP_alignedSize("
		  << argname << "," << sizevar << ");\n";
      }
      break;

    case tArrayFixed:
      {
	argType atype;
	argMapping dummymapping;
	atype = ast2ArgMapping(o2be_array::narrow_from_decl(decl)
			         ->getElementType(),
			       wIN,         // dummy argument
			       dummymapping // ignored
			       );
	switch (atype)
	  {
	  case tBoolean:
	  case tChar:
	  case tOctet:
	    IND(s); s << sizevar << " += "
		      << o2be_array::narrow_from_decl(decl)->getNumOfElements()
		      << ";\n";
	    break;

	  case tShort:
	  case tUShort:
	    IND(s); s << sizevar << " = omni::align_to(" << sizevar
		      << ",omni::ALIGN_2);\n";
	    IND(s); s << sizevar << " += "
		      << o2be_array::narrow_from_decl(decl)->getNumOfElements()
		      << "*2;\n";
	    break;

	  case tLong:
	  case tULong:
	  case tEnum:
	  case tFloat:
	    IND(s); s << sizevar << " = omni::align_to(" << sizevar
		      << ",omni::ALIGN_4);\n";
	    IND(s); s << sizevar << " += "
		      << o2be_array::narrow_from_decl(decl)->getNumOfElements()
		      << "*4;\n";
	    break;

	  case tDouble:
	    IND(s); s << sizevar << " = omni::align_to(" << sizevar
		      << ",omni::ALIGN_8);\n";
	    IND(s); s << sizevar << " += "
		      << o2be_array::narrow_from_decl(decl)->getNumOfElements()
		      << "*8;\n";
	    break;

	  case tStructFixed:
	  case tUnionFixed:
	    {
	      unsigned int ndim = 0;
	      unsigned int dimval;
	      o2be_array::dim_iterator next(o2be_array::narrow_from_decl(decl));
	      IND(s); s << "{\n";
	      INC_INDENT_LEVEL();
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  dimval = next();
		  IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
			    << "_i" << ndim << " < " << dimval << ";"
			    << "_i" << ndim << "++) {\n";
		  INC_INDENT_LEVEL();
		  ndim++;
		}
	      IND(s); s << sizevar << " = ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ".NP_alignedSize(" << sizevar << ");\n";
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  DEC_INDENT_LEVEL();
		  IND(s); s << "}\n";
		  ndim++;
		}
	      DEC_INDENT_LEVEL();
	      IND(s); s << "}\n";
	    }
	  break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unexpected element type for fixed array");
	  }
      }

      break;

    case tArrayVariable:
      {
	argType atype;
	argMapping dummymapping;
	atype = ast2ArgMapping(o2be_array::narrow_from_decl(decl)
			         ->getElementType(),
			       wIN,         // dummy argument
			       dummymapping // ignored
			       );
	unsigned int ndim = 0;
	unsigned int dimval;
	o2be_array::dim_iterator next(o2be_array::narrow_from_decl(decl));
	IND(s); s << "{\n";
	INC_INDENT_LEVEL();
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    dimval = next();
	    IND(s); s << "for (CORBA::ULong _i" << ndim << " =0;"
		      << "_i" << ndim << " < " << dimval << ";"
		      << "_i" << ndim << "++) {\n";
	    INC_INDENT_LEVEL();
	    ndim++;
	  }
	switch (atype)
	  {
	  case tStructVariable:
	  case tUnionVariable:
	  case tSequence:
	  case tAny:
	    {
	      IND(s); s << sizevar << " = ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while(ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ".NP_alignedSize(" << sizevar << ");\n";
	      break;
	    }
	  case tString:
	    {
	      IND(s); s << sizevar << " = omni::align_to(" << sizevar
			<< ", omni::ALIGN_4);\n";
	      IND(s); s << sizevar << " += 4 + (((const char*) ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ")? strlen((const char*) ";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << ") + 1 : 1);\n";
	      break;
	    }
	  case tObjref:
	    {
	      AST_Decl *tdecl = o2be_array::narrow_from_decl(decl)->getElementType();
	      while (tdecl->node_type() == AST_Decl::NT_typedef) {
		tdecl = o2be_typedef::narrow_from_decl(tdecl)->base_type();
	      }
	      IND(s); s << sizevar << " = "
			<< o2be_interface::narrow_from_decl(tdecl)->fqname()
			<< "_Helper::NP_alignedSize(";
	      if (!mapping.is_arrayslice) {
		s << argname;
	      }
	      else {
		s << "(("
		  << o2be_name::narrow_and_produce_unambiguous_name(decl,used_in)
		  << "_slice*) " << argname << ")";
	      }
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "._ptr," << sizevar << ");\n";
 	    }
	    break;

	  case tTypeCode:
	    {
	      IND(s); s << sizevar << " = ((" << argname;
	      ndim = 0;
	      while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
		{
		  s << "[_i" << ndim << "]";
		  ndim++;
		}
	      s << "._ptr)->NP_alignedSize(" << sizevar << "));\n";
	    }
	    break;

	  default:
	    throw o2be_internal_error(__FILE__,__LINE__,
				      "Unexpected element type for variable array");
	  }
	ndim = 0;
	while (ndim < o2be_array::narrow_from_decl(decl)->getNumOfDims())
	  {
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	    ndim++;
	  }
	DEC_INDENT_LEVEL();
	IND(s); s << "}\n";
      }
      break;

    default:
      throw o2be_internal_error(__FILE__,__LINE__,"Unexpected argument type");
      break;
    }
}


void
o2be_operation::produceConstStringMarshalCode(std::fstream &s,
			      const char *netstream,
			      const char *str,size_t len)
{
  IND(s); s << "CORBA::ULong _len = " << len << ";\n";
  IND(s); s << "_len >>= " << netstream << ";\n";
  IND(s); s << netstream << ".put_char_array((CORBA::Char *)"
	    << str << "," << len << ");\n";
}

void
o2be_operation::produceConstStringSizeCalculation(std::fstream &s,
				  const char *sizevar,
				  size_t len)
{
  IND(s); s << sizevar << " = omni::align_to("
	    << sizevar << ",omni::ALIGN_4);\n";
  IND(s); s << sizevar << " += " << (4+len) << ";\n";
}
