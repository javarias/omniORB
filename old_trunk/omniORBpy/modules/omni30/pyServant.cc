// -*- Mode: C++; -*-
//                            Package   : omniORBpy
// pyServant.cc               Created on: 1999/07/29
//                            Author    : Duncan Grisby (dpg1)
//
//    Copyright (C) 1999 AT&T Laboratories Cambridge
//
//    This file is part of the omniORBpy library
//
//    The omniORBpy library is free software; you can redistribute it
//    and/or modify it under the terms of the GNU Lesser General
//    Public License as published by the Free Software Foundation;
//    either version 2.1 of the License, or (at your option) any later
//    version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
//    MA 02111-1307, USA
//
//
// Description:
//    Implementation of Python servant object

// $Id$

// $Log$
// Revision 1.15  2000/03/06 18:48:28  dpg1
// Support for our favourite compiler, MSVC.
//
// Revision 1.14  2000/03/06 16:38:45  dpg1
// Additions to compile on Solaris.
//
// Revision 1.13  2000/03/03 17:41:41  dpg1
// Major reorganisation to support omniORB 3.0 as well as 2.8.
//
// Revision 1.12  1999/12/15 12:17:19  dpg1
// Changes to compile with SunPro CC 5.0.
//
// Revision 1.11  1999/11/25 11:21:36  dpg1
// Proper support for server-side _is_a().
//
// Revision 1.10  1999/11/16 17:32:36  dpg1
// Changes for AIX.
//
// Revision 1.9  1999/11/08 11:43:35  dpg1
// Changes for NT support.
//
// Revision 1.8  1999/10/01 11:07:20  dpg1
// Error reporting if up-call raises an unexpected exception.
//
// Revision 1.7  1999/09/29 15:46:50  dpg1
// lockWithNewThreadState now creates a dummy threading.Thread object so
// threading doesn't get upset that it's not there. Very dependent on the
// implementation of threading.py.
//
// Revision 1.6  1999/09/29 11:38:29  dpg1
// Comments removed.
//
// Revision 1.5  1999/09/29 09:53:23  dpg1
// Workaround to Python's lack of concurrency control on its
// PyInterpreterState.
//
// Revision 1.4  1999/09/24 09:22:02  dpg1
// Added copyright notices.
//
// Revision 1.3  1999/09/22 15:46:12  dpg1
// Fake POA implemented.
//
// Revision 1.2  1999/09/20 14:56:12  dpg1
// GCC 2.95 is more pedantic than egcs.
//
// Revision 1.1  1999/07/29 14:20:49  dpg1
// Initial revision
//


#include <omnipy.h>


// Helper class to create a Python ThreadState object and grab the
// Python interpreter lock, then release the lock and delete the
// object when it goes out of scope.

class lockWithNewThreadState {
public:
  lockWithNewThreadState() {
    // Create thread state
    omniPy::pyInterpreterLock->lock();
    newstate_ = PyThreadState_New(omniPy::pyInterpreter);
    omniPy::pyInterpreterLock->unlock();

    // Acquire global interpreter lock
    PyEval_AcquireLock();
    oldstate_ = PyThreadState_Swap(newstate_);

    // Create a threading.Thread object, so the threading module is
    // happy.
    worker_thread_ = PyEval_CallObject(omniPy::pyWorkerThreadClass,
				       omniPy::pyEmptyTuple);
    // If the user hits Ctrl-C during the above call, it will
    // fail. There's not much we can do except carry on without the
    // Thread object...
  }

  ~lockWithNewThreadState() {
    // Delete worker thread
    if (worker_thread_) {
      PyObject* argtuple = PyTuple_New(1);
      PyTuple_SET_ITEM(argtuple, 0, worker_thread_);

      PyObject* tmp = PyEval_CallObject(omniPy::pyWorkerThreadDel, argtuple);
      Py_XDECREF(tmp);
      Py_DECREF(argtuple);
    }

    // Return to the previous thread state
    PyThreadState_Swap(oldstate_);

    // We would like to release the interpreter lock here, before
    // deleting the ThreadState struct. Unfortunately, if we do that
    // the Python program may end before we get to do the delete. In
    // that situation, we might call Delete() while Python is clearing
    // up its interpreter state, leading to a segfault. So we have to
    // delete the ThreadState first, then release the interpreter
    // lock. Python really ought to do some concurrency control on the
    // PyInterpreterState structure.

    omniPy::pyInterpreterLock->lock();
    PyThreadState_Delete(newstate_);
    omniPy::pyInterpreterLock->unlock();

    PyEval_ReleaseLock();
  }

private:
  PyThreadState*   newstate_;
  PyThreadState*   oldstate_;
  PyObject*        worker_thread_;
};


// Implementation classes for ServantManagers and AdapterActivator

