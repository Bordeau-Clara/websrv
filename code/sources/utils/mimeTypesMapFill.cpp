/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimeTypesMapFill.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 18:15:29 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "helpers.hpp"

void	mimeTypesMapFill(const char* path, std::map<std::string, std::string>& mime)
{
	std::string str = extractStr(path);
	std::stringstream	ss(str);
	std::vector<std::string>	ext;
	std::string	line;
	int	lineCount;

	if(ss.fail())
		throw (std::runtime_error("Cannot open mime file:" + std::string(path)));
	lineCount = 0;
	while (std::getline(ss, line))
	{
		ext.clear();
        lineCount++;
        // On utilise un autre stringstream pour découper la ligne par espaces
        std::stringstream lineStream(line);
        std::string word;

		// tant que je trouve un token
		while (lineStream >> word)
		{
			// on l'ajoute au vecteur d'extensions
			ext.push_back(word);
		}
        // Si la ligne est vide, on passe
        if (ext.empty())
            continue;

        // Si pas de deuxieme mot alors extension par defaut puis au suivant
        if (ext.size() == 1)
		{
			mime.insert(std::make_pair(ext.at(0), ""));
			continue ;
		}

		// sinon tant qu'on n'est pas au dernier ajouter une paire <n, dernier>
		for (std::vector<std::string>::iterator it = ext.begin(); it != ext.end() - 1; it++)
		{
			if (it->at(0) != '.')
				throw (std::runtime_error("mime extension has to start with '.' :" + *it + " l:" + nbrToString(lineCount)));
			mime.insert(std::make_pair(*it, *(ext.end() - 1)));
		}

    }
}
