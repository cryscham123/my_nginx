/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainRule.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:58:14 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:06:45 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include "MainRule.hpp"
#include "HttpRule.hpp"
#include "SyntaxChecker.hpp"
#include <vector>

MainRule::MainRule():
	ARule(MAIN),
	_http_rule(NULL) {}

MainRule::MainRule(const MainRule &ref):
	ARule(ref)
{
	_http_rule = ref._http_rule->clone();
}

MainRule &MainRule::operator=(const MainRule &ref)
{
	if (this != &ref)
	{
		ARule::operator=(ref);
		_http_rule = ref._http_rule->clone();
	}
	return (*this);
}

MainRule::~MainRule()
{
	delete _http_rule;
}

MainRule	*MainRule::clone() const
{
	return (new MainRule(*this));
}

void	MainRule::set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &vs)
{
	if (rule == SyntaxChecker::HTTP)
	{
		if (_http_rule != NULL)
			throw (SyntaxChecker::get_error_msg("Http rule is duplicated"));
		if (vs.size() != 0)
			throw (SyntaxChecker::get_error_msg("Invalid option number for \"http\" directive"));
		_http_rule = new HttpRule();
		SyntaxChecker::parse_context(*_http_rule);
	}
}

void	MainRule::set_option(std::vector<std::string> dir_list[])
{
	// no option setting, no inherit to sub_rule in main_rule
	(void)dir_list;
}

HttpRule	*MainRule::get_http_rule() const
{
	return (_http_rule);
}