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

Location::Location(void):_name(DEFAULT_LOCATION_NAME),
	_root(DEFAULT_LOCATION_ROOT),
	_alias(DEFAULT_LOCATION_ALIAS),
	_client_max_body_size(DEFAULT_LOCATION_MAX_BODY_SIZE),
	_cgi_suffix(DEFAULT_CGI),
	_methods((bool){true, false, false}),
	_redirect(""),
	_autoindex(false),
	
	
