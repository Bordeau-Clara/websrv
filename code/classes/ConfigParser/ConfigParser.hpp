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

class	Server;
class	Location;

class	ConfigParser
{
	public:
		ConfigParser(const char *);
		~ConfigParser(){}
		std::vector<Server>	run(void);

	private:

		std::string							_str;

		void	tokenInit(void);
		void	tokenize(void);

		std::vector<std::string>			_token_vec;
		std::vector<std::string>::iterator	_token_it;

		void				next(void);
		void				nextAsserted(void);
		bool				end(void) const;
		const std::string	get(void) const;
		int					getDirective(void) const;
		int					checkDirective(void);

		int									_last_directive;

		void							parseServer(std::vector<Server>&);
		std::map<std::string, Location>	parseServerLoop(Server&);
		void				parseLocation(std::map<std::string, Location>&);
		Location			parseLocationLoop(Location&);

	// parseDirective
		// server directive
		void	parseListen(Server&);

		// location directive
		void	parseRoot(Location&);
		void	parseAlias(Location&);
		void	parseClientMaxBodySize(Location&);
		void	parseClientMaxHeaderSize(Location&);
		void	parseCgi(Location&);
		void	parseAllowedMethods(Location&);
		void	parseReturn(Location&);
		void	parseAutoIndex(Location&);
		void	parseErrorPages(Location&);
		void	parsePostLocation(Location&);
		void	parseIndex(Location&);

		ConfigParser(){}
};
