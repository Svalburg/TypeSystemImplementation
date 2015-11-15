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
		int getPStateValue(string name);
		int getCStateValue(string name);
		void declarePState(string name, int value);
		void declareCState(string name, int value);
		vector<declaration> getPState();
		vector<declaration> getCState();
        virtual ~StateTuple();
    protected:
	private:
		vector<declaration> cState;
		vector<declaration> pState;
};

#endif // STATETUPLE_H
