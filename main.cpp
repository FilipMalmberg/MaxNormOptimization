#include "MaxNormOptimization.h"
#include <iostream>
#include <vector>

int main() {
	
	//Example usage, optimal labeling of a graph with 4 vertices

	ObjectiveFunction f(4 /*number of vertices*/ , 2 /*Maximum number of edges incident at any single vertex*/);
	
	//By default, all costs are of type "int"
	//If you want to change this, e.g., to use floating point costs, just change the typedef "cost_t" in the file "Typedefs.h"

	//Adding unary terms for two of the vertices
	f.AddUnaryTerm(0, 100,50);
	f.AddUnaryTerm(1, 25, 80);

	//Adding pairwise (binary) terms
	f.AddBinaryTerm(0, 1, 10, 10, 300, 300);
	f.AddBinaryTerm(1, 2, 200, 220, 10, 20);  // Non-submodular term
	f.AddBinaryTerm(2, 3, 10, 10, 500, 600); 

	std::vector<int8_t> labels(4, 0); //Allocate a vector to store the labeling
	Optimize(f, labels);
	

	//The vector "labels" now contains the result. 
	for (int i = 0; i < labels.size(); i++) {
		std::cout << int(labels[i]) << std::endl;
	}

	return 0;
}