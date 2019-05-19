#include "includes.h"

#include "csv.hpp"

//Open the file with the set name
FILE* csv_open_file(string file_name){

  		FILE* fd;
  		fd = fopen(file_name.c_str(),"r+");
      int n_lines;
      char line[1024];
      int check = 0;

      //Check if the file is new (meaning new trip), if yes, create header
      if(fd == NULL){
        cout << "No file found, creating a new one" << endl;
        fd = fopen(file_name.c_str(),"w+");
        fprintf(fd,"ID,Day,Month,Year,Hour,Minute,Second,Latitude,Longitude,Speed\n");
      }
      else{
        while(fscanf(fd,"%s",line) != EOF){
          //printf("%s : %d\n",line,i);
          n_lines++;
        }
        //cout << "read " << n_lines << " line "<< endl;
        rewind(fd);
      }

      return fd;
}

// Read method works, returns the number of lines read
unsigned int csv_read(FILE* csv_file){

  char line[1024];
  int i = 0;

  while(fscanf(csv_file,"%s",line) != EOF){
    //printf("%s : %d\n",line,i);
    i++;
  }

  //rewind(csv_file);
  return i;

}
