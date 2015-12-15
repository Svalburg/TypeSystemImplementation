#ifndef RULEFUNCDEF_H
#define RULEFUNCDEF_H

#include <Rule.h>


class RuleFuncDef : public Rule
{
    public:
        RuleFuncDef(string functionname, string argumentname, Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        void updateEnvironment(Environment* env);
        virtual ~RuleFuncDef();
    protected:
    private:
        string functionname;
        string argumentname;
};

#endif // RULEFUNCDEF_H
