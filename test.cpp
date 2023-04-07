#include <iostream>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>


// int main(int ac, char **av, char **env)
// {
//     int _pipe[2];
//     std::string _body;
//     char *args[2] = {(char *)"/Users/otmallah/Desktop/1337-WebServ/index/script.php", NULL};
//     char *command = (char *)"/usr/bin/php";
//     pipe(_pipe);
//     int id = fork();
//     if (!id)
//     {
//         dup2(_pipe[1], STDOUT_FILENO);
//         close(_pipe[1]);
//         close(_pipe[0]);
//         execve(command, args, env);
//         perror("execve()");
//         exit(1);
//     }

//     wait(NULL);
//     close(_pipe[1]);
//     char buffer[100];
//     int bytes = read(_pipe[0], buffer, 100);
//     // buffer[bytes] = '\0';
//     std::cout << buffer << std::endl;
//     close(_pipe[0]);
//     std::cout << _body << std::endl;
// }


#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

// int main(int ac, char **av, char **env)
// {
//     int _pipe[2];
//     char *args[2] = {(char *)"/Users/otmallah/Desktop/1337-WebServ/index/script.php", NULL};
//     char *command = (char *)"/usr/bin/php";
//     pipe(_pipe);
//     pid_t id = fork();
//     if (id == -1)
//     {
//         perror("fork");
//         exit(1);
//     }
//     else if (id == 0)
//     {
//         dup2(_pipe[1], STDOUT_FILENO);
//         close(_pipe[1]);
//         close(_pipe[0]);
//         execve(command, args, env);
//         perror("execve");
//         exit(1);
//     }
//     close(_pipe[1]);
//     char buffer[101];
//     int bytes = read(_pipe[0], buffer, 100);
//     if (bytes == -1)
//     {
//         perror("read");
//         exit(1);
//     }
//     buffer[bytes] = '\0';
//     close(_pipe[0]);
//     std::cout << buffer << std::endl;

//     int status;
//     waitpid(id, &status, 0);
//     if (WIFEXITED(status))
//     {
//         int exit_status = WEXITSTATUS(status);
//         std::cout << "Child process exited with status " << exit_status << std::endl;
//     }
//     else if (WIFSIGNALED(status))
//     {
//         int signal_number = WTERMSIG(status);
//         std::cout << "Child process terminated by signal " << signal_number << std::endl;
//     }

//     return 0;
// }
