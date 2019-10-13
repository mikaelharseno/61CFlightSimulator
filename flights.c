/*
 * CS61C Summer 2019
 * Name:
 * Login:
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flights.h"
#include "flight_structs.h"
#include "timeHM.h"

/*
 *  This should be called if memory allocation failed.
 */
static void allocation_failed(void) {
  fprintf(stderr, "Out of memory.\n");
  exit(EXIT_FAILURE);
}


/* DON'T NEED INPUT TESTING. SEGMENTATION FAULT DONE.
 *  Creates and initializes a flight system, which stores the flight schedules of several airports.
 *  Returns a pointer to the system created.
 */
flightSys_t* createSystem(void) {
	flightSys_t* newflightsys = (flightSys_t*) malloc(sizeof(flightSys_t));
	if (newflightsys == NULL) {
		allocation_failed();
	}
	newflightsys->airportFirst = NULL;
	newflightsys->airportLast = NULL;
	return newflightsys;	
}


/*
 *   Given a destination airport, departure and arrival times, and a cost, return a pointer to new flight_t.
 *
 *   Note that this pointer must be available to use even after this function returns.
 *   (What does this mean in terms of how this pointer should be instantiated?)
 *   Additionally you CANNOT assume that the `departure` and `arrival` pointers will persist after this function completes.
 *   (What does this mean about copying dep and arr?)
 */

flight_t* createFlight(airport_t* destination, timeHM_t* departure, timeHM_t* arrival, int cost) {
	if ((cost < 0)||(destination == NULL) || (departure == NULL) || (arrival == NULL) || isAfter(departure, arrival)) {
		return NULL;
	}
	timeHM_t *depcopy, *arrcopy;
	depcopy = (timeHM_t*) malloc(sizeof(timeHM_t));
	memcpy(depcopy, departure, sizeof(timeHM_t));
	arrcopy = (timeHM_t*) malloc(sizeof(timeHM_t));
	memcpy(arrcopy, arrival, sizeof(timeHM_t));
	flight_t *newflight = (flight_t*) malloc(sizeof(flight_t));
	newflight->fcost = cost;
	newflight->dep = depcopy;
	newflight->arr = arrcopy;
	newflight->next = NULL;
	newflight->prev = NULL;
	newflight->dest = destination;
	newflight->source = NULL;
	return newflight;
}

/*
 *  Frees all memory associated with this system; that's all memory you dynamically allocated in your code.
 */
void deleteSystem(flightSys_t* system) {
	if (system == NULL) {
		return NULL;
	}
	airport_t *cura = system->airportFirst, *nexta;
	flight_t *curf, *nextf;
	while (cura != NULL) {
		if (cura->flightFirst != NULL) {
			curf = cura->flightFirst;
			while (curf != NULL) {
				nextf = curf->next;
				deleteFlight(curf);
				curf = nextf;
			}
		}
		nexta = cura->next;
		free(cura->name);
		free(cura);
		cura = nexta;
	}
	free(system);
}


/*
 *  Frees all memory allocated for a single flight. You may or may not decide
 *  to use this in delete system but you must implement it.
 */
void deleteFlight(flight_t* flight) {
	if (flight == NULL) {
		return NULL;
	}
	flight_t *prevf, *nextf;
	airport_t *sourcef;
	sourcef = flight->source;
	prevf = flight->prev;
	nextf = flight->next;
	free(flight->dep);
	free(flight->arr);
	free(flight);
	if (sourcef != NULL) {
		if ((prevf == NULL) && (nextf == NULL)) {
			sourcef->flightFirst = NULL;
			sourcef->flightLast = NULL;
		} else {
			if (prevf == NULL) {
				sourcef->flightFirst = nextf;
			} else {
				prevf->next = nextf;
			}
			if (nextf == NULL) {
				sourcef->flightLast = prevf;
			} else {
				nextf->prev = prevf;
			}

		}
	}
}


/*
 *  Adds a airport with the given name to the system. You must copy the string and store it.
 *  Do not store `name` (the pointer) as the contents it point to may change.
 */
