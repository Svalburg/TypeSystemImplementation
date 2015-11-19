#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "ComponentFunction.h"
#include "TimeDependentEC.h"

class Environment
{
    public:
        Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop,
                vector<ComponentFunction> componentFunctions,
                vector<Function> functions,
                vector<TimeDependentEC> timeDependentEC);
        int getTInput();
        int getTConst();
        int getTVar();
        int getTAssign();
        int getTBinop();
        ComponentFunction getComponentFunction(string component, string name);
        Function getFunction(string name);
        TimeDependentEC getTimeDependentEC(string componentState);
        Environment clone();
        void addFunction(string name, string argumentName, Rule* definition);
        ~Environment();
    private:
        int t_input, t_const, t_var, t_assign, t_binop; //t_binop needs to be changed to reflect different binops
        vector<ComponentFunction> componentFunctions;
        vector<Function> functions;
        vector<TimeDependentEC> timeDependentEC;
    protected:
        
};

#endif // ENVIRONMENT_H
