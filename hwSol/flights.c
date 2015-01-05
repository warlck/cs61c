#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flights.h"
#include "timeHM.h"

typedef struct entry {
    airport_t *destination;
    timeHM_t departure, arrival;
    int cost;
    struct entry *next;
} entry;

/* Deletes A and all heap memory reachable from A. */
void delete_airport(airport_t *a);

struct flightSys {
    airport_t **airports;
    unsigned n;
};

struct airport {
    char *name;
    entry *sched;
};

/*
   This should be called if memory allocation failed.
 */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(EXIT_FAILURE);
}


/*
   Creates and initializes a flight system, which stores the flight schedules of several airports.
   Returns a pointer to the system created.
 */
flightSys_t* createSystem() {
    // Replace this line with your code
    flightSys_t *f = calloc(1, sizeof(flightSys_t));
    if (!f) {
	allocation_failed();
    }
    return f;
}


/*
   Frees all memory associated with this system; that's all memory you dynamically allocated in your code.
 */
void deleteSystem(flightSys_t* s) {
    // Replace this line with your code
    for (int i = 0; i < s->n; i += 1) {
	delete_airport(s->airports[i]);
    }
    free(s->airports);
    free(s);
}


/*
 * Adds a airport with the given name to the system. You must copy the string and store it.
 * Do not store "name" (the pointer) as the contents it point to may change.
 */
void addAirport(flightSys_t* s, char* name) {
    // Replace this line with your code
    /* Too lazy to implement amortized doubling here. */
    s->n += 1;
    s->airports = realloc(s->airports, s->n * sizeof(airport_t *));
    airport_t *port = calloc(1, sizeof(airport_t));
    char *new_name = malloc((strlen(name) + 1) * sizeof(char));
    if (!s->airports || !port || !new_name)
	allocation_failed();
    strcpy(new_name, name);
    port->name = new_name;
    s->airports[s->n - 1] = port;
}


/*
 * Returns a pointer to the airport with the given name.
 * If the airport doesn't exist, return NULL.
 */
airport_t* getAirport(flightSys_t* s, char* name) {
    // Replace this line with your code
    for (int i = 0; i < s->n; i += 1) {
	if (strcmp(s->airports[i]->name, name) == 0)
	    return s->airports[i];
    }
    return NULL;
}


/*
 * Print each airport name in the order they were added through addAirport, one on each line.
 * Make sure to end with a new line. You should compare your output with the correct output
 * in flights.out to make sure your formatting is correct.
 */
void printAirports(flightSys_t* s) {
    // Replace this line with your code
    for (int i = 0; i < s->n; i += 1) {
	printf("%s\n", s->airports[i]->name);
    }
}

/*
   Adds a flight to src's schedule, stating a flight will leave to dst at departure time and arrive at arrival time.
 */
void addFlight(airport_t* src, airport_t* dst, timeHM_t* departure, timeHM_t* arrival, int cost) {
    // Replace this line with your code
    entry *e = malloc(sizeof(entry));
    if (!e) {
	allocation_failed();
    }
    e->destination = dst;
    e->departure = *departure;
    e->arrival = *arrival;
    e->cost = cost;
    e->next = NULL;
    if (!src->sched) {
	src->sched = e;
    } else {
	entry *tmp = src->sched;
	while (tmp->next) {
	    tmp = tmp->next;
	}
	tmp->next = e;
    }
}


/*
   Prints the schedule of flights of the given airport.

   Prints the airport name on the first line, then prints a schedule entry on each
   line that follows, with the format: "destination_name departure_time arrival_time $cost_of_flight".

   You should use printTime (look in timeHM.h) to print times, and the order should be the same as
   the order they were added in through addFlight. Make sure to end with a new line.
   You should compare your output with the correct output in flights.out to make sure your formatting is correct.
 */
void printSchedule(airport_t* s) {
    // Replace this line with your code
    printf("%s\n", s->name);
    entry *tmp = s->sched;
    while (tmp) {
	printf("%s ", tmp->destination->name);
	printTime(&tmp->departure);
	printf(" ");
	printTime(&tmp->arrival);
	printf(" $%d\n", tmp->cost);
	tmp = tmp->next;
    }
}


/*
   Given a src and dst airport, and the time now, finds the next flight to take based on the following rules:
   1) Finds the cheapest flight from src to dst that departs after now.
   2) If there are multiple cheapest flights, take the one that arrives the earliest.

   If a flight is found, you should store the flight's departure time, arrival time, and cost in departure, arrival,
   and cost params and return true. Otherwise, return false.

   Please use the function isAfter() from time.h when comparing two timeHM_t objects.
 */
bool getNextFlight(airport_t* src, airport_t* dst, timeHM_t* now, timeHM_t* departure, timeHM_t* arrival, int* cost) {
    entry *best = NULL;
    entry sentinel, *tmp = &sentinel;
    sentinel.next = src->sched;
    while ((tmp = tmp->next)) {
	if ((strcmp(tmp->destination->name, dst->name) != 0) || isAfter(now, &tmp->departure)) {
	    continue;
	}
	if (!best
	    || tmp->cost < best->cost
	    || (tmp->cost == best->cost && isAfter(&best->arrival, &tmp->arrival))) {
	    best = tmp;
	}
    }
    if (!best)
	return false;
    *departure = best->departure;
    *arrival = best->arrival;
    *cost = best->cost;
    return true;
}

void delete_airport(airport_t *a) {
    while (a->sched) {
	entry *tmp = a->sched;
	a->sched = a->sched->next;
	free(tmp);
    }
    free(a->name);
    free(a);
}