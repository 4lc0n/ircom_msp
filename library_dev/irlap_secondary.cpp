#include "irlap_secondary.hpp"
#include "irphy.hpp"
#include "irda.hpp"
#include <msp430.h>


/**
 * @brief Construct a new IrLAP_secondary::IrLAP_secondary object
 * 
 */
IrLAP_secondary::IrLAP_secondary() : IrPHY()
{
    
}

void IrLAP_secondary::init(){
    IrPHY::init();

    // initialize the LAP layer

    // generate a unique device ID: 
    // use the random number generated on production, available in the TLV Table
    // under address 0x01A30 - 0x01A3F
    device_address = *((uint32_t*)(0x01A30));

    
}


/**
 * @brief interface function for the IrPHY to let the IrLAP know, a new
 * frame has arrived
 * 
 * @param frame data pointer
 * @param length length of the frame
 */
void IrLAP_secondary::notify_new_frame(uint8_t* frame, uint16_t length)
{
    // TODO: implement
}


/**
 * @brief calculates the CRC16 sum over a given data block
 * 
 * @param data data block to be calculated over
 * @param length size in bytes of the data block
 * @return uint16_t 16 bit CRC checksum over the data block
 */
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

