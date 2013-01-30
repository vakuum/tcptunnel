/*
 * Copyright (C) 2000-2012 Clemens Fuchslocher <clemens@vakuumverpackt.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 */

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __MINGW32__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include "tcptunnel.h"

struct struct_rc rc;
struct struct_options options;
struct struct_settings settings = { 0, 0, 0, 0, 0, 0, 0 };

static struct option long_options[] = {
	{ "local-port",    required_argument, NULL, LOCAL_PORT_OPTION },
	{ "remote-host",   required_argument, NULL, REMOTE_HOST_OPTION },
	{ "remote-port",   required_argument, NULL, REMOTE_PORT_OPTION },
	{ "bind-address",  required_argument, NULL, BIND_ADDRESS_OPTION },
#ifndef __MINGW32__
	{ "fork",          no_argument,       NULL, FORK_OPTION },
#endif
	{ "log-to-stdout", no_argument,       NULL, LOG_TO_STDOUT },
	{ "stay-alive",    no_argument,       NULL, STAY_ALIVE_OPTION },
	{ "help",          no_argument,       NULL, HELP_OPTION },
	{ "version",       no_argument,       NULL, VERSION_OPTION },
	{ 0, 0, 0, 0 }
};

int main(int argc, char *argv[])
{
#ifdef __MINGW32__
	WSADATA info;
	if (WSAStartup(MAKEWORD(1,1), &info) != 0)
	{
		perror("main: WSAStartup()");
		exit(1);
	}
#endif

	name = argv[0];

	set_options(argc, argv);

	if (build_server() == 1)
	{
		exit(1);
	}

#ifndef __MINGW32__
	signal(SIGCHLD, SIG_IGN);
#endif

	do
	{
		if (wait_for_clients() == 0)
		{
			handle_client();
		}
	}
	while (settings.stay_alive);

	close(rc.server_socket);

	return 0;
}

void set_options(int argc, char *argv[])
{
	int opt;
	int index;

	do
	{
		opt = getopt_long(argc, argv, "", long_options, &index);
		switch (opt)
		{
			case LOCAL_PORT_OPTION:
			{
				set_option(&options.local_port, optarg);
				settings.local_port = 1;
				break;
			}

			case REMOTE_PORT_OPTION:
			{
				set_option(&options.remote_port, optarg);
				settings.remote_port = 1;
				break;
			}

			case REMOTE_HOST_OPTION:
			{
				set_option(&options.remote_host, optarg);
				settings.remote_host = 1;
				break;
			}

			case BIND_ADDRESS_OPTION:
			{
				set_option(&options.bind_address, optarg);
				settings.bind_address = 1;
				break;
			}

			case FORK_OPTION:
			{
				settings.fork = 1;
				settings.stay_alive = 1;
				break;
			}

			case LOG_TO_STDOUT:
			{
				settings.log_to_stdout = 1;
				break;
			}

			case STAY_ALIVE_OPTION:
			{
				settings.stay_alive = 1;
				break;
			}

			case HELP_OPTION:
			{
				print_usage();
				print_help();
				exit(0);
			}

			case VERSION_OPTION:
			{
				print_version();
				exit(0);
			}

			case '?':
			{
				print_usage();
				print_helpinfo();
				exit(0);
			}
		}
	}
	while (opt != -1);

	if (!settings.local_port)
	{
		print_missing("missing '--local-port=' option.");
		exit(1);
	}

	if (!settings.remote_port)
	{
		print_missing("missing '--remote-port=' option.");
		exit(1);
	}

	if (!settings.remote_host)
	{
		print_missing("missing '--remote-host=' option.");
		exit(1);
	}
}

void set_option(char **option, char *value)
{
	size_t size = sizeof(char) * (strlen(value) + 1);

	*option = (char *) malloc(size);
	if (*option == NULL)
	{
		perror("set_option: malloc()");
		exit(1);
	}

	strncpy(*option, value, size);
}

