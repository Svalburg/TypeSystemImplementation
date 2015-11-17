#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Rule.h"

class Function
{
	public:
        Function(string name, string argumentName, Rule* definition);
        StateTuple sigma(StateTuple states);
        int value(StateTuple states);
        string getArgumentName();
		string getName();
        virtual ~Function();
	private:
		string name;
        string argumentName;
        Rule* definition;
    protected:
        

};

#endif // COMPONENTFUNCTION_H
