#pragma once
#include "Typedefs.h"
#include <vector>
#include "Atom.h"

class ObjectiveFunction {
public:	//Methods
	ObjectiveFunction(long n, int max_neighbors); 

	void AddUnaryTerm(long i, cost_t c0, cost_t c1);
	void AddBinaryTerm(long i, long j, cost_t c00, cost_t c11, cost_t c10, cost_t c01);

	long GetNumberOfElements();

public:	//Members, made public but should not be touched
	long nelem;
	int max_n;
	std::vector<Atom> Atoms;
};