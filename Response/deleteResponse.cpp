/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 01:28:20 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/01 23:34:20 by otmallah         ###   ########.fr       */
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
        int _ok = 0;
        while (name != NULL)
        {
            std::string file = path + "/" + name->d_name;
            if (stat(file.c_str(), &filestat) == 0) 
            {
                if (S_ISDIR(filestat.st_mode) || !(filestat.st_mode & S_IWUSR))
                {
                    std::cout << "you don't have permission" << std::endl;
                    _ok = 1;
                    break;
                }
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
    else if (stat(path.c_str(), &filestat) == 0)
    {
        if (filestat.st_mode & S_IWUSR)
            remove(path.c_str());
    }
    return 0;
}