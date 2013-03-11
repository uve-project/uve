%{
/************** VHDL scanner in LEX format **********
 *
 * Version 0.2 Wed Aug 11, 1993
 *
 * This scanner is derived from a scanner of the ALLIANCE CAD toolset,
 * release 1.1. That toolset was written from:
 *   MASI/CAO-VLSI CAD Team
 *   Laboratoire MASI/CAO-VLSI
 *   Tour 55-65, 2eme etage, Porte 13
 *   Universite Pierre et Marie Curie (PARIS VI)
 *   4, place Jussieu 75252 PARIS Cedex 05, FRANCE
 * The ALLIANCE CAD Toolset can be obtained from ftp site : ftp-masi.ibp.fr    
 *
 * This scanner is avail at: ftp.cs.utwente.nl in pub/src/VHDL/Grammar
 * A corresponding Yacc grammar is available at the same site
 *
 * author of this derived scanner version:
 * Thomas Dettmer
 * Dortmund University
 * Dept. of Computer Scienc, LS1
 * PB 500 500
 * D-44221 Dortmund (Germany)
 * Phone: +49-231-755-6464
 * e-mail: dettmer@ls1.informatik.uni-dortmund.de
 *
 * Please report discovered bugs, critics, suggestions
 * and changes you make to dettmer@ls1.informatik.uni-dortmund.de
 * a list of those that reported repaired (hopefully) errors can be found
 * at the end
 *
 ****************************************************************
 * 
 * This file is intended not to be used for commercial purposes
 * without permission of the University of Dortmund
 *
 * NOTE THAT THERE IS NO WARRANTY FOR CORRECTNES, COMPLETENESS, SUPPORT
 * OR ANYTHING ELSE.
 *******************************************************/
 
 /*
	modifications : Romain Cornaz
					Etudiant HEIG-VD
					Departement TIC
	Date 		  :	28.07.2011
	
	Le parser recupere tous les champs
	necessaires a la reconstruction
	des composants definit par le projet
*/
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <search.h>
#include "y.tab.h"

void lex_restart()
{
	YY_FLUSH_BUFFER;
}

extern char *myinputptr;
extern int nbchar;

extern int linenumber;
extern char *curString;
extern int curInt;

#define min(x,y) ((x)<(y)?(x):(y))

int yta_yyinput(char *buf,int maxsize) {
	int n=min(maxsize,nbchar);
	if (n>0){
		memcpy(buf,myinputptr,n);
		myinputptr += n;
		nbchar-=n;
	}
	return n;
}

#define YY_INPUT(buf,result,max_size) (result= yta_yyinput(buf,max_size))



#define YYLMAX 10000
char yytchar;
char *yysptr;
char *yysbuf;

#ifdef BASE_REPRESENTATION
#ifdef ECHO
#undef ECHO
#endif
#define ECHO echo(yytext)
static void echo(s)
char* s;
{
  char  loc[YYLMAX];
  int   l;
  l=strlen(s);
  loc[l]='\0';
  while(l--) loc[l]=toupper(*(s+l));
  fprintf(yyout, "%s\n", loc);
}
#endif

// I do not want ECHO on the std output, so I have to define it.
// Else it is automatically defined by flex
#ifndef ECHO

  // Well, ok, we do not really want any echo
#define ECHO
//#define ECHO echo(yytext)
//  static void echo(char *s)
//{
//}
#endif // ECHO

int yycolumno=0;
#undef input
int input(void){
  if (yysptr>yysbuf){
   // yytchar=U(*--yysptr);
  }else{
    yytchar=getc(yyin);
  }
  if (yytchar=='\t')
    yycolumno+=8;
  else
    ++yycolumno;
  if (yytchar=='\n'){
    yylineno++;
  }
  if (yytchar==EOF){
  //EOF {YY_NEW_FILE; yyterminate();}
  
    return 0;
  }
  return yytchar;
}
#undef unput
void unput(char c)
{
  yytchar=c;
  *yysptr++=yytchar;
  if(yytchar=='\n')
    yylineno--;
  if (c=='\t')
    yycolumno-=8;
  else
    --yycolumno;
}


#define MVL_LG_MC 15
#define MVL_NB_MC 81

int MVL_LINNUM=1;

typedef struct {
        char nom[MVL_LG_MC];
        int kval;
        } el_mc;

static el_mc tab_mc []=
  {
    {"abs"		,t_ABS},
    {"access"		,t_ACCESS},
    {"after"		,t_AFTER},
    {"alias"		,t_ALIAS},
    {"all"		,t_ALL},
    {"and"		,t_AND},
    {"architecture"	,t_ARCHITECTURE},
    {"array"		,t_ARRAY},
    {"assert"		,t_ASSERT},
    {"attribute"	,t_ATTRIBUTE},

    {"begin"		,t_BEGIN},
    {"block"		,t_BLOCK},
    {"body"		,t_BODY},
    {"buffer"		,t_BUFFER},
    {"bus"		,t_BUS},

    {"case"		,t_CASE},
    {"component"	,t_COMPONENT},




    {"configuration"	,t_CONFIGURATION},
    {"constant"		,t_CONSTANT},

    {"disconnect"	,t_DISCONNECT},
    {"downto"		,t_DOWNTO},

    {"else"		,t_ELSE},
    {"elsif"		,t_ELSIF},
    {"end"		,t_END},
    {"entity"		,t_ENTITY},
    {"exit"		,t_EXIT},

    {"file"		,t_FILE},
    {"for"		,t_FOR},
    {"function"		,t_FUNCTION},

    {"generate"		,t_GENERATE},
    {"generic"		,t_GENERIC},
    {"guarded"		,t_GUARDED},

    {"if"		,t_IF},
    {"in"		,t_IN},
    {"inout"		,t_INOUT},
    {"is"		,t_IS},

    {"label"		,t_LABEL},
    {"library"		,t_LIBRARY},
    {"linkage"		,t_LINKAGE},
    {"loop"		,t_LOOP},

    {"map"		,t_MAP},
    {"mod"		,t_MOD},

    {"nand"		,t_NAND},
    {"new"		,t_NEW},
    {"next"		,t_NEXT},
    {"nor"		,t_NOR},
    {"not"		,t_NOT},
    {"null"		,t_NULL},

    {"of"		,t_OF},
    {"on"		,t_ON},
    {"open"		,t_OPEN},
    {"or"		,t_OR},
    {"others"		,t_OTHERS},
    {"out"		,t_OUT},

    {"package"		,t_PACKAGE},
    {"port"		,t_PORT},
    {"procedure"	,t_PROCEDURE},
    {"process"		,t_PROCESS},

    {"range"		,t_RANGE},
    {"record"		,t_RECORD},
    {"register"		,t_REGISTER},
    {"rem"		,t_REM},
    {"report"		,t_REPORT},
    {"return"		,t_RETURN},

    {"select"		,t_SELECT},
    {"severity"		,t_SEVERITY},
    {"signal"		,t_SIGNAL},
    {"subtype"		,t_SUBTYPE},

    {"then"		,t_THEN},
    {"to"		,t_TO},
    {"transport"	,t_TRANSPORT},
    {"type"		,t_TYPE},

    {"units"		,t_UNITS},
    {"until"		,t_UNTIL},
    {"use"		,t_USE},

    {"variable"		,t_VARIABLE},

    {"wait"		,t_WAIT},
    {"when"		,t_WHEN},
    {"while"		,t_WHILE},
    {"with"		,t_WITH},

    {"xor"		,t_XOR}
  };

int mycmp(const void *s1,const void *s2)
{
    return strcmp((char *)s1,(char *)s2);
}

static int find_mc(char *s)
{
  char  loc[YYLMAX];
  int   l;
  el_mc *pt;

  l=strlen(s);
  strcpy(loc,s);
  while(l--) loc[l]=tolower(loc[l]);	/* conversion en minuscules */
  pt= (el_mc *) bsearch(loc, (char *)tab_mc,MVL_NB_MC,sizeof(el_mc),mycmp);
  if (pt==NULL) return(-1);
  else return(pt->kval);
}

%}

