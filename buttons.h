#ifndef BUTTONS_H_INC
#define BUTTONS_H_INC

#include <GenericHID.h>

const int NUM_BUTTONS = 11;

class buttons {
public:
    bool values[NUM_BUTTONS];
    buttons(GenericHID& h) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            values[i] = h.GetRawButton(i + 1);
        }
    }
    buttons(bool b1 = false, bool b2 = false, bool b3 = false, bool b4 = false,
        bool b5 = false, bool b6 = false, bool b7 = false, bool b8 = false,
        bool b9 = false, bool b10 = false, bool b11 = false)
    {
        values[0] = b1;
        values[1] = b2;
        values[2] = b3;
        values[3] = b4;
        values[4] = b5;
        values[5] = b6;
        values[6] = b7;
        values[7] = b8;
        values[8] = b9;
        values[9] = b10;
        values[10] = b11;
    }
    bool& GetRawButton(int i) {
        return values[i - 1];
    }
    bool GetRawButton(int i) const {
        return values[i - 1];
    }
};

#endif
