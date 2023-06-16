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
	print_action("attacked", pTarget, pCaller, false);
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
	print_action("bit", pTarget, pCaller, false);
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
	print_action("protected", pTarget, pCaller, false);
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
	print_action("blocked", pTarget, pCaller, false);
	return false;
}

void print_action(std::string action, Townie* pTarget, Townie* pCaller, bool success) {
	if (success) {
		std::cout << pCaller->role->name << " " << pCaller->name << " " << action << " " << pTarget->role->name << " " << pTarget->name << std::endl;
	}
	else
	{
		std::cout << pCaller->role->name << " " << pCaller->name << " " << action << " " << pTarget->role->name << " " << pTarget->name << ", but failed!" << std::endl;
	}
}