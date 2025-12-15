/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 13:31:52 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>
#include <map>
#include <ostream>
#include "requestDefines.hpp"

class	Location
{
	public:
	Location(const std::string&);
	Location(const Location&);
	~Location(void);
	Location						&operator=(const Location&);
	Location(void);

	public:
	void							setRoot(const std::string&);
	const std::string				&getRoot(void) const;
	private:
	std::string					_root;

	public:
	void							setAlias(const std::string&);
	const std::string				&getAlias(void) const;
	private:
	std::string					_alias;

	public:
	void							setClientMaxBodySize(unsigned int);
	unsigned int					getClientMaxBodySize(void) const;
	private:
	unsigned int				_client_max_body_size;

	public:// upgrade it to pair string suffix string interpretater
	void							setCgiSuffixSet(const std::set<std::string>&);
	void							addCgiSuffix(const std::string&);
	const std::set<std::string>		&getCgiSuffix(void) const;
	private:
	std::set<std::string>		_cgi_suffix;

	public:
	void							setMethods(bool[3]);
	const bool						*getMethods(void) const;
	private:
	bool						_methods[3];

	public:
	void							setRedirect(const std::string&);
	const std::string				&getRedirect(void) const;
	private:
	std::string					_redirect;

	public:
	void							setAutoindex(bool);
	bool							getAutoindex(void) const;
	private:
	bool						_autoindex;

	public:
	const std::string				&getIndex(void) const;
	void							setIndex(const std::string&);
	private:
	std::string					_index;

	public:
	const std::map<int, std::string>	&getErrorPages(void) const;
	void								setErrorPage(int, const std::string&);
	void								setErrorPagesVector(const std::map<int, std::string>&);
	private:
	std::map<int, std::string>		_error_page;

	public:
	const std::string				&getPostDirectory(void) const;
	void							setPostDirectory(const std::string&);
	private:
	std::string					_post_directory;
};

std::ostream			&operator<<(std::ostream&, const Location&);
