%option never-interactive

%{
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

/*
 * idl.ll - Lexical scanner for IDL 1.1
 */

#include <idl.hh>
#include <idl_extern.hh>

#include <fe_private.hh>

#if defined(__VMS)
#include "y_tab.h"
#else
#include "y.tab.h"
#endif

#include <string.h>

static unsigned char	idl_escape_reader(char *);
static double	idl_atof(char *);
static long	idl_atoi(char *, long);
static void	idl_parse_line_and_file(char *);
static void	idl_parse_line_and_file_NT(char *);
static void	idl_store_pragma(char *);

%}

%%

module		return MODULE;
raises		return RAISES;
readonly	return READONLY;
attribute	return ATTRIBUTE;
exception	return EXCEPTION;
context		return CONTEXT;
interface	return INTERFACE;
const		return CONST;
typedef		return TYPEDEF;
struct		return STRUCT;
enum		return ENUM;
string		return STRING;
wstring 	return WSTRING;
sequence	return SEQUENCE;
union		return UNION;
switch		return SWITCH;
case		return CASE;
default		return DEFAULT;
float		return FLOAT;
double		return DOUBLE;
long		return LONG;
short		return SHORT;
unsigned	return UNSIGNED;
char		return CHAR;
wchar		return WCHAR;
boolean		return BOOLEAN;
octet		return OCTET;
void		return VOID;

TRUE		return TRUETOK;
FALSE		return FALSETOK;

inout		return INOUT;
in		return IN;
out		return OUT;
oneway		return ONEWAY;

\<\<		return LEFT_SHIFT;
\>\>		return RIGHT_SHIFT;
\:\:		{
		  yylval.strval = "::";    
		  return SCOPE_DELIMITOR;
		}

[a-zA-Z][a-zA-Z0-9_]*	{
    char *z = (char *) malloc(strlen(yytext) + 1);
    strcpy(z, yytext);
    yylval.strval = z;
    return IDENTIFIER;
}

[0-9]*"."[0-9]*([eE][+-]?[0-9]+)?[lLfF]?      {
                  yylval.dval = idl_atof(yytext);
                  return FLOATING_PT_LITERAL;
                }
[0-9]+[eE][+-]?[0-9]+[lLfF]?  {
                  yylval.dval = idl_atof(yytext);
                  return FLOATING_PT_LITERAL;
                }

[1-9][0-9]*	{
		  yylval.ival = idl_atoi(yytext, 10);
		  return INTEGER_LITERAL;
	        }
0[xX][a-fA-F0-9]+ {
		  yylval.ival = idl_atoi(yytext, 16);
		  return INTEGER_LITERAL;
	        }
0[0-7]*	{
		  yylval.ival = idl_atoi(yytext, 8);
		  return INTEGER_LITERAL;
	      	}
"\""([^\"\\]|\\.)*"\""	{
		  yytext[strlen(yytext)-1] = '\0';
		  yylval.sval = new String(yytext + 1);
		  return STRING_LITERAL;
	      	}

"'"."'"		{
		  yylval.cval = yytext[1];
		  return CHARACTER_LITERAL;
	      	}
"'"\\([0-7]{1,3})"'"	{
		  // octal character constant
		  yylval.cval = idl_escape_reader(yytext + 1);
		  return CHARACTER_LITERAL;
		}
"'"\\([xX][0-9a-fA-F]{1,2})"'"	{
		  // hex character constant
		  yylval.cval = idl_escape_reader(yytext + 1);
		  return CHARACTER_LITERAL;
		}
"'"\\."'"	{
		  yylval.cval = idl_escape_reader(yytext + 1);
		  return CHARACTER_LITERAL;
		}

^[ \t]*#[ \t0-9\$]*pragma[ \t]+version/[ \t]+[a-zA-Z\:][a-zA-Z0-9_\:]*[ \t]+[0-9]+\.[0-9]+[ \t]*\n {
                   return PRAGMA_VERSION;
                 }               

^[ \t]*#[ \t0-9\$]*pragma[ \t]+ID/[ \t]+[a-zA-Z\:][a-zA-Z0-9_\:]*[ \t]+"\""[^\"]*"\""[ \t]*\n {
                    return PRAGMA_ID;
		 }

