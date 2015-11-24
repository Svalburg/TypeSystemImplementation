#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Function.h"

class ComponentFunction : public Function
{
    public:
        ComponentFunction(string component, string name, string argumentName, Rule* definition, 
        Rule* energyFunc, int time);
        string getComponent();
        int getTime();
        int energy(StateTuple states);
        ~ComponentFunction();
    private:
        string component;
        Rule* energyFunc;
        int time;
    protected:
         
};

#endif // COMPONENTFUNCTION_H
