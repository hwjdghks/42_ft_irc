#include "Irc.hpp"

/*
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 001 nickname :Welcome to the Omega IRC Network nickname!jeongjinse@localhost
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 002 nickname :Your host is penguin.omega.example.org, running version InspIRCd-3
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 003 nickname :This server was created 06:33:48 Jan 15 2024
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 004 nickname penguin.omega.example.org InspIRCd-3 iosw biklmnopstv :bklov
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 005 nickname AWAYLEN=200 CASEMAPPING=ascii CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=60 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=300 LINELEN=512 MAXLIST=b:100 :are supported by this server
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 005 nickname MAXTARGETS=20 MODES=20 NAMELEN=130 NETWORK=Omega NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=330 USERLEN=10 USERMODES=,,s,iow WHOX :are supported by this server
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 251 nickname :There are 0 users and 0 invisible on 1 servers
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 253 nickname 1 :unknown connections
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 254 nickname 0 :channels formed
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 255 nickname :I have 0 clients and 0 servers
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 265 nickname :Current local users: 0  Max: 1
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 266 nickname :Current global users: 0  Max: 1
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 375 nickname :penguin.omega.example.org message of the day
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname : _____                        _____   _____    _____      _
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :|_   _|                      |_   _| |  __ \  / ____|    | |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :  | |    _ __    ___   _ __    | |   | |__) || |       __| |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :  | |   | '_ \  / __| | '_ \   | |   |  _  / | |      / _` |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname : _| |_  | | | | \__ \ | |_) | _| |_  | | \ \ | |____ | (_| |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :|_____| |_| |_| |___/ | .__/ |_____| |_|  \_\ \_____| \__,_|
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname : _____________________| |__________________________________
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :|_____________________|_|__________________________________|
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :                         Powering IRC communities since 2006
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :       /\     /\
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      {  `---'  }  WELCOME TO AN 13I04N07S08P03I10R12C06D99 NETWORK
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      {  O   O  }
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      ~~>  V  <~~    If you see this, I'm probably new.
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :       \  \|/  /       If I'm not new, my owner is lazy. ;-)
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :        `-----'____
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :        /     \    \_
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :       {       }\  )_\_   _
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :       |  \_/  |/ /  \_\_/ )
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :        \__/  /(_/     \__/
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :          (__/
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      +---- To change this see motd.txt.example ----+
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |                                             |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |   * Web: https://www.inspircd.org           |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |   * IRC: irc.inspircd.org #inspircd         |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |   * Docs: https://docs.inspircd.org         |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |   * Issues: https://git.io/JIuYi            |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |   * Discussions: https://git.io/JIuYv       |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |                                             |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      | We hope you like this software.  Please do  |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      | make sure to customise your configuration,  |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      | though, so you love it.  Enjoy.             |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |                                             |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      |                        -- The InspIRCd Team |
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 372 nickname :      +---------------------------------------------+
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org 376 nickname :End of message of the day.
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org PRIVMSG nickname :*** Raw I/O logging is enabled on this server. All messages, passwords, and commands are being recorded.
Mon Jan 15 2024 06:37:38 CONNECT: Client connecting on port 6667 (class main): nickname!jeongjinse@localhost (127.0.0.1) [Jeong JinSeok]
Mon Jan 15 2024 06:37:38 BANCACHE: BanCache: Adding NEGATIVE hit for 127.0.0.1
Mon Jan 15 2024 06:37:38 USERINPUT: C[422AAAAAB] I MODE nickname +i
Mon Jan 15 2024 06:37:38 USEROUTPUT: C[422AAAAAB] O :nickname!jeongjinse@localhost MODE nickname :+i
Mon Jan 15 2024 06:37:43 USERINPUT: C[422AAAAAB] I PING penguin.omega.example.org
Mon Jan 15 2024 06:37:43 USEROUTPUT: C[422AAAAAB] O :penguin.omega.example.org PONG penguin.omega.example.org :penguin.omega.example.org
*/

std::string Irc::_001_rpl_welcome(std::string prefix, std::string clientnick, std::string user_prefix)
{
	std::string str = prefix + " 001 " + clientnick + " :" + user_prefix + "\r\n";
	str += prefix + " 002 " + clientnick + " :" + user_prefix + "\r\n";
	str += prefix + " 003 " + clientnick + " :" + user_prefix + "\r\n";
	str += prefix + " 004 " + clientnick + " :" + user_prefix + "\r\n";
	str += prefix + " 005 " + clientnick + " :" + user_prefix + "\r\n";
	return (str);
}

std::string Irc::_321_rpl_liststart(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 321 " + clientnick + " Channel :Users  Name" + "\r\n";
	return (str);
}

std::string Irc::_322_rpl_list(std::string prefix, std::string clientnick, std::string channelname, std::string usercnt, std::string topic)
{
	std::string str = prefix + " 322 " + clientnick + " " + channelname + " " + usercnt + " :" + topic + "\r\n";
	return (str);
}

std::string Irc::_323_rpl_listend(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 323 " + clientnick + " :End of /LIST" + "\r\n";
	return (str);
}

std::string Irc::_352_rpl_whoreply(std::string prefix, std::string clientnick, std::string channelname, std::string username, std::string nickname, std::string op, std::string realname)
{
	std::string str = prefix + " 352 " + clientnick + " " + channelname + " " + username + " " + HOST + " " + SERVERNAME + " " + nickname + " H" + op + " :0";
	str += " " + realname + "\r\n";
	return (str);
}

std::string Irc::_315_rpl_endofwho(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 315 " + clientnick + " " + channelname + " :End of /WHO list." + "\r\n";
	return (str);
}

std::string Irc::_331_rpl_notopic(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 331 " + clientnick + " " + channelname + " :No topic is set." + "\r\n";
	return (str);
}

std::string Irc::_332_rpl_topic(std::string prefix, std::string clientnick, std::string channelname, std::string topic)
{
	std::string str = prefix + " 332 " + clientnick + " " + channelname + " :" + topic + "\r\n";
	return (str);
}

std::string Irc::_341_rpl_inviting(std::string prefix, std::string clientnick, std::string channelname, std::string nick)
{
	std::string str = prefix + " 341 " + clientnick + " " + channelname + " " + nick + "\r\n";
	return (str);
}

std::string Irc::_324_rpl_channelmodeis(std::string prefix, std::string clientnick, std::string channelname, std::string mode, std::string mode_params)
{
	std::string str = prefix + " 324 " + clientnick + " " + channelname  + " " + mode  + " " + mode_params + "\r\n";
	return (str);
}
