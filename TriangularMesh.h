#include "Mesh.h"
class TriangularMesh : public Mesh {
	public:
		~TriangularMesh();
		TriangularMesh(int length, int filterLength);
		TriangularMesh(const TriangularMesh& mesh);
		int value(const Loc& c); // 0 is empty, 1 is occupied, -1 DNE
		void value(const Loc& c, int value);
		void output(ifstream output);
		int valid(const Loc& c);
		int neighbors();
		Loc* neighbors(const Loc& c);
		Loc* edges();
		Loc** matrix(const Loc& start, const Loc& collision);
	private:
		int** vals;
		int length;
		int filterLength;
};