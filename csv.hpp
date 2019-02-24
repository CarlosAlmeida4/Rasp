

class csv_file{

  public:
	int n_lines;
	FILE* csv_file;

  FILE* csv_open_file(string file_name);
  unsigned int csv_read(void);
  char csv_write(void);
  
};
