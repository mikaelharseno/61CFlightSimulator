/*
 * CS61C Summer 2019
 * Name:
 * Login:
 */

#ifndef FLIGHT_STRUCTS_H
#define FLIGHT_STRUCTS_H

#include "timeHM.h"

typedef struct flightSys flightSys_t;
typedef struct airport airport_t;
typedef struct flight flight_t;
typedef struct llx ll;

struct flightSys {
	airport_t *airportFirst;
	airport_t *airportLast;
  // Place the members you think are necessary for the flightSys struct here.
};

struct airport {
	char *name;
	flight_t *flightFirst;
	flight_t *flightLast;
	airport_t *next;
  // Place the members you think are necessary for the airport struct here.
};

struct flight {
	airport_t *dest;
	timeHM_t *dep;
	timeHM_t *arr;
	int fcost;
	flight_t *next;
	flight_t *prev;
	airport_t *source;
  // Place the members you think are necessary for the flight struct here.
};

struct llx {
	flight_t *flight;
	timeHM_t *arr;
	int fcost;
	ll *next;
	ll *prev;
};

#endif

