/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:57:39 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/01 21:51:08 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
std::string _checker;
int     Response::checkPathOfPostmethod(serverParse& server, std::string line, int index)
{
    std::string path;
    static int i = 0;
    std::cout <<  "i = " << i  << std::endl;
    // if (i == 0)
    // {
    //     path = server.locations[index].data["root"][0] + line;
    //     _checker = line;
    // }
    // if (_checker != line )
        path = line;
    _postPath = path;
    std::cout << _checker << std::endl;
    std::cout << line << std::endl;
    std::cout << path << std::endl;
    if (server.data["upload"].size() > 0 && server.data["upload"].front() == "on")
    {
        if (server.data["upload_path"].size() > 0)
            _uploadPath = server.data["upload_path"].front();
        _indexLocation = index;
    }
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
    if (path[path.size() - 1] != '/')
    {
        std::cout << "301 moved -> path = " << path << std::endl;
        path += "/";
        _statusCode = 301;
        postResponse();
        return false;
    }
    i++;
    return true;
}

void    Response::postResponse(void)
{
    char buffer[100];
    sprintf(buffer, "HTTP/1.1 %d\r\n", _statusCode);
    this->_response += buffer;
    sprintf(buffer, "content-type : %s\r\n\r\n", "text/html");
    this->_response += buffer;
    this->_response += _body;
}

void    Response::postType(std::string path)
{
    if (path == "text/css") this->_contentType = ".css";
    else if (path == "text/csv") this->_contentType = ".csv";
    else if (path == "image/gif") this->_contentType = ".gif";
    else if (path == "text/html") this->_contentType = ".htm";
    else if (path == "text/html" || path == ".php") this->_contentType = ".html";
    else if (path == "image/x-icon") this->_contentType = ".ico";
    else if (path == "image/jpeg") this->_contentType = ".jpeg";
    else if (path == "image/jpeg") this->_contentType = ".jpg";
    else if (path == "application/javascript")  this->_contentType = ".js";
    else if (path == "application/json") this->_contentType = ".json";
    else if (path == "image/png")  this->_contentType = ".png";
    else if (path == "application/pdf")  this->_contentType = ".pdf";
    else if (path == "image/svg+xml")  this->_contentType = ".svg";
    else if (path == "text/plain")  this->_contentType = ".txt";
    else if (path == "video/mp4")  this->_contentType = ".mp4";
    else if (path == "video/webm")  this->_contentType = ".WebM";
    else if (path == "video/ogg")  this->_contentType = ".Ogg";
    else if (path == "video/x-msvideo")  this->_contentType = ".AVI";
    else if (path == "video/mpeg")  this->_contentType = ".MPEG";
}

int     Response::postMethod(Config& config)
{
    srand(time(0));
    // int number = rand() / 2;
    if (getMatchedLocation(config) == true)
    {
        std::cout << request.data["body"] << std::endl;
        // if (_indexLocation != -1)
        // {
        //     if (_uploadPath.size() > 0)
        //     {
        //         // DIR *dir = opendir(_uploadPath.c_str());
        //         _uploadPath = "./upload/";
        //     }
        //     _statusCode = 201;
        //     _uploadPath += std::to_string(number) + ".html";
        //     int fd = open(_uploadPath.c_str() , O_CREAT | O_TRUNC | O_RDWR , 0644);
        //     write(fd, request.data["body"].c_str(), request.data["body"].size());
        //     _body += "<h1> kolchi daze </h1>";
        //     postResponse();
        //     return 0;
        // }
        // DIR *dir = opendir(_postPath.c_str());
        // if (dir)
        // {
            
        // }
    }
    else
        errorPages(config.servers[_indexServer], _indexLocation, 404);
    postResponse();
    std::cout << _response << std::endl;
    return 0;
}