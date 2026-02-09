/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bad_header.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:19:45 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
const std::string	CRLF = "\r\n";
const std::string	PROTOCOL = "HTTP/1.1";

int	main(void)
{
	std::cout << "GET / " << PROTOCOL << CRLF;
	std::cout << "host: " << CRLF;
	std::cout << "host: " << CRLF;
	std::cout << CRLF;
}
