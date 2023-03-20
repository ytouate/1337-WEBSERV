NAME = webserv

FILES = \
	./Parse/Config.cpp			\
	./Parse/serverParse.cpp		\
	./main.cpp					\
	./Parse/locationParse.cpp	\
	./Parse/requestParse.cpp

CC = c++ 
O_FILE = $(FILES:.cpp=.o)
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

all: $(NAME)

$(NAME): $(O_FILE)
	$(CC) $(CPPFLAGS) $(O_FILE) -o $(NAME)

clean:
	rm -rf Parse/*.o
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all