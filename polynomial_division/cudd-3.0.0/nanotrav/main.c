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
    DdNode *f1, *f2, *f3;
    DdNode *f1_zdd;
    DdNode *r0, *r1, *r2, *r3;
    DdNode *r0_zdd;
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
    //z = Cudd_bddNewVar(dd);
    y = Cudd_bddNewVar(dd);
    //x = Cudd_bddNewVar(dd);
    d = Cudd_bddNewVar(dd);
    //c = Cudd_bddNewVar(dd);
    // b = Cudd_bddNewVar(dd);
    // a = Cudd_bddNewVar(dd); // */

    //char * names[7] = { "z", "y", "x", "d", "c", "b", "a" };
    //char * names[3] = { "y", "x", "c" };
    char * names[2] = { "y", "d" };

    // r1_zdd = yd
    r1 = Cudd_bddAnd(dd, y, d);
    Cudd_Ref(r1);
    DdNode *r1_zdd = Cudd_zddPortFromBdd(dd, r1);
    ZDD_dotfile(dd, r1_zdd, "r1_zdd", names);

    // r2_zdd = yd'
    r2 = Cudd_bddAnd(dd, y, Cudd_Not(d));
    Cudd_Ref(r2);
    DdNode *r2_zdd = Cudd_zddPortFromBdd(dd, r2);
    ZDD_dotfile(dd, r2_zdd, "r2_zdd", names);

    // r3_zdd = y'd
    r3 = Cudd_bddAnd(dd, Cudd_Not(y), d);
    Cudd_Ref(r3);
    DdNode *r3_zdd = Cudd_zddPortFromBdd(dd, r3);
    ZDD_dotfile(dd, r3_zdd, "r3_zdd", names);
    
    
    DdNode *r4 = Cudd_zddUnion(dd, r1_zdd, r2_zdd);
    r4 = Cudd_zddUnion(dd, r4, r3_zdd);
    Cudd_Ref(r4);

    ZDD_dotfile(dd, r4, "r4_zdd", names);
    
    
    // //DdNode *r3_zdd = Cudd_zddPortFromBdd(dd, r3);
    // ZDD_dotfile(dd, r3, "r3_zdd", names);

    // DdNode *r4 = Cudd_zddIntersect(dd, r1_zdd, r2_zdd);
    // Cudd_Ref(r4);
    // //DdNode *r4_zdd = Cudd_zddPortFromBdd(dd, r4);
    // ZDD_dotfile(dd, r4, "r4_zdd", names);


    // DdNode *r5 = Cudd_zddDiff(dd, r3, r4);
    // Cudd_Ref(r5);
    // //DdNode *r5_zdd = Cudd_zddPortFromBdd(dd, r5);
    // ZDD_dotfile(dd, r5, "r5_zdd", names);

    exit(0);


    // r2 = Cudd_bddOr(dd, r1, d); // + d
    // Cudd_Ref(r2);

    // r3 = Cudd_bddOr(dd, r2, y); // + y
    // Cudd_Ref(r3);

    // DdNode *r1_zdd = Cudd_zddPortFromBdd(dd, r2);
    // Cudd_Ref(r1_zdd);
    // ZDD_dotfile(dd, r1_zdd, "r1_zdd", names);


    // exit(0);

    // f2 = Cudd_bddAnd(dd, y, Cudd_Not(x));
    // f2 = Cudd_bddAnd(dd, f2, Cudd_Not(c));
    // Cudd_Ref(f2);
    // // f2 = yx'c'

    // DdNode *temp0 = Cudd_bddAnd(dd, x, c);
    // temp0 = Cudd_bddAnd(dd, temp0, Cudd_Not(y));
    // Cudd_Ref(temp0);
    // // temp0 = y'xc

    // DdNode *temp1 = Cudd_bddAnd(dd, x, Cudd_Not(y));
    // temp1 = Cudd_bddAnd(dd, temp1, Cudd_Not(c));
    // Cudd_Ref(temp1);
    // // temp1 = y'xc'

    // DdNode *temp2 = Cudd_bddAnd(dd, c, Cudd_Not(y));
    // temp2 = Cudd_bddAnd(dd, temp2, Cudd_Not(x));
    // Cudd_Ref(temp2);
    // // temp2 = y'x'c

    // DdNode *f2_final = Cudd_bddOr(dd, f2, temp0);
    // f2_final = Cudd_bddOr(dd, f2_final, temp1);
    // f2_final = Cudd_bddOr(dd, f2_final, temp2);
    // Cudd_Ref(f2_final);
    // // f2 = y + xc + x + c


    // ///// DUMP /////


    // DdNode *f2_zdd = Cudd_zddPortFromBdd(dd, f2_final);
    // Cudd_Ref(f2_zdd);
    // ZDD_dotfile(dd, f2_zdd, "f2_zdd", names);
    // //r1 = cuddZddDivide(dd, r0_zdd, f1_zdd);
    // //Cudd_Ref(r1);

    // // display result
    // //printf("printing r0 --f1--> r1\n");
    // //ZDD_dotfile(dd, r0_zdd, "r0_zdd", names);

    // exit(0);


    // DdNode *G = Cudd_bddAnd(dd, d, one);
    // Cudd_Ref(G);

    // DdNode *G_zdd = Cudd_zddPortFromBdd(dd, G);
    // Cudd_Ref(G_zdd);
    // ZDD_dotfile(dd, G_zdd, "G_zdd", NULL);

