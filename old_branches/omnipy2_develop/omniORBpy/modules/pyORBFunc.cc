// -*- Mode: C++; -*-
//                            Package   : omniORBpy
// pyORBFunc.cc               Created on: 2000/02/04
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
//    ORB functions

// $Id$

// $Log$
// Revision 1.1.2.1  2000/10/13 13:55:25  dpg1
// Initial support for omniORB 4.
//


#include <omnipy.h>
#include <pyThreadCache.h>
#include <initialiser.h>

extern "C" {

  static PyObject*
  pyORB_string_to_object(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;
    char* s;

    if (!PyArg_ParseTuple(args, (char*)"Os", &pyorb, &s))
      return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);

    OMNIORB_ASSERT(orb);

    if (!s || strlen(s) == 0) {
      CORBA::INV_OBJREF ex;
      return omniPy::handleSystemException(ex);
    }
    CORBA::Object_ptr objref;

    try {
      objref = omniPy::stringToObject(s);
    }
    OMNIPY_CATCH_AND_HANDLE_SYSTEM_EXCEPTIONS
    return omniPy::createPyCorbaObjRef(0, objref);
  }

  static PyObject*
  pyORB_object_to_string(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;
    PyObject* pyobjref;

    if (!PyArg_ParseTuple(args, (char*)"OO", &pyorb, &pyobjref))
      return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);

    OMNIORB_ASSERT(orb);

    CORBA::Object_ptr objref;

    if (pyobjref == Py_None) {
      objref = CORBA::Object::_nil();
    }
    else {
      if (!PyInstance_Check(pyobjref)) {
	PyErr_SetString(PyExc_TypeError,
			(char*)"Argument must be an object reference.");
	return NULL;
      }	
      objref = (CORBA::Object_ptr)omniPy::getTwin(pyobjref, OBJREF_TWIN);
    }
    RAISE_PY_BAD_PARAM_IF(!objref);

    CORBA::String_var str = orb->object_to_string(objref);

    return PyString_FromString((char*)str);
  }

  static PyObject*
  pyORB_list_initial_services(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;

    if (!PyArg_ParseTuple(args, (char*)"O", &pyorb))
      return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);
    OMNIORB_ASSERT(orb);

    CORBA::ORB::ObjectIdList_var ids = orb->list_initial_services();

    PyObject* pyids = PyList_New(ids->length());

    for (CORBA::ULong i=0; i<ids->length(); i++) {
      PyList_SetItem(pyids, i, PyString_FromString(ids[i]));
    }
    return pyids;
  }

  static PyObject*
  pyORB_resolve_initial_references(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;
    char*     id;

    if (!PyArg_ParseTuple(args, (char*)"Os", &pyorb, &id))
      return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);
    OMNIORB_ASSERT(orb);

    CORBA::Object_ptr objref;

    try {
      objref = orb->resolve_initial_references(id);
    }
    catch (CORBA::ORB::InvalidName& ex) {
      PyObject* excc = PyObject_GetAttrString(pyorb, (char*)"InvalidName");
      OMNIORB_ASSERT(excc);
      PyObject* exci = PyEval_CallObject(excc, omniPy::pyEmptyTuple);
      PyErr_SetObject(excc, exci);
      return 0;
    }
    OMNIPY_CATCH_AND_HANDLE_SYSTEM_EXCEPTIONS

    if (!(CORBA::is_nil(objref) || objref->_NP_is_pseudo())) {
      omniObjRef* cxxref = objref->_PR_getobj();
      omniObjRef* pyref  = omniPy::createObjRef(CORBA::Object::_PD_repoId,
						cxxref->_getIOR(), 0, 0);
      CORBA::release(objref);
      objref =
	(CORBA::Object_ptr)pyref->_ptrToObjRef(CORBA::Object::_PD_repoId);
    }
    return omniPy::createPyCorbaObjRef(0, objref);
  }

  static PyObject*
  pyORB_shutdown(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;
    int       wait;

    if (!PyArg_ParseTuple(args, (char*)"Oi", &pyorb, &wait)) return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);
    OMNIORB_ASSERT(orb);

    try {
      omniPy::InterpreterUnlocker _u;
      orb->shutdown(wait);
    }
    OMNIPY_CATCH_AND_HANDLE_SYSTEM_EXCEPTIONS

    Py_INCREF(Py_None);
    return Py_None;
  }

  static PyObject*
  pyORB_destroy(PyObject* self, PyObject* args)
  {
    PyObject* pyorb;

    if (!PyArg_ParseTuple(args, (char*)"O", &pyorb)) return NULL;

    CORBA::ORB_ptr orb = (CORBA::ORB_ptr)omniPy::getTwin(pyorb, ORB_TWIN);
    OMNIORB_ASSERT(orb);

    try {
      omniPy::InterpreterUnlocker _u;
      orb->destroy();
    }
    OMNIPY_CATCH_AND_HANDLE_SYSTEM_EXCEPTIONS

    Py_INCREF(Py_None);
    return Py_None;
  }


  ////////////////////////////////////////////////////////////////////////////
  // Python method table                                                    //
  ////////////////////////////////////////////////////////////////////////////

  static PyMethodDef pyORB_methods[] = {
    {(char*)"string_to_object", pyORB_string_to_object,          METH_VARARGS},
    {(char*)"object_to_string", pyORB_object_to_string,          METH_VARARGS},
    {(char*)"list_initial_services",
                                pyORB_list_initial_services,     METH_VARARGS},
    {(char*)"resolve_initial_references",
                                pyORB_resolve_initial_references,METH_VARARGS},
    {(char*)"shutdown",         pyORB_shutdown,                  METH_VARARGS},
    {(char*)"destroy",          pyORB_destroy,                   METH_VARARGS},
    {NULL,NULL}
  };
}


class omni_python_initialiser : public omniInitialiser {
public:
  void attach() { }
  void detach() {
    omnipyThreadCache::shutdown();
    delete this;
  }
};



void
omniPy::initORBFunc(PyObject* d)
{
  PyObject* m = Py_InitModule((char*)"_omnipy.orb_func", pyORB_methods);
  PyDict_SetItemString(d, (char*)"orb_func", m);

  omni_python_initialiser* init = new omni_python_initialiser();
  omniInitialiser::install(init);
}
