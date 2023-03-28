/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:03:24 by otmallah          #+#    #+#             */
/*   Updated: 2023/03/28 15:54:52 by otmallah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
#define _RESPONSE_HPP_

#include "../inc.hpp"
#include "../Parse/requestParse.hpp"
#include "../Parse/serverParse.hpp"
#include "../Parse/Config.hpp"


class Response
{
    private:
        long        _contentLength;
        int         _statusCode;
        std::string _contentType;
        std::string _requestPath;
        std::string _checker;
        requestParse request;
        std::string _body;
        bool    getMatchedLocation(Config& config);
        bool    checkPathIfValid(serverParse& server, int index , std::string line);
        void    getContentType();
        void    faildResponse();
        int     getIndexOfServerBlock(Config &config);
        int     checkContent();
        void    errorPages(serverParse& server, int id, int statusCode);
        bool    methodAllowed(serverParse& server, int index);
        bool    validFile(serverParse& server, int index, std::string path);
        int     getMethod(Config &config);
        int     postMethod(Config &config);
        int    validateRequest();
        bool    executeCgi(serverParse& server, int index);
    public:
        std::string _response;
        Response(Config &config, requestParse& _request);
        ~Response();
};

#endif