/*
    DdNode *func = Cudd_bddAnd(dd, d, c);
    func = Cudd_bddAnd(dd, func, b);
    func = Cudd_bddAnd(dd, func, a);
    Cudd_Ref(func);
    
    DdNode *func_zdd = Cudd_zddPortFromBdd(dd, func);
    Cudd_Ref(func_zdd);
    ZDD_dotfile(dd, func_zdd, "func_zdd", NULL);

    DdNode *G = Cudd_bddAnd(dd, d, Cudd_Not(c));
    G = Cudd_bddAnd(dd, G, Cudd_Not(b));
    G = Cudd_bddAnd(dd, G, Cudd_Not(a));
    Cudd_Ref(G);

    Cudd_zddVarsFromBddVars(dd, 0);
    DdNode *G_zdd = Cudd_zddPortFromBdd(dd, G);
    Cudd_Ref(G_zdd);


    BDD_dotfile(dd, G, "G_bdd", NULL);
    ZDD_dotfile(dd, G_zdd, "G_zdd", NULL);
    DdNode *q0 = Cudd_zddDivide(dd, func_zdd, G_zdd);
    ZDD_dotfile(dd, q0, "q0", NULL);

    exit(0);

*/

    // paper example
    // z = a + b + c + d

    // x = a + b
    // x = Cudd_bddOr(dd, a, b); 
    // Cudd_Ref(x);
    // Cudd_RecursiveDeref(dd, a);
    // Cudd_RecursiveDeref(dd, b);

    // BDD_dotfile(dd, x, "x_bdd", names);

    // //exit(0);
  
    // // y = x + c
    // y = Cudd_bddOr(dd, x, c);   
    // Cudd_Ref(y);
    // Cudd_RecursiveDeref(dd, x);
    // Cudd_RecursiveDeref(dd, c);

    // BDD_dotfile(dd, y, "y_bdd", names);

    // // z = y + d
    // z = Cudd_bddOr(dd, y, d);   
    // Cudd_Ref(z);
    // Cudd_RecursiveDeref(dd, y);
    // Cudd_RecursiveDeref(dd, d);

    // ///// print z //////
    // BDD_dotfile(dd, z, "z_bdd", names);

    /* BDD to ZDD */
    // g = Cudd_zddPortFromBdd(dd, z);
    // Cudd_Ref(g);

    // ZDD_dotfile(dd, g, "z_zdd", names);


    ///// create f1, f2, f3 /////

    // f1 = z + yd + y + d
    // f1 = Cudd_bddAnd(dd, y, d); // f1 = yd
    // f1 = Cudd_bddOr(dd, f1, z); // f1 = z + yd
    // f1 = Cudd_bddOr(dd, f1, y); // f1 = z + yd + y
    // f1 = Cudd_bddOr(dd, f1, d); // f1 = z + yd + y + d
    // Cudd_Ref(f1);

    // f1_zdd = Cudd_zddPortFromBdd(dd, f1);
    // Cudd_Ref(f1_zdd);

    // ///// TODO: create f2, f3 ////

    // ///// compute r0 --f1--> r1
    // r0 = z;
    // r0_zdd = Cudd_zddPortFromBdd(dd, r0);
    // Cudd_Ref(r0_zdd);
    // r1 = cuddZddDivide(dd, r0_zdd, f1_zdd);
    // Cudd_Ref(r1);

    // // display result
    // printf("printing r0 --f1--> r1\n");
    // ZDD_dotfile(dd, r0_zdd, "r0_zdd", names);
    // ZDD_dotfile(dd, f1_zdd, "f1_zdd", names);
    // ZDD_dotfile(dd, r1, "r1_zdd", names);


    exit(0);

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

    BDD_dotfile(dd, e, "abc_bdd", names);

    /* BDD to ZDD */
    g = Cudd_zddPortFromBdd(dd, e);
    Cudd_Ref(g);

    ZDD_dotfile(dd, g, "abc_zdd", names);

    /*** Create f1, f2 ***/
    // f1 = e + dc
    // f2 = d + ab

    // we need to do z -f1-> r1
    z = Cudd_bddNewVar(dd); // z is e
    Cudd_Ref(z);

    printf("Printing the minterms of z:");
    Cudd_PrintMinterm(dd, z); 
    BDD_dotfile(dd, z, "z_bdd", names);

    //r0 = Cudd_bddIte(dd, z, g, e);
    /*
    r1 = Cudd_bddDivide(dd, z, e);
    Cudd_Ref(r1);


    printf("Printing the minterms of r1:");
    Cudd_PrintMinterm(dd, r1); 
    BDD_dotfile(dd, r1, "r1_bdd");
    */

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


/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
