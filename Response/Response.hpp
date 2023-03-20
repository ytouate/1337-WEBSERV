/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:03:24 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/20 19:49:50 by otmallah         ###   ########.fr       */
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
        std::string _response;
        int         _statusCode;
    public:
        bool    getMatchedLocation(Config& config, requestParse& request);
        bool    checkPathIfValid(serverParse& server, int index, std::string line);
        void    getContentType();
        void    faildResponse();
        int     getMethod(Config &config, requestParse& request);
        int     getIndexOfServerBlock(Config &config, requestParse &request);
        int     checkContent();
        Response();
        ~Response();
};

#endif