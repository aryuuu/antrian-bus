#include "lib/simlib.h"
#include "location.h"
#include "duration.h"
#include "events.h"
#include "queue.h"

// Global Var
int bus_location;
double bus_wait_time;

// Initizialize Model of Car Rental System
void init_model()
{
  // Init bus waiting timestamp
  bus_wait_time = sim_time;

  // Bus Arrive in Car Rental First
  event_schedule(sim_time, EVENT_BUS_CAR_RENTAL);

  // Generate Random for each Location
  event_schedule(sim_time + random_arrival_time(CAR_RENTAL), EVENT_PEOPLE_CAR_RENTAL);
  event_schedule(sim_time + random_arrival_time(TERMINAL_1), EVENT_PEOPLE_TERMINAL_1);
  event_schedule(sim_time + random_arrival_time(TERMINAL_2), EVENT_PEOPLE_TERMINAL_2);

  // End Simulation
  event_schedule(DURATION_SIMULATION, EVENT_END_SIM);
}

// void arrive_car_rental()
// {
//   /* Schedule next arrival. */
//   event_schedule(sim_time + expon(CAR_RENTAL_ARRIVAL_RATE, STREAM_CAR_RENTAL),
//                  EVENT_ARRIVAL_CAR_RENTAL);

//   /* Check if Bus in Car Rental */
//   if (bus_location == CAR_RENTAL)
//   {
//     /* Bus Full */
//     if (list_size[LIST_BUS_TO_CAR_RENTAL] + list_size[LIST_BUS_TO_TERMINAL_1] +
//             list_size[LIST_BUS_TO_TERMINAL_2] ==
//         20)
//     {
//       transfer[1] = sim_time;
//       transfer[2] = random_destination();
//       list_file(LAST, LIST_QUEUE_CAR_RENTAL);
//     }
//     else
//     {
//       event_schedule(sim_time + random_load_time(), EVENT_LOAD_CAR_RENTAL);
//     }
//   }
//   else /* Bus not in Car Rental */
//   {
//     transfer[1] = sim_time;
//     transfer[2] = random_destination();
//     list_file(LAST, LIST_QUEUE_CAR_RENTAL);
//   }

//   event_schedule(sim_time + MAXIMUM_WAITING_TIME, EVENT_BUS_TO_TERMINAL_1);
// }

// void load_car_rental()
// {
//   bus_wait_time = sim_time;
//   transfer[1] = sim_time;
//   transfer[2] = random_destination();

//   if (transfer[2] == TERMINAL_1)
//   {
//     list_file(LAST, LIST_BUS_TO_TERMINAL_1);
//   }
//   else
//   {
//     list_file(LAST, LIST_BUS_TO_TERMINAL_2);
//   }
// }

// void unload_car_rental()
// {
//   bus_wait_time = sim_time;
//   list_remove(FIRST, LIST_BUS_TO_CAR_RENTAL);
// }

// void arrive_terminal_1()
// {
//   /* Schedule next arrival. */
//   event_schedule(sim_time + expon(TERMINAL_1_ARRIVAL_RATE, STREAM_TERMINAL_1),
//                  EVENT_ARRIVAL_TERMINAL_1);

//   /* Bus Full */
//   if (list_size[LIST_BUS_TO_CAR_RENTAL] + list_size[LIST_BUS_TO_TERMINAL_1] +
//           list_size[LIST_BUS_TO_TERMINAL_2] ==
//       20)
//   {
//     transfer[1] = sim_time;
//     list_file(LAST, LIST_QUEUE_TERMINAL_1);
//   }
// }

// void arrive_terminal_2()
// {
//   /* Schedule next arrival. */
//   event_schedule(sim_time + expon(TERMINAL_2_ARRIVAL_RATE, STREAM_TERMINAL_2),
//                  EVENT_ARRIVAL_TERMINAL_2);

