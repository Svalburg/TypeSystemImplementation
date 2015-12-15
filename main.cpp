#include <iostream>
#include <stdio.h>
#include <vector>
#include "libtypesystem.h"
#include "parse.h"

using namespace std;
using namespace bitpowder::lib;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << argv[0] << " [filename]" << std::endl;
        return -1;
    }
    MemoryPool mp;
    auto result = ParseECAFile(argv[1], mp);
    if (!result)
        return -1;

    Rule* root = result.result()->getTypeRule();

	vector<ComponentFunction*> compfuncs;
	vector<Function*> funcs;
	vector<TimeDependentEC*> tdec;
	Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
	
	root->updateEnvironment(env);
	root->updatePath();
    
	try
	{
		StateTuple* states = new StateTuple();
		StateTuple stateEnd = root->sigma(*states);
		cout << stateEnd.toStringPState();
		cout << "Energy usage is : " << root->energy(*states);
	}
	catch (const runtime_error& e)
    {
        cerr << e.what() << endl;
		exit(-1);
    }

	getchar();
	return 0;
}
