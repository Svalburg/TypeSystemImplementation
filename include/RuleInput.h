#ifndef RULEEDINPUT_H
#define RULEEDINPUT_H

#include <Rule.h>


class RuleEdInput : public Rule
{
    public:
        RuleEdInput(std::string statement);
        std::string toStringSigma(std::string x, std::string y);
        std::string toStringE(std::string x, std::string y);
        virtual ~RuleEdInput();
    protected:
    private:
};

#endif // RULEEDINPUT_H
