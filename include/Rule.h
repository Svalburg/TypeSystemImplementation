#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>
#include "StateTuple.h"

using namespace std;

class Rule
{
    public:
        Rule();
        ~Rule();
        string getStatement();
        string getPath();
        void updatePath();
        virtual string toStringSigma() = 0;
        virtual string toStringE() = 0;
        virtual string toStringV() = 0;
    protected:
        vector<Rule*> branches;
        string statement;
        string ruleName;
        string path;
        int left, right, middle;
    private:
        void updatePath(string path);
};

#endif // RULE_H
