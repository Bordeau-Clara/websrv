/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                         :+:      :+:    :+:   */
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

class	FileStream
{
	public:
		FileStream(){};
		~FileStream();
		void					add(const std::string&);
		std::ofstream		&print(const std::string&);
	private:
		std::map<std::string, std::ofstream*>	_outfile;
};
