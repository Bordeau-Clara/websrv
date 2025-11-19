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


#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class	ConfigParser
{
	public:
		static void	run(char *);

	private:
		ConfigParser(){}
		~ConfigParser(){}
};
