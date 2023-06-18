class Townie;



class Visit {
public:
	enum visit_types { NORMAL, ASTRAL };
	int type = NORMAL;
	Townie* Visitor;
	Townie* Visited;
	bool bHostile;
	bool (*pAction)(Townie* pTarget, Townie* pCaller);
};