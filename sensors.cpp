#include <iostream> // cout
#include <fstream>
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pthread.h>
//#include <postgresql/libpq-fe.h>//Postgres library
#include <signal.h>

//user created
// #include "libgpsmm.h"
// #include "rs232.h" // https://www.teuniz.net/RS-232/

#define BUF_SIZE 128

using namespace std;

//PGconn* conn =NULL;//declaração do PGcon
// // uncomment from this point for rasp with sensor
// gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);
// volatile struct gps_data_t* data/*=gps_rec.read()*/;
//
bool gps_tracker = false;
//
// /*
//  * We should get libgps_dump_state() from the client library, but
//  * scons has a bug; we can't get it to add -lgps to the link line,
//  * apparently because it doesn't honor parse_flags on a Program()
//  * build of a C++ file.
//  */
// void libgps_dump_state(struct gps_data_t *collect)
// {
//     /* no need to dump the entire state, this is a sanity check */
// #ifndef USE_QT
//     /* will fail on a 32-bit macine */
//     (void)fprintf(stdout, "flags: (0x%04x) %s\n",
// 		  (unsigned int)collect->set, gps_maskdump(collect->set));
// #endif
//     if (collect->set & ONLINE_SET)
// 	(void)fprintf(stdout, "ONLINE: %lf\n", collect->online);
//     if (collect->set & TIME_SET)
// 	(void)fprintf(stdout, "TIME: %lf\n", collect->fix.time);
//     if (collect->set & LATLON_SET)
// 	(void)fprintf(stdout, "LATLON: lat/lon: %lf %lf\n",
// 		      collect->fix.latitude, collect->fix.longitude);
//     if (collect->set & ALTITUDE_SET)
// 	(void)fprintf(stdout, "ALTITUDE: altitude: %lf  U: climb: %lf\n",
// 		      collect->fix.altitude, collect->fix.climb);
//     if (collect->set & SPEED_SET)
// 	(void)fprintf(stdout, "SPEED: %lf\n", collect->fix.speed);
//     if (collect->set & TRACK_SET)
// 	(void)fprintf(stdout, "TRACK: track: %lf\n", collect->fix.track);
//     if (collect->set & CLIMB_SET)
// 	(void)fprintf(stdout, "CLIMB: climb: %lf\n", collect->fix.climb);
//     if (collect->set & STATUS_SET)
// 	(void)fprintf(stdout, "STATUS: status: %d\n", collect->status);
//     if (collect->set & MODE_SET)
// 	(void)fprintf(stdout, "MODE: mode: %d\n", collect->fix.mode);
//     if (collect->set & DOP_SET)
// 	(void)fprintf(stdout,
// 		      "DOP: satellites %d, pdop=%lf, hdop=%lf, vdop=%lf\n",
// 		      collect->satellites_used, collect->dop.pdop,
// 		      collect->dop.hdop, collect->dop.vdop);
//     if (collect->set & VERSION_SET)
// 	(void)fprintf(stdout, "VERSION: release=%s rev=%s proto=%d.%d\n",
// 		      collect->version.release,
// 		      collect->version.rev,
// 		      collect->version.proto_major,
// 		      collect->version.proto_minor);
//     if (collect->set & POLICY_SET)
// 	(void)fprintf(stdout,
// 		      "POLICY: watcher=%s nmea=%s raw=%d scaled=%s timing=%s, devpath=%s\n",
// 		      collect->policy.watcher ? "true" : "false",
// 		      collect->policy.nmea ? "true" : "false",
// 		      collect->policy.raw,
// 		      collect->policy.scaled ? "true" : "false",
// 		      collect->policy.timing ? "true" : "false",
// 		      collect->policy.devpath);
//     if (collect->set & SATELLITE_SET) {
// 	int i;
//
// 	(void)fprintf(stdout, "SKY: satellites in view: %d\n",
// 		      collect->satellites_visible);
// 	for (i = 0; i < collect->satellites_visible; i++) {
// 	    (void)fprintf(stdout, "    %2.2d: %2.2d %3.3d %3.0f %c\n",
// 			  collect->PRN[i], collect->elevation[i],
// 			  collect->azimuth[i], collect->ss[i],
// 			  collect->used[i] ? 'Y' : 'N');
// 	}
//     }
//     if (collect->set & DEVICE_SET)
// 	(void)fprintf(stdout, "DEVICE: Device is '%s', driver is '%s'\n",
// 		      collect->dev.path, collect->dev.driver);
// #ifdef OLDSTYLE_ENABLE
//     if (collect->set & DEVICEID_SET)
// 	(void)fprintf(stdout, "GPSD ID is %s\n", collect->dev.subtype);
// #endif /* OLDSTYLE_ENABLE */
//     if (collect->set & DEVICELIST_SET) {
// 	int i;
// 	(void)fprintf(stdout, "DEVICELIST:%d devices:\n",
// 		      collect->devices.ndevices);
// 	for (i = 0; i < collect->devices.ndevices; i++) {
// 	    (void)fprintf(stdout, "%d: path='%s' driver='%s'\n",
// 			  collect->devices.ndevices,
// 			  collect->devices.list[i].path,
// 			  collect->devices   .list[i].driver);
// 	}
//     }
// }
//
// return latitude info
double latitude(void){
  return 0;
  //return data->fix.latitude;
}
//return longitude info
double longitude(void){
  return 0;
  //return data->fix.longitude;
}
// return speed info
double speed(void){
  return 0;
  //return data->fix.speed;
}



