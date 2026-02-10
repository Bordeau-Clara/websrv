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


#include <fstream>

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
