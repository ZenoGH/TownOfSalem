#include "Townie.h"

bool Townie::DoAction(Townie* pTarget) {
	return role->DoAction(pTarget, this);
}