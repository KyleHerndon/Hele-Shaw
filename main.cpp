#include <stdio.h>
#include <stdlib.h>

#include "SquareMesh.h"

#define simSize 128
#define filterSize 2 // edges of length 2*filterSize+1
#define A 1
#define B 0.5

static int nl(Mesh* mesh, const Loc& start, const Loc& end) {
	int ret = 0;
	Loc** locs = mesh->matrix(start, end);
	for (int i = 0; i < filterSize*(filterSize*2+1); i++) {
		ret += (mesh->value(locs[0][i]) > 0);
	}
	for (int i = 0; i < filterSize*(filterSize*2+1); i++) {
		ret += (mesh->value(locs[1][i]) == 0);
	}
	delete locs[0];
	delete locs[1];
	delete locs;
	return ret;
}

int main() {
	Mesh* mesh = new SquareMesh(simSize,simSize,filterSize);
	int collisions = 0;
	int onEdge = 0;
	while (!onEdge) { // Simulation Loop
		Loc walker;
		walker.x = rand()%simSize;
		walker.y = rand()%simSize;
		int collided = 0;
		while (!collided) { // Randomly move walker
			Loc* neighbors = mesh->neighbors(walker);
			Loc next = neighbors[rand() % mesh->neighbors()];
			delete neighbors;
			if (mesh->value(next) > 0) { // possible collision
				if (A * (nl(mesh, walker, next)/(1.0*(filterSize*2+1)*(filterSize*2+1)) - (filterSize/(filterSize*2.0+1.0))) + B > (rand()*1.0/RAND_MAX)) { // probability
					mesh->value(walker, collisions);
					collisions++;
					collided = 1; // breaks loop
					if (walker.x == 0 || walker.y == 0 || walker.x == simSize-1 || walker.y == simSize-1) {
						onEdge = 1;
					}
				}
			} else {
				walker = next;
			}
		}
		printf("%d\n", collisions);
	}
	return 0;
}