class Py_ServantActivator: public virtual POA_PortableServer::ServantActivator,
			   public virtual omniPy::Py_omniServant
{
public:
  Py_ServantActivator(PyObject* pysa, PyObject* opdict, const char* repoId);
  virtual ~Py_ServantActivator();

  PortableServer::Servant incarnate(const PortableServer::ObjectId& oid,
				    PortableServer::POA_ptr         poa);

  void etherealize(const PortableServer::ObjectId& oid,
		   PortableServer::POA_ptr         poa,
		   PortableServer::Servant         serv,
		   CORBA::Boolean                  cleanup_in_progress,
		   CORBA::Boolean                  remaining_activations);

  void*                   _ptrToInterface(const char* repoId);

  CORBA::Boolean          _is_a(const char* logical_type_id) {
    return Py_omniServant::_is_a(logical_type_id);
  }
  PortableServer::POA_ptr _default_POA() {
    return Py_omniServant::_default_POA();
  }
  const char* _mostDerivedRepoId() {
    return Py_omniServant::_mostDerivedRepoId();
  }
  CORBA::Boolean _dispatch(GIOP_S& giop_s) {
    return Py_omniServant::_dispatch(giop_s);
  }

private:
  PyObject* pysa_;

  // Not implemented
  Py_ServantActivator(const Py_ServantActivator&);
  Py_ServantActivator& operator=(const Py_ServantActivator&);
};

class Py_ServantLocator: public virtual POA_PortableServer::ServantLocator,
			 public virtual omniPy::Py_omniServant
{
public:
  Py_ServantLocator(PyObject* pysl, PyObject* opdict, const char* repoId);
  virtual ~Py_ServantLocator();

  PortableServer::Servant preinvoke(const PortableServer::ObjectId& oid,
				    PortableServer::POA_ptr         poa,
				    const char*                     operation,
				    void*&                          cookie);

  void postinvoke(const PortableServer::ObjectId& oid,
		  PortableServer::POA_ptr         poa,
		  const char*                     operation,
		  void*                           cookie,
		  PortableServer::Servant         serv);

  void*                   _ptrToInterface(const char* repoId);

  CORBA::Boolean          _is_a(const char* logical_type_id) {
    return Py_omniServant::_is_a(logical_type_id);
  }
  PortableServer::POA_ptr _default_POA() {
    return Py_omniServant::_default_POA();
  }
  const char* _mostDerivedRepoId() {
    return Py_omniServant::_mostDerivedRepoId();
  }
  CORBA::Boolean _dispatch(GIOP_S& giop_s) {
    return Py_omniServant::_dispatch(giop_s);
  }

private:
  PyObject* pysl_;

  // Not implemented
  Py_ServantLocator(const Py_ServantLocator&);
  Py_ServantLocator& operator=(const Py_ServantLocator&);
};


class Py_AdapterActivator: public virtual POA_PortableServer::AdapterActivator,
			   public virtual omniPy::Py_omniServant
{
public:
  Py_AdapterActivator(PyObject* pyaa, PyObject* opdict, const char* repoId);
  virtual ~Py_AdapterActivator();

  CORBA::Boolean unknown_adapter(PortableServer::POA_ptr parent,
				 const char*             name);

  void*                   _ptrToInterface(const char* repoId);

  CORBA::Boolean          _is_a(const char* logical_type_id) {
    return Py_omniServant::_is_a(logical_type_id);
  }
  PortableServer::POA_ptr _default_POA() {
    return Py_omniServant::_default_POA();
  }
  const char* _mostDerivedRepoId() {
    return Py_omniServant::_mostDerivedRepoId();
  }
  CORBA::Boolean _dispatch(GIOP_S& giop_s) {
    return Py_omniServant::_dispatch(giop_s);
  }

private:
  PyObject* pyaa_;

  // Not implemented
  Py_AdapterActivator(const Py_AdapterActivator&);
  Py_AdapterActivator& operator=(const Py_AdapterActivator&);
};



// Implementation of Py_omniServant

omniPy::
Py_omniServant::Py_omniServant(PyObject* pyservant, PyObject* opdict,
			       const char* repoId)
  : pyservant_(pyservant), opdict_(opdict)
{
  repoId_ = CORBA::string_dup(repoId);

  OMNIORB_ASSERT(PyInstance_Check(pyservant));
  OMNIORB_ASSERT(PyDict_Check(opdict));
  Py_INCREF(pyservant_);
  Py_INCREF(opdict_);

  pyskeleton_ = PyObject_GetAttrString(pyservant_, (char*)"_omni_skeleton");
  OMNIORB_ASSERT(pyskeleton_ && PyClass_Check(pyskeleton_));

  omniPy::setTwin(pyservant, (omniPy::Py_omniServant*)this, SERVANT_TWIN);
}

