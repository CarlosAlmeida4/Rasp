

class csv_file{

  public:
  bool new_file = false;
	int n_lines;
  int curr_line = 0;
	FILE* csv_file;

  FILE* csv_open_file(string file_name);
  //Read method works, returns the number of lines read
  unsigned int csv_read(void);
  char csv_write(string line);

};
