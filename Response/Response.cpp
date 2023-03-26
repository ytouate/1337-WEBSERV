/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:34:07 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/25 16:45:38 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


Response::~Response()
{
}

Response::Response(Config &config, requestParse& _request) : request(_request)
{
    getMethod(config);
}

int     Response::getIndexOfServerBlock(Config &config)
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


bool    Response::getMatchedLocation(Config& config)
{
    size_t index = 0;
    int finalPath = -1;
    size_t counterMatch = 0;
    size_t counterNoMatch = 0;
    size_t matchPath = 0;
    size_t sec_matchPath = 0;
    char *save;
    int indexServer = getIndexOfServerBlock(config);
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
    if (!checkPathIfValid(config.servers[indexServer], finalPath, line))
        return 1;
    return 0;
}

void    Response::errorPages(serverParse& server, int id, int statusCode)
{
    std::string path = "./index/";
    std::ifstream infile;
    std::string line;
    size_t size = server.locations[id].errorPages[statusCode].size();
    if (size > 0 || size == 0)
    {
        if (size == 0)
            path += "404.html";
        else
            path += server.locations[id].errorPages[statusCode];
        infile.open(path);
        while (getline(infile, line))
            _body += line;
        _statusCode = statusCode;
    }
}

bool    Response::methodAllowed(serverParse& server, int index)
{
    if (server.locations[index].data["allowed_methods"].size() > 0)
    {
        for (size_t i = 0; i < server.locations[index].data["allowed_methods"].size(); i++)
        {
            if (request.data["method"] == server.locations[index].data["allowed_methods"][i])
                return true;
        }
        errorPages(server, index, 405);
        return false;
    }
    return true;
}

bool    Response::validFile(serverParse& server, int index, std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::binary);
    if(file)
    {
        if (methodAllowed(server, index) == true)
        {
            file.seekg(0, std::ios::end);
            _contentLength = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<char> buffer(_contentLength);
            file.read(buffer.data(), _contentLength);
            size_t i = 0;
            while (i < buffer.size())
            {
                _body += buffer[i];
                i++;
            }
            _statusCode = 200;
            this->_requestPath = path;
            getContentType();
        }
        else
            return false;
    }
    else
    {
        errorPages(server, index, 404);
        return false;
    }
    return true;
}

bool    Response::checkPathIfValid(serverParse& server, int index , std::string line)
{
    static int i;
    std::string path;
    if (i == 0)
        path = server.locations[index].data["root"][0] + line;
    else
        path = line;
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return validFile(server, index, path);
    else
    {
        if (methodAllowed(server, index) == false)
            return false;
        _statusCode = 200;
        if (path[path.size() - 1] != '/')
        {
            path += "/";
            std::cout << "301 moved -> path = " << path << std::endl;
        }
        if (server.locations[index].data["index"].size() > 0 )
        {
            path += server.locations[index].data["index"][0];
            this->_requestPath = path;
            return validFile(server, index, path);
        }
        if (server.locations[index].autoIndex == true)
        {
            dirent *test ;
            std::string line;
            std::string content = "";
            while ((test = readdir(dir)) != NULL)
            {
                content += "<a href=\"";
                content += path + test->d_name;
                content += "\">";
                content += test->d_name ;
                content += "</a>";
                content += "\n";
                content += "<br>";
            }
            _contentType = "text/html";
            // FILE *file = fopen("t.html" , "w+");
            // if (file)
            //     fwrite(content.data(), sizeof(char), content.size(), file);
            _body += content;
        }
        else
        {
            errorPages(server, index, 404); return false;
        }
    }
    i++;
    return true;
}

void   Response::getContentType()
{
    std::string path = this->_requestPath;
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
    else if (path == ".mp4")  this->_contentType = "video/mp4";
    else if (path == ".WebM")  this->_contentType = "video/webm";
    else if (path == ".Ogg")  this->_contentType = "video/ogg";
    else if (path == ".AVI")  this->_contentType = "video/x-msvideo";
    else if (path == ".MPEG")  this->_contentType = "video/mpeg";
    else this->_contentType = "application/octet-stream";
}

void    Response::faildResponse()
{
    char buffer[100];
    sprintf(buffer, "HTTP/1.1 %d \r\n", this->_statusCode);
    this->_response += buffer;
    sprintf(buffer, "Connection: closed\r\n");
    this->_response += buffer;
    sprintf(buffer, "Content-Type: text/html\r\n\r\n");
    this->_response += buffer;
    this->_response += _body;
}

int    Response::getMethod(Config &config)
{
    std::string line = request.data["path"];
    if (getMatchedLocation(config) == 1 && _statusCode != 200)
    {
        getContentType();
        faildResponse();
        return (1);
    }
    else
    {
        this->_statusCode = 200;
        char buffer[100];
        sprintf(buffer, "%s %d OK\r\n", request.data["version"].c_str() , this->_statusCode);
        this->_response += buffer;
        if (_contentType != "text/html")
        {
            sprintf(buffer, "Content-Type: %s\r\n", this->_contentType.c_str());
            this->_response += buffer;
            sprintf(buffer, "Content-Length: %ld\r\n\r\n", this->_contentLength);
            this->_response += buffer;
        }
        else
        {
            sprintf(buffer, "Content-Type: %s\r\n\r\n", this->_contentType.c_str());
            this->_response += buffer;
        }
        _response += _body;
    }
    _requestPath = "";
   return 0; 
}

// bool    Response::validationRequestPath(Config& config, requestParse& request)
// {
//     if (getMatchedLocation(config, request) == false)
//         std::cout << "404 not found" << std::endl;
//     else
//     {
    
//     }
// }
