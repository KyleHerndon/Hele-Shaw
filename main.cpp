#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>

#include "SquareMesh.h"

#define simStart 16 // keep at powers of 2
int simSize = 256; // keep at powers of 2
int filterSize = 2; // edges of length 2*filterSize+1
double A = 1.0;
double B = 0.5;
double C = 0.0;

static int nl(Mesh* mesh, const Loc& start, const Loc& end) {
	int ret = 0;
	Loc** locs = mesh->matrix(start, end);
	for (int i = 0; i < (filterSize+1)*(filterSize*2+1); i++) {
		ret += (mesh->value(locs[0][i]) > 0);
		//ret -= (mesh->value(locs[0][i]) == 0);
	}
	for (int i = 0; i < filterSize*(filterSize*2+1); i++) {
		//ret += (mesh->value(locs[1][i]) == 0);
		ret += (mesh->value(locs[1][i]) > 0);
	}
	delete[] locs[0];
	delete[] locs[1];
	delete[] locs;

	return ret;
}

int main(int argc, char* argv[]) {
	if (argc != 8) {
		std::cout << "usage: " << argv[0] << " <filename> <simSize> <filterSize> <A> <B> <C> <seed>\n";
		exit(1);
	}
	std::ofstream output(argv[1]);
	simSize = std::stoul(argv[2]);
	filterSize = std::stoul(argv[3]);
	A = std::stod(argv[4]);
	B = std::stod(argv[5]);
	C = std::stod(argv[6]);
	unsigned seed = std::stoul(argv[7]);	
	srand(seed);
	int currentSize = simStart;
	SquareMesh* mesh;
	int collisions = 1;
	while(currentSize <= simSize) {
		if (currentSize == simStart) {
			mesh = new SquareMesh(simStart,simStart,filterSize);
		} else {
			//mesh->output();
			Mesh* prev = mesh;
			mesh = new SquareMesh(*mesh);
			delete prev;
		}
		int onEdge = 0;
		Loc* edges = mesh->edges();
		int edgesLength = currentSize*4-4;
		while (!onEdge) { // Simulation Loop
			Loc walker;
			walker = edges[rand()%edgesLength];
			int collided = 0;
			while (!collided) { // Randomly move walker
				Loc* neighbors = mesh->neighbors(walker);
				Loc next = neighbors[rand() % mesh->neighbors()];
				delete[] neighbors;
				if (mesh->value(next) > 0) { // possible collision
					double proportion = nl(mesh, walker, next)/(1.0*(filterSize*2+1)*(filterSize*2+1));
					double newtonian = B+A * (proportion - (filterSize/(filterSize*2.0+1.0)));
					double nonnewtonian = 1.+C*sqrt(collisions)/(1+collisions-mesh->value(walker));
					double val = newtonian * nonnewtonian;
					val = val > 0.01 ? val : .01;
					//double val2 = (A * (nl(mesh, walker, next)/(1.0*(filterSize*2+1)*(filterSize*2+1)) - (filterSize/(filterSize*2.0+1.0))) + B)*(1.+C*sqrt(collisions)/(1+collisions-mesh->value(walker)));				
					//printf("val:%f\n", val);
					if (mesh->valid(walker) && val > (rand()*1.0/RAND_MAX)) { // probability
						collisions++;
						mesh->value(walker, collisions);
						collided = 1; // breaks loop
						if (walker.x < 2 || walker.y < 2 || walker.x > currentSize-3 || walker.y > currentSize-3) {
							onEdge = 1;
						}
					}
				} else {
					walker = next;
				}
			}
			//printf("currentSize:%d, collisions: %d\n", currentSize, collisions);
		}
		currentSize *= 2;
		delete[] edges;
	}
	mesh->output(output);
	delete mesh;
	return 0;
}
