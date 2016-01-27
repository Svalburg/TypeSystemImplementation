#ifndef STATETUPLE_H
#define STATETUPLE_H

#include <string>
#include <vector>
#include <iostream>
#include "ValueString.h"
#include "ValueInt.h"

using namespace std;

class StateTuple
{
	public:
		//stores a variable name and corresponding value
		//will need to be changed for multi type support
		struct declaration
		{
			string name;
			Value* value;
		};
		
		StateTuple();
		StateTuple(vector<declaration> cState, vector<declaration> pState);
		//returns the value for a variable name
		Value* getPStateValue(string name);
		Value* getCStateValue(string name);
		Value* getStateValue(string name);
		//updates the value for a given variable if it exists, creates it if it doesn't
		void declarePState(string name, Value* value);
		void declareCState(string name, Value* value);
		void declareState(string name, Value* value);
		void deleteCState(string name);
		void deletePState(string name);
		//updates the accessCState value
		void setAccessCState(bool flag);
		//returns the entire state vector
		vector<declaration> getPState();
		vector<declaration> getCState();
		//return a string with the entire state
		string toStringPState();
		string toStringCState();
        virtual ~StateTuple();
    protected:
	private:
		//collections of Componentstates and Programstates
		vector<declaration> cState;
		vector<declaration> pState;
		//Flag for accessing the component state instead of the program state
		bool accessCState;
};

#endif // STATETUPLE_H
