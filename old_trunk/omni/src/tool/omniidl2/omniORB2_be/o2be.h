// -*- Mode: C++; -*-
//                          Package   : omniidl
// o2be.h                   Created on: 5/8/1996
//			    Author    : Sai-Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
//    Defines all classes for omniORB2 backend

/*
 $Log$
 */

#ifndef _O2BE_CLASSES_H_
#define _O2BE_CLASSES_H_

#include <fstream.h>
#include <string.h>

#define USE_SEQUENCE_TEMPLATE_IN_PLACE 1

class o2be_name 
{
public:
  o2be_name(AST_Decl *decl);

  // fully qualified name of the scope
  virtual char *scopename() { return pd_scopename; }
                    
  // un-qualified name
  virtual char *uqname()    { return pd_uqname; }

  // fully qualified name, scope name + un-qualified name
  virtual char *fqname()    { return pd_fqname; }

  // same as scopename but with '_' as the separator
  virtual char *_scopename() { return pd__scopename; }

  // same as fqname but with '_' as the separator  
  virtual char *_fqname()    { return pd__fqname; }

  virtual char *repositoryID() { return pd_repositoryID; }

  void set_scopename(char *n) { pd_scopename = n; }
  void set_uqname(char *n)    { pd_uqname = n; }
  void set_fqname(char *n)    { pd_fqname = n; }                    
  void set__scopename(char *n){ pd__scopename = n; }
  void set__fqname(char *n)   { pd__fqname = n; }

  static char *narrow_and_produce_fqname(AST_Decl *type);
  static char *narrow_and_produce__fqname(AST_Decl *type);
  static char *narrow_and_produce_scopename(AST_Decl *type);
  static char *narrow_and_produce__scopename(AST_Decl *type);
  static char *narrow_and_produce_uqname(AST_Decl *type);

private:
  o2be_name();

  char *pd_scopename;
  char *pd_uqname;
  char *pd_fqname;
  char *pd__scopename;
  char *pd__fqname;
  char *pd_repositoryID;
};

class o2be_sequence;
class o2be_sequence_chain {
public:
  o2be_sequence_chain(AST_Decl *decl) { pd_seq_decl = 0; pd_decl = decl; }
  virtual ~o2be_sequence_chain() {}
  void set_seq_decl(o2be_sequence *d);
  o2be_sequence *get_seq_decl()  { return pd_seq_decl; }
  void produce_seq_hdr_if_defined(fstream &s);
private:
  o2be_sequence *pd_seq_decl;
  AST_Decl *pd_decl;
  o2be_sequence_chain();
};

class o2be_predefined_type : public virtual AST_PredefinedType,
			     public virtual o2be_name,
			     public virtual o2be_sequence_chain
{
public:

  o2be_predefined_type(AST_PredefinedType::PredefinedType t, 
		       UTL_ScopedName *n,
		       UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_predefined_type, AST_PredefinedType);
  DEF_NARROW_FROM_DECL(o2be_predefined_type);

private:
  o2be_predefined_type();

};

class o2be_constant : public virtual AST_Constant,
		      public virtual o2be_name
{
public:

  o2be_constant(AST_Expression::ExprType et,
		AST_Expression *v,
		UTL_ScopedName *n,
		UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_constant, AST_Constant);
  DEF_NARROW_FROM_DECL(o2be_constant);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

private:
  o2be_constant();

};

class o2be_enum : public virtual AST_Enum,
		  public virtual o2be_name,
		  public virtual o2be_sequence_chain
{
public:

  o2be_enum(UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_enum, AST_Enum);
  DEF_NARROW_FROM_DECL(o2be_enum);
  DEF_NARROW_FROM_SCOPE(o2be_enum);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  void set_hdr_produced_in_field() { pd_hdr_produced_in_field = I_TRUE; }
  idl_bool get_hdr_produced_in_field() { return pd_hdr_produced_in_field; }
  void set_skel_produced_in_field() { pd_skel_produced_in_field = I_TRUE; }
  idl_bool get_skel_produced_in_field() { return pd_skel_produced_in_field; }

private:
  idl_bool pd_hdr_produced_in_field;
  idl_bool pd_skel_produced_in_field;
  o2be_enum();
};

class o2be_enum_val : public virtual AST_EnumVal,
		      public virtual o2be_name 
{
public:

  o2be_enum_val(unsigned long v, UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_enum_val, AST_EnumVal);
  DEF_NARROW_FROM_DECL(o2be_enum_val);

private:
  o2be_enum_val();

};

