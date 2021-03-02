#include "lib/simlib.h"

#define CAR_RENTAL_LOCATION 0
#define TERMINAL_1_LOCATION 4.5
#define TERMINAL_2_LOCATION 5.5

#define TRACK_LENGTH 10

#define SIMULATION_DURATION placeholder
#define MINIMUM_WAITING_TIME placeholder

#define TERMINAL_1_PROBABILITY 0.583
#define TERMINAL_2_PROBILITY 0.417

#define CAR_RENTAL_ARRIVAL_RATE 24
#define TERMINAL_1_ARRIVAL_RATE 14
#define TERMINAL_2_ARRIVAL_RATE 10

#define BUS_SPEED 30

struct Queue {};

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

void load_passenger();

void unload_passenger();

int main() {
  int result = 0;
  return result;
}
