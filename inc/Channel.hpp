#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

#include "Client.hpp"

enum {
	T, O, K, L, I
};

class Channel
{
public:
	Channel();
	Channel(const Channel &ref);
	~Channel();

	Channel &operator=(const Channel &ref);

private:
	bool						option[5];
	int							limit;
	std::string					name;
	std::string					password;
	std::string					topic;
	std::vector<Client *>			operators;
	std::vector<Client *>			users;
	std::vector<std::string>	invited;
};
#endif