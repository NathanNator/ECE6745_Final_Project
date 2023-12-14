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
DdNode* Cudd_zddDC(DdManager* dd, DdNode* a);
DdNode* Cudd_zddAndGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b);
DdNode* Cudd_zddMultMonRed(DdManager* dd, DdNode* z, DdNode *poly_list[], int poly_size); 

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
    Cudd_ReduceHeap(dd, CUDD_REORDER_NONE, 0);
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

    char *names[7] = { "z", "y", "x", "d", "c", "b", "a"};

    DdNode *z_zdd = Cudd_zddDC(dd, z); Cudd_Ref(z_zdd);
    DdNode *y_zdd = Cudd_zddDC(dd, y); Cudd_Ref(y_zdd);
    DdNode *x_zdd = Cudd_zddDC(dd, x); Cudd_Ref(x_zdd);
    DdNode *d_zdd = Cudd_zddDC(dd, d); Cudd_Ref(d_zdd);
    DdNode *c_zdd = Cudd_zddDC(dd, c); Cudd_Ref(c_zdd);
    DdNode *b_zdd = Cudd_zddDC(dd, b); Cudd_Ref(b_zdd);
    DdNode *a_zdd = Cudd_zddDC(dd, a); Cudd_Ref(a_zdd);

    // f1 = z + yd + y + d
    DdNode *f1 = Cudd_zddAndGate(dd, z_zdd, y_zdd, d_zdd);
    ZDD_dotfile(dd, f1, "../../zdd_plots/f1_zdd", names);

    // create f2 = y + xc + x + c
    DdNode *f2 = Cudd_zddAndGate(dd, y_zdd, x_zdd, c_zdd);
    ZDD_dotfile(dd, f2, "../../zdd_plots/f2_zdd", names);

    // create f3 = x + ba + b + a
    DdNode *f3 = Cudd_zddAndGate(dd, x_zdd, b_zdd, a_zdd);
    ZDD_dotfile(dd, f3, "../../zdd_plots/f3_zdd", names);
 
    DdNode *poly_list[] = {f1, f2, f3};
    int poly_size = sizeof(poly_list)/sizeof(poly_list[0]);
    DdNode *zi_zdd = Cudd_zddMultMonRed(dd, z_zdd, poly_list, poly_size);
    ZDD_dotfile(dd, zi_zdd, "../../zdd_plots/zi_zdd", names);

    // improved one step reduction!
    /// r0 = z -f1-> r1
    // DdNode *r0 = z_zdd;
    // DdNode *r0_T = Cudd_T(r0); Cudd_Ref(r0_T);
    // DdNode *r0_E = Cudd_E(r0); Cudd_Ref(r0_E);
    // DdNode *f1_E = Cudd_E(f1); Cudd_Ref(f1_E);

    // DdNode *r1 = Cudd_zddUnateProduct(dd, r0_T, f1_E);
    // r1 = Cudd_zddModSum(dd, r1, r0_E); Cudd_Ref(r1);
    // ZDD_dotfile(dd, r1, "../../zdd_plots/r1_zdd", names);


    // // r1 --f2--> r2
    // DdNode *r1_T = Cudd_T(r1); Cudd_Ref(r1_T);
    // DdNode *r1_E = Cudd_E(r1); Cudd_Ref(r1_E);
    // DdNode *f2_E = Cudd_E(f2); Cudd_Ref(f2_E);

    // DdNode *r2 = Cudd_zddUnateProduct(dd, r1_T, f2_E);
    // r2 = Cudd_zddModSum(dd, r2, r1_E); Cudd_Ref(r2);
    // ZDD_dotfile(dd, r2, "../../zdd_plots/r2_zdd", names);
    // Cudd_zddPrintCover(dd, r2);


    // // r2 --f3--> r3
    // DdNode *r2_T = Cudd_T(r2); Cudd_Ref(r2_T);
    // DdNode *r2_E = Cudd_E(r2); Cudd_Ref(r2_E);
    // DdNode *f3_E = Cudd_E(f3); Cudd_Ref(f3_E);

    // DdNode *r3 = Cudd_zddUnateProduct(dd, r2_T, f3_E);
    // r3 = Cudd_zddModSum(dd, r3, r2_E); Cudd_Ref(r3);
    // ZDD_dotfile(dd, r3, "../../zdd_plots/r3_zdd", names);
    // Cudd_zddPrintMinterm(dd, r3);
    
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

DdNode* Cudd_zddDC(DdManager* dd, DdNode* a)
{
    DdNode *a_dc = a; Cudd_Ref(a_dc);
    DdNode *x_town; 
    int size = Cudd_ReadSize(dd);
    for (int i=0; i < size; i++)
    {
      if (i == a->index) continue;
      x_town = Cudd_bddIthVar(dd, i); Cudd_Ref(x_town);
      a_dc = Cudd_bddAnd(dd, a_dc, Cudd_Not(x_town));
      Cudd_Deref(x_town);
    }
    
    a_dc = Cudd_zddPortFromBdd(dd, a_dc); Cudd_Ref(a_dc);
    return a_dc;
}


DdNode* Cudd_zddAndGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b)
{
    // create f = z + ab + a + b
    DdNode *ab_zdd = Cudd_zddUnateProduct(dd, a, b); Cudd_Ref(ab_zdd);
    DdNode *f = Cudd_zddModSum(dd, z, ab_zdd);
    f = Cudd_zddModSum(dd, f, a);
    f = Cudd_zddModSum(dd, f, b); Cudd_Ref(f);

    return f; 
}


DdNode* Cudd_zddMultMonRed(DdManager* dd, DdNode* z, DdNode *poly_list[], int poly_size)
{
    DdNode *zi = z; 
    for (int i = 0; i < poly_size; i++){
      
        DdNode *g = poly_list[i];

        if(g->index == zi->index){
          DdNode *zi_T = Cudd_T(zi); Cudd_Ref(zi_T);
          DdNode *zi_E = Cudd_E(zi); Cudd_Ref(zi_E);
          DdNode *g_E = Cudd_E(g); Cudd_Ref(g_E);

          zi = Cudd_zddUnateProduct(dd, zi_T, g_E);
          zi = Cudd_zddModSum(dd, zi, zi_E); Cudd_Ref(zi);

          Cudd_RecursiveDeref(dd, zi_T);
          Cudd_RecursiveDeref(dd, zi_E);
          Cudd_RecursiveDeref(dd, g_E);
        }
    }
    
    return zi;
}


/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
