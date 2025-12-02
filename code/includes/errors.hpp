/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:05:30 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

static const char ARG_TOO_MUCH[] = "Too much arguments.";
static const char ARG_TOO_FEW[] = "Too few arguments.";

static const char INVALID_FILE_MESSAGE[] = ": invalid file.";

static const char USAGE[] = "Usage: ./webserv <configuration file>";

static const char OPEN_ERROR[] = ": unable to open file.";

static const char INVALID_CONFIG_EXTENSION[] = ": invalid config file extension (must be .conf).";
 
static const char READ_ERROR[] = ": error while reading file.";

static const char IO_CONFIG_ERROR[] = "I/O error while reading config";

static const char INVALID_CONFIG_FILE[] = "Invalid config file";
static const char INVALID_CONFIG_FORMAT[] = "Invalid config format";

static const char INVALID_PORT[] = "Invalid port value. Must be between 0 && 65535 inclusive";
