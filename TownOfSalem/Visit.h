#pragma once
//#include "Townie.h"
class Townie;
class Action;

class Visit {
	public:
		bool bHostile = false;
		bool bBlocked = false;
		Townie* pVisitor = nullptr;
		Townie* pTarget = nullptr;
		Action* pAction = nullptr;
};