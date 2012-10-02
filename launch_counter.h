#ifndef LAUNCH_COUNTER_H_INC
#define LAUNCH_COUNTER_H_INC

#include <Counter.h>
#include <PIDSource.h>

class launch_counter : public PIDSource {
public:
    launch_counter(Counter&);
    virtual ~launch_counter() {}
    double PIDGet();
    double get_frequency();
private:
    launch_counter() {}
    Counter * counter_obj;
};

#endif
