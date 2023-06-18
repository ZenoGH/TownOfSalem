#pragma once
#include <string>
#include "Role.h"
class Townie {
public:
	std::string name;
	//const char* name;
	Role* role = nullptr;
	Faction* faction = nullptr;
	bool bAlive = true;
	bool bProtected = false;
	bool bBlocked = false;
	bool bMarkedForDeath = false;
	bool bBitten = false;
	int iVotes = 0;
	int iNightsToLive = -1;
	int index;
	bool DoAction(Townie* pTarget);
};