omniPy::
Py_omniServant::~Py_omniServant()
{
  lockWithNewThreadState _t;
  omniPy::remTwin(pyservant_, SERVANT_TWIN);
  Py_DECREF(pyservant_);
  Py_DECREF(opdict_);
  Py_DECREF(pyskeleton_);
  CORBA::string_free(repoId_);
}


void*
omniPy::
Py_omniServant::_ptrToInterface(const char* repoId)
{
  OMNIORB_ASSERT(repoId);

  if (!strcmp(repoId, CORBA::Object::_PD_repoId)) return (void*)1;
  if (!strcmp(repoId, "Py_omniServant")) return (Py_omniServant*)this;
  return 0;
}


const char*
omniPy::
Py_omniServant::_mostDerivedRepoId()
{
  return repoId_;
}


CORBA::Boolean
omniPy::
Py_omniServant::_is_a(const char* logical_type_id)
{
  if (!strcmp(logical_type_id, repoId_))
    return 1;
  else if (!strcmp(logical_type_id, CORBA::Object::_PD_repoId))
    return 1;
  else {
    lockWithNewThreadState _t;
    PyObject* pyisa = PyObject_CallMethod(omniPy::pyomniORBmodule,
					  (char*)"static_is_a", (char*)"Ns",
					  pyskeleton_, logical_type_id);
    if (!pyisa) PyErr_Print();
    OMNIORB_ASSERT(pyisa && PyInt_Check(pyisa));

    CORBA::Boolean isa = PyInt_AS_LONG(pyisa);
    Py_DECREF(pyisa);
    return isa;
  }
}


PortableServer::POA_ptr
omniPy::
Py_omniServant::_default_POA()
{
  lockWithNewThreadState _t;
  PyObject* pyPOA = PyObject_CallMethod(pyservant_, (char*)"_default_POA", 0);

  if (pyPOA) {
    PortableServer::POA_ptr poa =
      (PortableServer::POA_ptr)omniPy::getTwin(pyPOA, POA_TWIN);

    Py_DECREF(pyPOA);
    if (poa) {
      return PortableServer::POA::_duplicate(poa);
    }
    else {
      if (omniORB::trace(1)) {
	omniORB::logger l;
	l << "Python servant returned an invalid object from `_default_POA'.\n"
             "Returning Root POA\n";
      }
    }      
  }
  else {
    if (omniORB::trace(1)) {
      omniORB::logger l;
      l << "Exception while trying to call _default_POA(). "
	   "Returning Root POA\n";
      PyErr_Print();
    }
  }
  CORBA::Object_var obj = omniPy::orb->resolve_initial_references("RootPOA");
  return PortableServer::POA::_narrow(obj);
}


PyObject*
omniPy::
Py_omniServant::py_this()
{
  CORBA::Object_ptr objref;
  {
    omniPy::InterpreterUnlocker _u;
    objref = (CORBA::Object_ptr)_do_this(CORBA::Object::_PD_repoId);
  }
  CORBA::Object_ptr lobjref = omniPy::makeLocalObjRef(repoId_, objref);
  return omniPy::createPyCorbaObjRef(repoId_, lobjref);
}


