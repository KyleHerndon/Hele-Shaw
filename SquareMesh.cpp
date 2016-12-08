#include <new>
#include <stdlib.h> 
#include <stdio.h>

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

int SquareMesh::value(const Loc& c) {
	return vals[c.x%xlength][c.y%ylength];
}

void SquareMesh::value(const Loc& c, int value) {
	vals[c.x%xlength][c.y%ylength] = value;
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
	locs[0].x = locs[0].x != (int) xlength-1 ? locs[0].x + 1 : 0;
	locs[1].y = locs[1].y != (int) ylength-1 ? locs[1].y + 1 : 0;
	locs[2].x = locs[2].x != 0 ? locs[2].x - 1 : xlength-1;
	locs[3].y = locs[3].y != 0 ? locs[3].y - 1 : ylength-1;
	return locs;
}

Loc** SquareMesh::matrix(const Loc& start, const Loc& collision) {
	if (abs(start.x-collision.x) + abs(start.y-collision.y) != 1) { // Check cells are adjacent
		return NULL;
	}
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