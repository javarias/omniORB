// -*- Mode: C++; -*-
//                          Package   : omniidl2
// o2be_union_branch.cc     Created on: 12/08/1996
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

/*
  $Log$
// Revision 1.2  1997/05/06  14:10:45  sll
// Public release.
//
  */

#include <idl.hh>
#include <idl_extern.hh>
#include <o2be.h>

o2be_union_branch::o2be_union_branch(AST_UnionLabel *lab, AST_Type *ft,
				     UTL_ScopedName *n, UTL_StrList *p)
                    : AST_Decl(AST_Decl::NT_union_branch, n, p),
                      AST_Field(AST_Decl::NT_union_branch, ft, n, p),
                      AST_UnionBranch(lab, ft, n, p),
		      o2be_field(ft,n,p),
		      o2be_name(AST_Decl::NT_union_branch, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(o2be_union_branch, AST_UnionBranch)
IMPL_NARROW_FROM_DECL(o2be_union_branch)

