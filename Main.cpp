#include <iostream>
#include <fstream>
using namespace std;

#include "Construction.h"

void main()
{
	queue*InactivE = new queue;
	InitQueue(InactivE);
	castle*ct = Initiallizations(InactivE);
	float c1, c2, c3;
	simulateprogram(InactivE, ct, c1, c2, c3);

	cin.get();
}
