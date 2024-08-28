/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 15:16:56 by hyunghki          #+#    #+#             */
/*   Updated: 2023/11/09 07:52:40 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEMANAGER_HPP
# define FILEMANAGER_HPP

# include <iostream>

class FileManager
{
	public:
		// read file content in path, store in buf
		// if error occur, throw exception
		static void read_file(const char *path, std::string &buf);
		// read buf, and write in path
		// if error occur, throw exception
		static void write_file(const char *path, const std::string &buf, std::ios_base::openmode flag);
		static void try_new_file(const char *path, const std::string &buf);

	private:
		FileManager();
		FileManager(const FileManager &ref);
		FileManager &operator=(const FileManager &ref);
		~FileManager();
};

#endif
