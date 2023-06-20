#include <iostream>
#include "Town.h"
using namespace std;

int main() {
	char custom[2];
	char y[2];
	cout << "Create Custom town? y/n ";
	cin.getline(custom, 10);
	int Size;
	cout << "How many town members? ";
	cin >> Size;
	Town MyTown(Size, custom[0] == "y"[0] ? true : false);
	return 0;
}
