#pragma once
#include <string>
class Townie;
class Role {
public:
	std::string name;
	//bool bHostile;
	bool (*pAction)(Townie* pTarget, Townie* pCaller);
	bool DoAction(Townie* pTarget, Townie* pCaller);
};

class Faction {
public:
	std::string name;
	bool bHostile;
	bool bIntel = false;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	//bool DoAction(Townie* pTarget, Townie* pCaller);
};