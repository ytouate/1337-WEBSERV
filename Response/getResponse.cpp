/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:34:07 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/08 21:23:05 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <stdlib.h>
#include "../Server/Server.hpp"
#include "../Parse/Config.hpp"
#include <sys/stat.h>

Response::~Response()
{
}

Response::Response(Config &config, requestParse& _request) : request(_request)
{
    _indexLocation = -1;
    std::cout << request.data["method"] << std::endl;
    if (request.data["method"] == "GET")
        getMethod(config);
    if (request.data["method"] == "DELETE")
        deleteMethod(config);
    if (request.data["method"] == "POST")
        postMethod(config);
}

int    Response::validateRequest()
{
    int i = 0;
    if (request.data["path"].size() > 2048)
        return 414;
    while (request.data["path"][i])
    {
        if ((request.data["path"][i] >= 48 && request.data["data"][i] <= 59) ||
            (request.data["path"][i] >= 61 && request.data["data"][i] <= 93) ||
            (request.data["path"][i] >= 97 && request.data["data"][i] <= 122) ||
            (request.data["path"][i] >= 36 && request.data["data"][i] <= 47) ||
            (request.data["path"][i] == 95))
            i++;
        else
            return 400;
    }
    return 0;
}

int     Response::getIndexOfServerBlock(Config &config)
{
    std::string host = request.data["host"];
    
    host.erase(std::remove_if(host.begin(), host.end(), ::isspace));
    if (host.rfind('/') == std::string::npos and host.rfind(':') == std::string::npos)
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
        port = host.erase(0, host.rfind(':') + 1);
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
    std::cout << line << std::endl;
    for (size_t i = 0; i < config.servers[indexServer].locations.size(); i++)
    {
        _indexServer = indexServer;
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
            if (counterMatch)
            finalPath = i;
            matchPath = counterMatch;
            sec_matchPath = counterNoMatch;
        }
        index = 0;
        counterMatch = 0;
        counterNoMatch = 0;
    }
    if (finalPath == -1)
    {
        errorPages(config.servers[indexServer], 0, 404);
        return 1;
    }
    if (config.servers[indexServer].locations[finalPath].data["body_size"].size() > 0)
    {
        if (request.data["body_size"].size() > 0 and request.data["body_size"] > config.servers[indexServer].locations[finalPath].data["body_size"][0])
            return 1;
    }
    if (request.data["method"] == "POST") return checkPathOfPostmethod(config.servers[indexServer], line, finalPath);
    if (request.data["method"] == "DELETE") return checkPathOfDeletemethod(config.servers[indexServer], line, finalPath);
    if (!checkPathIfValid(config.servers[indexServer], finalPath, line))
        return 1;
    return 0;
}

void    Response::errorPages(Config::serverParse& server, int id, int statusCode)
{
    std::string path = "./index/";
    std::ifstream infile;
    std::string line;
    size_t size = server.locations[id].errorPages[statusCode].size();
    if (size > 0)
    {
        path += server.locations[id].errorPages[statusCode];
        infile.open(path.c_str());
        while (getline(infile, line))
            _body += line;
        _statusCode = statusCode;
    }
    else
    {
        if (statusCode == 400)
            path += "400.html";
        if (statusCode == 404)
            path += "404.html";
        if (statusCode == 405)
            path += "405.html";
        if (statusCode == 413)
            path += "413.html";
        if (statusCode == 414)
            path += "414.html";
        if (statusCode == 501)
            path += "501.html";
        if (statusCode == 403)
            path += "403.html";
        if (statusCode == 401)
            path += "401.html";
        infile.open(path.c_str());
        while (getline(infile, line))
            _body += line;
        _statusCode = statusCode;
    }
}

bool    Response::methodAllowed(Config::serverParse& server, int index)
{
    if (server.locations[index].data["allowed_methods"].size() > 0)
    {
        for (size_t i = 0; i < server.locations[index].data["allowed_methods"].size(); i++)
        {
            if (request.data["method"] == server.locations[index].data["allowed_methods"][i])
            {
                return true;
            }
        }
        errorPages(server, index, 405);
        return false;
    }
    return true;
}

