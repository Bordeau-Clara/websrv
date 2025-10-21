/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:39:53 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 15:24:15 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <sys/epoll.h>
#include <string>
#include <iostream>
#include "../classes/Request/Request.hpp"

const std::string DCRLF = "\r\n\r\n";
const std::string CRLF = "\r\n";

#define HEADER 0
#define BODY 1

int	move_cursor(std::string::size_type *cursor, std::string str, std::string toFind);
void	tokenize(std::string *buffer, Request *request, std::string::size_type cursor, int mode);
void	tokenize(std::string *buffer, Request *request, int mode);
void	get_token(std::string *header, std::string *token, std::string::size_type *cursor);

void	parse_buffer(std::string *buffer, Request *request);
void	parse_header(std::string *header, Request *request);
