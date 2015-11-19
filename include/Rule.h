#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>
#include "StateTuple.h"
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
        void updatePath();
		void updateEnvironment(Environment* env);
        virtual string toStringSigma() = 0;
        virtual string toStringE() = 0;
        virtual string toStringV() = 0;
        virtual int value(StateTuple states) = 0;
		virtual StateTuple sigma(StateTuple states) = 0;
    protected:
        vector<Rule*> branches;
        string statement;
        string ruleName;
        string path;
		Environment* env;
        int left, right, middle;
    private:
        void updatePath(string path);
};

#endif // RULE_H