std::vector<std::string>    Response::setEnv()
{
    std::string line = "REQUEST_METHOD,SCRIPT_FILENAME,CONTENT_LENGTH,CONTENT_TYPE,QUERY_STRING,REDIRECT_STATUS";
    std::string temp = line;
    std::string result;
    std::vector<std::string> vec;
    for (size_t i = 0; i < 6; i++)
    {
        if (temp.rfind(",") != std::string::npos)
        {
            temp.erase(0, temp.rfind(","));
            vec.push_back(temp.c_str() + 1);
            line.erase(line.rfind(","), line.size());
            temp = line;
        }
        else
            vec.push_back(temp);
    }
    vec[1] += "=" + request.data["path"].erase(0, request.data["path"].rfind("?"));
    if (_flag == 1)
        vec[4] += "=" + _postPath;
    else
        vec[4] += "=" + _getPath;
    vec[3] += "=" + request.data["content-length"];
    vec[2] += "=" + request.data["content-type"];
    vec[5] += "=" + request.data["method"];
    vec[0] += "=" + std::to_string(200);
    return vec;
}



bool Response::executeCgi(Config::serverParse& , int, int flag)
{
    _flag = flag;
    std::vector<std::string> _env = setEnv();
    char *env[_env.size()  + 1];
    for (size_t i = 0; i < _env.size(); i++)
        env[i] = (char *)_env[i].c_str();
    env[_env.size()] = NULL;
    int fd[2];
    int fdw = open("/tmp/test1", O_CREAT | O_RDWR | O_TRUNC , 0644);
    std::string path1 = "./cgi_bin/php-cgi";
    std::string path2 = _getPath;
    char *commad[] = {(char *)path1.c_str(), (char *)path2.c_str(), NULL};
    if (fdw < 0)
    {
        std::cerr << "faild to open" << std::endl;
        return false;
    }
    pipe(fd);
    int id = fork();
    if (id == 0)
    {
        if (flag == 1)
        {
            if (write(fdw, request.body.content.c_str(), request.body.content.size()) < 0)
            {
                perror("write()");
                exit(1);
            }
            close(fdw);
            fdw = open("/tmp/test1" , O_RDONLY);
            dup2(fdw, 0);
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execve(commad[0], commad, env);
            perror("execve()");
            exit(1);
        }
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execve(commad[0], commad, env);
        perror("execve()");
        exit(1);
    }
    wait(NULL);
    close(fd[1]);
    char buffer[100];
    int bytes;
    if (flag != 1)
    {
        sprintf(buffer, "%s 200 OK\r\n", request.data["version"].c_str());
        _body += buffer;
    }
    while ((bytes = read(fd[0], buffer, 100)) > 0)
    {
        std::string line(buffer, bytes);
        if (line.find("status") )
        _body += line;
    }
    close(fd[0]);
    unlink("/tmp/test1");
    return true;
}

