#include "includes.h"

using namespace std;

typedef struct {
  FILE* csv_fd;
  unsigned int n_lines;
  string file_name;
  bool gps_online;
}csv_file_t;



FILE* csv_open_file(string file_name);
//Read method works, returns the number of lines read
unsigned int csv_read(FILE* csv_file);
