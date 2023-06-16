#pragma once
#include <stdlib.h>
#include <map>
#include <random>
#include <windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Townie.h"
#include "Actions.h"



class Town {
	public:
		std::map <Role*, Faction*> Roles;
		Faction* Factions[3];
		bool bInAction = true;
		bool bDay = false;
		bool bAI = true;
		int iSize = 15;
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