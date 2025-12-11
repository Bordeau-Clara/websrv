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

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3)
	{
        std::cerr << "Usage ./requestGenerator headerfile [bodyfile]" << std::endl;
        return 1;
	}
	//Rempli le header
	{
		const std::string CRLF = "\r\n";
		// ouvre le header
		char *filename = argv[1];
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file)
		{
			std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
			return 1;
		}
		// ligne par ligne a chaque fin de ligne ou il ya quelquechose (pas de ligne vide) je met un CRLF
		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty())
				continue ;
			std::cout << line << CRLF;
		}
		// fin du fichier j'en net un supplementaire
		std::cout << CRLF;
	}
	if (argc == 2)
		return (0);
	//Rempli le body
	{
		//ouvre le body
		char *filename = argv[2];
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file)
		{
			std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
			return 1;
		}
		// fout tout le body dans std cout
		std::cout << file.rdbuf();
	}
    return 0;
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
