#include "Selection.h"
#include <iostream>
Selection::Selection() {

}

Selection::Selection(long nelem) {
	selected = vector<int>(nelem, false);
	selection_list = vector<long>();
}

void Selection::insert(long index) {
	if (!selected[index]) {
		selected[index] = true;
		selection_list.push_back(index);
		
	}
}


bool Selection::find(long index) {
	return selected[index];
}

void Selection::clear() {
	for (int i = 0; i < selection_list.size(); i++) {
		selected[selection_list[i]] = false;
	}
	selection_list.clear();
}


long Selection::GetSelectionSize() {
	return long(selection_list.size());
}

long& Selection::operator[](long i) {
	return selection_list[i];
}