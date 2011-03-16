/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.  
2550 Garcia Avenue 
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

// utl_identifier - Implementation of identifiers

#include	<idl.hh>
#include	<idl_extern.hh>

#if defined(__VMS) && __VMS_VER < 70000000
static int  strcasecmp(const char * s1, const char * s2) {

    while (*s1 && *s2) {
        char c1 = isalpha(*s1) ? tolower(*s1) : *s1;
        char c2 = isalpha(*s2) ? tolower(*s2) : *s2;
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        ++s1;
        ++s2;
    }
    if (*s1==0 && *s2 == 0) return 0;
    if (*s1 == 0) return -1;
    return 1;

}

#endif
/*
 * Constructors
 */

Identifier::Identifier()
	  : pv_string(NULL)
{
}

Identifier::Identifier(char *s, long, long, long)
	  : pv_string(s)
{
}

Identifier::~Identifier() {}

// Operations

char *
Identifier::get_string()
{
  return pv_string;
}

// Compare two Identifier *
long
Identifier::compare(Identifier *o,idl_bool ignore_case)
{
  if (o == NULL) return I_FALSE;
  if (pv_string == NULL || o->get_string() == NULL)
    return I_FALSE;
  if (!ignore_case) {
    return (strcmp(pv_string, o->get_string()) == 0) ? I_TRUE : I_FALSE;
  }
  else {
#ifndef __WIN32__
    // Use BSD 4.3 function strcasecmp. 
    return (strcasecmp(pv_string,o->get_string()) == 0) ? I_TRUE : I_FALSE;
#else
   // Use WIN32 case insensitive string compare _stricmp
    return (_stricmp(pv_string,o->get_string()) == 0) ? I_TRUE : I_FALSE;
#endif
  }
}

// Dumping

void
Identifier::dump(std::ostream &o)
{
  if (pv_string == NULL) return;

  o << get_string();
}