^[ \t]*#[ \t0-9\$]*pragma[ \t]+prefix/[ \t]+"\""[^\"]*"\""[ \t]*\n {
                     return PRAGMA_PREFIX;
                 }

^[ \t]*#[ \t0-9\$]*pragma[ \t].*\n	{/* remember pragma */
		  idl_store_pragma(yytext);
		}
^#[ \t]*[0-9]*" ""\""[^\"]*"\"".*\n			{
		  idl_parse_line_and_file(yytext);
		}
^#[ \t]*[0-9]*\n	{
		  idl_parse_line_and_file(yytext);
	        }
^#[ \t]*line[ \t]*[0-9]*" ""\""[^\"]*"\"".*\n			{
		  idl_parse_line_and_file_NT(yytext);
		}
^#[ \t]*line[ \t]*[0-9]*\n	{
		  idl_parse_line_and_file_NT(yytext);
	        }
^#[ \t]*ident.*\n	{
		  /* ignore cpp ident */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
^#[ \t]*file.*\n	{
		/* ignore "#file" provided by SCO UDK 7.x.x preprocessor */
		idl_global->set_lineno(idl_global->lineno() + 1);
		}

\/\/.*\n	{
		  /* ignore comments */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
"/*"		{
		  for(;;) {
		    char c = yyinput();
		    if (c == '*') {
		      char next = yyinput();
		      if (next == '/')
			break;
		      else
			/* yyunput(c); */
			unput(c);
	              if (c == '\n') 
		        idl_global->set_lineno(idl_global->lineno() + 1);
		    }
	          }
	        }
[ \t]*		;
\n		{
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
.		return yytext[0];

%%
	/* subroutines */

/*
 * Strip down a name to the last component, i.e. everything after the last
 * '/' character
 */
static char *
stripped_name(UTL_String *fn)
{
    char	*n = fn->get_string();
    long	l;

    if (n == NULL)
	return NULL;
    l = strlen(n);
#if defined(__WIN32__)
    for (n += l; l > 0 && *n != 92; l--, n--);
    if (*n == 92) n++;
#elif defined(__VMS)

    for (n += l; l > 0 && *n != ';'; l--, n--);
    if (*n == ';') {
        static UTL_String temp;
        temp = UTL_String(fn);
        n = temp.get_string();
        n[l] = 0;
    }

    for (n += l; l > 0 && *n != ']' && *n != ':'; l--, n--);
    if (*n == ']' || *n==':') n++;

#else
    for (n += l; l > 0 && *n != '/'; l--, n--);
    if (*n == '/') n++;
#endif
	
    return n;
}

/*
 * Parse a #line statement generated by the C preprocessor
 */
static void
idl_parse_line_and_file(char *buf)
{
  char		*r = buf;
  char 		*h;
  UTL_String	*nm;

  /* Skip initial '#' */
  if (*r != '#') {
    return;
  }

  /* Find line number */
  for (r++; *r == ' ' || *r == '\t'; r++);
  h = r;
  for (; *r != '\0' && *r != ' ' && *r != '\t'; r++);
  if (*r != '\0') {
     *r++ = 0;
  }
  idl_global->set_lineno(idl_atoi(h, 10));

  if (*r == '\0') {
    return;
  }
  
  /* Find file name, if present */
  for (; *r != '"'; r++) {
    if (*r == '\n' || *r == '\0')
      return;
  }
  h = ++r;
  for (; *r != '"'; r++);
  *r = 0;
  if (*h == '\0')
    idl_global->set_filename(new String("standard input"));
  else {
#ifdef __VMS
    // kill version
    char* v(r);
    for(--v; v != h && *v != ';'; --v);
    if (*v==';') *v = 0;
    else v = r;
    // kill device/directory:
    for(r=v; r != h && *r != ']' && *r != ':'; --r) {
        if(isalpha(*r)) *r = tolower(*r);
    }
    if (*r==']' || *r==':') h = r+1;
#endif
    idl_global->set_filename(new String(h));
  }

  idl_global->set_in_main_file(
    (idl_global->filename()->compare(idl_global->real_filename())) ?
    I_TRUE :
    I_FALSE
  );
  /*
   * If it's an import file store the stripped name for the BE to use
   */
  if (!(idl_global->in_main_file()) && idl_global->import()) {
    nm = new UTL_String(stripped_name(idl_global->filename()));
    idl_global->store_include_file_name(nm);
  }
}


/*
 * Parse a #line statement generated by the C preprocessor
 * - NT version.
 */
static void
idl_parse_line_and_file_NT(char *buf)
{
  char		*r = buf;
  char 		*h;
  char 		*j;
  int count=0,jcount=0;
  UTL_String	*nm;

  /* Skip initial '#' */
  if (*r != '#') {
    return;
  }

  /* Find line number */

  /* Skip whitespace: */
  for (r++; *r == ' ' || *r == '\t'; r++);

  /* Skip "line": */
  r+=4;
	
  /* Skip whitespace: */
  for (r++; *r == ' ' || *r == '\t'; r++);

  h = r;
  for (; *r != '\0' && *r != ' ' && *r != '\t'; r++);
  if (*r != '\0') {
    *r++ = 0;
  }
  idl_global->set_lineno(idl_atoi(h, 10));

  if (*r == '\0') {
     return;
  }
  
  
  /* Find file name, if present */
  for (; *r != '"'; r++) {
    if (*r == '\n' || *r == '\0')
      return;
  }
  h = ++r;
  for (; *r != '"'; r++);
  *r = 0;

  if (*h == '\0')
    idl_global->set_filename(new String("standard input"));
  else
    {
#ifdef __VMS
      // kill version
      char* v(r);
      for(--v; v != h && *v != ';'; --v);
      if (*v==';') *v = 0;
      else v = r;
      // kill device/directory:
      for(r=v; r != h && *r != ']' && *r != ':'; --r) {
	if(isalpha(*r)) *r = tolower(*r);
      }
      if (*r==']' || *r==':') h = r+1;
      idl_global->set_filename(new String(h));
#else
      /* Get around the NT include problem: */
      
      j = new char[strlen(h)+1];
      
      int nflag = 0;
      for (count = 0; count < strlen(h); count++)
	{
	  if (h[count] == 92 && h[count+1] == 92 && nflag == 0) 
	    {
	      nflag = 1;
	      continue;
	    }
	  nflag = 0;
	  j[jcount] = h[count];
	  jcount++;
	}
      j[jcount] = 0;	
      idl_global->set_filename(new String(j));
      delete[] j;
#endif	
    }

  idl_global->set_in_main_file(
    (idl_global->filename()->compare(idl_global->real_filename())) ?
    I_TRUE :
    I_FALSE
  );
  /*
   * If it's an import file store the stripped name for the BE to use
   */
  if (!(idl_global->in_main_file()) && idl_global->import()) {
    nm = new UTL_String(stripped_name(idl_global->filename()));
    idl_global->store_include_file_name(nm);
  }
}

    
/*
 * Store a #pragma line into the list of pragmas
 */
static void
idl_store_pragma(char *buf)
{
  char *cp = buf + 1;
  while(*cp != 'p')
    cp++;
  while(*cp != ' ' && *cp != '\t')
    cp++;
  while(*cp == ' ' || *cp == '\t')
    cp++;

  char *pp = cp;
  while (*pp != '\n') pp++;
  char* pragma = new char[pp-cp+1];
  pp = pragma;
  while(*cp != '\n') {
    *pp++ = *cp++;
  }
  *pp = 0;
  pp = strchr(pragma,' ');
  if (!pp) pp = strchr(pragma,'\t');
  if (strncmp(pragma,"version",7) == 0) {
    if (!pp || pp == pragma + 7) {
      idl_global->set_parse_state(IDL_GlobalData::PS_PragmaVersionSeen);
      idl_global->err()->syntax_error(idl_global->parse_state());
      idl_global->set_lineno(idl_global->lineno() + 1);
      return;
    }
  }
  else if (strncmp(pragma,"ID",2) == 0) {
    if (!pp || pp == pragma + 2) {
      idl_global->set_parse_state(IDL_GlobalData::PS_PragmaIDSeen);
      idl_global->err()->syntax_error(idl_global->parse_state());
      idl_global->set_lineno(idl_global->lineno() + 1);
      return;
    }
  }
  else if (strncmp(pragma,"prefix",6) == 0) {
    if (!pp || pp == pragma + 6) {
      idl_global->set_parse_state(IDL_GlobalData::PS_PragmaPrefixSeen);
      idl_global->err()->syntax_error(idl_global->parse_state());
      idl_global->set_lineno(idl_global->lineno() + 1);
      return;
    }
  }
  else if (strcmp(pragma, "import") == 0) {
    idl_global->set_import(I_TRUE);
    return;
  } 
  else if (strcmp(pragma, "include") == 0) {
    idl_global->set_import(I_FALSE);
    return;
  }

  UTL_StrList *p = idl_global->pragmas();
  if (p == NULL)
    idl_global->set_pragmas(new UTL_StrList(new String(""),
	 	                            new UTL_StrList(new String(buf), 
                                                           NULL
                                                           )
                                            )
                            );
  else {
    p->nconc(new UTL_StrList(new String(""),
                             new UTL_StrList(new String(buf), 
                                             NULL
                                            )
	                    )
             );
    idl_global->set_pragmas(p);
  }
  idl_global->set_lineno(idl_global->lineno() + 1);
}

/*
 * idl_atoi - Convert a string of digits into an integer according to base b
 */
static long
idl_atoi(char *s, long b)
{
	long	r = 0;
	long	negative = 0;

	if (*s == '-') {
	  negative = 1;
	  s++;
	}
	if (b == 8 && *s == '0')
	  s++;
	else if (b == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
	  s += 2;

	for (; *s; s++)
	  if (*s <= '9' && *s >= '0')
	    r = (r * b) + (*s - '0');
	  else if (b > 10 && *s <= 'f' && *s >= 'a')
	    r = (r * b) + (*s - 'a' + 10);
	  else if (b > 10 && *s <= 'F' && *s >= 'A')
	    r = (r * b) + (*s - 'A' + 10);
	  else
	    break;

	if (negative)
	  r *= -1;

	return r;
}

/*
 * Convert a string to a float; atof doesn't seem to work, always.
 */
static double
idl_atof(char *s)
{
	char    *h = s;
	double	d = 0.0;
	double	f = 0.0;
	double	e, k;
	long	neg = 0, negexp = 0;

	if (*s == '-') {
	  neg = 1;
	  s++;
	}
	while (*s >= '0' && *s <= '9') {
		d = (d * 10) + *s - '0';
		s++;
	}
	if (*s == '.') {
		s++;
		e = 10;
		while (*s >= '0' && *s <= '9') {
			d += (*s - '0') / (e * 1.0);
			e *= 10;
			s++;
		}
	}
	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '-') {
			negexp = 1;
			s++;
		} else if (*s == '+')
			s++;
		e = 0;
		while (*s >= '0' && *s <= '9') {
			e = (e * 10) + *s - '0';
			s++;
		}
		if (e > 0) {
			for (k = 1; e > 0; k *= 10, e--);
			if (negexp)
				d /= k;
			else
				d *= k;
		}
	}

	if (neg) d *= -1.0;

	return d;
}	

/*
 * Convert (some) escaped characters into their ascii values
 */
static unsigned char
idl_escape_reader(
    char *str
)
{
    if (str[0] != '\\') {
	return str[0];
    }

    switch (str[1]) {
      case 'n':
	return '\n';
      case 't':
	return '\t';
      case 'v':
	return '\v';
      case 'b':
	return '\b';
      case 'r':
	return '\r';
      case 'f':
	return '\f';
      case 'a':
	return '\a';
      case '\\':
	return '\\';
      case '\?':
	return '?';
      case '\'':
	return '\'';
      case '"':
	return '"';
      case 'x':
	{
	    // hex value
            int i;
	    for (i = 2; str[i] != '\0' && isxdigit(str[i]); i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[2], 16);
	    str[i] = save;
	    return out;
	}
	break;
      default:
	// check for octal value
	if (str[1] >= '0' && str[1] <= '7') {
            int i;
	    for (i = 1; str[i] >= '0' && str[i] <= '7'; i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[1], 8);
	    str[i] = save;
	    return out;
	} else {
	  return str[1] - 'a';
	}
	break;
    }
}