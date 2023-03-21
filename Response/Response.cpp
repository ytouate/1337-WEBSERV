/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:34:07 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/21 10:03:45 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
    _requestPath = "";
}

Response::~Response()
{
}

int     Response::getIndexOfServerBlock(Config &config, requestParse &request)
{
    std::string host = request.data["host"];
    if (host.rfind('/') == std::string::npos)
    {
        for (size_t i = 0; i < config.servers.size() ; i++)
        {
            for (size_t j = 0; j < config.servers[i].data["server_name"].size(); j++)
            {
                if (host == config.servers[i].data["server_name"].at(j))
                    return i;
            }
        }
    }
    else
    {
        std::string port = host.erase(0, host.rfind('/') + 1);
        for (size_t i = 0; i < config.servers.size() ; i++)
        {
            for (size_t j = 0; j < config.servers[i].data["listen"].size(); j++)
            {
                if (port == config.servers[i].data["listen"].at(j))
                    return i;
            }
        }
    }
    return (0);
}


bool    Response::getMatchedLocation(Config& config, requestParse& request)
{
    size_t index = 0;
    int finalPath = -1;
    size_t counterMatch = 0;
    size_t counterNoMatch = 0;
    size_t matchPath = 0;
    size_t sec_matchPath = 0;
    char *save;
    int indexServer = getIndexOfServerBlock(config, request);
    std::string line = request.data["path"];
    for (size_t i = 0; i < config.servers[indexServer].locations.size(); i++)
    {
        save = (char *)config.servers[indexServer].locations[i].path.c_str(); 
        while (index < line.size() && save[index])
        {
            if (save[index] == line[index])
                counterMatch++;
            else
                counterNoMatch++;
            index++;
        }
        if (i == 0 || ((counterMatch > matchPath) && (counterNoMatch <= sec_matchPath)))
        {
            finalPath = i;
            matchPath = counterMatch;
            sec_matchPath = counterNoMatch;
        }
        index = 0;
        counterMatch = 0;
        counterNoMatch = 0;
    }
    // std::cout << server.locations[finalPath].path << std::endl;
    if (!checkPathIfValid(config.servers[indexServer] , finalPath, line))
        return 1;
    return 0;
}

bool    Response::checkPathIfValid(serverParse& server, int index , std::string line)
{
    std::string path = server.locations[index].data["root"][0] + line;
    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        std::ifstream file;
        file.open(path);
        if(file)
            this->_requestPath = path;
        else
            return false;
    }
    else
    {
        this->_requestPath = path;
    }
    return true;
}

void   Response::getContentType()
{
    std::string path = this->_requestPath;
    std::cout << path.erase(0, path.rfind('.')) << std::endl;
    try
    {
        path = path.erase(0, path.rfind('.'));
    }
    catch(...)
    {
        this->_contentType = "application/octet-stream";
    }
    if (path == ".css") this->_contentType = "text/css";
    else if (path == ".csv") this->_contentType = "text/csv";
    else if (path == ".gif") this->_contentType = "image/gif";
    else if (path == ".htm") this->_contentType = "text/html";
    else if (path == ".html") this->_contentType = "text/html";
    else if (path == ".ico") this->_contentType = "image/x-icon";
    else if (path == ".jpeg") this->_contentType = "image/jpeg";
    else if (path == ".jpg") this->_contentType = "image/jpeg";
    else if (path == ".js")  this->_contentType = "application/javascript";
    else if (path == ".json") this->_contentType = "application/json";
    else if (path == ".png")  this->_contentType = "image/png";
    else if (path == ".pdf")  this->_contentType = "application/pdf";
    else if (path == ".svg")  this->_contentType = "image/svg+xml";
    else if (path == ".txt")  this->_contentType = "text/plain";
    else this->_contentType = "application/octet-stream";
}

void    Response::faildResponse()
{
    char buffer[100];
    this->_statusCode = 404;
    sprintf(buffer, "GET HTTP/1.1 %d not found\r\n", this->_statusCode);
    this->_response += buffer;
    sprintf(buffer, "Connection: closed\r\n\r\n");
    this->_response += buffer;
    std::ifstream infile;
    infile.open("/Users/otmallah/Desktop/1337-WebServ/index1.html");
    std::string line;
    while (getline(infile, line))
        this->_response += line;
    std::cout << _response << std::endl;
}

int    Response::getMethod(Config &config, requestParse& request)
{
    std::string line = request.data["path"];
    getMatchedLocation(config, request);
    getContentType();
    std::ifstream infile;
    infile.open(this->_requestPath);
    if (!infile)
    {
        faildResponse();
        return 1;
    }
    else
    {
        this->_statusCode = 200;
        char buffer[100];
        sprintf(buffer, "GET %s  %d OK\r\n", request.data["version"].c_str() , this->_statusCode);
        this->_response += buffer;
        sprintf(buffer, "Content-Type: %s\r\n\r\n", this->_contentType.c_str());
        this->_response += buffer;
        std::string line1;
        while (getline(infile, line1))
            this->_response += line1;
    }
    std::cout << _response << std::endl;
   return 0; 
}
