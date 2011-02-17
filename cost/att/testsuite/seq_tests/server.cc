// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//    02111-1307, USA
//
//
// Usage: server [<seq object name> <shutdown object name>]
//        names are COSNaming compound names (e.g. x/y/test.obj)>]
//

#include <iostream>
#include <common/omnitest.h>
#include <seq1.hh>
#include <shutdown.hh>

using namespace std;


// All the operations below have similar signature: xxx dump_xxx (in xxx a1, inout xxx a2, out xxx a3) in IDL
// The implementation of these operations all looks like this:
//       a2 = a1; a3 = new xxx(a1); return (new xxx(a1));
// Essentially, the in argument a1 is copied and returned as the inout argument a2, out argument a3
// and the result.

class Exercise1_i : public POA_Exercise1,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise1_i() { }
  ~Exercise1_i() {}
  virtual seq_long * dump_seq_long ( const seq_long & a1, seq_long & a2, seq_long_out a3 ) { a2 = a1; a3 = new seq_long(a1); return (new seq_long(a1)); }
  virtual seqseq_long * dump_seqseq_long ( const seqseq_long & a1, seqseq_long & a2, seqseq_long_out a3 ) { a2 = a1; a3 = new seqseq_long(a1); return (new seqseq_long(a1)); }
  virtual seqseqseq_long * dump_seqseqseq_long ( const seqseqseq_long & a1, seqseqseq_long & a2, seqseqseq_long_out a3 ) { a2 = a1; a3 = new seqseqseq_long(a1); return (new seqseqseq_long(a1)); }
  virtual seq_long_10 * dump_seq_long_10 ( const seq_long_10 & a1, seq_long_10 & a2, seq_long_10_out a3 ) { a2 = a1; a3 = new seq_long_10(a1); return (new seq_long_10(a1)); }
  virtual seqseq_long_10_10 * dump_seqseq_long_10 ( const seqseq_long_10_10 & a1, seqseq_long_10_10 & a2, seqseq_long_10_10_out a3 ) { a2 = a1; a3 = new seqseq_long_10_10(a1); return (new seqseq_long_10_10(a1)); }
  virtual seqseq_long_10_10_10 * dump_seqseqseq_long_10 ( const seqseq_long_10_10_10 & a1, seqseq_long_10_10_10 & a2, seqseq_long_10_10_10_out a3 ) { a2 = a1; a3 = new seqseq_long_10_10_10(a1); return (new seqseq_long_10_10_10(a1)); }

};


class Exercise2_i : public POA_Exercise2,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise2_i() { }
  ~Exercise2_i() {}

  virtual seq_string * dump_seq_string ( const seq_string & a1, seq_string & a2, seq_string_out a3 ) { a2 = a1; a3 = new seq_string(a1); return (new seq_string(a1)); }
  virtual seqseq_string * dump_seqseq_string ( const seqseq_string & a1, seqseq_string & a2, seqseq_string_out a3 ) { a2 = a1; a3 = new seqseq_string(a1); return (new seqseq_string(a1)); }
  virtual seqseqseq_string * dump_seqseqseq_string ( const seqseqseq_string & a1, seqseqseq_string & a2, seqseqseq_string_out a3 ) { a2 = a1; a3 = new seqseqseq_string(a1); return (new seqseqseq_string(a1)); }
  virtual seq_string_10 * dump_seq_string_10 ( const seq_string_10 & a1, seq_string_10 & a2, seq_string_10_out a3 ) { a2 = a1; a3 = new seq_string_10(a1); return (new seq_string_10(a1)); }
  virtual seqseq_string_10_10 * dump_seqseq_string_10 ( const seqseq_string_10_10 & a1, seqseq_string_10_10 & a2, seqseq_string_10_10_out a3 ) { a2 = a1; a3 = new seqseq_string_10_10(a1); return (new seqseq_string_10_10(a1)); }
  virtual seqseq_string_10_10_10 * dump_seqseqseq_string_10 ( const seqseq_string_10_10_10 & a1, seqseq_string_10_10_10 & a2, seqseq_string_10_10_10_out a3 ) { a2 = a1; a3 = new seqseq_string_10_10_10(a1); return (new seqseq_string_10_10_10(a1)); }
};

