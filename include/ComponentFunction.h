#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Rule.h"

class ComponentFunction
{
	public:
        ComponentFunction(string component, string name, string argumentName, int time, int energy, Rule* definition);
        StateTuple sigma(StateTuple states);
        int value(StateTuple states);
        string getArgumentName();
		string getComponent();
		string getName();
        int getTime();
        int getEnergy();
        virtual ~ComponentFunction();
	private:
		string component;
		string name;
        string argumentName;
        int time;
        int energy;
        Rule* definition;
    protected:
        

};

#endif // COMPONENTFUNCTION_H
