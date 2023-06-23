#include "Townie.h"

#include "Town.h"

bool Townie::DoVisit(Townie* pTarget) {
	Visit* pVisit = this->pTown->Visits[this->index];
	pVisit->pTarget = pTarget;
	pVisit->pVisitor = this;
	pVisit->bHostile = pRole->pAction->bHostile;


	return pRole->DoAction(pTarget, this);
}