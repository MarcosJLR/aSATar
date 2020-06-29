#include <iostream>
#include <fstream>

#include "Solver.hpp"

using namespace asatar;
using namespace std;

Solver solver;

int main(int argc, char* argv[])
{
	int timeout = 0;
	if(argc > 1)
	{
		timeout = atoi(argv[1]);
	}

	if(timeout > 0) { solver.setTimeout(timeout); }

	solver.readFromFile(argv[1]);
	solver.solve();
	solver.printToFile("solution.txt");

	return 0;
}