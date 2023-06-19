#include <iostream>
#include "Town.h"
using namespace std;

int main() {
	char custom[1];
	cout << "Create Custom town? y/n ";
	cin.getline(custom, 1);
	int Size;
	cout << "How many town members? ";
	cin >> Size;
	Town MyTown(Size, custom == "y" ? true : false);
	return 0;
}
