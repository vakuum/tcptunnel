#ifndef TCPTUNNEL_H
#define TCPTUNNEL_H

#define VERSION "0.6"

#define SIZE 4096

#define LOCAL_PORT_OPTION   'a'
#define REMOTE_PORT_OPTION  'b'
#define REMOTE_HOST_OPTION  'c'
#define BIND_ADDRESS_OPTION 'd'
#define FORK_OPTION         'e'
#define LOG_TO_STDOUT       'f'
#define STAY_ALIVE_OPTION   'g'
#define HELP_OPTION         'h'
#define VERSION_OPTION      'i'

#define PATH_SEPARATOR '/'

const char *exec_name;

int build_server(void);
int wait_for_clients(void);
void handle_client(void);
void handle_tunnel(void);
int build_tunnel(void);
int use_tunnel(void);
int fd(void);

void set_options(int argc, char *argv[]);
void set_option(char **option, char *value);

char *get_current_timestamp(void);
char *get_exec_name(char *argv[]);

void print_help(void);
void print_helpinfo(void);
void print_usage(void);
void print_version(void);
void print_missing(const char *message);

struct struct_settings {
	unsigned int local_port    : 1;
	unsigned int remote_host   : 1;
	unsigned int remote_port   : 1;
	unsigned int bind_address  : 1;
	unsigned int fork          : 1;
	unsigned int log_to_stdout : 1;
	unsigned int stay_alive    : 1;
};

struct struct_options {
	char *local_port;
	char *remote_host;
	char *remote_port;
	char *bind_address;
};

struct struct_rc {
	unsigned int server_socket;
	unsigned int client_socket;
	unsigned int remote_socket;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	struct sockaddr_in remote_addr;
	struct hostent *remote_host;
};

#endif

