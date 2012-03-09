#ifndef VISION_THREAD_H_INC
#define VISION_THREAD_H_INC

#include "vision_alt.h"
#include "612.h"

#include <Vision/AxisCamera.h>
#include <Vision/ColorImage.h>
#include <Vision/HSLImage.h>
#include <Task.h>
#include <vector>

class vision_thread {
public:
    typedef void(*vision_processor)(vision_thread&, ColorImage&);
    vision_thread(AxisCamera&, vision_processor);
    ~vision_thread();
    void enable();
    void disable();
    void lock_targets();
    void release_targets();
    bool lock_targets_nowait(); //do not block
private:
    SEM_ID target_lock;
    Task thread_obj;
    AxisCamera& cam;
    vision_processor callback;
    HSLImage image;
    //no lock (only write from main thread), only atomic
    bool enabled;
    static int thread_worker(uinteger);
    void process_loop();
};

//TODO:  Use lock objects, RAII, and other nicities
class vision_targets {
public:
    vision_targets();
    ~vision_targets();
#ifdef VISION_ALT_HEURISTIC
    target& bottom();
    target& top();
    target& left();
    target& right();
#else
    std::vector<target> targets();
#endif
private:
    static void update_helper(void*);
    void update();
#ifdef VISION_ALT_HEURISTIC
    target bottom_t;
    target top_t;
    target left_t;
    target right_t;
#else
    std::vector<target> targets_vec;
#endif
};

vision_thread& get_vision_thread();
vision_targets& get_targets();

#endif
