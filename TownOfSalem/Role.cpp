#include "Role.h"
#include "Actions.h"

bool Role::DoAction(Townie* pTarget, Townie* pCaller) {
	if (pAction->pActionFunc != nullptr) {
		return pAction->pActionFunc(pTarget, pCaller);
	}
	return false;
}

void Action::SetupActions() {
	Action* action = new Action;
	action->index = Action::ATTACK;
	action->pActionFunc = &Attack;
	action->bHostile = true;
	Action::Actions[Action::ATTACK] = action;

	action = new Action;
	action->index = Action::BITE;
	action->pActionFunc = &Bite;
	action->bHostile = true;
	Action::Actions[Action::BITE] = action;

	action = new Action;
	action->index = Action::PROTECT;
	action->pActionFunc = &Protect;
	action->bHostile = false;
	Action::Actions[Action::PROTECT] = action;

	action = new Action;
	action->index = Action::BLOCK;
	action->pActionFunc = &Block;
	action->bHostile = true;
	Action::Actions[Action::BLOCK] = action;

	action = new Action;
	action->index = Action::BRAINWASH;
	action->pActionFunc = &Brainwash;
	action->bHostile = true;
	Action::Actions[Action::BRAINWASH] = action;

}