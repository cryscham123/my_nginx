/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxChecker.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:29:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:17:46 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SyntaxChecker.hpp"
#include "ARule.hpp"
#include "FileManager.hpp"
#include "HttpRule.hpp"
#include "ServerRule.hpp"
#include "LocationRule.hpp"
#include "NetInfo.hpp"
#include <vector>
#include <sstream>

std::string		SyntaxChecker::_buf;
size_t			SyntaxChecker::_cur;
size_t			SyntaxChecker::_line_num;
size_t			SyntaxChecker::_depth;

SyntaxChecker::SyntaxChecker() {}
SyntaxChecker::SyntaxChecker(const SyntaxChecker &ref) { (void)ref; }
SyntaxChecker &SyntaxChecker::operator=(const SyntaxChecker &ref) { (void) ref; return (*this); }
SyntaxChecker::~SyntaxChecker() {}

std::logic_error	SyntaxChecker::get_error_msg(const std::string &msg)
{
	std::stringstream ss;
	ss << (_line_num + 1);
	return (std::logic_error("\e[91mError(line " + ss.str() + "):\e[0m " + msg));
}

SyntaxChecker::t_directive	SyntaxChecker::get_directive_enum(const std::string &directive)
{
	if (directive == "http")
		return (HTTP);
	if (directive == "server")
		return (SERVER);
	if (directive == "error_page")
		return (ERROR_PAGE);
	if (directive == "client_max_body_size")
		return (CLIENT_MAX_BODY_SIZE);
	if (directive == "client_body_buffer_size")
		return (CLIENT_BODY_BUFFER_SIZE);
	if (directive == "root")
		return (ROOT);
	if (directive == "autoindex")
		return (AUTOINDEX);
	if (directive == "index")
		return (INDEX);
	if (directive == "upload_store")
		return (UPLOAD_STORE);
	if (directive == "cgi_script_pass")
		return (CGI_SCRIPT_PASS);
	if (directive == "deny")
		return (DENY);
	if (directive == "location")
		return (LOCATION);
	if (directive == "listen")
		return (LISTEN);
	if (directive == "server_name")
		return (SERVER_NAME);
	if (directive == "return")
		return (RETURN);
	if (directive == "limit_except")
		return (LIMIT_EXCEPT);
	return (DIRECTIVE_SIZE);
}

