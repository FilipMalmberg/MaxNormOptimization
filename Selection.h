#pragma once

#include <vector>
using namespace std;

class Selection {
public:
	Selection();
	Selection(long nelem);

	void insert(long index);
	bool find(long index);

	void clear();

	//Get the number of currently selected elements
	long GetSelectionSize();

	//Get the i:th selected element. Assumes 0<=i<GeSelectionSize()
	long& operator[](long i);

private:
	vector<int> selected;
	vector<long> selection_list;
};