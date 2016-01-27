#include "ValueString.h"

ValueString::ValueString(std::string value)
{
	this->value = value;
}

std::string ValueString::getValue()
{
	return value;
}

ValueString::~ValueString(){}