class Exercise3_i : public POA_Exercise3,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise3_i() { }
  ~Exercise3_i() {}

  virtual seq_echo * dump_seq_echo ( const seq_echo & a1, seq_echo & a2, seq_echo_out a3 ) { a2 = a1; a3 = new seq_echo(a1); return (new seq_echo(a1)); }
  virtual seqseq_echo * dump_seqseq_echo ( const seqseq_echo & a1, seqseq_echo & a2, seqseq_echo_out a3 ) { a2 = a1; a3 = new seqseq_echo(a1); return (new seqseq_echo(a1)); }
  virtual seqseqseq_echo * dump_seqseqseq_echo ( const seqseqseq_echo & a1, seqseqseq_echo & a2, seqseqseq_echo_out a3 ) { a2 = a1; a3 = new seqseqseq_echo(a1); return (new seqseqseq_echo(a1)); }
  virtual seq_echo_10 * dump_seq_echo_10 ( const seq_echo_10 & a1, seq_echo_10 & a2, seq_echo_10_out a3 ) { a2 = a1; a3 = new seq_echo_10(a1); return (new seq_echo_10(a1)); }
  virtual seqseq_echo_10_10 * dump_seqseq_echo_10 ( const seqseq_echo_10_10 & a1, seqseq_echo_10_10 & a2, seqseq_echo_10_10_out a3 ) { a2 = a1; a3 = new seqseq_echo_10_10(a1); return (new seqseq_echo_10_10(a1)); }
  virtual seqseq_echo_10_10_10 * dump_seqseqseq_echo_10 ( const seqseq_echo_10_10_10 & a1, seqseq_echo_10_10_10 & a2, seqseq_echo_10_10_10_out a3 ) { a2 = a1; a3 = new seqseq_echo_10_10_10(a1); return (new seqseq_echo_10_10_10(a1)); }

};

class Exercise4_i : public POA_Exercise4,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise4_i() { }
  ~Exercise4_i() {}

  virtual seq_arr_double * dump_seq_arr_double ( const seq_arr_double & a1, seq_arr_double & a2, seq_arr_double_out a3 ) { a2 = a1; a3 = new seq_arr_double(a1); return (new seq_arr_double(a1)); }
  virtual seqseq_arr_double * dump_seqseq_arr_double ( const seqseq_arr_double & a1, seqseq_arr_double & a2, seqseq_arr_double_out a3 ) { a2 = a1; a3 = new seqseq_arr_double(a1); return (new seqseq_arr_double(a1)); }
  virtual seqseqseq_arr_double * dump_seqseqseq_arr_double ( const seqseqseq_arr_double & a1, seqseqseq_arr_double & a2, seqseqseq_arr_double_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_double(a1); return (new seqseqseq_arr_double(a1)); }
  virtual seq_arr_double_10 * dump_seq_arr_double_10 ( const seq_arr_double_10 & a1, seq_arr_double_10 & a2, seq_arr_double_10_out a3 ) { a2 = a1; a3 = new seq_arr_double_10(a1); return (new seq_arr_double_10(a1)); }
  virtual seq_arr_double_10_10 * dump_seq_arr_double_10_10 ( const seq_arr_double_10_10 & a1, seq_arr_double_10_10 & a2, seq_arr_double_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_double_10_10(a1); return (new seq_arr_double_10_10(a1)); }
  virtual seq_arr_double_10_10_10 * dump_seq_arr_double_10_10_10 ( const seq_arr_double_10_10_10 & a1, seq_arr_double_10_10_10 & a2, seq_arr_double_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_double_10_10_10(a1); return (new seq_arr_double_10_10_10(a1)); }
  virtual seq_arrarr_double * dump_seq_arrarr_double ( const seq_arrarr_double & a1, seq_arrarr_double & a2, seq_arrarr_double_out a3 ){ a2 = a1; a3 = new seq_arrarr_double(a1); return (new seq_arrarr_double(a1)); }
  virtual seqseq_arrarr_double * dump_seqseq_arrarr_double ( const seqseq_arrarr_double & a1, seqseq_arrarr_double & a2, seqseq_arrarr_double_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_double(a1); return (new seqseq_arrarr_double(a1)); }
  virtual seqseqseq_arrarr_double * dump_seqseqseq_arrarr_double ( const seqseqseq_arrarr_double & a1, seqseqseq_arrarr_double & a2, seqseqseq_arrarr_double_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_double(a1); return (new seqseqseq_arrarr_double(a1)); }
  virtual seq_arrarr_double_10 * dump_seq_arrarr_double_10 ( const seq_arrarr_double_10 & a1, seq_arrarr_double_10 & a2, seq_arrarr_double_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_double_10(a1); return (new seq_arrarr_double_10(a1)); }
  virtual seq_arrarr_double_10_10 * dump_seq_arrarr_double_10_10 ( const seq_arrarr_double_10_10 & a1, seq_arrarr_double_10_10 & a2, seq_arrarr_double_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_double_10_10(a1); return (new seq_arrarr_double_10_10(a1)); }
  virtual seq_arrarr_double_10_10_10 * dump_seq_arrarr_double_10_10_10 ( const seq_arrarr_double_10_10_10 & a1, seq_arrarr_double_10_10_10 & a2, seq_arrarr_double_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_double_10_10_10(a1); return (new seq_arrarr_double_10_10_10(a1)); }
};

