# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# POA_PortableServer.py      Created on: 2000/02/24
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2000 AT&T Laboratories Cambridge
#
#    This file is part of the omniORBpy library
#
#    The omniORBpy library is free software; you can redistribute it
#    and/or modify it under the terms of the GNU Lesser General
#    Public License as published by the Free Software Foundation;
#    either version 2.1 of the License, or (at your option) any later
#    version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#    MA 02111-1307, USA
#
#
# Description:
#    PortableServer skeletons

# $Id$
# $Log$

import omniORB
from omniORB import CORBA, PortableServer
import _omnipy

# ServantManager skeleton
class ServantManager (PortableServer.Servant):
    _NP_RepositoryId = PortableServer.ServantManager._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {}
    _omni_special = 1

ServantManager._omni_skeleton = ServantManager


# ServantActivator skeleton
class ServantActivator (ServantManager):
    _NP_RepositoryId = PortableServer.ServantActivator._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {"incarnate":  PortableServer.ServantActivator._d_incarnate,
                  "etherealize":PortableServer.ServantActivator._d_etherealize}
    _omni_op_d.update(ServantManager._omni_op_d)
    _omni_special = 1

ServantActivator._omni_skeleton = ServantActivator


# ServantLocator skeleton
class ServantLocator (ServantManager):
    _NP_RepositoryId = PortableServer.ServantLocator._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {"preinvoke":  PortableServer.ServantLocator._d_preinvoke,
                  "postinvoke": PortableServer.ServantLocator._d_postinvoke}
    _omni_op_d.update(ServantManager._omni_op_d)
    _omni_special = 1

ServantLocator._omni_skeleton = ServantLocator



# AdapterActivator skeleton
class AdapterActivator (PortableServer.Servant):
    _NP_RepositoryId = PortableServer.AdapterActivator._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {"unknown_adapter":
                  PortableServer.AdapterActivator._d_unknown_adapter}
    _omni_special = 1

AdapterActivator._omni_skeleton = AdapterActivator
