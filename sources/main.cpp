/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:22:22 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/18 15:29:29 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

//main parsing header test

#include "../includes/parsing_header.hpp"
#include <iostream>
#include <string>

int	main()
{
	Request::initFields();

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
	std::cout << "==========================" << std::endl;
	{
		std::string header;

		Request request = Request();
		header = "GET index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "Accept:42\r\nRange:40\r\n";
		request.appendBuffer(header, 0, header.length());
		header.append("Host:58\r\norigiN:66\r\n");
		header.append("Expect:67\r\nCookies:70\r\n");
		header.append("Connection:102\r\naccept-language:147\r\n");
		header.append("accept-encoding:150\r\ncache-control:151\r\n");
		header.append("authorization:164\r\ncontent-type:185\r\n");
		header.append("if-none-match:187\r\ncontent-length:189\r\n");
		header.append("transfer-encoding:201\r\nif-modified-since:205\r\n\r\n");
		request.appendBuffer(header, 0, header.length());
		header = "body\r\n\r\n";
		request.appendBuffer(header, 0, header.length());
		header = "POST index.html HTTP/1.1\r\n";
		request.appendBuffer(header, 0, header.length());

		parse_buffer(&request);
	}
}