class Exercise5_i : public POA_Exercise5,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise5_i() { }
  ~Exercise5_i() {}

  virtual seq_arr_string * dump_seq_arr_string ( const seq_arr_string & a1, seq_arr_string & a2, seq_arr_string_out a3 ) { a2 = a1; a3 = new seq_arr_string(a1); return (new seq_arr_string(a1)); }
  virtual seqseq_arr_string * dump_seqseq_arr_string ( const seqseq_arr_string & a1, seqseq_arr_string & a2, seqseq_arr_string_out a3 ) { a2 = a1; a3 = new seqseq_arr_string(a1); return (new seqseq_arr_string(a1)); }
  virtual seqseqseq_arr_string * dump_seqseqseq_arr_string ( const seqseqseq_arr_string & a1, seqseqseq_arr_string & a2, seqseqseq_arr_string_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_string(a1); return (new seqseqseq_arr_string(a1)); }
  virtual seq_arr_string_10 * dump_seq_arr_string_10 ( const seq_arr_string_10 & a1, seq_arr_string_10 & a2, seq_arr_string_10_out a3 ) { a2 = a1; a3 = new seq_arr_string_10(a1); return (new seq_arr_string_10(a1)); }
  virtual seq_arr_string_10_10 * dump_seq_arr_string_10_10 ( const seq_arr_string_10_10 & a1, seq_arr_string_10_10 & a2, seq_arr_string_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_string_10_10(a1); return (new seq_arr_string_10_10(a1)); }
  virtual seq_arr_string_10_10_10 * dump_seq_arr_string_10_10_10 ( const seq_arr_string_10_10_10 & a1, seq_arr_string_10_10_10 & a2, seq_arr_string_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_string_10_10_10(a1); return (new seq_arr_string_10_10_10(a1)); }
  virtual seq_arrarr_string * dump_seq_arrarr_string ( const seq_arrarr_string & a1, seq_arrarr_string & a2, seq_arrarr_string_out a3 ){ a2 = a1; a3 = new seq_arrarr_string(a1); return (new seq_arrarr_string(a1)); }
  virtual seqseq_arrarr_string * dump_seqseq_arrarr_string ( const seqseq_arrarr_string & a1, seqseq_arrarr_string & a2, seqseq_arrarr_string_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_string(a1); return (new seqseq_arrarr_string(a1)); }
  virtual seqseqseq_arrarr_string * dump_seqseqseq_arrarr_string ( const seqseqseq_arrarr_string & a1, seqseqseq_arrarr_string & a2, seqseqseq_arrarr_string_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_string(a1); return (new seqseqseq_arrarr_string(a1)); }
  virtual seq_arrarr_string_10 * dump_seq_arrarr_string_10 ( const seq_arrarr_string_10 & a1, seq_arrarr_string_10 & a2, seq_arrarr_string_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_string_10(a1); return (new seq_arrarr_string_10(a1)); }
  virtual seq_arrarr_string_10_10 * dump_seq_arrarr_string_10_10 ( const seq_arrarr_string_10_10 & a1, seq_arrarr_string_10_10 & a2, seq_arrarr_string_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_string_10_10(a1); return (new seq_arrarr_string_10_10(a1)); }
  virtual seq_arrarr_string_10_10_10 * dump_seq_arrarr_string_10_10_10 ( const seq_arrarr_string_10_10_10 & a1, seq_arrarr_string_10_10_10 & a2, seq_arrarr_string_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_string_10_10_10(a1); return (new seq_arrarr_string_10_10_10(a1)); }
};

