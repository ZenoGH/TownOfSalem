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
#include "Visit.h"


class Town {
	public:
		std::map <Role*, Faction*> Roles;
		Faction* Factions[4];
		enum { NEUTRALS, ZOMBIES, TOWN, ELITE };
		bool bInAction = true;
		bool bDay = false;
		bool bCustom;
		int iDay = 0;
		bool bAI = true;
		int iSize = 10;
		Townie** Townies;
		Visit** Visits;
		std::vector<Townie*> vecTownies;
		Town(int, bool);
		void FillTown();
		void AssignRoles();
		void Think();
		void ClearTown();
		void RemoveTownie(Townie* Townie);
		void CheckTown();
		void PrintTownies();
		void ProcessVisits(Visit* visit);
		static std::string FormatDisplayName(Townie*);
		void DoDay();
		void DoNight();
		void DoVoting();
};