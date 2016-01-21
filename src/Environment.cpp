#include "Environment.h"

Environment::Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop, int t_if,
                int t_while,
                vector<ComponentFunction*> componentFunctions,
                vector<Function*> functions,
                vector<TimeDependentEC*> tdecList)
{
    this->t_input = t_input;
    this->t_const = t_const;
    this->t_var = t_var;
    this->t_assign = t_assign;
    this->t_binop = t_binop;
	this->t_if = t_if;
    this->t_while = t_while;
    this->componentFunctions = componentFunctions;
    this->functions = functions;
    this->tdecList = tdecList;
	for(size_t i = 0; i < componentFunctions.size(); i++)
	{
		if(find(componentNames.begin(), componentNames.end(), 
			componentFunctions.at(i)->getComponent()) == componentNames.end())
			componentNames.push_back(componentFunctions.at(i)->getComponent());
	}
}

int Environment::getTInput()
{
    return t_input;
}

int Environment::getTConst()
{
    return t_const;
}

int Environment::getTVar()
{
    return t_var;
}

int Environment::getTAssign()
{
    return t_assign;
}

int Environment::getTBinop()
{
    return t_binop;
}

int Environment::getTIf()
{
	return t_if;
}

int Environment::getTWhile()
{
    return t_while;
}

ComponentFunction* Environment::getComponentFunction(string component, string name)
{
	for(size_t i=0;i<componentFunctions.size();i++)
	{
		ComponentFunction* cfunction = componentFunctions.at(i);
		if(cfunction->getComponent() == component && cfunction->getName() == name)
			return cfunction;
	}
	throw runtime_error("Exception: No component function found with name: " + component + '.' + name);
}

Function* Environment::getFunction(string name)
{
	for(size_t i=0;i<functions.size();i++)
	{
		Function* function = functions.at(i);
		if(function->getName() == name)
		{
			return function;
		}
	}
	throw runtime_error("Exception: No function found with name: " + name);
}

TimeDependentEC* Environment::getTimeDependentEC(string componentName)
{
	for(size_t i=0;i<tdecList.size();i++)
	{
		TimeDependentEC* timedependent = tdecList.at(i);
		if(timedependent->getComponentName() == componentName)
			return timedependent;
	}
	return new TimeDependentEC(componentName, new RuleConst("0"));
	//throw runtime_error("Exception: No component found with name: " + componentName);
}

Environment* Environment::clone()
{
    vector<Function*> functioncopy;
    vector<ComponentFunction*> compfunccopy;
    vector<TimeDependentEC*> tdeccopy;
    for(size_t i = 0; i < functions.size(); i++)
        functioncopy.push_back(functions.at(i));
    for(size_t i = 0; i < componentFunctions.size(); i++)
        compfunccopy.push_back(componentFunctions.at(i));
    for(size_t i = 0; i < tdecList.size(); i++)
        tdeccopy.push_back(tdecList.at(i));
    return new Environment(t_input, t_const, t_var, t_assign, t_binop, t_if, t_while, compfunccopy, functioncopy, tdeccopy);
}

void Environment::addFunction(string name, string argumentName, Rule* definition)
{
    Function* newfunction = new Function(name, argumentName, definition);
    functions.push_back(newfunction);
}

vector<string> Environment::getComponentNames()
{
	return componentNames;
}

