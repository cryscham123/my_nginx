/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARule.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:06:02 by hyunghki          #+#    #+#             */
/*   Updated: 2023/11/10 12:01:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include <iostream>

ARule::ARule(ARule::t_rule rule_type):
	_rule_type(rule_type) {}

ARule::ARule() {}
ARule::ARule(const ARule &ref):
	_rule_type(ref._rule_type) {}

ARule &ARule::operator=(const ARule &ref)
{
	if (this != &ref)
		_rule_type = ref._rule_type;
	return (*this);
}

ARule::~ARule() {}

ARule::t_rule ARule::get_rule_type() const
{
	return (_rule_type);
}
