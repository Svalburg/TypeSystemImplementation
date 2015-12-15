#ifndef COMPONENTFUNCTION_H
#define COMPONENTFUNCTION_H

#include "Function.h"

/* A component function represents an action that component can perform.
 * It has a component name, a function name, and a single argument.
 * The component function definition represents the changes calling the 
 * function has on the component state, and should thus generally consist 
 * of assignments to the component state.
 * Time represents the amount of time it takes to execute this function. 
 * The energy definition represents the energy it takes to use this component
 * function. This should be arithmetic operations with possible usage of 
 * component state variables, for example:
 * 
 *          Antenna.Send(x) is a function of Antenna. It's energy rule is
 *          20 * antenna_on. In this case, if the antenna is not on, send 
 *          won't take any energy.
 *  */

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
