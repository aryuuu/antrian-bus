#include "lib/simlib.h"

#define CAR_RENTAL_LOCATION 0
#define TERMINAL_1_LOCATION 4.5
#define TERMINAL_2_LOCATION 5.5

#define TRACK_LENGTH 10

#define SIMULATION_DURATION 80 * 3600
#define MINIMUM_WAITING_TIME 5 * 60

#define TERMINAL_1_PROBABILITY 0.583
#define TERMINAL_2_PROBILITY 0.417

#define CAR_RENTAL_ARRIVAL_RATE 24
#define TERMINAL_1_ARRIVAL_RATE 14
#define TERMINAL_2_ARRIVAL_RATE 10

#define BUS_SPEED 30

/* GLOBAL VARS */
int GLOBAL_CLOCK = 0;

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

/**
 * @brief Tick the global clock
 *
 * @param n time in seconds
 */
void tick_clock(int n);

void generate_report();

int main() {
  int result = 0;
  return result;
}