void Environment::addComponentFunction(ifstream &compFile, StateTuple &states)
{
	string line;
	getline(compFile, line);
	while(line.empty())
		getline(compFile, line);
	size_t pos = line.find("component");
	if(pos == string::npos)
	{
		cerr << "Missing component name in file." << endl;
		exit(-1);
	}
	string compName = line.substr(pos+10);
	componentNames.push_back(compName);
	cout << "Component name: " << compName << endl;
	
	getline(compFile, line);
	while(line.empty())
		getline(compFile, line);
	
	pos = line.find("variable");
	while(pos !=  string::npos)
	{
		line.erase(pos, 9);
		pos = line.find(":=");
		if(pos == string::npos)
		{
			cerr << "Variable incorrectly defined in file." << endl;
			exit(-1);
		}
		
		string varName = line.substr(0, pos-1);
		line.erase(0, pos+2);
		stringstream ss;
		ss << line;
		int val = -1;
		ss >> val;
		ss.str("");
		ss.clear();
		states.declareCState(varName, val);
		
		cout << "Assigned component variable: " << varName << " := " << val << endl;
		
		getline(compFile, line);
		while(line.empty())
			getline(compFile, line);
		pos = line.find("variable");
	}
	
	pos = line.find("phi:");
	if(pos != string::npos)
	{
		getline(compFile, line);
		string phiString = "";
		pos = line.find("end phi");
		while(pos == string::npos)
		{
			phiString += line;
			getline(compFile, line);
			while(line.empty())
				getline(compFile, line);
			pos = line.find("end phi");
		}
		bitpowder::lib::MemoryPool mpPhi;
		auto phiResult = ParseECAMemory(phiString, mpPhi);
		if(!phiResult)
		{
			cerr << "Invalid phi function." << endl;
			exit(-1);
		}
		Rule* phiRule = phiResult.result()->getTypeRule();
		TimeDependentEC* phi = new TimeDependentEC(compName, phiRule);
		tdecList.push_back(phi);
		cout << "Added phi function." << endl << phiRule->getStatement() << endl;
	}
	
	getline(compFile, line);
	while(line.empty())
		getline(compFile, line);
	
	pos = line.find("component function");
	while(pos != string::npos)
	{
		Rule* energyRule;
		Rule* definitionRule;
		
		bool error = false;
		line.erase(0, 19);
		pos = line.find('(');
		if(pos == string::npos)
			error = true;
		string funcName = line.substr(0, pos);
		line.erase(0, pos+1);
		pos = line.find(')');
		if(pos == string::npos)
			error = true;
		line.erase(pos, 1);
		string argument = line;
		getline(compFile, line);
		while(line.empty())
			getline(compFile, line);
		pos = line.find("time");
		if(pos == string::npos)
			error = true;
		line.erase(0, pos+4);
		stringstream ss;
		ss << line;
		int time = 0;
		ss >> time;
		
		getline(compFile, line);
		while(line.empty())
			getline(compFile, line);
		pos = line.find("energy:");
		if(pos != string::npos && !error)
		{
			getline(compFile, line);
			while(line.empty())
				getline(compFile, line);
			string energyString = "";
			pos = line.find("end energy");
			while(pos == string::npos)
			{
				energyString += line;
				getline(compFile, line);
				while(line.empty())
					getline(compFile, line);
				pos = line.find("end energy");
			}
			bitpowder::lib::MemoryPool mpEnergy;
			auto energyResult = ParseECAMemory(energyString, mpEnergy);
			if(!energyResult)
			{
				cerr << "Invalid energy function." << endl;
				exit(-1);
			}
			energyRule = energyResult.result()->getTypeRule();
		}
		else
			error = true;
			
		getline(compFile, line);
		while(line.empty())
			getline(compFile, line);
		pos = line.find("definition:");
		if(pos != string::npos && !error)
		{
			getline(compFile, line);
			while(line.empty())
				getline(compFile, line);
			string definitionString = "";
			pos = line.find("end definition");
			while(pos == string::npos)
			{
				definitionString += line;
				getline(compFile, line);
				while(line.empty())
					getline(compFile, line);
				pos = line.find("end definition");
			}
			bitpowder::lib::MemoryPool mpDefinition;
			auto definitionResult = ParseECAMemory(definitionString, mpDefinition);
			if(!definitionResult)
			{
				cerr << "Invalid definition function." << endl;
				exit(-1);
			}
			definitionRule = definitionResult.result()->getTypeRule();
		}
		else
			error = true;
		
		if(error)
		{
			cerr << "Component function incorrectly defined." << endl;
			exit(-1);
		}
		
		ComponentFunction* func = new ComponentFunction(compName, funcName, argument, definitionRule, energyRule, time);
		componentFunctions.push_back(func);
		
		cout << "Added component function: " << funcName << endl;
		
		getline(compFile, line);
		while(line.empty())
			getline(compFile, line);
		pos = line.find("component function");
	}
	pos = line.find("end component");
	if(pos == string::npos)
	{
		cerr << "Missing \"end component\"." << endl;
		exit(-1);
	}
}

Environment::~Environment()
{
}

