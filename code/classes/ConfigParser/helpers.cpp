/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 11:51:16 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "tokens.hpp"

int	ConfigParser::checkDirective(void)
{
	for (int i = 0; i != NONE; ++i)
	{
		if (get() == DIRECTIVE[i])
		{
			_last_directive = i;
			return (i);
		}
	}
	throw (std::runtime_error("checkDirective(): unrecognized token '" + get() + '\''));
}

void	ConfigParser::next(void)
{
	++_token_it;
}

void	ConfigParser::nextAsserted(void)
{
	++_token_it;
	if (end())
	{
		--_token_it;
		throw (std::runtime_error("next() :'" + get() + "' shoudl not be the last tok"));
	}
}

bool	ConfigParser::end(void) const
{
	return(_token_it == _token_it_end);
}

const std::string	ConfigParser::get(void) const
{
	return(*_token_it);
}

int	ConfigParser::getDirective(void) const
{
	return (_last_directive);
}
