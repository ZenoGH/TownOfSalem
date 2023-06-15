#pragma once
#include <string>
#include "Role.h"
class Townie {
public:
	std::string name;
	//const char* name;
	Role* role = nullptr;
	bool bAlive = true;
	bool bProtected = false;
	bool bBlocked = false;
	bool bMarkedForDeath = false;
	bool iVotes = 0;
	bool DoAction(Townie* pTarget);
};