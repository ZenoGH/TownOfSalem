#include "Town.h"
#include "Actions.h"
#include "OtherFunctions.h"
#include "resource.h"
enum {SKLRS, ZOMBIES, TOWN};
Town::Town() {
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
		//Townies[i] = &townie;
	}
}

void Town::AssignRoles() {
	Role* role = new Role;
	Faction* faction = new Faction; // Serial Killers
	Factions[SKLRS] = faction;
	role->name = "Serial Killer";
	role->pAction = &Attack;
	faction->name = "Serial Killers";
	faction->bHostile = true;
	Roles.insert({ role, faction });

	role = new Role;
	faction = new Faction; // Zombies
	Factions[ZOMBIES] = faction;
	role->name = "Zombie";
	role->pAction = &Bite;
	faction->name = "Zombies";
	faction->bHostile = true;
	faction->bIntel = true;
	Roles.insert({ role, faction });


	role = new Role;
	faction = new Faction; //Town
	Factions[TOWN] = faction;
	role->name = "Clown";
	role->pAction = &Block;
	faction->name = "Town";
	faction->bHostile = false;
	Roles.insert({ role, faction });

	role = new Role;
	role->name = "Bodyguard";
	role->pAction = &Protect;
	faction->name = "Town";
	faction->bHostile = false;
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
		//Townies[i]->role = &role;
	}
}

void Town::Think() {
	if (bDay) {
		DoDay();
		CheckTown();
		Sleep(5000);
		DoVoting();
		Sleep(5000);
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
			std::cout << "[" << Townies[i]->role->name << "] " << Townies[i]->name << " died." << std::endl;
		}
		else if (Townies[i]->bBitten and !Townies[i]->bProtected) {
			//auto it = std::next(Roles.begin(), ZOMBIES+1);
			//Townies[i]->role = it->first;
			Townies[i]->faction = Factions[ZOMBIES];
			//std::cout << Townies[i]->role->name << " " << Townies[i]->name << " was bitten." << std::endl;
		}
		else if (Townies[i]->bMarkedForDeath or Townies[i]->bBitten) {
			std::cout << "[" << Townies[i]->role->name << "] " << Townies[i]->name << " was attacked, but got protected!" << std::endl;
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
			if (Doer->role->pAction != &Block or !Doer->bAlive) {
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
			if (Doer->role->pAction == &Block or !Doer->bAlive) {
				continue;
			}
			Townie* Target = Townies[rand() % iSize];

			while (!Target->bAlive or Target == Doer or (Doer->faction == Target->faction and Doer->faction->bIntel)) {
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
			std::cout << "[" << Doer->role->name << "] " << Doer->name << " voted for [" << Target->role->name << "] " << Target->name << std::endl;
			Target->iVotes++;
		}
		int Alives = 0;
		for (int i = 0; i < iSize; i++) {
			if (Townies[i]->bAlive) {
				Alives++;
			}
		}
		Townie* MostVotes = nullptr;
		for (int i = 0; i < iSize; i++) {
			if (MostVotes == nullptr or Townies[i]->iVotes > MostVotes->iVotes) {
				MostVotes = Townies[i];
			}
		}
		Sleep(1000);
		if (MostVotes != nullptr and MostVotes->iVotes >= Alives / 4) {
			MostVotes->bAlive = false;
			std::cout << "[" << MostVotes->role->name << "] " << MostVotes->name << " was voted out." << std::endl;
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
					std::cout << "[" << Townies[i]->role->name << "] " << Townies[i]->name << " wins!" << std::endl;
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
	std::cout << "Alive Townies:" << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			std::cout << i+1 << ") [" << Townies[i]->role->name << "] " << Townies[i]->name << std::endl;
		}
	}
	std::cout << "Total: " << Alives << std::endl;
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