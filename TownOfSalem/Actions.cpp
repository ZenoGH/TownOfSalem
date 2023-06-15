#include <iostream>
#include "Actions.h"

void print_action(std::string, Townie* pTarget, Townie* pCaller);

bool Attack(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			pTarget->bMarkedForDeath = true;
			print_action("attacked", pTarget, pCaller);
			return true;
		}
	}
	return false;
}

bool Protect(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pCaller->bBlocked) {
			pTarget->bProtected = true;
			print_action("protected", pTarget, pCaller);
			return true;
		}
	}
	return false;
}

bool Block(Townie* pTarget, Townie* pCaller) {
	if (pTarget != nullptr and pCaller != nullptr) {
		if (!pTarget->bProtected and !pCaller->bBlocked) {
			pTarget->bBlocked = true;
			print_action("blocked", pTarget, pCaller);
			return true;
		}
	}
	return false;
}

void print_action(std::string action, Townie* pTarget, Townie* pCaller) {
	std::cout << pCaller->role->name << " " << pCaller->name << " " << action << " " << pTarget->role->name << " " << pTarget->name << std::endl;
}