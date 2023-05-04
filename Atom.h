#pragma once
#include "Typedefs.h"
#include <algorithm>
#include <iostream>
using namespace std;

class Atom {
public:
	long i;
	long j;
	config configuration;
	cost_t weight;


	Atom() {}

	Atom(long a, long b, config c, cost_t w) {
		i = a;
		j = b;
		weight = w;
		configuration = c;
	}

	Atom(long a, config c, cost_t w) {
		i = a;
		j = a;
		weight = w;
		configuration = c;
	}
	
	void Print() {
		if (configuration == l0) {
			cout <<"Atom ("<< i << "=0)" <<endl;
		}
		else if (configuration == l1) {
			cout << "Atom (" << i << "=1)" <<endl;
		}
		else if (configuration == l00) {
			cout << "Atom (" << i << "=0, " <<j<<"=0)"<< endl;
		}
		else if (configuration == l11) {
			cout << "Atom (" << i << "=1, " << j << "=1)" <<endl;
		}
		else if (configuration == l01) {
			cout << "Atom (" << i << "=0, " << j << "=1)" <<endl;
		}
		else if (configuration == l10) {
			cout << "Atom (" << i << "=1, " << j << "=0)" <<endl;
		}
	}
};

struct CompareAtomByWeight {
	bool operator()(const Atom& lhs, const Atom& rhs) const {
		if (lhs.weight == rhs.weight) {
			return lhs.configuration > rhs.configuration;
		}
		return lhs.weight>rhs.weight;
	}
};

Atom inline GetAtom(long i, long j, config c, cost_t cost) {
	if (i == j) { //Unary atom
		return Atom(i, c, cost);
	}
	else {	//Binary atom
		//Edge order convention: Smallest index first
		long a = min(i, j);
		long b = max(i, j);

		if (a != i) { //Swap config
			if (c == l01) {
				c = l10;
			}
			else if (c == l10) {
				c = l01;
			}
		}
		return Atom(a, b, c, cost);
	}
}
