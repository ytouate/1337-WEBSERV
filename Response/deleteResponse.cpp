/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 01:28:20 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/01 20:46:56 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sys/stat.h>

int     Response::deleteMethod(Config& config)
{
    (void)config;
    struct stat filestat;
    std::string path = "//Users/otmallah/Desktop/1337-WebServ/test";
    path += request.data["data"];
    std::cout << "path = "<< path << std::endl;
    DIR *dir = opendir(path.c_str());
    dirent *name = readdir(dir);
    std::cout << path << std::endl;
    while (name != NULL)
    {
        std::string file = path + "/" + name->d_name;
        if (stat(file.c_str(), &filestat) == 0) 
        {
            if (filestat.st_mode & S_IWUSR)
            {
                std::cout << "yes" << std::endl;
                remove(file.c_str());
            } 
        }
        name = readdir(dir);
    }
    
    return 0;
}