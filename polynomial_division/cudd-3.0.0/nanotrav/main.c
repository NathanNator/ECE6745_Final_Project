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
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names, char* outname);
void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names, char* outname);
DdNode* Cudd_zddDC(DdManager* dd, DdNode* a);
DdNode* Cudd_zddModSum (DdManager* manager, DdNode* a, DdNode* b);
DdNode* Cudd_zddMultMonRed(DdManager* dd, DdNode* z, DdNode *poly_list[], int poly_size); 
DdNode* Cudd_zddAndGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b);
DdNode* Cudd_zddOrGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b);
DdNode* Cudd_zddXorGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b);

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
    
    /* Primary Inputs */
    DdNode *z = Cudd_bddNewVar(dd);
    DdNode *y = Cudd_bddNewVar(dd);
    DdNode *x = Cudd_bddNewVar(dd);
    DdNode *d = Cudd_bddNewVar(dd);
    DdNode *c = Cudd_bddNewVar(dd);
    DdNode *b = Cudd_bddNewVar(dd);
    DdNode *a = Cudd_bddNewVar(dd);

    char *names[7] = { "z", "y", "x", "d", "c", "b", "a"};

    DdNode *z_zdd = Cudd_zddDC(dd, z);
    DdNode *y_zdd = Cudd_zddDC(dd, y);
    DdNode *x_zdd = Cudd_zddDC(dd, x);
    DdNode *d_zdd = Cudd_zddDC(dd, d);
    DdNode *c_zdd = Cudd_zddDC(dd, c);
    DdNode *b_zdd = Cudd_zddDC(dd, b);
    DdNode *a_zdd = Cudd_zddDC(dd, a);

    // f1 = z + yd + y + d
    DdNode *f1 = Cudd_zddOrGate(dd, z_zdd, y_zdd, d_zdd);
    ZDD_dotfile(dd, f1, "../../zdd_plots/f1_zdd", names, "f1");

    // create f2 = y + xc + x + c
    DdNode *f2 = Cudd_zddOrGate(dd, y_zdd, x_zdd, c_zdd);
    ZDD_dotfile(dd, f2, "../../zdd_plots/f2_zdd", names, "f2");

    // create f3 = x + ba + b + a
    DdNode *f3 = Cudd_zddOrGate(dd, x_zdd, b_zdd, a_zdd);
    ZDD_dotfile(dd, f3, "../../zdd_plots/f3_zdd", names, "f3");
 
    // apply z --G--> func w/ primary inputs
    DdNode *poly_list[] = {f1, f2, f3};
    int poly_size = sizeof(poly_list)/sizeof(poly_list[0]);
    DdNode *zi_zdd = Cudd_zddMultMonRed(dd, z_zdd, poly_list, poly_size);
    ZDD_dotfile(dd, zi_zdd, "../../zdd_plots/zi_zdd", names, "zi_zdd");

    // improved one step reduction!
    /*
    // r0 = z -f1-> r1
    DdNode *r0 = z_zdd;
    DdNode *r0_T = Cudd_T(r0); Cudd_Ref(r0_T);
    DdNode *r0_E = Cudd_E(r0); Cudd_Ref(r0_E);
    DdNode *f1_E = Cudd_E(f1); Cudd_Ref(f1_E);

    DdNode *r1 = Cudd_zddUnateProduct(dd, r0_T, f1_E);
    r1 = Cudd_zddModSum(dd, r1, r0_E); Cudd_Ref(r1);
    ZDD_dotfile(dd, r1, "../../zdd_plots/r1_zdd", names);


    // r1 --f2--> r2
    DdNode *r1_T = Cudd_T(r1); Cudd_Ref(r1_T);
    DdNode *r1_E = Cudd_E(r1); Cudd_Ref(r1_E);
    DdNode *f2_E = Cudd_E(f2); Cudd_Ref(f2_E);

    DdNode *r2 = Cudd_zddUnateProduct(dd, r1_T, f2_E);
    r2 = Cudd_zddModSum(dd, r2, r1_E); Cudd_Ref(r2);
    ZDD_dotfile(dd, r2, "../../zdd_plots/r2_zdd", names);
    Cudd_zddPrintCover(dd, r2);


    // r2 --f3--> r3
    DdNode *r2_T = Cudd_T(r2); Cudd_Ref(r2_T);
    DdNode *r2_E = Cudd_E(r2); Cudd_Ref(r2_E);
    DdNode *f3_E = Cudd_E(f3); Cudd_Ref(f3_E);

    DdNode *r3 = Cudd_zddUnateProduct(dd, r2_T, f3_E);
    r3 = Cudd_zddModSum(dd, r3, r2_E); Cudd_Ref(r3);
    ZDD_dotfile(dd, r3, "../../zdd_plots/r3_zdd", names);
    Cudd_zddPrintMinterm(dd, r3);
    */
    
    // 2 - bit multiplier
    DdManager *dd_mult = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    Cudd_ReduceHeap(dd_mult, CUDD_REORDER_NONE, 0);

    DdNode *z0, *z1, *r0, *c0, *c3, *c1, *c2;
    DdNode *a0, *a1, *b0, *b1;
    z0 = Cudd_bddNewVar(dd_mult);
    z1 = Cudd_bddNewVar(dd_mult);
    r0 = Cudd_bddNewVar(dd_mult);
    c0 = Cudd_bddNewVar(dd_mult);
    c3 = Cudd_bddNewVar(dd_mult);
    c1 = Cudd_bddNewVar(dd_mult);
    c2 = Cudd_bddNewVar(dd_mult);
    a0 = Cudd_bddNewVar(dd_mult);
    a1 = Cudd_bddNewVar(dd_mult);
    b0 = Cudd_bddNewVar(dd_mult);
    b1 = Cudd_bddNewVar(dd_mult);
    
    char *names_m[11] = { "z0", "z1", "r0", "c0", "c3", "c1", "c2", "a0", "a1", "b0", "b1" };

    DdNode *z0_zdd = Cudd_zddDC(dd_mult, z0);
    DdNode *z1_zdd = Cudd_zddDC(dd_mult, z1);
    DdNode *r0_zdd = Cudd_zddDC(dd_mult, r0);
    DdNode *c0_zdd = Cudd_zddDC(dd_mult, c0);
    DdNode *c3_zdd = Cudd_zddDC(dd_mult, c3);
    DdNode *c1_zdd = Cudd_zddDC(dd_mult, c1);
    DdNode *c2_zdd = Cudd_zddDC(dd_mult, c2);
    DdNode *a0_zdd = Cudd_zddDC(dd_mult, a0);
    DdNode *a1_zdd = Cudd_zddDC(dd_mult, a1);
    DdNode *b0_zdd = Cudd_zddDC(dd_mult, b0);
    DdNode *b1_zdd = Cudd_zddDC(dd_mult, b1);

    DdNode *f1_m = Cudd_zddAndGate(dd_mult, c0_zdd, a0_zdd, b0_zdd);
    DdNode *f2_m = Cudd_zddAndGate(dd_mult, c1_zdd, a0_zdd, b1_zdd);
    DdNode *f3_m = Cudd_zddAndGate(dd_mult, c2_zdd, a1_zdd, b0_zdd);
    DdNode *f4_m = Cudd_zddAndGate(dd_mult, c3_zdd, a1_zdd, b1_zdd);
    DdNode *f5_m = Cudd_zddXorGate(dd_mult, r0_zdd, c1_zdd, c2_zdd);
    DdNode *f6_m = Cudd_zddXorGate(dd_mult, z0_zdd, c0_zdd, c3_zdd);
    DdNode *f7_m = Cudd_zddXorGate(dd_mult, z1_zdd, r0_zdd, c3_zdd);

    DdNode *poly_list_m[] = {f7_m, f6_m, f5_m, f1_m, f4_m, f2_m, f3_m}; // RTTO important!
    int poly_size_m = sizeof(poly_list_m)/sizeof(poly_list_m[0]);
    
    DdNode *z0_out_zdd = Cudd_zddMultMonRed(dd_mult, z0_zdd, poly_list_m, poly_size_m);
    ZDD_dotfile(dd_mult, z0_out_zdd, "../../zdd_plots/z0_out_zdd", names_m, "z0_out");
    
    DdNode *z1_out_zdd = Cudd_zddMultMonRed(dd_mult, z1_zdd, poly_list_m, poly_size_m);
    ZDD_dotfile(dd_mult, z1_out_zdd, "../../zdd_plots/z1_out_zdd", names_m, "z1_out");

    ///// Manually confirm functions /////
    // z0 manual = a0b0 + a1b1
    DdNode *temp0 = Cudd_zddUnateProduct(dd_mult, a0_zdd, b0_zdd); Cudd_Ref(temp0);
    DdNode *temp1 = Cudd_zddUnateProduct(dd_mult, a1_zdd, b1_zdd); Cudd_Ref(temp1);
    DdNode *z0_manual = Cudd_zddModSum(dd_mult, temp0, temp1); Cudd_Ref(z0_manual);
    ZDD_dotfile(dd_mult, z0_manual, "../../zdd_plots/z0_manual_zdd", names_m, "z0_manual");

    // z1 manual = a0b1 + a1b0 + a1b1 
    DdNode *temp2 = Cudd_zddUnateProduct(dd_mult, a0_zdd, b1_zdd); Cudd_Ref(temp2);
    DdNode *temp3 = Cudd_zddUnateProduct(dd_mult, a1_zdd, b0_zdd); Cudd_Ref(temp3);
    DdNode *z1_manual = Cudd_zddModSum(dd_mult, temp2, temp3); 
    z1_manual = Cudd_zddModSum(dd_mult, z1_manual, temp1);  Cudd_Ref(z1_manual);
    ZDD_dotfile(dd_mult, z1_manual, "../../zdd_plots/z1_manual_zdd", names_m, "z1_manual");

    exit(0);

} /* end of main */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/