class Exercise6_i : public POA_Exercise6,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise6_i() { }
  ~Exercise6_i() {}


  virtual seq_arr_Echo * dump_seq_arr_Echo ( const seq_arr_Echo & a1, seq_arr_Echo & a2, seq_arr_Echo_out a3 ) { a2 = a1; a3 = new seq_arr_Echo(a1); return (new seq_arr_Echo(a1)); }
  virtual seqseq_arr_Echo * dump_seqseq_arr_Echo ( const seqseq_arr_Echo & a1, seqseq_arr_Echo & a2, seqseq_arr_Echo_out a3 ) { a2 = a1; a3 = new seqseq_arr_Echo(a1); return (new seqseq_arr_Echo(a1)); }
  virtual seqseqseq_arr_Echo * dump_seqseqseq_arr_Echo ( const seqseqseq_arr_Echo & a1, seqseqseq_arr_Echo & a2, seqseqseq_arr_Echo_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_Echo(a1); return (new seqseqseq_arr_Echo(a1)); }
  virtual seq_arr_Echo_10 * dump_seq_arr_Echo_10 ( const seq_arr_Echo_10 & a1, seq_arr_Echo_10 & a2, seq_arr_Echo_10_out a3 ) { a2 = a1; a3 = new seq_arr_Echo_10(a1); return (new seq_arr_Echo_10(a1)); }
  virtual seq_arr_Echo_10_10 * dump_seq_arr_Echo_10_10 ( const seq_arr_Echo_10_10 & a1, seq_arr_Echo_10_10 & a2, seq_arr_Echo_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_Echo_10_10(a1); return (new seq_arr_Echo_10_10(a1)); }
  virtual seq_arr_Echo_10_10_10 * dump_seq_arr_Echo_10_10_10 ( const seq_arr_Echo_10_10_10 & a1, seq_arr_Echo_10_10_10 & a2, seq_arr_Echo_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_Echo_10_10_10(a1); return (new seq_arr_Echo_10_10_10(a1)); }
  virtual seq_arrarr_Echo * dump_seq_arrarr_Echo ( const seq_arrarr_Echo & a1, seq_arrarr_Echo & a2, seq_arrarr_Echo_out a3 ){ a2 = a1; a3 = new seq_arrarr_Echo(a1); return (new seq_arrarr_Echo(a1)); }
  virtual seqseq_arrarr_Echo * dump_seqseq_arrarr_Echo ( const seqseq_arrarr_Echo & a1, seqseq_arrarr_Echo & a2, seqseq_arrarr_Echo_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_Echo(a1); return (new seqseq_arrarr_Echo(a1)); }
  virtual seqseqseq_arrarr_Echo * dump_seqseqseq_arrarr_Echo ( const seqseqseq_arrarr_Echo & a1, seqseqseq_arrarr_Echo & a2, seqseqseq_arrarr_Echo_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_Echo(a1); return (new seqseqseq_arrarr_Echo(a1)); }
  virtual seq_arrarr_Echo_10 * dump_seq_arrarr_Echo_10 ( const seq_arrarr_Echo_10 & a1, seq_arrarr_Echo_10 & a2, seq_arrarr_Echo_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_Echo_10(a1); return (new seq_arrarr_Echo_10(a1)); }
  virtual seq_arrarr_Echo_10_10 * dump_seq_arrarr_Echo_10_10 ( const seq_arrarr_Echo_10_10 & a1, seq_arrarr_Echo_10_10 & a2, seq_arrarr_Echo_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_Echo_10_10(a1); return (new seq_arrarr_Echo_10_10(a1)); }
  virtual seq_arrarr_Echo_10_10_10 * dump_seq_arrarr_Echo_10_10_10 ( const seq_arrarr_Echo_10_10_10 & a1, seq_arrarr_Echo_10_10_10 & a2, seq_arrarr_Echo_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_Echo_10_10_10(a1); return (new seq_arrarr_Echo_10_10_10(a1)); }
};

