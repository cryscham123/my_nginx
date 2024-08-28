/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRule.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 13:00:18 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/22 14:24:19 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONRULE_HPP
# define LOCATIONRULE_HPP

# include <iostream>
# include <vector>
# include <map>
# include "ARule.hpp"
# include "MethodRule.hpp"
# include "NetInfo.hpp"

class LocationRule: public ARule
{
	public:
		LocationRule(const std::string &path);
		LocationRule(const LocationRule &ref);
		LocationRule &operator=(const LocationRule &ref);
		~LocationRule();

		LocationRule	*clone() const;
		void			set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &);
		void			set_option(std::vector<std::string> dir_list[]);
		bool			can_access(const std::string &method) const;

		const std::string				&get_path() const;
		size_t							get_client_body_buffer_size() const;
		size_t							get_client_max_body_size() const;
		const std::string				&get_root() const;
		const std::vector<std::string>	&get_index() const;
		bool							get_autoindex() const;
		// [0] = status code, [1] = return resource. this can be empty
		const std::vector<std::string>	&get_return() const;
		// if no resource corresponding with status code, return NULL
		const char						*get_upload_store() const;
		const char						*get_error_page(const std::string &status_code) const;
		const char						*get_cgi_script_pass() const;

	private:
		LocationRule();

		MethodRule							*_method_rule[MethodRule::METHOD_SIZE];
		std::string							_path;
		size_t								_body_buffer[2];
		std::vector<std::string>			_root;
		std::vector<std::string>			_upload_store;
		std::vector<std::string>			_cgi_script_pass;
		std::vector<std::string>			_index;
		int									_autoindex;
		std::vector<std::string>			_return;
		std::map<std::string, std::string>	_error_page;
};
#endif
