/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:22:22 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/14 10:00:34 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//main parsing header test

#include "../includes/parsing_header.hpp"
#include <string>

int	main()
{

	//buffer avec header entier + body entier + bout de next buffer
	{
		std::string header;
		
		Request request = Request();
		header = "GET index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "Accept:text/html\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "body\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "POST index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());

		// std::string::iterator it = header.begin();
		// it += 5;
		// std::cout << it.base() << "\n++++++\n" << *it << std::endl;
		// std::string news(it.base());


		parse_buffer(&request);
	}
	std::cout << "==========================" << std::endl;
	{
		std::string header;
		Request request = Request();
		header = "GET index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());
		parse_buffer(&request);
		std::cout << "-------------" << std::endl;
		header = "Accept:text/html\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		parse_buffer(&request);
		std::cout << "-------------" << std::endl;
		header = "body\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "POST index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());

		parse_buffer(&request);
	}
	std::cout << "==========================" << std::endl;
	{
		std::string header;
		Request request = Request();
		header = "GET index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());
		parse_buffer(&request);
		std::cout << "-------------" << std::endl;
		header = "Accept:text/html\r\n";
		request.appendBuffer(header, 0, header.length());
		parse_buffer(&request);
		std::cout << "-------------" << std::endl;
		header = "\r\n";
		request.appendBuffer(header, 0, header.length());
		parse_buffer(&request);
		std::cout << "-------------" << std::endl;
		header = "body\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "POST index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());

		parse_buffer(&request);
	}
}
