#include "Mesh.h"
class SquareMesh : public Mesh {
	public:
		~SquareMesh();
		SquareMesh(unsigned xlength, unsigned ylength, unsigned filterLength);
		int value(const Loc& c); // 0 is empty, 1 is occupied, -1 DNE
		void value(const Loc& c, int value);
		int neighbors();
		Loc* neighbors(const Loc& c);
		Loc** matrix(const Loc& start, const Loc& collision);
	private:
		int** vals;
		unsigned xlength;
		unsigned ylength;
		unsigned filterLength;
};