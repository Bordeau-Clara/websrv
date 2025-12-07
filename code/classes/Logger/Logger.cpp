/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:09:13 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
std::string strTimestamp(void);

// Constructeur par défaut : statut vide
Logger::Logger(void) : _status_line(""), _stream(std::cout)
{
}

// Constructeur paramétré : affiche le statut initial
Logger::Logger(const String& defaultStatus) : _status_line(defaultStatus), _stream(std::cout)
{
    _refreshStatusLine();
}

// Constructeur par défaut : statut vide
Logger::Logger(std::ostream& output) : _status_line(""), _stream(output)
{
}

// Constructeur paramétré : affiche le statut initial
Logger::Logger(const String& defaultStatus, std::ostream& output) : _status_line(defaultStatus), _stream(output)
{
    _refreshStatusLine();
}

Logger::~Logger()
{
    // Optionnel : Nettoyer la ligne de statut à la fermeture
    this->_stream << ANSI_CLEAR_LINE << ANSI_MOVE_START << std::endl;
}

// Helper privé : Efface la ligne courante et réécrit le status
void Logger::_refreshStatusLine(void) const
{
    // 1. \033[2K : Efface la ligne où se trouve le curseur (l'ancienne status line)
    // 2. \r      : Remet le curseur au début (au cas où)
    // 3. affiche le status
    // 4. flush   : Force l'affichage sans attendre un \n
    this->_stream << ANSI_CLEAR_LINE << ANSI_MOVE_START << strTimestamp() + _status_line << std::flush;
}

void Logger::editStatusLine(const String& newStatus)
{
    _status_line = newStatus;
    _refreshStatusLine(); // Met à jour l'affichage immédiatement
}

void Logger::printNewLine(const String& msg) const
{
    // 1. On efface la ligne de statut actuelle pour faire de la place
    this->_stream << ANSI_CLEAR_LINE << ANSI_MOVE_START;
    
    // 2. On imprime le nouveau log (qui va faire descendre l'écran d'une ligne)
    this->_stream << strTimestamp() + msg << std::endl;
    
    // 3. On réimprime la ligne de statut en bas
    this->_stream << strTimestamp() + _status_line << std::flush;
}

void Logger::popStatus(const String& nextStatus)
{
    // 1. On sauvegarde le statut actuel qui va devenir un log permanent
    String completedTask = _status_line;

    // 2. On met à jour le futur statut (celui qui restera en bas)
    _status_line = nextStatus;

    // 3. On utilise la méthode existante pour imprimer l'ancien statut
    //    et rafraîchir la ligne du bas avec le nouveau.
    this->printNewLine(completedTask);
}
