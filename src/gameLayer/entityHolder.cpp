#include "entityHolder.h"
#include <asserts.h>

std::uint64_t EntityIdHolder::getEntityIdAndIncreament()
{
	std::uint64_t id = idCounter;
	idCounter++;

	permaAssertComment(id < UINT64_MAX - 1, "We ran out of ids somehow...");

	return id;
}