#include "Town.h"
#include "Actions.h"
#include "OtherFunctions.h"
#include "resource.h"

Town::Town() {
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
	Roles.insert({"Serial Killer", &Attack});
	Roles.insert({"Bodyguard", &Protect});
	Roles.insert({"Clown", &Block});
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());


	for (int i = 0; i < iSize; i++) {
		// Generate a random index
		std::uniform_int_distribution<> dist(0, Roles.size() - 1);
		auto randIndex = dist(gen);

		// Advance an iterator to the random index
		auto it = std::next(Roles.begin(), randIndex);
		Townies[i]->role = new Role;
		Townies[i]->role->name = it->first;
		Townies[i]->role->bHostile = Roles[it->first] == &Attack;
		Townies[i]->role->pAction = Roles[it->first];
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
		DoNight();
	}
	bDay = !bDay;
}

void Town::DoDay() {
	std::cout << "Day" << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bMarkedForDeath and !Townies[i]->bProtected) {
			Townies[i]->bAlive = false;
			std::cout << Townies[i]->role->name << " " << Townies[i]->name << " died." << std::endl;
		}
		else if (Townies[i]->bMarkedForDeath) {
			std::cout << Townies[i]->role->name << " " << Townies[i]->name << " was attacked, but got protected!" << std::endl;
		}
		Townies[i]->bBlocked = false;
		Townies[i]->bMarkedForDeath = false;
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
			while (Target == Doer or !Target->bAlive) {
				Target = Townies[rand() % iSize];
			}
			std::cout << Doer->role->name << " " << Doer->name << " voted for " << Target->role->name << " " << Target->name << std::endl;
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
			std::cout << MostVotes->role->name << " " << MostVotes->name << " was voted out." << std::endl;
		}
	}
}

void Town::CheckTown() {
	int AliveTownies = 0;
	int AliveEvils = 0;
	int Alives = 0;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			if (!Townies[i]->role->bHostile) {
				AliveTownies++;
			}
			else {
				AliveEvils++;
			}
		}
	}
	if (AliveTownies < 1 or AliveEvils < 1) {
		bInAction = false;
		for (int i = 0; i < iSize; i++) {
			if (Townies[i]->bAlive) {
				std::cout << Townies[i]->role->name << " " << Townies[i]->name << " wins!" << std::endl;
			}
		}
		if (AliveTownies < 1) {
			std::cout << "Evils win!" << std::endl;
			exit(0);
		}
		else {
			std::cout << "Town wins!" << std::endl;
			exit(0);
		}
	}
	std::cout << "Alive Townies:" << std::endl;
	for (int i = 0; i < iSize; i++) {
		if (Townies[i]->bAlive) {
			Alives++;
			std::cout << i << ") " << Townies[i]->role->name << " " << Townies[i]->name << std::endl;
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