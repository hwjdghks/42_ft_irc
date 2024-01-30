NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC_PATH = ./src
SRC_NAME = Server.cpp Client.cpp Channel.cpp Irc.cpp main.cpp
SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))

OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJ_PATH = ./obj
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

INC_LINK = -I./inc 

all : $(NAME)

$(NAME) : $(OBJ_PATH) $(OBJ)
	c++	$(FLAGS) $(OBJ) $(INC_LINK) -o $(NAME) 

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	c++ $(FLAGS) $(INC_LINK) -o $@ -c $<

clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(OBJ) $(NAME)

re : fclean all

.PHONY: all clean fclean re