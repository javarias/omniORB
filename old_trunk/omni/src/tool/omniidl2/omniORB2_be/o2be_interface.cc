// -*- Mode: C++; -*-
//                          Package   : omniidl2
// o2be_interface.cc        Created on: 12/08/1996
//			    Author    : Sai-Lai Lo (sll)
//
//    Copyright (C) 1996, 1997 Olivetti & Oracle Research Laboratory
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

/*
  $Log$
  Revision 1.11  1997/08/22 12:43:23  sll
  Oh well, gcc does not like variable names starting with __, changed
  the prefix to _0RL_.

  Revision 1.10  1997/08/21 21:20:08  sll
  - Names of internal variables inside the stub code now all start with the
    prefix __ to avoid potential clash with identifiers defined in IDL.
  - fixed bug in the stub code generated for typedef Object X.

// Revision 1.9  1997/05/06  13:58:53  sll
// Public release.
//
  */

#include "idl.hh"
#include "idl_extern.hh"
#include "o2be.h"

#define PROXY_CLASS_PREFIX        "_proxy_"
#define SERVER_CLASS_PREFIX       "_sk_"
#define NIL_CLASS_PREFIX           "_nil_"
#define PROXY_OBJ_FACTORY_POSTFIX "_proxyObjectFactory"
#define IRREPOID_POSTFIX          "_IntfRepoID"
#define FIELD_MEMBER_TEMPLATE     "_CORBA_ObjRef_Member"
#define ADPT_INOUT_CLASS_TEMPLATE "_CORBA_ObjRef_INOUT_arg"
#define ADPT_OUT_CLASS_TEMPLATE   "_CORBA_ObjRef_OUT_arg"

o2be_interface::o2be_interface(UTL_ScopedName *n, AST_Interface **ih, long nih,
			       UTL_StrList *p)
            : AST_Interface(n, ih, nih, p),
	      AST_Decl(AST_Decl::NT_interface, n, p),
	      UTL_Scope(AST_Decl::NT_interface),
	      o2be_name(this),
	      o2be_sequence_chain(this)
{
  if (strcmp(fqname(),"Object") == 0)
    {
      // This node is for the psuedo object "Object" and should be mapped
      // to CORBA::Object.
      // Set the names to properly scoped under "CORBA::".
      set_uqname("Object");
      set_fqname("CORBA::Object");
      set__fqname("CORBA_Object");
      set_scopename("CORBA");
      set__scopename("CORBA");
      pd_objref_uqname = "Object_ptr";
      pd_objref_fqname = "CORBA::Object_ptr";
      pd_fieldmem_uqname = "Object_member";
      pd_fieldmem_fqname = "CORBA::Object_member";
      pd_inout_adptarg_name = "CORBA::Object_INOUT_arg";
      pd_out_adptarg_name = "CORBA::Object_OUT_arg";
      return;
    }

  pd_objref_uqname = new char[strlen(uqname())+strlen("_ptr")+1];
  strcpy(pd_objref_uqname,uqname());
  strcat(pd_objref_uqname,"_ptr");

  pd_objref_fqname = new char[strlen(fqname())+strlen("_ptr")+1];
  strcpy(pd_objref_fqname,fqname());
  strcat(pd_objref_fqname,"_ptr");

  pd_proxy_uqname = new char[strlen(uqname())+strlen(PROXY_CLASS_PREFIX)+1];
  strcpy(pd_proxy_uqname,PROXY_CLASS_PREFIX);
  strcat(pd_proxy_uqname,uqname());

  if (strlen(scopename())) 
    {
      pd_proxy_fqname = new char[strlen(scopename())+
				strlen(pd_proxy_uqname)+1];
      strcpy(pd_proxy_fqname,scopename());
      strcat(pd_proxy_fqname,pd_proxy_uqname);
    }
  else
    {
      pd_proxy_fqname = pd_proxy_uqname;
    }
			    

  pd_server_uqname = new char[strlen(uqname())+strlen(SERVER_CLASS_PREFIX)+1];
  strcpy(pd_server_uqname,SERVER_CLASS_PREFIX);
  strcat(pd_server_uqname,uqname());

  if (strlen(scopename()))
    {
      pd_server_fqname = new char[strlen(scopename())+
				 strlen(pd_server_uqname)+1];
      strcpy(pd_server_fqname,scopename());
      strcat(pd_server_fqname,pd_server_uqname);
    }
  else
    {
      pd_server_fqname = pd_server_uqname;
    }

  pd_nil_uqname = new char[strlen(uqname())+strlen(NIL_CLASS_PREFIX)+1];
  strcpy(pd_nil_uqname,NIL_CLASS_PREFIX);
  strcat(pd_nil_uqname,uqname());

  if (strlen(scopename()))
    {
      pd_nil_fqname = new char[strlen(scopename())+
				 strlen(pd_nil_uqname)+1];
      strcpy(pd_nil_fqname,scopename());
      strcat(pd_nil_fqname,pd_nil_uqname);
    }
  else
    {
      pd_nil_fqname = pd_nil_uqname;
    }



  pd_fieldmem_uqname = new char[strlen(uqname())+
			        strlen(uqname())+strlen("_Helper")+
			        strlen(FIELD_MEMBER_TEMPLATE)+4];
  strcpy(pd_fieldmem_uqname,FIELD_MEMBER_TEMPLATE);
  strcat(pd_fieldmem_uqname,"<");
  strcat(pd_fieldmem_uqname,uqname());
  strcat(pd_fieldmem_uqname,",");
  strcat(pd_fieldmem_uqname,uqname());
  strcat(pd_fieldmem_uqname,"_Helper");
  strcat(pd_fieldmem_uqname,">");

  pd_fieldmem_fqname = new char[strlen(fqname())+
			        strlen(fqname())+strlen("_Helper")+
			        strlen(FIELD_MEMBER_TEMPLATE)+4];
  strcpy(pd_fieldmem_fqname,FIELD_MEMBER_TEMPLATE);
  strcat(pd_fieldmem_fqname,"<");
  strcat(pd_fieldmem_fqname,fqname());
  strcat(pd_fieldmem_fqname,",");
  strcat(pd_fieldmem_fqname,fqname());
  strcat(pd_fieldmem_fqname,"_Helper");
  strcat(pd_fieldmem_fqname,">");

  pd_IRrepoIdSize = strlen(_fqname()) + strlen(IRREPOID_POSTFIX) + 1;
  pd_IRrepoId = new char[pd_IRrepoIdSize];
  strcpy(pd_IRrepoId,_fqname());
  strcat(pd_IRrepoId,IRREPOID_POSTFIX);

  pd_inout_adptarg_name = new char[strlen(ADPT_INOUT_CLASS_TEMPLATE)+
				   strlen("<,, >")+
                                   strlen(fqname())+
				   strlen(fqname())+strlen("_var")+
				   strlen(pd_fieldmem_fqname)+1];
  strcpy(pd_inout_adptarg_name,ADPT_INOUT_CLASS_TEMPLATE);
  strcat(pd_inout_adptarg_name,"<");
  strcat(pd_inout_adptarg_name,fqname());
  strcat(pd_inout_adptarg_name,",");
  strcat(pd_inout_adptarg_name,fqname());
  strcat(pd_inout_adptarg_name,"_var,");
  strcat(pd_inout_adptarg_name,pd_fieldmem_fqname);
  strcat(pd_inout_adptarg_name," >");

  pd_out_adptarg_name = new char[strlen(ADPT_OUT_CLASS_TEMPLATE)+
				   strlen("<,,, >")+
                                   strlen(fqname())+
				   strlen(fqname())+strlen("_var")+
				   strlen(pd_fieldmem_fqname)+
				   strlen(fqname())+strlen("_Helper")+1];
  strcpy(pd_out_adptarg_name,ADPT_OUT_CLASS_TEMPLATE);
  strcat(pd_out_adptarg_name,"<");
  strcat(pd_out_adptarg_name,fqname());
  strcat(pd_out_adptarg_name,",");
  strcat(pd_out_adptarg_name,fqname());
  strcat(pd_out_adptarg_name,"_var,");
  strcat(pd_out_adptarg_name,pd_fieldmem_fqname);
  strcat(pd_out_adptarg_name,",");
  strcat(pd_out_adptarg_name,fqname());
  strcat(pd_out_adptarg_name,"_Helper");
  strcat(pd_out_adptarg_name," >");
}

