// This file is generated by omniidl (C++ backend)- omniORB_3_0. Do not edit.
#ifndef __Naming_hh__
#define __Naming_hh__

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

_CORBA_MODULE CosNaming

_CORBA_MODULE_BEG

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_Istring;

  typedef char* Istring;
  typedef CORBA::String_var Istring_var;

  struct NameComponent {
    typedef _CORBA_ConstrType_Variable_Var<NameComponent> _var_type;

    CORBA::String_member id;

    CORBA::String_member kind;

    void operator>>= (cdrStream &) const;
    void operator<<= (cdrStream &);
  };

  typedef NameComponent::_var_type NameComponent_var;

  typedef _CORBA_ConstrType_Variable_OUT_arg< NameComponent,NameComponent_var > NameComponent_out;

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_NameComponent;

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_Name;

  class Name_var;

  class Name : public _CORBA_Unbounded_Sequence< NameComponent>  {
  public:
    typedef Name_var _var_type;
    inline Name() {}
    inline Name(const Name& s)
      : _CORBA_Unbounded_Sequence< NameComponent> (s) {}

    inline Name(_CORBA_ULong _max)
      : _CORBA_Unbounded_Sequence< NameComponent> (_max) {}
    inline Name(_CORBA_ULong _max, _CORBA_ULong _len, NameComponent* _val, _CORBA_Boolean _rel=0)
      : _CORBA_Unbounded_Sequence< NameComponent> (_max, _len, _val, _rel) {}

    inline Name& operator = (const Name& s) {
      _CORBA_Unbounded_Sequence< NameComponent> ::operator=(s);
      return *this;
    }
  };

  class Name_out;

  class Name_var {
  public:
    typedef Name T;
    typedef Name_var T_var;

    inline Name_var() : _pd_seq(0) {}
    inline Name_var(T* s) : _pd_seq(s) {}
    inline Name_var(const T_var& s) {
      if( s._pd_seq )  _pd_seq = new T(*s._pd_seq);
      else             _pd_seq = 0;
    }
    inline ~Name_var() { if( _pd_seq )  delete _pd_seq; }

    inline T_var& operator = (T* s) {
      if( _pd_seq )  delete _pd_seq;
      _pd_seq = s;
      return *this;
    }
    inline T_var& operator = (const T_var& s) {
      if( s._pd_seq ) {
        if( !_pd_seq )  _pd_seq = new T;
        *_pd_seq = *s._pd_seq;
      } else if( _pd_seq ) {
        delete _pd_seq;
        _pd_seq = 0;
      }
      return *this;
    }

    inline NameComponent& operator [] (_CORBA_ULong s) {
      return (*_pd_seq)[s];
    }

    inline T* operator -> () { return _pd_seq; }
#if defined(__GNUG__) && __GNUG__ == 2 && __GNUC_MINOR__ == 7
    inline operator T& () const { return *_pd_seq; }
#else
    inline operator const T& () const { return *_pd_seq; }
    inline operator T& () { return *_pd_seq; }
#endif

    inline const T& in() const { return *_pd_seq; }
    inline T&       inout()    { return *_pd_seq; }
    inline T*&      out() {
      if( _pd_seq ) { delete _pd_seq; _pd_seq = 0; }
      return _pd_seq;
    }
    inline T* _retn() { T* tmp = _pd_seq; _pd_seq = 0; return tmp; }

    friend class Name_out;

  private:
    T* _pd_seq;
  };

  class Name_out {
  public:
    typedef Name T;
    typedef Name_var T_var;

    inline Name_out(T*& s) : _data(s) { _data = 0; }
    inline Name_out(T_var& s)
      : _data(s._pd_seq) { s = (T*) 0; }
    inline Name_out(const Name_out& s) : _data(s._data) {}
    inline Name_out& operator = (const Name_out& s) {
      _data = s._data;
      return *this;
    }  inline Name_out& operator = (T* s) {
      _data = s;
      return *this;
    }
    inline operator T*&()  { return _data; }
    inline T*& ptr()       { return _data; }
    inline T* operator->() { return _data; }

    inline NameComponent& operator [] (_CORBA_ULong i) {
      return (*_data)[i];
    }

    T*& _data;

  private:
    Name_out();
    Name_out& operator=(const T_var&);
  };

  enum BindingType { nobject, ncontext };
  typedef BindingType& BindingType_out;

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_BindingType;

  struct Binding {
    typedef _CORBA_ConstrType_Variable_Var<Binding> _var_type;

    Name binding_name;

    BindingType binding_type;

    void operator>>= (cdrStream &) const;
    void operator<<= (cdrStream &);
  };

  typedef Binding::_var_type Binding_var;

  typedef _CORBA_ConstrType_Variable_OUT_arg< Binding,Binding_var > Binding_out;

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_Binding;

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_BindingList;

  class BindingList_var;

  class BindingList : public _CORBA_Unbounded_Sequence< Binding>  {
  public:
    typedef BindingList_var _var_type;
    inline BindingList() {}
    inline BindingList(const BindingList& s)
      : _CORBA_Unbounded_Sequence< Binding> (s) {}

    inline BindingList(_CORBA_ULong _max)
      : _CORBA_Unbounded_Sequence< Binding> (_max) {}
    inline BindingList(_CORBA_ULong _max, _CORBA_ULong _len, Binding* _val, _CORBA_Boolean _rel=0)
      : _CORBA_Unbounded_Sequence< Binding> (_max, _len, _val, _rel) {}

    inline BindingList& operator = (const BindingList& s) {
      _CORBA_Unbounded_Sequence< Binding> ::operator=(s);
      return *this;
    }
  };

  class BindingList_out;

  class BindingList_var {
  public:
    typedef BindingList T;
    typedef BindingList_var T_var;

    inline BindingList_var() : _pd_seq(0) {}
    inline BindingList_var(T* s) : _pd_seq(s) {}
    inline BindingList_var(const T_var& s) {
      if( s._pd_seq )  _pd_seq = new T(*s._pd_seq);
      else             _pd_seq = 0;
    }
    inline ~BindingList_var() { if( _pd_seq )  delete _pd_seq; }

    inline T_var& operator = (T* s) {
      if( _pd_seq )  delete _pd_seq;
      _pd_seq = s;
      return *this;
    }
    inline T_var& operator = (const T_var& s) {
      if( s._pd_seq ) {
        if( !_pd_seq )  _pd_seq = new T;
        *_pd_seq = *s._pd_seq;
      } else if( _pd_seq ) {
        delete _pd_seq;
        _pd_seq = 0;
      }
      return *this;
    }

    inline Binding& operator [] (_CORBA_ULong s) {
      return (*_pd_seq)[s];
    }

    inline T* operator -> () { return _pd_seq; }
#if defined(__GNUG__) && __GNUG__ == 2 && __GNUC_MINOR__ == 7
    inline operator T& () const { return *_pd_seq; }
#else
    inline operator const T& () const { return *_pd_seq; }
    inline operator T& () { return *_pd_seq; }
#endif

    inline const T& in() const { return *_pd_seq; }
    inline T&       inout()    { return *_pd_seq; }
    inline T*&      out() {
      if( _pd_seq ) { delete _pd_seq; _pd_seq = 0; }
      return _pd_seq;
    }
    inline T* _retn() { T* tmp = _pd_seq; _pd_seq = 0; return tmp; }

    friend class BindingList_out;

  private:
    T* _pd_seq;
  };

  class BindingList_out {
  public:
    typedef BindingList T;
    typedef BindingList_var T_var;

    inline BindingList_out(T*& s) : _data(s) { _data = 0; }
    inline BindingList_out(T_var& s)
      : _data(s._pd_seq) { s = (T*) 0; }
    inline BindingList_out(const BindingList_out& s) : _data(s._data) {}
    inline BindingList_out& operator = (const BindingList_out& s) {
      _data = s._data;
      return *this;
    }  inline BindingList_out& operator = (T* s) {
      _data = s;
      return *this;
    }
    inline operator T*&()  { return _data; }
    inline T*& ptr()       { return _data; }
    inline T* operator->() { return _data; }

    inline Binding& operator [] (_CORBA_ULong i) {
      return (*_data)[i];
    }

    T*& _data;

  private:
    BindingList_out();
    BindingList_out& operator=(const T_var&);
  };

