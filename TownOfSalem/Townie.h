#pragma once
#include <string>
#include "Role.h"

class Town;

class Townie {
public:
	std::string name;
	//const char* name;
	Role* pRole = nullptr;
	Faction* pFaction = nullptr;
	Town* pTown = nullptr;
	//Visit* pVisit = nullptr;
	bool bAlive = true;
	bool bProtected = false;
	bool bBlocked = false;
	bool bMarkedForDeath = false;
	bool bBitten = false;
	bool bBrainwashed = false;
	int iVotes = 0;
	int iVotingPower = 1;
	int iNightsToLive = -1;
	int index = 0;
	bool DoVisit(Townie* pTarget);
};