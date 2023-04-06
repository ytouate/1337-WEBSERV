NAME = webserv

FILES = \
	./Parse/Config.cpp				\
	./Parse/serverParse.cpp			\
	./Parse/locationParse.cpp		\
	./Parse/requestParse.cpp 		\
	./Response/getResponse.cpp		\
	./Response/postResponse.cpp		\
	./Response/deleteResponse.cpp	\
	./Server/Server.cpp				\
	./main.cpp

CC = c++ 
CPPFLAGS = -Wall -Wextra -Werror  -std=c++98 -fsanitize=address -g

O_FILE = $(FILES:.cpp=.o)

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

run:
	./webserv nginx.conf