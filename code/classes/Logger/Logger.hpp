/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:33:49 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <fstream>

#include "logfiles.hpp"

static const std::string	SEPARATOR = "##############################################################################################################\n";

class	Logger
{
	public:
		static void					add(const std::string&);
		static std::ofstream		&print(const std::string&);
		static void					delete_streams(void);
	private:
		Logger();
		~Logger();
		static std::map<std::string, std::ofstream*>	_outfile;
};
