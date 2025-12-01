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
		ConfigParser(char *);
		~ConfigParser(){}
		std::vector<Server>	run(void);

	private:

		std::string							_str;
		std::vector<std::string>			_token_vec;
		std::vector<std::string>::iterator	_token_it;
		std::vector<std::string>::iterator	_token_it_end;
		int									_last_directive;

		void				next(void);
		bool				end(void) const;
		const std::string	get(void) const;
		int					getDirective(void) const;

		void	tokenInit(void);
		void	tokenize(void);

		int		checkDirective(void);
		void	parseServer(std::vector<Server>&);
		std::map<std::string, Location>	parseServerLoop(Server&);

		void	parseListen(Server&);

		void	parseLocation(std::map<std::string, Location>&);
		Location	parseLocationLoop(Location&);

		void	parseRoot(Location&);
		void	parseAlias(Location&);
		void	parseClientMaxBodySize(Location&);
		void	parseCgi(Location&);
		void	parseAllowedMethods(Location&);
		void	parseReturn(Location&);
		void	parseAutoIndex(Location&);

		void	parseErrorPages(Location&);

		void	parsePostLocation(Location&);
		ConfigParser(){}
};
