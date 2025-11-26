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

#include <string>
#include <set>
#include <map>
#include <ostream>

static const int	GET = 0;
static const int	POST = 1;
static const int	DELETE = 2;
static const std::string METHODS[] = {"GET", "POST", "DELETE"};

class	Location
{
	public:Location(const std::string&);
	public:Location(const Location&);
	public:~Location(void);
	public:Location							&operator=(const Location&);
	public:Location(void);

	// public:void								setName(std::string);
	// public:const std::string				&getName(void) const;
	// private:std::string					_name;

	public:const std::string				&getRoot(void) const;
	public:void								setRoot(std::string);
	private:std::string					_root;

	public:void								setAlias(std::string);
	public:const std::string				&getAlias(void) const;
	private:std::string					_alias;

	public:void								setClientMaxBodySize(unsigned int);
	public:unsigned int						getClientMaxBodySize(void) const;
	private:unsigned int				_client_max_body_size;

	public:void								setCgiSuffixSet(std::set<std::string>);
	public:void								addCgiSuffix(std::string);
	public:const std::set<std::string>	&getCgiSuffix(void) const;
	private:std::set<std::string>	_cgi_suffix;

	public:void								setMethods(bool[3]);
	public:const bool						*getMethods(void) const;
	private:bool						_methods[3];

	public:const std::string				&getRedirect(void) const;
	public:void								setRedirect(std::string);
	private:std::string					_redirect;

	public:bool								getAutoindex(void) const;
	public:void								setAutoindex(bool);
	private:bool						_autoindex;

	public:const std::map<int, std::string>	&getErrorPages(void) const;
	public:void								setErrorPage(int, std::string);
	public:void								setErrorPagesVector(std::map<int, std::string>);
	private:std::map<int, std::string>	_error_page;

	public:const std::string				&getPostDirectory(void) const;
	public:void								setPostDirectory(std::string);
	private:std::string					_post_directory;
};

std::ostream			&operator<<(std::ostream&, const Location&);
