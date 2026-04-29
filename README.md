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

### Preparation for the 42 official tester

The official 42 tester binary expects a specific filesystem layout and a CGI executable. Before running it, set up the following at the repository root:

1. **Place the tester binaries** (`tester`, `cgi_tester`) at the project root and make them executable:
   ```bash
   chmod +x tester cgi_tester
   ```

2. **Create the `YoupiBanane` test directory** with the exact layout the tester expects:
   ```bash
   mkdir -p YoupiBanane/nop YoupiBanane/Yeah
   touch YoupiBanane/youpi.bad_extension
   touch YoupiBanane/youpi.bla
   touch YoupiBanane/nop/youpi.bad_extension
   touch YoupiBanane/nop/other.pouic
   touch YoupiBanane/Yeah/not_happy.bad_extension
   ```
   The tester reads each file, so populate them with any content you want it to display (e.g. `echo "youpi.bad_extension content" > YoupiBanane/youpi.bad_extension`).

3. **Use the supplied `tester.conf`** (already in the repo). It maps:
   - `/` → GET only on the project root, with `autoindex`.
   - `/post_body` → POST with `client_max_body_size 100`.
   - `/directory` → GET/POST on `YoupiBanane`, `index youpi.bad_extension`, `cgi_redir .bla ./cgi_tester`.

### Running the 42 tester

In one terminal:
```bash
./webserv tester.conf
```

In another terminal:
```bash
./tester http://localhost:8080
```

`./tester` is the test driver. `./cgi_tester` is **not** a separate tester — it is the CGI executable invoked by the server through the `cgi_redir .bla ./cgi_tester` directive in `tester.conf`, so it must remain present and executable at the project root. Press Enter through the introductory prompts. The run should reach the final test (`Post on /directory/youpi.bla with size 100000000`) and exit with code `0` without printing `FATAL ERROR`.

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
- **Virtual hosts** (multiple `server` blocks on the same port, selected via `Host` header / `server_name`)
- **Non-blocking I/O** with epoll (single event loop)
- **Connection timeouts** (60 seconds)
- **Graceful shutdown** via SIGINT/SIGTERM

## Architecture & Troubleshooting

This section documents the four issues that cost the most time to diagnose against the official 42 tester, and how each was fixed. They are listed because the symptoms were misleading and a future reader might hit the same walls.

### 0. RST-on-close vs. the "always go through epoll" rule

**Symptom:** Large CGI POSTs (`100 MB` to `/directory/youpi.bla`) randomly produced `connection reset by peer` mid-response.

**Root cause:** When `close(fd)` is called while the kernel still has unread bytes in the socket's recv buffer, Linux sends `RST` instead of `FIN` (RFC 1122 § 4.2.2.13). The Go-based tester treats `RST` as a truncated stream.

**Constraint:** the 42 subject forbids `read()`/`write()` on a client fd that did not just come out of `epoll_wait()`. A naive drain loop directly inside `close()` violates that rule.

**Fix:** introduce an explicit `CS_CLOSING` state (`src/services/Closer.cpp`). `Sender::Exit()` returns `CS_CLOSING`; `TaskManager::HandleClientUpdate` re-arms the fd for `EPOLLIN`; `Closer::Initialize` issues `shutdown(SHUT_WR)` so the peer sees `FIN`; `Closer::Execute` runs only after a real `epoll_wait` `EPOLLIN` event and drains the recv buffer until empty, then transitions to `CS_DEAD` for cleanup. Stuck draining clients are cleared by the existing 60 s timeout.

### 1. CGI dual-pipe deadlock at the 64 KB pipe buffer

**Symptom:** Large CGI POSTs (e.g. `100 MB` to `/directory/youpi.bla`) hung indefinitely. The `cgi_tester` binary echoes its input back to stdout, so the parent has to write the request body to the child's stdin **and** read the child's stdout concurrently.

**Root cause:** A single-threaded "write all input, then read all output" loop deadlocks on Linux as soon as the child's stdout exceeds ~64 KB (the kernel pipe buffer). The child blocks on `write(stdout)`, the parent blocks on `write(stdin)`, neither side moves.

**Fix:** Both pipes are registered in epoll independently. `TaskManager::HandleCgiEvent` (`src/routers/TaskManager.cpp`) drives `EPOLLOUT` on the input pipe and `EPOLLIN` on the output pipe in the same poll cycle, so the parent always drains stdout while still feeding stdin.

### 2. Listen backlog SOMAXCONN

**Symptom:** The `Test multiple workers(128) doing multiple times(50): GET on /directory/nop` step intermittently failed with refused connections.

**Root cause:** `listen()` was called with a backlog of `10` (the old `DEF_MAX_CONNS`). With 128 concurrent workers, the kernel's accept queue overflowed and new SYNs were dropped before `accept()` had a chance to drain them.

**Fix:** `Socket::OpenMainSocket` now calls `listen(fd, SOMAXCONN)`. The kernel-supplied maximum is the right ceiling for a stress test, and our application-level limit is enforced separately by `connectionsMax`.

### 3. Sender retry semantics under load

**Symptom:** During the `multiple workers × multiple times × 100 MB POST` stress, occasional 400 responses appeared even though the request was well-formed.

**Root cause:** A debug branch had been added to `Sender::Execute` returning `ERROR` on `write()` returning `<= 0`. Per the 42 subject we cannot inspect `errno`, so this incorrectly treated transient `EAGAIN` (the socket buffer was full) as a fatal error and synthesised a 400.

**Fix:** `Sender::Execute` (`src/services/Sender.cpp`) now treats `n > 0` as progress, `n == 0` as `FINISHED`, and any other value as "retry on the next `EPOLLOUT`" — matching the subject's no-`errno` rule.

### Smoke test (without the 42 tester)

A quick way to verify GET / POST / DELETE end-to-end. With `./webserv default.conf` running:

```bash
# GET an existing page
curl -i http://localhost:8080/

# POST a file into the upload location
curl -i -X POST http://localhost:8080/upload/note.txt -d "hello"
ls test_site/upload/note.txt   # should now exist with body "hello"

# DELETE that file
curl -i -X DELETE http://localhost:8080/upload/note.txt
ls test_site/upload/note.txt   # should now be missing
```

Expected status codes: `200`, `201`, `200`. POST against a location without `upload_store` returns `403`; against a missing `upload_store` directory, `404`.

### How to reproduce / verify

After the preparation steps above:
```bash
make re
./webserv tester.conf              # terminal 1
./tester http://localhost:8080     # terminal 2 — must finish with exit 0, no FATAL ERROR
```

## Resources

- [RFC 2616 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616)
- [RFC 3875 - The Common Gateway Interface (CGI)](https://www.rfc-editor.org/rfc/rfc3875)
- [NGINX Documentation](https://nginx.org/en/docs/)
- [epoll(7) - Linux manual page](https://man7.org/linux/man-pages/man7/epoll.7.html)

### AI Usage

AI (Claude) was used as a development assistant for the following tasks:
- Debugging critical issues (epoll registration, state machine transitions, non-blocking I/O)
- Code review and compliance checking against project requirements
- Writing this README and the architecture documentation

