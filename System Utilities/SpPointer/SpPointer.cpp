/*
	Author: Sean Pesce

	Multi-level pointer class
*/

#include "SpPointer.h"


// Constructors/destructors
SpPointer::SpPointer()
{
	base = NULL;
}
SpPointer::SpPointer(void *starting_address, std::vector<long> new_offsets)
{
	base = starting_address;
	for (auto offset : new_offsets)
	{
		offsets.push_back(offset);
	}
}
SpPointer::SpPointer(void *starting_address)
{
	base = starting_address;
}

SpPointer::~SpPointer() {};


// Returns final resolved address
void *SpPointer::resolve()
{
	void *address = base;
	for (auto offset : offsets)
	{
		address = (void*)((*(uint64_t*)address) + offset);
	}
	return address;
}
