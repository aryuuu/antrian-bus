#include "location.h"

// Random Parameter
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

double random_unload_time()
{
  return uniform(UNLOADING_RATE_MIN, UNLOADING_RATE_MAX, STREAM_UNLOADING_TIME);
}

double random_arrival_time(int location)
{
    if (location == CAR_RENTAL)
    {
        return expon(CAR_RENTAL_ARRIVAL_RATE, STREAM_CAR_RENTAL) * 3600;
    }
    else if (location == TERMINAL_1)
    {
        return expon(TERMINAL_1_ARRIVAL_RATE, STREAM_TERMINAL_1) * 3600;
    }
    else if (location == TERMINAL_2)
    {
        return expon(TERMINAL_2_ARRIVAL_RATE, STREAM_TERMINAL_2) * 3600;
    }
}