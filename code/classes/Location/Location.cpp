/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:45:42 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "default.hpp"

Location::Location(void):
	// _name(DEFAULT_LOCATION_NAME),
	_root(DEFAULT_LOCATION_ROOT),
	_alias(DEFAULT_LOCATION_ALIAS),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_redirect(DEFAULT_LOCATION_REDIRECTION),
	_autoindex(DEFAULT_LOCATION_AUTOINDEX),
	_post_directory(DEFAULT_LOCATION_POST_DIRECTORY)
{
	this->addCgiSuffix(DEFAULT_LOCATION_CGI_PYTHON);
	this->addCgiSuffix(DEFAULT_LOCATION_CGI_PHP);

	this->_methods[GET] = DEFAULT_LOCATION_GET;
	this->_methods[POST] = DEFAULT_LOCATION_POST;
	this->_methods[DELETE] = DEFAULT_LOCATION_DELETE;
}

Location::Location(const std::string &name):
	_root(DEFAULT_LOCATION_ROOT),
	_alias(name),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_redirect(DEFAULT_LOCATION_REDIRECTION),
	_autoindex(DEFAULT_LOCATION_AUTOINDEX),
	_post_directory(DEFAULT_LOCATION_POST_DIRECTORY)
{
	this->addCgiSuffix(DEFAULT_LOCATION_CGI_PYTHON);
	this->addCgiSuffix(DEFAULT_LOCATION_CGI_PHP);

	this->_methods[GET] = DEFAULT_LOCATION_GET;
	this->_methods[POST] = DEFAULT_LOCATION_POST;
	this->_methods[DELETE] = DEFAULT_LOCATION_DELETE;
}

Location::Location(const Location &copy):
	// _name(copy._name),
	_root(copy._root),
	_alias(copy._alias),
	_client_max_body_size(copy._client_max_body_size),
	_cgi_suffix(copy._cgi_suffix),
	_redirect(copy._redirect),
	_autoindex(copy._autoindex),
	_error_page(copy._error_page),
	_post_directory(copy._post_directory)
{
	this->_methods[GET] = copy._methods[GET];
	this->_methods[POST] = copy._methods[POST];
	this->_methods[DELETE] = copy._methods[DELETE];
}

Location::~Location(void)
{
	;
}

Location	&Location::operator=(const Location &copy)
{
	// this->_name = copy._name;
	this->_root = copy._root;
	this->_alias = copy._alias;
	this->_client_max_body_size = copy._client_max_body_size;
	this->_cgi_suffix = copy._cgi_suffix;
	this->_redirect = copy._redirect;
	this->_autoindex = copy._autoindex;
	this->_error_page = copy._error_page;
	this->_post_directory = copy._post_directory;
	this->_methods[GET] = copy._methods[GET];
	this->_methods[POST] = copy._methods[POST];
	this->_methods[DELETE] = copy._methods[DELETE];
	return (*this);
}

//location
// void	Location::setName(std::string  name)
// {
// 	this->_name = name;
// }
//
// const std::string	&Location::getName(void) const
// {
// 	return (this->_name);
// }

//root
void	Location::setRoot(std::string  root)
{
	this->_root = root;
}

const std::string	&Location::getRoot(void) const
{
	return (this->_root);
}

//alias
void	Location::setAlias(std::string  alias)
{
	this->_alias = alias;
}

const std::string	&Location::getAlias(void) const
{
	return (this->_alias);
}

//client_max_body_size
void	Location::setClientMaxBodySize(unsigned int  client_max_body_size)
{
	this->_client_max_body_size = client_max_body_size;
}

unsigned int	Location::getClientMaxBodySize(void) const
{
	return (this->_client_max_body_size);
}

// cgi_suffix
void	Location::setCgiSuffixSet(std::set<std::string>  cgi_suffix)
{
	this->_cgi_suffix = cgi_suffix;
}

void	Location::addCgiSuffix(std::string cgi_suffix)
{
	this->_cgi_suffix.insert(cgi_suffix);
}

const std::set<std::string>	&Location::getCgiSuffix(void) const
{
	return (this->_cgi_suffix);
}

// methods
void	Location::setMethods(bool  methods[3])
{
	this->_methods[GET] = methods[GET];
	this->_methods[POST] = methods[POST];
	this->_methods[DELETE] = methods[DELETE];
}

const bool	*Location::getMethods(void) const
{
	return (this->_methods);
}

// return
const std::string	&Location::getRedirect(void) const
{
	return (this->_redirect);
}

void	Location::setRedirect(std::string redirect)
{
	this->_redirect = redirect;
}

// autoindex
bool	Location::getAutoindex(void) const
{
	return (this->_autoindex);
}

void	Location::setAutoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

// error_page
const std::map<int, std::string>	&Location::getErrorPages(void) const
{
	return (this->_error_page);
}

void	Location::setErrorPagesVector(std::map<int, std::string> error_page)
{
	this->_error_page = error_page;
}

void	Location::setErrorPage(int key, std::string value)
{
	this->_error_page[key] = value;
}

// post_directory
const std::string	&Location::getPostDirectory(void) const
{
	return (this->_post_directory);
}


void	Location::setPostDirectory(std::string post_directory)
{
	this->_post_directory = post_directory;
}

std::ostream	&operator<<(std::ostream &lhs, const Location &rhs)
{
	// lhs << "location: "
	// 	<< rhs.getName()
	// 	<< std::endl;
	//
	lhs << "root: "
		<< rhs.getRoot()
		<< std::endl;

	lhs << "alias: "
		<< rhs.getAlias()
		<< std::endl;

	lhs << "client_max_body_size: "
		<< rhs.getClientMaxBodySize()
		<< std::endl;

	lhs << "CGI suffixes: ";
	std::set<std::string>	CGI = rhs.getCgiSuffix();
	for (std::set<std::string>::iterator it = CGI.begin(); it != CGI.end(); ++it)
	{
		lhs << "'"<< *it << "' ";
	}
	lhs << std::endl;

	const bool	*methods = rhs.getMethods();
	lhs << "methods: "
		<< (methods[GET] ? "GET ": "")
		<< (methods[POST] ? "POST ": "")
		<< (methods[DELETE] ? "DELETE ": "")
		<< std::endl;

	lhs << "redirection: "
		<< rhs.getRedirect()
		<< std::endl;

	lhs << "autoindex: "
		<< (rhs.getAutoindex() ? "on" : "off")
		<< std::endl;

	lhs << "post_directory: "
		<< rhs.getPostDirectory()
		<< std::endl;
	// error pages
	std::map<int, std::string>	Pages = rhs.getErrorPages();
	for (std::map<int, std::string>::iterator it = Pages.begin(); it != Pages.end(); it++)
	{
		lhs << "error "
			<< it->first 
			<< ": "
			<< it->second
			<< std::endl;
	}
	return (lhs);
}
