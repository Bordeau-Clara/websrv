/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:26:46 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <vector>
#include <string>

class Location;

typedef std::string String;

// Source - https://stackoverflow.com/a
// Posted by Sergey Kalinichenko
// Retrieved 2025-12-09, License - CC BY-SA 3.0

#include <map>

struct StringComp
{
    bool	operator()(const String &a,const String &b)
	{
		if (a.length() != b.length())
			return (a.length() > b.length());
		return (a < b);
	}
};

typedef std::map<String, Location, StringComp> LocationMap;

