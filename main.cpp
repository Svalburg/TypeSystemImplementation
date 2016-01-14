#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
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
    auto result = ParseECAFile(argv[argc-1], mp);
    if (!result)
        return -1;

    Rule* root = result.result()->getTypeRule();

	vector<ComponentFunction*> compfuncs;
	vector<Function*> funcs;
	vector<TimeDependentEC*> tdec;
	
	Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
	StateTuple* states = new StateTuple();
	
	for(int i = 1; i < argc-1; i++)
	{
		ifstream compFile;
		compFile.open(argv[i]);
		if(!compFile)
		{
			cerr << argv[i] << " - File does not exist." << endl;
			return -1;
		}
		cout << "Reading file " << argv[i] << endl;
		env->addComponentFunction(compFile, *states);
		compFile.close();
	}
	
	root->updateEnvironment(env);
	root->updatePath();
    cout << root->getStatement() << endl;
    
	try
	{
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
