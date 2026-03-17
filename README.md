*This project has been created as part of the 42 curriculum by vtrofyme, ikulik.*

## Description

Webserv is an HTTP/1.1 server written in C++98. It is designed to handle multiple simultaneous connections using a non-blocking I/O model powered by `epoll`. The server supports serving static files, file uploads, CGI script execution, directory listing, custom error pages, and HTTP redirects.

The server uses an NGINX-inspired configuration file format that allows defining multiple server blocks with different ports and per-route settings.

## Instructions

### Compilation

```bash
make        # Build the project
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Rebuild from scratch
```

### Execution

```bash
./webserv [configuration file]
```

If no configuration file is provided, the server will look for `default.conf` in the current directory.

### Configuration

The configuration file uses an NGINX-inspired syntax. Example:

```nginx
server {
    listen 8080;
    server_name localhost;
    client_max_body_size 1M;
    error_page 404 test_site/errors/404.html;

    location / {
        root test_site;
        index index.html;
        autoindex on;
        methods GET POST DELETE;
    }

    location /upload {
        root test_site/upload;
        upload_store test_site/upload;
        methods GET POST DELETE;
        autoindex on;
    }

    location /cgi-bin {
        root test_site/cgi-bin;
        cgi_extension .py;
        cgi_path /usr/bin/python3;
        methods GET POST;
    }
}
```

### Configuration Directives

**Server block:**
- `listen` - Port to listen on
- `server_name` - Server hostname
- `client_max_body_size` - Maximum request body size (supports K, M, G suffixes)
- `error_page` - Custom error page (e.g., `error_page 404 /path/to/404.html`)

**Location block:**
- `root` - Document root directory
- `index` - Default index file
- `autoindex` - Enable/disable directory listing (`on`/`off`)
- `methods` - Allowed HTTP methods (GET, POST, DELETE)
- `redirect` - HTTP redirect URL
- `upload_store` - Directory for file uploads
- `cgi_extension` - CGI file extension (e.g., `.py`)
- `cgi_path` - Path to CGI interpreter

### Features

- **HTTP Methods:** GET, POST, DELETE
- **Static file serving** with MIME type detection
- **File uploads** (raw and multipart/form-data)
- **CGI execution** (Python, and other interpreters)
- **Directory listing** (autoindex)
- **Custom error pages**
- **HTTP redirects** (301)
- **Chunked transfer encoding** support
- **Multiple server blocks** with different ports
- **Non-blocking I/O** with epoll (single event loop)
- **Connection timeouts** (60 seconds)
- **Graceful shutdown** via SIGINT/SIGTERM

## Resources

- [RFC 2616 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616)
- [RFC 3875 - The Common Gateway Interface (CGI)](https://www.rfc-editor.org/rfc/rfc3875)
- [NGINX Documentation](https://nginx.org/en/docs/)
- [epoll(7) - Linux manual page](https://man7.org/linux/man-pages/man7/epoll.7.html)

