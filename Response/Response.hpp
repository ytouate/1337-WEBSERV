/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otmallah <otmallah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:03:24 by otmallah          #+#    #+#             */
/*   Updated: 2023/04/06 22:02:24 by otmallah         ###   ########.fr       */
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
        int         _fdFile;
        std::string _contentType;
        std::string _requestPath;
        std::string _checker;
        requestParse request;
        std::string _body;
        std::string _postPath;
        std::string _uploadPath;
        std::string _deletePath;
        std::string _getPath;
        int         _flag;
        int         _indexLocation;
        int         _indexServer;
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
        int     postMethod(Config& config);
        int     deleteMethod(Config& config);
        int     checkPathOfPostmethod(serverParse& server, std::string line, int index);
        int     checkPathOfDeletemethod(serverParse& server, std::string line, int index);
        int    validateRequest();
        bool    executeCgi(serverParse& server, int index, int flag);
        void    postResponse(void);
        void    success();
        void    postType(std::string path);
        std::vector<std::string>    setEnv();
    public:
        char **g_env;
        std::string _header;
        std::string _response;
        Response(Config &config, requestParse& _request);
        ~Response();
};

#endif
