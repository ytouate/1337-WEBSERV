/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 01:28:20 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/08 16:13:09 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sys/stat.h>

int     Response::checkPathOfDeletemethod(Config::serverParse& server, std::string line, int index)
{
    std::string path;
    _indexLocation = index;
    path = server.locations[index].data["root"][0] + line;
    _deletePath = path;
    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        std::ifstream infile;
        infile.open(path.c_str());
        if (infile)
            return true;
        return false;
    }
    if (methodAllowed(server, index) == false)
    {
        return false;
    }
    if (path[path.size() - 1] != '/')
    {
        std::cout << "301 moved -> path = " << path << std::endl;
        path += "/";
        _deletePath = path;
        _statusCode = 301;
    }
    return true;
}

void    Response::success()
{
    std::ifstream infile("./index/200.html");
    std::string line;
    while (getline(infile,  line))
        _body += line;
    postResponse();
}

int deleteDirectory(const std::string& path)
{
    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        return -1;
    }
    
    dirent* entry;
    struct stat statbuf;
    
    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            std::string filepath = path + "/" + entry->d_name;
        
            stat(filepath.c_str(), &statbuf);
            std::cout << filepath << std::endl;
            if (S_ISDIR(statbuf.st_mode))
            {
                deleteDirectory(filepath);
            }
            else
            {
                if (remove(filepath.c_str()) != 0)
                {
                    
                }
            }
        }
    }
    closedir(dir);
    rmdir(path.c_str());
    return 0;
}

int     Response::deleteMethod(Config& config)
{
    struct stat filestat;
    if (getMatchedLocation(config) == false)
    {
        errorPages(config.servers[0], 0, 404);
        postResponse();
        return 0;
    }
    DIR *dir = opendir(_deletePath.c_str());
    if (dir)
    {
        dirent *name = readdir(dir);
        while (name != NULL)
        {
            dir = opendir(_deletePath.c_str());
              dirent* entry;

                while ((entry = readdir(dir)))
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    {
                        std::string filepath = _deletePath + "/" + entry->d_name;
                        struct stat statbuf;
                        if (stat(filepath.c_str(), &statbuf) == -1)
                            std::cerr << "Error get stat of =  " << filepath << std::endl;
    
                        if (S_ISDIR(statbuf.st_mode))
                        {
                            if (deleteDirectory(filepath) != 0)
                               {}
                        }
                        else
                            remove(filepath.c_str());
                    }
                }
            success();
            return 0;
        }
        errorPages(config.servers[_indexServer], _indexLocation, 403);
        postResponse();
        return 0;
    }
    else if (stat(_deletePath.c_str(), &filestat) == 0)
    {
        if (filestat.st_mode & S_IWUSR)
        {
            remove(_deletePath.c_str());
            success();
        }
        else
            errorPages(config.servers[_indexServer], _indexLocation, 403);
    }
    return 0;
}
