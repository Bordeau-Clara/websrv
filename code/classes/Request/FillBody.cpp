/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBody.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:41:39 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "parsing_header.hpp"
#include "statusCodes.hpp"

void	Request::fillBody()
{
	//tester erreur avec content Length trop grand et trop petit
	if (this->_length == 0)
	{
		this->_status.assign(LENGTH_REQUIRED);
		return;
	}
	if ((this->_body.size() + this->_buffer.size()) <= this->_contentLength)
	{
		this->_body.append(this->_buffer, 0, this->_buffer.size());
		this->_buffer.erase(0, this->_buffer.size());
	}
	else
	{
		this->_body.append(this->_buffer, 0, this->_contentLength - this->_body.size());
		this->_buffer.erase(0, this->_contentLength - this->_body.size());
	}
	if (this->_body.size() == this->_contentLength)
	{
		this->_state = SEND;
		streams.print(LOG_REQUEST) << "[STATE]" << std::endl
			<< "Client state has been put in SEND mode"
			<< std::endl;
	}
	//else 400 Bad request -> non va mettre erreur si body pas lu d'un coup
}

void	Request::fillChunkedBody()
{
	std::string				line;
	static unsigned long	chunk_size = 0;
	std::string::size_type	cursor = 0;

	while(1)
	{
		if (this->_body.size() > MAX_BODY_SIZE)
		{
			this->_status.assign(PAYLOAD_TOO_LARGE);
			return;
		}
		if (this->_state == CHUNK_SIZE)
		{
			if (!move_cursor(&cursor, this->_buffer, CRLF))
				break;
			line.assign(this->_buffer.substr(0, cursor));
			this->_buffer.erase(0, line.size() + 2);
			chunk_size = hexToLong(line);
			if (chunk_size == 0 && this->_trailer)
				this->_state = TRAILERS;
			else if (chunk_size == 0)
			{
				this->_state = SEND;
				this->_contentLength = this->_body.size();
				streams.print(LOG_REQUEST) << "[STATE]" << std::endl
					<< "Client state has been put in SEND mode"
					<< std::endl;
				return;
			}
			else
				this->_state = BODY;
			continue;
		}
		if (this->_state == BODY && this->_buffer.size() >= chunk_size + 2)
		{
			this->_body.append(this->_buffer, 0, chunk_size);
			if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
			{
				this->_status = BAD_REQUEST;
				streams.print(LOG_REQUEST) << "[ERROR]" << std::endl
					<< "Number of octet till CRLF is not equal to the number of octet to read"
					<< std::endl;
				return;
			}
			this->_buffer.erase(0, chunk_size + 2);
			this->_state = CHUNK_SIZE;
			continue;
		}
		if (this->_state == TRAILERS)
		{
			if (!move_cursor(&cursor, this->_buffer, DCRLF))
				break;
			this->_buffer.erase(0, cursor + 3);
			this->_state = SEND;
			this->_contentLength = this->_body.size();
			streams.print(LOG_REQUEST) << "[STATE]" << std::endl
				<< "Client state has been put in SEND mode"
				<< std::endl;
			return;
		}
		break;
	}
}
