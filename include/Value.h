#ifndef VALUE_H
#define VALUE_H

class Value
{   /* A variable type holder class. Every subclass of this class represents a type,
       and should have a value of that type, and a getter that returns that value. */
	public:
		Value();
		virtual ~Value();
};

#endif //VALUE_H