/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetInfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 15:25:07 by hyunghki          #+#    #+#             */
/*   Updated: 2023/11/09 07:49:55 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetInfo.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

NetInfo::NetInfo(const std::string &listen_literal)
{
	size_t i = listen_literal.find_last_of(':');
	std::string ip(listen_literal.begin(), listen_literal.begin() + i);
	std::string port(listen_literal.begin() + i + 1, listen_literal.end());
	*this = NetInfo(ip.c_str(), port.c_str());
}

NetInfo::NetInfo(const char *ip, const char *port_num):
	_domain(AF_UNSPEC)
{
    struct addrinfo hints, *result;
	std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	if (getaddrinfo(ip, port_num, &hints, &result) != 0)
		return ;
	_domain = result->ai_family;
	if (_domain == AF_INET)
	{
		std::memcpy(&_ipv4, result->ai_addr, result->ai_addrlen);
		_port_num = ntohs(_ipv4.sin_port);
		_ipv4.sin_port = htons(_port_num);
		_ipv4.sin_family = AF_INET;
	}
	else
	{
		std::memcpy(&_ipv6, result->ai_addr, result->ai_addrlen);
		_port_num = ntohs(_ipv6.sin6_port);
		_ipv6.sin6_port = htons(_port_num);
		_ipv6.sin6_family = AF_INET6;
	}
	freeaddrinfo(result);
}

NetInfo::NetInfo() {}
NetInfo::NetInfo(const NetInfo &ref):
	_domain(ref._domain),
	_port_num(ref._port_num),
	_ipv4(ref._ipv4),
	_ipv6(ref._ipv6) {}

NetInfo &NetInfo::operator=(const NetInfo &ref)
{
	if (this != &ref)
	{
		_domain = ref._domain;
		_port_num = ref._port_num;
		_ipv4 = ref._ipv4;
		_ipv6 = ref._ipv6;
	}
	return (*this);
}
NetInfo::~NetInfo() {}

bool	NetInfo::operator==(const NetInfo &ref) const
{
	//if (get_port_num() != ref.get_port_num())
	//	return (false);
	if (_domain == AF_INET && ref.get_domain() == AF_INET)
		if (std::memcmp(&_ipv4.sin_addr, &ref._ipv4.sin_addr, sizeof(struct in_addr)) == 0)
			return (true);
	if (_domain == AF_INET6 && ref.get_domain() == AF_INET6)
		if (std::memcmp(&_ipv6.sin6_addr, &ref._ipv6.sin6_addr, sizeof(struct in6_addr)) == 0)
			return (true);
	return (false);
}

bool	NetInfo::operator!=(const NetInfo &ref) const
{
	return (!(*this == ref));
}

bool	NetInfo::is_accept_all(int domain) const
{
	if (_domain != domain)
		return (false);
	if (_domain == AF_INET)
		return (_ipv4.sin_addr.s_addr == INADDR_ANY);
	if (_domain == AF_INET6)
		return (std::memcmp(&_ipv6.sin6_addr, &in6addr_any, sizeof(struct in6_addr)) == 0);
	return (false);
}

int NetInfo::bind_socket(int listen_sfd) const
{
	if (_domain == AF_INET)
		return (bind(listen_sfd, (struct sockaddr *)&_ipv4, sizeof(_ipv4)));
	else if (_domain == AF_INET6)
		return (bind(listen_sfd, (struct sockaddr *)&_ipv6, sizeof(_ipv6)));
	return (-1);
}

int NetInfo::accept_socket(int listen_sfd)
{
	int	size;
	if (_domain == AF_INET)
	{
		size = sizeof(_ipv4);
		return (accept(listen_sfd, (struct sockaddr *)&_ipv4, (socklen_t *) &size));
	}
	else if (_domain == AF_INET6)
	{
		size = sizeof(_ipv6);
		return (accept(listen_sfd, (struct sockaddr *)&_ipv6, (socklen_t *) &size));
	}
	return (-1);
}


int	NetInfo::get_domain() const
{
	return (_domain);
}

// need to fix ipv6
const char *NetInfo::get_ip_addr()
{
	if (_domain == AF_INET)
		_ip_addr = std::to_string(ntohl(_ipv4.sin_addr.s_addr));
	else if (_domain == AF_INET6)
		_ip_addr = std::to_string(ntohl(_ipv6.sin6_addr.__u6_addr.__u6_addr32[0]));

	if (_ip_addr.size() != 0)
		return (_ip_addr.c_str());
	else
		return (NULL);
}

uint16_t	NetInfo::get_port_num() const
{
	return (_port_num);
}
