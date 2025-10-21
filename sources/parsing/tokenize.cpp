/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:42:17 by aykrifa           #+#    #+#             */
/*   Updated: 2025/10/21 17:20:40 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>

#define WS "\f\r\n\t\v "

void	tokenize(std::vector<std::string> &token, std::string str)
{
	size_t cursor = 0;
	while (true)
	{
		cursor = str.find_first_not_of(WS);
		if (cursor == std::string::npos)
			break ;
	}
}
