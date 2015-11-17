#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Function.h"

class ComponentFunction : public Function
{
    public:
        ComponentFunction(string component, string name, string argumentName, Rule* definition, 
        int energy, int time);
        string getComponent();
        int getTime();
        int getEnergy();
        ~ComponentFunction();
    private:
        string component;
        int energy;
        int time;
    protected:
         
};

#endif // COMPONENTFUNCTION_H
