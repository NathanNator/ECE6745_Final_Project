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
#include "util.h"


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

/**Function********************************************************************

  Synopsis    [Main program for ntr.]

  Q3 main.c file

  SideEffects [None]

  SeeAlso     []

******************************************************************************/

int
main(
  int  argc,
  char ** argv)
{
    DdManager* gbm; /* Global BDD manager. */
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    // Test for f = w'x'z' + wx'z + w'yz + wyz'

    DdNode *one, *zero;
    DdNode *w, *x, *y, *z, *f, *g; 
    DdNode *w_not, *x_not, *z_not; 
    DdNode *m1, *m2, *m3, *m4; 
    DdNode *f_g, *f_gbar, *g_not;
    DdNode *r1, *r2;

    one = Cudd_ReadOne(gbm);  Cudd_Ref(one); 

    w = Cudd_bddNewVar(gbm);  
    x = Cudd_bddNewVar(gbm);  
    y = Cudd_bddNewVar(gbm);  
    z = Cudd_bddNewVar(gbm);  
   
    w_not = Cudd_Not(w);  Cudd_Ref(w_not);
    x_not = Cudd_Not(x);  Cudd_Ref(x_not);
    z_not = Cudd_Not(z);  Cudd_Ref(z_not);

    // Constructing Minterms
    
    m1 = Cudd_bddAnd(gbm, w_not, x_not); Cudd_Ref(m1);
    m1 = Cudd_bddAnd(gbm, m1, z_not); Cudd_Ref(m1);
  
    m2 = Cudd_bddAnd(gbm, w, x_not); Cudd_Ref(m2);
    m2 = Cudd_bddAnd(gbm, m2, z); Cudd_Ref(m2);
    
    m3 = Cudd_bddAnd(gbm, w_not, y); Cudd_Ref(m3);
    m3 = Cudd_bddAnd(gbm, m3, z); Cudd_Ref(m3);

    m4 = Cudd_bddAnd(gbm, w, y); Cudd_Ref(m4);
    m4 = Cudd_bddAnd(gbm, m4, z_not); Cudd_Ref(m4);

    // Constructing f
    
    f = Cudd_bddOr(gbm, m1, m2); Cudd_Ref(f);
    f = Cudd_bddOr(gbm, f, m3); Cudd_Ref(f);
    f = Cudd_bddOr(gbm, f, m4); Cudd_Ref(f);
    
    // Constructing g

    g = Cudd_bddXor(gbm, w, z); Cudd_Ref(g);
    g_not = Cudd_Not(g);  Cudd_Ref(g_not);

    // i) bddRestrict 

    f_g = Cudd_bddRestrict(gbm, f, g); Cudd_Ref(f_g);
    f_gbar = Cudd_bddRestrict(gbm, f, g_not); Cudd_Ref(f_gbar);
    
    // Printing Minterms
    printf("Printing the minterms using bddRestrict:\n"); 
    printf("wxyz\n"); 
    Cudd_PrintMinterm(gbm, f_g);
    Cudd_PrintMinterm(gbm, f_gbar);

    // f ^ g ⊆ f @ g ⊆ f + g'
    
    // Test if f ^ g ⊆ f @ g
    // (f ^ g)' + f @ g = 1

    r1 = Cudd_bddAnd(gbm, f, g); Cudd_Ref(r1);
    r1 = Cudd_Not(r1); Cudd_Ref(r1);
    r1 = Cudd_bddOr(gbm, r1, f_g); Cudd_Ref(r1);

    if(r1 == one) {
        printf("Equivalence (f ^ g)' + (f @ g) = 1\n");
    }
    else{
        printf("Equivalence (f ^ g)' + (f @ g) != 1\n");
    }

    // Test if f @ g ⊆ f + g'
    // (f @ g)' + f + g' = 1

    r2 = Cudd_Not(f_g); Cudd_Ref(r2);
    r2 = Cudd_bddOr(gbm, r2, f); Cudd_Ref(r2);
    r2 = Cudd_bddOr(gbm, r2, g_not); Cudd_Ref(r2);

    if(r2 == one) {
        printf("Equivalence (f @ g)' + f + g' = 1\n");
    }
    else{
        printf("Equivalence (f @ g)' + f + g' != 1\n");
    }


    // ii) bddConstrain 

    f_g = Cudd_bddConstrain(gbm, f, g); Cudd_Ref(f_g);
    f_gbar = Cudd_bddConstrain(gbm, f, g_not); Cudd_Ref(f_gbar);

    // Printing Minterms
    printf("Printing the minterms bddConstrain:\n"); 
    printf("wxyz\n"); 
    Cudd_PrintMinterm(gbm, f_g);
    Cudd_PrintMinterm(gbm, f_gbar);

    // f ^ g ⊆ f @ g ⊆ f + g'
    
    // Test if f ^ g ⊆ f @ g
    // (f ^ g)' + f @ g = 1

    r1 = Cudd_bddAnd(gbm, f, g); Cudd_Ref(r1);
    r1 = Cudd_Not(r1); Cudd_Ref(r1);
    r1 = Cudd_bddOr(gbm, r1, f_g); Cudd_Ref(r1);

    if(r1 == one) {
        printf("Equivalence (f ^ g)' + (f @ g) = 1\n");
    }
    else{
        printf("Equivalence (f ^ g)' + (f @ g) != 1\n");
    }

    // Test if f @ g ⊆ f + g'
    // (f @ g)' + f + g' = 1

    r2 = Cudd_Not(f_g); Cudd_Ref(r2);
    r2 = Cudd_bddOr(gbm, r2, f); Cudd_Ref(r2);
    r2 = Cudd_bddOr(gbm, r2, g_not); Cudd_Ref(r2);

    if(r2 == one) {
        printf("Equivalence (f @ g)' + f + g' = 1\n");
    }
    else{
        printf("Equivalence (f @ g)' + f + g' != 1\n");
    }

    /* Writing Out to .dot files */

    //f = Cudd_BddToAdd(gbm, f); /*Convert BDD to ADD for display purpose*/
    const char* filename1 = "f.dot";
    FILE* outfile1 = fopen(filename1, "w");
    Cudd_DumpDot(gbm, 1, &f, NULL, NULL, outfile1);
    fclose(outfile1);
    printf("Written to file: %s\n", filename1);

    //g = Cudd_BddToAdd(gbm, g); /*Convert BDD to ADD for display purpose*/
    const char* filename2 = "wz_xor.dot";
    FILE* outfile2 = fopen(filename2, "w");
    Cudd_DumpDot(gbm, 1, &g, NULL, NULL, outfile2);
    fclose(outfile2);
    printf("Written to file: %s\n", filename2);

    Cudd_Quit(gbm);
    return 0; 

} /* end of main */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

