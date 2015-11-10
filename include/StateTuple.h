#ifndef STATETUPLE_H
#define STATETUPLE_H

#include <string>

using namespace std;

class StateTuple
{
    public:
        StateTuple();
        StateTuple(string cState, string pState);
        string getCState();
        string getPState();
        string getTuple();
        void setCState(string cState);
        void setPState(string pState);
        void setBoth(string cState, string pState);
        virtual ~StateTuple();
    protected:
    private:
        string cState;
        string pState;
};

#endif // STATETUPLE_H
