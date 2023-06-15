#include "Role.h"

bool Role::DoAction(Townie* pTarget, Townie* pCaller) {
	if (pAction != nullptr) {
		return pAction(pTarget, pCaller);
	}
}