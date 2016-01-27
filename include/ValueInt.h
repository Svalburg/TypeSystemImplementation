#ifndef VALUEINT_H
#define VALUEINT_H

#include "Value.h"

class ValueInt : public Value
{
	public:
		ValueInt(int value);
		int getValue();
		virtual ~ValueInt();
	private:
		int value;
};

#endif //VALUEINT_H