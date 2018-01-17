/*
 * Rasp_TCP.cpp
 *
 * Copyright 2018 root <root@kali>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *	compilation command
 *	g++ Rasp_TCP.cpp -lm TCP_connections.o -lgps -o server
 *	g++ -c TCP_connections.cpp -o TCP_connections.o
 */

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
#include "TCP_connections.hpp"
#include "libgpsmm.h"

int port =8001;

using namespace std;

int main(int argc, char *argv[]){

	  int server_socket,client_socket=0;

		//Inicializa
		server_socket = tcp_init_server(port);
		//Cria o thread para acompanhar o client_socket
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, tcp_client_receiver, &server_socket);
		//From where theres no more tcp client stuff

		while(1){



		}

	  return 0;
}
