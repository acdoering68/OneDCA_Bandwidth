# OneDCA_Bandwidth
Determine the Bandwidth for 1D Cellular Automata

This is the code for the paper "Bandwidth of firing squad algorithms" in PARS Mitteilungen 2010.
This paper is also available as Research Report RZ 3772 from the IBM Research Laboratory, Zurich.

Firing Squad is one of the most basic problems for computation on one-dimensional cellular automata. 
Over the time, several algorithms for this problem have been published. 
With this algorithm it is analyzed how much data needs to be transferred from one part to another
if the cellular automaton was cut at a specific site. 
This is done with a SAT-count algorithm for Binary Decision Diagrams.
The configuration data is collected in an XML file and the results are also collected in XML.

## Dependencies

The CUDD (Colorado University BDD) by Fabio Somenzi needs to be present, it can be obtained here:
https://github.com/ivmai/cudd

XercesC XML parser is required, available here:
https://github.com/apache/xerces-c.git

## Building

Set include file paths

## Running


