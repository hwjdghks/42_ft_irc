#include <iostream>

#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "argument not matched." << '\n';
		return 1;
	}	
	Server	server;

	if (!server.pasreAndSetArguements(argv))
	{
		std::cout << "Invalid argument." << '\n';
		return 1;
	}
	try
	{
		server.run();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}