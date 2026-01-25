/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:56:49 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "Request.hpp"
#include <exception>
#include <stdexcept>
#include <string>

Cgi::Cgi(Request *request): Event(PIPE) ,_env(CGI_HEADER), _contentLength(0), _client(request)
{
}

std::string	httpToCgiHeader(std::string field)
{
	for (int i = 0; !field[i]; i++)
	{
		if (field[i] == '-')
			field[i] = '_';
		if (field[i] >= 'a' && field[i] <= 'z')
			field[i] += 32;
	}
	if (!field.compare("CONTENT_TYPE") || !field.compare("CONTENT_LENGTH"))
		return (field + "=");
	return (HTTP + field + "=");
}

void	Cgi::addFields(std::string field, std::string token)
{
	std::string	variable;

	//check for host/type/length to be parse separated cause dont have HTTP_ preposition
	//field has to be in field tab (not an error if not but do not put in env)

	variable.assign(httpToCgiHeader(field));
	variable.append(token);
	this->_env.push_back(variable);
}

void	Cgi::getFieldFromUri(Request *request)
{
	std::string	token;
	token.assign(REQUEST_URI);
	token.append("=" + request->getUri());
	this->_env.push_back(token);
	token.assign(QUERY_STRING);
	token.append("=" + request->getQueryString());
	this->_env.push_back(token);
	token.assign(METHOD);
	if (request->getMethod() == GET)
		token.append("=GET");
	else if (request->getMethod() == POST)
		token.append("=POST");
	else if (request->getMethod() == DELETE)
		token.append("=DELETE");
	this->_env.push_back(token);
}

#include <unistd.h>
#include "deleteVector.hpp"

void	Cgi::init(void)
{
	if (pipe(_bodyPipe) == -1)
		throw (std::runtime_error("Cannot Pipe !"));
	if (pipe(_responsePipe) == -1)
		std::runtime_error("Cannot Pipe !");
}

void	Cgi::start()
{
	_pid = fork();
	if (_pid == -1)
		throw (std::runtime_error("Cannot fork !"));
	if (_pid == 0)
		//this->childProcess;
		//close bodypipe[1]
		//close response pipe[0]
		//dup2(responsepipe[1] on stdout)
		//dup2(bodypipe[0] on stdin)
		//close ?
		//execve
		//exit failure
	{
		close(_bodyPipe[1]);
		close(_responsePipe[0]);

		dup2(_responsePipe[1], STDOUT_FILENO);
		close(_responsePipe[1]);

		dup2(_bodyPipe[0], STDIN_FILENO);
		close(_bodyPipe[0]);

		std::vector<char*> arg = strToArray(_arg);
		std::vector<char*> env = strToArray(_env);
		execve(_exec.c_str(), arg.data(), env.data());
		deleteVector(arg);
		deleteVector(env);
		exit(1);
	}
	else
		//close bodypipe[0]
		//close responsepipe[1]
		// put body in bodypipe[1]
		// close bodypipe[1]
	{
		close(_bodyPipe[0]);
		close(_responsePipe[1]);


		std::string body = _client->getBody();
		write(_bodyPipe[1], body.c_str(), body.size());
		close(_bodyPipe[1]);
	}
}

std::vector<char*>	Cgi::strToArray(std::vector<std::string> vect_str)
{
	std::vector<char*> array;

	array.reserve(vect_str.size() + 1); // Optionnel mais optimisé
	for (std::vector<std::string>::iterator it = vect_str.begin();
		it != vect_str.end(); it++)
	{
		char	*copy = new char[it->size() + 1];
		std::strcpy(copy, it->c_str());
		array.push_back(copy);
	}
	array.push_back(NULL);
	return (array);
}
