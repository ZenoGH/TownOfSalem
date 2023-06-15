#pragma once
#include <stdlib.h>
#include <map>
#include <random>
#include <windows.h>
#include <string>
#include <iostream>
#include "Townie.h"
#include "Actions.h"



class Town {
	public:
		std::map <const char*, bool(*)(Townie*, Townie*)> Roles;
		bool bInAction = true;
		bool bDay = false;
		bool bAI = true;
		int iSize = 5;
		Townie** Townies = new Townie* [iSize];
		explicit Town();
		void FillTown();
		void AssignRoles();
		void Think();
		void ClearTown();
		void RemoveTownie(Townie* Townie);
		void CheckTown();
		void DoDay();
		void DoNight();
		void DoVoting();
};