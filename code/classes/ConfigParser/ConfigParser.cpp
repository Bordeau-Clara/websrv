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

void	tokenize(std::vector<std::string> &token, std::string str, std::string sep, std::string solotok)
{
	size_t	cursor = 0;
	while (true)
	{
		/*Look for beginning of a token
			*/
		size_t	tok_start = str.find_first_not_of(sep, cursor);
		/*Break if only whitespaces left
		 */
		if (tok_start == std::string::npos)
			break ;
		/*Look for end of a token
			*/
		size_t	tok_end = str.find_first_of(sep, tok_start);
		/*push back a substring fromn tok_start to tok_end
			*/
		token.push_back(str.substr(tok_start, tok_end == std::string::npos ? std::string::npos : tok_end - tok_start));
		cursor = tok_end;
	}
}

#define WS "\f\r\n\t\v "
void	ConfigParser::run(char *file)
{
	std::string	str = extractStr(file);
	std::vector<std::string>	token;

	std::cout << "|| Extracted str \nVV" << std::endl;
	std::cout << str;
	commentFilter(str);
	std::cout << "|| with no comment\nVV" << std::endl;
	std::cout << str;
	tokenize(token, str, WS);
	std::cout << "|| Token list\nVV" << std::endl;
	std::cout << str;
	for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); ++it)
	{
		std::cout << *it << std::endl;
		/* verifier quelle directive j'ai
	  * cree un serveur parent pour les directives trouvee.
	  * Si serveur trouver saute a la next brace
	  * Si brace pas fermee: erreur
	  * Si directive non autorisee erreur
	  *
	  */
	  /*
	  */
	}
}
