/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locations.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:06:50 by aykrifa           #+#    #+#             */
/*   Updated: 2025/10/21 10:18:55 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <string>

class	Location
{
    public:

        Location(void);
		Location(const Location& src);
        ~Location();

    	Location& operator=(const Location& src);
	
	private:
		std::map<int, std::string>	_page;
		std::string					_root;
		std::string					_key;
		std::string					_index;
		bool						_get;
		bool						_post;
		bool						_delete;
};
