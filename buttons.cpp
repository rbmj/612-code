#include "joysmooth.h"
#include "buttons.h"

buttons::buttons(joysmooth& h) {
    init(h.GetRawButton(1),
        h.GetRawButton(2),
        h.GetRawButton(3),
        h.GetRawButton(4),
        h.GetRawButton(5),
        h.GetRawButton(6),
        h.GetRawButton(7),
        h.GetRawButton(8),
        h.GetRawButton(9),
        h.GetRawButton(10),
        h.GetRawButton(11)
    );
}

buttons::buttons(bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7,
    bool b8, bool b9, bool b10, bool b11)
{
    init(b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11);
}
    
void buttons::init(bool b1, bool b2, bool b3, bool b4, bool b5, bool b6,
    bool b7, bool b8, bool b9, bool b10, bool b11)
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

bool& buttons::GetRawButton(int i) {
    return values[i - 1];
}

const bool buttons::GetRawButton(int i) const {
    return values[i - 1];
}
