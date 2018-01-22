/*
 * Rasp_TCP.cpp
 *
 * Copyright 2018 root <root@kali>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *	compilation command
 *	g++ Rasp_TCP.cpp -lm TCP_connections.o -lgps -o server
 *  g++ Rasp_TCP.cpp -lm TCP_connections.o -lgps -lpthread -o server
 *	g++ -c TCP_connections.cpp -o TCP_connections.o
 */

#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//user created
#include "TCP_connections.hpp"
#include "libgpsmm.h"

int port =8001;

using namespace std;

/*
 * We should get libgps_dump_state() from the client library, but
 * scons has a bug; we can't get it to add -lgps to the link line,
 * apparently because it doesn't honor parse_flags on a Program()
 * build of a C++ file.
 */
static void libgps_dump_state(struct gps_data_t *collect)
{
    /* no need to dump the entire state, this is a sanity check */
#ifndef USE_QT
    /* will fail on a 32-bit macine */
    (void)fprintf(stdout, "flags: (0x%04x) %s\n",
		  (unsigned int)collect->set, gps_maskdump(collect->set));
#endif
    if (collect->set & ONLINE_SET)
	(void)fprintf(stdout, "ONLINE: %lf\n", collect->online);
    if (collect->set & TIME_SET)
	(void)fprintf(stdout, "TIME: %lf\n", collect->fix.time);
    if (collect->set & LATLON_SET)
	(void)fprintf(stdout, "LATLON: lat/lon: %lf %lf\n",
		      collect->fix.latitude, collect->fix.longitude);
    if (collect->set & ALTITUDE_SET)
	(void)fprintf(stdout, "ALTITUDE: altitude: %lf  U: climb: %lf\n",
		      collect->fix.altitude, collect->fix.climb);
    if (collect->set & SPEED_SET)
	(void)fprintf(stdout, "SPEED: %lf\n", collect->fix.speed);
    if (collect->set & TRACK_SET)
	(void)fprintf(stdout, "TRACK: track: %lf\n", collect->fix.track);
    if (collect->set & CLIMB_SET)
	(void)fprintf(stdout, "CLIMB: climb: %lf\n", collect->fix.climb);
    if (collect->set & STATUS_SET)
	(void)fprintf(stdout, "STATUS: status: %d\n", collect->status);
    if (collect->set & MODE_SET)
	(void)fprintf(stdout, "MODE: mode: %d\n", collect->fix.mode);
    if (collect->set & DOP_SET)
	(void)fprintf(stdout,
		      "DOP: satellites %d, pdop=%lf, hdop=%lf, vdop=%lf\n",
		      collect->satellites_used, collect->dop.pdop,
		      collect->dop.hdop, collect->dop.vdop);
    if (collect->set & VERSION_SET)
	(void)fprintf(stdout, "VERSION: release=%s rev=%s proto=%d.%d\n",
		      collect->version.release,
		      collect->version.rev,
		      collect->version.proto_major,
		      collect->version.proto_minor);
    if (collect->set & POLICY_SET)
	(void)fprintf(stdout,
		      "POLICY: watcher=%s nmea=%s raw=%d scaled=%s timing=%s, devpath=%s\n",
		      collect->policy.watcher ? "true" : "false",
		      collect->policy.nmea ? "true" : "false",
		      collect->policy.raw,
		      collect->policy.scaled ? "true" : "false",
		      collect->policy.timing ? "true" : "false",
		      collect->policy.devpath);
    if (collect->set & SATELLITE_SET) {
	int i;

	(void)fprintf(stdout, "SKY: satellites in view: %d\n",
		      collect->satellites_visible);
	for (i = 0; i < collect->satellites_visible; i++) {
	    (void)fprintf(stdout, "    %2.2d: %2.2d %3.3d %3.0f %c\n",
			  collect->PRN[i], collect->elevation[i],
			  collect->azimuth[i], collect->ss[i],
			  collect->used[i] ? 'Y' : 'N');
	}
    }
    if (collect->set & DEVICE_SET)
	(void)fprintf(stdout, "DEVICE: Device is '%s', driver is '%s'\n",
		      collect->dev.path, collect->dev.driver);
#ifdef OLDSTYLE_ENABLE
    if (collect->set & DEVICEID_SET)
	(void)fprintf(stdout, "GPSD ID is %s\n", collect->dev.subtype);
#endif /* OLDSTYLE_ENABLE */
    if (collect->set & DEVICELIST_SET) {
	int i;
	(void)fprintf(stdout, "DEVICELIST:%d devices:\n",
		      collect->devices.ndevices);
	for (i = 0; i < collect->devices.ndevices; i++) {
	    (void)fprintf(stdout, "%d: path='%s' driver='%s'\n",
			  collect->devices.ndevices,
			  collect->devices.list[i].path,
			  collect->devices.list[i].driver);
	}
    }
}



//main
int main(int argc, char *argv[]){

	  int server_socket,client_socket=0;

		//Inicializa
		server_socket = tcp_init_server(port);
		//Cria o thread para acompanhar o client_socket
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, tcp_client_receiver, &server_socket);
		//From where theres no more tcp client stuff

		gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

		if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        cerr << "No GPSD running.\n";
        return 1;
    }

		while(1){

			struct gps_data_t* newdata;

			if (!gps_rec.waiting(5000000))
				continue;

			if ((newdata = gps_rec.read()) == NULL) {
					cerr << "Read error.\n";
					return 1;
			}
			else {
					libgps_dump_state(newdata);
			}


		}

	  return 0;
}
