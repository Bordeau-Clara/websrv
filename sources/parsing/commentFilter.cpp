/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commentFilter.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:02:44 by aykrifa           #+#    #+#             */
/*   Updated: 2025/10/21 15:40:38 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

void	commentFilter(std::string &str)
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
		// Si find renvoie la meme chose alors c'est qu'il ya plus de commentaires.
		if (diez == slash)
			break ;
		// si le debut de commentaire le plus proche
		else if (diez < slash && diez != std::string::npos)
		{
			found = diez;
			// on cherche l'expression fermante a partir de la fin de l'expression ouvrante
			eraseUntil = str.find("\n", found + 1);
			// meme si ca trouve rien on efface de found a eraseUntil
			// on efface de found a erase until pour pas effacer le \n
			// un bonne evolution serait de replace par un \n
			len = (eraseUntil == std::string::npos) ? eraseUntil : eraseUntil - found;
			str.erase(found, len);
			// fin du fichier on break;
			if (eraseUntil == std::string::npos)
				break ;
		}
		// sinon
		else if (slash < diez && slash != std::string::npos)
		{
			found = slash;
			// on cherche l'expression fermante a partir de la fin de l'expression ouvrante
			eraseUntil = str.find("*/", found + 2);
			// meme si ca trouve rien on efface de found a eraseUntil
			// copy pasta du premier
			len = (eraseUntil == std::string::npos) ? eraseUntil : eraseUntil - found + 2;
			str.erase(found, len);
			// fin du fichier on break;
			if (eraseUntil == std::string::npos)
				break ;
		}
	}
}
