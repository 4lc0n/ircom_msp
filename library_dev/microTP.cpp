/**
 * @file microTP.cpp
 * @author Juergen Markl (s-jmarkl@haw-landshut.de)
 * @brief 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "microTP.hpp"
#include "irlap.hpp"
#include "irlap_primary.hpp"
#include "irphy.hpp"
#include <cstring>

using IrLAP_primary;        // use namespace of IrLAP class

/**
 * @brief Construct a new MicroTP::MicroTP object
 * 
 */
MicroTP::MicroTP() 
    : irphy()
    ,IrLAP_primary(irphy) 
{
    ;
}

/**
 * @brief initialize this and base classes 
 * 
 */
void MicroTP::init() {
    IrLAP_primary::init();
    is_initialized_ = true;
}

/**
 * @brief deinitialize this and base classes
 * 
 */
void MicroTP::deinit() {
    IrLAP_primary::deinit();
    is_initialized_ = false;
    free(buffer_in);
}


/**
 * @brief send a data packet out via a USERDATA frame (unack and unnumbered)
 * 
 * @param data pointer to data packet
 * @param length length of data packet
 * @return int result of transmission
 * @retval -1: instances are not initialized yet
 * @retval >= 0: number of transmitted bytes
 */
int MicroTP::send(uint8_t* data, uint16_t length) {

    if(!is_initialized_) {
        return -1;
    }

    packet_wrap(data, length);

    return IrLAP_USERDATA_request(buffer_out, length + added_overhead);
}

/**
 * @brief receive a packet via USERDATA
 * 
 * @param data pointer to data puffer
 * @param length number of bytes received
 * @return int receive indicator
 * @retval 0 successfully received
 * @retval -1 no data to be received
 */
int MicroTP::receive(uint8_t* data, uint16_t* length) {
    
    // check if a packet is available
    if(packet_available != 0) {
        memcpy(data, buffer_in, packet_available);
        *length = packet_available;
        return 0;
    }

    return -1;
}


/**
 * @brief callback function from IrLAP to notify, a frame was received
 * 
 * @param userData  information of the received UI frame
 * @param length    max. length of the information block
 */
void MicroTP::IrLAP_USERDATA_indication(uint8_t *userData, uint16_t length) {
    if(packet_unwrap(userData, length)) {
        packet_available = length - added_overhead;
    }
    else {  // not successfully unwrapped
        packet_available = 0;
    }

}


/**
 * @brief function to copy data into out puffer append packet type and length + data
 * 
 * @param data data to be wrapped
 * @param length length of data; must be less equal (buffer_length - 2)
 */
void MicroTP::packet_wrap(uint8_t* data, uint16_t length) {
    if(length > buffer_length - 2) {
        // not enough space in buffer
    }

    buffer_out[0] = 0x69;
    buffer_out[1] = (length > buffer_length) ? buffer_length : length;

    memcpy(&buffer_length[2], data, buffer_out[1]);
}

bool MicroTP::packet_unwrap(uint8_t* data, uint16_t length) {
    if(data[0] != microTP_identifier) {
        // wrong packet identifier
        // do not unwrap
        return false;
    }
    uint16_t incoming_length = data[1];
    if(incoming_length != (length - 2)) {
        // error: 
        // the assumed data length does not match the length of the packet
        return false;
    }

    // copy data into input buffer
    memcpy(buffer_in, &buffer[2], incoming_length);
    return true;
}