void addAirport(flightSys_t* system, char* name) {
	if ((system == NULL) || (name == NULL)) {
		return;
	}
	//Limit name string to 10000
	char* checker = name;
	int count = 0;
	while (count < 100000) {
		if (*checker == '\0') {
			break;
		}
		checker = checker + 1;
		count++;
	}
	if (count == 100000) {
		return;
	}
	airport_t *newairport = (airport_t*) malloc(sizeof(airport_t));
	unsigned int stringlength = strlen(name);
	char *namecopy = (char*) malloc(stringlength * sizeof(char) + 1);
	strcpy(namecopy, name);
	newairport->name = namecopy; 
	newairport->next = NULL;
	newairport->flightFirst = NULL;
	newairport->flightLast = NULL;
	if (system->airportLast != NULL) {
		system->airportLast->next = newairport;
	}
	system->airportLast = newairport;
	if (system->airportFirst == NULL) {
		system->airportFirst = newairport;
	}
}


/*
 *  Returns a pointer to the airport with the given name.
 *  If the airport doesn't exist, return NULL.
 */
airport_t* getAirport(flightSys_t* system, char* name) {
	if ((system == NULL) || (name == NULL)) {
		return NULL;
	}
	airport_t* curair = system->airportFirst;
	while (curair != NULL) {
		if (strcmp(curair->name,name) == 0) { 
			return curair;
		}
		curair = curair->next;
	}
	return NULL;
}


/*
 *  Print each airport name in the order they were added through addAirport, one on each line.
 *  Make sure to end with a new line. You should compare your output with the correct output
 *  in `flights.out` to make sure your formatting is correct.
 */
void printAirports(flightSys_t* system) {
	if (system == NULL) {
		return NULL;
	}
	airport_t *cura = system->airportFirst;
	while (cura != NULL) {
		printf("%s\n", cura->name);
		cura = cura->next;
	}
}


/* OK
 *  Adds a flight to source's schedule, stating a flight will leave to destination at departure time and arrive at arrival time.
 */
void addFlight(airport_t* source, airport_t* destination, timeHM_t* departure, timeHM_t* arrival, int cost) {
	if ((cost < 0) || (source == NULL) || (destination == NULL) || (departure == NULL) || (arrival == NULL) || isAfter(departure, arrival)) {
		return NULL;
	}
	flight_t *newflight = createFlight(destination, departure, arrival, cost);
	newflight->source = source;
	if (source->flightLast == NULL) {
		source->flightFirst = newflight;
		source->flightLast = newflight;
	} else {
		source->flightLast->next = newflight;
		newflight->prev = source->flightLast;
		source->flightLast = newflight;
	}
}


/*
 *  Prints the schedule of flights of the given airport.
 *
 *  Prints the airport name on the first line, then prints a schedule entry on each
 *  line that follows, with the format: "destination_name departure_time arrival_time $cost_of_flight".
 *
 *  You should use `printTime()` (look in `timeHM.h`) to print times, and the order should be the same as
 *  the order they were added in through addFlight. Make sure to end with a new line.
 *  You should compare your output with the correct output in flights.out to make sure your formatting is correct.
 */
void printSchedule(airport_t* airport) {
	if (airport == NULL) {
		return NULL;
	}
	printf("%s\n", airport->name);
	flight_t *curf = airport->flightFirst;
	while (curf != NULL) {
		printf("%s ", curf->dest->name);
		printTime(curf->dep);
		printf(" ");
		printTime(curf->arr);
		printf(" $%d\n",curf->fcost);
		curf = curf->next;
	}
}


/*
 *   Given a source and destination airport, and the time now, finds the next flight to take based on the following rules:
 *   1) Finds the earliest arriving flight from source to destination that departs after now.
 *   2) If there are multiple earliest flights, take the one that costs the least.
 *
 *   If a flight is found, you should store the flight's departure time, arrival time, and cost in the `departure`, `arrival`,
 *   and `cost` params and return true. Otherwise, return false.
 *
 *   Please use the functions `isAfter()` and `isEqual()` from `timeHM.h` when comparing two timeHM_t objects and compare
 *   the airport names to compare airports, not the pointers.
 *   Step 1: Filter destination
 *   Step 2: Filter such that all departure times are above now (remove ones below)
 *   Step 3: Get smallest arrival time by traversing list
 *   Step 4: Remove everything that do not have smallest arrival time
 *   Step 5: Compare cost and return first cheapest flight
 */
