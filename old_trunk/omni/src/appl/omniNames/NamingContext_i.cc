#include <iostream.h>
#include <assert.h>
#include "NamingContext_i.h"
#include "ObjectBinding.h"
#include "BindingIterator_i.h"

#ifdef DEBUG_NC
#define DB(x) x
#else
#define DB(x)
#endif

ReadersWritersLock NamingContext_i::lock;
NamingContext_i* NamingContext_i::headContext = (NamingContext_i*)0;
NamingContext_i* NamingContext_i::tailContext = (NamingContext_i*)0;


//
// Ctor.
//

NamingContext_i::NamingContext_i(CORBA::BOA_ptr boa,
				 const omniORB::objectKey& k, log* l)
  : CosNaming::_sk_NamingContext(k), redolog(l)
{
  headBinding = tailBinding = (ObjectBinding*)0;
  size = 0;

  WriterLock w(lock);

  prev = tailContext;
  next = (NamingContext_i*)0;
  tailContext = this;
  if (prev) {
    prev->next = this;
  } else {
    headContext = this;
  }

  _obj_is_ready(boa);

  redolog->create(k);
}


//
// new_context().
//

CosNaming::NamingContext_ptr
NamingContext_i::new_context()
{
  omniORB::objectKey k;
  omniORB::generateNewKey(k);

  NamingContext_i* nc = new NamingContext_i(_boa(), k, redolog);

  return nc->_this();
}



//
// resolve_simple() returns the ObjectBinding for a given simple name.
// The thread calling this must have called either lock.readerIn() or
// lock.writerIn() before calling this routine.
//

ObjectBinding* 
NamingContext_i::resolve_simple(const CosNaming::Name& n)
{
  assert(n.length() == 1);

  DB(cerr << "  resolve_simple name (" << n[0].id << "," << n[0].kind << ")"
     << " in context " << this << endl);

  for (ObjectBinding* ob = headBinding; ob; ob = ob->next) {

    assert(ob->binding.binding_name.length() == 1);

    if ((strcmp(n[0].id,ob->binding.binding_name[0].id) == 0) &&
	(strcmp(n[0].kind,ob->binding.binding_name[0].kind) == 0))
      {
	DB(cerr << "  resolve_simple: found (" << n[0].id << "," << n[0].kind
	   << ")" << " in context " << this << ", bound to "
	   << ob->object << endl);

	return ob;
      }
  }

  DB(cerr << "  resolve_simple: didn't find (" << n[0].id << "," << n[0].kind
     << ")" << " in context " << this << "; raising exception"
     << endl);

  throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::
					   missing_node, n);
}


//
// resolve_compound() returns an object reference for the first component of
// the given name (which must be a context), and the rest of the name.
//

CosNaming::NamingContext_ptr
NamingContext_i::resolve_compound(const CosNaming::Name& n,
				  CosNaming::Name& restOfName)
{
  DB(cerr << "  resolve_compound name ");
  for (int j = 0; j < n.length(); j++) {
    DB(cerr << "(" << n[j].id << "," << n[j].kind << ")");
  }
  DB(cerr << " in context " << this << endl);

  if (n.length() == 0) {
    throw CosNaming::NamingContext::InvalidName();
  }

  CosNaming::Name contextName = n;
  contextName.length(1);
  restOfName.length(n.length() - 1);
  for (int i = 0; i < n.length() - 1; i++) {
    restOfName[i] = n[i + 1];
  }

  ObjectBinding* ob;

  ReaderLock r(lock);

  try {
    ob = resolve_simple(contextName);
  }
  catch (CosNaming::NamingContext::NotFound& ex) {
    ex.rest_of_name = n;
    throw;
  }

  CosNaming::NamingContext_ptr context
    = CosNaming::NamingContext::_narrow(ob->object);

  if (CORBA::is_nil((const CosNaming::NamingContext_ptr)context) ||
      (ob->binding.binding_type != CosNaming::ncontext))
  {
    DB(cerr << "  resolve_compound: object " << ob->object
       << " not (bound as) a context; raising exception" << endl);
    throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::
					     not_context, n);
  }

  return context;
}


