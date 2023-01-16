#ifndef TIME_H
#define TIME_H

#include <cstdint>
#include "msp430.h"


extern volatile uint32_t time_of;

/**
 * @brief Get the time in ms
 * 
 * @return uint32_t 
 */
uint32_t get_time() {
    return time_of + TA0R / 66;     // 
}


#endif