bool getNextFlight(airport_t* source, airport_t* destination, timeHM_t* now, timeHM_t* departure, timeHM_t* arrival,
                   int* cost) {
	if ((source == NULL) || (destination == NULL) || (departure == NULL) || (arrival == NULL) || (now == NULL) || (cost == NULL)) {
		return NULL;
	}	
	flight_t *curf = source->flightFirst;
	ll *startll = NULL;
	ll *endll = NULL;
	ll *newll;
	timeHM_t *smallest;
	int count = 0;
	while(curf != NULL) {
		if ((isAfter(curf->dep,now)) && (strcmp(curf->dest->name,destination->name) == 0)) {
			newll = (ll*) malloc(sizeof(ll));
			newll->fcost = curf->fcost;
			newll->arr = curf->arr;
			newll->flight = curf;
			newll->next = NULL;
			newll->prev = NULL;
			if (startll == NULL) {
				startll = newll;
				endll = newll;
			} else {
				endll->next = newll;
				newll->prev = endll;
				endll = newll;
			}
			if (count == 0) {
				smallest = curf->arr;
				count++;
			}
			if (isAfter(smallest, curf->arr)) {
				smallest = curf->arr;
			}
		}
		curf = curf->next;
	}
	ll *curll = startll;
	ll *nextll;
	while (curll != NULL) {
		nextll = curll->next;
		if (isAfter(curll->arr, smallest)) {
			if (curll->prev == NULL) {
				startll = curll->next;
			} else {
				curll->prev->next = curll->next;
			}
			if (curll->next == NULL) {
				endll = curll->prev;
			} else {
				curll->next->prev = curll->prev;
			}
			free(curll);
		}
		curll = nextll;
	}
	//Now we find the cheapest flight
	int cheapest;
	count = 0;
	flight_t *bestflight = NULL;
	curll = startll;
	while (curll != NULL) {
		nextll = curll->next;
		if (count == 0) {
			cheapest = curll->fcost;
			bestflight = curll->flight;
			count++;
		}
		if (curll->fcost < cheapest) {
			cheapest = curll->fcost;
			bestflight = curll->flight;
		}
		free(curll);
		curll = nextll;
	}
	if (bestflight == NULL) {
		return false;
	} else {
		*departure = *(bestflight->dep);
		*arrival = *(bestflight->arr);
		*cost = bestflight->fcost;
		return true;
	}
	return false;
}

/*
 *  Given a list of flight_t pointers (`flight_list`) and a list of destination airport names (`airport_name_list`),
 *  first confirm that it is indeed possible to take these sequences of flights, (i.e. be sure that the i+1th flight departs
 *  after or at the same time as the ith flight arrives) (HINT: use the `isAfter()` and `isEqual()` functions).
 *  Then confirm that the list of destination airport names match the actual destination airport names of the provided flight_t structs.
 *
 *  `size` tells you the number of flights and destination airport names to consider. Be sure to extensively test for errors.
 *  As one example, if you encounter NULL's for any values that you might expect to be non-NULL return -1, but test for other possible errors too.
 *
 *  Return from this function the total cost of taking these sequence of flights.
 *  If it is impossible to take these sequence of flights,
 *  if the list of destination airport names doesn't match the actual destination airport names provided in the flight_t struct's,
 *  or if you run into any errors mentioned previously or any other errors, return -1.
 */
int validateFlightPath(flight_t** flight_list, char** airport_name_list, int size) {
	if ((flight_list == NULL) || (airport_name_list == NULL) || (size < 0)) {
		return -1;
	}
	int i;
	for (i = 0; i < (size - 1); i++) {
		if (isAfter((*(flight_list+i))->arr,(*(flight_list+(i+1)))->dep)) {
			return -1;
		}
	}
	int total = 0;
	for (i = 0; i < size; i++) {
		if (strcmp((*(flight_list+i))->dest->name,(*(airport_name_list+i))) != 0) {
			return -1;
		}
		total = total + (*(flight_list+i))->fcost;
	}
	return total;
}

