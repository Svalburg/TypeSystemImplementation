#include "StateTuple.h"

StateTuple::StateTuple()
{
    cState = "";
    pState = "";
}

StateTuple::StateTuple(string cState, string pState)
{
    this->cState = cState;
    this->pState = pState;
}

string StateTuple::getCState()
{
    return cState;
}

string StateTuple::getPState()
{
    return pState;
}

string StateTuple::getTuple()
{
    return '(' + pState + ", " + cState + ')';
}

void StateTuple::setCState(string cState)
{
    this->cState = cState;
}

void StateTuple::setPState(string pState)
{
    this->pState = pState;
}

void StateTuple::setBoth(string cState, string pState)
{
    this->cState = cState;
    this->pState = pState;
}

StateTuple::~StateTuple()
{
    //dtor
}
