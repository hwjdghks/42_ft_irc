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

    // Checking whether command exists
       // 가용한 command list는 어느 곳(class)에 저장할지?
       // 존재하지 않는 command 입력 오류인 경우 ircMessage command에 특정값 설정하여 return? 
       

    // Extracting parameters
    while (iss >> token) 
    {
        ircMessage.parameters.push_back(token);
    }

    return ircMessage;
    }