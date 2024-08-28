/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:59:39 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:07:06 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONTEXT_HPP
# define SERVERCONTEXT_HPP

# include <iostream>
# include <vector>
# include "ARule.hpp"
# include "LocationRule.hpp"
# include "NetInfo.hpp"

class ServerRule: public ARule
{
	public:
		ServerRule();
		ServerRule(const ServerRule &ref);
		ServerRule &operator=(const ServerRule &ref);
		~ServerRule();

		ServerRule	*clone() const;
		void		set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &);
		void		set_option(std::vector<std::string> dir_list[]);

		bool		can_connect(const NetInfo &host) const;
		bool		is_server_name_exist(const std::string &s) const;

		const NetInfo					&get_net_info() const;
		const std::vector<std::string>	&get_listen_literal() const;
		const std::vector<std::string>	&get_server_name() const;
		LocationRule					*get_location(const std::string &uri) const;
		
		void							set_net_info(const NetInfo &info);
		void							set_listen_literal(const std::string &s);

	private:
		NetInfo						_net_info;
		std::vector<std::string>	_listen_literal;
		std::vector<std::string>	_server_name;

		std::vector<LocationRule *>	_cgi_location_rule;
		std::vector<LocationRule *>	_location_rule;
};

#endif
