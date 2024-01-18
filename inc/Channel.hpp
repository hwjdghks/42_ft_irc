#include <string>
#include <vector>

#include "Client.hpp"

class Channel
{
public:
	Channel();
	Channel(const Channel &ref);
	~Channel();

	Channel &operator=(const Channel &ref);

private:
	std::string					name;
	std::string					password;
	std::string					topic;
	std::vector<bool>			options;
	std::vector<Client>			operators;
	std::vector<Client>			users;
	std::vector<std::string>	invited;
};