#pragma once
#include <stdlib.h>
#include <map>
#include <random>
#include <windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include <vector>
#include "Townie.h"
#include "Actions.h"



class Town {
	public:
		std::map <Role*, Faction*> Roles;
		Faction* Factions[3];
		bool bInAction = true;
		bool bDay = false;
		int iDay = 0;
		bool bAI = true;
		int iSize = 10;
		Townie** Townies;
		std::vector<Townie*> vecTownies;
		Town(int);
		void FillTown();
		void AssignRoles();
		void Think();
		void ClearTown();
		void RemoveTownie(Townie* Townie);
		void CheckTown();
		void PrintTownies();
		static std::string FormatDisplayName(Townie*);
		void DoDay();
		void DoNight();
		void DoVoting();
};