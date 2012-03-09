#ifndef BUTTONS_H_INC
#define BUTTONS_H_INC

#include "joysmooth.h"

class buttons {
private:
    static const int NUM_BUTTONS = 11;
    bool values[NUM_BUTTONS];
    void init(bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool);
public:
    buttons(joysmooth& h);
    buttons(bool = false, bool = false, bool = false, bool = false,
        bool = false, bool = false, bool = false, bool = false,
        bool = false, bool = false, bool = false);
    bool& GetRawButton(int i);
    const bool GetRawButton(int i) const;
};

#endif
