#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <vector>

struct IRCMessage 
{
	std::string prefix;
	std::string command;
	std::vector <std::string> parameters;
};

class Parser 
{
public:
		IRCMessage parseMessage(const std::string &message);
};

#endif