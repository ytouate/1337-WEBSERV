#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

int main ()
{
    int fd = open("./out" , O_CREAT | O_RDWR , 0644);
    std::string path1 = "./php-cgi";
    std::string path2 = "php.php";
    char *commad[] = {(char *)path1.c_str(), (char *)path2.c_str(), NULL};
    if (fork() == 0)
    {
        dup2(fd, 1);
        execve(commad[0], commad, NULL);
    }
}
