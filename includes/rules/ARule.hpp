/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARule.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:06:40 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:06:13 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARULE_HPP
# define ARULE_HPP

# include <iostream>
# include <climits>
# include <ctime>
# include "SyntaxChecker.hpp"

class ARule
{
	public:
		enum t_rule
		{
			MAIN,
			HTTP,
			SERVER,
			LOCATION,
			METHOD
		};
		ARule(t_rule rule_type);
		ARule(const ARule &ref);
		ARule &operator=(const ARule &ref);
		virtual ~ARule();

		t_rule			get_rule_type() const;
		virtual	void	set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &) = 0;
		virtual	void	set_option(std::vector<std::string> dir_list[]) = 0;
		virtual	ARule	*clone() const = 0;

	protected:
		ARule();
		const static size_t			BUFFER_DEFAULT = ULONG_MAX;
		const static int 			BOOL_DEFAULT = 2;

		t_rule					_rule_type;
};

#endif
