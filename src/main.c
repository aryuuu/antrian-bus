#include "lib/simlib.h"
#include "duration.h"
#include "events.h"
#include "location.h"
#include "queue.h"
#include "sample.h"
#include <stdio.h>

// Global Var
int bus_location;
double bus_wait_time;

// Initizialize Model of Car Rental System
void init_model() {
  // Init bus waiting timestamp
  bus_wait_time = sim_time;

  // Bus Arrive in Car Rental First
  event_schedule(sim_time, EVENT_BUS_CAR_RENTAL);

  // Generate Random for each Location
  event_schedule(sim_time + random_arrival_time(CAR_RENTAL),
                 EVENT_PEOPLE_CAR_RENTAL);
  event_schedule(sim_time + random_arrival_time(TERMINAL_1),
                 EVENT_PEOPLE_TERMINAL_1);
  event_schedule(sim_time + random_arrival_time(TERMINAL_2),
                 EVENT_PEOPLE_TERMINAL_2);

  // End Simulation
  event_schedule(DURATION_SIMULATION, EVENT_END_SIM);
}

void report() {
  printf("------------REPORT------------\n");
  // Number Queue
  printf("a) NUMBER QUEUE\n");
  for (int i = TERMINAL_1; i <= CAR_RENTAL; i++) {
    if (i == TERMINAL_1)
      printf(" > QUEUE TERMINAL 1 ");
    if (i == TERMINAL_2)
      printf(" > QUEUE TERMINAL 2 ");
    if (i == CAR_RENTAL)
      printf(" > QUEUE CAR_RENTAL ");
    sampst(0.0, -(NUMBER_QUEUE + i - 1));
    printf("(AVG : %.2f, MAX : %.2f)\n", transfer[1], transfer[3]);
  }

  // Delay
  printf("b) DELAY QUEUE\n");
  for (int i = TERMINAL_1; i <= CAR_RENTAL; i++) {
    if (i == TERMINAL_1)
      printf(" > QUEUE TERMINAL 1 ");
    if (i == TERMINAL_2)
      printf(" > QUEUE TERMINAL 2 ");
    if (i == CAR_RENTAL)
      printf(" > QUEUE CAR_RENTAL ");
    sampst(0.0, -(DELAY + i - 1));
    printf("(AVG : %.2f s, MAX : %.2f s)\n", transfer[1], transfer[3]);
  }

  // Number on Bus
  printf("c) NUMBER ON BUS ");
  sampst(0.0, -(TOTAL_PASSENGER));
  printf("(AVG : %.2f, MAX : %.2f)\n", transfer[1], transfer[3]);

  // Bus Stopping Time
  printf("d) BUS STOPPING TIME\n");
  for (int i = STOP_TIME_CAR_RENTAL; i <= STOP_TIME_TERMINAL_2; i++) {
    if (i == STOP_TIME_CAR_RENTAL)
      printf(" > CAR RENTAL ");
    if (i == STOP_TIME_TERMINAL_1)
      printf(" > TERMINAL 1 ");
    if (i == STOP_TIME_TERMINAL_2)
      printf(" > TERMINAL 2 ");

    sampst(0.0, -(i));
    printf("(AVG : %.2f, MAX : %2.f, MIN : %.2f)\n", transfer[1], transfer[3],
           transfer[2]);
  }

  int T, T1, T2;

  printf("e) BUS LOOP\n");
  for (int i = EVENT_BUS_GO_TO_TERMINAL_1; i <= EVENT_PEOPLE_CAR_RENTAL; i++) {
    T = 0;
    T = T + i;

    sampst(0.0, -(T));
    printf("(AVG : %.2f, MAX : %2.f, MIN : %.2f)\n", transfer[1], transfer[3],
           transfer[2]);
  }

  printf("f.1) A PERSON IN THE SYSTEM ARRIVE TO CAR RENTAL\n");
  for (int i = EVENT_BUS_GO_TO_TERMINAL_1; i <= EVENT_PEOPLE_CAR_RENTAL; i++) {
    T1 = 0;
    T2 = 0;
    // Terminal 1 ke Car Rental
    if (i != EVENT_BUS_GO_TO_TERMINAL_1) {
        T1 = T1 + i;
        //  Terminal 2 ke Car Rental
        if ((i != EVENT_PEOPLE_TERMINAL_1) && (i != EVENT_BUS_GO_TO_TERMINAL_2)) {
            T2 = T2 + i;
        }
    }
    sampst(0.0, -(T1 + T2));
    printf("(AVG : %.2f, MAX : %2.f, MIN : %.2f)\n", transfer[1], transfer[3],
           transfer[2]);
  }

  printf("f.2) A PERSON IN THE SYSTEM ARRIVE TO TERMINAL 1\n");
  for (int i = EVENT_BUS_GO_TO_TERMINAL_1; i <= EVENT_PEOPLE_CAR_RENTAL; i++) {
    T = 0;
    if ((i == EVENT_PEOPLE_CAR_RENTAL) || (i == EVENT_BUS_GO_TO_TERMINAL_1) || (i == EVENT_PEOPLE_TERMINAL_1)) {
        T = T + i;
    }
    sampst(0.0, -(T));
    printf("(AVG : %.2f, MAX : %2.f, MIN : %.2f)\n", transfer[1], transfer[3],
           transfer[2]);
  }

  printf("f.3) A PERSON IN THE SYSTEM ARRIVE TO TERMINAL 2\n");
  for (int i = EVENT_BUS_GO_TO_TERMINAL_1; i <= EVENT_PEOPLE_CAR_RENTAL; i++) {
    T = 0;
    if (i != EVENT_BUS_GO_TO_CAR_RENTAL) {
        T = T + i;
    }
    sampst(0.0, -(T));
    printf("(AVG : %.2f, MAX : %2.f, MIN : %.2f)\n", transfer[1], transfer[3],
           transfer[2]);
  }
}

