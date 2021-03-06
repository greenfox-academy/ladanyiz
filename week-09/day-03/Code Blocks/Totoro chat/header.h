#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdint.h>

#define MESSAGE_PORT    7777
#define DISCO_PORT      7007
#define BRDCST_PORT     12345
#define BRDCST_IP       inet_addr("255.255.255.255")

typedef struct {
    char name[20];
    char ip[20];
    uint16_t port;
} user_t;


user_t user_list[50];

uint8_t list_size;

FILE *file;

char my_name[20];


void handle_error(const char *error_string);

void wsa_init();

void start_screen();

void message_listen();

void disco_listen();

void broadcast_listen();

void broadcast();

uint8_t tcp_send(char *server_ip, uint16_t server_port, char *msg);

void list_users();

uint8_t send_message();

uint8_t save_to_file(char *path);

uint8_t read_from_file(char *path);

#endif // HEADER_H_INCLUDED