o2be_interface_fwd::o2be_interface_fwd(UTL_ScopedName *n, UTL_StrList *p)
  : AST_InterfaceFwd(n, p),
    AST_Decl(AST_Decl::NT_interface_fwd, n, p),
    o2be_name(this)
{
}

idl_bool
o2be_interface::check_opname_clash(o2be_interface *p,char *opname)
{
  {
    UTL_ScopeActiveIterator i(p,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	switch(d->node_type()) {
	case AST_Decl::NT_op:
	  if (strcmp(opname,o2be_operation::narrow_from_decl(d)->uqname())==0)
	    return I_TRUE;
	  break;
	case AST_Decl::NT_attr:
	  {
	    char *attrop = new char[strlen(o2be_attribute::narrow_from_decl(d)->uqname())+6];
	    strcpy(attrop,"_get_");
	    strcat(attrop,o2be_attribute::narrow_from_decl(d)->uqname());
	    if (strcmp(opname,attrop)==0)
	      {
		delete [] attrop;
		return I_TRUE;
	      }
	    strcpy(attrop,"_set_");
	    strcat(attrop,o2be_attribute::narrow_from_decl(d)->uqname());
	    if (strcmp(opname,attrop)==0)
	      {
		delete [] attrop;
		return I_TRUE;
	      }
	    delete [] attrop;
	  }
	  break;
	default:
	  break;
	}
	i.next();
      }
  }
  {
    int ni,j;
    AST_Interface **intftable;
    if ((ni = p->n_inherits()) != 0)
      {
	intftable = p->inherits();
	for (j=0; j< ni; j++)
	  {
	    o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	    if (o2be_interface::check_opname_clash(intf,opname))
	      return I_TRUE;
	  }
      }
  }
  return I_FALSE;
}

