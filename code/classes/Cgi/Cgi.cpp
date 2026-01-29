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
#include "define_cgi.hpp"
#include "statusCodes.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>

Cgi::Cgi(Request *request): Event(PIPE) ,_env(CGI_HEADER), _client(request)
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
	//if content-length -> trigger variable
	//if no content-length -> take content-length for client (means it was a chunked body)
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

void	Cgi::getFieldFromUri()
{
	streams.get(LOG_EVENT) << "[getFieldFromUri]" << std::endl;
	std::string	token;
	token.assign(METHOD);
	if (_client->getMethod() == GET)
		token.append("GET");
	else if (_client->getMethod() == POST)
		token.append("POST");
	else if (_client->getMethod() == DELETE)
		token.append("DELETE");
	this->_env.push_back(token);

	token.assign(REQUEST_URI);
	token.append(_client->getUri());
	this->_env.push_back(token);

	token.assign(PATH_INFO);
	token.append(_client->getUri());
	this->_env.push_back(token);

	token.assign(QUERY_STRING);
	token.append(_client->getQueryString());
	this->_env.push_back(token);
}

#include <unistd.h>
#include <fcntl.h>
#include "deleteVector.hpp"

void	Cgi::init(void)
{
	if (pipe(_bodyPipe) == -1)
		throw (std::runtime_error("Cannot Pipe !"));
	if (pipe(_responsePipe) == -1)
		std::runtime_error("Cannot Pipe !");
	fcntl(_responsePipe[0], F_SETFL, O_NONBLOCK);
}

void	Cgi::start(EventManager &webServ)
{
	(void)webServ;
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
		// execve(std::string("bash").c_str(), arg.data(), env.data());
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
		write(_bodyPipe[1], body.data(), body.size());
		(streams.get(LOG_EVENT) << "body:").write(body.data(), body.size()) << std::endl;
		close(_bodyPipe[1]);
	}
}

std::vector<char*>	Cgi::strToArray(std::vector<std::string> vect_str)
{
	std::vector<char*> array;

	array.reserve(vect_str.size() + 1); // Optionnel mais optimisé
	streams.get(LOG_EVENT) << "[ENV in std::vector<string>]" << std::endl;
	for (std::vector<std::string>::iterator it = vect_str.begin();
		it != vect_str.end(); it++)
	{
	streams.get(LOG_EVENT) << *it << std::endl;
		char	*copy = new char[it->size() + 1];
		std::strcpy(copy, it->c_str());
		array.push_back(copy);
	}
	array.push_back(NULL);
	return (array);
}

void	Cgi::parseBuffer()
{
	//can a \r or \n be alone in header???
	std::string::size_type cursor = 0;
	if (!moveCursor(&cursor, this->_buffer, DCRLF))
	{
		//error
		/**/streams.get(LOG_EVENT) << "(1)[error no DCRLF]" << this->_header << std::endl;
		this->_client->setError(Status(INTERNAL_SERVER_ERROR, 500));
		this->_client->buildErrorResponse();
		return;
	}
	streams.get(LOG_EVENT) << "[PARSING HEADER]" << std::endl
		<< std::endl;
	this->fillHeader(cursor);
	this->parseHeader();

	if (this->_length != this->_buffer.size())
	{
		//error
		/**/streams.get(LOG_EVENT) << "(1)[size body differz]" << this->_header << std::endl;
		this->_client->setError(Status(INTERNAL_SERVER_ERROR, 500));
		this->_client->buildErrorResponse();
		return;
	}
	// buffer is body hehe
	this->_client->_response.str.append(this->_buffer.data(), this->_buffer.size());
	/**/streams.get(LOG_EVENT) << "[in cgi]" << this->_client->_response.cursor << std::endl
	/**/<< "[in cgi]" << this->_client->_response.str.size() << std::endl
	/**/<< "[in cgi]" << this->_client->_response.str << std::endl
		/**/<< std::endl;
	//mettre client EPOLLOUT
}

void	Cgi::fillHeader(std::string::size_type cursor)
{
	this->_header.append(this->_buffer, 0, cursor + 2);
	this->_buffer.erase(0, cursor + 4);
}

#include "helpers.hpp"
void	Cgi::parseHeader()
{
	std::string::size_type cursorStart = 0;
	std::string::size_type cursorEnd = 0;
	if (moveCursor(&cursorStart, this->_header, STATUS))
	{
		if (!moveCursor(&cursorEnd, this->_header, cursorStart, CRLF))
		{
			/**/streams.get(LOG_EVENT) << "(1)[error no CRLF]" << this->_header << std::endl;
			//error
			this->_client->setError(Status(INTERNAL_SERVER_ERROR, 500));
			this->_client->buildErrorResponse();
			return;
		}
		this->_client->_response.str.append("HTTP/1.1");
		this->_client->_response.str.append(this->_header.substr(cursorStart + STATUS.size(), cursorEnd + 2 - (cursorStart + STATUS.size())));
		this->_header.erase(cursorStart, cursorEnd + 2);
	}
	if (!moveCursor(&cursorStart, this->_header, "Content-Type:"))
	{
		//what to do??
	}
	if (!moveCursor(&cursorStart, this->_header, CON_LEN))
	{
		this->_length = this->_buffer.size();//what about binaries
		this->_client->_response.str.append(CON_LEN);
		this->_client->_response.str.append(nbrToString(this->_length));
		this->_client->_response.str.append(CRLF);
	}
	else
	{
		if (!moveCursor(&cursorEnd, this->_header, cursorStart, CRLF))
		{
			/**/streams.get(LOG_EVENT) << "(2)[error no CRLF]" << this->_header << std::endl;
			//error
			this->_client->setError(Status(INTERNAL_SERVER_ERROR, 500));
			this->_client->buildErrorResponse();
			return;
		}
		this->_length = strtol(this->_header.substr(cursorStart + CON_LEN.size(), cursorEnd).c_str(), NULL, 10);
	}
	if (!moveCursor(&cursorStart, this->_header, "Connection:"))
	{
		if (this->_client->getConnection() == KEEP_ALIVE)
			this->_client->_response.str.append(CON_KEEP_ALIVE);
		else
			this->_client->_response.str.append(CON_CLOSE);
	}
	this->_client->_response.str.append(this->_header);
	this->_client->_response.str.append(CRLF);

}
