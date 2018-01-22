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
 *  g++ Rasp_TCP.cpp -lm TCP_connections.o -lgps -lpthread -o server
 *	g++ -c TCP_connections.cpp -o TCP_connections.o
 *	g++ -c GPS.cpp -o GPS.o
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

//GPS
#include "libgpsmm.h"


//user created
#include "TCP_connections.hpp"
#include "GPS.hpp"

int port =8001;

using namespace std;


//main
int main(int argc, char *argv[]){

	  int server_socket;

		//Inicializa
		server_socket = tcp_init_server(port);
		//Cria o thread para acompanhar o client_socket
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, tcp_client_receiver, &server_socket);
		//From where theres no more tcp client stuff

		//Init da estrutura do gps
		gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

		if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        cerr << "No GPSD running.\n";
        return 1;
    }

		//Estrutura que guarda os dados gps
		struct gps_data_t* newdata;

		//Check if has fix
		if (!gps_rec.waiting(5000000))
			continue;

		//Check if it has data
		if ((newdata = gps_rec.read()) == NULL) {
				cerr << "Read error.\n";
				return 1;
		}

		else {
				libgps_dump_state(newdata);
		}




		while(1){

			if(newdata->set & LATLON_SET){
				cout << "Latitude: " << newdata->fix.latitude << endl;
				cout << "Longitude: "<< newdata->fix.longitude << endl;
			}

		}

	  return 0;
}
