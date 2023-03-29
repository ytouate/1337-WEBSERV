/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:57:39 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/29 21:46:29 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int     Response::checkPathOfPostmethod(serverParse& server, std::string line, int index)
{
    std::string path;
    path = server.locations[index].data["root"][0] + line;
    _postPath = path;
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
        return false;
    _statusCode = 200;
    if (path[path.size() - 1] != '/')
    {
        path += "/";
        std::cout << "301 moved -> path = " << path << std::endl;
    }
    return true;
}


int     Response::postMethod(Config& config)
{
    std::string body = "<h1> Hello post method </h1>";
    std::string type = ".html";
    if (getMatchedLocation(config) == 1)
    {
        DIR *dir = opendir(_postPath.c_str());
        if (dir)
        {
            std::string file = "." + request.data["path"] + "/" + "test" + type;
            int fd = open(file.c_str(), O_CREAT | O_RDWR , 0644);
            std::cout << fd << std::endl;
            write(fd, body.c_str(), body.size());
        }
        
    }
    return 0;
}