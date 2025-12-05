/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestDefines.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 10:18:10 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

typedef enum method
{
	GET = 0,
	POST,
	DELETE,
	OTHER,
} method;
const std::string	DCRLF = "\r\n\r\n";
const std::string	CRLF = "\r\n";
const std::string	OWS = " \t";

static const std::string METHODS[] = {"GET", "POST", "DELETE"};
