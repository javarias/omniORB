// This file is generated by omniidl (C++ backend)- omniORB_3_0. Do not edit.
#ifndef __echo_hh__
#define __echo_hh__

#ifndef USE_omniORB_logStream
#define USE_omniORB_logStream
#endif

#ifndef __CORBA_H_EXTERNAL_GUARD__
#include <omniORB4/CORBA.h>
#endif

#ifdef USE_stub_in_nt_dll
#ifndef USE_core_stub_in_nt_dll
#define USE_core_stub_in_nt_dll
#endif
#ifndef USE_dyn_stub_in_nt_dll
#define USE_dyn_stub_in_nt_dll
#endif
#endif

#ifdef _core_attr
# error "A local CPP macro _core_attr has already been defined."
#else
# ifdef  USE_core_stub_in_nt_dll
#  define _core_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _core_attr
# endif
#endif

#ifdef _dyn_attr
# error "A local CPP macro _dyn_attr has already been defined."
#else
# ifdef  USE_dyn_stub_in_nt_dll
#  define _dyn_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _dyn_attr
# endif
#endif

#ifndef __Echo__
#define __Echo__

class Echo;
class _objref_Echo;
class _impl_Echo;
typedef _objref_Echo* Echo_ptr;
typedef Echo_ptr EchoRef;

class Echo_Helper {
public:
  typedef Echo_ptr _ptr_type;

  static _ptr_type _nil();
  static _CORBA_Boolean is_nil(_ptr_type);
  static void release(_ptr_type);
  static void duplicate(_ptr_type);
  static void marshalObjRef(_ptr_type, cdrStream&);
  static _ptr_type unmarshalObjRef(cdrStream&);
};

typedef _CORBA_ObjRef_Var<_objref_Echo, Echo_Helper> Echo_var;
typedef _CORBA_ObjRef_OUT_arg<_objref_Echo,Echo_Helper > Echo_out;

#endif

class Echo {
public:
  // Declarations for this interface type.
  typedef Echo_ptr _ptr_type;
  typedef Echo_var _var_type;

  static _ptr_type _duplicate(_ptr_type);
  static _ptr_type _narrow(CORBA::Object_ptr);
  static _ptr_type _nil();

  static inline void _marshalObjRef(_ptr_type, cdrStream&);

  static inline _ptr_type _unmarshalObjRef(cdrStream& s) {
    omniObjRef* o = omniObjRef::_unMarshal(_PD_repoId,s);
    if (o)
      return (_ptr_type) o->_ptrToObjRef(_PD_repoId);
    else
      return _nil();
  }

  static _core_attr const char* _PD_repoId;

  // Other IDL defined within this scope.
  
};

class _objref_Echo :
  public virtual CORBA::Object, public virtual omniObjRef
{
public:
  char* echoString(const char* mesg);
  
  inline _objref_Echo() { _PR_setobj(0); }  // nil
  _objref_Echo(omniIOR*, omniIdentity*, omniLocalIdentity*);

protected:
  virtual ~_objref_Echo();

private:
  virtual void* _ptrToObjRef(const char*);

  _objref_Echo(const _objref_Echo&);
  _objref_Echo& operator = (const _objref_Echo&);
  // not implemented
};

class _pof_Echo : public proxyObjectFactory {
public:
  inline _pof_Echo() : proxyObjectFactory(Echo::_PD_repoId) {}
  virtual ~_pof_Echo();

  virtual omniObjRef* newObjRef(omniIOR*, omniIdentity*, omniLocalIdentity*);
  virtual _CORBA_Boolean is_a(const char*) const;
};

class _impl_Echo :
  public virtual omniServant
{
public:
  virtual ~_impl_Echo();

  virtual char* echoString(const char* mesg) = 0;
  
public:  // Really protected, workaround for xlC
  virtual _CORBA_Boolean _dispatch(GIOP_S&);

private:
  virtual void* _ptrToInterface(const char*);
  virtual const char* _mostDerivedRepoId();
};

_CORBA_GLOBAL_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_Echo;

class POA_Echo :
  public virtual _impl_Echo,
  public virtual PortableServer::ServantBase
{
public:
  virtual ~POA_Echo();

  inline Echo_ptr _this() {
    return (Echo_ptr) _do_this(Echo::_PD_repoId);
  }
};

#undef _core_attr
#undef _dyn_attr

void operator<<=(CORBA::Any& _a, Echo_ptr _s);
void operator<<=(CORBA::Any& _a, Echo_ptr* _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, Echo_ptr& _s);

inline void
Echo::_marshalObjRef(Echo_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}

#endif  // __echo_hh__

