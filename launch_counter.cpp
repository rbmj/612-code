#include "launch_counter.h"
#include <Counter.h>

launch_counter::launch_counter(Counter& c) {
    counter_obj = &c;
}

double launch_counter::PIDGet() {
    //return rate
    return 0.0;
}
