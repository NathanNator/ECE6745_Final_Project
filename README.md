# ECE6745 FINAL PROJECT

Verification of Polynomial Division using ZDDs

## Introduction

This project uses ZDDs (from the CUDD package) to compute verification for digital circuits. 

A circuit can be represented by a list of polynomials, where each polynomial represents a gate and the associated nets in the circuit. Thus, we can implement these polynomials as ZDDs to represent a given circuit.

We follow the methodology described in Dr. Kalla's paper on [Grobner Basis Reductions](https://my.ece.utah.edu/~kalla/papers/TCAD-GBR-ZDD.pdf) to apply polynomial division. Given an output net from our circuit, we show that its reduction with the list of ZDD bit-level polynomials gives us a ZDD output made up only primary inputs. We use this output to verify against the expected circuit implementation.

See the project report for a breakdown on our approach, challenges we faced, and results for the verification of a 4 input OR gate and a 2-bit multiplier.

## Setup

This project uses the CUDD package. You can follow [these](https://davidkebo.com/cudd/) instructions to download and install it. However, the repo already contains the package (**tools/cudd-3.0.0.tar.gz**) and a start up bash script (**start_up.sh**). 

**So all that is required after cloning the repo is to run the start up script.** The script automatically extracts the tar file, restores the main.c file that is saved on git, and the makefile. The modified makefile is set up so debugging works in VS Code. 

The source file (main.c) is located here: **polynomial_division/cudd-3.0.0/nanotrav/main.c**

After modifying main.c, you must run **'make check'** to recompile the code.

Finally, run the nanotrav command to run the code:
```
./polynomial_division/cudd-3.0.0/nanotrav/nanotrav
```

## Authors

* [Fernando Araujo](https://www.linkedin.com/in/fernando-araujo-0ba6551aa/)
* [Nathan Sartnurak](https://github.com/NathanNator)
* [Kidus Yohannes](https://kidusyohannes.me/)


## Resources

* Dr. Kalla's paper on [Grobner Basis Reductions](https://my.ece.utah.edu/~kalla/papers/TCAD-GBR-ZDD.pdf)

* CUDD Package & [Documentation](https://web.mit.edu/sage/export/tmp/y/usr/share/doc/polybori/cudd/cuddAllDet.html#prototypes)

* Special thanks to Dr. Kalla for his guidance and feedback! :)
