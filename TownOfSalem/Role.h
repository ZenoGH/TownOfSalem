#pragma once
#include <string>
class Townie;

class Action {
public:
	enum { ATTACK, BITE, PROTECT, BLOCK, BRAINWASH };
	inline static Action* Actions[5];
	bool (*pActionFunc)(Townie* pTarget, Townie* pCaller) = nullptr;
	int index = -1;
	bool bHostile = NULL;
	static void SetupActions();
};

class Role {
public:
	std::string name;
	Action* pAction = nullptr;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	bool DoAction(Townie* pTarget, Townie* pCaller);
};

class Faction {
public:
	std::string name;
	//bool bHostile;
	bool bIntel = false;
	Role* pDefaultRole = nullptr;
	//bool (*pAction)(Townie* pTarget, Townie* pCaller);
	//bool DoAction(Townie* pTarget, Townie* pCaller);
};