#ifndef __CosNaming_mBindingIterator__
#define __CosNaming_mBindingIterator__

  class BindingIterator;
  class _objref_BindingIterator;
  class _impl_BindingIterator;
  typedef _objref_BindingIterator* BindingIterator_ptr;
  typedef BindingIterator_ptr BindingIteratorRef;

  class BindingIterator_Helper {
  public:
    typedef BindingIterator_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_BindingIterator, BindingIterator_Helper> BindingIterator_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_BindingIterator,BindingIterator_Helper > BindingIterator_out;

#endif

#ifndef __CosNaming_mNamingContext__
#define __CosNaming_mNamingContext__

  class NamingContext;
  class _objref_NamingContext;
  class _impl_NamingContext;
  typedef _objref_NamingContext* NamingContext_ptr;
  typedef NamingContext_ptr NamingContextRef;

  class NamingContext_Helper {
  public:
    typedef NamingContext_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_NamingContext, NamingContext_Helper> NamingContext_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_NamingContext,NamingContext_Helper > NamingContext_out;

#endif

  class NamingContext {
  public:
    // Declarations for this interface type.
    typedef NamingContext_ptr _ptr_type;
    typedef NamingContext_var _var_type;

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
    enum NotFoundReason { missing_node, not_context, not_object };
    typedef NotFoundReason& NotFoundReason_out;

    static _dyn_attr const CORBA::TypeCode_ptr _tc_NotFoundReason;

    class NotFound : public CORBA::UserException {
    public:

      NotFoundReason why;

      Name rest_of_name;

      inline NotFound() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      NotFound(const NotFound&);
      NotFound(NotFoundReason i_why, const Name i_rest_of_name);
      NotFound& operator=(const NotFound&);
      virtual ~NotFound();
      virtual void _raise();
      static NotFound* _downcast(CORBA::Exception*);
      static const NotFound* _downcast(const CORBA::Exception*);
      static inline NotFound* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      void operator>>=(cdrStream&) const ;
      void operator<<=(cdrStream&) ;

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_NotFound;

    class CannotProceed : public CORBA::UserException {
    public:

      _CORBA_ObjRef_Member< _objref_NamingContext, NamingContext_Helper>  cxt;

      Name rest_of_name;

      inline CannotProceed() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      CannotProceed(const CannotProceed&);
      CannotProceed(NamingContext_ptr i_cxt, const Name i_rest_of_name);
      CannotProceed& operator=(const CannotProceed&);
      virtual ~CannotProceed();
      virtual void _raise();
      static CannotProceed* _downcast(CORBA::Exception*);
      static const CannotProceed* _downcast(const CORBA::Exception*);
      static inline CannotProceed* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      void operator>>=(cdrStream&) const ;
      void operator<<=(cdrStream&) ;

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_CannotProceed;

    class InvalidName : public CORBA::UserException {
    public:

      inline InvalidName() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      InvalidName(const InvalidName&);
      
      InvalidName& operator=(const InvalidName&);
      virtual ~InvalidName();
      virtual void _raise();
      static InvalidName* _downcast(CORBA::Exception*);
      static const InvalidName* _downcast(const CORBA::Exception*);
      static inline InvalidName* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      inline void operator>>=(cdrStream&) const { }
      inline void operator<<=(cdrStream&) { }

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_InvalidName;

    class AlreadyBound : public CORBA::UserException {
    public:

      inline AlreadyBound() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      AlreadyBound(const AlreadyBound&);
      
      AlreadyBound& operator=(const AlreadyBound&);
      virtual ~AlreadyBound();
      virtual void _raise();
      static AlreadyBound* _downcast(CORBA::Exception*);
      static const AlreadyBound* _downcast(const CORBA::Exception*);
      static inline AlreadyBound* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      inline void operator>>=(cdrStream&) const { }
      inline void operator<<=(cdrStream&) { }

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_AlreadyBound;

    class NotEmpty : public CORBA::UserException {
    public:

      inline NotEmpty() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      NotEmpty(const NotEmpty&);
      
      NotEmpty& operator=(const NotEmpty&);
      virtual ~NotEmpty();
      virtual void _raise();
      static NotEmpty* _downcast(CORBA::Exception*);
      static const NotEmpty* _downcast(const CORBA::Exception*);
      static inline NotEmpty* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      inline void operator>>=(cdrStream&) const { }
      inline void operator<<=(cdrStream&) { }

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_NotEmpty;

  };

  class _objref_NamingContext :
    public virtual CORBA::Object, public virtual omniObjRef
  {
  public:
    void bind(const Name& n, CORBA::Object_ptr obj);
    void rebind(const Name& n, CORBA::Object_ptr obj);
    void bind_context(const Name& n, NamingContext_ptr nc);
    void rebind_context(const Name& n, NamingContext_ptr nc);
    CORBA::Object_ptr resolve(const Name& n);
    void unbind(const Name& n);
    NamingContext_ptr new_context();
    NamingContext_ptr bind_new_context(const Name& n);
    void destroy();
    void list(CORBA::ULong how_many, BindingList_out bl, _CORBA_ObjRef_OUT_arg< CosNaming::_objref_BindingIterator, BindingIterator_Helper > bi);
    
    inline _objref_NamingContext() { _PR_setobj(0); }  // nil
    _objref_NamingContext(omniIOR*, omniIdentity*, omniLocalIdentity*);

  protected:
    virtual ~_objref_NamingContext();

  private:
    virtual void* _ptrToObjRef(const char*);

    _objref_NamingContext(const _objref_NamingContext&);
    _objref_NamingContext& operator = (const _objref_NamingContext&);
    // not implemented
  };

  class _pof_NamingContext : public proxyObjectFactory {
  public:
    inline _pof_NamingContext() : proxyObjectFactory(NamingContext::_PD_repoId) {}
    virtual ~_pof_NamingContext();

    virtual omniObjRef* newObjRef(omniIOR*,omniIdentity*, omniLocalIdentity*);
    virtual _CORBA_Boolean is_a(const char*) const;
  };

  class _impl_NamingContext :
    public virtual omniServant
  {
  public:
    virtual ~_impl_NamingContext();

    virtual void bind(const Name& n, CORBA::Object_ptr obj) = 0;
    virtual void rebind(const Name& n, CORBA::Object_ptr obj) = 0;
    virtual void bind_context(const Name& n, NamingContext_ptr nc) = 0;
    virtual void rebind_context(const Name& n, NamingContext_ptr nc) = 0;
    virtual CORBA::Object_ptr resolve(const Name& n) = 0;
    virtual void unbind(const Name& n) = 0;
    virtual NamingContext_ptr new_context() = 0;
    virtual NamingContext_ptr bind_new_context(const Name& n) = 0;
    virtual void destroy() = 0;
    virtual void list(CORBA::ULong how_many, BindingList_out bl, _CORBA_ObjRef_OUT_arg< CosNaming::_objref_BindingIterator, BindingIterator_Helper > bi) = 0;
    
  public:  // Really protected, workaround for xlC
    virtual _CORBA_Boolean _dispatch(GIOP_S&);

  private:
    virtual void* _ptrToInterface(const char*);
    virtual const char* _mostDerivedRepoId();
  };

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_NamingContext;

#ifndef __CosNaming_mBindingIterator__
#define __CosNaming_mBindingIterator__

  class BindingIterator;
  class _objref_BindingIterator;
  class _impl_BindingIterator;
  typedef _objref_BindingIterator* BindingIterator_ptr;
  typedef BindingIterator_ptr BindingIteratorRef;

  class BindingIterator_Helper {
  public:
    typedef BindingIterator_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_BindingIterator, BindingIterator_Helper> BindingIterator_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_BindingIterator,BindingIterator_Helper > BindingIterator_out;

#endif

  class BindingIterator {
  public:
    // Declarations for this interface type.
    typedef BindingIterator_ptr _ptr_type;
    typedef BindingIterator_var _var_type;

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

  class _objref_BindingIterator :
    public virtual CORBA::Object, public virtual omniObjRef
  {
  public:
    CORBA::Boolean next_one(Binding_out b);
    CORBA::Boolean next_n(CORBA::ULong how_many, BindingList_out bl);
    void destroy();
    
    inline _objref_BindingIterator() { _PR_setobj(0); }  // nil
    _objref_BindingIterator(omniIOR*, omniIdentity*, omniLocalIdentity*);

  protected:
    virtual ~_objref_BindingIterator();

  private:
    virtual void* _ptrToObjRef(const char*);

    _objref_BindingIterator(const _objref_BindingIterator&);
    _objref_BindingIterator& operator = (const _objref_BindingIterator&);
    // not implemented
  };

  class _pof_BindingIterator : public proxyObjectFactory {
  public:
    inline _pof_BindingIterator() : proxyObjectFactory(BindingIterator::_PD_repoId) {}
    virtual ~_pof_BindingIterator();

    virtual omniObjRef* newObjRef(omniIOR*, omniIdentity*, omniLocalIdentity*);
    virtual _CORBA_Boolean is_a(const char*) const;
  };

  class _impl_BindingIterator :
    public virtual omniServant
  {
  public:
    virtual ~_impl_BindingIterator();

    virtual CORBA::Boolean next_one(Binding_out b) = 0;
    virtual CORBA::Boolean next_n(CORBA::ULong how_many, BindingList_out bl) = 0;
    virtual void destroy() = 0;
    
  public:  // Really protected, workaround for xlC
    virtual _CORBA_Boolean _dispatch(GIOP_S&);

  private:
    virtual void* _ptrToInterface(const char*);
    virtual const char* _mostDerivedRepoId();
  };

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_BindingIterator;

#ifndef __CosNaming_mNamingContextExt__
#define __CosNaming_mNamingContextExt__

  class NamingContextExt;
  class _objref_NamingContextExt;
  class _impl_NamingContextExt;
  typedef _objref_NamingContextExt* NamingContextExt_ptr;
  typedef NamingContextExt_ptr NamingContextExtRef;

  class NamingContextExt_Helper {
  public:
    typedef NamingContextExt_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_NamingContextExt, NamingContextExt_Helper> NamingContextExt_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_NamingContextExt,NamingContextExt_Helper > NamingContextExt_out;

#endif

  class NamingContextExt {
  public:
    // Declarations for this interface type.
    typedef NamingContextExt_ptr _ptr_type;
    typedef NamingContextExt_var _var_type;

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
    static _dyn_attr const CORBA::TypeCode_ptr _tc_StringName;

    typedef char* StringName;
    typedef CORBA::String_var StringName_var;

    static _dyn_attr const CORBA::TypeCode_ptr _tc_Address;

    typedef char* Address;
    typedef CORBA::String_var Address_var;

    static _dyn_attr const CORBA::TypeCode_ptr _tc_URLString;

    typedef char* URLString;
    typedef CORBA::String_var URLString_var;

    class InvalidAddress : public CORBA::UserException {
    public:

      inline InvalidAddress() {
        pd_insertToAnyFn    = insertToAnyFn;
        pd_insertToAnyFnNCP = insertToAnyFnNCP;
      }
      InvalidAddress(const InvalidAddress&);
      
      InvalidAddress& operator=(const InvalidAddress&);
      virtual ~InvalidAddress();
      virtual void _raise();
      static InvalidAddress* _downcast(CORBA::Exception*);
      static const InvalidAddress* _downcast(const CORBA::Exception*);
      static inline InvalidAddress* _narrow(CORBA::Exception* _e) {
        return _downcast(_e);
      }

      inline void operator>>=(cdrStream&) const { }
      inline void operator<<=(cdrStream&) { }

      static _core_attr insertExceptionToAny    insertToAnyFn;
      static _core_attr insertExceptionToAnyNCP insertToAnyFnNCP;

      virtual CORBA::Exception* _NP_duplicate() const;

      static _core_attr const char* _PD_repoId;

    private:
      virtual const char* _NP_typeId() const;
      virtual const char* _NP_repoId(int*) const;
      virtual void _NP_marshal(cdrStream&) const;
    };

    static _dyn_attr const CORBA::TypeCode_ptr _tc_InvalidAddress;

  };

  class _objref_NamingContextExt :
    public virtual _objref_NamingContext
  {
  public:
    char* to_string(const Name& n);
    Name* to_name(const char* sn);
    char* to_url(const char* addr, const char* sn);
    CORBA::Object_ptr resolve_str(const char* n);
    
    inline _objref_NamingContextExt() { _PR_setobj(0); }  // nil
    _objref_NamingContextExt(omniIOR*, omniIdentity*, omniLocalIdentity*);

  protected:
    virtual ~_objref_NamingContextExt();

  private:
    virtual void* _ptrToObjRef(const char*);

    _objref_NamingContextExt(const _objref_NamingContextExt&);
    _objref_NamingContextExt& operator = (const _objref_NamingContextExt&);
    // not implemented
  };

  class _pof_NamingContextExt : public proxyObjectFactory {
  public:
    inline _pof_NamingContextExt() : proxyObjectFactory(NamingContextExt::_PD_repoId) {}
    virtual ~_pof_NamingContextExt();

    virtual omniObjRef* newObjRef(omniIOR*,omniIdentity*, omniLocalIdentity*);
    virtual _CORBA_Boolean is_a(const char*) const;
  };

  class _impl_NamingContextExt :
    public virtual _impl_NamingContext
  {
  public:
    virtual ~_impl_NamingContextExt();

    virtual char* to_string(const Name& n) = 0;
    virtual Name* to_name(const char* sn) = 0;
    virtual char* to_url(const char* addr, const char* sn) = 0;
    virtual CORBA::Object_ptr resolve_str(const char* n) = 0;
    
  public:  // Really protected, workaround for xlC
    virtual _CORBA_Boolean _dispatch(GIOP_S&);

  private:
    virtual void* _ptrToInterface(const char*);
    virtual const char* _mostDerivedRepoId();
  };

  _CORBA_MODULE_VAR _dyn_attr const CORBA::TypeCode_ptr _tc_NamingContextExt;

_CORBA_MODULE_END

_CORBA_MODULE POA_CosNaming
_CORBA_MODULE_BEG

  class NamingContext :
    public virtual CosNaming::_impl_NamingContext,
    public virtual PortableServer::ServantBase
  {
  public:
    virtual ~NamingContext();

    inline CosNaming::NamingContext_ptr _this() {
      return (CosNaming::NamingContext_ptr) _do_this(CosNaming::NamingContext::_PD_repoId);
    }
  };

  class BindingIterator :
    public virtual CosNaming::_impl_BindingIterator,
    public virtual PortableServer::ServantBase
  {
  public:
    virtual ~BindingIterator();

    inline CosNaming::BindingIterator_ptr _this() {
      return (CosNaming::BindingIterator_ptr) _do_this(CosNaming::BindingIterator::_PD_repoId);
    }
  };

  class NamingContextExt :
    public virtual CosNaming::_impl_NamingContextExt,
    public virtual NamingContext
  {
  public:
    virtual ~NamingContextExt();

    inline CosNaming::NamingContextExt_ptr _this() {
      return (CosNaming::NamingContextExt_ptr) _do_this(CosNaming::NamingContextExt::_PD_repoId);
    }
  };

_CORBA_MODULE_END

#undef _core_attr
#undef _dyn_attr

extern void operator<<=(CORBA::Any& _a, const CosNaming::NameComponent& _s);
extern void operator<<=(CORBA::Any& _a, CosNaming::NameComponent* _sp);
extern CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::NameComponent*& _sp);
extern CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NameComponent*& _sp);

extern void operator <<= (CORBA::Any& _a, const CosNaming::Name& _s);
inline void operator <<= (CORBA::Any& _a, CosNaming::Name* _sp) {
  _a <<= *_sp;
  delete _sp;
}
extern _CORBA_Boolean operator >>= (const CORBA::Any& _a, CosNaming::Name*& _sp);
extern _CORBA_Boolean operator >>= (const CORBA::Any& _a, const CosNaming::Name*& _sp);

inline void operator >>=(CosNaming::BindingType _e, cdrStream& s) {
  ::operator>>=((CORBA::ULong)_e, s);
}

inline void operator <<= (CosNaming::BindingType& _e, cdrStream& s) {
  CORBA::ULong _0RL_e;
  ::operator<<=(_0RL_e,s);
  switch (_0RL_e) {
    case CosNaming::nobject:

    case CosNaming::ncontext:

    _e = (CosNaming::BindingType) _0RL_e;
    break;
  default:
    _CORBA_marshal_error();
  }
}

void operator<<=(CORBA::Any& _a, CosNaming::BindingType _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::BindingType& _s);

extern void operator<<=(CORBA::Any& _a, const CosNaming::Binding& _s);
extern void operator<<=(CORBA::Any& _a, CosNaming::Binding* _sp);
extern CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::Binding*& _sp);
extern CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::Binding*& _sp);

extern void operator <<= (CORBA::Any& _a, const CosNaming::BindingList& _s);
inline void operator <<= (CORBA::Any& _a, CosNaming::BindingList* _sp) {
  _a <<= *_sp;
  delete _sp;
}
extern _CORBA_Boolean operator >>= (const CORBA::Any& _a, CosNaming::BindingList*& _sp);
extern _CORBA_Boolean operator >>= (const CORBA::Any& _a, const CosNaming::BindingList*& _sp);

inline void operator >>=(CosNaming::NamingContext::NotFoundReason _e, cdrStream& s) {
  ::operator>>=((CORBA::ULong)_e, s);
}

inline void operator <<= (CosNaming::NamingContext::NotFoundReason& _e, cdrStream& s) {
  CORBA::ULong _0RL_e;
  ::operator<<=(_0RL_e,s);
  switch (_0RL_e) {
    case CosNaming::NamingContext::missing_node:

    case CosNaming::NamingContext::not_context:

    case CosNaming::NamingContext::not_object:

    _e = (CosNaming::NamingContext::NotFoundReason) _0RL_e;
    break;
  default:
    _CORBA_marshal_error();
  }
}

void operator<<=(CORBA::Any& _a, CosNaming::NamingContext::NotFoundReason _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::NamingContext::NotFoundReason& _s);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::NotFound& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::NotFound* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContext::NotFound*& _sp);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::CannotProceed& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::CannotProceed* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContext::CannotProceed*& _sp);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::InvalidName& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::InvalidName* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContext::InvalidName*& _sp);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::AlreadyBound& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::AlreadyBound* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContext::AlreadyBound*& _sp);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::NotEmpty& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContext::NotEmpty* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContext::NotEmpty*& _sp);

void operator<<=(CORBA::Any& _a, CosNaming::NamingContext_ptr _s);
void operator<<=(CORBA::Any& _a, CosNaming::NamingContext_ptr* _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::NamingContext_ptr& _s);

void operator<<=(CORBA::Any& _a, CosNaming::BindingIterator_ptr _s);
void operator<<=(CORBA::Any& _a, CosNaming::BindingIterator_ptr* _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::BindingIterator_ptr& _s);

void operator<<=(CORBA::Any& _a, const CosNaming::NamingContextExt::InvalidAddress& _s);
void operator<<=(CORBA::Any& _a, const CosNaming::NamingContextExt::InvalidAddress* _sp);
CORBA::Boolean operator>>=(const CORBA::Any& _a, const CosNaming::NamingContextExt::InvalidAddress*& _sp);

void operator<<=(CORBA::Any& _a, CosNaming::NamingContextExt_ptr _s);
void operator<<=(CORBA::Any& _a, CosNaming::NamingContextExt_ptr* _s);
CORBA::Boolean operator>>=(const CORBA::Any& _a, CosNaming::NamingContextExt_ptr& _s);

inline void
CosNaming::NamingContext::_marshalObjRef(CosNaming::NamingContext_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}

inline void
CosNaming::BindingIterator::_marshalObjRef(CosNaming::BindingIterator_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}

inline void
CosNaming::NamingContextExt::_marshalObjRef(CosNaming::NamingContextExt_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}


#endif  // __Naming_hh__

