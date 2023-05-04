#include "MaxNormOptimization.h"
#include "Implication.h"
#include <vector>
#include "Selection.h"



using namespace std;

Selection changed_vertices;
Selection fixed_vertices;

vector<int> Q;


class Qelem {
public:
	Qelem(long i, long j, bool isVertex) {
		a = i;
		b = j;
		vertex = isVertex;
	}

	//If "vertex" is true, then b is the index of that vertex, and a is the index of the other end of the edge through which we arrived here
	//If "vertex" is false, the a and b are the indices of the edge, traversed from a to b
	long a; 
	long b;
	bool vertex;
};


bool Relabel(long i, long j, config c, ImplicationStructure& is, vector<int8_t>& labels) {
	Q.clear();

	changed_vertices.clear();
	fixed_vertices.clear();

	long popped = 0;

	if (i == j) { //vertex
		Q.push_back(i);
		fixed_vertices.insert(i);
		if (c == l0 && labels[i] != 0) {
			labels[i] = 0;
			changed_vertices.insert(i);
		}
		else if (c == l1 && labels[i] != 1) {
			labels[i] = 1;
			changed_vertices.insert(i);
		}

	}
	else { //edge
		fixed_vertices.insert(i);
		fixed_vertices.insert(j);


		if (c == l00) {
			if (labels[i] != 0) { changed_vertices.insert(i); Q.push_back(i); }
			if (labels[j] != 0) { changed_vertices.insert(j); Q.push_back(j); }
			labels[i] = 0;
			labels[j] = 0;
		}
		else if (c == l01) {
			if (labels[i] != 0) { changed_vertices.insert(i); Q.push_back(i); }
			if (labels[j] != 1) { changed_vertices.insert(j); Q.push_back(j); }
			labels[i] = 0;
			labels[j] = 1;
		}
		else if (c == l10) {
			if (labels[i] != 1) { changed_vertices.insert(i); Q.push_back(i); }
			if (labels[j] != 0) { changed_vertices.insert(j); Q.push_back(j); }

			labels[i] = 1;
			labels[j] = 0;
		}
		else if (c == l11) {
			if (labels[i] != 1) { changed_vertices.insert(i); Q.push_back(i); }
			if (labels[j] != 1) { changed_vertices.insert(j); Q.push_back(j); }
			labels[i] = 1;
			labels[j] = 1;
		}

	}

	long v;
	long w;
	int w_label;

	int n;
	Implication* implications;
	while (!Q.empty()) {
		v = Q.back();
		Q.pop_back();
		popped++;
		implications = is.GetImplications(v, labels[v]);
		n = is.GetNumberOfImplications(v, labels[v]);

		for (int i = 0; i < n; i++) {
			w = implications[i].target;
			w_label = implications[i].target_label;
			if (labels[w] != w_label) {
				if (fixed_vertices.find(w)  ) {
					long nchanged = changed_vertices.GetSelectionSize();

					long index;
					for (int i = 0; i < nchanged; i++) {
						index = changed_vertices[i];
						labels[index] = !labels[index];
					}
					return false;
				}
				else {
					labels[w] = w_label;
					changed_vertices.insert(w);
					Q.push_back(w);
				}
			}
			fixed_vertices.insert(w);
		}
	}
	return true;

}




//Relabel vertex
bool TryRelabel(long i, ImplicationStructure& is, vector<int8_t>& labels) {
	
	config new_config = l1;
	if (labels[i] == 1) { new_config = l0; }
	
	return Relabel(i, i, new_config, is, labels);

}

bool TryRelabel(long i, long j, ImplicationStructure& is, vector<int8_t>& labels) {

	int i_label = labels[i];
	int j_label = labels[j];

	if (  !(i_label == 0 && j_label == 0)) {
			if (Relabel(i, j, l00, is, labels)) { return true; }
	}

	

	if ( !(i_label == 1 && j_label == 1)) {
			if (Relabel(i, j, l11, is, labels)) { return true; }
	}

	if ( !(i_label == 1 && j_label == 0)) {
			if (Relabel(i, j, l10, is, labels)) { return true; }
	}

	if ( !(i_label == 0 && j_label == 1)) {
			if (Relabel(i, j, l01, is, labels)) { return true; }
	}

	return false;
}



void RemoveAtom(ImplicationStructure& is,long a, long b, config c) {
	if (a == b) { //Unary atom
		if (c == l0) {
			is.AddImplication(a, 0, a, 1);
		}
		else { //c assumed to be l1
			is.AddImplication(a, 1, a, 0);
		}

	}
	else { //Binary atom
		if (c == l00) {
			is.AddImplication(a, 0, b, 1);
			is.AddImplication(b, 0, a, 1);
		}
		else if (c == l11) {
			is.AddImplication(a, 1, b, 0);
			is.AddImplication(b, 1, a, 0);
		}
		else if (c == l01) {
			is.AddImplication(a, 0, b, 0);
			is.AddImplication(b, 1, a, 1);
		}
		else if (c == l10) {
			is.AddImplication(a, 1, b, 1);
			is.AddImplication(b, 0, a, 0);
		}

	}
}


void Optimize(ObjectiveFunction& f, vector<int8_t>& labels, bool presorted) {
	
	//Create convenient shortcuts
	long nelem = f.nelem;
	std::vector<Atom>& Atoms = f.Atoms;

	changed_vertices = Selection(nelem);
	fixed_vertices = Selection(nelem);

	//Initialize labels
	labels.resize(nelem);
	std::fill(labels.begin(), labels.end(), 0);

	//Sort atoms by weight
	CompareAtomByWeight ac;
	
	if (!presorted) {
		sort(Atoms.begin(), Atoms.end(), ac);
	}

	ImplicationStructure is(nelem, f.max_n);
	
	bool can_remove;

	int i_label, j_label;

	Atom a;

	for (long i = 0; i < Atoms.size(); i++) {
		a = Atoms[i];

		//Try to remove "a" here
		can_remove = false;
		
		i_label = labels[a.i];
		j_label = labels[a.j];

		if (a.i == a.j) { //Unary atom

			if ((a.configuration == l0 && i_label == 1) || (a.configuration == l1 && i_label == 0)) {
				//Current labeling does not use the atom "a", so we can safely remove it
				can_remove = true;

			}
			else {
				//Do full check
				can_remove = TryRelabel(a.i, is, labels);
			}
		}
		else { //Binary atom			
			if (a.configuration == l00 && !(i_label== 0 && j_label == 0)) {
				//Current labeling does not use the atom "a", so we can safely remove it
				can_remove = true;
			}
			else if (a.configuration == l01 && !(i_label == 0 && j_label == 1)) {
				//Current labeling does not use the atom "a", so we can safely remove it
				can_remove = true;
			}
			else if (a.configuration == l10 && !(i_label == 1 && j_label == 0)) {
				//Current labeling does not use the atom "a", so we can safely remove it
				can_remove = true;
			}
			else if (a.configuration == l11 && !(i_label == 1 && j_label == 1)) {
				//Current labeling does not use the atom "a", so we can safely remove it
				can_remove = true;
			}
			else {
				//Do full check
				can_remove = TryRelabel(a.i, a.j, is, labels);
			}
		}

		if (can_remove) {
			RemoveAtom(is, a.i, a.j, a.configuration);
		}
	}
	
}

