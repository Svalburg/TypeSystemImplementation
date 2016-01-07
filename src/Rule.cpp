#include "Rule.h"
#include "Environment.h"
#include "RuleFuncDef.h"

Rule::Rule()
{
	branches.reserve(3);
    path = "";
}

string Rule::getStatement()
{
    return statement;
}

string Rule::getPath()
{
    return path;
}

void Rule::updatePath()
{
    path = "root";
    for(size_t i = 0; i < branches.size(); i++)
    {
        stringstream stream;
        stream << path << '-' << i;
        branches.at(i)->updatePath(stream.str());
    }
}

void Rule::updatePath(string path)
{
    this->path = path;
    for(size_t i = 0; i < branches.size(); i++)
    {
        stringstream stream;
        stream << path << '-' << i;
        branches.at(i)->updatePath(stream.str());
    }
}

void Rule::updateEnvironment(Environment* env)
{
	this->env = env;
	for(size_t i = 0; i < branches.size(); i++)
		branches.at(i)->updateEnvironment(env);
}

int Rule::td_ec(int t, StateTuple states)
{
	int total = 0;
	vector<string> components = env->getComponentNames();
	for(size_t i = 0; i < components.size(); i++)
	{
		TimeDependentEC* phi = env->getTimeDependentEC(components.at(i));
		total += (phi->getEnergyCost(states))*t;
	}
	return total;
}

Rule::~Rule()
{
    //dtor
}
