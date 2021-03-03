#include "queue.h"
#include "random.h"

// Event Trigger from Bus
#define EVENT_BUS_TERMINAL_1 1
#define EVENT_BUS_TERMINAL_2 2
#define EVENT_BUS_CAR_RENTAL 3
// #define EVENT_BUS_TRANSIT 7

// Event Trigger by People
#define EVENT_PEOPLE_TERMINAL_1 4 // People Arrive in Terminal 1
#define EVENT_PEOPLE_TERMINAL_2 5 // People Arrive in Terminal 2
#define EVENT_PEOPLE_CAR_RENTAL 6 // People Arrive in Car Rental

// Event End Simulation
#define EVENT_END_SIM 7

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
    return list_size[LIST_BUS_TO_CAR_RENTAL] + list_size[LIST_BUS_TO_TERMINAL_1] + list_size[LIST_BUS_TO_TERMINAL_2];
}

void unload_bus(int stop_location)
{
    if (stop_location == CAR_RENTAL)
    {
        double total_unload_time = 0;
        while (list_size[LIST_BUS_TO_CAR_RENTAL] > 0)
        {
            list_remove(FIRST, LIST_BUS_TO_CAR_RENTAL);
            double unload_time = random_unload_time();
            total_unload_time += unload_time;
            event_cancel(EVENT_BUS_TERMINAL_1);
        }
        event_schedule(sim_time + total_unload_time + MAXIMUM_WAITING_TIME, EVENT_BUS_TERMINAL_1);
    }
    else if (stop_location == TERMINAL_1)
    {
        double total_unload_time = 0;
        while (list_size[LIST_BUS_TO_TERMINAL_1] > 0)
        {
            list_remove(FIRST, LIST_BUS_TO_TERMINAL_1);
            double unload_time = random_unload_time();
            total_unload_time += unload_time;
            event_cancel(EVENT_BUS_TERMINAL_2);
        }
        event_schedule(sim_time + total_unload_time + MAXIMUM_WAITING_TIME, EVENT_BUS_TERMINAL_2);
    }
    else if (stop_location == TERMINAL_2)
    {
        double total_unload_time = 0;
        while (list_size[LIST_BUS_TO_TERMINAL_2] > 0)
        {
            list_remove(FIRST, LIST_BUS_TO_TERMINAL_2);
            double unload_time = random_unload_time();
            total_unload_time += unload_time;
            event_cancel(EVENT_BUS_CAR_RENTAL);
        }
        event_schedule(sim_time + total_unload_time + MAXIMUM_WAITING_TIME, EVENT_BUS_CAR_RENTAL);
    }
}

void load_bus(int from_location)
{
    if (from_location == CAR_RENTAL)
    {
        while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_CAR_RENTAL] > 0)
        {
            list_remove(FIRST, LIST_QUEUE_CAR_RENTAL);
            double wait_time = sim_time - transfer[1];
            double destination = transfer[2];

            if (destination == TERMINAL_1)
            {
                // transfer[1] = sim_time; // Departure Time
                list_file(LAST, LIST_BUS_TO_TERMINAL_1);
            }
            else if (destination == TERMINAL_2)
            {
                // transfer[1] = sim_time; // Departure Time
                list_file(LAST, LIST_BUS_TO_TERMINAL_2);
            }

            double load_time = random_load_time();
            event_cancel(EVENT_BUS_TERMINAL_1);
            event_schedule(sim_time + load_time + MAXIMUM_WAITING_TIME, EVENT_BUS_TERMINAL_1);
        }
    }
    else if (from_location == TERMINAL_1)
    {
        while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_TERMINAL_1] > 0)
        {
            list_remove(FIRST, LIST_QUEUE_TERMINAL_1);
            list_file(LAST, LIST_BUS_TO_CAR_RENTAL);

            double load_time = random_load_time();
            event_cancel(EVENT_BUS_CAR_RENTAL);
            event_schedule(sim_time + load_time + MAXIMUM_WAITING_TIME, EVENT_BUS_CAR_RENTAL);
        }
    }
    else if (from_location == TERMINAL_2)
    {
        while (bus_num_passenger() < 20 && list_size[LIST_QUEUE_TERMINAL_2] > 0)
        {
            list_remove(FIRST, LIST_QUEUE_TERMINAL_2);
            list_file(LAST, LIST_BUS_TO_CAR_RENTAL);

            double load_time = random_load_time();
            event_cancel(EVENT_BUS_CAR_RENTAL);
            event_schedule(sim_time + load_time + MAXIMUM_WAITING_TIME, EVENT_BUS_CAR_RENTAL);
        }
    }
}

void bus_in(int location)
{
    // printf("bus\n");
    unload_bus(location);
    load_bus(location);
}

void people_in(int location)
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
    }
    else if (location == TERMINAL_1 || location == TERMINAL_2)
    {
        list_file(LAST, LIST_BUS_TO_CAR_RENTAL);
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