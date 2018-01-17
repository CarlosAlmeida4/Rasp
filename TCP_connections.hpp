
#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>



int tcp_init_server(int);

int tcp_new_client(int);

void tcp_client_receiver(int);

void client_handler(int);
