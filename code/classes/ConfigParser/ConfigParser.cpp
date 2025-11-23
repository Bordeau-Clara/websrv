/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:23:47 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "logfiles.hpp"
#include "tokens.hpp"
#include <exception>
#include <ostream>

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
}

static std::string	extractStr(const char *file)
{
	std::ifstream		ifs;

	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	ifs.open(file);

	std::ostringstream	ostrs;
	ostrs	<< ifs.rdbuf();
	return (ostrs.str());
}

void	tokenize(std::vector<std::string> &token, std::string &str)
{
	size_t	cursor = 0;

	while (cursor < str.length())
	{
		// std::cout << "iterate ..." << std::endl;
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
}

int	checkDirective(std::string &token)
{
	for (int i = 0; i != NONE; ++i)
	{
		if (token == DIRECTIVE[i])
			return (i);
	}
	throw (std::runtime_error("Unrecognized token " + token));
}

void	ConfigParser::run(char *file)
{
	std::string	str = extractStr(file);
	/**/Logger::print(LOG_CONFIGPARSER) << "|| Extracted str \nVV" << std::endl << str;

	commentFilter(str);
	/**/Logger::print(LOG_CONFIGPARSER) << "|| Comment trimmed \nVV" << std::endl << str;

	std::vector<std::string>	token;
	tokenize(token, str);
	/**/Logger::print(LOG_CONFIGPARSER) << "|| Token list \nVV";
	/**/for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	/**/{Logger::print(LOG_CONFIGPARSER) << "<"<< *it << "> ";}
	/**/Logger::print(LOG_CONFIGPARSER) << std::endl;

	for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	{
		switch (checkDirective(*it))
		{
			// check if there is a brace open
			// skip until next brace
			case SERVER:
				it++;
				if (*it != "{")
					throw (std::runtime_error("Missing opening bracket after server directive"));
				// skip until corresponding }
				// if no corresponding throw
				break ;
			// fill global template
			case LISTEN:
				break ;
			case ERROR_PAGE:
				break ;
			case CLIENT_MAX_BODY_SIZE:
				break ;
			case METHODS:
				break ;
			case RETURN:
				break ;
			case ROOT:
				break ;
			case ALIAS:
				break ;
			case AUTOINDEX:
				break ;
			case CGI_SUFFIX:
				break ;
			case POST_LOCATION:
				break ;
			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + *it));
		}
		/* TODO
			* verifier quelle directive j'ai
			* cree un serveur parent pour les directives trouvee.
			* Si serveur trouver saute a la next brace
			* Si brace pas fermee: erreur
			* Si directive non autorisee erreur
		*/
	}
}
