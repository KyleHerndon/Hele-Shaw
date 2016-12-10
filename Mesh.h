struct Loc {
	int x;
	int y;
} typedef Loc;

class Mesh {
	public:
		virtual ~Mesh() {};
		virtual int value(const Loc& c) = 0; // 0 is empty, 1 is occupied, -1 DNE
		virtual void value(const Loc& c, int value) = 0;
		virtual int neighbors() = 0;
		virtual Loc* neighbors(const Loc& c) = 0;
		virtual Loc* edges() = 0;
		virtual Loc** matrix(const Loc& start, const Loc& collision) = 0;
};