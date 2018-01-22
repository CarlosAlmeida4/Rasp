
#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//user created
#include "GPS.hpp"

void* client_handler(void*);

int tcp_init_server(int);

int tcp_new_client(int);

void* tcp_client_receiver(void*);
