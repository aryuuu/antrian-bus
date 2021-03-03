#include "lib/simlib.h"

// Locations
#define CAR_RENTAL 1
#define TERMINAL_1 2
#define TERMINAL_2 3

#define MINIMUM_WAITING_TIME 5 * 60 // in second
#define BUS_SPEED 30                // in miles/hour
#define DURATION_TO_TERMINAL_1 540  // in second
#define DURATION_TO_TERMINAL_2 120  // in second
#define DURATION_TO_CAR_RENTAL 540  // in second

// Events
#define EVENT_ARRIVAL_TERMINAL_1 1 // People Arrive in Terminal 1
#define EVENT_ARRIVAL_TERMINAL_2 2 // People Arrive in Terminal 2
#define EVENT_ARRIVAL_CAR_RENTAL 3 // People Arrive in Car Rental
#define EVENT_LOAD_CAR_RENTAL 4    // Loading People to Bus
#define EVENT_UNLOAD_CAR_RENTAL 5  // Unloading People from Bus

#define EVENT_BUS_TO_TERMINAL_1 6
#define EVENT_BUS_TO_TERMINAL_2 7
#define EVENT_BUS_TO_CAR_RENTAL 8

#define EVENT_BUS_ARRIVE_TERMINAL_1 9
#define EVENT_BUS_ARRIVE_TERMINAL_2 10
#define EVENT_BUS_ARRIVE_CAR_RENTAL 11

// Random Number Parameter
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

// Queue
#define LIST_QUEUE_CAR_RENTAL 1
#define LIST_QUEUE_TERMINAL_1 2
#define LIST_QUEUE_TERMINAL_2 3
#define LIST_BUS 4

int bus_location;
double bus_wait_time;

int random_destination()
{
    if (lcgrand(STREAM_DESTINATION) <= DESTINATION_TERMINAL_1)
    {
        return TERMINAL_1;
    }
    else
    {
        return TERMINAL_2;
    }
}

double random_load_time()
{
    return uniform(LOADING_RATE_MIN, LOADING_RATE_MAX, STREAM_LOADING_TIME);
}

int random_unload_time()
{
    return uniform(UNLOADING_RATE_MIN, UNLOADING_RATE_MAX, STREAM_UNLOADING_TIME);
}

void arrive_car_rental()
{
    /* Schedule next arrival. */
    event_schedule(sim_time + expon(CAR_RENTAL_ARRIVAL_RATE, STREAM_CAR_RENTAL), EVENT_ARRIVAL_CAR_RENTAL);

    /* Check if Bus in Car Rental */
    if (bus_location == CAR_RENTAL)
    {
        /* Bus Full */
        if (list_size[LIST_BUS] == 20)
        {
            transfer[1] = sim_time;
            transfer[2] = random_destination();
            list_file(LAST, LIST_QUEUE_CAR_RENTAL);
        }
        else
        {
            event_schedule(sim_time + random_load_time(), EVENT_LOAD_CAR_RENTAL);
        }
    }
    else /* Bus not in Car Rental */
    {
        transfer[1] = sim_time;
        transfer[2] = random_destination();
        list_file(LAST, LIST_QUEUE_CAR_RENTAL);
    }

    event_schedule(sim_time + MINIMUM_WAITING_TIME, EVENT_BUS_TO_TERMINAL_1);
}

void load_car_rental()
{
    bus_wait_time = sim_time;
    transfer[1] = sim_time;
    transfer[2] = random_destination();
    list_file(LAST, LIST_BUS);
}

void unload_car_rental()
{
    bus_wait_time = sim_time;
    list_remove(FIRST, LIST_BUS);
}

void arrive_terminal_1()
{
    /* Schedule next arrival. */
    event_schedule(sim_time + expon(TERMINAL_1_ARRIVAL_RATE, STREAM_TERMINAL_1), EVENT_ARRIVAL_TERMINAL_1);

    /* Bus Full */
    if (list_size[LIST_BUS] == 20)
    {
        transfer[1] = sim_time;
        list_file(LAST, LIST_QUEUE_TERMINAL_1);
    }
}

void arrive_terminal_2()
{
    /* Schedule next arrival. */
    event_schedule(sim_time + expon(TERMINAL_2_ARRIVAL_RATE, STREAM_TERMINAL_2), EVENT_ARRIVAL_TERMINAL_2);

    /* Bus Full */
    if (list_size[LIST_BUS] == 20)
    {
        transfer[1] = sim_time;
        list_file(LAST, LIST_QUEUE_TERMINAL_2);
    }
}

void bus_to_terminal_1()
{
    if (sim_time - bus_wait_time > MINIMUM_WAITING_TIME)
    {
        bus_wait_time = sim_time;
        event_schedule(sim_time + DURATION_TO_TERMINAL_1, EVENT_BUS_ARRIVE_TERMINAL_1);
    }
    else {
        event_cancel(EVENT_BUS_TO_TERMINAL_1);
    }
}

void bus_arrive_terminal_1()
{
    event_schedule(sim_time + random_unload_time(), EVENT_UNLOAD_CAR_RENTAL);
}

void bus_to_terminal_2()
{
    if (sim_time - bus_wait_time > MINIMUM_WAITING_TIME)
    {
        bus_wait_time = sim_time;
        bus_location = TERMINAL_2;
    }
    else {
        event_cancel(EVENT_BUS_TO_TERMINAL_2);
    }
}

void bus_to_car_rental()
{
    if (sim_time - bus_wait_time > MINIMUM_WAITING_TIME)
    {
        bus_wait_time = sim_time;
        bus_location = CAR_RENTAL;
    }
    else {
        event_cancel(EVENT_BUS_TO_CAR_RENTAL);
    }
}

int main()
{
    /* Initialize simlib */
    init_simlib();

    while (list_size[LIST_EVENT] > 0)
    {
        /* Determine the next event. */
        timing();

        switch (next_event_type)
        {
        case EVENT_LOAD_CAR_RENTAL:
            load_car_rental();
            break;
        case EVENT_UNLOAD_CAR_RENTAL:
            unload_car_rental();
            break;
        case EVENT_ARRIVAL_CAR_RENTAL:
            arrive_car_rental();
            break;
        case EVENT_BUS_ARRIVE_TERMINAL_1:
            bus_arrive_terminal_1();
            break;
        case EVENT_ARRIVAL_TERMINAL_1:
            arrive_terminal_1();
            break;
        case EVENT_ARRIVAL_TERMINAL_2:
            arrive_terminal_2();
            break;
        case EVENT_BUS_TO_TERMINAL_1:
            bus_to_terminal_1();
            break;
        }
    }

    return 0;
}