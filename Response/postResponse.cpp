/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:57:39 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/29 22:41:25 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int     Response::checkPathOfPostmethod(serverParse& server, std::string line, int index)
{
    std::string upload = "on";
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
    std::cout << request.data["content-length"] << std::endl;
    std::cout << "body  = " << request.data["body"] << std::endl;
    std::cout << "body = " << request.data["body"].length() << std::endl;
    (void)config;
    // if (getMatchedLocation(config) == 1)
    // {
    //     DIR *dir = opendir(_postPath.c_str());
    //     if (dir)
    //     {

    //     }        
    // }
    // std::cout << _body << std::endl;
    return 0;
}