CORBA::Boolean
omniPy::
Py_omniServant::_dispatch(GIOP_S& giop_s)
{
  int i;
  lockWithNewThreadState _t;

  PyObject* desc = PyDict_GetItemString(opdict_, (char*)giop_s.operation());

  if (!desc) return 0; // Unknown operation name

  OMNIORB_ASSERT(PyTuple_Check(desc));

  PyObject *in_d, *out_d, *exc_d;
  int       in_l,  out_l;

  in_d    = PyTuple_GET_ITEM(desc,0);
  out_d   = PyTuple_GET_ITEM(desc,1);
  exc_d   = PyTuple_GET_ITEM(desc,2);

  OMNIORB_ASSERT(PyTuple_Check(in_d));

  in_l  = PyTuple_GET_SIZE(in_d);

  if (out_d == Py_None)
    out_l = -1;
  else
    out_l = PyTuple_GET_SIZE(out_d);

  // Unmarshal in arguments
  PyObject* argtuple = PyTuple_New(in_l);

  for (i=0; i < in_l; i++) {
    PyTuple_SET_ITEM(argtuple, i,
		     omniPy::unmarshalPyObject(giop_s,
					       PyTuple_GET_ITEM(in_d, i)));
  }

  giop_s.RequestReceived();

  // Do the up-call
  PyObject* method = PyObject_GetAttrString(pyservant_,
					    (char*)giop_s.operation());

  if (!method) {
    PyErr_Clear();
    Py_DECREF(argtuple);
    throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  }

  PyObject* result = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  if (result) {
    // No exception was thrown. Marshal the return value
    if (out_l >= 0) {
      CORBA::ULong msgsize = GIOP_S::ReplyHeaderSize();

      if (out_l == 1) {
	msgsize = omniPy::alignedSize(msgsize,
				      PyTuple_GET_ITEM(out_d, 0),
				      result,
				      CORBA::COMPLETED_MAYBE);
      }
      else if (out_l > 1) {
	for (i=0; i < out_l; i++) {
	  msgsize = omniPy::alignedSize(msgsize,
					PyTuple_GET_ITEM(out_d,  i),
					PyTuple_GET_ITEM(result, i),
					CORBA::COMPLETED_MAYBE);
	}
      }
      giop_s.InitialiseReply(GIOP::NO_EXCEPTION, msgsize);

      if (out_l == 1) {
	omniPy::marshalPyObject(giop_s,
				PyTuple_GET_ITEM(out_d, 0),
				result);
      }
      else if (out_l > 1) {
	for (i=0; i < out_l; i++) {
	  omniPy::marshalPyObject(giop_s,
				  PyTuple_GET_ITEM(out_d,  i),
				  PyTuple_GET_ITEM(result, i));
	}
      }
    }
    else {
      if (giop_s.response_expected()) {
	Py_DECREF(result);
	throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_MAYBE);
      }
    }
    Py_DECREF(result);
    giop_s.ReplyCompleted();
    return 1;
  }
  else {
    // An exception of some sort was thrown
    PyObject *etype, *evalue, *etraceback;
    PyObject *erepoId = 0;
    PyErr_Fetch(&etype, &evalue, &etraceback);
    OMNIORB_ASSERT(etype);

    if (evalue && PyInstance_Check(evalue))
      erepoId = PyObject_GetAttrString(evalue, (char*)"_NP_RepositoryId");

    if (!erepoId) {
      omniORB::log << "omniORBpy: *** Warning: caught an unexpected "
		   << "exception during up-call.\n"
		   << "omniORBPy: Traceback follows:\n";
      omniORB::log.flush();
      PyErr_Restore(etype, evalue, etraceback);
      PyErr_Print();
      throw CORBA::UNKNOWN(0,CORBA::COMPLETED_MAYBE);
    }

    Py_DECREF(etype);
    Py_XDECREF(etraceback);

    // Is it a user exception?
    if (exc_d != Py_None) {
      OMNIORB_ASSERT(PyDict_Check(exc_d));

      PyObject* edesc = PyDict_GetItem(exc_d, erepoId);

      if (edesc) {
	CORBA::ULong msgsize = GIOP_S::ReplyHeaderSize();
	msgsize = omniPy::alignedSize(msgsize, edesc, evalue,
				      CORBA::COMPLETED_MAYBE);
	giop_s.InitialiseReply(GIOP::USER_EXCEPTION, msgsize);
	omniPy::marshalPyObject(giop_s, edesc, evalue);
	giop_s.ReplyCompleted();
	Py_DECREF(erepoId);
	Py_DECREF(evalue);
	return 1;
      }
    }
    omniPy::produceSystemException(evalue, erepoId);
  }
  OMNIORB_ASSERT(0); // Never reach here.
  return 0;
}


