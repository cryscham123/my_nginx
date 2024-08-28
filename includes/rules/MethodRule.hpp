/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodRule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:35:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:08:59 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODRULE_HPP
# define METHODRULE_HPP

# include <iostream>
# include <vector>
# include "ARule.hpp"
# include "NetInfo.hpp"

class MethodRule: public ARule
{
	public:
		// if you want more, include here.
		// and get_method_enum function.
		enum	t_method
		{
			GET,
			PUT,
			POST,
			DELETE,
			METHOD_SIZE
		};
		MethodRule();
		MethodRule(const MethodRule &ref);
		MethodRule &operator=(const MethodRule &ref);
		~MethodRule();

		MethodRule	*clone() const;
		void		set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &);
		void		set_option(std::vector<std::string> dir_list[]);

		static t_method	get_method_enum(const std::string &method);
		bool			is_deny() const;

	private:
		bool					_is_deny;
};

#endif
