#include "Town.h"
#include "Actions.h"
#include "OtherFunctions.h"
#include "resource.h"
enum {SKLRS, ZOMBIES, TOWN, ELITE};
Town::Town(int Size, bool Custom) {
	bCustom = Custom;
	iSize = Size;
	Townies = new Townie * [iSize];
	Visits = new Visit * [iSize];
	srand(time(NULL));
	FillTown();
	AssignRoles();
	while (bInAction) {
		Think();
		Sleep(5000);
	}
}
void Town::FillTown() {
	for (int i = 0; i < iSize; i++) {
		Townies[i] = new Townie;
		Visits[i] = new Visit;
		Townies[i]->index = i;
		Townies[i]->pTown = this;
		Townies[i]->name = wstringToString(get_random_line_rc(IDR_TXT1));
	}
}

void Town::AssignRoles() {
	Action::SetupActions();
	Role* role = new Role;
	Faction* faction = new Faction; // Serial Killers
	Factions[SKLRS] = faction;
	role->name = "Serial Killer";
	role->pAction = Action::Actions[Action::ATTACK];
	//role->bHostile = true;
	faction->name = "Serial Killers";
	faction->pDefaultRole = role;
	Roles.insert({ role, faction });

	role = new Role;
	faction = new Faction; // The Elite
	Factions[ELITE] = faction;
	role->name = "Elite";
	role->pAction = Action::Actions[Action::BRAINWASH];
	faction->name = "The Upper Class";
	faction->pDefaultRole = role;
	faction->bIntel = true;
	Roles.insert({ role, faction });

	role = new Role;
	faction = new Faction; // Zombies
	Factions[ZOMBIES] = faction;
	role->name = "Zombie";
	role->pAction = Action::Actions[Action::BITE];
	faction->name = "Zombies";
	faction->pDefaultRole = role;
	faction->bIntel = true;
	Roles.insert({ role, faction });


	role = new Role;
	faction = new Faction; //Town
	Factions[TOWN] = faction;
	role->name = "Clown";
	role->pAction = Action::Actions[Action::BLOCK];
	faction->name = "Town";
	faction->pDefaultRole = role;
	Roles.insert({ role, faction });


	role = new Role;
	role->name = "Bodyguard";
	role->pAction = Action::Actions[Action::PROTECT];
	faction->name = "Town";
	Roles.insert({ role, faction });


	role = new Role;
	role->name = "Vigilante";
	role->pAction = Action::Actions[Action::ATTACK];
	faction->name = "Town";
	Roles.insert({ role, faction });

	role = new Role;
	role->name = "Civilian";
	role->pAction = nullptr;
	faction->name = "Town";
	Roles.insert({ role, faction });


	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	if (!bCustom) {
		for (int i = 0; i < iSize; i++) {
			// Generate a random index
			std::uniform_int_distribution<> dist(0, Roles.size() - 1);
			auto randIndex = dist(gen);

			// Advance an iterator to the random index
			auto it = std::next(Roles.begin(), randIndex);
			Townies[i]->pRole = it->first;
			Townies[i]->pFaction = Roles[it->first];
			if (Townies[i]->pRole->pAction == nullptr) {
				Townies[i]->pRole->pAction = new Action;
			}
			vecTownies.push_back(Townies[i]);
			//Townies[i]->role = &role;
		}
	}
	else {
		int index;
		std::cout << "Available roles and their factions: " << std::endl;
		for (int i = 0; i < Roles.size(); i++) {
			auto it = std::next(Roles.begin(), i);
			std::cout << i << ")" << it->first->name << " " << "(" << Roles[it->first]->name << ")" << std::endl;
		}
		std::cout << "Usage: 'Townie x: (role number)" << std::endl;
		for (int i = 0; i < iSize; i++) {
			std::cout << "Townie " << i + 1 << ": ";
			std::cin >> index;
			auto it = std::next(Roles.begin(), index);
			Townies[i]->pRole = it->first;
			Townies[i]->pFaction = Roles[it->first];
		}
	}
}

void Town::Think() {
	if (bDay) {
		DoDay();
		PrintTownies();
		CheckTown();
		Sleep(5000);
		DoVoting();
		Sleep(1000);
		CheckTown();
	}
	else {
		CheckTown();
		DoNight();
		iDay++;
	}
	bDay = !bDay;
}

void Town::DoDay() {
	std::cout << "Day " << iDay << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (!Townies[i]->bAlive) {
			continue;
		}
		if (Townies[i]->bMarkedForDeath and !Townies[i]->bProtected and !Townies[i]->bBitten) {
			Townies[i]->bAlive = false;
			std::cout << FormatDisplayName(Townies[i]) << " was killed." << std::endl;
		}
		else if (Townies[i]->bBitten and !Townies[i]->bProtected) {
			//auto it = std::next(Roles.begin(), ZOMBIES+1);
			Townies[i]->pRole = Factions[ZOMBIES]->pDefaultRole;
			Townies[i]->pFaction = Factions[ZOMBIES];
			//std::cout << Townies[i]->role->name << " " << Townies[i]->name << " was bitten." << std::endl;
		}
		else if (Townies[i]->bMarkedForDeath or Townies[i]->bBitten) {
			std::cout << FormatDisplayName(Townies[i]) << " was attacked, but got protected!" << std::endl;
		}
		if (Townies[i]->bBrainwashed and !Townies[i]->bProtected) {
			Townies[i]->pFaction = Factions[ELITE];
		}
		if (Townies[i]->bAlive and Townies[i]->pRole == Factions[ZOMBIES]->pDefaultRole) {
			if (Townies[i]->iNightsToLive < 0) {
				Townies[i]->iNightsToLive = 2;
			}
		}
		if (Townies[i]->bAlive and Townies[i]->iNightsToLive > 0) {
			Townies[i]->iNightsToLive--;
		}
		if (Townies[i]->bAlive and Townies[i]->iNightsToLive == 0) {
			Townies[i]->bAlive = false;
			std::cout << FormatDisplayName(Townies[i]) << " has expired." << std::endl;
		}
		Townies[i]->bBlocked = false;
		Townies[i]->bMarkedForDeath = false;
		Townies[i]->bBitten = false;
		Townies[i]->bProtected = false;
		Townies[i]->bBrainwashed = false;
		Townies[i]->iVotes = 0;
		
	}
}

