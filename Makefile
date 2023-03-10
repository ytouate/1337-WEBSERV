NAME = webserv

FILES = \
	./Lexer.cpp	\
	./Token.cpp	\
	./Server.cpp\

CC = C++
O_FILE = $(FILES:.cpp=.o)
CPPFLAGS =  -fsanitize=address -g

all: $(O_FILE)
	$(CC) $(CPPFLAGS) main.cpp $(O_FILE) -o $(NAME)

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all
