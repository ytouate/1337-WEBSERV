NAME = webserv

FILES = \
	./lexer.cpp	\
	./token.cpp

CC = C++
O_FILE = $(FILES:.cpp=.o)
# CPPFLAGS = -Wall -Wextra -Werror

all: $(O_FILE)
	$(CC) $(CPPFLAGS) main.cpp $(O_FILE) -o $(NAME)

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all
