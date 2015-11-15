#include "StateTuple.h"

StateTuple::StateTuple()
{
	
}

StateTuple::StateTuple(vector<declaration> pState, vector<declaration> cState)
{
	this->pState = pState;
	this->cState = cState;
}

int StateTuple::getPStateValue(string name)
{
	for(int i = 0; i < pState.size(); i++)
		if(name == pState.at(i).name)
			return pState.at(i).value;
	cout << "Value has not been declared.\n";
	getchar();
	exit(-4);
}

int StateTuple::getCStateValue(string name)
{
	for(int i = 0; i < cState.size(); i++)
		if(name == cState.at(i).name)
			return cState.at(i).value;
	cout << "Value has not been declared.\n";
	getchar();
	exit(-4);
}

void StateTuple::declarePState(string name, int value)
{
	for(int i = 0; i < pState.size(); i++)
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
	for(int i = 0; i < cState.size(); i++)
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

vector<StateTuple::declaration> StateTuple::getPState()
{
	return pState;
}

vector<StateTuple::declaration> StateTuple::getCState()
{
	return cState;
}

StateTuple::~StateTuple()
{}
