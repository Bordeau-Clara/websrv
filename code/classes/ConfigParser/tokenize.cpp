/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:04:02 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <vector>
#include <sstream>

// Si find renvoie la meme chose alors c'est qu'il ya plus de commentaires.
// si le debut de commentaire le plus proche
		// on cherche l'expression fermante a partir de la fin de l'expression ouvrante
		// meme si ca trouve rien on efface de found a eraseUntil
		// on efface de found a erase until pour pas effacer le \n
		// un bonne evolution serait de replace par un \n
		// fin du fichier on break;
// sinon
		// on cherche l'expression fermante a partir de la fin de l'expression ouvrante
		// meme si ca trouve rien on efface de found a eraseUntil
		// copy pasta du premier
		// fin du fichier on break;
static void	commentFilter(std::string &str)
{
	size_t	found = 0;
	size_t	slash = 0;
	size_t	diez = 0;
	size_t	eraseUntil = 0;
	size_t	len = 0;

	while (true)
	{
		slash = str.find("/*", found);
		diez = str.find("#", found);
		if (diez == slash)
			break ;
		else if (diez < slash && diez != std::string::npos)
		{
			found = diez;
			eraseUntil = str.find("\n", found + 1);
			len = (eraseUntil == std::string::npos) ? eraseUntil : eraseUntil - found;
			str.erase(found, len);
			if (eraseUntil == std::string::npos)
				break ;
		}
		else if (slash < diez && slash != std::string::npos)
		{
			found = slash;
			eraseUntil = str.find("*/", found + 2);
			len = (eraseUntil == std::string::npos) ? eraseUntil : eraseUntil - found + 2;
			str.erase(found, len);
			if (eraseUntil == std::string::npos)
				break ;
		}
	}
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Comment trimmed \nVV" << std::endl << str;
}

static std::string	extractStr(const char *file)
{
	//https://stackoverflow.com/questions/29310166/check-if-a-fstream-is-either-a-file-or-directory
	std::fstream		fs(file);
	if (fs.fail())
		throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));

	std::ostringstream	ostrs;
	ostrs	<< fs.rdbuf();
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Extracted str \nVV" << std::endl << ostrs;
	return (ostrs.str());
}

void	ConfigParser::tokenize(std::vector<std::string> &token, char *file)
{
	std::string	str = extractStr(file);

	commentFilter(str);

	size_t	cursor = 0;

	while (cursor < str.length())
	{
		size_t	end;

		if (WHITESPACES.find(str[cursor]) != std::string::npos)
		{
			end = str.find_first_not_of(WHITESPACES, cursor);
		}
		else if (OPERAND.find(str[cursor]) != std::string::npos)
		{
			end = cursor + 1;
			token.push_back(str.substr(cursor, end - cursor));
		}
		else
		{
			end = str.find_first_of(WHITESPACES + OPERAND, cursor);
			token.push_back(str.substr(cursor, end - cursor));
		}
		cursor = end;
	}
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Token list \nVV";
	/**/for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	/**/{Logger::print(LOG_CONFIGPARSER) << "<"<< *it << "> ";}
	/**/Logger::print(LOG_CONFIGPARSER) << std::endl;
}
