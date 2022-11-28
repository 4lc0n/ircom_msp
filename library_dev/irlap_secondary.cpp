#include "irlap_secondary.hpp"
#include <msp430.h>



uint16_t IrLAP_secondary::calcualte_CRC(uint8_t* data, uint16_t length)
{
    int ii = 0;

    // initialize the crc unit of the msp430
    // with 0xFFFF as specified in the stanard. 
    // per default / factory, the following polynomial is set
    // x^16 + x^12 + x^5 + 1
    // it matches the one specified by IrDA


    CRCINIRES = 0xFFFF;

    // feed all the data in the input gate
    for(ii = 0; ii < length; ii++){
        CRCDI = data[ii];
    }

    // read the output of the system
    return CRCINIRES;
}
