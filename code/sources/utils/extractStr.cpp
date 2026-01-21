/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractStr.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 12:13:00 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include <sstream>
#include <ostream>

#include "FileStream.hpp"
extern FileStream	streams;

// std::string	extractStr(const char *file)
// {
// 	//https://stackoverflow.com/questions/29310166/check-if-a-fstream-is-either-a-file-or-directory
// 	std::ifstream	ifs(file, std::ifstream::in);
// 	if (ifs.fail())
// 		throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));
//
// 	std::string	str;
//
// 	ifs >> str;
// 	/**/streams.get(LOG_CONFIGPARSER) << SEPARATOR + "VV Extracted str VV" << std::endl << str << std::endl;
// 	return (str);
// }
//

std::string extractStr(const char *file)
{
    std::ifstream ifs(file, std::ios::binary);
    if (ifs.fail())
        throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));
	// constructor range
    std::string str((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
    return (str);
}
