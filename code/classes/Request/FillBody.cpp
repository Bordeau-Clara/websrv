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

void	Request::fillBody()
{
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
		std::cout << "client state is SEND" << std::endl;
	}
	//else 400 Bad request
}

void	Request::fillChunkedBody()
{
	std::string				line;
	static unsigned long	chunk_size = 0;
	std::string::size_type	cursor = 0;

	while(1)
	{
		if (this->_state == CHUNK_SIZE && move_cursor(&cursor, this->_buffer, CRLF))
		{
			line.assign(this->_buffer.substr(0, cursor));
			this->_buffer.erase(0, line.size() + 2);
			chunk_size = hexToLong(line);
			if (chunk_size == 0 && this->_trailer)
				this->_state = TRAILERS;
			else if (chunk_size == 0)
				this->_state = SEND;
			else
				this->_state = BODY;
			continue;
		}
		if (this->_state == BODY && this->_buffer.size() >= chunk_size + 2)
		{
			//put chunk_size octets in body
			this->_body.append(this->_buffer, 0, chunk_size);
			if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
			{
				//trow 400 Bad request
				std::cout << RED << "Number of octet not in adequation whith chunk size" << WHITE << std::endl;
			}
			//erase chunk_size octet + 2 from buffer
			this->_buffer.erase(0, chunk_size + 2);
			this->_state = CHUNK_SIZE;
			continue;
		}
		if (this->_state == TRAILERS && move_cursor(&cursor, this->_buffer, DCRLF))
		{
			this->_buffer.erase(0, cursor + 3);
			this->_state = SEND;
			continue;
		}
		break;
	}
}
