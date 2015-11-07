#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include <stdexcept>

class Rule
{
    public:
        Rule(std::string statement, std::string ruleName);
        ~Rule();
        virtual std::string toStringSigma(std::string x, std::string y) = 0;
        virtual std::string toStringE(std::string x, std::string y) = 0;
        Rule* getBranch(int i);
        std::string getStatement();
        std::string getRuleName();
    protected:
    private:
        std::vector<Rule*> branches;
        std::string statement;
        std::string ruleName;
};

#endif // RULE_H
