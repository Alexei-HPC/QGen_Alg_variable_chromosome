#include "Events.h"


Events::Events(void)
{
}

int Events::PrintEvents()
{
	for(unsigned long i = 0; i < events_vector.size(); i++)
	{
		events_vector[i].Print();
	}
	return 0;
}

Events::~Events(void)
{
}
