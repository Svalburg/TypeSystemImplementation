#include "StateTuple.h"
#include <gtest/gtest.h>

TEST(StateTupleTest, DeclareAndGet)
{
	StateTuple* testTuple = new StateTuple();
	std::string testx = "x", testa = "AAAAAAAAA", testb = "jfhp2i7thj/*";
	int x = 5, x2 = -500, a = 42, b = 9001;
	testTuple->StateTuple::declarePState(testx, x);
	testTuple->declareCState(testa, a);
	testTuple->declareCState(testb, b);
	EXPECT_EQ(x, testTuple->getPStateValue(testx));
	EXPECT_EQ(a, testTuple->getCStateValue(testa));
	EXPECT_EQ(b, testTuple->getCStateValue(testb));
	testTuple->declarePState(testx, x2);
	EXPECT_EQ(x2, testTuple->getPStateValue(testx));
}
