#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>
#include "StateTuple.h"
#include <exception>

//#include "Environment.h"

using namespace std;

class Environment;

class Rule
{
    public:
        Rule();
        ~Rule();
        string getStatement();
        string getPath();
        // updatePath updates the paths of all the branches of this rule to take this rule as root. 
        // It should only be called by the root of the tree.
        void updatePath();
        // updateEnvironment gives al the branches of this rule the new environment.
        // It should be called immediately after construction of the complete tree.
		virtual void updateEnvironment(Environment* env);
        virtual string toStringSigma() = 0;
        virtual string toStringE() = 0;
        virtual string toStringV() = 0;
        virtual int value(StateTuple states) = 0;           // Returns the value of this statement if it is an expression. Otherwise, an exception is thrown.
		virtual StateTuple sigma(StateTuple states) = 0;    // Returns the state after execution of this statement.
        virtual int energy(StateTuple states) = 0;          // Returns the total energy use after execution of this statement.
    protected:
        vector<Rule*> branches;
        string statement;       // Contains the statement that rule represents.
        string ruleName;        // Contains the name of the rule.
        string path;            // Represents the path from the root of the tree to this rule.
        
         // The td_ec function returns the time dependent energy use of the model during this rule.
         // It takes the current state and the time this rule takes, and returns energy usage
         // during that time.
		int td_ec(int t, StateTuple states);
		Environment* env;
        int left, right, middle;
    private:
        void updatePath(string path);
};

#endif // RULE_H
