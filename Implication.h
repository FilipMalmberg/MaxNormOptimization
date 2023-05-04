#pragma once

#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

//Represents the implication (source==source_label)->(target==target_label)
class Implication {
public:
	int target;
	int target_label;

public:
	Implication() {

	}

	Implication( int _target, int _label) {
	
		target = _target;
		target_label =_label;
	}
};

class ImplicationStructure {
private:
	vector<Implication> implications;

	long max_implications; //Max number of implications per vertex
	vector<int> n_implications; //No element in this array should be >max_implications
	int n;

public:
	ImplicationStructure(long nelem, int max_neighbors) {
		n = nelem;
		max_implications = 2 * max_neighbors +2; //Each vertex*truth_value can have at most 2 outgoing implications per neighbor
		implications = vector<Implication>(2*nelem * max_implications);
		n_implications = vector<int>(2*nelem, 0); //At the start, there are no implications

		//cout << "Size: " << implications.size()<<endl;

	}

	void AddImplication(long source, int source_label, long target, int target_label) {

		int nimp = n_implications[2*source+source_label];
		if (nimp < max_implications) {
			implications[max_implications * (2*source+source_label) + nimp] = Implication(target, target_label);
			n_implications[2*source+source_label]++;
		}

	}


	Implication* GetImplications(long vertex, int label) {
		return &implications[max_implications * (2 * vertex + label)];
	}

	void GetNeighbors(int v, vector<int>& neighbors) {
		//cout << "Get neighbors" << endl;
		//cout << int(n_implications.size())<<" > "<<v * max_implications << "?" << endl;
		int n_neighbors = n_implications[v];
		neighbors.resize(n_neighbors);
		Implication imp;
		for (int i = 0; i < n_neighbors; i++) {
			imp = implications[max_implications *v +i];
			neighbors[i] = 2 * imp.target + imp.target_label;
		}
		//cout << "Done" << endl;
	}


	int GetNumberOfImplications(long vertex, int label) {
		return n_implications[2 * vertex + label];
	}

	void PrintMaxImplications() {
		int maximp = 0;
		for (int i = 0; i < n_implications.size(); i++) {
			if (n_implications[i] > maximp) {
				maximp = n_implications[i];
			}
		}
		cout << "Max implications: " << maximp << endl;
	}

	void Print() {
		cout << "--------BIG -----------" << endl;

		Implication imp;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 2; j++) {
				int n_impl = GetNumberOfImplications(i, j);
				for (int k = 0; k < n_impl; k++) {
					imp = implications[max_implications * (2 * i + j)+k];
					cout << i << "=" << j << " -> " << imp.target << " = " << imp.target_label << endl;
				}
			}
		}
		cout << "---------------------" << endl;
		/*cout << "------BIG--------" << endl;
		Implication imp;
		for (int i = 0; i < implications.size(); i++) {
			int n = i / 2;
			int label = i % 2;
			
			for (int j = 0; j < implications[i].size(); j++) {
				imp = implications[i][j];
				cout << "(" << n << "=" << label << ")->(" << imp.target << "=" << imp.target_label << ")" << endl;
			}
		}
		cout << "-----------------" << endl;*/

	}

};