void 
o2be_interface::produce_hdr(fstream &s)
{
  s << "#ifndef __" << _fqname() << "__\n";
  s << "#define __" << _fqname() << "__\n";
  IND(s); s << "class   " << uqname() << ";\n";
  IND(s); s << "typedef " << uqname() << "* " << objref_uqname() << ";\n";
  IND(s); s << "typedef " << objref_uqname() << " " << uqname() << "Ref;\n\n";
  IND(s); s << "class " << uqname() << "_Helper {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "public:\n";
  IND(s); s << "static " << objref_uqname() << " _nil();\n";
  IND(s); s << "static CORBA::Boolean is_nil(" << objref_uqname() << " p);\n";
  IND(s); s << "static void release(" << objref_uqname() << " p);\n";
  IND(s); s << "static void duplicate(" << objref_uqname() << " p);\n";
  IND(s); s << "static size_t NP_alignedSize("
	    << objref_uqname() << " obj,size_t initialoffset);\n";
  IND(s); s << "static void marshalObjRef("
	    << objref_uqname() << " obj,NetBufferedStream &s);\n";
  IND(s); s << "static "
	    << objref_uqname() 
	    << " unmarshalObjRef(NetBufferedStream &s);\n";
  IND(s); s << "static void marshalObjRef("
	    << objref_uqname() << " obj,MemBufferedStream &s);\n";
  IND(s); s << "static "
	    << objref_uqname() 
	    << " unmarshalObjRef(MemBufferedStream &s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n";
  IND(s); s << "typedef _CORBA_ObjRef_Var<"
	    << uqname()
	    << ","
	    << uqname() << "_Helper"
	    << "> "<<uqname()<<"_var;\n\n";
  s << "#endif\n";
  s << "#define " << IRrepoId() << " \"" << repositoryID() << "\"\n\n";


  IND(s); s << "class " << uqname() << " : ";
  {
    int ni,j;
    AST_Interface **intftable;
    if ((ni = n_inherits()) != 0)
      {
	intftable = inherits();
	for (j=0; j< ni; j++)
	  {
	    o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	    s << " public virtual " << intf->fqname() << ((j<(ni-1))?",":"");
	  }
      }
    else
      s << "public virtual omniObject, public virtual CORBA::Object";
    s << " {\n";
  }
  IND(s); s << "public:\n\n";
  INC_INDENT_LEVEL();
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	switch(d->node_type()) {
	case AST_Decl::NT_op:
	case AST_Decl::NT_attr:
	case AST_Decl::NT_enum_val:
	  break;
	case AST_Decl::NT_const:
	  o2be_constant::narrow_from_decl(d)->produce_hdr(s);
	  break;
	case AST_Decl::NT_enum:
	  o2be_enum::narrow_from_decl(d)->produce_hdr(s);
	  break;
	case AST_Decl::NT_except:
	  o2be_exception::narrow_from_decl(d)->produce_hdr(s);
	  break;
	case AST_Decl::NT_struct:
	  o2be_structure::narrow_from_decl(d)->produce_hdr(s);
	  break;
	case AST_Decl::NT_typedef:
	  o2be_typedef::narrow_from_decl(d)->produce_hdr(s);
	  break;
	case AST_Decl::NT_union:
	  o2be_union::narrow_from_decl(d)->produce_hdr(s);
	  break;
	default:
	  throw o2be_internal_error(__FILE__,__LINE__,"unexpected type under interface class");
	}
	i.next();
      }
  }
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    o2be_operation* op = o2be_operation::narrow_from_decl(d);

	    if (op->has_variable_out_arg() || op->has_pointer_inout_arg())
	      {
		IND(s); s << "virtual ";
		op->produce_decl(s,0,"_0RL__");
		s << " = 0;\n";
		op->produce_mapping_with_indirection(s,"_0RL__");
	      }
	    else
	      {
		IND(s); s << "virtual ";
		op->produce_decl(s);
		s << " = 0;\n";
	      }
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    IND(s); s << "virtual ";
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    a->produce_decl_rd(s);
	    s << " = 0;\n";
	    if (!a->readonly())
	      {
		IND(s); s << "virtual ";
		a->produce_decl_wr(s);
		s << " = 0;\n";
	      }
	  }
	i.next();
      }
  }
  IND(s); s << "static " << objref_uqname() << " _duplicate(" << objref_uqname() << ");\n";
  IND(s); s << "static " << objref_uqname() << " _narrow(CORBA::Object_ptr);\n";
  IND(s); s << "static " << objref_uqname() << " _nil();\n\n";

  IND(s); s << "static inline size_t NP_alignedSize("
	    << objref_uqname() << " obj,size_t initialoffset) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return CORBA::AlignedObjRef(obj,"
	    << IRrepoId() << "," << (strlen(repositoryID())+1) 
	    << ",initialoffset);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "static inline void marshalObjRef("
	    << objref_uqname() << " obj,NetBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::MarshalObjRef(obj,"
	    << IRrepoId() << "," << (strlen(repositoryID())+1) 
	    << ",s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "static inline " << objref_uqname()
	    << " unmarshalObjRef(NetBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::Object_ptr _obj = CORBA::UnMarshalObjRef("
	    << IRrepoId() << ",s);\n";
  IND(s); s << objref_uqname() << " _result = " << fqname() << 
	    "::_narrow(_obj);\n";
  IND(s); s << "CORBA::release(_obj);\n";
  IND(s); s << "return _result;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "static inline void marshalObjRef("
	    << objref_uqname() << " obj,MemBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::MarshalObjRef(obj,"
	    << IRrepoId() << "," << (strlen(repositoryID())+1) 
	    << ",s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "static inline " << objref_uqname()
	    << " unmarshalObjRef(MemBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::Object_ptr _obj = CORBA::UnMarshalObjRef("
	    << IRrepoId() << ",s);\n";
  IND(s); s << objref_uqname() << " _result = " << fqname() << 
	    "::_narrow(_obj);\n";
  IND(s); s << "CORBA::release(_obj);\n";
  IND(s); s << "return _result;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "static CORBA::Boolean _is_a(const char *base_repoId);\n\n";

  DEC_INDENT_LEVEL();
  IND(s); s << "protected:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << uqname() << "() {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (!is_proxy())\n";
  INC_INDENT_LEVEL();
  IND(s); s << "omniObject::PR_IRRepositoryId(" << IRrepoId() << ");\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "this->PR_setobj(this);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  IND(s); s << "virtual ~" << uqname() << "() {}\n";
  IND(s); s << "virtual void *_widenFromTheMostDerivedIntf(const char *repoId);\n";
  s << "\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "private:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << uqname() << "(const " << uqname() << "&);\n";
  IND(s); s << uqname() << " &operator=(const " << uqname() << "&);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  IND(s); s << "class " << server_uqname() << " : ";
  {
    AST_Interface **intftable = inherits();
    int ni = n_inherits();
    for (int j=0; j< ni; j++)
      {
	o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	s << " public virtual " << intf->server_fqname() << ",";
      }
  }
  s << " public virtual " << uqname() << " {\n";
  IND(s); s << "public:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << server_uqname() << "() {}\n";
  IND(s); s << server_uqname() << "(const omniORB::objectKey& k);\n";
  IND(s); s << "virtual ~" << server_uqname() << "() {}\n";
  IND(s); s << objref_uqname() << " _this() { return "
	    << uqname() << "::_duplicate(this); }\n";
  IND(s); s << "void _obj_is_ready(CORBA::BOA_ptr boa) { boa->obj_is_ready(this); }\n";
  IND(s); s << "CORBA::BOA_ptr _boa() { return CORBA::BOA::getBOA(); }\n";
  IND(s); s << "void _dispose() { _boa()->dispose(this); }\n";
  IND(s); s << "omniORB::objectKey _key();\n";
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    o2be_operation* op = o2be_operation::narrow_from_decl(d);
	    IND(s); s << "virtual ";
	    op->produce_decl(s);
	    s << " = 0;\n";
	    if (op->has_variable_out_arg() || op->has_pointer_inout_arg())
	      {
		IND(s); s << "virtual ";
		op->produce_decl(s,0,"_0RL__");
		s << " {\n";
		INC_INDENT_LEVEL();
		IND(s);
		if (!op->return_is_void()) {
		  s << "return ";
		}
		op->produce_invoke(s);
		s << ";\n";
		DEC_INDENT_LEVEL();
		IND(s); s << "}\n";
	      }
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    IND(s); s << "virtual ";
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    a->produce_decl_rd(s);
	    s << " = 0;\n";
	    if (!a->readonly())
	      {
		IND(s); s << "virtual ";
		a->produce_decl_wr(s);
		s << " = 0;\n";
	      }
	  }
	i.next();
      }
  }
  IND(s); s << "virtual CORBA::Boolean dispatch(GIOP_S &s,const char *op,CORBA::Boolean response);\n";
  DEC_INDENT_LEVEL();
  s << "\n";
  IND(s); s << "protected:\n";
  INC_INDENT_LEVEL();
  IND(s); s << "virtual void *_widenFromTheMostDerivedIntf(const char *repoId) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << uqname() << "::_widenFromTheMostDerivedIntf(repoId);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";	
  DEC_INDENT_LEVEL();
  IND(s); s << "private:\n";
  INC_INDENT_LEVEL();
  IND(s); s << server_uqname() << " (const " << server_uqname() << "&);\n";
  IND(s); s << server_uqname() << " &operator=(const " << server_uqname() << "&);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  IND(s); s << "class " << proxy_uqname() << " : ";
  {
    AST_Interface **intftable = inherits();
    int ni = n_inherits();
    for (int j=0; j< ni; j++)
      {
	o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	s << " public virtual " << intf->proxy_fqname() << ",";
      }
  }
  s << " public virtual " << uqname() << " {\n";
  IND(s); s << "public:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << proxy_uqname() << " (Rope *r,CORBA::Octet *key,size_t keysize,IOP::TaggedProfileList *profiles,CORBA::Boolean release) :\n";
  INC_INDENT_LEVEL();
  IND(s); s << "omniObject(" << IRrepoId() << ",r,key,keysize,profiles,release) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "omni::objectIsReady(this);\n";
  DEC_INDENT_LEVEL();
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  IND(s); s << "virtual ~" << proxy_uqname() << "() {}\n";
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    o2be_operation* op = o2be_operation::narrow_from_decl(d);
	    IND(s); s << "virtual ";
	    if (op->has_variable_out_arg() ||
		op->has_pointer_inout_arg())
	      {
		op->produce_decl(s,0,"_0RL__");
	      }
	    else
	      {
		op->produce_decl(s);
	      }
	    s << ";\n";
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    IND(s); s << "virtual ";
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    a->produce_decl_rd(s);
	    s << ";\n";
	    if (!a->readonly())
	      {
		IND(s); s << "virtual ";
		a->produce_decl_wr(s);
		s << ";\n";
	      }
	  }
	i.next();
      }
  }
  DEC_INDENT_LEVEL();
  s << "\n";
  IND(s); s << "protected:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << proxy_uqname() << " () {}\n\n";
  IND(s); s << "virtual void *_widenFromTheMostDerivedIntf(const char *repoId) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << uqname() << "::_widenFromTheMostDerivedIntf(repoId);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";	
  DEC_INDENT_LEVEL();
  IND(s); s << "private:\n\n";
  INC_INDENT_LEVEL();
  IND(s); s << proxy_uqname() << " (const " << proxy_uqname() << "&);\n";
  IND(s); s << proxy_uqname() << " &operator=(const " << proxy_uqname() << "&);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  // _nil
  IND(s); s << "class " << nil_uqname() << " : ";
  {
    int ni,j;
    AST_Interface **intftable;
    if ((ni = n_inherits()) != 0)
      {
	intftable = inherits();
	for (j=0; j< ni; j++)
	  {
	    o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	    s << " public virtual " 
	      << intf->nil_fqname() << ", ";
	  }
      }
  }
  IND(s); s << "public virtual " << uqname() << " {\n";
  IND(s); s << "public:\n";
  INC_INDENT_LEVEL();
  IND(s); s << nil_uqname() << "()"
	    << " { this->PR_setobj(0); }\n";
  IND(s); s << "virtual ~" << nil_uqname() << "() {}\n";
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    o2be_operation* op = o2be_operation::narrow_from_decl(d);
	    if (op->has_variable_out_arg() || op->has_pointer_inout_arg()) {
	      op->produce_nil_skel(s,"_0RL__");
	    }
	    else {
	      op->produce_nil_skel(s);
	    }
	    s << "\n";
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    a->produce_nil_rd_skel(s);
	    s << "\n";
	    if (!a->readonly())
	      {
		a->produce_nil_wr_skel(s);
		s << "\n";
	      }
	  }
	i.next();
      }
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "protected:\n";
  INC_INDENT_LEVEL();
  IND(s); s << "virtual void *_widenFromTheMostDerivedIntf(const char *repoId) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << uqname() << "::_widenFromTheMostDerivedIntf(repoId);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";	
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  // proxyObjectFactory
  IND(s); s << "class " << uqname() << PROXY_OBJ_FACTORY_POSTFIX
	    << " : public proxyObjectFactory {\n";
  IND(s); s << "public:\n";
  INC_INDENT_LEVEL();
  IND(s); s << uqname() << PROXY_OBJ_FACTORY_POSTFIX << " () {}\n";
  IND(s); s << "virtual ~" << uqname() << PROXY_OBJ_FACTORY_POSTFIX << " () {}\n";
  IND(s); s << "virtual const char *irRepoId() const;\n";
  IND(s); s << "virtual CORBA::Object_ptr newProxyObject(Rope *r,CORBA::Octet *key,size_t keysize,IOP::TaggedProfileList *profiles,CORBA::Boolean release);\n";
  IND(s); s << "virtual CORBA::Boolean is_a(const char *base_repoId) const;\n";
  // _nil()
  IND(s); s << "static " << objref_fqname() << " _nil() {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (!_" << nil_uqname() << ") {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "_" << nil_uqname() << " = new " << nil_fqname() << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  IND(s); s << "return _" << nil_uqname() << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "private:\n";
  INC_INDENT_LEVEL();
  IND(s); s << "static " << objref_fqname() << " _" << nil_uqname() << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n\n";

  produce_seq_hdr_if_defined(s);
  return;
}