//
// //Obtain values from the series connection to the arduino
// int get_series_value(string line){
//
//   //Variables needed for the serial connection
//   int cport_nr = 24; // /dev/ttyACMA0
//   int bdrate = 9600; // Baudrate
//   char mode[] = {'8','N','1',0}; //8 data bits, no parity, 1 stop bit
//   char str_send[2][BUF_SIZE]; // send data buffer
//   unsigned char str_recv[BUF_SIZE]; //recv data buffer
//   strcpy(str_send[0], "165");
//   strcpy(str_send[1], "1000");
//
//   int i = 0;
//
//   pthread_mutex_t mutex;
//   pthread_mutex_init(&mutex,NULL);
//
//   if(RS232_OpenComport(cport_nr, bdrate, mode))
//   {
//     printf("Can not open comport\n");
//   }
//
//   usleep(2000000);  /* waits 2000ms for stable condition */
//
//   pthread_mutex_lock(&mutex);
//   RS232_cputs(cport_nr, str_send[i]); // sends string on serial
//   printf("Sent to Arduino: '%s'\n", str_send[i]);
//   pthread_mutex_unlock(&mutex);
//   usleep(1000);  /* waits for reply 1000ms */
//   pthread_mutex_lock(&mutex);
//   int n = RS232_PollComport(cport_nr, str_recv, (int)BUF_SIZE);
//   pthread_mutex_unlock(&mutex);
//
//   if(n > 0){
//     str_recv[n] = 0;   /* always put a "null" at the end of a string! */
//     printf("Received %i bytes: '%s'\n", n, (char *)str_recv);
//   }
//    i++;
//    i %= 2;
//    usleep(500000);  /* sleep for 5 Second */
//
//    return 0;
// }


void periodic_save_position(int signum)
{

	static int count = 0;
	//printf(" periodic task in C++ timer %d \n", count);

	if (count == 3) {

		count = 0;
	}

	count++;
}

//Sensors thread routine, also updates the gps values
void* client_sensors(void* args){

		int client = *(int*) args;



    // if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
    //     cerr << "No GPSD running.\n";
    //
    // }

    //not used I beelieve
    //struct gps_data_t* newdata;

	// Thank god for stack overflow
	// How to do timed tasks in C++ in linux
	// https://stackoverflow.com/questions/28874139/how-write-program-for-periodic-task-in-c
	struct sigaction sa;
	struct itimerval timer;

	/* Install periodic_task  as the signal handler for SIGVTALRM. */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &periodic_save_position;
	sigaction(SIGVTALRM, &sa, NULL);

	/* Configure the timer to expire after 250 msec... */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 250000;

	/* ... and every 250 msec after that. */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 250000;

	/* Start a virtual timer. It counts down whenever this process is    executing. */
	setitimer(ITIMER_VIRTUAL, &timer, NULL);


    while(1){
      //Check if has fix
      // if (!gps_rec.waiting(5000000))
      //   continue;
      //
      //   //Check if it has data
      // if ((data = gps_rec.read()) == NULL) {
      //   cerr << "Read error.\n";
      //   break;
      // }
      //check if gps_tracker is on or off
      // if (gps_tracker) {
      //   init_DB();
      // }


    }
}
