#ifndef STATETUPLE_H
#define STATETUPLE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class StateTuple
{
    public:
        StateTuple();
		int getCStateValue(string name);
		int getPStateValue(string name);
		void declareCState(string name, int value);
		void declarePState(string name, int value);
        virtual ~StateTuple();
    protected:
	private:
		struct declaration
		{
			string name;
			int value;
		};
		vector<declaration> cState;
		vector<declaration> pState;
};

#endif // STATETUPLE_H