void
o2be_interface_fwd::produce_hdr(fstream &s)
{
  o2be_interface *intf = o2be_interface::narrow_from_decl(full_definition());
  s << "#ifndef __" << intf->_fqname() << "__\n";
  s << "#define __" << intf->_fqname() << "__\n";
  IND(s); s << "class   " << intf->uqname() << ";\n";
  IND(s); s << "typedef " << intf->uqname() << "* " 
	    << intf->objref_uqname() << ";\n";
  IND(s); s << "typedef " << intf->objref_uqname() << " " 
	    << intf->uqname() << "Ref;\n\n";
  IND(s); s << "class " << intf->uqname() << "_Helper {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "public:\n";
  IND(s); s << "static " << intf->objref_uqname() << " _nil();\n";
  IND(s); s << "static CORBA::Boolean is_nil(" << intf->objref_uqname() << " p);\n";
  IND(s); s << "static void release(" << intf->objref_uqname() << " p);\n";
  IND(s); s << "static void duplicate(" << intf->objref_uqname() << " p);\n";
  IND(s); s << "static size_t NP_alignedSize("
	    << intf->objref_uqname() << " obj,size_t initialoffset);\n";
  IND(s); s << "static void marshalObjRef("
	    << intf->objref_uqname() << " obj,NetBufferedStream &s);\n";
  IND(s); s << "static "
	    << intf->objref_uqname() 
	    << " unmarshalObjRef(NetBufferedStream &s);\n";
  IND(s); s << "static void marshalObjRef("
	    << intf->objref_uqname() << " obj,MemBufferedStream &s);\n";
  IND(s); s << "static "
	    << intf->objref_uqname() 
	    << " unmarshalObjRef(MemBufferedStream &s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "};\n";
  IND(s); s << "typedef _CORBA_ObjRef_Var<"
	    << intf->uqname()
	    << ","
	    << uqname() << "_Helper"
	    << "> "<< intf->uqname()<<"_var;\n\n";
    s << "#endif\n";
  return;
}


