#include "Role.h"

bool Role::DoAction(Townie* pTarget, Townie* pCaller) {
	if (action->pAction != nullptr) {
		return action->pAction(pTarget, pCaller);
	}
}