//
// resolve.
//

CORBA::Object_ptr
NamingContext_i::resolve(const CosNaming::Name& n)
{
  if (n.length() == 1) {
    DB(cerr << "resolve simple name (" << n[0].id << "," << n[0].kind
       << ") in context " << this << endl);

    ReaderLock r(lock);

    ObjectBinding* ob = resolve_simple(n);

    DB(cerr << "returning " << ob->object << endl);

    return CORBA::Object::_duplicate(ob->object);

  } else {

    DB(cerr << "resolve compound name ");
    for (int i = 0; i < n.length(); i++) {
      DB(cerr << "(" << n[i].id << "," << n[i].kind << ")");
    }
    DB(cerr << " in context " << this << endl);

    CosNaming::Name restOfName;
    CosNaming::NamingContext_var context = resolve_compound(n, restOfName);

    return context->resolve(restOfName);
  }
}


//
// bind_helper implements the 4 flavours of bind ([re]bind[_context]).
//

void
NamingContext_i::bind_helper(const CosNaming::Name& n, CORBA::Object_ptr obj,
			     CosNaming::BindingType t, CORBA::Boolean rebind)
{
  DB(cerr << "bind in context " << this << " (type " << t << " rebind "
     << (int)rebind << ")" << endl);

  if (n.length() == 1) {

    //
    // Bind a simple name - i.e. bind object in this context.
    //

    DB(cerr << "  bind simple name (" << n[0].id << "," << n[0].kind << ") to "
       << obj << " in context " << this << endl);

    WriterLock w(lock);

    try {

      ObjectBinding* ob = resolve_simple(n);
      if (!rebind)
	throw CosNaming::NamingContext::AlreadyBound();
      DB(cerr << "  rebind in context " << this
	 << ": unbinding simple name (" << n[0].id << "," << n[0].kind
	 << ") from " << ob->object << endl);
      delete ob;
    }
    catch (CosNaming::NamingContext::NotFound& ex) {
      DB(cerr << "  bind in context " << this
	 << ": caught not found exception from resolving simple name\n"
	 << "    reason " << ex.why << " rest of name ");

      for (int i = 0; i < ex.rest_of_name.length(); i++) {
	DB(cerr << "(" << ex.rest_of_name[i].id << ","
	   << ex.rest_of_name[i].kind << ")");
      }
      DB(cerr << endl);
    }

    new ObjectBinding(n, t, CORBA::Object::_duplicate(obj), this);

    CosNaming::NamingContext_var nc = _this();
    redolog->bind(nc, n, obj, t);

    DB(cerr << "  bind in context " << this << ": bound simple name ("
       << n[0].id << "," << n[0].kind << ") to " << obj << endl);

  } else {

    //
    // Bind a compound name <c1;c2;...;cn> - i.e. bind object to name
    // <c2;...;cn> in the context <c1>.
    //

    DB(cerr << "  bind compound name ");
    for (int i = 0; i < n.length(); i++) {
      DB(cerr << "(" << n[i].id << "," << n[i].kind << ")");
    }
    DB(cerr << " to " << obj << " in context " << this << endl);

    CosNaming::Name restOfName;
    CosNaming::NamingContext_var context = resolve_compound(n, restOfName);

    if (t == CosNaming::nobject) {
      if (rebind)
	context->rebind(restOfName, obj);
      else
	context->bind(restOfName, obj);
    } else {
      if (rebind)
	context->rebind_context(restOfName,
				CosNaming::NamingContext::_narrow(obj));
      else
	context->bind_context(restOfName,
			      CosNaming::NamingContext::_narrow(obj));
    }
  }
}


//
// unbind()
//

