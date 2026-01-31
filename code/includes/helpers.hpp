/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:16:50 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <map>
#include <string>
std::string	nbrToString(size_t nbr);
std::string	extractStr(const char *file);
void	mimeTypesMapFill(const char* path, std::map<std::string, std::string>& mime);
void		trimSlash(std::string &str);
void	trimDots(std::string &str);
std::string	buildIpPortStr(unsigned int	ip, unsigned short int port);
bool	canBuildOnDir(std::string &str);
