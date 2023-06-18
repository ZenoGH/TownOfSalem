#include "Town.h"
#include "Actions.h"
#include "OtherFunctions.h"
#include "resource.h"

Town::Town(int Size) {
	iSize = Size;
	Townies = new Townie * [iSize];
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
	}

	for (int i = 0; i < iSize; i++) {
		Townies[i]->name = wstringToString(get_random_line_rc(IDR_TXT1));// +L" " + get_random_line_rc(IDR_TXT2));
		Townies[i]->index = i;
		//Townies[i] = &townie;
	}
}

void Town::AssignRoles() {
	Role* role = new Role;
	Faction* faction = new Faction; // Serial Killers
	Factions[SKLRS] = faction;
	role->name = "Serial Killer";
	role->action = Action::Actions[Action::ATTACK];
	faction->name = "Serial Killers";
	faction->default_role = role;
	faction->index = SKLRS;
	Roles.insert({ role, faction });

	role = new Role;
	faction = new Faction; // Zombies
	Factions[ZOMBIES] = faction;
	role->name = "Zombie";
	Action::Actions[Action::BITE];
	faction->name = "Zombies";
	faction->default_role = role;
	faction->bIntel = true;
	faction->index = ZOMBIES;
	Roles.insert({ role, faction });


	role = new Role;
	faction = new Faction; //Town
	Factions[TOWN] = faction;
	role->name = "Clown";
	Action::Actions[Action::BLOCK];
	faction->name = "Town";
	faction->default_role = role;
	faction->index = TOWN;

	Roles.insert({ role, faction });

	role = new Role;
	role->name = "Bodyguard";
	Action::Actions[Action::PROTECT];
	faction->name = "Town";
	
	Roles.insert({ role, faction });

	role = new Role;
	role->name = "Vigilante";
	Action::Actions[Action::ATTACK];
	faction->name = "Town";
	
	Roles.insert({ role, faction });

	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());


	for (int i = 0; i < iSize; i++) {
		// Generate a random index
		std::uniform_int_distribution<> dist(0, Roles.size() - 1);
		auto randIndex = dist(gen);

		// Advance an iterator to the random index
		auto it = std::next(Roles.begin(), randIndex);
		Townies[i]->role = it->first;
		Townies[i]->faction = Roles[it->first];
		vecTownies.push_back(Townies[i]);
		//Townies[i]->role = &role;
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
	}
	bDay = !bDay;
}

void Town::DoDay() {
	std::cout << "Day" << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bMarkedForDeath and !Townies[i]->bProtected) {
			Townies[i]->bAlive = false;
			std::cout << FormatDisplayName(Townies[i]) << " was killed." << std::endl;
		}
		else if (Townies[i]->bBitten and !Townies[i]->bProtected) {
			//auto it = std::next(Roles.begin(), ZOMBIES+1);
			Townies[i]->role = Factions[ZOMBIES]->default_role;
			Townies[i]->faction = Factions[ZOMBIES];
			//std::cout << Townies[i]->role->name << " " << Townies[i]->name << " was bitten." << std::endl;
		}
		else if (Townies[i]->bMarkedForDeath or Townies[i]->bBitten) {
			std::cout << FormatDisplayName(Townies[i]) << " was attacked, but got protected!" << std::endl;
		}
		if (Townies[i]->bAlive and Townies[i]->role == Factions[ZOMBIES]->default_role) {
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
		Townies[i]->iVotes = 0;
	}
}


void Town::DoNight() {
	std::cout << "Night" << std::endl;
	if (bAI) {
		for (int i = 0; i < iSize; i++) {
			Townie* Doer = Townies[i];
			if (Doer->role->action->pAction != &Block or !Doer->bAlive) {
				continue;
			}
			Townie* Target = Townies[rand() % iSize];
			while (Target == Doer or !Target->bAlive) {
				Target = Townies[rand() % iSize];
			}
			Doer->DoAction(Target);
		}
		for (int i = 0; i < iSize; i++) {
			Townie* Doer = Townies[i];
			if (Doer->role->action->pAction == &Block or !Doer->bAlive) {
				continue;
			}
			Townie* Target = Townies[rand() % iSize];

			while (!Target->bAlive or Target == Doer or (Doer->faction == Target->faction and Doer->faction->bIntel and Doer->role->action->bHostile)) {
				Target = Townies[rand() % iSize];
			}
			while (Target == Doer or !Target->bAlive) {
				Target = Townies[rand() % iSize];
			}
			Doer->DoAction(Target);
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
			Townie* Target = Townies[rand() % iSize];
			while (Target == Doer or !Target->bAlive or Target->faction == Doer->faction and Doer->faction->bIntel) {
				Target = Townies[rand() % iSize];
			}
			std::cout << FormatDisplayName(Doer) << " voted for " << FormatDisplayName(Target) << std::endl;
			Target->iVotes++;
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
				if (Townies[j]->bAlive and Townies[i]->faction != Townies[j]->faction) {
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
					WinningFaction = Townies[i]->faction;
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
			std::cout << i + 1 << ") " << FormatDisplayName(Townies[i]) << std::endl;
		}
	}
	std::cout << "Total: " << Alives << std::endl;
}

std::string Town::FormatDisplayName(Townie* townie) {
	return "[" + townie->faction->name + "] (" + townie->role->name + ") " + townie->name;
}

void Town::ClearTown() {
	for (int i = 0; i < iSize; i++) {
		if (Townies[i] != nullptr) {
			delete Townies[i];
		}
	}
	delete[] Townies;
}

void Town::RemoveTownie(Townie* Townie) {
	delete Townie;
}