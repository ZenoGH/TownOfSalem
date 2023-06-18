#pragma once
#include <string>
class Townie;

class Role {
public:
	std::string name;
	Action* action;
	//bool bHostile;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	bool DoAction(Townie* pTarget, Townie* pCaller);
};

class Faction {
public:
	std::string name;
	//bool bHostile;
	bool bIntel = false;
	Role* default_role = nullptr;
	int index;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	//bool DoAction(Townie* pTarget, Townie* pCaller);
};