class o2be_string : public virtual AST_String,
		    public virtual o2be_name,
		    public virtual o2be_sequence_chain
{
public:

  o2be_string(AST_Expression *v);
  o2be_string(AST_Expression *v, long wide);

  DEF_NARROW_METHODS1(o2be_string, AST_String);
  DEF_NARROW_FROM_DECL(o2be_string);

  static const char *fieldMemberTypeName();

  size_t max_length();

private:
  o2be_string();

};

class o2be_field : public virtual AST_Field,
		   public virtual o2be_name
{
public:

  o2be_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_field, AST_Field);
  DEF_NARROW_FROM_DECL(o2be_field);

private:
  o2be_field();

};

class o2be_union : public virtual AST_Union,
		   public virtual o2be_name,
		   public virtual o2be_sequence_chain

{
public:

  o2be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_union, AST_Union);
  DEF_NARROW_FROM_DECL(o2be_union);
  DEF_NARROW_FROM_SCOPE(o2be_union);

  virtual AST_UnionBranch *add_union_branch(AST_UnionBranch *un);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  idl_bool isVariable() { return pd_isvar; }
  idl_bool nodefault() { return pd_nodefault; }
  idl_bool no_missing_disc_value();

  void set_hdr_produced_in_field() { pd_hdr_produced_in_field = I_TRUE; }
  idl_bool get_hdr_produced_in_field() { return pd_hdr_produced_in_field; }
  void set_skel_produced_in_field() { pd_skel_produced_in_field = I_TRUE; }
  idl_bool get_skel_produced_in_field() { return pd_skel_produced_in_field; }

private:
  idl_bool pd_hdr_produced_in_field;
  idl_bool pd_skel_produced_in_field;
  idl_bool pd_isvar;
  idl_bool pd_nodefault;
  o2be_union();

};

class o2be_union_branch : public virtual AST_UnionBranch,
			  public virtual o2be_field
{
public:

  o2be_union_branch(AST_UnionLabel *lab, AST_Type *ft, UTL_ScopedName *n,
		  UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_union_branch, AST_UnionBranch);
  DEF_NARROW_FROM_DECL(o2be_union_branch);

private:
  o2be_union_branch();

};

class o2be_structure : public virtual AST_Structure, 
		       public virtual o2be_name,
		       public virtual o2be_sequence_chain
{
public:

  o2be_structure(UTL_ScopedName *n, UTL_StrList *p);

  virtual AST_Field *add_field(AST_Field *f);

  DEF_NARROW_METHODS1(o2be_structure, AST_Structure);
  DEF_NARROW_FROM_DECL(o2be_structure);
  DEF_NARROW_FROM_SCOPE(o2be_structure);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  idl_bool isVariable() { return pd_isvar; }

  void set_hdr_produced_in_field() { pd_hdr_produced_in_field = I_TRUE; }
  idl_bool get_hdr_produced_in_field() { return pd_hdr_produced_in_field; }
  void set_skel_produced_in_field() { pd_skel_produced_in_field = I_TRUE; }
  idl_bool get_skel_produced_in_field() { return pd_skel_produced_in_field; }

private:
  idl_bool pd_hdr_produced_in_field;
  idl_bool pd_skel_produced_in_field;
  idl_bool pd_isvar;
  o2be_structure();

};

class o2be_exception : public virtual AST_Exception,
		       public virtual o2be_name 
{
public:

  o2be_exception(UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_exception, AST_Exception);
  DEF_NARROW_FROM_DECL(o2be_exception);
  DEF_NARROW_FROM_SCOPE(o2be_exception);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  size_t repoIdConstLen() const { return pd_repoidsize; }
  const char *repoIdConstName() const { return pd_repoid; }

private:
  char *pd_repoid;
  size_t pd_repoidsize;
  o2be_exception();

};