SyntaxChecker::t_directive_info	SyntaxChecker::check_directive(const ARule &r, t_directive d)
{
	switch (r.get_rule_type())
	{
		case ARule::MAIN:
			switch (d)
			{
				case HTTP:
					return (CONTEXT);
				default:
					break;
			}
			break;
		case ARule::HTTP:
			switch (d)
			{
				case SERVER:
					return (CONTEXT);
				case ROOT:
				case ERROR_PAGE:
				case CLIENT_BODY_BUFFER_SIZE:
				case CLIENT_MAX_BODY_SIZE:
				case AUTOINDEX:
				case INDEX:
				case DENY:
					return (NO_INHERIT);
				default:
					break;
			}
			break;
		case ARule::SERVER:
			switch (d)
			{
				case LOCATION:
					return (CONTEXT);
				case LISTEN:
				case SERVER_NAME:
				case RETURN:
				case UPLOAD_STORE:
					return (NO_INHERIT);
				case ROOT:
				case ERROR_PAGE:
				case CLIENT_BODY_BUFFER_SIZE:
				case CLIENT_MAX_BODY_SIZE:
				case AUTOINDEX:
				case INDEX:
				case DENY:
					return (VALID);
				default:
					break;
			}
			break;
		case ARule::LOCATION:
			switch (d)
			{
				case LIMIT_EXCEPT:
					return (CONTEXT);
				case CGI_SCRIPT_PASS:
					return (NO_INHERIT);
				case RETURN:
				case UPLOAD_STORE:
				case ROOT:
				case ERROR_PAGE:
				case CLIENT_BODY_BUFFER_SIZE:
				case CLIENT_MAX_BODY_SIZE:
				case AUTOINDEX:
				case INDEX:
				case DENY:
					return (VALID);
				default:
					break;
			}
			break;
		case ARule::METHOD:
			switch (d)
			{
				case DENY:
					return (VALID);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return (INVALID);
}

void	SyntaxChecker::insert_directive(ARule &r, std::vector<std::string> directive_list[])
{
	t_directive target;
	if (directive_list[target = CLIENT_MAX_BODY_SIZE].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("1048576");
	if (directive_list[target = CLIENT_BODY_BUFFER_SIZE].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("16384");
	if (directive_list[target = UPLOAD_STORE].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("");
	if (directive_list[target = ROOT].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("html");
	if (directive_list[target = AUTOINDEX].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("off");
	if (directive_list[target = INDEX].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("index.html");
	if (directive_list[target = LISTEN].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("0.0.0.0:80");
	if (directive_list[target = SERVER_NAME].empty() &&
		check_directive(r, target) == NO_INHERIT)
		directive_list[target].push_back("");
	r.set_option(directive_list);
}

void	SyntaxChecker::learn(int ac, const char *target_file)
{
	switch (ac)
	{
		case 1:
			target_file = DEFAULT_PATH;
			FileManager::try_new_file(DEFAULT_PATH, DEFAULT_CONTENT);
			// FALLTHROUGH
		case 2:
			FileManager::read_file(target_file, _buf);
			break ;
		default:
			throw (std::logic_error("\e[91mError: \e[0mToo many program arguments"));
	}
	std::cout << "Webserv is parsing path \e[93m\"" + std::string(target_file) + "\"\e[0m" << std::endl;
}

std::string SyntaxChecker::get_word(char end_char, t_parse_state &flag)
{
	char quote = 0;
	size_t size = 0;
	while (flag != WORD_END && flag != END)
	{
		switch (flag)
		{
			case SKIP_SPACE:
				switch (_buf[_cur])
				{
					case '\n':
						++_line_num;
						// FALLTHROUGH
					case '\t':
					case ' ':
						++_cur;
						break ;
					case '\"':
					case '\'':
						quote = _buf[_cur++];
						flag = PROGRESS;
						break ;
					case '#':
						flag = COMMENTS;
						++_cur;
						break ;
					case '\0':
					case ';':
					case '{':
					case '}':
						flag = (_buf[_cur] == end_char) ? END : ERROR;
						_cur += (end_char != '\0');
						break ;
					default:
						flag = PROGRESS;
						break;
				}
				break ;
			case PROGRESS:
				switch (quote)
				{
					case 0:
						switch (_buf[_cur + size])
						{
							case '\n':
								++_line_num;
								// FALLTHROUGH
							case '\0':
							case '\t':
							case '#':
							case ' ':
							case ';':
							case '{':
							case '}':
								flag = WORD_END;
								break ;
							default:
								++size;
						}
						break ;
					default:
						switch (_buf[_cur + size])
						{
							case '\0':
								flag = ERROR;
								break ;
							case '\"':
							case '\'':
								flag = (_buf[_cur + size] == quote) ? WORD_END : PROGRESS;
								// FALLTHROUGH
							default:
								_line_num += (_buf[_cur + size] == '\n');
								++size;
						}
				}
				break ;
			case COMMENTS:
				switch (_buf[_cur])
				{
					case 0:
						flag = (end_char == '\0') ? END : ERROR;
						break ;
					case '\n':
						flag = SKIP_SPACE;
						++_line_num;
						// FALLTHROUGH
					default:
						++_cur;
				}
				break ;
			case ERROR:
				throw (get_error_msg("Expected word is not found"));
			default:
				break ;
		}
	}
	_cur += size;
	return (_buf.substr(_cur - size, size - (quote != 0)));
}

void	SyntaxChecker::parse_context(ARule &r)
{
	vs directive_list[DIRECTIVE_SIZE];

	char end_char = (_depth == F_ROOT_RULE) ? '\0' : '}';
	while (1)
	{
		t_parse_state flag = SKIP_SPACE;
		const std::string &directive = get_word(end_char, flag);
		if (flag == END)
			break ;
		t_directive d = get_directive_enum(directive);
		vs tmp;
		switch (check_directive(r, d))
		{
			case INVALID:
				throw (get_error_msg("Unexpected directive \"" + directive + "\" is found"));
			case CONTEXT:
				parse_directive(tmp, '{');
				++_depth;
				r.set_sub_rule(d, tmp);
				break ;
			default:
				parse_directive(tmp, ';');
				option_validate(directive_list, d, tmp);
		}
	}
	insert_directive(r, directive_list);
	(_depth == F_ROOT_RULE) ? _buf.clear() : (void)--_depth;
}

void	SyntaxChecker::parse_directive(vs &tmp, char end_char)
{
	while(1)
	{
		t_parse_state flag = SKIP_SPACE;
		const std::string &option = get_word(end_char, flag);
		if (flag == END)
			break ;
		tmp.push_back(option);
	}
}

void	SyntaxChecker::option_validate(vs dirs[], t_directive d, const vs &target)
{
	switch (d)
	{
		case LISTEN:
			listen_rule(dirs, target);
			break ;
		case SERVER_NAME:
			server_name_rule(dirs, target);
			break ;
		case RETURN:
			return_rule(dirs, target);
			break ;
		case UPLOAD_STORE:
		case ROOT:
		case CGI_SCRIPT_PASS:
			path_rule(dirs, d, target);
			break ;
		case ERROR_PAGE:
			error_page_rule(dirs, target);
			break ;
		case CLIENT_BODY_BUFFER_SIZE:
		case CLIENT_MAX_BODY_SIZE:
			size_rule(dirs, d, target);
			break ;
		case AUTOINDEX:
			autoindex_rule(dirs, target);
			break ;
		case INDEX:
			index_rule(dirs, target);
			break ;
		case DENY:
			access_rule(dirs, d, target);
			break ;
		default:
			break;
	}
}

bool	SyntaxChecker::is_num(const std::string &s, unsigned long max_num)
{
	if (s.empty())
		return (false);
	unsigned long tmp = 0;
	for (size_t i=0; i<s.size(); ++i)
	{
		if (s[i] < '0' || s[i] > '9')
			return (false);
		tmp = tmp * 10 + s[i] - '0';
		if (tmp > max_num)
			return (false);
	}
	return (true);
}

void	SyntaxChecker::listen_rule(vs dirs[], const vs &target)
{
	if (target.size() != 1)
		throw (get_error_msg("Invalid option numbers for \"listen\" rule"));
	size_t i = target[0].find(']');
	if (i == std::string::npos)
		i = 0;
	i = target[0].find(':', i);
	std::string option;
	if (i != std::string::npos)
	{
		std::string ip(target[0].begin(), target[0].begin() + i);
		std::string port(target[0].begin() + i + 1, target[0].end());
		if (ip == "localhost")
			ip = "127.0.0.1";
		else if (ip == "*")
			ip = "0.0.0.0";
		else if (ip[0] == '[' && ip[ip.size() - 1] == ']')
		{
			ip = ip.substr(1, ip.size() - 2);
			NetInfo tmp(ip.c_str(), NULL);
			if (tmp.get_domain() != AF_INET6)
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in listen rule"));
		}
		else
		{
			NetInfo tmp(ip.c_str(), NULL);
			if (tmp.get_domain() != AF_INET)
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in listen rule"));
		}
		if (!is_num(port, PORT_MAX_NUM))
			throw (get_error_msg("Invalid option \"" + target[0] + "\" in listen rule"));
		option = ip + ":" + port;
	}
	else
	{
		if (target[0] == "localhost")
			option = "127.0.0.1:80";
		else if (target[0] == "*")
			option = "0.0.0.0:80";
		else if (is_num(target[0], PORT_MAX_NUM))
			option = "0.0.0.0:" + target[0];
		else if (target[0][0] == '[' && target[0][target[0].size() - 1] == ']')
		{
			option = target[0].substr(1, target[0].size() - 2);
			NetInfo tmp(option.c_str(), NULL);
			if (tmp.get_domain() != AF_INET6)
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in listen rule"));
			option += ":80";
		}
		else
		{
			NetInfo tmp(target[0].c_str(), NULL);
			if (tmp.get_domain() != AF_INET)
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in listen rule"));
			option = target[0] + ":80";
		}
	}
	vs::const_iterator cmp;
	VEC_LOOP(dirs[LISTEN], cmp)
		if (*cmp == option)
			throw (get_error_msg("Duplicated option \"" + option + "\" in listen rule"));
	dirs[LISTEN].push_back(option);
}

void	SyntaxChecker::server_name_rule(vs dirs[], const vs &target)
{
	if (target.empty())
		throw (get_error_msg("Invalid option numbers for \"server_name\" rule"));
	for (size_t i = 0; i < target.size(); ++i)
	{
		for (size_t j = i + 1; j < target.size(); ++j)
			if (target[i] == target[j])
				throw (get_error_msg("Duplicated name \"" + target[i] + "\" for server_name rule"));
		vs::const_iterator cmp;
		VEC_LOOP(dirs[SERVER_NAME], cmp)
			if (*cmp == target[i])
				throw (get_error_msg("Duplicated name \"" + target[i] + "\" for server_name rule"));
		dirs[SERVER_NAME].push_back(target[i]);
	}
}

void	SyntaxChecker::error_page_rule(vs dirs[], const vs &target)
{
	if (target.size() < 2)
		throw (get_error_msg("Invalid option numbers for \"error_page\" rule"));
	dirs[ERROR_PAGE].push_back(":" + target[target.size() - 1]);
	for (size_t i = 0; i < target.size() - 1; ++i)
	{
		if (!is_num(target[i], 599) || target[i].size() != 3  || target[i][0] < '3')
			throw (get_error_msg("Invalid option numbers for \"error_page\" rule"));
		vs::const_iterator cmp;
		VEC_LOOP(dirs[ERROR_PAGE], cmp)
			if (*cmp == target[i])
				throw (get_error_msg("Duplicated option \"" + target[i] + "\"for error_page rule"));
		dirs[ERROR_PAGE].push_back(target[i]);
	}
}

void	SyntaxChecker::size_rule(vs dirs[], t_directive d, const vs &target)
{
	if (!dirs[d].empty())
		throw (get_error_msg("Directive \"buffer size\" is duplicated"));
	if (target.size() != 1)
		throw (get_error_msg("Invalid option numbers for \"buffer size\" rule"));
	std::string option = target[0];
	size_t tmp = 0;
	for (size_t i = 0; i < option.size(); ++i)
	{
		if (option[i] < '0' || option[i] > '9')
		{
			if (i ==  option.size() - 1)
				break ;
			throw (get_error_msg("Invalid option \"" + option + "\" in buffer size rule"));
		}
		tmp = tmp * 10 + option[i] - '0';
		if (tmp > SIZE_MAX_NUM)
			throw (get_error_msg("Invalid option \"" + option + "\" in buffer size rule"));
	}
	switch (option[option.size() - 1])
	{
		case 'G':
		case 'g':
			tmp <<= 10;
			// FALLTHROUGH
		case 'M':
		case 'm':
			tmp <<= 10;
			// FALLTHROUGH
		case 'K':
		case 'k':
			tmp <<= 10;
			if (option.size() == 1)
				throw (get_error_msg("Invalid option \"" + option + "\" in buffer size rule"));
			// FALLTHROUGH
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break ;
		default:
			throw (get_error_msg("Invalid option \"" + option + "\" in buffer size rule"));
	}
	std::stringstream ss; ss << tmp;
	dirs[d].push_back(ss.str());
	if (!dirs[CLIENT_BODY_BUFFER_SIZE].empty() && !dirs[CLIENT_MAX_BODY_SIZE].empty())
	{
		if (dirs[CLIENT_BODY_BUFFER_SIZE][0].size() > dirs[CLIENT_MAX_BODY_SIZE][0].size())
			throw (SyntaxChecker::get_error_msg("client_body_buffer_size can't bigger than client_max_body_size"));
		else if (dirs[CLIENT_BODY_BUFFER_SIZE][0].size() == dirs[CLIENT_MAX_BODY_SIZE][0].size() &&
				dirs[CLIENT_BODY_BUFFER_SIZE][0] > dirs[CLIENT_MAX_BODY_SIZE][0])
				throw (SyntaxChecker::get_error_msg("client_body_buffer_size can't bigger than client_max_body_size"));
	}
}

void	SyntaxChecker::path_rule(vs dirs[], t_directive d, const vs &target)
{
	if (!dirs[d].empty())
		throw (get_error_msg("Path directive is duplicated"));
	if (target.size() != 1)
		throw (get_error_msg("Invalid option numbers for path rule"));
	if (d != CGI_SCRIPT_PASS)
		dirs[d].push_back(target[0] + "/");
	else
		dirs[d].push_back(target[0]);
}

void	SyntaxChecker::autoindex_rule(vs dirs[], const vs &target)
{
	if (!dirs[AUTOINDEX].empty())
		throw (get_error_msg("Directive \"autoindex\" is duplicated"));
	if (target.size() != 1)
		throw (get_error_msg("Invalid option numbers for \"autoindex\" rule"));
	if (target[0] != "on" && target[0] != "off")
		throw (get_error_msg("Invalid option \"" + target[0] + "\" in autoindex rule"));
	dirs[AUTOINDEX].push_back(target[0]);
}

void	SyntaxChecker::index_rule(vs dirs[], const vs &target)
{
	if (target.empty())
		throw (get_error_msg("Invalid option numbers for \"index\" rule"));
	for (size_t i = 0; i < target.size(); ++i)
	{
		for (size_t j = i + 1; j < target.size(); ++j)
			if (target[i] == target[j])
				throw (get_error_msg("Duplicated name \"" + target[i] + "\" for index rule"));
		vs::const_iterator cmp;
		VEC_LOOP(dirs[INDEX], cmp)
			if (*cmp == target[i])
				throw (get_error_msg("Duplicated name \"" + target[i] + "\" for index rule"));
		dirs[INDEX].push_back(target[i]);
	}
}

void	SyntaxChecker::return_rule(vs dirs[], const vs &target)
{
	if (dirs[RETURN].empty())
	{
		if (target.size() == 1)
		{
			if (is_num(target[0], 999))
				dirs[RETURN].push_back(target[0]), dirs[RETURN].push_back("");
			else if (target[0].compare(0, 7, "http://") == 0)
				dirs[RETURN].push_back("302"), dirs[RETURN].push_back(target[0]);
			else
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in return rule"));
		}
		else if (target.size() == 2)
		{
			if (!is_num(target[0], 999))
				throw (get_error_msg("Invalid option \"" + target[0] + "\" in return rule"));
			dirs[RETURN].push_back(target[0]);
			dirs[RETURN].push_back(target[1]);
		}
		else
			throw (get_error_msg("Invalid option numbers for \"return\" rule"));
	}
}

void	SyntaxChecker::access_rule(vs dirs[], t_directive d, const vs &target)
{
	if (!dirs[DENY].empty())
		throw (get_error_msg("Directive \"deny\" is duplicated"));
	if (target.size() != 1)
		throw (get_error_msg("Invalid option numbers for access rule"));
	if (target[0] != "all")
		throw (get_error_msg("Invalid option \"" + target[0] + "\" in allow / deny rule"));
	dirs[d].push_back(target[0]);
}