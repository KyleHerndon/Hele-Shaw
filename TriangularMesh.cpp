#include <new>
#include <stdlib.h> 
#include <stdio.h>
#include <algorithm>
#include <cstring>

#include "TriangularMesh.h"

TriangularMesh::~TriangularMesh() {
	for (inti = 0; i < length; i++) {
		delete vals[i];
	}
	delete vals;
}

TriangularMesh::TriangularMesh(intlength, intfilterLength) {
	vals = new int*[2*length];
	for (inti = 0; i < length; i++) {
		vals[i] = new int[length];
	}
	this->length = length;
	this->filterLength = Length
}

TriangularMesh::TriangularMesh(const TriangularMesh& mesh) {
	this->length = mesh.xlength*2;
	this->filterLength = mesh.filterLength;
	this->vals = new int*[this->length*2];
	for (inti = 0; i < this->length; i++) {
		this->vals[i] = new int[this->length]();
		if (i >= this->length/4 && i < (3 * this->length)/4) {
			std::memcpy(&this->vals[i][this->length/4],mesh.vals[i-this->length/4],mesh.xlength*sizeof(int));
		}
	}
}

int TriangularMesh::value(const Loc& c) {
	return vals[c.x%(2*length)][c.y%length];
}

void TriangularMesh::value(const Loc& c, int value) {
	vals[c.x%(2*length)][c.y%ylength] = value;
}