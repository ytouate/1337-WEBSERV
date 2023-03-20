NAME = webserv

FILES = \
	./Config.cpp	\
	./Server.cpp	\
	./main.cpp		\
	./Location.cpp	\
	./Request.cpp

CC = c++ 
O_FILE = $(FILES:.cpp=.o)
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

all: $(NAME)

$(NAME): $(O_FILE)
	$(CC) $(CPPFLAGS) $(O_FILE) -o $(NAME)

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all