bool    Response::validFile(Config::serverParse& server, int index, std::string path)
{
    std::ifstream file;
    file.open(path.c_str(), std::ios::binary);
    int fd = open(path.c_str() , O_RDWR);
    struct stat fileStat;
    _getPath = path;
    if (stat(path.c_str(), &fileStat) == 0) {
        // if ((fileStat.st_mode & S_IRUSR & S_IEXEC) != 0) {}
        // else
        // {
        //     errorPages(server, index, 401);
        //     return false;
        // }
    }
    if(file.is_open())
    {
        if (path.erase(0, path.rfind('.')) == ".php" && server.data["cgi_path"].size() > 0)
        {
            return executeCgi(server, index, 2);
        }
        if (methodAllowed(server, index) == true)
        {
            file.seekg(0, std::ios::end);
            _contentLength = file.tellg();
            file.seekg(0, std::ios::beg);
            char buffer[1000];
            int bytes;
            while ((bytes = read(fd, buffer, 1000)) > 0)
            {
                std::string line(buffer, bytes);
                _body += line;
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

std::string checker;

bool    Response::checkPathIfValid(Config::serverParse& server, int index , std::string line)
{
    std::string path;
    static int i = 0;
    path = server.locations[index].data["root"][0] + line;
    if (i == 0)
        checker = line;
    if (checker != line)
        path = line;
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return validFile(server, index, path);
    else {
        if (methodAllowed(server, index) == false)
            return false;
        _statusCode = 200;
        if (path[path.size() - 1] != '/') {
            path += "/";
            std::cout << "301 moved -> path = " << path << std::endl;
        }
        if (server.locations[index].data["index"].size() > 0 ) {
            path += server.locations[index].data["index"][0];
            this->_requestPath = path;
            return validFile(server, index, path);
        }
        if (server.locations[index].autoIndex == true) {
            dirent *test ;
            std::string line;
            std::string content = "";
            std::string prev = "..";
            while ((test = readdir(dir)) != NULL) {
                content += "<a href=\"";
                if (test->d_name != prev)
                    content += path + test->d_name;
                content += "\">";
                content += test->d_name ;
                content += "</a>";
                content += "\n";
                content += "<br>";
            }
            _contentType = "text/html";
            content = "<html><head><title>Index of " + path + "</title><style>body {background-color: #f2f2f2; font-family: Arial, sans-serif;} h1 {background-color: #4CAF50; color: white; padding: 10px;} table {border-collapse: collapse; width: 100%; margin-top: 20px;} th, td {text-align: left; padding: 8px;} th {background-color: #4CAF50; color: white;} tr:nth-child(even) {background-color: #f2f2f2;} a {text-decoration: none; color: #333;} a:hover {text-decoration: underline;}</style></head><body><h1>Index of " + path + "</h1>" + content + "</body></html>";
            _body += content;
        } else {
            errorPages(server, index, 404);
            return false;
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
    else if (path == ".html" || path == ".php") this->_contentType = "text/html";
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
    else if (path == ".tiff")  this->_contentType = "image/tif";
    else if (path == ".tif")  this->_contentType = "image/tif";

    else this->_contentType = "application/octet-stream";
}

void    Response::faildResponse()
{
    char buffer[100];
    sprintf(buffer, "%s %d \r\n", request.data["version"].c_str(), this->_statusCode);
    this->_response += buffer;
    _header += buffer;
    sprintf(buffer, "Connection: closed\r\n");
    this->_response += buffer;
    _header += buffer;
    sprintf(buffer, "Content-Type: text/html\r\n\r\n");
    this->_response += buffer;
    _header += buffer;
    this->_response += _body;
}

bool    Response::validRequestFormat(Config &config)
{
    if (request.data["path"].size() > 2084)
    {
        errorPages(config.servers[0], 0, 414); return false;
    }
    if (request.data["transfer-encoding"].size() > 0)
    {
        if (request.data["transfer-encoding"] != "chunked")
            errorPages(config.servers[0], 0 , 501); return false;
    }
    if (request.data["transfer-encoding"].size() == 0)
    {
        // if (request.data["content-length"].size() == 0)
        //     errorPages(config.servers[0], 0, 400); return false;
    }
    std::string allowedChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%";
    int k = 0;
    for (size_t i = 0; i < request.data["path"].size(); i++)
    {
        for (size_t j = 0; j < allowedChar.size(); j++)
        {
            if (request.data["path"][i] == allowedChar[j])
                k = 1;
        }
        if (k != 1)
            errorPages(config.servers[0], 0, 400); return false;
        k = 0;
    }
    return true;
}

int    Response::getMethod(Config &config)
{
    std::string line = request.data["path"];
    validRequestFormat(config);
    if ( getMatchedLocation(config) == 1 and _statusCode != 200)
    {
        getContentType();
        faildResponse();
        return (1);
    }
    if (_flag == 2 || _flag == 1)
        _response += _body;
    if (_response.size() == 0)
    {
        
        this->_statusCode = 200;
        char buffer[100];
        sprintf(buffer, "%s %d OK\r\n", request.data["version"].c_str(), this->_statusCode);
        this->_response += buffer;
        this->_header += buffer;
        if (_contentType != "text/html" )
        {
            sprintf(buffer, "Content-Type: %s\r\n", this->_contentType.c_str());
            this->_response += buffer;
            _header += buffer;
            sprintf(buffer, "Content-Length: %ld\r\n", this->_contentLength);
            this->_response += buffer;
            _header += buffer;
            sprintf(buffer, "Connection: keep-alive\r\n\r\n");
            this->_response += buffer;
            _header += buffer;
        }
        else
        {
            sprintf(buffer, "Content-Type: %s\r\n\r\n", this->_contentType.c_str());
            this->_response += buffer;
            _header += buffer;
        }
        _response += _body;
    }
    _requestPath = "";
   return 0; 
}
