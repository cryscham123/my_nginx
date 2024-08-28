/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRule.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 13:02:14 by hyunghki          #+#    #+#             */
/*   Updated: 2024/01/04 17:02:18 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include "SyntaxChecker.hpp"
#include "HttpRule.hpp"
#include "ServerRule.hpp"
#include "NetInfo.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

HttpRule::HttpRule():
	ARule(HTTP) {}

HttpRule::HttpRule(const HttpRule &ref):
	ARule(ref),
	_socket_list(ref._socket_list)
{
	std::map<int, std::vector<ServerRule *> >::const_iterator i;
	std::vector<ServerRule *>::const_iterator j;
	MAP_LOOP(ref._server_rule, i)
		VEC_LOOP(i->second, j)
			_server_rule[i->first].push_back((*j)->clone());
}

HttpRule &HttpRule::operator=(const HttpRule &ref)
{
	std::map<int, std::vector<ServerRule *> >::const_iterator i;
	std::vector<ServerRule *>::const_iterator j;
	if (this != &ref)
	{
		ARule::operator=(ref);
		_socket_list = ref._socket_list;
		MAP_LOOP(ref._server_rule, i)
			VEC_LOOP(i->second, j)
				_server_rule[i->first].push_back((*j)->clone());
	}
	return (*this);
}

HttpRule::~HttpRule()
{
	std::map<int, std::vector<ServerRule *> >::const_iterator i;
	std::vector<ServerRule *>::const_iterator j;
	MAP_LOOP(_server_rule, i)
		VEC_LOOP(i->second, j)
			delete *j;
}

HttpRule *HttpRule::clone() const
{
	return (new HttpRule(*this));
}

void HttpRule::set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &vs)
{
	if (rule == SyntaxChecker::SERVER)
	{
		if (vs.size() != 0)
			throw (SyntaxChecker::get_error_msg("Invalid option number for \"server\" directive"));
		ServerRule target;
		SyntaxChecker::parse_context(target);
		const std::vector<std::string> &listen = target.get_listen_literal();

		std::vector<std::string>::const_iterator i;
		VEC_LOOP(listen, i)
		{
			NetInfo	tmp(*i);
			const std::vector<std::string> &server_names = target.get_server_name();
			std::vector<std::string>::const_iterator k;
			std::vector<ServerRule *>::const_iterator j;
			VEC_LOOP(_server_rule[tmp.get_port_num()], j)
				if ((*j)->get_listen_literal().front() == *i)
					VEC_LOOP(server_names, k)
					{
						if ((*j)->is_server_name_exist(*k))
							std::cout << "\e[91mWarning: \e[0mDuplicated Server \"" <<
							*k << "\" listening to \"" << *i << "\" will be ignored" << std::endl;
					}
			_server_rule[tmp.get_port_num()].push_back(target.clone());
			_server_rule[tmp.get_port_num()].back()->set_net_info(tmp);
			_server_rule[tmp.get_port_num()].back()->set_listen_literal(*i);
			if (tmp.is_accept_all(tmp.get_domain()))
				set_socket_list(_server_rule[tmp.get_port_num()].back()->get_net_info());
		}
	}
}

void HttpRule::set_option(std::vector<std::string> dir_list[])
{
	if (_server_rule.empty())
	{
		_server_rule[80].push_back(new ServerRule());
		std::vector<std::string> tmp[SyntaxChecker::DIRECTIVE_SIZE];
		SyntaxChecker::insert_directive(*_server_rule[80].front(), tmp);
		_server_rule[80].front()->set_net_info(NetInfo("0.0.0.0", "80"));
	}
	std::map<int, std::vector<ServerRule *> >::const_iterator i;
	std::vector<ServerRule *>::const_iterator j;
	MAP_LOOP(_server_rule, i)
		VEC_LOOP(i->second, j)
		{
			(*j)->set_option(dir_list);
			set_socket_list((*j)->get_net_info());
		}
}

std::vector<ServerRule *>	HttpRule::get_server(const NetInfo &n) const
{
	std::vector<ServerRule *> ret;
	std::map<int, std::vector<ServerRule *> >::const_iterator i;
	std::vector<ServerRule *>::const_iterator j;

	// map is not hash, so same speed with [] operator
	i = _server_rule.find(n.get_port_num());
	if (i == _server_rule.end())
		return (ret);
	VEC_LOOP(i->second, j)
		if ((*j)->can_connect(n))
			ret.push_back(*j);
	return (ret);
}

const std::vector<NetInfo>	&HttpRule::get_socket_list() const
{
	return (_socket_list);
}

void	HttpRule::set_socket_list(const NetInfo &n)
{
	int port_num = n.get_port_num();
	int domain = n.get_domain();
	std::vector<NetInfo>::iterator i;
	VEC_LOOP(_socket_list, i)
		if (i->get_port_num() == port_num && i->get_domain() == domain)
		{
			if (*i != n)
			{
				_socket_list.erase(i);
				std::stringstream ss; ss << port_num;
				if (domain == AF_INET)
					_socket_list.push_back(NetInfo("0.0.0.0", ss.str().c_str()));
				else
					_socket_list.push_back(NetInfo("::", ss.str().c_str()));
			}
			return ;
		}
	std::vector<ServerRule *>::const_iterator j;
	VEC_LOOP(_server_rule[port_num], j)
	{
		const NetInfo &cmp = (*j)->get_net_info();
		if (&cmp != &n && cmp.is_accept_all(n.get_domain()))
			return ;
	}
	_socket_list.push_back(n);
}