/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRule.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 13:03:03 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/22 14:24:57 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ARule.hpp"
#include "LocationRule.hpp"
#include "SyntaxChecker.hpp"
#include "MethodRule.hpp"
#include <vector>
#include <iostream>
#include <sstream>

LocationRule::LocationRule(const std::string &path):
	ARule(LOCATION),
	_path(path),
	_autoindex(BOOL_DEFAULT)
{
	for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
		_method_rule[i] = NULL;
	_body_buffer[0] = BUFFER_DEFAULT;
	_body_buffer[1] = BUFFER_DEFAULT;
}

LocationRule::LocationRule() {}
LocationRule::LocationRule(const LocationRule &ref):
	ARule(ref),
	_path(ref._path),
	_autoindex(ref._autoindex),
	_return(ref._return),
	_error_page(ref._error_page)
{
	for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
		_method_rule[i] = (ref._method_rule[i] ? ref._method_rule[i]->clone() : NULL);
	_body_buffer[0] = ref._body_buffer[0];
	_body_buffer[1] = ref._body_buffer[1];
	_root = ref._root;
	_upload_store = ref._upload_store;
	_cgi_script_pass = ref._cgi_script_pass;
	_index = ref._index;
}

LocationRule &LocationRule::operator=(const LocationRule &ref)
{
	if (this != &ref)
	{
		ARule::operator=(ref);
		for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
			_method_rule[i] = (ref._method_rule[i] ? ref._method_rule[i]->clone() : NULL);
		_path = ref._path;
		_autoindex = ref._autoindex;
		_return = ref._return;
		_error_page = ref._error_page;
		_body_buffer[0] = ref._body_buffer[0];
		_body_buffer[1] = ref._body_buffer[1];
		_root = ref._root;
		_upload_store = ref._upload_store;
		_cgi_script_pass = ref._cgi_script_pass;
		_index = ref._index;
	}
	return (*this);
}

LocationRule::~LocationRule()
{
	for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
		delete _method_rule[i];
}

LocationRule	*LocationRule::clone() const
{
	return (new LocationRule(*this));
}

void	LocationRule::set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &vs)
{
	if (rule == SyntaxChecker::LIMIT_EXCEPT)
	{
		if (vs.size() == 0)
			throw (SyntaxChecker::get_error_msg("Invalid option number for \"limit_except\" directive"));
		for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
			if (_method_rule[i] != NULL)
				throw (SyntaxChecker::get_error_msg("Directive \"limit_except\" is duplicated"));
		bool tmp[MethodRule::METHOD_SIZE] = {0};
		std::vector<std::string>::const_iterator i;
		VEC_LOOP(vs, i)
		{
			MethodRule::t_method m = MethodRule::get_method_enum(*i);
			if (m == MethodRule::METHOD_SIZE)
				throw (SyntaxChecker::get_error_msg("Invalid method \"" + *i + "\" in limit_except rule"));
			if (tmp[m])
				throw (SyntaxChecker::get_error_msg("Duplicated method \"" + *i + "\" in limit_except rule"));
			tmp[m] = true;
		}
		MethodRule target;
		SyntaxChecker::parse_context(target);
		for (int i = 0; i < MethodRule::METHOD_SIZE; ++i)
			if (!tmp[i])
				_method_rule[i] = target.clone();
	}
}

void	LocationRule::set_option(std::vector<std::string> dir_list[])
{
	SyntaxChecker::t_directive target;
	if (!dir_list[target = SyntaxChecker::ROOT].empty() &&
		_root.empty())
		_root = dir_list[target];
	if (!dir_list[target = SyntaxChecker::UPLOAD_STORE].empty() &&
		_upload_store.empty())
		_upload_store = dir_list[target];
	if (!dir_list[target = SyntaxChecker::CGI_SCRIPT_PASS].empty() &&
		_cgi_script_pass.empty())
			_cgi_script_pass = dir_list[target];
	if (!dir_list[target = SyntaxChecker::INDEX].empty() &&
		_index.empty())
		_index = dir_list[target];
	if (!dir_list[target = SyntaxChecker::AUTOINDEX].empty() &&
		_autoindex == BOOL_DEFAULT)
		_autoindex = (dir_list[target][0] == "on");
	if (!dir_list[target = SyntaxChecker::RETURN].empty() &&
		_return.empty())
		_return = dir_list[target];
	if (!dir_list[target = SyntaxChecker::ERROR_PAGE].empty() &&
		_error_page.empty())
		{
			std::vector<std::string>::const_iterator i;
			std::string s;
			VEC_LOOP(dir_list[target], i)
				((*i)[0] == ':') ?
				s = std::string(i->begin() + 1, i->end()) :
				_error_page[*i] = s;
		}
	std::stringstream ss;
	if (!dir_list[target = SyntaxChecker::CLIENT_BODY_BUFFER_SIZE].empty() &&
		_body_buffer[0] == BUFFER_DEFAULT)
		ss << (dir_list[target][0] + " "), ss >> _body_buffer[0], ss.str("");
	if (!dir_list[target = SyntaxChecker::CLIENT_MAX_BODY_SIZE].empty() &&
		_body_buffer[1] == BUFFER_DEFAULT)
		ss << dir_list[target][0], ss >> _body_buffer[1];
	for (int i = 0; i < MethodRule::METHOD_SIZE; i++)
		if (_method_rule[i] != NULL)
			_method_rule[i]->set_option(dir_list);
}

const std::string	&LocationRule::get_path() const
{
	return (_path);
}

size_t	LocationRule::get_client_body_buffer_size() const
{
	return (_body_buffer[0]);
}

size_t	LocationRule::get_client_max_body_size() const
{
	return (_body_buffer[1]);
}

const std::string	&LocationRule::get_root() const
{
	return (_root.front());
}

const char*	LocationRule::get_upload_store() const
{
	if (_upload_store.empty())
		return (NULL);
	return (_upload_store.front().c_str());
}

const std::vector<std::string>	&LocationRule::get_index() const
{
	return (_index);
}

bool	LocationRule::get_autoindex() const
{
	return (_autoindex);
}

const std::vector<std::string>	&LocationRule::get_return() const
{
	return (_return);
}

const char	*LocationRule::get_error_page(const std::string &status_code) const
{
	std::map<std::string, std::string>::const_iterator i;
	i = _error_page.find(status_code);
	if (i == _error_page.end())
		return (NULL);
	return (i->second.c_str());
}

const char	*LocationRule::get_cgi_script_pass() const
{
	if (_cgi_script_pass.empty())
		return (NULL);
	return (_cgi_script_pass.front().c_str());
}

bool	LocationRule::can_access(const std::string &method) const
{
	MethodRule::t_method m = MethodRule::get_method_enum(method);
	if (m == MethodRule::METHOD_SIZE)
		return (false);
	if (_method_rule[m] == NULL)
		return (true);
	return (_method_rule[m]->is_deny() == false);
}
