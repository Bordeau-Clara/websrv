/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_cgi.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:59:50 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
//define des header de cgi
	//variable static:
				//GATEWAY_INTERFACE
				//SERVER_PROTOCOL
				//SERVER_SOFTWARE (voir chat)
	//leave empty:
				//AUTH_TYPE
				//HOST_NAME ?
				//SERVER_PORT ?
				//REMOTE_IDENT
				//REMOTE_USER
	//request line ->
				//REQUEST_URI
				//PATH_INFO = SCRIPT_NAME
				//PATH_TRANSLATED
				//QUERY_STRING
				//METHOD
	//a parser du header:
				//REMOTE_HOST
				//CONTENT_LENGTH
				//CONTENT_TYPE
	//a obtenir autrement:
				//REMOTE_ADDR -> ipv4 du client

// const std::vector<std::string> CGI_HEADER =
// 	{
// 		"GATEWAY_INTERFACE=CGI/1.1",
// 		"SERVER_PROTOCOL=HTTP/1.1",
// 		"SERVER_SOFTWARE=", //voir chat pour info
// 		"AUTH_TYPE=",
// 		"HOST_NAME=",
// 		"SERVER_PORT=",
// 		"REMOTE_IDENT=",
// 		"REMOTE_USER="
// 	};

static const char* CGI_HEADER_ARRAY[] = {
    "GATEWAY_INTERFACE=CGI/1.1",
    "SERVER_PROTOCOL=HTTP/1.1",
    "SERVER_SOFTWARE=",
    "AUTH_TYPE=",
    //"HOST_NAME=", ->what is this??
    "SERVER_PORT=",
    "REMOTE_IDENT=",
    "REMOTE_HOST=",
    "REMOTE_USER="
};

const std::vector<std::string> CGI_HEADER(
    CGI_HEADER_ARRAY,
    CGI_HEADER_ARRAY + sizeof(CGI_HEADER_ARRAY) / sizeof(CGI_HEADER_ARRAY[0])
);

const std::string REQUEST_URI = "REQUEST_URI=";
const std::string PATH_INFO = "PATH_INFO=";
const std::string SCRIPT_NAME = "SCRIPT_NAME=";
const std::string PATH_TRANSLATED = "PATH_TRANSLATED=";
const std::string QUERY_STRING = "QUERY_STRING=";
const std::string METHOD = "REQUEST_METHOD=";

//const std::string REMOTE_HOST = "REMOTE_HOST="; ->why here??
const std::string CONTENT_LENGTH = "CONTENT_LENGTH=";
const std::string CONTENT_TYPE = "CONTENT_TYPE=";

const std::string HTTP = "HTTP_";

const std::string REMOTE_ADDR = "REMOTE_ADDR=";
const std::string STATUS = "Status:";

// const std::string GATEWAY = "GATEWAY_INTERFACE=CGI/1.1";
// const std::string PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1";
// const std::string SOFTWARE = "SERVER_SOFTWARE="; //voir chat pour info
//
// const std::string AUTH_TYPE = "AUTH_TYPE=";
// const std::string HOST_NAME = "HOST_NAME=";
// const std::string SERVER_PORT = "SERVER_PORT=";
// const std::string REMOTE_IDENT = "REMOTE_IDENT=";
// const std::string REMOTE_USER = "REMOTE_USER=";
