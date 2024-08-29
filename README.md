<h3 align="center">
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="30" width="0px"/><br/>
    <img src="https://42seoul.kr/template/cms/seoul42/images/common/head_logo.png" width="250" alt="Logo"/><br/>
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="70" width="0px"/>
    Webserv Project for <a href="https://42seoul.kr/seoul42/main/view">42 Seoul</a><br/>
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="10" width="0px"/>
</h3>

<p align="center">
This repository is for the 42 seoul project <strong>Webserv</strong>.<br />The objective of this project is to implement own http server.
</p>

> [!Note]
> This repository contains only a portion of the code related to parsing the configuration file.  
> Due to the fact that I am not the sole author of the project, I cannot share the full codebase.

## Features

- Support MacOS, Linux

## Usage
- You can execute file with one argument [ConfigFile](https://github.com/cryscham123/my_nginx#Config)
- If there is no argument, default config file will be created and excuted.
```bash
./webserv [config file]
```
- Create your own config file in `conf` directory

## Config

- You can put multiple `directives` in proper `context`.
- possible context: `http`, `server`, `location`, `limit_except`
- directives can be nested from parent context.
- If directive is not setted, default value can be inserted.

### Http Context
Basic context. All configuration must be written in here.
<details>
<summary>possible directives</summary>
<ul>
<li>server</li>
<li>root</li>
<li>error_page</li>
<li>client_body_buffer_size</li>
<li>client_max_body_size</li>
<li>autoindex</li>
<li>index</li>
<li>deny</li>
</ul>
</details>

### Server Context
Sets configuration for a virtual server. The first server in config file will be default server.  
<details>
<summary>possible directives</summary>
<ul>
<li>location</li>
<li>listen</li>
<li>server_name</li>
<li>return</li>
<li>upload_store</li>
<li>root</li>
<li>error_page</li>
<li>client_body_buffer_size</li>
<li>client_max_body_size</li>
<li>autoindex</li>
<li>index</li>
<li>deny</li>
</ul>
</details>

### Location Context
Sets configuration depending on a request URI.
<details>
<summary>possible directives</summary>
<ul>
<li>limit_except</li>
<li>cgi_script_pass</li>
<li>return</li>
<li>upload_store</li>
<li>root</li>
<li>error_page</li>
<li>client_body_buffer_size</li>
<li>client_max_body_size</li>
<li>autoindex</li>
<li>index</li>
<li>deny</li>
</ul>
</details>

### Limit_Except Context
Sets possible http method.
<details>
<summary>possible directives</summary>
<ul>
<li>deny</li>
</ul>
</details>

> If you want to know about syntax of each directives, see [Nginx docs](https://nginx.org/en/docs/dirindex.html).  
> almost same.

### Example
```nginx
http {
        root "/tmp/html";
	# This will be default server. All unmatched request will be handled by this server.
        server {
                listen 80;
                listen [::]:80;
                server_name "localhost";
                index index.html;
                location /{
                        upload_store "/upload_from_client";
                        limit_except GET PUT {
                                deny all;
                        }
                        error_page 404                          404.html;
                        error_page 500 502 503 504      /50x.html;
                }
                location /upload_from_client {
                        root "/tmp/html/upload_from_client";
                        autoindex on;
                        limit_except GET {
                                deny all;
                        }
                }
		# you can set location based on request's extension
                location ~ .jpg { 
                        root "/tmp/data/photos";
                        client_body_buffer_size 8k;
                        client_max_body_size 1m;
                }
                location ~ .py{
                        cgi_script_pass "/tmp/cgi-bin/python-cgi";
                }
                location ~ .php{ 
                        cgi_script_pass "/tmp/cgi-bin/php-cgi";
                }
        }
        server {
                listen 80;
                listen [::]:80;
                server_name "" "wrong_host";
                return 404;
        }
	# This request will be redirected to naver.com
        server {
                listen 8080;
                location / {
                        return "http://www.naver.com";
                }
        }
}
```
