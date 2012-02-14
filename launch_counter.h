#ifndef LAUNCH_COUNTER_H_INC
#define LAUNCH_COUNTER_H_INC

class launch_counter : public PIDSource {
public:
    launch_counter(Counter&);
private:
    launch_counter() {}
};

#endif
