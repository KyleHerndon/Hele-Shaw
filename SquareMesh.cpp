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
		this->vals[i] = new int[this->xlength];
		if (i > this->ylength/4 && i < (3 * this->ylength)/4) {
			std::memcpy(&this->vals[i][this->xlength/4],mesh.vals[i],sizeof(Loc)*mesh.xlength);
		}
	}
}

int SquareMesh::value(const Loc& c) {
	return vals[c.x%xlength][c.y%ylength];
}

void SquareMesh::value(const Loc& c, int value) {
	vals[c.x%xlength][c.y%ylength] = value;
}

int SquareMesh::holeless(const Loc& c) {
	int adjacent = 0;
	Loc* neighbors = SquareMesh::neighbors(c);
	for (int i = 0; i < 4; i++) {
		adjacent += SquareMesh::value(neighbors[i]) > 0 ? 1 : 0;
	}
	delete neighbors;
	if (adjacent == 3) {
		return 1;
	} else if (adjacent == 2) {
		return 0;
	}
	if (vals[(c.x-1)%xlength][(c.y-1)%ylength] || vals[(c.x-1)%xlength][(c.y+1)%ylength] || vals[(c.x+1)%xlength][(c.y-1)%ylength] || vals[(c.x+1)%xlength][(c.y+1)%ylength]) { // if any diagonal is occupied
		return 0;
	}
	return 1;
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