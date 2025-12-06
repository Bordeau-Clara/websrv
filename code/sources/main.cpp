/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:22:22 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include "parsing_header.hpp"
#include <iostream>
#include <string>
#include "webserv.hpp"

FileStream	streams;

int	main(int argc, char **argv)
{
	std::vector<Server>	servers;
	try
	{
		streams.add(LOG_CONFIGPARSER);
		streams.add(LOG_SERVER);
		streams.add(LOG_DIRECTIVE);
		streams.add(LOG_LOCATION);
		streams.add(LOG_EVENT);

		ArgChecker::checkargs(argc);
		ConfigParser	parser(argv[1]);
		servers = parser.run();
	}
	catch (std::exception	&e)
	{
		std::cerr << "configParser Exception caught :"<< e.what() << std::endl;
		return (1);
	}
	// try
	// {
	// 	servers.at(0).startListen();
	// }
	// catch (std::exception	&e)
	// {
	// 	std::cerr << "Exception caught :"<< e.what() << std::endl;
	// 	return (1);
	// }
	printServerInfo(servers);
	Request::initFields();

	try
	{
		EventManager	Webserv(servers);
		Webserv.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "WEBSERV BOBO :" << e.what() << std::endl;
	}

	// std::cout << "==========================" << std::endl;
	// {
	// 	std::cout<< RED + "test 1" + WHITE << std::endl;
	// 	std::string header;
	//
	// 	Request request(servers.at(0));
	// 	header = "GET index.html HTTP/1.1\r\n";
	// 	header.append("Host:58\r\n");
	// 	header.append("content-type:185\r\n");
	// 	header.append("content-length:189\r\n");
	// 	header.append("transfer-encoding:201\r\n\r\n");
	//
	// 	header.append("4\r\nWiki\r\n5\r\n");
	// 	header.append("pedia\r\n0\r\n\r\n");
	// 	request.appendBuffer(header, 0, header.length());
	//
	// 	parse_buffer(&request);
	// }
	//
	//buffer avec header entier + body entier + bout de next buffer
	// {
		// std::string header;
		// 
		// Request request = Request();
		// header = "GET index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "Accept:text/html\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "body\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "POST index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		//
		// // std::string::iterator it = header.begin();
		// // it += 5;
		// // std::cout << it.base() << "\n++++++\n" << *it << std::endl;
		// // std::string news(it.base());
		//
		//
		// parse_buffer(&request);
	// }
	// std::cout << "==========================" << std::endl;
	// {
		// std::string header;
		// Request request = Request();
		// header = "GET index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		// parse_buffer(&request);
		// std::cout << "-------------" << std::endl;
		// header = "Accept:text/html\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// parse_buffer(&request);
		// std::cout << "-------------" << std::endl;
		// header = "body\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "POST index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		//
		// parse_buffer(&request);
	// }
	// std::cout << "==========================" << std::endl;
	// {
		// std::string header;
		// Request request = Request();
		// header = "GET index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		// parse_buffer(&request);
		// std::cout << "-------------" << std::endl;
		// header = "Accept:text/html\r\n";
		// request.appendBuffer(header, 0, header.length());
		// parse_buffer(&request);
		// std::cout << "-------------" << std::endl;
		// header = "\r\n";
		// request.appendBuffer(header, 0, header.length());
		// parse_buffer(&request);
		// std::cout << "-------------" << std::endl;
		// header = "body\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "POST index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		//
		// parse_buffer(&request);
	// }
	// std::cout << "==========================" << std::endl;
	// {
		// std::string header;
		//
		// Request request = Request();
		// header = "GET index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "Accept:42\r\nRange:40\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header.append("Host:58\r\norigiN:66\r\n");
		// header.append("Expect:67\r\nCookies:70\r\n");
		// header.append("Connection:102\r\naccept-language:147\r\n");
		// header.append("accept-encoding:150\r\ncache-control:151\r\n");
		// header.append("authorization:164\r\ncontent-type:185\r\n");
		// header.append("if-none-match:187\r\ncontent-length:189\r\n");
		// header.append("transfer-encoding:201\r\nif-modified-since:205\r\n\r\n");
		// request.appendBuffer(header, 0, header.length());
		// header = "body\r\n\r\n";
		// request.appendBuffer(header, 0, header.length());
		// header = "POST index.html HTTP/1.1\r\n";
		// request.appendBuffer(header, 0, header.length());
		//
		// parse_buffer(&request);
	// }
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 2" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("7\r\nMozilla\r\n9\r\nDeveloper\r\n");
// 		header.append("7\r\nNetwork\r\n0\r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 3" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("4;ext1=value;ext2\r\nWiki\r\n5;lang=fr\r\n");
// 		header.append("pedia\r\n0\r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 4" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("A;foo=123;bar=xyz\r\nHelloWorld\r\n6;debug=true\r\nSalut!\r\n");
// 		header.append("A;foo=123;bar=xyz\r\nHelloWorld\r\n6;debug=true\r\nSalut!\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 5" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n");
// 		header.append("trailer:76\r\n\r\n");
//
// 		header.append("0\r\nContent-MD5: 123abc\r\nX-Foo: test\r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 6" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("1E\r\nThis is a longer chunk of text\nwith a newline\r\n");
// 		header.append("A\r\nShort text\r\n0\r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 7" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html HTTP/1.1\r\n";
// 		header.append("Host:58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("  4  ;  foo = bar   \r\nTest\r\n0  \r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
// 	std::cout << "==========================" << std::endl;
// 	{
// 		std::cout<< RED + "test 8" + WHITE << std::endl;
// 		std::string header;
//
// 		Request request(servers.at(0));
// 		header = "GET index.html?lan=en HTTP/1.1\r\n";
// 		header.append("Host:                  58\r\n");
// 		header.append("content-type:185\r\n");
// 		header.append("content-length:189\r\n");
// 		header.append("transfer-encoding:201\r\n\r\n");
//
// 		header.append("1E\r\nThis is a longer");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
//
// 		header.assign(" chunk of text\nwith a newline\r\n");
// 		header.append("A\r\nShort text\r\n0\r\n\r\n");
// 		request.appendBuffer(header, 0, header.length());
//
// 		parse_buffer(&request);
// 	}
}

//find, append, erase reminder
	// {
	// 	std::string header = "abcdefghijklmnopqrstuvwxyz";
	// 	std::string str;
	// 	std::string::size_type cursor = 0;
	//
	// 	cursor = header.find("e");
	// 	std::cout << cursor << " " << header[cursor] << std::endl;
	// 	str.append(header, 0, cursor);
	// 	std::cout << str << std::endl;
	// 	header.erase(0, cursor);
	// 	std::cout << header << std::endl;
	// 	cursor = header.find("mn");
	// 	std::cout << cursor << " " << header[cursor] << std::endl;
	// 	str.append(header, 0, cursor);
	// 	std::cout << str << std::endl;
	// 	header.erase(0, cursor);
	// 	std::cout << header << std::endl;
	//
	// }
