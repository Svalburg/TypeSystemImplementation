#include "ValueInt.h"

ValueInt::ValueInt(int value)
{
	this->value = value;
}

int ValueInt::getValue()
{
	return value;
}

ValueInt::~ValueInt(){}