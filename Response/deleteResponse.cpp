/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 01:28:20 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/01 21:25:30 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sys/stat.h>

int     Response::deleteMethod(Config& config)
{
    (void)config;
    struct stat filestat;
    std::string path = "/Users/otmallah/Desktop/1337-WebServ/test";
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        dirent *name = readdir(dir);
        std::cout << path << std::endl;
        int _ok = 0;
        while (name != NULL)
        {
            std::string file = path + "/" + name->d_name;
            if (stat(file.c_str(), &filestat) == 0) 
            {
                if (!(filestat.st_mode & S_IWUSR))
                    _ok = 1;
            }
            name = readdir(dir);
        }
        if (_ok == 0)
        {
            dir = opendir(path.c_str());
            dirent *name = readdir(dir);
            while (name != NULL)
            {
                std::string file = path + "/" + name->d_name;
                remove(file.c_str());
                name = readdir(dir);
            }
        }
        return 0;
    }
    if (stat(path.c_str(), &filestat) == 0)
    {
        if (filestat.st_mode & S_IWUSR)
            remove(path.c_str());
    }
    return 0;
}