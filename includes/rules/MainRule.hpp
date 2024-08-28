/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainRule.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:57:23 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:07:08 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINRULE_HPP
# define MAINRULE_HPP

# include <iostream>
# include <vector>
# include "SyntaxChecker.hpp"
# include "ARule.hpp"
# include "HttpRule.hpp"

class MainRule: public ARule
{
	public:
		MainRule();
		MainRule(const MainRule &ref);
		MainRule &operator=(const MainRule &ref);
		~MainRule();

		void		set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &);
		void		set_option(std::vector<std::string> dir_list[]);

		MainRule	*clone() const;

		HttpRule	*get_http_rule() const;
	
	private:
		HttpRule	*_http_rule;
};

#endif
