/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTimeOut.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:52:03 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <ctime>

void	Request::editTime(void)
{
	_lastTimeStamp = std::time(NULL);
}

time_t	Request::getLastTime(void)
{
	return (_lastTimeStamp);
}

bool	Request::timeOut(unsigned int sec)
{
	return (_lastTimeStamp > sec + std::time(NULL));
}
