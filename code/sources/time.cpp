/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 14:49:47 by aykrifa           #+#    #+#             */
/*   Updated: 2025/12/07 14:52:04 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <string>

std::string strTimestamp(void)
{
    std::time_t	now = std::time(NULL);
    std::tm		*ltm = std::localtime(&now);
    char		buffer[16];

    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", ltm);
    return ("[" + std::string(buffer) + "] ");
}
