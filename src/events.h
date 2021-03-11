#include "queue.h"
#include "random.h"
#include "sample.h"

// Event Trigger from Bus
#define EVENT_BUS_TERMINAL_1 1
#define EVENT_BUS_TERMINAL_2 2
#define EVENT_BUS_CAR_RENTAL 3
#define EVENT_BUS_GO_TO_TERMINAL_1 4
#define EVENT_BUS_GO_TO_TERMINAL_2 5
#define EVENT_BUS_GO_TO_CAR_RENTAL 6

// Event Trigger by People
#define EVENT_PEOPLE_TERMINAL_1 7 // People Arrive in Terminal 1
#define EVENT_PEOPLE_TERMINAL_2 8 // People Arrive in Terminal 2
#define EVENT_PEOPLE_CAR_RENTAL 9 // People Arrive in Car Rental

// Event End Simulation
#define EVENT_END_SIM 10

void end_sim()
{
  int ievent = EVENT_BUS_TERMINAL_1;
  for (ievent; ievent < EVENT_PEOPLE_CAR_RENTAL; ievent++)
  {
    event_cancel(ievent);
  }
}

int bus_num_passenger()
{
  return list_size[LIST_BUS_TO_CAR_RENTAL] + list_size[LIST_BUS_TO_TERMINAL_1] +
         list_size[LIST_BUS_TO_TERMINAL_2];
}

double unload_bus(int stop_location)
{
  double unload_time = 0;
  if (stop_location == CAR_RENTAL)
  {
    while (list_size[LIST_BUS_TO_CAR_RENTAL] > 0)
    {
      list_remove(FIRST, LIST_BUS_TO_CAR_RENTAL);
      unload_time += random_unload_time();
    }
  }
  else if (stop_location == TERMINAL_1)
  {
    while (list_size[LIST_BUS_TO_TERMINAL_1] > 0)
    {
      list_remove(FIRST, LIST_BUS_TO_TERMINAL_1);
      unload_time += random_unload_time();
    }
  }
  else if (stop_location == TERMINAL_2)
  {
    while (list_size[LIST_BUS_TO_TERMINAL_2] > 0)
    {
      list_remove(FIRST, LIST_BUS_TO_TERMINAL_2);
      unload_time += random_unload_time();
    }
  }
  return unload_time;
}

double load_bus(int from_location)
{
  double load_time = 0.0;
  if (from_location == CAR_RENTAL)
  {
    while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_CAR_RENTAL] > 0)
    {
      list_remove(FIRST, LIST_QUEUE_CAR_RENTAL);
      double delay = sim_time - transfer[1];
      sampst(delay, DELAY + CAR_RENTAL - 1);
      double destination = transfer[2];

      if (destination == TERMINAL_1)
      {
        list_file(LAST, LIST_BUS_TO_TERMINAL_1);
      }
      else if (destination == TERMINAL_2)
      {
        list_file(LAST, LIST_BUS_TO_TERMINAL_2);
      }

      load_time += random_load_time();
    }
  }
  else if (from_location == TERMINAL_1)
  {
    while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_TERMINAL_1] > 0)
    {
      list_remove(FIRST, LIST_QUEUE_TERMINAL_1);
      double delay = sim_time - transfer[1];
      sampst(delay, DELAY + TERMINAL_1 - 1);
      list_file(LAST, LIST_BUS_TO_CAR_RENTAL);

      load_time += random_load_time();
    }
  }
  else if (from_location == TERMINAL_2)
  {
    while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_TERMINAL_2] > 0)
    {
      list_remove(FIRST, LIST_QUEUE_TERMINAL_2);
      double delay = sim_time - transfer[1];
      sampst(delay, DELAY + TERMINAL_2 - 1);
      list_file(LAST, LIST_BUS_TO_CAR_RENTAL);

      load_time += random_load_time();
    }
  }
  return load_time;
}

void bus_in(int location)
{
  // printf("bus\n");
  double unload_time = unload_bus(location);
  double load_time = load_bus(location);

  double going_time = 0.0;
  if (load_time + unload_time <= MAXIMUM_WAITING_TIME)
  {
    going_time = sim_time + MAXIMUM_WAITING_TIME;
  }
  else
  {
    going_time = sim_time + load_time + unload_time;
  }

  if (location == CAR_RENTAL)
  {
    event_schedule(going_time, EVENT_BUS_GO_TO_TERMINAL_1);
  }
  else if (location == TERMINAL_1)
  {
    event_schedule(going_time, EVENT_BUS_GO_TO_TERMINAL_2);
  }
  else if (location == TERMINAL_2)
  {
    event_schedule(going_time, EVENT_BUS_GO_TO_CAR_RENTAL);
  }
}

void people_in(int location, double bus_wait_time)
{
  int destination = random_destination();
  if (location == CAR_RENTAL)
  {
    if (destination == TERMINAL_1)
    {
      list_file(LAST, LIST_BUS_TO_TERMINAL_1);
    }
    else if (destination == TERMINAL_2)
    {
      list_file(LAST, LIST_BUS_TO_TERMINAL_2);
    }

    double load_time = random_load_time();

    if (sim_time + load_time > bus_wait_time + MAXIMUM_WAITING_TIME)
    {
      event_cancel(EVENT_BUS_GO_TO_TERMINAL_1);
      event_schedule(sim_time + load_time,
                     EVENT_BUS_GO_TO_TERMINAL_1);
    }
  }
  else if (location == TERMINAL_1 || location == TERMINAL_2)
  {
    list_file(LAST, LIST_BUS_TO_CAR_RENTAL);

    double load_time = random_load_time();

    if (location == TERMINAL_1)
    {
      if (sim_time + load_time > bus_wait_time + MAXIMUM_WAITING_TIME)
      {
        event_cancel(EVENT_BUS_GO_TO_TERMINAL_2);
        event_schedule(sim_time + load_time,
                       EVENT_BUS_GO_TO_TERMINAL_2);
      }
    }
    else if (location == TERMINAL_2)
    {
      if (sim_time + load_time > bus_wait_time + MAXIMUM_WAITING_TIME)
      {
        event_cancel(EVENT_BUS_GO_TO_CAR_RENTAL);
        event_schedule(sim_time + load_time,
                       EVENT_BUS_GO_TO_CAR_RENTAL);
      }
    }
  }
}

void people_queue(int location)
{
  int destination = random_destination();
  transfer[1] = sim_time;
  transfer[2] = destination;
  if (location == CAR_RENTAL)
  {
    list_file(LAST, LIST_QUEUE_CAR_RENTAL);
  }
  else if (location == TERMINAL_1)
  {
    list_file(LAST, LIST_QUEUE_TERMINAL_1);
  }
  else if (location == TERMINAL_2)
  {
    list_file(LAST, LIST_QUEUE_TERMINAL_2);
  }
}