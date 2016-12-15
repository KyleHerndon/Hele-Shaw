CC = g++
CFLAGS = -Wall -std=c++11

all: simulation

simulation: SquareMesh.o main.o
	$(CC) $(CFLAGS) -o simulation SquareMesh.o main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

SquareMesh.o: SquareMesh.cpp SquareMesh.h Mesh.h
	$(CC) $(CFLAGS) -c SquareMesh.cpp

TriangularMesh.o: TriangularMesh.cpp TriangularMesh.h Mesh.h
	$(CC) $(CFLAGS) -c TriangularMesh.cpp

clean:
	rm *.o