class o2be_typedef;
class o2be_array : public virtual AST_Array,
		   public virtual o2be_name
{
public:

  o2be_array(UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims);

  DEF_NARROW_METHODS1(o2be_array, AST_Array);
  DEF_NARROW_FROM_DECL(o2be_array);

  idl_bool isVariable() { return pd_isvar; }

  size_t getSliceDim();
  // Get the array slice dimension

  AST_Decl *getElementType();
  // returns the element type. If the element is an array, returns the
  // value returned by the recursive call to getElementType().

  size_t   getNumOfElements();
  // return the total number of elements. This is the sum of all the
  // dimensions. If the element is an array, multiply by the
  // value returned by the recursive call to getNumOfElements().

  size_t   getNumOfDims();
  // return the number of dimensions. If the element is an array, add
  // the value returned by the recursive call to getNumOfDims();

  class dim_iterator {
  public:
    dim_iterator(o2be_array *);
    ~dim_iterator() {};
    size_t operator() ();
    // iterates and returns the value of each dimension. If the element is
    // an array, also returns the dimensions of the array element.
  private:
    size_t pd_ndim;
    size_t pd_next;
    AST_Expression **pd_dims;
  };

  void produce_typedef_hdr (fstream &s, o2be_typedef *tdef);
  void produce_typedef_in_union(fstream &s, const char *tname);
  void produce_typedef_skel(fstream &s, o2be_typedef *tdef);
  void produce_struct_member_decl (fstream &s, AST_Decl *fieldtype);
  void produce_union_member_decl (fstream &s, AST_Decl *fieldtype);

private:
  idl_bool pd_isvar;
  o2be_array();
  void _produce_member_decl(fstream &s, char *varname);

};

class o2be_sequence : public virtual AST_Sequence,
		      public virtual o2be_name,
		      public virtual o2be_sequence_chain
{
public:

  o2be_sequence(AST_Expression *v, AST_Type *bt);
  ~o2be_sequence() {}

  char* seq_template_name() { return pd_seq_template_name; }

  DEF_NARROW_METHODS1(o2be_sequence, AST_Sequence);
  DEF_NARROW_FROM_DECL(o2be_sequence);
  DEF_NARROW_FROM_SCOPE(o2be_sequence);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  static void produce_hdr_for_predefined_types(fstream &s);
  static AST_Sequence *attach_seq_to_base_type(AST_Sequence *se);

private:
  char *pd_seq_template_name;
  o2be_sequence();
  enum seqnametype { EFFECTIVE_TYPE, IMMEDIATE_TYPE };
  char *pd_effname;
  static char *internal_produce_seqname(AST_Decl *basetype,
					enum seqnametype stype);
  char *internal_produce_localname(enum seqnametype stype);
};


class o2be_argument : public virtual AST_Argument,
		      public virtual o2be_name
{
public:
  o2be_argument(AST_Argument::Direction d, AST_Type *ft, UTL_ScopedName *n,
		UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_argument, AST_Argument);
  DEF_NARROW_FROM_DECL(o2be_argument);
private:
  o2be_argument();
};

class o2be_interface;
class o2be_attribute : public virtual AST_Attribute,
		       public virtual o2be_name
{
public:

  o2be_attribute(idl_bool ro, AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_attribute, AST_Attribute);
  DEF_NARROW_FROM_DECL(o2be_attribute);
  DEF_NARROW_FROM_SCOPE(o2be_attribute);

  void produce_decl_rd(fstream &s,
		       const char *prefix = 0,
		       idl_bool out_var_default = I_TRUE);

  void produce_decl_wr(fstream &s,
		       const char *prefix = 0,
		       idl_bool out_var_default = I_TRUE);

  void produce_proxy_rd_skel(fstream &s,o2be_interface &defined_in);
  // produce the definition of the proxy's method to get this attribute

  void produce_proxy_wr_skel(fstream &s,o2be_interface &defined_in);
  // produce the definition of the proxy's method to set this attribute

  void produce_server_rd_skel(fstream &s,o2be_interface &defined_in);
  // produce the code fragment within the server's dispatch routine
  // to handle getting this attribute

  void produce_server_wr_skel(fstream &s,o2be_interface &defined_in);
  // produce the code fragment within the server's dispatch routine
  // to handle setting this attirbute

  void produce_nil_rd_skel(fstream &s);
  // produce the definition of the nil object's method to get this attribute

  void produce_nil_wr_skel(fstream &s);
  // produce the definition of the nil object's method to set this attribute

private:
  o2be_attribute();
};

