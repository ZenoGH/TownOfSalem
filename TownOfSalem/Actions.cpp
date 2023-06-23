#include <iostream>
#include "Actions.h"

void print_action(std::string, Townie* pTarget, Townie* pCaller, bool);

bool Attack(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			pTarget->bMarkedForDeath = true;
			print_action("attacked", pTarget, pCaller, true);
			return true;
		}
	}
	print_action("attack", pTarget, pCaller, false);
	return false;
}

bool Bite(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			pTarget->bBitten = true;
			print_action("bit", pTarget, pCaller, true);
			return true;
		}
	}
	print_action("bite", pTarget, pCaller, false);
	return false;
}

bool Protect(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pCaller->bBlocked) {
			pTarget->bProtected = true;
			print_action("protected", pTarget, pCaller, true);
			return true;
		}
	}
	print_action("protect", pTarget, pCaller, false);
	return false;
}

bool Block(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			pTarget->bBlocked = true;
			print_action("blocked", pTarget, pCaller, true);
			return true;
		}
	}
	print_action("block", pTarget, pCaller, false);
	return false;
}

bool Brainwash(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			//pTarget->faction = pCaller->faction;
			pTarget->bBrainwashed = true;
			print_action("paid off", pTarget, pCaller, true);
			return true;
		}
	}
	print_action("pay off", pTarget, pCaller, false);
	return false;
}



//void SetupActions() {
//	Action* action = new Action;
//	action->index = Action::ATTACK;
//	action->pActionFunc = &Attack;
//	action->bHostile = true;
//		
//	action = new Action;
//	action->index = Action::BITE;
//	action->pActionFunc = &Bite;
//	action->bHostile = true;
//
//	action = new Action;
//	action->index = Action::PROTECT;
//	action->pActionFunc = &Protect;
//	action->bHostile = true;
//
//	action = new Action;
//	action->index = Action::BLOCK;
//	action->pActionFunc = &Block;
//	action->bHostile = true;
//
//	action = new Action;
//	action->index = Action::BRAINWASH;
//	action->pActionFunc = &Brainwash;
//	action->bHostile = true;
//}

void print_action(std::string action, Townie* pTarget, Townie* pCaller, bool success) {
	if (success) {
		std::cout << Town::FormatDisplayName(pCaller) << " " << action << " " << Town::FormatDisplayName(pTarget) << std::endl;
	}
	else
	{
		std::cout << Town::FormatDisplayName(pCaller) << " tried to " << action << " " << Town::FormatDisplayName(pTarget) << ", but failed!" << std::endl;
	}
}