#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Rule.h"



class ComponentFunction
{
    public:
        ComponentFunction(string argumentName, int time, int energy, Rule* definition);
        StateTuple sigma(StateTuple states);
        int value(StateTuple states);
        string getArgumentName();
        int getTime();
        int getEnergy();
        virtual ~ComponentFunction();
    private:
        string argumentName;
        int time;
        int energy;
        Rule* definition;
    protected:
        

};

#endif // COMPONENTFUNCTION_H