class Exercise7_i : public POA_Exercise7,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise7_i() { }
  ~Exercise7_i() {}

  virtual seq_arr_sFixed * dump_seq_arr_sFixed ( const seq_arr_sFixed & a1, seq_arr_sFixed & a2, seq_arr_sFixed_out a3 ) { a2 = a1; a3 = new seq_arr_sFixed(a1); return (new seq_arr_sFixed(a1)); }
  virtual seqseq_arr_sFixed * dump_seqseq_arr_sFixed ( const seqseq_arr_sFixed & a1, seqseq_arr_sFixed & a2, seqseq_arr_sFixed_out a3 ) { a2 = a1; a3 = new seqseq_arr_sFixed(a1); return (new seqseq_arr_sFixed(a1)); }
  virtual seqseqseq_arr_sFixed * dump_seqseqseq_arr_sFixed ( const seqseqseq_arr_sFixed & a1, seqseqseq_arr_sFixed & a2, seqseqseq_arr_sFixed_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_sFixed(a1); return (new seqseqseq_arr_sFixed(a1)); }
  virtual seq_arr_sFixed_10 * dump_seq_arr_sFixed_10 ( const seq_arr_sFixed_10 & a1, seq_arr_sFixed_10 & a2, seq_arr_sFixed_10_out a3 ) { a2 = a1; a3 = new seq_arr_sFixed_10(a1); return (new seq_arr_sFixed_10(a1)); }
  virtual seq_arr_sFixed_10_10 * dump_seq_arr_sFixed_10_10 ( const seq_arr_sFixed_10_10 & a1, seq_arr_sFixed_10_10 & a2, seq_arr_sFixed_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_sFixed_10_10(a1); return (new seq_arr_sFixed_10_10(a1)); }
  virtual seq_arr_sFixed_10_10_10 * dump_seq_arr_sFixed_10_10_10 ( const seq_arr_sFixed_10_10_10 & a1, seq_arr_sFixed_10_10_10 & a2, seq_arr_sFixed_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_sFixed_10_10_10(a1); return (new seq_arr_sFixed_10_10_10(a1)); }
  virtual seq_arrarr_sFixed * dump_seq_arrarr_sFixed ( const seq_arrarr_sFixed & a1, seq_arrarr_sFixed & a2, seq_arrarr_sFixed_out a3 ){ a2 = a1; a3 = new seq_arrarr_sFixed(a1); return (new seq_arrarr_sFixed(a1)); }
  virtual seqseq_arrarr_sFixed * dump_seqseq_arrarr_sFixed ( const seqseq_arrarr_sFixed & a1, seqseq_arrarr_sFixed & a2, seqseq_arrarr_sFixed_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_sFixed(a1); return (new seqseq_arrarr_sFixed(a1)); }
  virtual seqseqseq_arrarr_sFixed * dump_seqseqseq_arrarr_sFixed ( const seqseqseq_arrarr_sFixed & a1, seqseqseq_arrarr_sFixed & a2, seqseqseq_arrarr_sFixed_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_sFixed(a1); return (new seqseqseq_arrarr_sFixed(a1)); }
  virtual seq_arrarr_sFixed_10 * dump_seq_arrarr_sFixed_10 ( const seq_arrarr_sFixed_10 & a1, seq_arrarr_sFixed_10 & a2, seq_arrarr_sFixed_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sFixed_10(a1); return (new seq_arrarr_sFixed_10(a1)); }
  virtual seq_arrarr_sFixed_10_10 * dump_seq_arrarr_sFixed_10_10 ( const seq_arrarr_sFixed_10_10 & a1, seq_arrarr_sFixed_10_10 & a2, seq_arrarr_sFixed_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sFixed_10_10(a1); return (new seq_arrarr_sFixed_10_10(a1)); }
  virtual seq_arrarr_sFixed_10_10_10 * dump_seq_arrarr_sFixed_10_10_10 ( const seq_arrarr_sFixed_10_10_10 & a1, seq_arrarr_sFixed_10_10_10 & a2, seq_arrarr_sFixed_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sFixed_10_10_10(a1); return (new seq_arrarr_sFixed_10_10_10(a1)); }
};

