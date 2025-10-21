/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:22:22 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 15:29:38 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

//main parsing header test

#include "../includes/parsing_header.hpp"

int	main()
{

	//buffer avec header entier + body entier + bout de next buffer
	{
		std::string header;
		Request request = Request();
		header.append("GET index.html HTTP/1.1\r\n");
		header.append("Accept:text/html\r\n\r\n");
		header.append("body\r\n\r\n");
		header.append("POST index.html HTTP/1.1\r\n");

		parse_buffer(&header, &request);
	}
	std::cout << "==========================" << std::endl;
	{
		std::string header;
		Request request = Request();
		header.append("GET index.html HTTP/1.1\r\n");
		parse_buffer(&header, &request);
		std::cout << "-------------" << std::endl;
		header.append("Accept:text/html\r\n\r\n");
		parse_buffer(&header, &request);
		std::cout << "-------------" << std::endl;
		header.append("body\r\n\r\n");
		header.append("POST index.html HTTP/1.1\r\n");

		parse_buffer(&header, &request);
	}
}