PyObject*
omniPy::
Py_omniServant::local_dispatch(const char* op,
			       PyObject*   in_d,  int in_l,
			       PyObject*   out_d, int out_l,
			       PyObject*   exc_d,
			       PyObject*   args)
{
  PyObject* method = PyObject_GetAttrString(pyservant_, (char*)op);
  if (!method) {
    CORBA::NO_IMPLEMENT ex;
    return omniPy::handleSystemException(ex);
  }

  // Copy args which would otherwise have reference semantics
  PyObject* argtuple = PyTuple_New(in_l);
  PyObject* t_o;

  int i, valid = 1;
  for (i=0; i < in_l; i++) {
    t_o = copyArgument(PyTuple_GET_ITEM(in_d, i),
		       PyTuple_GET_ITEM(args, i),
		       CORBA::COMPLETED_NO);
    if (t_o)
      PyTuple_SET_ITEM(argtuple, i, t_o);
    else {
      Py_INCREF(Py_None);
      PyTuple_SET_ITEM(argtuple, i, Py_None);
      valid = 0;
    }
  }

  if (valid) {
    // Do the call
    PyObject* result = PyEval_CallObject(method, argtuple);
    Py_DECREF(method);
    Py_DECREF(argtuple);

    if (result) {
      PyObject* retval;

      if (out_l == -1 || out_l == 0) {
	if (result == Py_None) {
	  return result;
	}
	else {
	  Py_DECREF(result);
	  CORBA::BAD_PARAM ex(0,CORBA::COMPLETED_MAYBE);
	  return omniPy::handleSystemException(ex);
	}
      }
      else if (out_l == 1) {
	retval = copyArgument(PyTuple_GET_ITEM(out_d, 0),
			      result, CORBA::COMPLETED_MAYBE);
      }
      else {
	valid = 1;
	retval = PyTuple_New(out_l);
	
	for (i=0; i < out_l; i++) {
	  t_o = copyArgument(PyTuple_GET_ITEM(out_d, i),
			     PyTuple_GET_ITEM(result, i),
			     CORBA::COMPLETED_MAYBE);
	  if (t_o)
	    PyTuple_SET_ITEM(retval, i, t_o);
	  else {
	    Py_INCREF(Py_None);
	    PyTuple_SET_ITEM(retval, i, Py_None);
	    valid = 0;
	  }
	}
	if (!valid) {
	  Py_DECREF(retval);
	  retval = 0;
	}
      }
      Py_DECREF(result);
      return retval;
    }
    else {
      // The call raised a Python exception
      PyObject *etype, *evalue, *etraceback;
      PyObject *erepoId = 0;
      PyErr_Fetch(&etype, &evalue, &etraceback);
      OMNIORB_ASSERT(etype);

      if (evalue && PyInstance_Check(evalue))
	erepoId = PyObject_GetAttrString(evalue, (char*)"_NP_RepositoryId");

      if (!erepoId) {
	omniORB::log << "omniORBpy: *** Warning: caught an unexpected "
		     << "exception during up-call.\n"
		     << "omniORBPy: Traceback follows:\n";
	omniORB::log.flush();
	PyErr_Restore(etype, evalue, etraceback);
	PyErr_Print();
	CORBA::UNKNOWN ex(0,CORBA::COMPLETED_MAYBE);
	return omniPy::handleSystemException(ex);
      }

      Py_DECREF(etype);
      Py_XDECREF(etraceback);

      // Is it a user exception?
      if (exc_d != Py_None) {
	OMNIORB_ASSERT(PyDict_Check(exc_d));

	PyObject* edesc = PyDict_GetItem(exc_d, erepoId);

	if (edesc) {
	  PyObject* cevalue = copyArgument(edesc, evalue,
					   CORBA::COMPLETED_MAYBE);
	  Py_DECREF(erepoId);
	  Py_DECREF(evalue);

	  if (cevalue) {
	    PyErr_SetObject(PyTuple_GET_ITEM(edesc, 1), cevalue);
	    Py_DECREF(cevalue);
	  }
	  else{
	    CORBA::MARSHAL ex(0,CORBA::COMPLETED_MAYBE);
	    omniPy::handleSystemException(ex);
	  }
	  return 0;
	}
      }
      // System exception?
      PyObject* excc = PyDict_GetItem(pyCORBAsysExcMap, erepoId);
      if (excc) {
	PyObject *pyminor, *pycompl;
	pyminor = PyObject_GetAttrString(evalue, "minor");
	pycompl = PyObject_GetAttrString(evalue, "completed");
	OMNIORB_ASSERT(pyminor && PyInt_Check(pyminor));
	OMNIORB_ASSERT(pycompl && PyInstance_Check(pycompl));
	PyObject* exca = Py_BuildValue((char*)"(NN)", pyminor, pycompl);
	PyObject* exci = PyEval_CallObject(excc, exca);
	Py_DECREF(exca);
	Py_DECREF(erepoId);
	Py_DECREF(evalue);
	if (exci) {
	  PyErr_SetObject(excc, exci);
	  Py_DECREF(exci);
	}
	return 0;
      }
      Py_DECREF(erepoId);
      Py_DECREF(evalue);
      CORBA::UNKNOWN ex(0, CORBA::COMPLETED_MAYBE);
      return omniPy::handleSystemException(ex);
    }
  }
  else {
    // Args were in invalid
    Py_DECREF(argtuple);
    Py_DECREF(method);
    return 0;
  }
}



// Implementation of Py_ServantActivator

Py_ServantActivator::Py_ServantActivator(PyObject*   pysa,
					 PyObject*   opdict,
					 const char* repoId)
  : Py_omniServant(pysa, opdict, repoId), pysa_(pysa)
{
  Py_INCREF(pysa);
}

Py_ServantActivator::~Py_ServantActivator()
{
  lockWithNewThreadState _t;
  Py_DECREF(pysa_);
}

