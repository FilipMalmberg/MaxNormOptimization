#include "ObjectiveFunction.h"
using namespace std;

bool keep_all_atoms = false;

ObjectiveFunction::ObjectiveFunction(long n, int max_neighbors) {
	nelem = n;
	max_n = max_neighbors;
	Atoms.reserve(2*nelem+ max_neighbors*nelem);
}


void ObjectiveFunction::AddUnaryTerm(long i, cost_t c0, cost_t c1) {
	if (i < 0 || i >= nelem) {
		return;
	}
	cost_t min_cost = c0;
	if (c1 < min_cost) { min_cost = c1; }

	//Atoms whose weight are equal to the minimum weight for the given vertex are disregarded, as they will not affect the result 
	if (keep_all_atoms || c0 > min_cost) {
		Atoms.push_back(Atom(i, l0, c0));
	}
	if (keep_all_atoms || c1 > min_cost) {
		Atoms.push_back(Atom(i, l1, c1));
	}
}

void ObjectiveFunction::AddBinaryTerm(long i, long j, cost_t c00, cost_t c11, cost_t c10, cost_t c01) {
	if (i < 0 || i >= nelem) {
		return;
	}
	if (j < 0 || j >= nelem) {
		return;
	}

	cost_t min_cost = c00;
	if (c11 < min_cost) { min_cost = c11; };
	if (c01 < min_cost) { min_cost = c01; };
	if (c10 < min_cost) { min_cost = c10; };

	//Atoms whose weight are equal to the minimum weight for the given edge are disregarded, as they will not affect the result. 
	//This optimization can be disabled by setting "keep_all_atoms" to true
	if (keep_all_atoms || c00 > min_cost) {
		Atoms.push_back(GetAtom(i, j, l00, c00));
	}
	if (keep_all_atoms || c11 > min_cost) {
		Atoms.push_back(GetAtom(i, j, l11, c11));
	}
	if (keep_all_atoms || c10 > min_cost) {
		Atoms.push_back(GetAtom(i, j, l10, c10));
	}
	if (keep_all_atoms || c01 > min_cost) {
		Atoms.push_back(GetAtom(i, j, l01, c01));
	}
	
}

long ObjectiveFunction::GetNumberOfElements() {
	return nelem;
}

