#ifndef LAUNCH_COUNTER_H_INC
#define LAUNCH_COUNTER_H_INC

#include <Counter.h>
#include <PIDSource.h>

class launch_counter : public PIDSource {
public:
    launch_counter(Counter&);
    double PIDGet(); //get rate
private:
    launch_counter() {}
    Counter * counter_obj;
};

#endif