class o2be_operation : public virtual AST_Operation,
		       public virtual o2be_name 
{
public:

  o2be_operation(AST_Type *rt, AST_Operation::Flags fl,
		 UTL_ScopedName *n,UTL_StrList *p);
  ~o2be_operation() {}

  DEF_NARROW_METHODS1(o2be_operation, AST_Operation);
  DEF_NARROW_FROM_DECL(o2be_operation);
  DEF_NARROW_FROM_SCOPE(o2be_operation);

  void produce_decl(fstream &s,
		    const char *prefix = 0,
		    idl_bool out_var_default = I_TRUE);
  // produce the declaration of the mapping of this operation

  void produce_proxy_skel(fstream &s,o2be_interface &defined_in);
  // produce the definition of the proxy's method to invoke this
  // operation

  void produce_server_skel(fstream &s,o2be_interface &defined_in);
  // produce the code fragment within the server's dispatch routine
  // to handle this operation

  void produce_nil_skel(fstream &s);
  // produce the definition of the nil object's method

  friend o2be_attribute;
  friend o2be_structure;
  friend o2be_sequence;
  friend o2be_union;
  friend o2be_exception;

  enum argType {
    tShort = 0, tLong = 1, tUShort = 2, tULong = 3, tFloat = 4, tDouble = 5,
    tBoolean = 6, tChar = 7, tOctet = 8, tEnum = 9, tObjref = 10,
    tStructFixed = 11, tStructVariable = 12, tUnionFixed = 13,
    tUnionVariable = 14, tString = 15, tSequence = 16, tArrayFixed = 17,
    tArrayVariable = 18, tAny = 19, tObjrefMember = 20, tStringMember = 21,
    _tMaxValue
  };

  struct argMapping {
    idl_bool is_const;
    idl_bool is_reference;
    idl_bool is_pointer;
    idl_bool is_arrayslice;
  };

  enum argWhich {
    wResult, wIN, wOUT, wINOUT
  };

private:
  o2be_operation();
  void produce_invoke(fstream &s);
  // produce an invocation. Use the argument names as defined in the
  // IDL.

  idl_bool return_is_void();
  // returns I_TRUE if the return value of this operation is void

  idl_bool no_user_exception();
  // returns I_TRUE if this operation does not raise a user exception

  static 
  argType ast2ArgMapping(AST_Decl *decl,argWhich dir,argMapping &mapping);

  static 
  void declareVarType(fstream &s, AST_Decl *decl, idl_bool is_var=0,
		      idl_bool is_arrayslice=0);

  static
  void produceUnMarshalCode(fstream &s, AST_Decl *decl,
			    const char *netstream,
			    const char *argname,
			    argType type, argMapping mapping,
			    idl_bool no_size_check=0);

  static
  void produceMarshalCode(fstream &s, AST_Decl *decl,
			  const char *netstream,
			  const char *argname,
			  argType type, argMapping mapping);

  static
  void produceSizeCalculation(fstream &s, AST_Decl *decl,
			      const char *netstream,
			      const char *sizevar,
			      const char *argname,
			      argType type, argMapping mapping);

  static
  void produceConstStringMarshalCode(fstream &s,
				     const char *netstream,
				     const char *str,size_t len);

  static
  void produceConstStringSizeCalculation(fstream &s,
					 const char *sizevar,
					 size_t len);
};

class o2be_typedef : public virtual AST_Typedef,
		     public virtual o2be_name,
		     public virtual o2be_sequence_chain
{
public:

  o2be_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_typedef, AST_Typedef);
  DEF_NARROW_FROM_DECL(o2be_typedef);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);
  const char *fieldMemberType_uqname() const { return pd_fm_uqname; }
  const char *fieldMemberType_fqname() const { return pd_fm_fqname; }

private:
  char *pd_fm_uqname;
  char *pd_fm_fqname;
  o2be_typedef();

};

