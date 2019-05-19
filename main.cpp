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
*  To test without TCP TCP_connections
*  g++ Rasp_main.cpp sensors.cpp csv.cpp -lpthread -o server
*	cd /
* 	cd mnt/c/Users/Carlos\ Almeida/Documents/Workspace
*/

#include "includes.h"

//GPS
//#include "libgpsmm.h"
#include "GPS_sensor.hpp"
#include "csv.hpp"

using namespace std;

string file_writer(csv_file_t csv_file, struct tm tm){

    ostringstream csv_tm_line,gps_line, complete_str;
		csv_tm_line << csv_file.n_lines << "," << tm.tm_mday << "," << tm.tm_mon << "," << tm.tm_year+1900 << "," << tm.tm_hour << "," << tm.tm_min<< "," << tm.tm_sec;
		gps_line << latitude() << "," << longitude() << "," << speed();
		complete_str <<  csv_tm_line.str() << "," << gps_line.str() << "\n";

		return complete_str.str();
}


void periodic_save_gps(size_t timer_id, void * user_data){

  static csv_file_t csv_file = *(csv_file_t*) user_data;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  //fprintf(csv_file.csv_fd,"ID,Day,Month,Year,Hour,Minute,Second,Latitude,Longitude,Speed\n");
	csv_file.n_lines++;
	string file_line = file_writer(csv_file,tm);
	fprintf(csv_file.csv_fd,file_line.c_str());
	//Debugging output
	//cout << csv_file.n_lines << endl;

}

//main
int main(int argc, char *argv[]){

	//variables
	csv_file_t csv_file;


	/***************************************************************************/
	/*			Initializations																										*/
	/*													Initializations						   									*/
	/*																			Initializations	  								*/
	/***************************************************************************/
	//Check if got a input name
	if(argv[1] == NULL){
		cout<< "No input files, process will now shutdown" << endl;
		exit(10);
	}
	//Initialize the timer library
	initialize_timer();

	csv_file.file_name = argv[1];
	csv_file.csv_fd = csv_open_file(csv_file.file_name);
	csv_file.n_lines = csv_read(csv_file.csv_fd);
	cout << "read " << csv_file.n_lines << " lines "<< endl;

	//save gps information into the csv file every half a second
	size_t timer_id = start_timer(500,periodic_save_gps,TIMER_PERIODIC,&csv_file);

	sleep(1000);

	//when finishing one must kill all threads, a clean code is a good code
	stop_timer(timer_id);
	finalize();
	fclose(csv_file.csv_fd);
  // uncomment to continue running
	//while(1);

	return 0;
}
