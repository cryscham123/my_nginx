/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodRule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:33:18 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:12:18 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include "MethodRule.hpp"
#include "SyntaxChecker.hpp"
#include <map>
#include <iostream>


MethodRule::MethodRule():
	ARule(METHOD),
	_is_deny(false) {}

MethodRule::MethodRule(const MethodRule &ref):
	ARule(ref),
	_is_deny(ref._is_deny) {}

MethodRule &MethodRule::operator=(const MethodRule &ref)
{
	if (this != &ref)
	{
		ARule::operator=(ref);
		_is_deny = ref._is_deny;
	}
	return (*this);
}

MethodRule::~MethodRule() {}

MethodRule	*MethodRule::clone() const
{
	return (new MethodRule(*this));
}

void	MethodRule::set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &vs)
{
	// no sub rule in method rule
	(void)rule;
	(void)vs;
}

void	MethodRule::set_option(std::vector<std::string> dir_list[])
{
	if (!dir_list[SyntaxChecker::DENY].empty())
		_is_deny = true;
}
bool	MethodRule::is_deny() const
{
	return (_is_deny);
}

MethodRule::t_method	MethodRule::get_method_enum(const std::string &method)
{
	if (method == "GET")
		return (GET);
	if (method == "PUT")
		return (PUT);
	if (method == "POST")
		return (POST);
	if (method == "DELETE")
		return (DELETE);
	return (METHOD_SIZE);
}