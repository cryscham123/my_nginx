/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 08:40:35 by hyunghki          #+#    #+#             */
/*   Updated: 2024/08/29 08:40:37 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "SyntaxChecker.hpp"
#include "FileManager.hpp"
#include "MainRule.hpp"
#include "ARule.hpp"

int main(int ac, char **av)
{
	MainRule	main_rule;

	try
	{
		SyntaxChecker::learn(ac, av[1]);
		SyntaxChecker::parse_context(main_rule);
		std::cout << "\e[92mWebserv server setting has been completed now.\e[0m" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (EXIT_SUCCESS);
}
