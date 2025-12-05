/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:52:05 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
static const std::string BLACK   = "\033[30m";
static const std::string RED     = "\033[31m";
static const std::string GREEN   = "\033[32m";
static const std::string YELLOW  = "\033[33m";
static const std::string BLUE    = "\033[34m";
static const std::string MAGENTA = "\033[35m";
static const std::string CYAN    = "\033[36m";
static const std::string WHITE   = "\033[37m";

static const std::string BRIGHT_BLACK   = "\033[90m";
static const std::string BRIGHT_RED     = "\033[91m";
static const std::string BRIGHT_GREEN   = "\033[92m";
static const std::string BRIGHT_YELLOW  = "\033[93m";
static const std::string BRIGHT_BLUE    = "\033[94m";
static const std::string BRIGHT_MAGENTA = "\033[95m";
static const std::string BRIGHT_CYAN    = "\033[96m";
static const std::string BRIGHT_WHITE   = "\033[97m";

static const std::string RESET = "\033[0m";
