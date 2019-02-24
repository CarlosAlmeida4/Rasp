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


FILE* csv_file::csv_open_file(string file_name){

  		FILE* fd;
  		fd = fopen(file_name.c_str(),"r+");


  		if(fd == NULL){
  			cout << "Error opening csv file" << endl;
  		}

      return fd;
}

unsigned int csv_file::csv_read(void){

  char line[1024];
  int i = 0;

  while(fscanf(csv_file,"%s",line) != EOF){
    //printf("%s : %d\n",line,i);
    i++;
  }
  n_lines = i;
  return i;
}

char csv_file::csv_write(void){

}
