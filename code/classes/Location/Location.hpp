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
#include "Status.hpp"

class	Location
{
	public:
	Location(const std::string&);
	Location(const Location&);
	~Location(void);
	Location						&operator=(const Location&);
	void	defaultErrorPagesMapFiller(void);
	Location(void);
	std::string					_name;

	public:
	void							setAlias(const std::string&);
	const std::string				&getAlias(void) const;
	private:
	std::string					_alias;

	public:
	void							setRoot(const std::string&);
	const std::string				&getRoot(void) const;
	private:
	std::string					_root;

	public:
	void							setClientMaxHeaderSize(unsigned int);
	unsigned int					getClientMaxHeaderSize(void) const;
	private:
	unsigned int				_client_max_header_size;

	public:
	void							setClientMaxBodySize(unsigned int);
	unsigned int					getClientMaxBodySize(void) const;
	private:
	unsigned int				_client_max_body_size;

	public:// upgrade it to pair string suffix string interpretater
	void							setCgiSuffixSet(const std::map<std::string, std::string>&);
	const std::map<std::string, std::string>		&getCgiSuffix(void) const;
	private:
	std::map<std::string, std::string>		_cgi_suffix;

	public:
	void							setMethods(bool[3]);
	const bool						*getMethods(void) const;
	private:
	bool						_methods[3];

	public:
	void							setReturn(const Status&);
	const Status					&getReturn(void) const;
	private:
	Status						_return;

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

	public:
	void							setCookies(bool);
	bool							getCookies(void) const;
	private:
	bool						_cookies;
};

std::ostream			&operator<<(std::ostream&, const Location&);
