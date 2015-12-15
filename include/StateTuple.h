#ifndef STATETUPLE_H
#define STATETUPLE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class StateTuple
{
	public:
		//stores a variable name and corresponding value
		//will need to be changed for multi type support
		struct declaration
		{
			string name;
			int value;
		};
		
        StateTuple();
		StateTuple(vector<declaration> cState, vector<declaration> pState);
		//returns the value for a variable name
		int getPStateValue(string name);
		int getCStateValue(string name);
		//updates the value for a given variable if it exists, creates it if it doesn't
		void declarePState(string name, int value);
		void declareCState(string name, int value);
		//returns the entire state vector
		vector<declaration> getPState();
		vector<declaration> getCState();
        virtual ~StateTuple();
    protected:
	private:
		//collections of Componentstates and Programstates
		vector<declaration> cState;
		vector<declaration> pState;
};

#endif // STATETUPLE_H
