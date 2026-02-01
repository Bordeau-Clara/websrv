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

void Location::defaultErrorPagesMapFiller(void)
{
    // 4xx Client Errors
    this->setErrorPage(400, DEFAULT_LOCATION_ERRORPAGES_PATH + "400.html");
    this->setErrorPage(403, DEFAULT_LOCATION_ERRORPAGES_PATH + "403.html");
    this->setErrorPage(404, DEFAULT_LOCATION_ERRORPAGES_PATH + "404.html");
    this->setErrorPage(405, DEFAULT_LOCATION_ERRORPAGES_PATH + "405.html");
    this->setErrorPage(408, DEFAULT_LOCATION_ERRORPAGES_PATH + "408.html");
    this->setErrorPage(411, DEFAULT_LOCATION_ERRORPAGES_PATH + "411.html");
    this->setErrorPage(413, DEFAULT_LOCATION_ERRORPAGES_PATH + "413.html");
    this->setErrorPage(414, DEFAULT_LOCATION_ERRORPAGES_PATH + "414.html");
    this->setErrorPage(431, DEFAULT_LOCATION_ERRORPAGES_PATH + "431.html");

    // 5xx Server Errors
    this->setErrorPage(500, DEFAULT_LOCATION_ERRORPAGES_PATH + "500.html");
    this->setErrorPage(504, DEFAULT_LOCATION_ERRORPAGES_PATH + "504.html");
}

Location::Location(void):
	_name(DEFAULT_LOCATION_NAME),
	_alias(DEFAULT_LOCATION_ALIAS),
	_root(DEFAULT_LOCATION_ROOT),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_return(DEFAULT_LOCATION_REDIRECTION),
	_autoindex(DEFAULT_LOCATION_AUTOINDEX),
	_index(DEFAULT_LOCATION_INDEX),
	_post_directory(DEFAULT_LOCATION_POST_DIRECTORY)
{
	this->defaultErrorPagesMapFiller();
	this->_methods[GET] = DEFAULT_LOCATION_GET;
	this->_methods[POST] = DEFAULT_LOCATION_POST;
	this->_methods[DELETE] = DEFAULT_LOCATION_DELETE;
}

Location::Location(const std::string &name):
	_name(name),
	_alias(name),
	_root(DEFAULT_LOCATION_ROOT),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_return(DEFAULT_LOCATION_REDIRECTION),
	_autoindex(DEFAULT_LOCATION_AUTOINDEX),
	_index(DEFAULT_LOCATION_INDEX),
	_post_directory(DEFAULT_LOCATION_POST_DIRECTORY)
{
	this->defaultErrorPagesMapFiller();
	this->_methods[GET] = DEFAULT_LOCATION_GET;
	this->_methods[POST] = DEFAULT_LOCATION_POST;
	this->_methods[DELETE] = DEFAULT_LOCATION_DELETE;
}

Location::Location(const Location &copy):
	_name(copy._name),
	_alias(copy._alias),
	_root(copy._root),
	_client_max_body_size(copy._client_max_body_size),
	_cgi_suffix(copy._cgi_suffix),
	_return(copy._return),
	_autoindex(copy._autoindex),
	_index(copy._index),
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
	this->_name = copy._name;
	this->_root = copy._root;
	this->_alias = copy._alias;
	this->_client_max_body_size = copy._client_max_body_size;
	this->_cgi_suffix = copy._cgi_suffix;
	this->_return = copy._return;
	this->_autoindex = copy._autoindex;
	this->_index = copy._index,
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
void	Location::setRoot(const std::string &root)
{
	this->_root = root;
}

const std::string	&Location::getRoot(void) const
{
	return (this->_root);
}

//alias
void	Location::setAlias(const std::string &alias)
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
void	Location::setCgiSuffixSet(const std::map<std::string, std::string> &cgi_suffix)
{
	this->_cgi_suffix = cgi_suffix;
}

const std::map<std::string, std::string>	&Location::getCgiSuffix(void) const
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
const Status	&Location::getReturn(void) const
{
	return (this->_return);
}

void	Location::setReturn(const Status &redirect)
{
	this->_return = redirect;
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

//root
void	Location::setIndex(const std::string &index)
{
	this->_index = index;
}

const std::string	&Location::getIndex(void) const
{
	return (this->_index);
}

// error_page
const std::map<int, std::string>	&Location::getErrorPages(void) const
{
	return (this->_error_page);
}

void	Location::setErrorPagesVector(const std::map<int, std::string> &error_page)
{
	this->_error_page = error_page;
}

void	Location::setErrorPage(int key, const std::string &value)
{
	this->_error_page[key] = value;
}

// post_directory
const std::string	&Location::getPostDirectory(void) const
{
	return (this->_post_directory);
}


void	Location::setPostDirectory(const std::string &post_directory)
{
	this->_post_directory = post_directory;
}

std::ostream	&operator<<(std::ostream &lhs, const Location &rhs)
{
	lhs << "name: "
		<< rhs._name
		<< std::endl;

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
	std::map<std::string, std::string>	CGI = rhs.getCgiSuffix();
	for (std::map<std::string, std::string>::iterator it = CGI.begin(); it != CGI.end(); ++it)
	{
		lhs << "'"<< it->first << " -> " << it->second << "' ";
	}
	lhs << std::endl;

	const bool	*methods = rhs.getMethods();
	lhs << "methods: "
		<< (methods[GET] ? "GET ": "")
		<< (methods[POST] ? "POST ": "")
		<< (methods[DELETE] ? "DELETE ": "")
		<< std::endl;

	lhs << "redirection: "
		<< rhs.getReturn().str
		<< " redirection code: "
		<< rhs.getReturn().code
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