void
o2be_interface::produce_skel(fstream &s)
{
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	switch(d->node_type()) {
	case AST_Decl::NT_op:
	case AST_Decl::NT_attr:
	case AST_Decl::NT_enum_val:
	  break;
	case AST_Decl::NT_const:
	  o2be_constant::narrow_from_decl(d)->produce_skel(s);
	  break;
	case AST_Decl::NT_enum:
	  o2be_enum::narrow_from_decl(d)->produce_skel(s);
	  break;
	case AST_Decl::NT_except:
	  o2be_exception::narrow_from_decl(d)->produce_skel(s);
	  break;
	case AST_Decl::NT_struct:
	  o2be_structure::narrow_from_decl(d)->produce_skel(s);
	  break;
	case AST_Decl::NT_typedef:
	  o2be_typedef::narrow_from_decl(d)->produce_skel(s);
	  break;
	case AST_Decl::NT_union:
	  o2be_union::narrow_from_decl(d)->produce_skel(s);
	  break;
	default:
	  throw o2be_internal_error(__FILE__,__LINE__,"unexpected type under interface class");
	}
	i.next();
      }
  }
  
  // proxy member functions
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    o2be_operation* op = o2be_operation::narrow_from_decl(d);
	    if (op->has_variable_out_arg() || op->has_pointer_inout_arg()) {
	      op->produce_proxy_skel(s,*this,"_0RL__");
	    }
	    else {
	      op->produce_proxy_skel(s,*this);
	    }
	    s << "\n";
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    a->produce_proxy_rd_skel(s,*this);
	    s << "\n";
	    if (!a->readonly())
	      {
		a->produce_proxy_wr_skel(s,*this);
		s << "\n";
	      }
	  }
	i.next();
      }
  }
  s << "\n";

  // server skeleton dispatch function
  IND(s); s << "CORBA::Boolean\n";
  IND(s); s << server_fqname() << "::dispatch(GIOP_S &_0RL_s,const char *_0RL_op,CORBA::Boolean _0RL_response_expected)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  {
    UTL_ScopeActiveIterator i(this,UTL_Scope::IK_decls);
    idl_bool notfirst = I_FALSE;
    while (!i.is_done())
      {
	AST_Decl *d = i.item();
	if (d->node_type() == AST_Decl::NT_op)
	  {
	    IND(s); s << ((notfirst)?"else ":"")
		      << "if (strcmp(_0RL_op,\""
		      << d->local_name()->get_string()
		      << "\") == 0)\n";
	    IND(s); s << "{\n";
	    INC_INDENT_LEVEL();
	    o2be_operation::narrow_from_decl(d)->produce_server_skel(s,*this);
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	    notfirst = I_TRUE;
	  }
	else if (d->node_type() == AST_Decl::NT_attr)
	  {
	    o2be_attribute *a = o2be_attribute::narrow_from_decl(d);
	    IND(s); s << ((notfirst)?"else ":"")
		      << "if (strcmp(_0RL_op,\""
		      << "_get_" << a->local_name()->get_string()
		      << "\") == 0)\n";
	    IND(s); s << "{\n";
	    INC_INDENT_LEVEL();
	    a->produce_server_rd_skel(s,*this);
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	    if (!a->readonly())
	      {
		IND(s); s << "else if (strcmp(_0RL_op,\""
			  << "_set_" << a->local_name()->get_string()
			  << "\") == 0)\n";
		IND(s); s << "{\n";
		INC_INDENT_LEVEL();
		a->produce_server_wr_skel(s,*this);
		DEC_INDENT_LEVEL();
		IND(s); s << "}\n";
	      }
	    notfirst = I_TRUE;
	  }
	i.next();
      }
    {
      AST_Interface **intftable = inherits();
      int ni = n_inherits();
      for (int j=0; j< ni; j++)
	{
	  o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	  IND(s); s << ((notfirst)?"else ":"")
		    << "if (" << intf->server_fqname() 
		    << "::dispatch(_0RL_s,_0RL_op,_0RL_response_expected)) {\n";
	  INC_INDENT_LEVEL();
	  IND(s); s << "return 1;\n";
	  DEC_INDENT_LEVEL();
	  IND(s); s << "}\n";
	  notfirst = I_TRUE;
	}
    }
    IND(s); s << ((notfirst)?"else {\n":"{\n");
    INC_INDENT_LEVEL();
    IND(s); s << "return 0;\n";
    DEC_INDENT_LEVEL();
    IND(s); s << "}\n";
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";
  

  IND(s); s << server_fqname() << "::" << server_uqname() 
	    << " (const omniORB::objectKey& k)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "omniRopeAndKey l(0,(CORBA::Octet*)&k,(CORBA::ULong)sizeof(k));\n";
  IND(s); s << "setRopeAndKey(l,0);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "omniORB::objectKey\n";
  IND(s); s << server_fqname() << "::_key()\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "omniRopeAndKey l;\n";
  IND(s); s << "getRopeAndKey(l);\n";
  IND(s); s << "return (*((omniORB::objectKey*)l.key()));\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  // _duplicate
  IND(s); s << objref_fqname() << "\n";
  IND(s); s << fqname() << "::_duplicate(" << objref_fqname() << " obj)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (CORBA::is_nil(obj))\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::_nil();\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "CORBA::Object::_duplicate(obj);\n";
  IND(s); s << "return obj;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";


  // _narrow
  IND(s); s << objref_fqname() << "\n";
  IND(s); s << fqname() << "::_narrow(CORBA::Object_ptr obj)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (CORBA::is_nil(obj))\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::_nil();\n";
  DEC_INDENT_LEVEL();
  IND(s); s << objref_fqname() << " e = (" 
	    << objref_fqname() << ") ((obj->PR_getobj())->_widenFromTheMostDerivedIntf("
	    << IRrepoId() << "));\n";
  IND(s); s << "if (e) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::Object::_duplicate(e);\n";
  IND(s); s << "return e;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  IND(s); s << "else\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::_nil();\n";
  DEC_INDENT_LEVEL();
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  // _widenFromTheMostDerivedIntf
  IND(s); s << "void *\n";
  IND(s); s << fqname() << "::_widenFromTheMostDerivedIntf(const char *repoId)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (!repoId)\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return (void *)((CORBA::Object_ptr)this);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "if (strcmp(" << IRrepoId() << ",repoId) == 0)\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return (void *)this;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "else {\n";
  INC_INDENT_LEVEL();
  {
    int ni,j;
    AST_Interface **intftable;
    if ((ni = n_inherits()) != 0)
      {
	IND(s); s << "void *_p;\n";
	intftable = inherits();
	for (j=0; j< ni; j++)
	  {
	    o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	    IND(s); s << ((j)?"else ":"") 
		      << "if ((_p = " << intf->fqname() 
		      << "::_widenFromTheMostDerivedIntf(repoId))) {\n";
	    INC_INDENT_LEVEL();
	    IND(s); s << "return _p;\n";
	    DEC_INDENT_LEVEL();
	    IND(s); s << "}\n";
	  }
	IND(s); s << "else\n";
	INC_INDENT_LEVEL();
	IND(s); s << "return 0;\n";
	DEC_INDENT_LEVEL();
      }
    else {
      IND(s); s << "return 0;\n";
    }
  }
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  // _is_a();
  IND(s); s << "CORBA::Boolean\n";
  IND(s); s << fqname() << "::_is_a(const char *base_repoId) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "if (strcmp(base_repoId,(const char *)" << IRrepoId() << ")==0)\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return 1;\n";
  DEC_INDENT_LEVEL();
  {
    AST_Interface **intftable = inherits();
    int ni = n_inherits();
    for (int j=0; j< ni; j++)
      {
	o2be_interface * intf = o2be_interface::narrow_from_decl(intftable[j]);
	IND(s); s << "else if (" 
		  << intf->fqname() << "::_is_a(base_repoId))\n";
	INC_INDENT_LEVEL();
	IND(s); s << "return 1;\n";
	DEC_INDENT_LEVEL();
      }
  }
  IND(s); s << "else\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return 0;\n";
  DEC_INDENT_LEVEL();
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  // T_Helper
  IND(s); s << objref_fqname() << "\n";
  IND(s); s << fqname() << "_Helper::_nil() {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::_nil();\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "CORBA::Boolean\n";
  IND(s); s << fqname() << "_Helper::is_nil(" << objref_fqname() << " p) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return CORBA::is_nil(p);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "void\n";
  IND(s); s << fqname() << "_Helper::release(" << objref_fqname() << " p) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::release(p);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "void\n";
  IND(s); s << fqname() << "_Helper::duplicate("<< objref_fqname() << " p) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "CORBA::Object::_duplicate(p);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "size_t\n";
  IND(s); s << fqname() << "_Helper::NP_alignedSize("
	    << objref_fqname() << " obj,size_t initialoffset) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() 
	    << "::NP_alignedSize(obj,initialoffset);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "void\n";
  IND(s); s << fqname() << "_Helper::marshalObjRef("
	    << objref_fqname() << " obj,NetBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << fqname() << "::marshalObjRef(obj,s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << objref_fqname()
	    << " "
	    << fqname() <<"_Helper::unmarshalObjRef(NetBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::unmarshalObjRef(s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << "void\n";
  IND(s); s << fqname() << "_Helper::marshalObjRef("
	    << objref_fqname() << " obj,MemBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << fqname() << "::marshalObjRef(obj,s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << objref_fqname()
	    << " "
	    << fqname() <<"_Helper::unmarshalObjRef(MemBufferedStream &s) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::unmarshalObjRef(s);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  // _irRepoId()
  IND(s); s << "const char *\n";
  IND(s); s << fqname() << PROXY_OBJ_FACTORY_POSTFIX << "::irRepoId() const\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return (const char *)" << IRrepoId() << ";\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";
  // _newProxyObject()
  IND(s); s << "CORBA::Object_ptr\n";
  IND(s); s << fqname() << PROXY_OBJ_FACTORY_POSTFIX << "::newProxyObject(Rope *r,CORBA::Octet *key,size_t keysize,IOP::TaggedProfileList *profiles,CORBA::Boolean release)\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << proxy_fqname() << " *p = new " << proxy_fqname()
	    << "(r,key,keysize,profiles,release);\n";
  IND(s); s << "if (!p) {\n";
  INC_INDENT_LEVEL();
  IND(s); s << "throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n";
  IND(s); s << "return (CORBA::Object_ptr) p;\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";
  // _is_a()
  IND(s); s << "CORBA::Boolean\n";
  IND(s); s << fqname() << PROXY_OBJ_FACTORY_POSTFIX << "::is_a(const char *base_repoId) const\n";
  IND(s); s << "{\n";
  INC_INDENT_LEVEL();
  IND(s); s << "return " << fqname() << "::_is_a(base_repoId);\n\n";
  DEC_INDENT_LEVEL();
  IND(s); s << "}\n\n";

  IND(s); s << objref_fqname() << "\n";
  IND(s); s << fqname() << "::_nil() {\n";
  INC_INDENT_LEVEL()
    IND(s); s << "return " << fqname() 
	      << PROXY_OBJ_FACTORY_POSTFIX << "::_nil();\n";
  DEC_INDENT_LEVEL()
  IND(s); s << "}\n\n";

  // single const instance
  IND(s); s << "static const " << fqname() << PROXY_OBJ_FACTORY_POSTFIX
	    << " " << _fqname() << PROXY_OBJ_FACTORY_POSTFIX << ";\n";
  IND(s); s << objref_fqname() << " " 
	    << fqname() << PROXY_OBJ_FACTORY_POSTFIX << "::_"
	    << nil_uqname() << " = 0;\n\n";
}

void
o2be_interface::produce_typedef_hdr(fstream &s, o2be_typedef *tdef)
{
  IND(s); s << "typedef " << fqname() << " " << tdef->uqname() << ";\n";
  IND(s); s << "typedef " << objref_fqname() << " " << tdef->uqname() << "_ptr;\n";
  IND(s); s << "typedef " << fqname() << "Ref " << tdef->uqname() << "Ref;\n";
  if (strcmp(uqname(),"Object") != 0) {
    IND(s); s << "typedef " << fqname() << "_Helper " << tdef->uqname() << "_Helper;\n";
    IND(s); s << "typedef " << proxy_fqname()
	      << " " << PROXY_CLASS_PREFIX << tdef->uqname() << ";\n";
    IND(s); s << "typedef " << server_fqname()
	      << " " << SERVER_CLASS_PREFIX << tdef->uqname() << ";\n";
    IND(s); s << "typedef " << nil_fqname()
	      << " " << NIL_CLASS_PREFIX << tdef->uqname() << ";\n";
    s << "#define " << tdef->_fqname() << IRREPOID_POSTFIX << " " << IRrepoId()
      << ";\n";
  }
  IND(s); s << "typedef " << fqname() << "_var " << tdef->uqname() << "_var;\n";
}


IMPL_NARROW_METHODS1(o2be_interface, AST_Interface)
IMPL_NARROW_FROM_DECL(o2be_interface)
IMPL_NARROW_FROM_SCOPE(o2be_interface)
IMPL_NARROW_METHODS1(o2be_interface_fwd, AST_InterfaceFwd)
IMPL_NARROW_FROM_DECL(o2be_interface_fwd)
IMPL_NARROW_FROM_SCOPE(o2be_interface_fwd)
IMPL_NARROW_METHODS1(o2be_argument, AST_Argument)
IMPL_NARROW_FROM_DECL(o2be_argument)

o2be_argument::o2be_argument(AST_Argument::Direction d, AST_Type *ft,
			     UTL_ScopedName *n, UTL_StrList *p)
	   : AST_Argument(d, ft, n, p),
	     AST_Field(AST_Decl::NT_argument, ft, n, p),
	     AST_Decl(AST_Decl::NT_argument, n, p),
             o2be_name(this)
{
}
