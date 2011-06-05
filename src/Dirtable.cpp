#include "Dirtable.h"


Dirtable::Dirtable()
:   dirty_(true)
{}


Dirtable::Dirtable(bool state)
:   dirty_(state)
{}


Dirtable::~Dirtable()
{}


void
Dirtable::makeClean() const
{
    performCleaning();
    dirty_ = false;
}
