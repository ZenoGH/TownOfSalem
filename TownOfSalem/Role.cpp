#include "Role.h"
#include "Actions.h"

bool Role::DoAction(Townie* pTarget, Townie* pCaller) {
	if (action->pAction != nullptr) {
		return action->pAction(pTarget, pCaller);
	}
	return false;
}

void Action::SetupActions() {
	Action* action = new Action;
	action->index = Action::ATTACK;
	action->pAction = &Attack;
	action->bHostile = true;
	Action::Actions[Action::ATTACK] = action;

	action = new Action;
	action->index = Action::BITE;
	action->pAction = &Bite;
	action->bHostile = true;
	Action::Actions[Action::BITE] = action;

	action = new Action;
	action->index = Action::PROTECT;
	action->pAction = &Protect;
	action->bHostile = true;
	Action::Actions[Action::PROTECT] = action;

	action = new Action;
	action->index = Action::BLOCK;
	action->pAction = &Block;
	action->bHostile = true;
	Action::Actions[Action::BLOCK] = action;

	action = new Action;
	action->index = Action::BRAINWASH;
	action->pAction = &Brainwash;
	action->bHostile = true;
	Action::Actions[Action::BRAINWASH] = action;

}