void Town::DoNight() {
	std::cout << "Night " << iDay << std::endl;
	if (bAI) {
		for (int i = 0; i < iSize; i++) {
			Townie* Doer = Townies[i];
			if (Doer->pRole->pAction->pActionFunc != &Block or !Doer->bAlive) {
				continue;
			}
			Townie* Target = Townies[rand() % iSize];
			while (Target == Doer or !Target->bAlive) {
				Target = Townies[rand() % iSize];
			}
			Doer->DoVisit(Target);
		}
		for (int i = 0; i < iSize; i++) {
			Townie* Doer = Townies[i];
			if (Doer->pRole->pAction->pActionFunc == &Block or !Doer->bAlive) {
				continue;
			}
			Townie* Target = Townies[rand() % iSize];

			while (!Target->bAlive or Target == Doer or (Doer->pFaction == Target->pFaction and Doer->pFaction->bIntel and Doer->pRole->pAction->bHostile) or (Doer->pFaction != Target->pFaction and Doer->pFaction->bIntel and !Doer->pRole->pAction->bHostile)) {
				Target = Townies[rand() % iSize];
			}
			while (Target == Doer or !Target->bAlive) {
				Target = Townies[rand() % iSize];
			}
			Doer->DoVisit(Target);
		}
	}
}

void Town::DoVoting() {
	std::cout << "Voting" << std::endl;
	if (bAI) {
		for (int i = 0; i < iSize; i++) {
			Townie* Doer = Townies[i];
			if (!Doer->bAlive) {
				continue;
			}
			if (Doer->pFaction == Factions[ELITE]) {
				Doer->iVotingPower++;
			}
			Townie* Target = Townies[rand() % iSize];
			while (Target == Doer or !Target->bAlive or Target->pFaction == Doer->pFaction and Doer->pFaction->bIntel) {
				Target = Townies[rand() % iSize];
			}
			std::cout << FormatDisplayName(Doer) << " voted for " << FormatDisplayName(Target) << std::endl;
			Target->iVotes = Target->iVotes + Doer->iVotingPower;
		}
		int Alives = 0;
		for (int i = 0; i < iSize; i++) {
			if (Townies[i]->bAlive) {
				Alives++;
			}
		}
		Townie* MostVotes = nullptr;
		int EqualVotes = 0;
		for (int i = 0; i < iSize; i++) {
			if (MostVotes == nullptr or Townies[i]->iVotes > MostVotes->iVotes) {
				MostVotes = MostVotes = Townies[i];
			}
			else if (Townies[i]->iVotes == MostVotes->iVotes) {
				EqualVotes = MostVotes->iVotes;
			}
		}
		Sleep(1000);
		std::cout << "Voting results:" << std::endl;
		if (MostVotes != nullptr and MostVotes->iVotes >= Alives / 2 and MostVotes->iVotes > EqualVotes) {
			MostVotes->bAlive = false;
			std::cout << FormatDisplayName(MostVotes) << " was voted out." << " (" << MostVotes->iVotes << " votes, " <<(double)MostVotes->iVotes / (double)Alives * 100 << "%)" << std::endl;
		}
		else {
			std::cout << "Not enough votes." << std::endl;
		}
	}
}

void Town::CheckTown() {
	Faction* WinningFaction = nullptr;
	bool bDifFactions = false;
	int Alives = 0;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			Alives++;
			for (int j = 0; j < iSize; j++) {
				if (Townies[j]->bAlive and Townies[i]->pFaction != Townies[j]->pFaction) {
					bDifFactions = true;
				}
			}
		}
	}
		if (!bDifFactions) {
			bInAction = false;
			for (int i = 0; i < iSize; i++) {
				if (Townies[i]->bAlive) {
					std::cout << FormatDisplayName(Townies[i]) << " wins!" << std::endl;
					WinningFaction = Townies[i]->pFaction;
				}
			}
			if (Alives < 1) {
				std::cout << "Draw." << std::endl;
				exit(0);
			}
			else {
				std::cout << WinningFaction->name << " win!" << std::endl;
				exit(0);
			}
		}
}

void Town::PrintTownies() {
	int Alives = 0;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			Alives++;
		}
	}
	std::cout << "Alive Townies:" << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			std::cout << FormatDisplayName(Townies[i]) << std::endl;
		}
	}
	std::cout << "Total: " << Alives << std::endl;
}

void Town::ProcessVisits(Visit* visit) {
	for (int i = 0; i < iSize; i++) {
		Visit* visit = Visits[i];
		if (visit->pAction->Actions[Action::BLOCK]) {
			Visits[visit->pTarget->index]->bBlocked = true;
		}
	}
}

std::string Town::FormatDisplayName(Townie* pTownie) {
	return "(" + std::to_string(pTownie->index + 1) + ") " + "[" + pTownie->pFaction->name + "] (" + pTownie->pRole->name + ") " + pTownie->name;
}



void Town::ClearTown() {
	for (int i = 0; i < iSize; i++) {
		if (Townies[i] != nullptr) {
			delete Townies[i];
		}
	}
	delete[] Townies;
}

void Town::RemoveTownie(Townie* pTownie) {
	delete pTownie;
}