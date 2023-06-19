#pragma once
#include <string>
class Townie;

class Action {
public:
	enum { ATTACK, BITE, PROTECT, BLOCK };
	inline static Action* Actions[4];
	bool (*pAction)(Townie* pTarget, Townie* pCaller);
	int index = -1;
	bool bHostile = NULL;
	void SetupActions();
};

class Role {
public:
	std::string name;
	bool bHostile;
	bool (*pAction)(Townie* pTarget, Townie* pCaller);
	bool DoAction(Townie* pTarget, Townie* pCaller);
};

class Faction {
public:
	std::string name;
	//bool bHostile;
	bool bIntel = false;
	Role* default_role = nullptr;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	//bool DoAction(Townie* pTarget, Townie* pCaller);
};