/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursiveReaddir.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 08:21:10 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <string>
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "helpers.hpp"

bool	Request::recursiveReaddir(std::string strbizar)
{
    struct dirent *entry;
	// root + alias + dossier courant dans l'alias
	std::string	current_path = _location->getRoot() + _location->getAlias() + strbizar;
    DIR *dir = opendir(current_path.c_str());

    // 1. Vérifier si le chemin peut être ouvert
    if (dir == NULL)
	{
        return (false);
    }
    // 2. Parcourir le contenu du dossier
    while ((entry = readdir(dir)) != NULL)
	{
        // Ignorer les dossiers spéciaux "." et ".." pour éviter une boucle infinie
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Construire le chemin complet : dossier/nom_entree
		// std::string	dir_or_file(path);

		if (entry->d_type == DT_DIR) // Si c'est un dossier
		{
			// : on relance la fonction récursivement
			// en ajoutant a subpath le dir trouve
			recursiveReaddir(strbizar + "/" + entry->d_name);
		}
		else if (entry->d_type == DT_REG)// C'est un fichier régulier
		{
			// to do ajouter les balises link
			// : append dans body;
			std::string	toTrim;
			toTrim.reserve(256);
			toTrim += "<a href=\"";
			toTrim += entry->d_name;
			toTrim += "\">";
			toTrim += _server.getIpPortStr();
			toTrim +=  "/";
			toTrim += _location->_name;
			toTrim += "/";
			toTrim += strbizar;
			toTrim += "/";
			toTrim += entry->d_name;
			toTrim += "</a><br>\n";
			trimSlash(toTrim);
			_response.body.append(toTrim);
			// to do ajouter les balises link
		}
    }
    closedir(dir);
	return (true);
}