/* Suppresses the warning about yyunput declared but not used */
%option nounput


upper_case_letter 	  [A-Z]
digit 			  [0-9]
special_character   	  [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character 	  [ \t]
format_effector		  [\t\v\r\l\f]
end_of_line		  \n 
lower_case_letter 	  [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character	  ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character	  ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  {digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
based_integer	   	  {extended_digit}(_?{extended_digit})*
extended_digit	   	  ({digit}|[a-fA-F])
base_specifier	  	  (B|b|O|o|X|x)




%%
{space_character}	{
			/* nothing */
#ifndef BASE_REPRESENTATION			
			ECHO; /*return(t_Space);*/
#endif
			}
\&									{
			ECHO; return(t_Ampersand);
									}
\'									{
			ECHO; return(t_Apostrophe);
									}
\(									{
			ECHO; return(t_LeftParen);
									}
\)									{
			ECHO; return(t_RightParen);
									}
"**"									{
			ECHO; return(t_DoubleStar);
									}
\*									{
			ECHO; return(t_Star);
									}
\+									{
			ECHO; return(t_Plus);
									}
\,									{
			ECHO; return(t_Comma);
									}
\-									{
			ECHO; return(t_Minus);
									}
":="									{
			ECHO; return(t_VarAsgn);
									}
\:									{
			ECHO; return(t_Colon);
									}
\;									{
			ECHO; return(t_Semicolon);
									}
"<="	{
	    ECHO;
	    return(t_LESym);
	}
">="	{
            ECHO;
	    return(t_GESym);
	}
\<	{
            ECHO;
	    return(t_LTSym);
	}
\>	{
            ECHO;
	    return(t_GTSym);
	}
=									{
			ECHO; return(t_EQSym);
									}
\/=									{
			ECHO; return(t_NESym);
									}
"=>"	{
            ECHO;
	    return(t_Arrow);
	}
"<>"	{
            ECHO;
	    return(t_Box);
	}
\|									{
			ECHO; return(t_Bar);
									}
!									{
			ECHO; return(t_Bar);
									}
\.									{
			ECHO; return(t_Dot);
									}
\/									{
			ECHO; return(t_Slash);
									}
{letter}(_?{letter_or_digit})* 	{
	int itoken;
	itoken=find_mc(yytext);
	if (itoken== -1) 
	{
	  ECHO;
	  yylval.ds_Id.pos=yycolumno;
	  yylval.ds_Id.len=strlen(yytext);
	  //recuperation des identifiants 
	  curString=strdup(yytext);
	  yylval.ds_Id.line=yylineno;
	  /* yylval.ds_Id.name= insertName(yytext); */
	  return ( t_Identifier );
			  }
			else
			  {
			  ECHO; return ( itoken );
			  }
									}
({decimal_literal})|({base}#{based_integer}(\.{based_integer})?#({exponent})?)|({base}:{based_integer}(\.{based_integer})?:({exponent})?)		{
                        
                        //recuperation des valeurs pour un type std_logic_vector
                        curInt=atoi(yytext);ECHO; return ( t_AbstractLit );
			}

'({graphic_character}|\"|\%)' 	{
			ECHO; return ( t_CharacterLit );
				}

(\"({graphic_character}|(\"\")|\%)*\")|(\%({graphic_character}|(\%\%)|\")*\%) {
			ECHO; return ( t_StringLit );
			}

{base_specifier}(\"{extended_digit}(_?{extended_digit})*\"|\%{extended_digit}(_?{extended_digit})*\%) 							{
			ECHO; return ( t_BitStringLit );
			}

\n									{
			/* end of line */
			linenumber++;
			MVL_LINNUM++;
			/* tobuf( "\n%4d\t", MVL_LINNUM);*/
#ifndef BASE_REPRESENTATION
			ECHO;
#endif
			yycolumno=0;
			/*return(t_NEWLINE);*/
									}
\-\-.*$ 								{
                    
			/* comment */
#ifndef BASE_REPRESENTATION
			ECHO;
#endif
			/*return(t_COMMENT);*/
									}
.									{
			ECHO; /*return (t_UNKNOWN);*/


									}
%%
/************************************************************
* J.Gaisler reported:
* 	MVL_NB_MC should be set to 81 - done
* The original of this stuff was an illegal and wrong pointer
*************************************************************/