int main() {
  /* Initialize simlib */
  init_simlib();

  /* Initialize model */
  init_model();

  do {
    /* Determine the next event. */
    timing();

    switch (next_event_type) {
    case EVENT_BUS_CAR_RENTAL:
      // printf("BC\n");
      bus_location = CAR_RENTAL;
      bus_wait_time = sim_time;
      bus_in(CAR_RENTAL);
      break;
    case EVENT_BUS_TERMINAL_1:
      // printf("BT1\n");
      bus_location = TERMINAL_1;
      bus_wait_time = sim_time;
      bus_in(TERMINAL_1);
      break;
    case EVENT_BUS_TERMINAL_2:
      // printf("BT2\n");
      bus_location = TERMINAL_2;
      bus_wait_time = sim_time;
      bus_in(TERMINAL_2);
      break;
    case EVENT_BUS_GO_TO_CAR_RENTAL:
      // printf("G T C\n");
      bus_location = ROAD;
      sampst(sim_time - bus_wait_time, STOP_TIME_TERMINAL_2);
      event_schedule(sim_time + DURATION_TO_CAR_RENTAL, EVENT_BUS_CAR_RENTAL);
      break;
    case EVENT_BUS_GO_TO_TERMINAL_1:
      // printf("G T T1\n");
      bus_location = ROAD;
      sampst(sim_time - bus_wait_time, STOP_TIME_CAR_RENTAL);
      event_schedule(sim_time + DURATION_TO_TERMINAL_1, EVENT_BUS_TERMINAL_1);
      break;
    case EVENT_BUS_GO_TO_TERMINAL_2:
      // printf("G T T2\n");
      bus_location = ROAD;
      sampst(sim_time - bus_wait_time, STOP_TIME_TERMINAL_1);
      event_schedule(sim_time + DURATION_TO_TERMINAL_2, EVENT_BUS_TERMINAL_2);
      break;

    case EVENT_PEOPLE_CAR_RENTAL:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(CAR_RENTAL),
                     EVENT_PEOPLE_CAR_RENTAL);

      if (bus_location == CAR_RENTAL && bus_num_passenger() < 20) {
        // printf("PC in\n");
        people_in(CAR_RENTAL);
        sampst(0.0, NUMBER_QUEUE + CAR_RENTAL - 1);
      } else {
        // printf("PC q\n");
        people_queue(CAR_RENTAL);
        sampst(list_size[LIST_QUEUE_CAR_RENTAL], NUMBER_QUEUE + CAR_RENTAL - 1);
      }
      break;
    case EVENT_PEOPLE_TERMINAL_1:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(TERMINAL_1),
                     EVENT_PEOPLE_TERMINAL_1);

      if (bus_location == TERMINAL_1 && bus_num_passenger() < 20) {
        // printf("PT1 in\n");
        people_in(TERMINAL_1);
        sampst(0.0, NUMBER_QUEUE + TERMINAL_1 - 1);
      } else {
        // printf("PT1 q\n");
        people_queue(TERMINAL_1);
        sampst(list_size[LIST_QUEUE_TERMINAL_1], NUMBER_QUEUE + TERMINAL_1 - 1);
      }
      break;
    case EVENT_PEOPLE_TERMINAL_2:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(TERMINAL_2),
                     EVENT_PEOPLE_TERMINAL_2);
      if (bus_location == TERMINAL_2 && bus_num_passenger() < 20) {
        // printf("PT2 in\n");
        people_in(TERMINAL_2);
        sampst(0.0, NUMBER_QUEUE + TERMINAL_2 - 1);
      } else {
        // printf("PT2 q\n");
        people_queue(TERMINAL_2);
        sampst(list_size[LIST_QUEUE_TERMINAL_2], NUMBER_QUEUE + TERMINAL_2 - 1);
      }
      break;
    case EVENT_END_SIM:
      // printf("end\n");
      break;
    }

  } while (next_event_type != EVENT_END_SIM);

  report();

  return 0;
}
