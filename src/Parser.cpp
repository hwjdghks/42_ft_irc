    #include "Parser.hpp"

    IRCMessage Parser::parseMessage(const std::string &message) 
    {
        IRCMessage ircMessage;
        
        std::istringstream iss(message);
        std::string token;
    
    // Extracting prefix
    if (message[0] == ':') 
        {
        iss >> token;
        ircMessage.prefix = token.substr(1);
        }

    // Extracting command
    iss >> ircMessage.command;

    // Extracting parameters
    while (iss >> token) 
    {
        ircMessage.parameters.push_back(token);
    }

    return ircMessage;
    }