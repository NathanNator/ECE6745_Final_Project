/**CFile***********************************************************************

  FileName    [main.c]

  PackageName [ntr]

  Synopsis    [Main program for the nanotrav program.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi/Priyank Kalla]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  Neither the name of the University of Colorado nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.]

******************************************************************************/

#include "ntr.h"
#include "cuddInt.h"
//#include "/home/u1170693/ECE_6745/HW2/cudd-3.0.0/cudd/cuddGenCof.c"
#include "../cudd/cuddGenCof.c"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define NTR_VERSION\
    "Nanotrav Version #0.12, Release date 2003/12/31"

#define BUFLENGTH 8192

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

#ifndef lint
static char rcsid[] UTIL_UNUSED = "$Id: main.c,v 1.38 2004/08/13 18:28:28 fabio Exp fabio $";
#endif

static  char    buffer[BUFLENGTH];
#ifdef DD_DEBUG
extern  st_table *checkMinterms (BnetNetwork *net, DdManager *dd, st_table *previous);
#endif

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/
/*-- Priyank: We don't need any of these--*/
/***************
static NtrOptions * mainInit ();
static void ntrReadOptions (int argc, char **argv, NtrOptions *option);
static void ntrReadOptionsFile (char *name, char ***argv, int *argc);
static char* readLine (FILE *fp);
static FILE * open_file (char *filename, const char *mode);
static int reorder (BnetNetwork *net, DdManager *dd, NtrOptions *option);
static void freeOption (NtrOptions *option);
static DdManager * startCudd (NtrOptions *option, int nvars);
static int ntrReadTree (DdManager *dd, char *treefile, int nvars);
******************/

/**AutomaticEnd***************************************************************/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname);
void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname);

/**Function********************************************************************

  Synopsis    [Main program for ntr.]

  Description [Main program for ntr. Performs
  initialization. Introduces variables, builds ROBDDs for some
  functions, and prints-out the BDD structure. A sample file given to
  the 5740 class.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
int
main(
  int  argc,
  char ** argv)
{
    NtrOptions	*option;	/* options */
    DdManager	*dd;		/* pointer to DD manager */
    int		exitval;	/* return value of Cudd_CheckZeroRef */
    int		ok;		/* overall return value from main() */
    int		result;		/* stores the return value of functions */
    int		i;		/* loop index */
    int		j;		/* loop index */
    double	*signatures;	/* array of signatures */
    int		pr;		/* verbosity level */
    int		reencoded;	/* linear transformations attempted */


    /****** Priyank's additions *******/
    DdNode *one, *zero;
    DdNode *a, *b, *c, *d, *e, *g;
    DdNode *x, *y, *z, *w, *v, *h;    
    /*********************************/


    /* Initialize manager. We start with 0 variables */

    dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    /*startCudd(option,net1->ninputs);*/
    if (dd == NULL) { exit(2); }


    /************ lets do our work here *************/
    one = Cudd_ReadOne( dd );
    Cudd_Ref(one); /* referenced for the first time */

    zero = Cudd_Not( one ); /* not the same as Cudd_ReadZero */
    /* Cudd_Ref(zero);*/ 
    /* reference count increment not needed because Cudd_Not returns
       the projection */ 

    /***** Add a new variable ***********/
    /* Primary Inputs */ // /* 
    a = Cudd_bddNewVar(dd);
    b = Cudd_bddNewVar(dd);
    c = Cudd_bddNewVar(dd); // */

    /* e = a*b*c  */ // /* Kalla's example circuit
    d = Cudd_bddAnd(dd, a, b); // a AND b
    Cudd_Ref(d);

    e = Cudd_bddAnd(dd, d, c); // a AND c  
    Cudd_Ref(e);


    /*************** e = a*b*c ***************/ 
    printf("e = a*b*c\n");
    /* print BDD structure of f */
    printf("\nPrinting the BDD graph for f: ptr to the nodes, T & E children\n");
    Cudd_PrintDebug(dd, e, 3, 3);
    printf("\n");
    printf("Printing the minterms of f:\nabc  e\n");
    Cudd_PrintMinterm(dd, e); 

    /* 
    Generate dot files and run them 
    dot -Txlib "abc_bdd.dot"
    dot -Txlib "abc_zdd.dot"
    */

    BDD_dotfile(dd, e, "abc_bdd");

    /* BDD to ZDD */
    g = Cudd_zddPortFromBdd(dd, e);
    Cudd_Ref(g);

    ZDD_dotfile(dd, g, "abc_zdd");

    exit(0); 

} /* end of main */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname)
{
    char filename[64];
    sprintf(filename, "%s.dot", fname);
    //const char* filename = ".dot", fname;
    FILE* outfile = fopen(filename, "w");
    Cudd_DumpDot(manager, 1, &node, NULL, NULL, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}

void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname)
{
    char filename[64];
    sprintf(filename, "%s.dot", fname);
    //const char* filename = ".dot", fname;
    FILE* outfile = fopen(filename, "w");
    Cudd_zddDumpDot(manager, 1, &node, NULL, NULL, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}



/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