/**
 * @brief Dumps the BDD as dot file with the specified name.
 */
void BDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names, char* outname)
{
    char filename[64];
    char *oname[1] = { outname };
    sprintf(filename, "%s.dot", fname);
    FILE* outfile = fopen(filename, "w");
    Cudd_DumpDot(manager, 1, &node, (char**)names, (char**)oname, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}


/**
 * @brief Dumps the ZDD as dot file with the specified name.
 */
void ZDD_dotfile (DdManager* manager, DdNode* node, char* fname, char* names, char* outname)
{
    char filename[64];
    char *oname[1] = { outname };
    sprintf(filename, "%s.dot", fname);
    FILE* outfile = fopen(filename, "w");
    Cudd_zddDumpDot(manager, 1, &node, (char**)names, (char**)oname, outfile);
    fclose(outfile);
    printf("\nWritten to file: %s\n", filename );
}


/**
 * @brief Implements a single variable ZDD. 
 * Applies the not/don't care with every other variable.
 */
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


/**
 * @brief Implements modulo 2 sum for ZDDs.
 * (Union of A and B) diff (Intersect of A and B).
 * f = (A ∪ B) - (A ∩ B)
 */
DdNode* Cudd_zddModSum (DdManager* dd, DdNode* a, DdNode* b)
{
    DdNode *uni = Cudd_zddUnion(dd, a, b); Cudd_Ref(uni);
    DdNode *isect = Cudd_zddIntersect(dd, a, b); Cudd_Ref(isect);
    DdNode *diff = Cudd_zddDiff(dd, uni, isect); Cudd_Ref(diff);
    Cudd_RecursiveDerefZdd(dd, uni);
    Cudd_RecursiveDerefZdd(dd, isect);
    return diff;
}


/**
 * @brief Implements multi-monomial reduction with ZDDs.
 * Applies reduction to z with every polynomial given in polylist.
 */
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


/**
 * @brief Implements an OR gate as a ZDD polynomial.
 * f = z + ab
 */
DdNode* Cudd_zddAndGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b)
{
    DdNode *ab_zdd = Cudd_zddUnateProduct(dd, a, b); Cudd_Ref(ab_zdd);
    DdNode *f = Cudd_zddModSum(dd, z, ab_zdd); Cudd_Ref(f);
    return f; 
}


/**
 * @brief Implements an OR gate as a ZDD polynomial.
 * f = z + ab + a + b
 */
DdNode* Cudd_zddOrGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b)
{
    DdNode *ab_zdd = Cudd_zddUnateProduct(dd, a, b); Cudd_Ref(ab_zdd);
    DdNode *f = Cudd_zddModSum(dd, z, ab_zdd);
    f = Cudd_zddModSum(dd, f, a);
    f = Cudd_zddModSum(dd, f, b); Cudd_Ref(f);

    return f; 
}


/**
 * @brief Implements an XOR gate as a ZDD polynomial.
 * f = z + a + b
 */
DdNode* Cudd_zddXorGate(DdManager* dd, DdNode* z, DdNode* a, DdNode* b)
{
    DdNode *f = Cudd_zddModSum(dd, z, a);
    f = Cudd_zddModSum(dd, f, b); Cudd_Ref(f);
    return f; 
}

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
