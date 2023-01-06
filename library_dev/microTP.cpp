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

    return IrLAP_USERDATA_request(data, length);
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

}


/**
 * @brief callback function from IrLAP to notify, a frame was received
 * 
 * @param userData  information of the received UI frame
 * @param length    max. length of the information block
 */
void IrLAP_USERDATA_indication(uint8_t *userData, uint16_t length) {

}
