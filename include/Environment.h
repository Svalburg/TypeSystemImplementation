#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "ComponentFunction.h"
#include "TimeDependentEC.h"
#include "RuleConst.h"
#include "parse.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>

/* The Environment for the type system. It contains the time each rule takes (TInput ... TWhile), 
 * a list of functions, a list of component functions, and a list of timedependentECs.
 * The lists of component functions and timedependentecs should not be able to change 
 * within the rule tree. A Function Definition can add a function to the environment.
 * At the program's beginning, the function list should (normally) be empty.
 * 
 * A component is represented in the environment by its component functions and timedependent 
 * energy costs.
 * */
class Environment
{
    public:
        Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop, int t_if,
                int t_while,
                vector<ComponentFunction*> componentFunctions,
                vector<Function*> functions,
                vector<TimeDependentEC*> tdecList);
        int getTInput();
        int getTConst();
        int getTVar();
        int getTAssign();
        int getTBinop();
		int getTIf();
        int getTWhile();
        ComponentFunction* getComponentFunction(string component, string name);
        Function* getFunction(string name);
        TimeDependentEC* getTimeDependentEC(string componentName);
        Environment* clone(); //Returns a new Environment containing a copy of all the values in this environment.
        void addFunction(string name, string argumentName, Rule* definition);
		void addComponentFunction(ifstream &compFile, StateTuple &states);
		vector<string> getComponentNames();
        ~Environment();
    private:
        int t_input, t_const, t_var, t_assign, t_binop, t_if, t_while; //t_binop needs to be changed to reflect different binops
        vector<ComponentFunction*> componentFunctions;
        vector<Function*> functions;
        vector<TimeDependentEC*> tdecList;
		vector<string> componentNames;
    protected:
        
};

#endif // ENVIRONMENT_H