class Exercise8_i : public POA_Exercise8,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise8_i() { }
  ~Exercise8_i() {}

  virtual seq_arr_sVariable * dump_seq_arr_sVariable ( const seq_arr_sVariable & a1, seq_arr_sVariable & a2, seq_arr_sVariable_out a3 ) { a2 = a1; a3 = new seq_arr_sVariable(a1); return (new seq_arr_sVariable(a1)); }
  virtual seqseq_arr_sVariable * dump_seqseq_arr_sVariable ( const seqseq_arr_sVariable & a1, seqseq_arr_sVariable & a2, seqseq_arr_sVariable_out a3 ) { a2 = a1; a3 = new seqseq_arr_sVariable(a1); return (new seqseq_arr_sVariable(a1)); }
  virtual seqseqseq_arr_sVariable * dump_seqseqseq_arr_sVariable ( const seqseqseq_arr_sVariable & a1, seqseqseq_arr_sVariable & a2, seqseqseq_arr_sVariable_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_sVariable(a1); return (new seqseqseq_arr_sVariable(a1)); }
  virtual seq_arr_sVariable_10 * dump_seq_arr_sVariable_10 ( const seq_arr_sVariable_10 & a1, seq_arr_sVariable_10 & a2, seq_arr_sVariable_10_out a3 ) { a2 = a1; a3 = new seq_arr_sVariable_10(a1); return (new seq_arr_sVariable_10(a1)); }
  virtual seq_arr_sVariable_10_10 * dump_seq_arr_sVariable_10_10 ( const seq_arr_sVariable_10_10 & a1, seq_arr_sVariable_10_10 & a2, seq_arr_sVariable_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_sVariable_10_10(a1); return (new seq_arr_sVariable_10_10(a1)); }
  virtual seq_arr_sVariable_10_10_10 * dump_seq_arr_sVariable_10_10_10 ( const seq_arr_sVariable_10_10_10 & a1, seq_arr_sVariable_10_10_10 & a2, seq_arr_sVariable_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_sVariable_10_10_10(a1); return (new seq_arr_sVariable_10_10_10(a1)); }
  virtual seq_arrarr_sVariable * dump_seq_arrarr_sVariable ( const seq_arrarr_sVariable & a1, seq_arrarr_sVariable & a2, seq_arrarr_sVariable_out a3 ){ a2 = a1; a3 = new seq_arrarr_sVariable(a1); return (new seq_arrarr_sVariable(a1)); }
  virtual seqseq_arrarr_sVariable * dump_seqseq_arrarr_sVariable ( const seqseq_arrarr_sVariable & a1, seqseq_arrarr_sVariable & a2, seqseq_arrarr_sVariable_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_sVariable(a1); return (new seqseq_arrarr_sVariable(a1)); }
  virtual seqseqseq_arrarr_sVariable * dump_seqseqseq_arrarr_sVariable ( const seqseqseq_arrarr_sVariable & a1, seqseqseq_arrarr_sVariable & a2, seqseqseq_arrarr_sVariable_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_sVariable(a1); return (new seqseqseq_arrarr_sVariable(a1)); }
  virtual seq_arrarr_sVariable_10 * dump_seq_arrarr_sVariable_10 ( const seq_arrarr_sVariable_10 & a1, seq_arrarr_sVariable_10 & a2, seq_arrarr_sVariable_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sVariable_10(a1); return (new seq_arrarr_sVariable_10(a1)); }
  virtual seq_arrarr_sVariable_10_10 * dump_seq_arrarr_sVariable_10_10 ( const seq_arrarr_sVariable_10_10 & a1, seq_arrarr_sVariable_10_10 & a2, seq_arrarr_sVariable_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sVariable_10_10(a1); return (new seq_arrarr_sVariable_10_10(a1)); }
  virtual seq_arrarr_sVariable_10_10_10 * dump_seq_arrarr_sVariable_10_10_10 ( const seq_arrarr_sVariable_10_10_10 & a1, seq_arrarr_sVariable_10_10_10 & a2, seq_arrarr_sVariable_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_sVariable_10_10_10(a1); return (new seq_arrarr_sVariable_10_10_10(a1)); }
};

class Exercise9_i : public POA_Exercise9,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise9_i() { }
  ~Exercise9_i() {}

  virtual seq_arr_uFixed * dump_seq_arr_uFixed ( const seq_arr_uFixed & a1, seq_arr_uFixed & a2, seq_arr_uFixed_out a3 ) { a2 = a1; a3 = new seq_arr_uFixed(a1); return (new seq_arr_uFixed(a1)); }
  virtual seqseq_arr_uFixed * dump_seqseq_arr_uFixed ( const seqseq_arr_uFixed & a1, seqseq_arr_uFixed & a2, seqseq_arr_uFixed_out a3 ) { a2 = a1; a3 = new seqseq_arr_uFixed(a1); return (new seqseq_arr_uFixed(a1)); }
  virtual seqseqseq_arr_uFixed * dump_seqseqseq_arr_uFixed ( const seqseqseq_arr_uFixed & a1, seqseqseq_arr_uFixed & a2, seqseqseq_arr_uFixed_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_uFixed(a1); return (new seqseqseq_arr_uFixed(a1)); }
  virtual seq_arr_uFixed_10 * dump_seq_arr_uFixed_10 ( const seq_arr_uFixed_10 & a1, seq_arr_uFixed_10 & a2, seq_arr_uFixed_10_out a3 ) { a2 = a1; a3 = new seq_arr_uFixed_10(a1); return (new seq_arr_uFixed_10(a1)); }
  virtual seq_arr_uFixed_10_10 * dump_seq_arr_uFixed_10_10 ( const seq_arr_uFixed_10_10 & a1, seq_arr_uFixed_10_10 & a2, seq_arr_uFixed_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_uFixed_10_10(a1); return (new seq_arr_uFixed_10_10(a1)); }
  virtual seq_arr_uFixed_10_10_10 * dump_seq_arr_uFixed_10_10_10 ( const seq_arr_uFixed_10_10_10 & a1, seq_arr_uFixed_10_10_10 & a2, seq_arr_uFixed_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_uFixed_10_10_10(a1); return (new seq_arr_uFixed_10_10_10(a1)); }
  virtual seq_arrarr_uFixed * dump_seq_arrarr_uFixed ( const seq_arrarr_uFixed & a1, seq_arrarr_uFixed & a2, seq_arrarr_uFixed_out a3 ){ a2 = a1; a3 = new seq_arrarr_uFixed(a1); return (new seq_arrarr_uFixed(a1)); }
  virtual seqseq_arrarr_uFixed * dump_seqseq_arrarr_uFixed ( const seqseq_arrarr_uFixed & a1, seqseq_arrarr_uFixed & a2, seqseq_arrarr_uFixed_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_uFixed(a1); return (new seqseq_arrarr_uFixed(a1)); }
  virtual seqseqseq_arrarr_uFixed * dump_seqseqseq_arrarr_uFixed ( const seqseqseq_arrarr_uFixed & a1, seqseqseq_arrarr_uFixed & a2, seqseqseq_arrarr_uFixed_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_uFixed(a1); return (new seqseqseq_arrarr_uFixed(a1)); }
  virtual seq_arrarr_uFixed_10 * dump_seq_arrarr_uFixed_10 ( const seq_arrarr_uFixed_10 & a1, seq_arrarr_uFixed_10 & a2, seq_arrarr_uFixed_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uFixed_10(a1); return (new seq_arrarr_uFixed_10(a1)); }
  virtual seq_arrarr_uFixed_10_10 * dump_seq_arrarr_uFixed_10_10 ( const seq_arrarr_uFixed_10_10 & a1, seq_arrarr_uFixed_10_10 & a2, seq_arrarr_uFixed_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uFixed_10_10(a1); return (new seq_arrarr_uFixed_10_10(a1)); }
  virtual seq_arrarr_uFixed_10_10_10 * dump_seq_arrarr_uFixed_10_10_10 ( const seq_arrarr_uFixed_10_10_10 & a1, seq_arrarr_uFixed_10_10_10 & a2, seq_arrarr_uFixed_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uFixed_10_10_10(a1); return (new seq_arrarr_uFixed_10_10_10(a1)); }
};

