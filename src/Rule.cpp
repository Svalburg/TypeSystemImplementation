#include "Rule.h"
#include "Environment.h"

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
	vector<StateTuple::declaration> ps = states.getPState();
	for(size_t i = 0; i < ps.size(); i++)
	{
		TimeDependentEC* phi = env->getTimeDependentEC(ps.at(i).name);
		total += (phi->getEnergyCost(states))*t;
	}
}

Rule::~Rule()
{
    //dtor
}
