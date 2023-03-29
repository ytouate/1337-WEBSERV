NAME = webserv

FILES = \
	./Parse/Config.cpp			\
	./Parse/serverParse.cpp		\
	./Parse/locationParse.cpp	\
	./Parse/requestParse.cpp 	\
	./Response/getResponse.cpp	\
	./Response/postResponse.cpp	\
	./Server/Server.cpp

CC = c++ 
O_FILE = $(FILES:.cpp=.o)
CPPFLAGS = -Wall -Wextra -Werror  -std=c++98 -fsanitize=address -g

all: $(NAME)

$(NAME): $(O_FILE)
	$(CC) $(CPPFLAGS) $(O_FILE) -o $(NAME)

clean:
	rm -rf Parse/*.o
	rm -rf Response/*.o
	rm -rf Server/*.o
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all