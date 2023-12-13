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
DdNode* Cudd_zddModSum (DdManager* manager, DdNode* a, DdNode* b);
DdNode* Cudd_DC(DdManager* dd, DdNode* a);
void print_dd(DdManager *gbm, DdNode *dd, int n, int pr); 

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
    DdNode *z = Cudd_bddNewVar(dd);
    DdNode *y = Cudd_bddNewVar(dd);
    DdNode *x = Cudd_bddNewVar(dd);
    DdNode *d = Cudd_bddNewVar(dd);
    DdNode *c = Cudd_bddNewVar(dd);
    DdNode *b = Cudd_bddNewVar(dd);
    DdNode *a = Cudd_bddNewVar(dd); // */

    char * names[7] = { "z", "y", "x", "d", "c", "b", "a" };

    DdNode *z_zdd = Cudd_DC(dd, z); Cudd_Ref(z_zdd);
    DdNode *y_zdd = Cudd_DC(dd, y); Cudd_Ref(y_zdd);
    DdNode *x_zdd = Cudd_DC(dd, x); Cudd_Ref(x_zdd);
    DdNode *d_zdd = Cudd_DC(dd, d); Cudd_Ref(d_zdd);
    DdNode *c_zdd = Cudd_DC(dd, c); Cudd_Ref(c_zdd);
    DdNode *b_zdd = Cudd_DC(dd, b); Cudd_Ref(b_zdd);
    DdNode *a_zdd = Cudd_DC(dd, a); Cudd_Ref(a_zdd);

    // create f1 = z + yd + y + d
    DdNode *yd_zdd = Cudd_zddProduct(dd, y_zdd, d_zdd); Cudd_Ref(yd_zdd);
    DdNode *f1 = Cudd_zddModSum(dd, z_zdd, yd_zdd);
    f1 = Cudd_zddModSum(dd, f1, y_zdd);
    f1 = Cudd_zddModSum(dd, f1, d_zdd); Cudd_Ref(f1);

    ZDD_dotfile(dd, f1, "f1_zdd", names);

    // create f2 = y + xc + x + c
    DdNode *xc_zdd = Cudd_zddProduct(dd, x_zdd, c_zdd); Cudd_Ref(xc_zdd);
    DdNode *f2 = Cudd_zddModSum(dd, y_zdd, xc_zdd);
    f2 = Cudd_zddModSum(dd, f2, x_zdd);
    f2 = Cudd_zddModSum(dd, f2, c_zdd); Cudd_Ref(f2);

    ZDD_dotfile(dd, f2, "f2_zdd", names);

    // create f3 = x + ba + b + a
    DdNode *ba_zdd = Cudd_zddProduct(dd, b_zdd, a_zdd); Cudd_Ref(ba_zdd);
    DdNode *f3 = Cudd_zddModSum(dd, x_zdd, ba_zdd);
    f3 = Cudd_zddModSum(dd, f3, b_zdd);
    f3 = Cudd_zddModSum(dd, f3, a_zdd); Cudd_Ref(f3);

    ZDD_dotfile(dd, f3, "f3_zdd", names);

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


DdNode* Cudd_zddModSum (DdManager* dd, DdNode* a, DdNode* b)
{
    DdNode *uni = Cudd_zddUnion(dd, a, b); Cudd_Ref(uni);
    DdNode *isect = Cudd_zddIntersect(dd, a, b); Cudd_Ref(isect);
    DdNode *diff = Cudd_zddDiff(dd, uni, isect); Cudd_Ref(diff);
    Cudd_RecursiveDerefZdd(dd, uni);
    Cudd_RecursiveDerefZdd(dd, isect);
    return diff;
}


DdNode* Cudd_DC(DdManager* dd, DdNode* a)
{
    DdNode *a_dc = a; Cudd_Ref(a_dc);
    DdNode *x; 
    int size = Cudd_ReadSize(dd);
    for (int i=0; i < size; i++)
    {
      if (i == a->index) continue;
      x = Cudd_bddIthVar(dd, i); Cudd_Ref(x);
      a_dc = Cudd_bddAnd(dd, a_dc, Cudd_Not(x));
      Cudd_Deref(x);
    }
    
    a_dc = Cudd_zddPortFromBdd(dd, a_dc); Cudd_Ref(a_dc);
    return a_dc;
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


/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
