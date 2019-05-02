/*
 * Rasp_TCP.cpp
 *
 *
 *	compilation command
 *	g++ Rasp_main.cpp -lm TCP_connections.o -lgps -o server
 *  g++ Rasp_main.cpp -lm TCP_connections.o -lgps -lpthread -o server
 *	g++ -c TCP_connections.cpp -o TCP_connections.o
 *	g++ -c GPS.cpp -o GPS.o
 *	//Full deployement on the Rasp
 *	g++ Rasp_main.cpp TCP_connections.cpp sensors.cpp rs232.c csv.cpp -lgps -lpthread -o server
 *  //Remote Testing
 *  g++ Rasp_main.cpp TCP_connections.cpp sensors.cpp csv.cpp -lpthread -o server
 *
 */

#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string> // bzero
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//GPS
//#include "libgpsmm.h"


//user created
#include "TCP_connections.hpp"
#include "sensors.hpp"
#include "csv.hpp"

int port =8001;

using namespace std;

//main
int main(int argc, char *argv[]){

	  int server_socket;
		int sensors_Flag=0;

		//initialize GPS client so that the gps is active
		pthread_t sensor_thread;
		pthread_create(&sensor_thread,NULL,client_sensors, &sensors_Flag);

		//Inicializa, must be initialized after gps is initialized
		server_socket = tcp_init_server(port);
		//Cria o thread para acompanhar o client_socket
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, tcp_client_receiver, &server_socket);

		csv_file csv_input;

		csv_input.csv_file = csv_input.csv_open_file("Example_file1.csv");

		char line[1024];
		string str;
		int i = 0;


		fclose(csv_input.csv_file);

		// // uncomment to continue running
		//while(1);

	  return 0;
}
