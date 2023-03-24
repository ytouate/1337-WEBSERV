/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:03:24 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/24 20:19:55 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
#define _RESPONSE_HPP_

#include <iostream>
#include <iostream>
#include <dirent.h>
#include "../Parse/requestParse.hpp"
#include "../Parse/serverParse.hpp"
#include "../Parse/Config.hpp"
#include <unistd.h>
#include <string>
#include <fcntl.h>

class Response
{
    private:
        std::string _requestPath;
        std::string _contentType;
        int         _statusCode;
        std::string _body;
        requestParse request;
        bool    getMatchedLocation(Config& config);
        bool    checkPathIfValid(serverParse& server, int index , std::string line);
        void    getContentType();
        void    faildResponse();
        int     getIndexOfServerBlock(Config &config);
        int     checkContent();
        void    errorPages(serverParse& server, int id, int statusCode);
        bool    methodAllowed(serverParse& server, int index);
        bool    validFile(serverParse& server, int index, std::string path);
        int     getMethod(Config &configt);
    public:
        std::string _response;
        Response(Config &config, requestParse& _request);
        ~Response();
};

#endif





