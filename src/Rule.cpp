#include "Rule.h"

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
    for(int i = 0; i < branches.size(); i++)
    {
        stringstream stream;
        stream << path << '-' << i;
        branches.at(i)->updatePath(stream.str());
    }
}

void Rule::updatePath(string path)
{
    this->path = path;
    for(int i = 0; i < branches.size(); i++)
    {
        stringstream stream;
        stream << path << '-' << i;
        branches.at(i)->updatePath(stream.str());
    }
}

Rule::~Rule()
{
    //dtor
}
