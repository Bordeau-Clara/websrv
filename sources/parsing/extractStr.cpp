/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractStr.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:30:48 by aykrifa           #+#    #+#             */
/*   Updated: 2025/10/21 15:41:52 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <sstream>

std::string	extractStr(char *InputFile)
{
	std::ifstream		ifs(InputFile);
	std::ostringstream	sstr;

	sstr << ifs.rdbuf();
	return (sstr.str());
}
