#include "StateTuple.h"

StateTuple::StateTuple()
{
	accessCState = false;
}

StateTuple::StateTuple(vector<declaration> pState, vector<declaration> cState)
{
	this->pState = pState;
	this->cState = cState;
	accessCState = false;
}

int StateTuple::getPStateValue(string name)
{
	for(size_t i = 0; i < pState.size(); i++)
		if(name == pState.at(i).name)
			return pState.at(i).value;
	throw runtime_error("Exception: Program value '" + name + "' has not been declared.\n");
}

int StateTuple::getCStateValue(string name)
{
	for(size_t i = 0; i < cState.size(); i++)
		if(name == cState.at(i).name)
			return cState.at(i).value;
	throw runtime_error("Exception: Component value '" + name + "' has not been declared.\n");
}

int StateTuple::getStateValue(string name)
{
	if(accessCState)
		return getCStateValue(name);
	else
		return getPStateValue(name);
}

void StateTuple::declarePState(string name, int value)
{
	for(size_t i = 0; i < pState.size(); i++)
		if(name == pState.at(i).name)
		{
			pState.at(i).value = value;
			return;
		}
	declaration newvar;
	newvar.name = name;
	newvar.value = value;
	pState.push_back(newvar);
}

void StateTuple::declareCState(string name, int value)
{
	for(size_t i = 0; i < cState.size(); i++)
		if(name == cState.at(i).name)
		{
			cState.at(i).value = value;
			return;
		}
	declaration newvar;
	newvar.name = name;
	newvar.value = value;
	cState.push_back(newvar);
}

void StateTuple::declareState(string name, int value)
{
	if(accessCState)
		declareCState(name, value);
	else
		declarePState(name, value);
}

void StateTuple::deleteCState(string name)
{
	for(size_t i = 0; i < cState.size(); i++)
		if(name == cState.at(i).name)
		{
			cState.erase(cState.begin()+i);
			return;
		}
	cerr << "Cannot delete non-existent component variable: " << name << endl;
}

void StateTuple::deletePState(string name)
{
	for(size_t i = 0; i < pState.size(); i++)
		if(name == pState.at(i).name)
		{
			pState.erase(pState.begin()+i);
			return;
		}
	cerr << "Cannot delete non-existent program variable: " << name << endl;
}

void StateTuple::setAccessCState(bool flag)
{
	accessCState = flag;
}

vector<StateTuple::declaration> StateTuple::getPState()
{
	return pState;
}

vector<StateTuple::declaration> StateTuple::getCState()
{
	return cState;
}

string StateTuple::toStringPState()
{
	string out = "";
	if(pState.size() == 0)
	{
		out += "State is empty.";
		return out;
	}
	else
		for(size_t i = 0; i < pState.size(); i++)
			out += pState.at(i).name + " = " + to_string(pState.at(i).value) + "\n";
	return out;
}

string StateTuple::toStringCState()
{
	string out = "";
	if(cState.size() == 0)
	{
		out += "State is empty.";
		return out;
	}
	else
		for(size_t i = 0; i < cState.size(); i++)
			out += cState.at(i).name + " = " + to_string(cState.at(i).value) + "\n";
	return out;
}

StateTuple::~StateTuple()
{}