PortableServer::Servant
Py_ServantActivator::incarnate(const PortableServer::ObjectId& oid,
			       PortableServer::POA_ptr         poa)
{
  PyObject *method, *argtuple, *pyservant;
  lockWithNewThreadState _t;

  method = PyObject_GetAttrString(pysa_, (char*)"incarnate");
  if (!method) {
    PyErr_Clear();
    throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?
  }
  PortableServer::POA::_duplicate(poa);
  argtuple = Py_BuildValue((char*)"s#N",
			   (const char*)oid.NP_data(), oid.length(),
			   omniPy::createPyPOAObject(poa));

  // Do the up-call
  pyservant = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  if (pyservant) {
    omniPy::Py_omniServant* servant = omniPy::getServantForPyObject(pyservant);
    Py_DECREF(pyservant);

    if (servant) return servant;
    else         throw CORBA::BAD_PARAM();
  }
  else {
    // An exception of some sort was thrown
    PyObject *etype, *evalue, *etraceback;
    PyObject *erepoId = 0;
    PyErr_Fetch(&etype, &evalue, &etraceback);
    OMNIORB_ASSERT(etype);

    if (evalue && PyInstance_Check(evalue))
      erepoId = PyObject_GetAttrString(evalue, (char*)"_NP_RepositoryId");

    if (!erepoId) {
      omniORB::log << "omniORBpy: *** Warning: caught an unexpected "
		   << "exception during up-call.\n"
		   << "omniORBPy: Traceback follows:\n";
      omniORB::log.flush();
      PyErr_Restore(etype, evalue, etraceback);
      PyErr_Print();
      throw CORBA::UNKNOWN(0,CORBA::COMPLETED_NO);
    }
    Py_DECREF(etype);
    Py_XDECREF(etraceback);

    if (!strcmp(PyString_AS_STRING(erepoId),
		PortableServer::ForwardRequest::_PD_repoId)) {
      Py_DECREF(erepoId);
      PyObject* pyfr = PyObject_GetAttrString(evalue,
					      (char*)"forward_reference");
      Py_DECREF(evalue);
      if (pyfr) {
	CORBA::Object_ptr fr = (CORBA::Object_ptr)omniPy::getTwin(pyfr,
								  OBJREF_TWIN);
	if (fr) {
	  PortableServer::ForwardRequest ex(fr);
	  Py_DECREF(pyfr);
	  throw ex;
	}
      }
      else {
	PyErr_Clear();
	throw CORBA::BAD_PARAM();
      }
    }
    omniPy::produceSystemException(evalue, erepoId);
  }
  OMNIORB_ASSERT(0); // Never reach here
  return 0;
}


void
Py_ServantActivator::etherealize(const PortableServer::ObjectId& oid,
				 PortableServer::POA_ptr poa,
				 PortableServer::Servant serv,
				 CORBA::Boolean          cleanup_in_progress,
				 CORBA::Boolean          remaining_activations)
{
  PyObject *method, *argtuple, *result;
  lockWithNewThreadState _t;

  omniPy::Py_omniServant* pyos;
  pyos = (omniPy::Py_omniServant*)serv->_ptrToInterface("Py_omniServant");
  if (!pyos) throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?

  method = PyObject_GetAttrString(pysa_, (char*)"etherealize");
  if (!method) {
    PyErr_Clear();
    throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?
  }
  PortableServer::POA::_duplicate(poa);
  argtuple = Py_BuildValue((char*)"s#NNii",
			   (const char*)oid.NP_data(), oid.length(),
			   omniPy::createPyPOAObject(poa),
			   pyos->pyServant(),
			   (int)cleanup_in_progress,
			   (int)remaining_activations);
  // Do the up-call
  result = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  pyos->_remove_ref();

  if (result)
    Py_DECREF(result);
  else {
    if (omniORB::trace(5))
      omniORB::logf("omniORBpy: Servant etherealization raised an exception!");
    if (omniORB::trace(10)) {
      omniORB::logf("omniORBpy: Traceback follows:");
      PyErr_Print();
    }
  }
}

void*
Py_ServantActivator::_ptrToInterface(const char* repoId)
{
  if (!strcmp(repoId, CORBA::Object::_PD_repoId))
    return (void*)1;
  if (!strcmp(repoId, "Py_omniServant"))
    return (omniPy::Py_omniServant*)this;
  if (!strcmp(repoId, PortableServer::ServantActivator::_PD_repoId))
    return (PortableServer::_impl_ServantActivator*)this;
  if (!strcmp(repoId, PortableServer::ServantManager::_PD_repoId))
    return (PortableServer::_impl_ServantManager*)this;

  return 0;
}


// Implementation of Py_ServantLocator

Py_ServantLocator::Py_ServantLocator(PyObject*   pysl,
				     PyObject*   opdict,
				     const char* repoId)
  : Py_omniServant(pysl, opdict, repoId), pysl_(pysl)
{
  Py_INCREF(pysl);
}

Py_ServantLocator::~Py_ServantLocator()
{
  lockWithNewThreadState _t;
  Py_DECREF(pysl_);
}

