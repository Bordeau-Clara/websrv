/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canBuildOnDir.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 10:54:09 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <string>
#include <unistd.h>

#include "Request.hpp"


std::string	getParentDir(std::string &str)
/*
creer une substr ayant le dossier parent de l'expression parentDir
Si il nya que un element (pas de x/x) alors parentDir est "."

placer lastSlash au dernier /
si il ya autre chose derriere
	retourner la sub de debut a lastSlash
sinon
	tant que l'actuel est un /
		reculer
	si on est au premier caractere et actu = / 
		return .
	sinon return sub debut actu
*/
{
	size_t	lastSlash;

	lastSlash = str.find_last_of("/");
	if (lastSlash == std::string::npos)
	{
		streams.get(LOG_REQUEST) << "[GET PARENT DIR]" << std::endl
			<< "no slash" << std::endl
			<< std::endl;
		return ("");
	}
	if (lastSlash != str.size() - 1) // something else behind / ie parentDir = str[0 to lastslash]
		return (str.substr(0, lastSlash));

	size_t	cursor = lastSlash;
	while (str.at(cursor) == '/' && cursor != 0)//ca skip pas du tous au dossier d'avant, ca skip juste si il y a plusiuers / a la fin
		cursor--;
	lastSlash = str.find_last_not_of("/" , cursor);
	if (lastSlash != 0)
		return (str.substr(0, cursor + 1));

	streams.get(LOG_REQUEST) << "[GET PARENT DIR]" << std::endl
		<< "no directory" << std::endl
		<< std::endl;
	return ("");
}

bool	canBuildOnDir(std::string &str)
/*
un algo qui verifie soit que on peut ouvrir un fichier en ecriture soit qu'on puisse le creer
je suis parti sur le fait d'abord de checker qu'il yai un dossier parent dans lequel on puisse ecrire,
puis que si le fichier existe pas c'est gagne et si il existe il faut quon puisse le modifier.
donc stat sur l'expression dont on a trim le dernier element.


recuperer parentDir a partir de str

stat parentDir:
	Stat echoue : FALSE 
	Not a dir alors FALSE
	si on ne peut pas write dans le dir FALSE

stat expression entiere:
	si stat reussi
		pas un fichier FALSE
		impossible d'ouvrir en ecriture FALSE

return TRUE

*/
{
	std::string	parentDir;

	parentDir = getParentDir(str);
	if (parentDir.empty())
		return (false);
	struct stat	statbuf;
	if (stat(parentDir.c_str(), &statbuf) == -1) // does not exist
			return (false);
	if ((statbuf.st_mode & S_IFMT) != S_IFDIR) // path is not a dir
			return (false);
	
	if (access(parentDir.c_str(), W_OK | X_OK) == -1)// cannot open in write mode and cannot exectue
        return (false);
	
	if (stat(str.c_str(), &statbuf) != -1) // exist
	{
		if ((statbuf.st_mode & S_IFMT) != S_IFREG) // path isnt a regular file
			return (false);
		if (access(parentDir.c_str(), W_OK) == -1) // cannot open in write mode
			return (false);
	}
	return (true);
}

//
// #include <iostream>
//
// int	main(void)
// {
// 	std::string	s1("");
// 	std::string	s2("/111/222/333/");
// 	std::string	s3("/111/222/");
// 	std::string	s4("/1/");
// 	std::cout << s1 << " become " << getParentDir(s1) << std::endl;
// 	std::cout << s2 << " become " << getParentDir(s2) << std::endl;
// 	std::cout << s3 << " become " << getParentDir(s3) << std::endl;
// 	std::cout << s4 << " become " << getParentDir(s4) << std::endl;
// }