void
NamingContext_i::unbind(const CosNaming::Name& n)
{
  DB(cerr << "unbind in context " << this << endl);

  if (n.length() == 1) {

    //
    // Unbind a simple name - i.e. remove it from this context.
    //

    DB(cerr << "  unbind simple name (" << n[0].id << "," << n[0].kind << ")"
       << " in context " << this << endl);

    WriterLock w(lock);

    ObjectBinding* ob = resolve_simple(n);

    DB(cerr << "  unbind: removing (" << n[0].id << "," << n[0].kind << ")"
       << " from context " << this << " (was bound to "
       << ob->object << ")" << endl);

    CORBA::release(ob->object);

    delete ob;

    CosNaming::NamingContext_var nc = _this();
    redolog->unbind(nc, n);

  } else {

    //
    // Unbind a compound name <c1;c2;...;cn> - i.e. unbind the name
    // <c2;...;cn> from the context <c1>.
    //

    DB(cerr << "  unbind compound name ");
    for (int i = 0; i < n.length(); i++) {
      DB(cerr << "(" << n[i].id << "," << n[i].kind << ")");
    }
    DB(cerr << " in context " << this << endl);

    CosNaming::Name restOfName;
    CosNaming::NamingContext_var context = resolve_compound(n, restOfName);

    context->unbind(restOfName);
  }
}


//
// bind_new_context()
//

CosNaming::NamingContext_ptr
NamingContext_i::bind_new_context(const CosNaming::Name& n)
{
  if (n.length() == 1) {

    //
    // Bind a new context with a simple name - i.e. create a new context and
    // bind it in this context.
    //

    DB(cerr << "bind_new_context simple (" << n[0].id << "," << n[0].kind
       << ") in context " << this << endl);
    CosNaming::NamingContext_ptr nc = new_context();
    bind_context(n, nc);
    return nc;

  } else {

    //
    // Bind a new context with a compound name <c1;c2;...;cn> - i.e.
    // bind_new_context <c2;...;cn> in the context <c1>.
    //

    DB(cerr << "bind_new_context compound name ");
    for (int i = 0; i < n.length(); i++) {
      DB(cerr << "(" << n[i].id << "," << n[i].kind << ")");
    }
    DB(cerr << " in context " << this << endl);

    CosNaming::Name restOfName;
    CosNaming::NamingContext_var context = resolve_compound(n, restOfName);

    return context->bind_new_context(restOfName);
  }
}


//
// destroy()
//

void
NamingContext_i::destroy()
{
  DB(cerr << "destroy" << endl);

  lock.writerIn();

  if (headBinding)
    throw CosNaming::NamingContext::NotEmpty();

  CosNaming::NamingContext_var nc = _this();
  redolog->destroy(nc);

  lock.writerOut();

  _dispose();
}


//
// list()
//

void
NamingContext_i::list(CORBA::ULong how_many, CosNaming::BindingList*& bl,
		      CosNaming::BindingIterator_ptr& bi)
{
  lock.readerIn();

  DB(cerr << "list context " << this << ", how_many " << how_many
     << ", size " << size << endl);

  CosNaming::BindingList* all = new CosNaming::BindingList(size);
  all->length(size);

  int i;
  ObjectBinding* ob;

  for (ob = headBinding, i = 0; ob; ob = ob->next, i++) {
    assert(i < size);
    (*all)[i] = ob->binding;
    DB(cerr << "  (" << (*all)[i].binding_name[0].id << ","
       << (*all)[i].binding_name[0].kind << ") binding type "
       << (((*all)[i].binding_type == CosNaming::nobject) ?
	   "nobject" : "ncontext")
       << endl);
  }

  lock.readerOut();

  BindingIterator_i* bii = new BindingIterator_i(_boa(), all);

  bi = bii->_this();

  if (CORBA::is_nil(bi)) {
    cerr << "couldn't narrow binding iterator" << endl;
    return;
  }

  DB(cerr << "  calling next_n()" << endl);
  bi->next_n(how_many, bl);
}


//
// destructor
//

NamingContext_i::~NamingContext_i()
{
  DB(cerr << "~NamingContext_i" << endl);

  lock.writerIn();

  if (prev) {
    prev->next = next;
  } else {
    headContext = next;
  }
  if (next) {
    next->prev = prev;
  } else {
    tailContext = prev;
  }

  while (headBinding)
    delete headBinding;

  lock.writerOut();
}
