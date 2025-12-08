/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:05:15 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "string.hpp"
#include <iostream>
#include <ostream>

// Efface toute la ligne actuelle
static const String	ANSI_CLEAR_LINE = "\033[2K";
// Remet le curseur au début de la ligne
static const String	ANSI_MOVE_START = "\r";

class Logger
{
public :
	//default constructor
	Logger(void);
	// constructor with initial status
	Logger(const String&);
	//constructor qith specific output
	Logger(std::ostream&);
	// constructor with initial status and specific output
	Logger(const String&, std::ostream&);
	// destructor
	~Logger(void);

	// edit line to be printed in status
	void	editStatusLine(const String&);
	// print a new line and keep status at start
	void	printNewLine(const String&) const;
	// print actual status as new line and edit status
	void	popStatus(const String&);
private:
	String				_status_line;
	void				_refreshStatusLine(void) const;
	std::ostream		&_stream;
};
