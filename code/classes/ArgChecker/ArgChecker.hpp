/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgChecker.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:36:31 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include "errors.hpp"

class	ArgChecker
{
	public:
		static void	checkargs(int argc);

	private:

	class TooMuchArgs : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (ARG_TOO_MUCH);
			}
	};

	class TooFewArgs : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (ARG_TOO_FEW);
			}
	};
};
