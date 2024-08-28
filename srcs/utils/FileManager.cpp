/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 15:25:07 by hyunghki          #+#    #+#             */
/*   Updated: 2023/11/11 17:49:55 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileManager.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

FileManager::FileManager() {}
FileManager::FileManager(const FileManager &ref) {(void)ref;}
FileManager &FileManager::operator=(const FileManager &ref) {(void)ref; return (*this);}
FileManager::~FileManager() {}

void FileManager::read_file(const char *path, std::string &buf)
{
	std::ifstream in(path);
	if (!in.is_open())
		throw (std::logic_error("Can't read file in \"" + std::string(path) + "\""));
	in.seekg(0, std::ios::end);
	int size = in.tellg();
	buf.resize(size);
	in.seekg(0, std::ios::beg);
	in.read(&buf[0], size);
	in.close();
}

void FileManager::try_new_file(const char *path, const std::string &buf)
{
	if (access(path, F_OK))
		write_file(path, buf, std::ios_base::out);
}

void FileManager::write_file(const char *path, const std::string &buf, std::ios_base::openmode flag)
{
	std::ofstream out(path, flag);
	if (!out.is_open())
		throw (std::logic_error("Can't write file in \"" + std::string(path) + "\""));
	out << buf;
	out.close();
}
