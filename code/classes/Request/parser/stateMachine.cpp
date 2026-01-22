/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stateMachine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:38:49 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "stateMachine.hpp"
#include "Request.hpp"

void	Request::setState(parsing_state new_state)
{
	switch (new_state)
	{
		case READ:
			this->_state &= ~BIT_RW;
			break;
		case EXEC:
			this->_state |= BIT_RW;
			break;

		case CGI:
			this->_state |= BIT_CGI;
			break;

		case HEADER:
			this->_state &= ~BIT_HB;
			break;
		case BODY:
			this->_state |= BIT_HB;
			break;

		case CHUNKED:
			this->_state |= BIT_CHNKD;
			break;

		case OCTET:
			this->_state &= ~BIT_OC;
			break;
		case CHUNK_SIZE:
			this->_state |= BIT_OC;
			break;

		case TRAILERS:
			this->_state |= BIT_TRAILER;
			break;

		case ERROR:
			this->_state |= BIT_ERROR;
			break;

		default:
			this->_state |= BIT_ERROR;
			break;
	}
}

bool	Request::isState(parsing_state new_state) const
{
	switch (new_state)
	{
		case READ:
			return !(this->_state & BIT_RW);
		case EXEC:
			return (this->_state & BIT_RW);

		case CGI:
			return (this->_state & BIT_CGI);

		case HEADER:
			return !(this->_state & BIT_HB);
		case BODY:
			return (this->_state & BIT_HB);

		case CHUNKED:
			return (this->_state & BIT_CHNKD);

		case OCTET:
			return !(this->_state & BIT_OC);
		case CHUNK_SIZE:
			return (this->_state & BIT_OC);

		case TRAILERS:
			return (this->_state & BIT_TRAILER);

		case ERROR:
			return (this->_state & BIT_ERROR);

		default:
			//erreur a gerer
			return 0;
			break;
	}
}
