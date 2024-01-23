#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

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

//우선 코드 구분을 위해 별도 class로 작성했으나 server class에 포함해도 될 듯