class o2be_interface : public virtual AST_Interface,
		       public virtual o2be_name,
		       public virtual o2be_sequence_chain
{
public:

  o2be_interface(UTL_ScopedName *n, AST_Interface **ih, long nih,
	       UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_interface, AST_Interface);
  DEF_NARROW_FROM_DECL(o2be_interface);
  DEF_NARROW_FROM_SCOPE(o2be_interface);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

  const char *objref_uqname() const { return pd_objref_uqname; }
  const char *objref_fqname() const { return pd_objref_fqname; }
  const char *proxy_uqname() const { return pd_proxy_uqname; }
  const char *proxy_fqname() const { return pd_proxy_fqname; }
  const char *server_uqname() const { return pd_server_uqname; }
  const char *server_fqname() const { return pd_server_fqname; }
  const char *fieldMemberType_uqname() const { return pd_fieldmem_uqname; }
  const char *fieldMemberType_fqname() const { return pd_fieldmem_fqname; }
  const char *nil_uqname() const { return pd_nil_uqname; }
  const char *nil_fqname() const { return pd_nil_fqname; }
  const char *IRrepoId() const { return pd_IRrepoId; }
  const size_t IRrepoIdSize() const { return pd_IRrepoIdSize; }

private:
  char *pd_objref_uqname;
  char *pd_objref_fqname;
  char *pd_proxy_uqname;
  char *pd_proxy_fqname;
  char *pd_server_uqname;
  char *pd_server_fqname;
  char *pd_fieldmem_uqname;
  char *pd_fieldmem_fqname;
  char *pd_nil_uqname;
  char *pd_nil_fqname;
  char *pd_IRrepoId;
  size_t pd_IRrepoIdSize;
  o2be_interface();
};

class o2be_interface_fwd : public virtual AST_InterfaceFwd,
			   public virtual o2be_name
{
public:

  o2be_interface_fwd(UTL_ScopedName *n, UTL_StrList *p);

  DEF_NARROW_METHODS1(o2be_interface_fwd, AST_InterfaceFwd);
  DEF_NARROW_FROM_DECL(o2be_interface_fwd);
  DEF_NARROW_FROM_SCOPE(o2be_interface_fwd);

  void produce_hdr(fstream &s);

private:
  o2be_interface_fwd();

};

class o2be_module : public virtual AST_Module,
		    public virtual o2be_name
{
public:
  o2be_module(UTL_ScopedName *n, UTL_StrList *p) 
                 : AST_Decl(AST_Decl::NT_module, n, p),
		   UTL_Scope(AST_Decl::NT_module),
		   o2be_name(this) {}
  ~o2be_module() {}

  DEF_NARROW_METHODS1(o2be_module, AST_Module);
  DEF_NARROW_FROM_DECL(o2be_module);
  DEF_NARROW_FROM_SCOPE(o2be_module);

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);

private:
  o2be_module();

};

class o2be_root : public virtual AST_Root,
		  public virtual o2be_module 
{
public:

  o2be_root(UTL_ScopedName *n, UTL_StrList *p);
  ~o2be_root() {}

  virtual AST_Sequence *add_sequence(AST_Sequence *s);

  DEF_NARROW_METHODS1(o2be_root, AST_Root);
  DEF_NARROW_FROM_DECL(o2be_root);
  DEF_NARROW_FROM_SCOPE(o2be_root);

  void produce();

private:
  o2be_root();

  char *basename;
  int   baselen;
  fstream pd_hdr;
  fstream pd_skel;

  void produce_hdr(fstream &s);
  void produce_skel(fstream &s);
};



#define DEFAULT_IDL_HDR_SUFFIX ".hh"
#define DEFAULT_IDL_SKEL_SUFFIX "SK.cc"
#define DEFAULT_IDL_SUFFIXLEN   5    // Max. of HDR_ and SKEL_SUFFIX

class o2be_global {
private:
  static o2be_root *myself;
  static char     *pd_hdrsuffix;
  static char     *pd_skelsuffix;
  static size_t    pd_suffixlen;
  static int       pd_aflag;      // generate stub for 'any' type
  static int       pd_fflag;      // generate stub for float and double

public:
  static void set_aflag(int f) { pd_aflag = f; }
  static int aflag() { return pd_aflag; }

  static void set_fflag(int f) { pd_fflag = f; }
  static int fflag() { return pd_fflag; }

  static int suffixlen() { return pd_suffixlen; }

  static void set_hdrsuffix(char *h) { 
    pd_hdrsuffix = new char[strlen(h)+1];
    if (strlen(h) > pd_suffixlen)
      pd_suffixlen = strlen(h);
    strcpy(pd_hdrsuffix,h);
    return; 
  }

  static char *hdrsuffix() { return pd_hdrsuffix; }

  static void set_skelsuffix(char *c) { 
    pd_skelsuffix = new char[strlen(c)+1];
    if (strlen(c) > pd_suffixlen)
      pd_suffixlen = strlen(c);
    strcpy(pd_skelsuffix,c);
    return; 
  }

  static char *skelsuffix() { return pd_skelsuffix; }

  static void set_root(o2be_root *v) { myself = v; }
  static o2be_root *root() { return myself; }
};

