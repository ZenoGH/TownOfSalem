#pragma once
#include "Town.h"
//class Townie;

bool Attack(Townie* pTarget, Townie* pCaller);
bool Bite(Townie* pTarget, Townie* pCaller);
bool Protect(Townie* pTarget, Townie* pCaller);
bool Block(Townie* pTarget, Townie* pCaller);
void SetupActions();

class Action {
public:
	enum { ATTACK, BITE, PROTECT, BLOCK };
	static Action* Actions[4];
	bool (*pAction)(Townie* pTarget, Townie* pCaller);
	int index = -1;
	bool bHostile = NULL;
	Action();
};