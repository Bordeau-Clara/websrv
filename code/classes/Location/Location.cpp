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
	_name(DEFAULT_LOCATION_NAME),
	_root(DEFAULT_LOCATION_ROOT),
	_alias(DEFAULT_LOCATION_ALIAS),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_redirect(DEFAULT_LOCATION_REDIRECTION),
	_autoindex(DEFAULT_LOCATION_AUTOINDEX),
	_post_directory(DEFAULT_LOCATION_POST_DIRECTORY)
{
	std::vector<std::string>	cgi;

	cgi.push_back(DEFAULT_LOCATION_CGI_PYTHON);
	cgi.push_back(DEFAULT_LOCATION_CGI_PHP);
	setCgiSuffix(cgi);

	this->_methods[GET] = DEFAULT_LOCATION_GET;
	this->_methods[POST] = DEFAULT_LOCATION_POST;
	this->_methods[DELETE] = DEFAULT_LOCATION_DELETE;
}

Location::Location(Location &copy):
	_name(copy._name),
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
	this->_name = copy._name;
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

void	Location::setName(std::string  name)
{
	this->_name = name;
}

void	Location::setRoot(std::string  root)
{
	this->_root = root;
}

void	Location::setAlias(std::string  alias)
{
	this->_alias = alias;
}

void	Location::setClientMaxBodySize(unsigned int  client_max_body_size)
{
	this->_client_max_body_size = client_max_body_size;
}

void	Location::setCgiSuffix(std::vector<std::string>  cgi_suffix)
{
	this->_cgi_suffix = cgi_suffix;
}

void	Location::setMethods(bool  methods[3])
{
	this->_methods[GET] = methods[GET];
	this->_methods[POST] = methods[POST];
	this->_methods[DELETE] = methods[DELETE];
}
