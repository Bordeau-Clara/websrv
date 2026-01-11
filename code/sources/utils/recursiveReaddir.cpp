/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gemini.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 08:21:10 by aykrifa           #+#    #+#             */
/*   Updated: 2026/01/11 09:05:53 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <string.h>

bool	recursiveReaddir(const std::string &path, std::string &target)
{
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());

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
		std::string	dir_or_file(path);
        dir_or_file.append("/");
        dir_or_file.append(entry->d_name);

		if (entry->d_type == DT_DIR) // Si c'est un dossier
		{
			// : on relance la fonction récursivement
			recursiveReaddir(dir_or_file, target);
		}
		else if (entry->d_type == DT_REG)// C'est un fichier régulier
		{
			// : append dans body;
			// to do ajouter les balises link
			target.append(dir_or_file);
		}
    }
    closedir(dir);
	return (true);
}
