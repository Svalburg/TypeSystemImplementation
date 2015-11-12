#ifndef STATETUPLE_H
#define STATETUPLE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class StateTuple
{
	struct declaration
	{
		string name;
		int value;
	};
    public:
        StateTuple();
		StateTuple(vector<declaration> cState, vector<declaration> pState);
		int getCStateValue(string name);
		int getPStateValue(string name);
		void declareCState(string name, int value);
		void declarePState(string name, int value);
		vector<declaration> getCState();
		vector<declaration> getPState();
        virtual ~StateTuple();
    protected:
	private:
		vector<declaration> cState;
		vector<declaration> pState;
};

#endif // STATETUPLE_H
