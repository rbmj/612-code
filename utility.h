#ifndef UTILITY_H_INC
#define UTILITY_H_INC

template<class T>
T coerce(T val, T min, T max) {
    if (val < min) {
        return min;
    }
    else if (val > max) {
        return max;
    }
    else {
        return val;
    }
}

#endif
