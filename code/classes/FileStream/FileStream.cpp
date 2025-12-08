/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 12:57:39 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "FileStream.hpp"

std::ofstream	&FileStream::get(const std::string &file)
{
    return (*this->_outfile.at(file));
}

void	FileStream::add(const std::string &file)
{
    if (this->_outfile.find(file) != this->_outfile.end())
		throw std::runtime_error("Log file already registered" + file);
	std::ofstream	*ofs = new std::ofstream();
    ofs->open(file.c_str());
    if (!ofs->is_open())
    {
        // Supprime l'entrée créée avant de throw
        this->_outfile.erase(file);
		delete (ofs);
        throw std::runtime_error("Failed to open log file: " + file);
    }
	this->_outfile.insert(std::make_pair(file, ofs));
}

FileStream::~FileStream(void)
{
    for (std::map<std::string, std::ofstream*>::iterator it = this->_outfile.begin();
         it != this->_outfile.end();
         ++it)
    {
        if (it->second)
        {
            it->second->close();   // ferme le flux si ouvert
            delete it->second;     // libère la mémoire
        }
    }
    this->_outfile.clear(); // vide la map
}