PortableServer::Servant
Py_ServantLocator::preinvoke(const PortableServer::ObjectId& oid,
			     PortableServer::POA_ptr         poa,
			     const char*                     operation,
			     void*&                          cookie)
{
  PyObject *method, *argtuple, *rettuple, *pyservant, *pycookie;
  lockWithNewThreadState _t;

  method = PyObject_GetAttrString(pysl_, (char*)"preinvoke");
  if (!method) {
    PyErr_Clear();
    throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?
  }
  PortableServer::POA::_duplicate(poa);
  argtuple = Py_BuildValue((char*)"s#Ns",
			   (const char*)oid.NP_data(), oid.length(),
			   omniPy::createPyPOAObject(poa),
			   operation);

  // Do the up-call
  rettuple = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  if (rettuple) {
    if (PyTuple_Size(rettuple) != 2) {
      Py_DECREF(rettuple);
      throw CORBA::BAD_PARAM();
    }
    pyservant = PyTuple_GET_ITEM(rettuple, 0);
    pycookie  = PyTuple_GET_ITEM(rettuple, 1);

    omniPy::Py_omniServant* servant = omniPy::getServantForPyObject(pyservant);

    if (servant) {
      Py_INCREF(pycookie);
      cookie = pycookie;
      Py_DECREF(rettuple);
      return servant;
    }
    else {
      Py_DECREF(rettuple);
      throw CORBA::BAD_PARAM();
    }
  }
  else {
    // An exception of some sort was thrown
    PyObject *etype, *evalue, *etraceback;
    PyObject *erepoId = 0;
    PyErr_Fetch(&etype, &evalue, &etraceback);
    OMNIORB_ASSERT(etype);

    if (evalue && PyInstance_Check(evalue))
      erepoId = PyObject_GetAttrString(evalue, (char*)"_NP_RepositoryId");

    if (!erepoId) {
      omniORB::log << "omniORBpy: *** Warning: caught an unexpected "
		   << "exception during up-call.\n"
		   << "omniORBPy: Traceback follows:\n";
      omniORB::log.flush();
      PyErr_Restore(etype, evalue, etraceback);
      PyErr_Print();
      throw CORBA::UNKNOWN(0,CORBA::COMPLETED_NO);
    }
    Py_DECREF(etype);
    Py_XDECREF(etraceback);

    if (!strcmp(PyString_AS_STRING(erepoId),
		PortableServer::ForwardRequest::_PD_repoId)) {
      Py_DECREF(erepoId);
      PyObject* pyfr = PyObject_GetAttrString(evalue,
					      (char*)"forward_reference");
      Py_DECREF(evalue);
      if (pyfr) {
	CORBA::Object_ptr fr = (CORBA::Object_ptr)omniPy::getTwin(pyfr,
								  OBJREF_TWIN);
	if (fr) {
	  PortableServer::ForwardRequest ex(fr);
	  Py_DECREF(pyfr);
	  throw ex;
	}
      }
      else {
	PyErr_Clear();
	throw CORBA::BAD_PARAM();
      }
    }
    omniPy::produceSystemException(evalue, erepoId);
  }
  OMNIORB_ASSERT(0); // Never reach here
  return 0;
}


void
Py_ServantLocator::postinvoke(const PortableServer::ObjectId& oid,
			      PortableServer::POA_ptr         poa,
			      const char*                     operation,
			      void*                           cookie,
			      PortableServer::Servant         serv)
{
  PyObject *method, *argtuple, *result;
  lockWithNewThreadState _t;

  omniPy::Py_omniServant* pyos;
  pyos = (omniPy::Py_omniServant*)serv->_ptrToInterface("Py_omniServant");
  if (!pyos) throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?

  method = PyObject_GetAttrString(pysl_, (char*)"postinvoke");
  if (!method) {
    PyErr_Clear();
    throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?
  }
  PortableServer::POA::_duplicate(poa);
  argtuple = Py_BuildValue((char*)"s#NsNN",
			   (const char*)oid.NP_data(), oid.length(),
			   omniPy::createPyPOAObject(poa),
			   operation,
			   (PyObject*)cookie,
			   pyos->pyServant());
  // Do the up-call
  result = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  pyos->_remove_ref();

  if (result)
    Py_DECREF(result);
  else {
    if (omniORB::trace(5))
      omniORB::logf("omniORBpy: postinvoke raised an exception!");
    if (omniORB::trace(10)) {
      omniORB::logf("omniORBpy: Traceback follows:");
      PyErr_Print();
    }
  }
}

void*
Py_ServantLocator::_ptrToInterface(const char* repoId)
{
  if (!strcmp(repoId, CORBA::Object::_PD_repoId))
    return (void*)1;
  if (!strcmp(repoId, "Py_omniServant"))
    return (omniPy::Py_omniServant*)this;
  if (!strcmp(repoId, PortableServer::ServantLocator::_PD_repoId))
    return (PortableServer::_impl_ServantLocator*)this;
  if (!strcmp(repoId, PortableServer::ServantManager::_PD_repoId))
    return (PortableServer::_impl_ServantManager*)this;

  return 0;
}


