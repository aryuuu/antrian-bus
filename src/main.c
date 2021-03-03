#include "lib/simlib.h"

#define CAR_RENTAL_LOCATION 0
#define TERMINAL_1_LOCATION 4.5
#define TERMINAL_2_LOCATION 5.5

#define TRACK_LENGTH 10

#define SIMULATION_DURATION 80 * 3600
#define MINIMUM_WAITING_TIME 5 * 60

// Random Number
#define CAR_RENTAL_ARRIVAL_RATE 24
#define TERMINAL_1_ARRIVAL_RATE 14
#define TERMINAL_2_ARRIVAL_RATE 10
#define UNLOADING_RATE_MIN 16
#define UNLOADING_RATE_MAX 24
#define LOADING_RATE_MIN 15
#define LOADING_RATE_MAX 25
#define DESTINATION_TERMINAL_1 0.583
#define DESTINATION_TERMINAL_2 0.417

// Stream
#define STREAM_TERMINAL_1 1
#define STREAM_TERMINAL_2 2
#define STREAM_CAR_RENTAL 3
#define STREAM_UNLOADING_TIME 4
#define STREAM_LOADING_TIME 5
#define STREAM_DESTINATION 6

#define BUS_SPEED 30 

// Queue
#define QUEUE_CAR_RENTAL 1
#define QUEUE_TERMINAL_1 2
#define QUEUE_TERMINAL_2 3
#define QUEUE_BUS 4

// Event
#define EVENT_ARRIVAL_CAR_RENTAL 1
#define EVENT_DEPART_CAR_RENTAL 2
#define EVENT_ARRIVAL_TERMINAL_1 3
#define EVENT_DEPART_TERMINAL_1 4
#define EVENT_ARRIVAL_TERMINAL_2 5
#define EVENT_DEPART_TERMINAL_2 6

/**
 * @brief generate passenger and push it to each queues in terminals
 *
 */
void generate_passenger();

/**
 * @brief move bus
 *
 */
void move_bus();

/**
 * @brief load passenger on a location
 *
 * @param location loading location
 * location's value should be 1 for car rental, 2 for terminal-1 and 3 for
 * terminal-2
 */
void load_passenger(int location);

/**
 * @brief unload passenger on a location
 *
 * @param location unloading location
 */
void unload_passenger(int location);

void generate_report();

/* CAR RENTAL FUNCTION */
void car_rental_depart()
{
  printf("depart car rental");
}

void car_rental_arrival()
{
  
  printf("arrival car rental");
}

/* TERMINAL 1 FUNCTION */
void terminal_1_depart()
{
  printf("depart terminal 1");
}

void terminal_1_arrival()
{
  printf("arrival terminal 1");
}

/* TERMINAL 2 FUNCTION */
void terminal_2_depart()
{
  printf("depart terminal 2");
}

void terminal_2_arrival()
{
  printf("arrival terminal 2");
}

int main() {
  /* Initialize simlib */
  init_simlib();

  /* Initialize model */
  init_model();

  /* Bus Start from Car Rental */
  event_schedule(0, EVENT_ARRIVAL_CAR_RENTAL);

  while (list_size[LIST_EVENT] > 0) {
    /* Determine the next event. */
    timing();

    switch (next_event_type) {
      // Bus in Car Rental
      case EVENT_ARRIVAL_CAR_RENTAL:
        car_rental_arrival();
        break;
      case EVENT_DEPART_CAR_RENTAL:
        car_rental_depart();
        break;
      case EVENT_ARRIVAL_TERMINAL_1:
        terminal_1_arrival();
        break;
      case EVENT_DEPART_TERMINAL_1:
        terminal_1_depart();
        break;
      case EVENT_ARRIVAL_TERMINAL_2:
        terminal_2_arrival();
        break;
      case EVENT_DEPART_TERMINAL_2:
        terminal_2_depart();
        break;
    }
  }

  return 0;
}
