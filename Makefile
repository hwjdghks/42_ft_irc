CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
CPPFLAGS = $(CXXFLAGS)
NAME = ircserv
RM = rm -rf
# Directoris
SRC_PATH = ./src/
OBJ_PATH = ./obj/
INC_PATH = ./inc/

# FILES
SRC_NAME = main.cpp \
			Server.cpp Server_utils.cpp \
			Irc.cpp Irc_regi.cpp Irc_rpl.cpp Irc_err.cpp \
			Channel.cpp \
			Client.cpp

OBJS = $(addprefix $(OBJ_PATH), $(SRC_NAME:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_PATH)%.o : $(SRC_PATH)%.cpp
	@test -d $(OBJ_PATH) || mkdir -p $(OBJ_PATH)
	$(CXX) $(CPPFLAGS) -o $@ -c $< -I$(INC_PATH)

clean:
	$(RM) $(OBJ_PATH)

fclean: clean
	$(RM) $(NAME)

re:
	make fclean
	make all

server:
	./ircserv 16667 password

client:
	irssi -c 127.0.0.1 -p 16667 -n nickname -w password

.PHONY: all clean fclean re