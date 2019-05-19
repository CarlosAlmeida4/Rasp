/*m_Timer*/
//https://qnaplus.com/implement-periodic-timer-linux/

#ifndef TIME_H
#define TIME_H
#include <stdint.h>
#include <string.h>
#include <sys/timerfd.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream> // cout
#include <sstream> // cout

typedef enum{
  TIMER_SINGLE_SHOT = 0, /*Periodic TImer*/
  TIMER_PERIODIC /*Single Shot timer*/
} t_timer;

typedef void (*time_handler)(size_t timer_id,void *user_data);

/*
*
* Initializes the library
* should be called only once
*
*/
int initialize_timer();

/*
Creates a Timer, returns the timer id to be used in stop_timer()
Interval : timeout interval in milliseconds
handler : callback function
type: specifies if single shot or periodic
user_data: data that the user can input, using the void* pointer
given by pthread
*/
size_t start_timer(unsigned int interval, time_handler handler, t_timer type, void * user_data);

/*
stops a specific timer with the timer_id
*/
void stop_timer(size_t timer_id);

/*
he function should be called when the
timer module is no longer required.
It stops (and deletes) all running timers.
*/
void finalize();


#endif
