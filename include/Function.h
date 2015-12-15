#ifndef FUNCTION_H
#define FUNCTION_H

#include "Rule.h"

/* A function is a program function designated within the program with a function definiton.
 * A function has a name and a single argument. Representing a function with no argument can
 * be done by using a constant that is not used in the function. Note that this will lead to a
 * slight overestimation of energy use.
 * The definition argument in the constructor is a rule tree describing what the function does.
 * A function is always an expression, and should return a value.
 * */

class Function
{
	public:
        Function(string name, string argumentName, Rule* definition);
        StateTuple sigma(StateTuple states);
        int value(StateTuple states);
		int energy(StateTuple states);
        string getArgumentName();
		string getName();
        virtual ~Function();
	private:
		string name;
        string argumentName;
        Rule* definition;
    protected:
        

};

#endif //FUNCTION_H
