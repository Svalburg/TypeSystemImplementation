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

Rule::~Rule()
{
    //dtor
}
