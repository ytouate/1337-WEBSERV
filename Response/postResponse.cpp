/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:57:39 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/05 00:18:19 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
std::string _checker;
int     Response::checkPathOfPostmethod(serverParse& server, std::string line, int index)
{
    std::string path;
    path = line;
    _postPath = path;
    std::cout << "path = " << path << std::endl;
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
    return true;
}

void    Response::postResponse(void)
{
    char buffer[100];
    sprintf(buffer, "HTTP/1.1 %d\r\n", _statusCode);
    this->_response += buffer;
    _header += buffer;
    sprintf(buffer, "content-type : %s\r\n\r\n", "text/html");
    this->_response += buffer;
    _header += buffer;
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
    else if (path == "application/zip")  this->_contentType = ".zip";
}

#define CHUNK 100

int     Response::postMethod(Config& config)
{
    srand(time(0));
    int number = rand() / 3;
    if (getMatchedLocation(config) == true)
    {
        std::cout << "size = " << request.body.content.size() << std::endl;
        if (_indexLocation != -1)
        {
            if (request.body.content.size() > 0 && request.body.contentName.size() == 0)
            {
                postType(request.body.contentType);
                request.body.contentName = std::to_string(number) + _contentType;
            }
            if (request.body.content.size() < 4)
            {
                _statusCode = 403;
                _body += "<h1> BODY KHAWI </h1>";
                postResponse();
                return 0;
            }
            if (_uploadPath.size() > 0)
            {
                DIR *dir = opendir(_uploadPath.c_str());
                if (!dir)
                    _uploadPath = "./upload/";
            }
            else
                _uploadPath = "./upload/";
            _statusCode = 201;
            _uploadPath += "upload_";
            _uploadPath += request.body.contentName;
            int fd = open(_uploadPath.c_str() , O_CREAT | O_TRUNC | O_RDWR , 0644);
            int sent = 0;
            int remaining = request.body.content.size();
            while (sent < (int)request.body.content.size())
            {
                int bytes = std::min(remaining, CHUNK);
                const char *chunkedStr = request.body.content.c_str() + sent;
                int ret = write(fd, chunkedStr, bytes);
                if (ret == -1)
                    break ;
                remaining -= ret;
                sent += ret;
            }
            _body += "<h1> kolchi daze </h1>";
            postResponse();
            return 0;
        }
        _indexLocation = 0;
        DIR *dir = opendir(_uploadPath.c_str());
        if (dir)
        {
            if (config.servers[_indexServer].locations[_indexLocation].data["index"].size() > 0)
            {
                if (config.servers[_indexServer].locations[_indexLocation].data["cgi_path"].size() > 0)
                {
                    executeCgi(config.servers[_indexServer], _indexLocation);
                    return 0;
                }
            }
            errorPages(config.servers[_indexServer], _indexLocation, 403);
        }
        else
        {
            if (config.servers[_indexServer].locations[_indexLocation].data["cgi_path"].size() > 0)
            {
                executeCgi(config.servers[_indexServer], _indexLocation);
                return 0;
            }
            errorPages(config.servers[_indexServer], _indexLocation, 403);
        }
    }
    else
    {
            std::cout << _postPath << std::endl;
        errorPages(config.servers[_indexServer], _indexLocation, 404);
    }
    postResponse();
    return 0;
}