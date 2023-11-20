#!/bin/bash          
echo Start Up Script for CUDD Library 

WORKING_DIR="$(pwd)"
WORKING_DIR+="/polynomial_division/cudd-3.0.0/cudd/"


CUDD_DIR="$(pwd)"
CUDD_DIR+="/polynomial_division/cudd-3.0.0/"

# If CUDD Lib does not exists
if ! [ -d "$WORKING_DIR" ]; 
then 
    
    echo CUDD Library does not exists.
    tar -xzvf tools/cudd-3.0.0.tar.gz 
    rm -rf $CUDD_DIR
    mv cudd-3.0.0 polynomial_division/ 

    #Restore to current main.c on git
    #git status
    git restore polynomial_division/cudd-3.0.0/nanotrav/main.c

    cd polynomial_division/cudd-3.0.0
    ./configure
    make
    make check

else 
    echo CUDD Library already exists in working directory
fi


