# ECE6745 FINAL PROJECT README SUBMISSION 

Verification of Polynomial Division using ZDDs

## Introduction

This project uses ZDDs (from the CUDD package) to compute verification for digital circuits. 

A circuit can be represented by a list of polynomials, where each polynomial represents a gate and the associated nets in the circuit. Thus, we can implement these polynomials as ZDDs to represent a given circuit.

We follow the methodology described in Dr. Kalla's paper on [Grobner Basis Reductions](https://my.ece.utah.edu/~kalla/papers/TCAD-GBR-ZDD.pdf) to apply polynomial division. Given an output net from our circuit, we show that its reduction with the list of ZDD bit-level polynomials gives us a ZDD output made up only primary inputs. We use this output to verify against the expected circuit implementation.

See the project report for a breakdown on our approach, challenges we faced, and results for the verification of a chain of OR gates and a 2-bit multiplier.

GITHUB LINK: 
https://github.com/NathanNator/ECE6745_Final_Project/tree/main

## Source Code Location and Commands: 

### Final Report Location
```
documentation/ECE6745_Final_Report.pdf 
```

### The source file for ZDD implementation (main.c) is located here
```
polynomial_division/cudd-3.0.0/nanotrav/main.c
```

### The source file for Singular Scripts are located here
```
Singular/
```

* 2-bit multiplier:
`/Singular/multiplier.sing`

* Chain of OR gates:
`/Singular/ORgates.sing`

* f = x + yz:
`/Singular/poly2.sing`




### Command to run main.c 
```bash 
cd polynomial_division/cudd-3.0.0/ 
./nanotrav/nanotrav
```

### ZDD Plots for figures
```
zdd_plots/
```

### Python Script to add the node variable names for all the dot files in a current directory
```
zdd_plots/add_node_names.py
```

## Authors

* [Fernando Araujo](https://www.linkedin.com/in/fernando-araujo-0ba6551aa/)
* [Nathan Sartnurak](https://github.com/NathanNator)
* [Kidus Yohannes](https://kidusyohannes.me/)


## Resources

* Dr. Kalla's paper on [Grobner Basis Reductions](https://my.ece.utah.edu/~kalla/papers/TCAD-GBR-ZDD.pdf)

* CUDD Package & [Documentation](https://web.mit.edu/sage/export/tmp/y/usr/share/doc/polybori/cudd/cuddAllDet.html#prototypes)

* Special thanks to Dr. Kalla for his guidance and feedback! :)