#define INC_INDENT_LEVEL()  idl_global->indent()->increase();
#define DEC_INDENT_LEVEL()  idl_global->indent()->decrease();
#define IND(s) idl_global->indent()->skip_to(s);


class o2be_generator : public AST_Generator {
public:
  virtual AST_Root *create_root(UTL_ScopedName *n,
				UTL_StrList *p);

  virtual AST_PredefinedType * 
          create_predefined_type(AST_PredefinedType::PredefinedType t,
				 UTL_ScopedName *n,
				 UTL_StrList *p);

  virtual AST_Module *
          create_module(UTL_ScopedName *n, UTL_StrList *p);

  virtual AST_Interface	*
          create_interface(UTL_ScopedName *n,
			   AST_Interface **ih,
			   long nih,
			   UTL_StrList *p);

  virtual AST_InterfaceFwd *
          create_interface_fwd(UTL_ScopedName *n,
			       UTL_StrList *p);

  virtual AST_Exception	*
          create_exception(UTL_ScopedName *n, UTL_StrList *p);

  virtual AST_Structure	*
          create_structure(UTL_ScopedName *n, UTL_StrList *p);

  virtual AST_Enum *
          create_enum(UTL_ScopedName *n, UTL_StrList *p);

  virtual AST_Operation	*
          create_operation(AST_Type *rt,
			   AST_Operation::Flags fl,
			   UTL_ScopedName *n,
			   UTL_StrList *p);

  virtual AST_Field *
          create_field(AST_Type *ft, 
		       UTL_ScopedName *n,
		       UTL_StrList *p);

  virtual AST_Argument *
          create_argument(AST_Argument::Direction d,
			  AST_Type *ft,
			  UTL_ScopedName *n,
			  UTL_StrList *p);

  virtual AST_Attribute	*
          create_attribute(idl_bool ro,
			   AST_Type *ft,
			   UTL_ScopedName *n,
			   UTL_StrList *p);

  virtual AST_Union *
          create_union(AST_ConcreteType *dt,
		       UTL_ScopedName *n,
		       UTL_StrList *p);

  virtual AST_UnionBranch *
          create_union_branch(AST_UnionLabel *lab,
			      AST_Type *ft,
			      UTL_ScopedName *n,
			      UTL_StrList *p);

  virtual AST_Constant *
          create_constant(AST_Expression::ExprType et,
			  AST_Expression *ev,
			  UTL_ScopedName *n,
			  UTL_StrList *p);

  virtual AST_EnumVal *
          create_enum_val(unsigned long v,
			  UTL_ScopedName *n,
			  UTL_StrList *p);

  virtual AST_Array *
          create_array(UTL_ScopedName *n,
		       unsigned long ndims,
		       UTL_ExprList *dims);

  virtual AST_Sequence *
          create_sequence(AST_Expression *v, AST_Type *bt);

  virtual AST_String *
          create_string(AST_Expression *v);

  virtual AST_String *
          create_wstring(AST_Expression *v);

  virtual AST_Typedef *
          create_typedef(AST_Type *bt,
			 UTL_ScopedName *n,
			 UTL_StrList *p);
};

class o2be_unsupported {
public:
  o2be_unsupported(const char *idlfile,int line,const char *msg) {
    pd_file = idlfile;
    pd_line = line;
    pd_msg = msg;
  }
  ~o2be_unsupported() {}
  const char *file() const { return pd_file; }
  int line() const { return pd_line; }
  const char *msg() const { return pd_msg; }
private:
  const char *pd_file;
  int	pd_line;
  const char *pd_msg;
  o2be_unsupported();
};

class o2be_internal_error {
public:
  o2be_internal_error(const char *file,int line,const char *errmsg) {
    pd_file = file;
    pd_line = line;
    pd_errmsg = errmsg;
  }
  ~o2be_internal_error(){}
  const char *file() const { return pd_file; }
  int line() const { return pd_line; }
  const char *errmsg() const { return pd_errmsg; }
private:
  const char *pd_file;
  int	      pd_line;
  const char *pd_errmsg;
  o2be_internal_error();
};

class o2be_fileio_error {
public:
  o2be_fileio_error(const char *errmsg) {
    pd_errmsg = errmsg;
  }
  ~o2be_fileio_error() {}
  const char *errmsg() const { return pd_errmsg; }
private:
  const char *pd_errmsg;
  o2be_fileio_error();
};

#endif
