/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:28:02 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include "tokens.hpp"

class	Server;
class	Location;

class	ConfigParser
{
	public:
		static void	run(char *);
		static void	tokenize(std::vector<std::string>&, char*);

		static void	parseServer(std::vector<Server>&,
						  std::vector<std::string>::iterator&,
						  std::vector<std::string>::iterator&);
		static std::map<std::string, Location>	parseServerLoop(std::vector<Server> &servers,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end);

		static void	parseLocation(std::map<std::string, Location>&,
							std::vector<std::string>::iterator&,
							std::vector<std::string>::iterator&);

		static void	parseRoot(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseAlias(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseClientMaxBodySize(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseCgi(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseAllowedMethods(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseReturn(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);
		static void	parseAutoIndex(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);

		static void	parseErrorPages(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);

		static void	parsePostLocation(Location&,
						std::vector<std::string>::iterator&,
						std::vector<std::string>::iterator&);

		static int	checkDirective(std::string&);

	private:
		ConfigParser(){}
		~ConfigParser(){}
};
