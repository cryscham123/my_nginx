/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxChecker.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 08:41:55 by hyunghki          #+#    #+#             */
/*   Updated: 2024/08/29 09:46:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SyntaxChecker_HPP
# define SyntaxChecker_HPP

# include <iostream>
# include <vector>

# define DEFAULT_PATH "./conf/nginx.conf"
# define DEFAULT_CONTENT "\
http {\n\
        # 필요에 의해 수정해주세요\n\
        root \"/tmp/html\";\n\
        server {\n\
                listen 80;\n\
                listen [::]:80;\n\
                server_name \"localhost\";\n\
                index index.html;\n\
                location /{\n\
                        upload_store \"/upload_from_client\";\n\
                        limit_except GET PUT {\n\
                                deny all;\n\
                        }\n\
                        error_page 404                          404.html;\n\
                        error_page 500 502 503 504      /50x.html;\n\
                }\n\
                location /upload_from_client {\n\
                        root \"/tmp/html/upload_from_client\";\n\
                        autoindex on;\n\
                        limit_except GET {\n\
                                deny all;\n\
                        }\n\
                }\n\
                location ~ .jpg {\n\
                        # 필요에 의해 수정해주세요\n\
                        root \"/tmp/data/photos\";\n\
                        # client_body_buffer_size 8k;\n\
                        # client_max_body_size 1m;\n\
                }\n\
                location ~ .py{\n\
                        # 필요에 의해 수정해주세요\n\
                        cgi_script_pass \"/tmp/cgi-bin/python-cgi\";\n\
                }\n\
                location ~ .php{\n\
                        # 필요에 의해 수정해주세요\n\
                        cgi_script_pass \"/tmp/cgi-bin/php-cgi\";\n\
                }\n\
        }\n\
        server {\n\
                listen 80;\n\
                listen [::]:80;\n\
                server_name \"\" \"wrong_host\";\n\
                return 404;\n\
        }\n\
        server {\n\
                listen 8080;\n\
                location / {\n\
                        return \"http://www.naver.com\";\n\
                }\n\
        }\n\
}"
# define MAP_LOOP(M, I) for(I = M.begin(); I != M.end(); ++I)
# define VEC_LOOP(V, I) for(I = V.begin(); I != V.end(); ++I)

class ARule;

class SyntaxChecker
{
	public:
		enum t_directive
		{
			HTTP,
			SERVER,
			LOCATION,
			LISTEN,
			SERVER_NAME,
			RETURN,
			UPLOAD_STORE,
			ROOT,
			LIMIT_EXCEPT,
			CGI_SCRIPT_PASS,
			ERROR_PAGE,
			CLIENT_BODY_BUFFER_SIZE,
			CLIENT_MAX_BODY_SIZE,
			AUTOINDEX,
			INDEX,
			DENY,
			DIRECTIVE_SIZE
		};
		static void				learn(int ac, const char *target_file);
		static void				parse_context(ARule &r);
		static std::logic_error	get_error_msg(const std::string &msg);
		static void				insert_directive(ARule &r, std::vector<std::string>[]);

	private:
		typedef std::vector<std::string> vs;
		const static int	F_ROOT_RULE = 0;
		const static int 	PORT_MAX_NUM = 65535;
		const static size_t SIZE_MAX_NUM = 9223372036;

		enum t_directive_info
		{
			INVALID,
			CONTEXT,
			NO_INHERIT,
			VALID
		};
		enum t_parse_state
		{
			SKIP_SPACE,
			PROGRESS,
			COMMENTS,
			ERROR,
			WORD_END,
			END
		};
		SyntaxChecker();
		SyntaxChecker(const SyntaxChecker &ref);
		SyntaxChecker &operator=(const SyntaxChecker &ref);
		~SyntaxChecker();

		static t_directive		get_directive_enum(const std::string &s);
		static std::string		get_word(char end_char, t_parse_state &flag);
		static void				parse_directive(vs &tmp, char end_char);
		static t_directive_info	check_directive(const ARule &rule_type, t_directive d);

		static bool	is_num(const std::string &s, unsigned long max_num);

		static void	option_validate(vs dirs[], t_directive d, const vs &target);
		static void	listen_rule(vs dirs[], const vs &target);
		static void	server_name_rule(vs dirs[], const vs &target);
		static void	error_page_rule(vs dirs[], const vs &target);
		static void	path_rule(vs dirs[], t_directive d, const vs &target);
		static void	autoindex_rule(vs dirs[], const vs &target);
		static void	index_rule(vs dirs[], const vs &target);
		static void	return_rule(vs dirs[], const vs &target);
		static void	size_rule(vs dirs[], t_directive d, const vs &target);
		static void	access_rule(vs dirs[], t_directive d, const vs &target);

		static std::string	_buf;
		static size_t		_cur;
		static size_t		_line_num;
		static size_t		_depth;
};

#endif