// Implementation of Py_AdapterActivator

Py_AdapterActivator::Py_AdapterActivator(PyObject*   pyaa,
					 PyObject*   opdict,
					 const char* repoId)
  : Py_omniServant(pyaa, opdict, repoId), pyaa_(pyaa)
{
  Py_INCREF(pyaa);
}

Py_AdapterActivator::~Py_AdapterActivator()
{
  lockWithNewThreadState _t;
  Py_DECREF(pyaa_);
}

CORBA::Boolean
Py_AdapterActivator::unknown_adapter(PortableServer::POA_ptr parent,
				     const char*             name)
{
  PyObject *method, *argtuple, *pyresult;
  lockWithNewThreadState _t;

  method = PyObject_GetAttrString(pyaa_, (char*)"unknown_adapter");
  if (!method) {
    PyErr_Clear();
    throw CORBA::OBJ_ADAPTER(); // *** Good choice of exn?
  }
  PortableServer::POA::_duplicate(parent);
  argtuple = Py_BuildValue((char*)"Ns",
			   omniPy::createPyPOAObject(parent), name);

  // Do the up-call
  pyresult = PyEval_CallObject(method, argtuple);
  Py_DECREF(method);
  Py_DECREF(argtuple);

  if (pyresult) {
    if (!PyInt_Check(pyresult)) {
      Py_DECREF(pyresult);
      throw CORBA::BAD_PARAM();
    }
    CORBA::Boolean result = PyInt_AS_LONG(pyresult);
    Py_DECREF(pyresult);
    return result;
  }
  else {
    if (omniORB::trace(5))
      omniORB::logf("omniORBpy: AdapterActivator::unknown_adapter "
		    "raised an exception!");
    if (omniORB::trace(10)) {
      omniORB::logf("omniORBpy: Traceback follows:");
      PyErr_Print();
    }
  }
  return 0;
}

void*
Py_AdapterActivator::_ptrToInterface(const char* repoId)
{
  if (!strcmp(repoId, CORBA::Object::_PD_repoId))
    return (void*)1;
  if (!strcmp(repoId, "Py_omniServant"))
    return (omniPy::Py_omniServant*)this;
  if (!strcmp(repoId, PortableServer::AdapterActivator::_PD_repoId))
    return (PortableServer::_impl_AdapterActivator*)this;

  return 0;
}




// Functions to create Py_omniServant objects

static
omniPy::Py_omniServant*
newSpecialServant(PyObject* pyservant, PyObject* opdict, char* repoId)
{
  if (!strcmp(repoId, PortableServer::ServantActivator::_PD_repoId))
    return new Py_ServantActivator(pyservant, opdict, repoId);

  if (!strcmp(repoId, PortableServer::ServantLocator::_PD_repoId))
    return new Py_ServantLocator(pyservant, opdict, repoId);

  if (!strcmp(repoId, PortableServer::AdapterActivator::_PD_repoId))
    return new Py_AdapterActivator(pyservant, opdict, repoId);

  OMNIORB_ASSERT(0);
  return 0;
}


omniPy::Py_omniServant*
omniPy::getServantForPyObject(PyObject* pyservant)
{
  if (!PyInstance_Check(pyservant))
    return 0;

  Py_omniServant* pyos;

  // Is there a Py_omniServant already?
  pyos = (omniPy::Py_omniServant*)omniPy::getTwin(pyservant, SERVANT_TWIN);
  if (pyos) {
    pyos->_add_ref();
    return pyos;
  }

  // Is it an instance of the right class?
  PyClassObject* pysc = ((PyInstanceObject*)pyservant)->in_class;

  if (!PyClass_IsSubclass((PyObject*)pysc, omniPy::pyServantClass))
    return 0;

  PyObject* opdict = PyObject_GetAttrString(pyservant, (char*)"_omni_op_d");
  if (!(opdict && PyDict_Check(opdict)))
    return 0;

  PyObject* pyrepoId = PyObject_GetAttrString(pyservant,
					      (char*)"_NP_RepositoryId");
  if (!(pyrepoId && PyString_Check(pyrepoId))) {
    Py_DECREF(opdict);
    return 0;
  }
  if (PyObject_HasAttrString(pyservant, (char*)"_omni_special")) {

    pyos = newSpecialServant(pyservant, opdict, PyString_AS_STRING(pyrepoId));
  }
  else {
    pyos = new omniPy::Py_omniServant(pyservant, opdict,
				      PyString_AS_STRING(pyrepoId));
  }

  Py_DECREF(opdict);
  Py_DECREF(pyrepoId);

  return pyos;
}
