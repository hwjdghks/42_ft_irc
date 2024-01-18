#include "Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(const Channel &ref)
{
	*this = ref;
}

Channel::~Channel(void) {}

Channel &Channel::operator=(const Channel &ref)
{
	if (this == &ref)
		return *this;
	/* Edit */
	return *this;
}
