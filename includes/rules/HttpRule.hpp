/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRule.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:58:51 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/21 15:06:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRULE_HPP
# define HTTPRULE_HPP

# include <iostream>
# include <vector>
# include <map>
# include "SyntaxChecker.hpp"
# include "ARule.hpp"
# include "ServerRule.hpp"
# include "NetInfo.hpp"

class HttpRule: public ARule
{
	public:
		HttpRule();
		HttpRule(const HttpRule &ref);
		HttpRule &operator=(const HttpRule &ref);
		~HttpRule();

		HttpRule	*clone() const;
		void		set_sub_rule(SyntaxChecker::t_directive rule, const std::vector<std::string> &);
		void		set_option(std::vector<std::string> dir_list[]);

		std::vector<ServerRule *>	get_server(const NetInfo &n) const;
		const std::vector<NetInfo>	&get_socket_list() const;

	private:
		void						set_socket_list(const NetInfo &n);

		std::map<int, std::vector<ServerRule *> >	_server_rule;
		std::vector<NetInfo>						_socket_list;
};

#endif