class Exercise10_i : public POA_Exercise10,
		    public PortableServer::RefCountServantBase
{
public:
  Exercise10_i() { }
  ~Exercise10_i() {}


  virtual seq_arr_uVariable * dump_seq_arr_uVariable ( const seq_arr_uVariable & a1, seq_arr_uVariable & a2, seq_arr_uVariable_out a3 ) { a2 = a1; a3 = new seq_arr_uVariable(a1); return (new seq_arr_uVariable(a1)); }
  virtual seqseq_arr_uVariable * dump_seqseq_arr_uVariable ( const seqseq_arr_uVariable & a1, seqseq_arr_uVariable & a2, seqseq_arr_uVariable_out a3 ) { a2 = a1; a3 = new seqseq_arr_uVariable(a1); return (new seqseq_arr_uVariable(a1)); }
  virtual seqseqseq_arr_uVariable * dump_seqseqseq_arr_uVariable ( const seqseqseq_arr_uVariable & a1, seqseqseq_arr_uVariable & a2, seqseqseq_arr_uVariable_out a3 ) { a2 = a1; a3 = new seqseqseq_arr_uVariable(a1); return (new seqseqseq_arr_uVariable(a1)); }
  virtual seq_arr_uVariable_10 * dump_seq_arr_uVariable_10 ( const seq_arr_uVariable_10 & a1, seq_arr_uVariable_10 & a2, seq_arr_uVariable_10_out a3 ) { a2 = a1; a3 = new seq_arr_uVariable_10(a1); return (new seq_arr_uVariable_10(a1)); }
  virtual seq_arr_uVariable_10_10 * dump_seq_arr_uVariable_10_10 ( const seq_arr_uVariable_10_10 & a1, seq_arr_uVariable_10_10 & a2, seq_arr_uVariable_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_uVariable_10_10(a1); return (new seq_arr_uVariable_10_10(a1)); }
  virtual seq_arr_uVariable_10_10_10 * dump_seq_arr_uVariable_10_10_10 ( const seq_arr_uVariable_10_10_10 & a1, seq_arr_uVariable_10_10_10 & a2, seq_arr_uVariable_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arr_uVariable_10_10_10(a1); return (new seq_arr_uVariable_10_10_10(a1)); }
  virtual seq_arrarr_uVariable * dump_seq_arrarr_uVariable ( const seq_arrarr_uVariable & a1, seq_arrarr_uVariable & a2, seq_arrarr_uVariable_out a3 ){ a2 = a1; a3 = new seq_arrarr_uVariable(a1); return (new seq_arrarr_uVariable(a1)); }
  virtual seqseq_arrarr_uVariable * dump_seqseq_arrarr_uVariable ( const seqseq_arrarr_uVariable & a1, seqseq_arrarr_uVariable & a2, seqseq_arrarr_uVariable_out a3 ) { a2 = a1; a3 = new seqseq_arrarr_uVariable(a1); return (new seqseq_arrarr_uVariable(a1)); }
  virtual seqseqseq_arrarr_uVariable * dump_seqseqseq_arrarr_uVariable ( const seqseqseq_arrarr_uVariable & a1, seqseqseq_arrarr_uVariable & a2, seqseqseq_arrarr_uVariable_out a3 ) { a2 = a1; a3 = new seqseqseq_arrarr_uVariable(a1); return (new seqseqseq_arrarr_uVariable(a1)); }
  virtual seq_arrarr_uVariable_10 * dump_seq_arrarr_uVariable_10 ( const seq_arrarr_uVariable_10 & a1, seq_arrarr_uVariable_10 & a2, seq_arrarr_uVariable_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uVariable_10(a1); return (new seq_arrarr_uVariable_10(a1)); }
  virtual seq_arrarr_uVariable_10_10 * dump_seq_arrarr_uVariable_10_10 ( const seq_arrarr_uVariable_10_10 & a1, seq_arrarr_uVariable_10_10 & a2, seq_arrarr_uVariable_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uVariable_10_10(a1); return (new seq_arrarr_uVariable_10_10(a1)); }
  virtual seq_arrarr_uVariable_10_10_10 * dump_seq_arrarr_uVariable_10_10_10 ( const seq_arrarr_uVariable_10_10_10 & a1, seq_arrarr_uVariable_10_10_10 & a2, seq_arrarr_uVariable_10_10_10_out a3 ) { a2 = a1; a3 = new seq_arrarr_uVariable_10_10_10(a1); return (new seq_arrarr_uVariable_10_10_10(a1)); }
};

