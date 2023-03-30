/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:57:39 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/30 14:58:51 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
std::string _checker;
int     Response::checkPathOfPostmethod(serverParse& server, std::string line, int index)
{
    std::string upload = "on";
    std::string path;
    static int i = 0;
    path = server.locations[index].data["root"][0] + line;
    if (i == 0)
        _checker = line;
    std::cout << _checker << std::endl;
    std::cout << line << std::endl;
    if (_checker != line || _checker == line)
        path = line;
    _postPath = path;
    std::cout << server.data["upload"].front() << std::endl;
    if (server.data["upload"].size() > 0 && server.data["upload"].front() == "on")
        _indexLocation = 2;
    DIR *dir = opendir(path.c_str());
    std::cout << "path = " << path << std::endl;
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
    i++;
    return true;
}

void    Response::postResponse(void)
{
    char buffer[100];
    sprintf(buffer, "HTTP/1.1 %d ok\r\n\r\n", _statusCode);
    this->_response += buffer;
    sprintf(buffer, "<h1> kolchi daze </h1>");
    this->_response += buffer;
}

int     Response::postMethod(Config& config)
{
    std::cout << request.data["body"] << std::endl;
    if (getMatchedLocation(config) == true)
    {
        if (_indexLocation == 2)
        {
            _statusCode = 201;
            int fd = open("./upload/13.html" , O_CREAT | O_TRUNC | O_RDWR , 0644);
            write(fd, request.data["body"].c_str(), request.data["body"].size());
            postResponse();
        }
    }
    std::cout << _response << std::endl;
    return 0;
}