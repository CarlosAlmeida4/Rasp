#include "includes.h"




//user created
//#include "libgpsmm.h"


using namespace std;


/*
 * We should get libgps_dump_state() from the client library, but
 * scons has a bug; we can't get it to add -lgps to the link line,
 * apparently because it doesn't honor parse_flags on a Program()
 * build of a C++ file.
 */
// void libgps_dump_state(struct gps_data_t *);
//
double latitude(void);
double longitude(void);
double speed(void);
