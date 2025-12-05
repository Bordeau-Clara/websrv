/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:39:53 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/28 11:56:10 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <sys/epoll.h>
#include <string>
#include <iostream>
#include "../classes/Request/Request.hpp"

#include "colors.hpp"
// const std::string DCRLF = "\r\n\r\n";
// const std::string CRLF = "\r\n";

int	move_cursor(std::string::size_type *cursor, std::string str, std::string toFind);
void	tokenize(std::string *buffer, Request *request, std::string::size_type cursor, int mode);
void	tokenize(std::string *buffer, Request *request, int mode);
void	get_token(std::string *header, std::string *token, std::string::size_type *cursor);

void	parse_buffer(Request *request);
void	parse_header_type(Request *request);
void	parse_header(Request *request);
void	parse_cgi_header(Request *request);
void	parse_request_line(Request *request, std::string token);
