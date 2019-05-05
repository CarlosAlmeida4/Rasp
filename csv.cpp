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

#include "TCP_connections.hpp"
#include "sensors.hpp"
#include "csv.hpp"

//Open the file with the set name
FILE* csv_file::csv_open_file(string file_name){

  		FILE* fd;
  		fd = fopen(file_name.c_str(),"r+");

      char line[1024];
      int check = 0;

      //Check if the file is new (meaning new trip), if yes, create header
      if(fd == NULL){

        cout << "No file found, creating a new one" << endl;
        fd = fopen(file_name.c_str(),"w+");
        csv_file::new_file = true;
        fprintf(fd,"ID,Day,Month,Year,Hour,Minute,Second,Latitude,Longitude,Speed\n");
        csv_file::curr_line = 1;

      }
      else{

        while(fscanf(fd,"%s",line) != EOF){
          //printf("%s : %d\n",line,i);
          check++;
        }

        csv_file::new_file = false;
        csv_file::curr_line = check;

      }

      return fd;
}

// Read method works, returns the number of lines read
unsigned int csv_file::csv_read(void){

  char line[1024];
  int i = 0;

  while(fscanf(csv_file,"%s",line) != EOF){
    //printf("%s : %d\n",line,i);
    i++;
  }

  rewind(csv_file);
  n_lines = i;
  return i;


}

char csv_file::csv_write(int ID){



}
