/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetInfo.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 08:41:27 by hyunghki          #+#    #+#             */
/*   Updated: 2024/08/29 08:41:31 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETINFO_HPP
# define NETINFO_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>

class NetInfo
{
	public:
		NetInfo();
		// dont use this constructor except when creating rule.
		NetInfo(const std::string &listen_literal);
		NetInfo(const char *ip, const char *port_num);
		NetInfo(const NetInfo &ref);
		NetInfo &operator=(const NetInfo &ref);
		~NetInfo();

		int			get_domain() const;
		uint16_t	get_port_num() const;
		const char	*get_ip_addr();
		// not accept regardless of domain. just in specific domain
		bool		is_accept_all(int domain) const;
		int			bind_socket(int listen_sfd) const;
		int			accept_socket(int listen_sfd);

		bool		operator==(const NetInfo &ref) const;
		bool		operator!=(const NetInfo &ref) const;

	private:
		int					_domain;
		uint16_t			_port_num;
		struct sockaddr_in	_ipv4;
		struct sockaddr_in6	_ipv6;
		std::string			_ip_addr;
};

#endif
