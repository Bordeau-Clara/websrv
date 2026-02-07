/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashBoard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:56:48 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "colors.hpp"
#include <sstream>
#include <sys/types.h>

struct EventStats
{
    u_int64_t up;        // Trafic sortant
    u_int64_t down;      // Trafic entrant
    u_int32_t sent;      // Paquets/Messages envoyés
    u_int32_t received;  // Paquets/Messages reçus

    // Initialisation à zéro
    EventStats() : up(0), down(0), sent(0), received(0) {}

    // Génère la ligne de statut formatée
	std::string format() const
	{
        std::stringstream ss;
        
        // Exemple de formatage propre avec des couleurs (si ton colors.hpp le permet)
        ss
			<< "↑" << up << "B ↓" << down << "B | "
			<< "S:" << sent << " R:" << received;
        
        return ss.str();
    }
};

class ServerMonitor
{
	public:
		// On peut passer un ostream spécifique au logger interne
		ServerMonitor(void): _logger("BASE MSG") {this->_update();}

		// Méthodes pour mettre à jour les stats facilement
		void addRBytes(u_int64_t bytes)
		{
			_stats.down += bytes;
			_stats.received++;
			_update();
		}
		void addTBytes(u_int64_t bytes)
		{
			_stats.up += bytes;
			_stats.sent++;
			_update();
		}
		// Pour logger un événement réel (ex: "New client connected")
		void log(const std::string& msg)
		{
			_logger.printNewLine(msg);
		}

		// Si une étape est finie (ton popStatus)
	private:
		EventStats _stats;
		Logger   _logger;

		void _update() {_logger.editStatusLine(_stats.format());}
};
