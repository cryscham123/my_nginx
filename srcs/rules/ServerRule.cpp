/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 13:02:41 by hyunghki          #+#    #+#             */
/*   Updated: 2024/01/04 16:58:08 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include "ServerRule.hpp"
#include "LocationRule.hpp"
#include "SyntaxChecker.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <sstream>

ServerRule::ServerRule():
	ARule(SERVER) {}

ServerRule::ServerRule(const ServerRule &ref):
	ARule(ref),
	_net_info(ref._net_info),
	_listen_literal(ref._listen_literal),
	_server_name(ref._server_name)
{
	std::vector<LocationRule *>::const_iterator i;
	VEC_LOOP(ref._cgi_location_rule, i)
		_cgi_location_rule.push_back((*i)->clone());
	VEC_LOOP(ref._location_rule, i)
		_location_rule.push_back((*i)->clone());
}

ServerRule &ServerRule::operator=(const ServerRule &ref)
{
	if (this != &ref)
	{
		ARule::operator=(ref);
		_net_info = ref._net_info;
		_listen_literal = ref._listen_literal;
		_server_name = ref._server_name;
		std::vector<LocationRule *>::const_iterator i;
		VEC_LOOP(ref._cgi_location_rule, i)
			_cgi_location_rule.push_back((*i)->clone());
		VEC_LOOP(ref._location_rule, i)
			_location_rule.push_back((*i)->clone());
	}
	return (*this);
}

ServerRule::~ServerRule()
{
	std::vector<LocationRule *>::const_iterator i;
	VEC_LOOP(_cgi_location_rule, i)
		delete *i;
	VEC_LOOP(_location_rule, i)
		delete *i;
}

ServerRule	*ServerRule::clone() const
{
	return (new ServerRule(*this));
}

void	ServerRule::set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &vs)
{
	if (rule == SyntaxChecker::LOCATION)
	{
		if (vs.size() == 0)
			throw (SyntaxChecker::get_error_msg("Invalid option number for \"location\" directive"));
		size_t is_cgi_extension = (vs[0] == "~");
		const std::string &path = vs[is_cgi_extension];
		if (vs.size() != 1 + is_cgi_extension)
			throw (SyntaxChecker::get_error_msg("Invalid option number for \"location\" directive"));
		std::vector<LocationRule *>::const_iterator i;
		if (is_cgi_extension)
		{
			if (path[0] != '.')
				throw (SyntaxChecker::get_error_msg("Location file extension \"" + path + "\" is invalid. It must start with \'.\'"));
			VEC_LOOP(_cgi_location_rule, i)
				if ((*i)->get_path() == path)
					throw (SyntaxChecker::get_error_msg("Location path \"" + path + "\" is duplicated"));
		}
		else
			VEC_LOOP(_location_rule, i)
				if ((*i)->get_path() == path)
					throw (SyntaxChecker::get_error_msg("Location path \"" + path + "\" is duplicated"));
		LocationRule *target = new LocationRule(path);
		if (is_cgi_extension)
			_cgi_location_rule.push_back(target);
		else
			_location_rule.push_back(target);
		SyntaxChecker::parse_context(*target);
	}
}

void	ServerRule::set_option(std::vector<std::string> dir_list[])
{
	if (_location_rule.empty())
	{
		_location_rule.push_back(new LocationRule("/"));
		std::vector<std::string> tmp[SyntaxChecker::DIRECTIVE_SIZE];
		SyntaxChecker::insert_directive(*_location_rule.back(), tmp);
	}
	SyntaxChecker::t_directive target;
	if (!dir_list[target = SyntaxChecker::LISTEN].empty())
		_listen_literal = dir_list[target];
	if (!dir_list[target = SyntaxChecker::SERVER_NAME].empty())
		_server_name = dir_list[target];
	std::vector<LocationRule *>::const_iterator i;
	VEC_LOOP(_cgi_location_rule, i)
		(*i)->set_option(dir_list);
	VEC_LOOP(_location_rule, i)
		(*i)->set_option(dir_list);
}

bool	ServerRule::can_connect(const NetInfo &host) const
{
	if (_net_info.is_accept_all(host.get_domain()) || host.is_accept_all(host.get_domain()))
		return (true);
	return (host == _net_info);
}

bool	ServerRule::is_server_name_exist(const std::string &s) const
{
	std::vector<std::string>::const_iterator i;
		VEC_LOOP(_server_name, i)
			if (*i == s)
				return (true);
	return (false);
}

const NetInfo	&ServerRule::get_net_info() const
{
	return (_net_info);
}

const std::vector<std::string>	&ServerRule::get_listen_literal() const
{
	return (_listen_literal);
}

const std::vector<std::string>	&ServerRule::get_server_name() const
{
	return (_server_name);
}

LocationRule	*ServerRule::get_location(const std::string &uri) const
{
	std::vector<LocationRule *>::const_iterator i;
	VEC_LOOP(_cgi_location_rule, i)
	{
		const std::string &file_extension = (*i)->get_path();
		size_t extension_size = file_extension.size();
		size_t cur = 0, size = 0;
		while (size != uri.size())
		{
			cur = uri.find_first_not_of("/", size);
			if (cur == std::string::npos)
				break ;
			size = uri.find_first_of("/", cur);
			size = (size == std::string::npos) ? uri.size() : size;
			if (extension_size > size - cur)
				continue ;
			if (uri.compare(size - extension_size, extension_size, file_extension) == 0)
				return (*i);
		}
	}
	LocationRule *target = NULL;
	size_t max_size = 0;
	VEC_LOOP(_location_rule, i)
	{
		const std::string &path = (*i)->get_path();
		if (uri.compare(0, path.size(), path) == 0 &&
			path.size() > max_size)
			target = *i, max_size = path.size();
	}
	return (target);
}

void	ServerRule::set_net_info(const NetInfo &info)
{
	_net_info = info;
}

void	ServerRule::set_listen_literal(const std::string &s)
{
	_listen_literal.clear();
	_listen_literal.push_back(s);
}