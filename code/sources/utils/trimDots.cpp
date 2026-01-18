/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimTwoDots.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:35:19 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <string>

void	trimDots(std::string &str)
{
    size_t pos = 0;
    size_t next = 0;
	while (str.size() > pos + 1)
	{
		pos = str.find_first_not_of("/", pos);
		if (pos == std::string::npos)
			break ;
    	next = str.find_first_of("/", pos);
		if (next == std::string::npos)
			next = str.size();
		size_t len = next - pos;
		if ((len == 2 || len == 1))
		{
			std::string	sub = str.substr(pos, len);
			if (sub == ".." || sub == ".")
				str.erase(pos, len);
		}
		else
			pos += len;
	}
}
//
// #include <iostream>
// int	main(void)
// {
// 	{
// 		std::string	s1("../.."), s2("../"), s3("/../"), s4("/..");
// 		trimTwoDots(s1);
// 		trimTwoDots(s2);
// 		trimTwoDots(s3);
// 		trimTwoDots(s4);
// 		std::cout << s1 << std::endl;
// 		std::cout << s2 << std::endl;
// 		std::cout << s3 << std::endl;
// 		std::cout << s4 << std::endl;
// 	}
// 	{
// 		std::string	s1("a..a"), s2("..a"), s3("a..a/"), s4("/a..a");
// 		trimTwoDots(s1);
// 		trimTwoDots(s2);
// 		trimTwoDots(s3);
// 		trimTwoDots(s4);
// 		std::cout << s1 << std::endl;
// 		std::cout << s2 << std::endl;
// 		std::cout << s3 << std::endl;
// 		std::cout << s4 << std::endl;
// 	}
// 	{
// 		std::string	s1(".."), s2("./."), s3("././"), s4("/.../");
// 		trimTwoDots(s1);
// 		trimTwoDots(s2);
// 		trimTwoDots(s3);
// 		trimTwoDots(s4);
// 		std::cout << s1 << std::endl;
// 		std::cout << s2 << std::endl;
// 		std::cout << s3 << std::endl;
// 		std::cout << s4 << std::endl;
// 	}
// }
//
