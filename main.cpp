#include "inc.hpp"


typedef std::map<std::string, std::vector<std::string> > directive;

void printDrective(directive &a)
{
	std::cout << "root:\t";
	for (size_t i = 0; i < a["root"].size(); ++i)
		std::cout << " " << a["root"][i];
	std::cout << "\n";
	std::cout << "index:\t";
	for (size_t i = 0; i < a["index"].size(); ++i)
		std::cout << " " << a["index"][i];
	std::cout << "\n";
	std::cout << "listen:\t";
	for (size_t i = 0; i < a["listen"].size(); ++i)
		std::cout << " " << a["listen"][i];
	std::cout << "\n";
}

void error(const char *s)
{
	perror(s);
	exit(1);
}

// int main(int ac, char **av)
// {

//     if (ac != 2)
//         return 1;
//     fd_set our_fds;
//     int fd = STDIN_FILENO;
//     FD_ZERO(&our_fds);
//     FD_SET(fd, &our_fds);
//     struct timeval time;
//     time.tv_sec = 2;
//     time.tv_usec = 0;

//     if ( select(2, 0, &our_fds,0,&time) <= 0)
//         perror("select()");
//     for (int i = 0; i <= 2; i++)
//     {
//         if (FD_ISSET(i, &our_fds))
//             std::cout << "hona\n";
//     }
//     return -1;
//     AConfig config(av[1]);
//     for (int i = 0; i < config.servers.size(); ++i)
//     {
//         std::cout << "Server[" << i << "]" << std::endl;
//         for (int j = 0; j < config.servers[i].locations.size(); ++j)
//         {
//             std::cout << "Server[" << i << "]" << "location[" << j << "]\n";
//             std::cout << "PATH: " << config.servers[i].locations[j].path << std::endl;
//         }
//         std::cout << "\n";
//     }
//     std::cout << config.servers[0].data["index"][0 ] << std::endl;
//     // std::cout << "=== config.server[0] Block data === " << std::endl;
//     // printDrective(config.servers[0].data);
//     // std::cout << "=== Location[0] data === " << std::endl;
//     // std::cout << config.servers[0].locations[0].path << std::endl;
//     // printDrective(config.servers[0].locations[0].data);
//     // std::cout << "=== Location[1] data === " << std::endl;
//     // std::cout << config.servers[0].locations[1].path << std::endl;
//     // printDrective(config.servers[0].locations[1].data);
//     // std::cout << "=== Location[2] data === " << std::endl;
//     // std::cout << config.servers[0].locations[2].path << std::endl;
//     // printDrective(config.servers[0].locations[2].data);
//     // std::cout << "\n === config.server[1] Block data === " << std::endl;
//     // printDrective(config.servers[1].data);
//     // std::cout << "=== Location[0] data === " << std::endl;
//     // std::cout << config.servers[1].locations[0].path << std::endl;
//     // printDrective(config.servers[1].locations[0].data);
//     // std::cout << "=== Location[1] data === " << std::endl;
//     // std::cout << config.servers[1].locations[1].path << std::endl;
//     // printDrective(config.servers[1].locations[1].data);
//     // std::cout << "=== Location[2] data === " << std::endl;
//     // std::cout << config.servers[1].locations[2].path << std::endl;
//     // printDrective(config.servers[1].locations[2].data);
//     return 0;
// }

int main(int ac, char **av)
{
	struct addrinfo hints, *data;
	fd_set fds;
	FD_ZERO(&fds);
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	int errorCode;
	if ((errorCode = getaddrinfo(av[1], av[2], &hints, &data)) == -1)
	{
		gai_strerror(errorCode);
		return 1;
	}

	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(data->ai_addr, data->ai_addrlen, address_buffer, 100, service_buffer, 100, NI_NUMERICHOST);
	std::cout << "remote server is running on: " << address_buffer << ":" << service_buffer << std::endl;
	// domain : type of ip address
	// type: socket type 
	// protocol tcp or udp
	int sockfd = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
	if (sockfd == -1)
		error("socket()");
	if (connect(sockfd, data->ai_addr, data->ai_addrlen) == -1)
		error("connect()");
	char read[4096];
	int bytes_recieved;
	while (1)
	{
		fd_set reads;
		FD_ZERO(&reads);
		FD_SET(sockfd, &reads);
		struct timeval time;
		time.tv_sec = 0;
		time.tv_usec = 100000;
		if (select(sockfd + 1, &reads, 0, 0, &time) < 0)
			error("select()");
		if (FD_ISSET(sockfd, &reads))
		{
			int bytes_recieved;
			char read[14];
			bytes_recieved = recv(sockfd, read, 14, 0);
			if (bytes_recieved < 0)
				error("recv()");
			if (bytes_recieved == 0)
				std::cout << "Recieved " << bytes_recieved << " bytes: " << read;
			
		}
		// if (FD_ISSET(0, &reads))
		// {
		//     int _bytes_recieved;
		//     char read[4096];
		//     if (!fgets(read, sizeof read, stdin)) break;
		//     std::cout << "Sending: " << read << std::endl;
		//     _bytes_recieved = send(sockfd, read, sizeof(read), 0);
		//     std::cout << "Sent " << _bytes_recieved << " bytes\n";
		// }
	}
	
}