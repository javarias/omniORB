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
// anyUnionTest_i.cc
//
#include <iostream>
#include <stdlib.h>

#include "anyParse.h"

#include "anyUnionTest_i.h"

using namespace std;


static CORBA::Boolean viewUnion(const CORBA::Any& a)
{
  const unionLong* asUn;

  const unionVariable* csUn;
  
  const seqUnion* aSeq;
  arrayUnion_forany aArr;

  const unionSequence* dsUn;
  const unionArray* esUn;
  const unionUnion* fsUn;
  const unionStruct* gsUn;

  const structUnion* aStr;
  const CORBA::Any*  b;

  const unionDouble1* bsUn;
  const unionDouble2* hsUn;

  if (a >>= b)
    {
      cout << "Any contains any: " << endl;
      return viewUnion(*b);
    }
  else if (a >>= asUn)
    {
      cout << "unionLong: " << endl;    
      cout << "Discriminant Value: " << (CORBA::Long) asUn->_d() << endl;
      cout << "Union Value:";
      switch(asUn->_d())
	{
	case 1:
	  cout << " long: " << asUn->l() << endl;
	  break;

	case 2:
	  cout << " short: " << asUn->s() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      return 1;
    }
  else if (a >>= bsUn)
    {
      cout << "unionDouble1: " << endl;    

      cout << "Discriminant Value: " <<  (CORBA::Long) bsUn->_d() << endl;
      cout << "Union Value:";
      switch(bsUn->_d())
	{
	case 1:
	  cout << " double: ";
	  cout<< bsUn->d() << endl;
	  break;

	default:
	  cout << " default (long) : " << bsUn->l() << endl;
	  break;
	}

      return 1;
    }
  else if (a >>= csUn)
    {
      cout << "unionVariable: " << endl;

      cout << "Discriminant Value  (enum) : " <<  (CORBA::ULong) csUn->_d() 
	   << endl;
      cout << "Union Value:";
      switch(csUn->_d())
	{
	case A:
	  cout << " string: " << csUn->str() << endl;
	  break;

	case B:
	  cout << " double: ";
	  cout << csUn->d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      
      cout << "-- [Test extracting again] --" << endl;
      const unionVariable* tmpSun;
      a >>= tmpSun;

      cout << "Discriminant Value  (enum) : " <<  (CORBA::ULong) tmpSun->_d() 
	   << endl;
      cout << "Union Value:";
      switch(tmpSun->_d())
	{
	case A:
	  cout << " string: " << tmpSun->str() << endl;
	  break;

	case B:
	  cout << " double: ";
	  cout << tmpSun->d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}

      return 1;
    }
  else if (a >>= aSeq)
    {

      cout << "seqUnion: " << endl;
      cout << "Number of members: " << aSeq->length() << endl;
      for (CORBA::ULong count = 0; count < aSeq->length(); count++)
	{
	  cout << "[" << count << "]" << endl;
	  cout << "   Discriminant Value  (enum) : " 
	       <<  (CORBA::ULong) ((*aSeq)[count])._d() << endl;
	  cout << "   Union Value:";
	  switch( ((*aSeq)[count])._d())
	    {
	    case A:
	      cout << "    string: " << ((*aSeq)[count]).str() << endl;
	      break;
	      
	    case B:
	      cout << "    double: ";
	      cout << ((*aSeq)[count]).d() << endl;
	      break;
	      
	    default:
	      cerr << "   Error getting union value. Test failed." << endl;
	      exit(-1);
	      break;
	    }	  
	}
      return 1;
    }
  else if (a >>= aArr)
    {
      cout << "arrayUnion: " << endl;
      for (CORBA::ULong count = 0; count < 2; count++)
	{
	  cout << "[" << count << "]" << endl;
	  cout << "   Discriminant Value  (enum) : " 
	       << (CORBA::Long) (aArr[count])._d() << endl;
	  cout << "   Union Value:";
	  switch((aArr[count])._d())
	    {
	    case A:
	      cout << "    string: " << (aArr[count]).str() << endl;
	      break;
	      
	    case B:
	      cout << "    double: ";
	      cout << (aArr[count]).d() << endl;
	      break;
	      
	    default:
	      cerr << "   Error getting union value. Test failed." << endl;
	      exit(-1);
	      break;
	    }           
	}
      return 1;
    }
  else if (a >>= dsUn)
    {
      cout << "unionSequence: " << endl;
      cout << "   Discriminant Value: " << (CORBA::Long) dsUn->_d() << endl;
      cout << "   Union Value:" << endl;
      switch(dsUn->_d())
	{
	case 1:
	  {
	    cout << "  seqUnion: " << endl;
	    cout << "    Number of members: " << (dsUn->s()).length() << endl;
	    CORBA::ULong count = 0;
	    for (count = 0; count < (dsUn->s()).length(); count++)
	      {
		cout << "    [" << count << "]" << endl;
		
		cout << "   Discriminant Value  (enum) : " 
		     << (CORBA::ULong) ((dsUn->s())[count])._d() << endl;
		cout << "   Union Value:" << endl;
		switch(((dsUn->s())[count])._d())
		       {
		       case A:
			 cout << " string: " << ((dsUn->s())[count]).str() 
			      << endl;
			 break;
			 
		       case B:
			 cout << " double: ";
			 cout << ((dsUn->s())[count]).d() 
			      << endl;
			 break;
			 
		       default:
			 cerr << "Error getting union value. Test failed." 
			      << endl;
			 exit(-1);
			 break;
		       }
	      }
	    break;
	  }
	case 2:
	  cout << "Double: ";
	  cout << dsUn->d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      

      cout << "-- [Test extracting again] --" << endl;

      const unionSequence* tmpsUn;
      a >>= tmpsUn;

      
      switch(tmpsUn->_d())
	{
	case 1:
	  {
	    cout << "  seqUnion: " << endl;
	    cout << "    Number of members: " << (tmpsUn->s()).length() << endl;
	    for (CORBA::ULong count = 0; count < (tmpsUn->s()).length(); count++)
	      {
		cout << "    [" << count << "]" << endl;

		cout << "   Discriminant Value  (enum) : " 
		     << (CORBA::ULong) ((tmpsUn->s())[count])._d() << endl;
		cout << "   Union Value: ";		
		switch(((tmpsUn->s())[count])._d())
		       {
		       case A:
			 cout << " string: " << ((tmpsUn->s())[count]).str() 
			      << endl;
			 break;
			 
		       case B:
			 cout << " double: "; 
			 cout << ((tmpsUn->s())[count]).d() 
			      << endl;
			 break;
			 
		       default:
			 cerr << "Error getting union value. Test failed." 
			      << endl;
			 exit(-1);
			 break;
		       }
	      }
	    break;
	  }
	case 2:
	  cout << "Double: ";
	  cout << tmpsUn->d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      
      return 1;
    }
  else if (a >>= esUn)
    {
      cout << "unionArray: " << endl;
      cout << "   Discriminant Value: " << (CORBA::Char) esUn->_d() << endl;

      switch(esUn->_d())
	{
	case 'a':
	  {
	    cout << "  arrayUnion:" << endl;
	    for (CORBA::ULong count = 0; count < 2; count++)
	      {
		cout << "[" << count << "]" << endl;
		cout << "   Discriminant Value  (enum) : " 
		     << (CORBA::ULong) ((esUn->ar())[count])._d() << endl;
		cout << "   Union Value:";
		switch(((esUn->ar())[count])._d())
		  {
		  case A:
		    cout << "    string: " << ((esUn->ar())[count]).str() 
			 << endl;
		    break;
		    
		  case B:
		    cout << "    double: "; 
		    cout << ((esUn->ar())[count]).d() << endl;
		    break;
		    
		  default:
		    cerr << "   Error getting union value. Test failed." 
			 << endl;
		    exit(-1);
		    break;
		  }           
	      }
	    break;
	  }
	case 'b':
	  cout << "Double: ";
	  cout << esUn->d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}

      return 1;
    }
  else if (a >>= fsUn)
    {
      cout << "unionUnion:" << endl;
      cout << "   Discriminant Value: " << (CORBA::Short) fsUn->_d() << endl;
      switch(fsUn->_d())
	{
	case 1:
	  {
	    cout << "Double: ";
	    cout << fsUn->d() << endl;
	    break;
	  }
	case 2:
	  {
	    cout << "unionSequence:" << endl;
	    cout << "   Discriminant Value: " 
		 << (CORBA::Long) (fsUn->st())._d() << endl;
	    cout << "Union Value:" << endl;
	    switch((fsUn->st())._d())
	      {
	      case 1:
		{
		  cout << "  seqUnion: " << endl;
		  cout << "    Number of members: " 
		       << ((fsUn->st()).s()).length() << endl;
		  for (CORBA::ULong count = 0; 
		       count < ((fsUn->st()).s()).length(); count++)
		    {
		      cout << "    [" << count << "]" << endl;
		      cout << "   Discriminant Value: (enum) " 
			   << (CORBA::ULong) (((fsUn->st()).s())[count])._d() 
			   << endl;
		      cout << "   Union Value:";
		      switch((((fsUn->st()).s())[count])._d())
		       {
		       case A:
			 cout << " string: " 
			      << (((fsUn->st()).s())[count]).str() << endl;
			 break;
			 
		       case B:
			 cout << " double: ";
			 cout << (((fsUn->st()).s())[count]).d() << endl;
			 break;
			 
		       default:
			 cerr << "Error getting union value. Test failed." 
			      << endl;
			 exit(-1);
			 break;
		       }
		    }
		  break;
		}
	      case 2:
		cout << "Double: ";
		cout << (fsUn->st()).d() << endl;
		break;
	  
	      default:
		cerr << "Error getting union value. Test failed." << endl;
		exit(-1);
		break;
	      }
	    break;
	  }
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      return 1;
    }
  else if (a >>= gsUn)
    {
      cout << "unionStruct: " << endl;
      cout << "Discriminant Value: " << (CORBA::Long) gsUn->_d() << endl;
      cout << "Union Value:";
      switch(gsUn->_d())
	{
	case 1:
	  cout << " double: ";
	  cout << gsUn->d() << endl;
	  break;

	case 2:
	  {
	    cout << "testStruct: " << endl;
	    cout << "  testStruct.l : " << (gsUn->tS()).l << endl;
	    cout << "  testStruct.d : " << (gsUn->tS()).d << endl;
	    break;
	  }

	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}

      return 1;
    }
  else if (a >>= aStr)
    {
      cout << "structUnion:" << endl;
      cout << " structUnion.d : " << aStr->d << endl;
      cout << " structUnion.u : " << endl;
      cout << "  Discriminant value : (enum) " << (CORBA::ULong) (aStr->u)._d()
	   << endl;

      cout << "Union Value:";
      switch((aStr->u)._d())
	{
	case A:
	  cout << " string: " << (aStr->u).str() << endl;
	  break;

	case B:
	  cout << " double: ";
	  cout << (aStr->u).d() << endl;
	  break;
	  
	default:
	  cerr << "Error getting union value. Test failed." << endl;
	  exit(-1);
	  break;
	}
      return 1;
    }
  else if (a >>= hsUn)
    {
      cout << "unionDouble2:" << endl;
      cout << " Discriminant value: (enum) " << (CORBA::ULong) hsUn->_d() 
	   << endl;
      cout << "Union Value: ";
      switch(hsUn->_d())
	{
	case A:
	  {
	    cout << "Object Reference (anyUnionTest).";
	    break;
	  }

	case C:
	  cout << " double: ";
	  cout << hsUn->d() << endl;
	  break;
	  
	default:
	  cout << " default (ulong) : " << hsUn->ul() << endl;
	  break;
	}
     
      return 1;
    }

  else return 0;
}


static void viewAny(const CORBA::Any& a)
{
  if (!anyParse::parse(a))
    {
      if (!viewUnion(a))
	{
	  cerr << "Unknown type contained in any. Test failed." << endl;
	  exit(-1);
	}
    }
}


CORBA::Any* 
anyUnionTest_i::testOp(const CORBA::Any& a, CORBA::Any& b, CORBA::Any_out c)
{

  cout << "\nTest number " << pass++ << endl;;
  cout << "** Incoming Anys:" << endl;

  cout << "a: " << endl;
  viewAny(a); 

  cout << "b: ";
  viewAny(b); 
  	
  c = new CORBA::Any;
  CORBA::Any* rc = new CORBA::Any;

  CORBA::ULong l = 2345;
  CORBA::Char ch = 'a';

  *c <<= l;
  *rc <<= CORBA::Any::from_char(ch);

  cout << "\n** Returning:" << endl;
  
  // bTC returned unmodified

  cout << "c: ";   viewAny(*c);
  cout << "Return value: "; viewAny(*rc);
  
  cout << "------------------------" << endl;
  return rc;
}