int build_server(void)
{
	memset(&rc.server_addr, 0, sizeof(rc.server_addr));

	rc.server_addr.sin_port = htons(atoi(options.local_port));
	rc.server_addr.sin_family = AF_INET;
	rc.server_addr.sin_addr.s_addr = INADDR_ANY;

	rc.server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (rc.server_socket < 0)
	{
		perror("build_server: socket()");
		return 1;
	}
	
	int optval = 1;
	if(setsockopt(rc.server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("build_server: setsockopt(SO_REUSEADDR");
		return 1;
	}

	if (settings.bind_address)
	{
		rc.server_addr.sin_addr.s_addr = inet_addr(options.bind_address);
	}

	if (bind(rc.server_socket, (struct sockaddr *) &rc.server_addr, sizeof(rc.server_addr)) < 0)
	{
		perror("build_server: bind()");
		return 1;
	}

	if (listen(rc.server_socket, 1) < 0)
	{
		perror("build_server: listen()");
		return 1;
	}

	return 0;
}

int wait_for_clients(void)
{
#ifndef __MINGW32__
	unsigned int client_addr_size;
#else
	int client_addr_size;
#endif

	client_addr_size = sizeof(struct sockaddr_in);

	rc.client_socket = accept(rc.server_socket, (struct sockaddr *) &rc.client_addr, &client_addr_size);
	if (rc.client_socket < 0)
	{
		if (errno != EINTR) {
			perror("wait_for_clients: accept()");
		}
		return 1;
	}

	if (settings.log_to_stdout)
	{
		printf("> %s tcptunnel: request from %s\n", get_current_timestamp(), inet_ntoa(rc.client_addr.sin_addr));
	}

	return 0;
}

void handle_client(void)
{
#ifndef __MINGW32__
	if (settings.fork)
	{
		if (fork() == 0)
		{
			close(rc.server_socket);
			handle_tunnel();
			exit(0);
		}
		close(rc.client_socket);
	}
	else
	{
#endif
		handle_tunnel();
#ifndef __MINGW32__
	}
#endif
}

void handle_tunnel(void)
{
	if (build_tunnel() == 0)
	{
		use_tunnel();
	}

}

int build_tunnel(void)
{
	rc.remote_host = gethostbyname(options.remote_host);
	if (rc.remote_host == NULL)
	{
		perror("build_tunnel: gethostbyname()");
		return 1;
	}

	memset(&rc.remote_addr, 0, sizeof(rc.remote_addr));

	rc.remote_addr.sin_family = AF_INET;
	rc.remote_addr.sin_port = htons(atoi(options.remote_port));

	memcpy(&rc.remote_addr.sin_addr.s_addr, rc.remote_host->h_addr, rc.remote_host->h_length);

	rc.remote_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (rc.remote_socket < 0)
	{
		perror("build_tunnel: socket()");
		return 1;
	}

	if (connect(rc.remote_socket, (struct sockaddr *) &rc.remote_addr, sizeof(rc.remote_addr)) < 0)
	{
		perror("build_tunnel: connect()");
		return 1;
	}

	return 0;
}

int use_tunnel(void)
{
	fd_set io;
	char buffer[SIZE];

	for (;;)
	{
		FD_ZERO(&io);
		FD_SET(rc.client_socket, &io);
		FD_SET(rc.remote_socket, &io);

		memset(buffer, 0, SIZE);

		if (select(fd(), &io, NULL, NULL, NULL) < 0 )
		{
			perror("use_tunnel: select()");
			break;
		}

		if (FD_ISSET(rc.client_socket, &io))
		{
			int count = recv(rc.client_socket, buffer, SIZE, 0);
			if (count < 0)
			{
				perror("use_tunnel: recv(rc.client_socket)");
				close(rc.client_socket);
				close(rc.remote_socket);
				return 1;
			}

			if (count == 0)
			{
				close(rc.client_socket);
				close(rc.remote_socket);
				return 0;
			}

			send(rc.remote_socket, buffer, count, 0);

			if (settings.log_to_stdout)
			{
				printf("> %s > ", get_current_timestamp());
				fwrite(buffer, sizeof(char), count, stdout);
				fflush(stdout);
			}
		}

		if (FD_ISSET(rc.remote_socket, &io))
		{
			int count = recv(rc.remote_socket, buffer, SIZE, 0);
			if (count < 0)
			{
				perror("use_tunnel: recv(rc.remote_socket)");
				close(rc.client_socket);
				close(rc.remote_socket);
				return 1;
			}

			if (count == 0)
			{
				close(rc.client_socket);
				close(rc.remote_socket);
				return 0;
			}

			send(rc.client_socket, buffer, count, 0);

			if (settings.log_to_stdout)
			{
				fwrite(buffer, sizeof(char), count, stdout);
				fflush(stdout);
			}
		}
	}

	return 0;
}

int fd(void)
{
	unsigned int fd = rc.client_socket;
	if (fd < rc.remote_socket)
	{
		fd = rc.remote_socket;
	}
	return fd + 1;
}

char *get_current_timestamp(void)
{
	static char date_str[SIZE];
	time_t date;

	time(&date);
	strftime(date_str, SIZE - 1, "%a %d %H:%M:%S", localtime(&date));
	return date_str;
}

void print_usage(void)
{
	fprintf(stderr, "Usage: %s [options]\n\n", name);
}

void print_helpinfo(void)
{
	fprintf(stderr, "Try `%s --help' for more options\n", name);
}

void print_help(void)
{
	fprintf(stderr, "\
Options:\n\
  --version\n\
  --help\n\n\
  --local-port=PORT    local port\n\
  --remote-port=PORT   remote port\n\
  --remote-host=HOST   remote host\n\
  --bind-address=IP    bind address\n"
#ifndef __MINGW32__
"  --fork               fork-based concurrency\n"
#endif
"  --log-to-stdout\n\
  --stay-alive\n\n\
\n");
}

void print_version(void)
{
	fprintf(stderr, "\
tcptunnel v" VERSION " Copyright (C) 2000-2012 Clemens Fuchslocher\n\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\n\
Written by Clemens Fuchslocher <clemens@vakuumverpackt.de>\n\
");
}

void print_missing(const char *message)
{
	print_usage();
	fprintf(stderr, "%s: %s\n", name, message);
	print_helpinfo();
}

