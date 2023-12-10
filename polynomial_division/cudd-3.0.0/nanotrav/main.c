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
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names);
void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names);
void print_dd(DdManager *gbm, DdNode *dd, int n, int pr); 
void write_dd (DdManager *gbm, DdNode *dd, char* filename);

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

    /* Initialize manager. We start with 0 variables */

    dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    //Cudd_ReduceHeap(dd, CUDD_REORDER_NONE, 0);
    /*startCudd(option,net1->ninputs);*/
    if (dd == NULL) { exit(2); }


    /************ lets do our work here *************/
    DdNode *one = Cudd_ReadOne(dd);
    Cudd_Ref(one); /* referenced for the first time */

    DdNode *zero = Cudd_Not(one); /* not the same as Cudd_ReadZero */
    /* Cudd_Ref(zero);*/ 
    /* reference count increment not needed because Cudd_Not returns
       the projection */ 

    // ordering = x1 < x2 < x3 < x4
    // ordering = d < c < b < a 
    char * names[4] = { "a", "b", "c", "d"};
    
    DdNode *a = Cudd_bddNewVar(dd);
    DdNode *b = Cudd_bddNewVar(dd);
    DdNode *c = Cudd_bddNewVar(dd);
    DdNode *d = Cudd_bddNewVar(dd);

    // f1 = a.b.!c + c.a!.b! 
    DdNode *f1 = Cudd_bddAnd(dd, a, b);
    Cudd_Ref(f1);

    // a.b.!c
    DdNode *f2 = Cudd_bddAnd(dd, f1, Cudd_Not(c));
    Cudd_Ref(f2);

    // c.!a 
    DdNode *f3 = Cudd_bddAnd(dd, c, Cudd_Not(a));
    Cudd_Ref(f3);

    // c.!a.!b 
    DdNode *f4 = Cudd_bddAnd(dd, f3, Cudd_Not(b));
    Cudd_Ref(f4);

    // a.b.!c + c.!a.!b 
    DdNode *f5 = Cudd_bddOr(dd, f2, f4);
    Cudd_Ref(f5);

    DdNode *f = Cudd_zddPortFromBdd(dd, f5);
    ZDD_dotfile (dd, f, "ab_c_zdd", names);

    // g = c + d

    // g = c.!.d.!a.!b + d.!c.!b.!a
    DdNode *g1 = Cudd_bddAnd(dd, c, Cudd_Not(d));
    Cudd_Ref(g1);

    DdNode *g2 = Cudd_bddAnd(dd, g1, Cudd_Not(a));
    Cudd_Ref(g2);

    DdNode *g3 = Cudd_bddAnd(dd, g2, Cudd_Not(b));
    Cudd_Ref(g3);

    DdNode *g4 = Cudd_bddAnd(dd, d, Cudd_Not(c));
    Cudd_Ref(g4);

    DdNode *g5 = Cudd_bddAnd(dd, g4, Cudd_Not(a));
    Cudd_Ref(g5);

    DdNode *g6 = Cudd_bddAnd(dd, g5, Cudd_Not(b));
    Cudd_Ref(g6);

    DdNode *g7 = Cudd_bddOr(dd, g3, g6);
    Cudd_Ref(g7);

    DdNode *g = Cudd_zddPortFromBdd(dd, g7);
    ZDD_dotfile (dd, g, "c_d_zdd", names);

    // f + g = f ∪ g - f ∩ g 

    DdNode *a1 = Cudd_zddUnion(dd, f, g);
    Cudd_Ref(a1);

    DdNode *a2 = Cudd_zddIntersect(dd, f, g);
    Cudd_Ref(a2);

    // a1 - a2 
    DdNode *fg = Cudd_zddDiff(dd, a1, a2);
    Cudd_Ref(fg);

    ZDD_dotfile (dd, fg, "f_g_zdd", names);

    exit(0); 

} /* end of main */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names)
{
    char filename[64];
    sprintf(filename, "%s.dot", fname);
    //const char* filename = ".dot", fname;
    FILE* outfile = fopen(filename, "w");
    Cudd_DumpDot(manager, 1, &node, (char**)names, NULL, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}

void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names)
{
    char filename[64];
    sprintf(filename, "%s.dot", fname);
    //const char* filename = ".dot", fname;
    FILE* outfile = fopen(filename, "w");
    Cudd_zddDumpDot(manager, 1, &node, (char**)names, NULL, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}


/**
 * Print a dd summary
 * pr = 0 : prints nothing
 * pr = 1 : prints counts of nodes and minterms
 * pr = 2 : prints counts + disjoint sum of product
 * pr = 3 : prints counts + list of nodes
 * pr > 3 : prints counts + disjoint sum of product + list of nodes
 * @param the dd node
 */
void print_dd (DdManager *gbm, DdNode *dd, int n, int pr )
{
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existence*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld \n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);  // Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
}

void write_dd (DdManager *gbm, DdNode *dd, char* filename)
{
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = dd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