//   /* Bus Full */
//   if (list_size[LIST_BUS_TO_CAR_RENTAL] + list_size[LIST_BUS_TO_TERMINAL_1] +
//           list_size[LIST_BUS_TO_TERMINAL_2] ==
//       20)
//   {
//     transfer[1] = sim_time;
//     list_file(LAST, LIST_QUEUE_TERMINAL_2);
//   }
// }

// void bus_to_terminal_1()
// {
//   if (sim_time - bus_wait_time > MAXIMUM_WAITING_TIME)
//   {
//     bus_wait_time = sim_time;
//     event_schedule(sim_time + DURATION_TO_TERMINAL_1,
//                    EVENT_BUS_ARRIVE_TERMINAL_1);
//   }
//   else
//   {
//     event_cancel(EVENT_BUS_TO_TERMINAL_1);
//   }
// }

// void bus_arrive_terminal_1()
// {
//   event_schedule(sim_time + random_unload_time(), EVENT_UNLOAD_CAR_RENTAL);
// }

// void bus_to_terminal_2()
// {
//   if (sim_time - bus_wait_time > MAXIMUM_WAITING_TIME)
//   {
//     bus_wait_time = sim_time;
//     bus_location = TERMINAL_2;
//   }
//   else
//   {
//     event_cancel(EVENT_BUS_TO_TERMINAL_2);
//   }
// }

// void bus_to_car_rental()
// {
//   if (sim_time - bus_wait_time > MAXIMUM_WAITING_TIME)
//   {
//     bus_wait_time = sim_time;
//     bus_location = CAR_RENTAL;
//   }
//   else
//   {
//     event_cancel(EVENT_BUS_TO_CAR_RENTAL);
//   }
// }

int main()
{
  /* Initialize simlib */
  init_simlib();

  /* Initialize model */
  init_model();

  do
  {
    /* Determine the next event. */
    timing();

    switch (next_event_type)
    {
    case EVENT_BUS_CAR_RENTAL:
      printf("BC\n");
      bus_location = CAR_RENTAL;
      bus_in(CAR_RENTAL);
      break;
    case EVENT_BUS_TERMINAL_1:
      printf("BT1\n");
      bus_location = TERMINAL_1;
      bus_in(TERMINAL_1);
      break;
    case EVENT_BUS_TERMINAL_2:
      printf("BT2\n");
      bus_location = TERMINAL_2;
      bus_in(TERMINAL_2);
      break;

    case EVENT_PEOPLE_CAR_RENTAL:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(CAR_RENTAL), EVENT_PEOPLE_CAR_RENTAL);

      if (bus_location == CAR_RENTAL && bus_num_passenger() < 20)
      {
        printf("PC in\n");
        people_in(CAR_RENTAL);
      }
      else
      {
        printf("PC q\n");
        people_queue(CAR_RENTAL);
      }
      break;
    case EVENT_PEOPLE_TERMINAL_1:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(TERMINAL_1), EVENT_PEOPLE_TERMINAL_1);

      if (bus_location == TERMINAL_1 && bus_num_passenger() < 20)
      {
        printf("PT1 in\n");
        people_in(TERMINAL_1);
      }
      else
      {
        printf("PT1 q\n");
        people_queue(TERMINAL_1);
      }
      break;
    case EVENT_PEOPLE_TERMINAL_2:
      // Schedule next
      event_schedule(sim_time + random_arrival_time(TERMINAL_2), EVENT_PEOPLE_TERMINAL_2);
      if (bus_location == TERMINAL_2 && bus_num_passenger() < 20)
      {
        printf("PT2 in\n");
        people_in(TERMINAL_2);
      }
      else
      {
        printf("PT2 q\n");
        people_queue(TERMINAL_2);
      }
      break;

    case EVENT_END_SIM:
      printf("end\n");
      end_sim();
      break;
    }

  } while (next_event_type != EVENT_END_SIM);

  return 0;
}