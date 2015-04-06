#ifndef TCPTUNNEL_H
#define TCPTUNNEL_H

#define VERSION "0.8"

#define LOCAL_PORT_OPTION   'a'
#define REMOTE_PORT_OPTION  'b'
#define REMOTE_HOST_OPTION  'c'
#define BIND_ADDRESS_OPTION 'd'
#define BUFFER_SIZE_OPTION  'e'
#define FORK_OPTION         'f'
#define LOG_OPTION          'g'
#define STAY_ALIVE_OPTION   'h'
#define HELP_OPTION         'i'
#define VERSION_OPTION      'j'

#define PATH_SEPARATOR '/'

const char *name;

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

void print_help(void);
void print_helpinfo(void);
void print_usage(void);
void print_version(void);
void print_missing(const char *message);

struct struct_settings {
	unsigned int local_port   : 1;
	unsigned int remote_host  : 1;
	unsigned int remote_port  : 1;
	unsigned int bind_address : 1;
	unsigned int buffer_size  : 1;
	unsigned int fork         : 1;
	unsigned int log          : 1;
	unsigned int stay_alive   : 1;
};

struct struct_options {
	char *local_port;
	char *remote_host;
	char *remote_port;
	char *bind_address;
	unsigned int buffer_size;
};

struct struct_rc {
	int server_socket;
	int client_socket;
	int remote_socket;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	struct sockaddr_in remote_addr;
	struct hostent *remote_host;
};

#endif

