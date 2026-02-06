/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:44:18 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Status.hpp"
#include <string>
#include <vector>
#include <map>

static const std::string				DEFAULT_LOCATION_NAME = "";
static const std::string				DEFAULT_LOCATION_ROOT = "";
static const std::string				DEFAULT_LOCATION_ALIAS = "";
static const unsigned int				DEFAULT_LOCATION_MAX_HEADER_SIZE = 2048;
static const unsigned int				DEFAULT_LOCATION_MAX_BODY_SIZE = 2048;
static const Status						DEFAULT_LOCATION_REDIRECTION = Status("", 0);
static const bool						DEFAULT_LOCATION_AUTOINDEX = false;
static const std::string				DEFAULT_LOCATION_INDEX = "index.html";
static const std::string				DEFAULT_LOCATION_POST_DIRECTORY = "";
const std::string						DEFAULT_LOCATION_ERRORPAGES_PATH = "./code/classes/Location/default_error_pages/";
static const bool						DEFAULT_LOCATION_GET = true;
static const bool						DEFAULT_LOCATION_POST = false;
static const bool						DEFAULT_LOCATION_DELETE = false;