class Exercise_i : public POA_Exercise,
		   public PortableServer::RefCountServantBase
{
public:
  Exercise_i(PortableServer::POA_ptr poa) { 
    PortableServer::ObjectId_var id;    
    CORBA::Object_var obj;

    pd_1 = new Exercise1_i;
    id = poa->activate_object(pd_1);
    pd_1->_remove_ref();

    pd_2 = new Exercise2_i;
    id = poa->activate_object(pd_2);
    pd_2->_remove_ref();

    pd_3 = new Exercise3_i;
    id = poa->activate_object(pd_3);
    pd_3->_remove_ref();

    pd_4 = new Exercise4_i;
    id = poa->activate_object(pd_4);
    pd_4->_remove_ref();

    pd_5 = new Exercise5_i;
    id = poa->activate_object(pd_5);
    pd_5->_remove_ref();

    pd_6 = new Exercise6_i;
    id = poa->activate_object(pd_6);
    pd_6->_remove_ref();

    pd_7 = new Exercise7_i;
    id = poa->activate_object(pd_7);
    pd_7->_remove_ref();

    pd_8 = new Exercise8_i;
    id = poa->activate_object(pd_8);
    pd_8->_remove_ref();

    pd_9 = new Exercise9_i;
    id = poa->activate_object(pd_9);
    pd_9->_remove_ref();

    pd_10 = new Exercise10_i;
    id = poa->activate_object(pd_10);
    pd_10->_remove_ref();

  }
  ~Exercise_i() {}

  Exercise1_ptr ex1() { return pd_1->_this(); }
  Exercise2_ptr ex2() { return pd_2->_this(); }
  Exercise3_ptr ex3() { return pd_3->_this(); }
  Exercise4_ptr ex4() { return pd_4->_this(); }
  Exercise5_ptr ex5() { return pd_5->_this(); }
  Exercise6_ptr ex6() { return pd_6->_this(); }
  Exercise7_ptr ex7() { return pd_7->_this(); }
  Exercise8_ptr ex8() { return pd_8->_this(); }
  Exercise9_ptr ex9() { return pd_9->_this(); }
  Exercise10_ptr ex10() { return pd_10->_this(); }

private:
  Exercise1_i* pd_1;
  Exercise2_i* pd_2;
  Exercise3_i* pd_3;
  Exercise4_i* pd_4;
  Exercise5_i* pd_5;
  Exercise6_i* pd_6;
  Exercise7_i* pd_7;
  Exercise8_i* pd_8;
  Exercise9_i* pd_9;
  Exercise10_i* pd_10;
};


class MyApp : public OmniServerApp {
public:
  virtual int initServer(int argc, char* argv[]);
};

int
MyApp::initServer(int argc, char** argv)
{
  Exercise_i *server_impl = new Exercise_i(poa());
  PortableServer::ObjectId_var server_id = poa()->activate_object(server_impl);
  CORBA::Object_var server_obj = server_impl->_this();
  server_impl->_remove_ref();
  server_object(server_obj);
  return 0;
}

static MyApp theApp;
