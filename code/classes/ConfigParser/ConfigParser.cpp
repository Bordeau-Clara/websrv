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
#include "Location.hpp"
#include "Server.hpp"
#include <exception>
#include <vector>
#include <fstream>
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
}

static std::string	extractStr(const char *file)
{
	//https://stackoverflow.com/questions/29310166/check-if-a-fstream-is-either-a-file-or-directory
	std::fstream		fs(file);
	if (fs.fail())
		throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));

	std::ostringstream	ostrs;
	ostrs	<< fs.rdbuf();
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

int	ConfigParser::checkDirective(std::string &token)
{
	for (int i = 0; i != NONE; ++i)
	{
		if (token == DIRECTIVE[i])
			return (i);
	}
	throw (std::runtime_error("Unrecognized token " + token));
}

void	parseServer(std::vector<Server> &servers, std::vector<std::string>::iterator it)
{
	;
}

void	ConfigParser::run(char *file)
{
	std::string	str = extractStr(file);
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Extracted str \nVV" << std::endl << str;

	commentFilter(str);
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Comment trimmed \nVV" << std::endl << str;

	std::vector<std::string>	token;
	tokenize(token, str);
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Token list \nVV";
	/**/for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	/**/{Logger::print(LOG_CONFIGPARSER) << "<"<< *it << "> ";}
	/**/Logger::print(LOG_CONFIGPARSER) << std::endl;

	std::vector<Server>	servers;
	for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	{
		switch (checkDirective(*it))
		{
			// check if there is a brace open
			// skip until next brace
			case SERVER:
				it++;
				parseServer(servers, it);
				// skip until corresponding }
				// if no corresponding throw
				break ;
			// // fill global template
			// // case LISTEN:
			// // 	break ;
			// case ERROR_PAGE:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case CLIENT_MAX_BODY_SIZE:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case METHODS:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case RETURN:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case ROOT:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case ALIAS:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case AUTOINDEX:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case CGI_SUFFIX:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
			// case POST_LOCATION:
			// 	// fill(globalLocationTemplate)
			// 	// until ; if no throw
			// 	break ;
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
