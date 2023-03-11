# NAME = webserv

# FILES = \
# 	./Server.cpp	\
# 	./main.cpp		\

# CC = C++
# O_FILE = $(FILES:.cpp=.o)
# CPPFLAGS = -Wall -Wextra -Werror  -fsanitize=address -g

# all: $(O_FILE)
# 	$(CC) $(CPPFLAGS)  $(O_FILE) -o $(NAME)

# clean:
# 	rm -rf *.o

# fclean: clean
# 	rm -rf $(NAME)

# re: fclean all
all:
	c++ -Wall -Wextra -Werror -fsanitize=address -g *.cpp
run:
	./a.out nginx.conf