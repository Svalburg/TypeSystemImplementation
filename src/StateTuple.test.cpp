#include "StateTuple.h"
#include <gtest/gtest.h>

TEST(StateTupleTest, DeclareAndGet)
{
	StateTuple* testTuple = new StateTuple();
	std::string testx = "x", testa = "AAAAAAAAA", testb = "jfhp2i7thj/*";
	int x = 5, x2 = -500, a = 42, b = 9001;
	testTuple->declarePState(testx, new ValueInt(x));
	testTuple->declareCState(testa, new ValueInt(a));
	testTuple->declareCState(testb, new ValueInt(b));
	ValueInt* v1 = dynamic_cast<ValueInt*>(testTuple->getPStateValue(testx));
	ValueInt* v2 = dynamic_cast<ValueInt*>(testTuple->getCStateValue(testa));
	ValueInt* v3 = dynamic_cast<ValueInt*>(testTuple->getCStateValue(testb));
	if(v1 && v2 && v3)
	{
		EXPECT_EQ(x, v1->getValue());
		EXPECT_EQ(a, v2->getValue());
		EXPECT_EQ(b, v3->getValue());
	}
	else
		FAIL();
	testTuple->declarePState(testx, new ValueInt(x2));
	ValueInt* v4 = dynamic_cast<ValueInt*>(testTuple->getPStateValue(testx));
	if(v4)
		EXPECT_EQ(x2, v4->getValue());
	else
		FAIL();
}
