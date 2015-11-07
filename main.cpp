#include <iostream>
#include <vector>
#include "Rule.h"
#include "RuleEdInput.h"

using namespace std;

int main()
{
    vector<Rule*> testVector;
    testVector.push_back(new RuleEdInput("test"));
    cout << (*testVector.at(0)).toStringE("x", "y") << endl;
    return 0;
}
