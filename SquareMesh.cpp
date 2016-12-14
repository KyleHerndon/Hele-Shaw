#include <new>
#include <stdlib.h> 
#include <stdio.h>
#include <algorithm>
#include <cstring>

#include "SquareMesh.h"

SquareMesh::~SquareMesh(){
	for (unsigned i = 0; i < ylength; i++) {
		delete vals[i];
	}
	delete vals;
}

SquareMesh::SquareMesh(unsigned xlength, unsigned ylength, unsigned filterLength) {
	vals = new int*[ylength];
	for (unsigned i = 0; i < ylength; i++) {
		vals[i] = new int[xlength];
	}
	vals[xlength/2][ylength/2]=1; // set the element near the center to be occupied
	this->xlength = xlength;
	this->ylength = ylength;
	this->filterLength = filterLength;
}

SquareMesh::SquareMesh(const SquareMesh& mesh) {
	this->xlength = mesh.xlength*2;
	this->ylength = mesh.ylength*2;
	this->filterLength = mesh.filterLength;
	this->vals = new int*[this->ylength];
	for (unsigned i = 0; i < this->ylength; i++) {
		this->vals[i] = new int[this->xlength]();
		if (i >= this->ylength/4 && i < (3 * this->ylength)/4) {
			std::memcpy(&this->vals[i][this->xlength/4],mesh.vals[i-this->ylength/4],mesh.xlength*sizeof(int));
		}
	}
}

int SquareMesh::value(const Loc& c) {
	return vals[c.x%xlength][c.y%ylength];
}

void SquareMesh::value(const Loc& c, int value) {
	vals[c.x%xlength][c.y%ylength] = value;
}

void SquareMesh::output(std::ofstream& output) {
	for (unsigned i = 0; i < xlength; i++) {
		for (unsigned j = 0; j < ylength; j++) {
			if (vals[i][j]) {
				output << "1";
			} else {
				output << "0";
			}
			if (j != ylength-1) {
				output << " ";
			}
		}
		output << "\n";
	}
}

int SquareMesh::valid(const Loc& c) { // returns true if setting the value at c to be > 1 can not cause unfilled gaps
	int separators = 0;
	Loc locs[8];
	locs[0].x = c.x -1; locs[0]. y = c.y -1;
	locs[1].x = c.x -1; locs[1]. y = c.y -0;
	locs[2].x = c.x -1; locs[2]. y = c.y +1;
	locs[3].x = c.x -0; locs[3]. y = c.y +1;
	locs[4].x = c.x +1; locs[4]. y = c.y +1;
	locs[5].x = c.x +1; locs[5]. y = c.y -0;
	locs[6].x = c.x +1; locs[6]. y = c.y -1;
	locs[7].x = c.x -0; locs[7]. y = c.y -1;
	int prevVal = SquareMesh::value(locs[7]) > 0 ? 1 : 0;
	for (int i = 0; i < 8; i++) {
		int newVal = SquareMesh::value(locs[i]) ? 1 : 0;
		if (prevVal != newVal) {
			separators++;
		}
		prevVal = newVal;
	}
	if (separators == 2) {
		return 1;
	} else {
		return 0;
	}
}

int SquareMesh::neighbors() {
	return 4;
}

Loc* SquareMesh::neighbors(const Loc& c) {
	Loc* locs = new Loc[4];
	for (int i = 0; i < 4; i++) {
		locs[i].x=c.x;
		locs[i].y=c.y;
	}
	locs[0].x = (locs[0].x + 1) % xlength;
	locs[1].y = (locs[1].y + 1) % xlength;
	locs[2].x = (locs[2].x - 1) % ylength;
	locs[3].y = (locs[3].y - 1) % ylength;
	return locs;
}

Loc* SquareMesh::edges() {
	Loc* ret = new Loc[2*xlength+2*ylength-4];
	int index = 0;
	for (unsigned i = 0; i < xlength; i++) {
		ret[index].x = i;
		ret[index].y = 0;
		index++;
		ret[index].x = i;
		ret[index].y = ylength-1;
		index++;
	}
	for (unsigned i = 1; i < ylength-1; i++) {
		ret[index].x = 0;
		ret[index].y = i;
		index++;
		ret[index].x = xlength-1;
		ret[index].y = i;
		index++;
	}
	return ret;
}

Loc** SquareMesh::matrix(const Loc& start, const Loc& collision) {
	/*if (abs(start.x-collision.x) + abs(start.y-collision.y) != 1) { // Check cells are adjacent
		return NULL;
	}*/
	unsigned fullLength = 2*filterLength+1;

	Loc* positives = new Loc[fullLength * (filterLength)];
	Loc* negatives = new Loc[fullLength * (filterLength+1)];
	Loc** ret = new Loc*[2];

	ret[0]=positives;
	ret[1]=negatives;
	int switchXY = abs(start.y-collision.y) != 1 ? 0 : 1;
	int side  = start.x > collision.x || start.y > collision.y ? -1 : 1;
	for (unsigned i = 0; i < fullLength; i++) {
		for (unsigned j = 0; j < filterLength; j++) {
			int dx = switchXY ? i : side * j;
			int dy = switchXY ? side * j : i;
			positives[filterLength*i+j].x = (collision.x-filterLength+dx) % xlength;
			positives[filterLength*i+j].y = (collision.y-filterLength+dy) % ylength;
		}
	}

	for (unsigned i = 0; i < fullLength; i++) {
		for (unsigned j = 0; j < filterLength+1; j++) {
			int dx = switchXY ? i : side * (j+filterLength);
			int dy = switchXY ? side * (j+filterLength) : i;
			negatives[(filterLength+1)*i+j].x = (collision.x-filterLength+dx) % xlength;
			negatives[(filterLength+1)*i+j].y = (collision.y-filterLength+dy) % ylength;
		}
	}
	return ret;
}