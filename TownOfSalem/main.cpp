#include <iostream>
#include "Town.h"
using namespace std;

int main() {
	int Size;
	cout << "How many town members? ";
	cin >> Size;
	Town MyTown(Size);
	return 0;
}
