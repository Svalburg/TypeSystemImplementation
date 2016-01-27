#ifndef VALUESTRING_H
#define VALUESTRING_H

#include "Value.h"
#include <string>

class ValueString : public Value
{
	public:
		ValueString(std::string value);
		std::string getValue();
		virtual ~ValueString();
	private:
		std::string value;
};

#endif //VALUESTRING_H