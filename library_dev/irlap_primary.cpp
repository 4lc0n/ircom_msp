#include "irlap_primary.hpp"
#include "irphy.hpp"
#include "irlap.hpp"
#include "irphy_interface.hpp"

#ifndef UNIT_TESTING
#include <msp430.h>
#endif

#include <cstdlib>
#include <cstring>

/**
 * @brief Construct a new IrLAP_secondary::IrLAP_secondary object
 * 
 * @param irphy irphy interface type
 */
IrLAP_primary::IrLAP_primary()
{
    ;
}

void IrLAP_primary::init(IrPHY *irphy){
    this->irphy = irphy;
    irphy->init();

    // initialize the LAP layer

    // generate a unique device ID: 
    // use the random number generated on production, available in the TLV Table
    // under address 0x01A30 - 0x01A3F
    // device_address = *((uint32_t*)(0x01A30));
    device_address = 0xDEADBEEF;

    // set the default parameters
    current_parameter.baud_rate = {BAUD_PI, 1, BAUD_9600};
    current_parameter.turnaround_time = {TURNAROUND_T_PI, 1, TURNAROUND_T_500};
    current_parameter.data_size = {DATA_SIZE_PI, 1, DATA_SIZE_64};
    current_parameter.window_size = {WINDOW_SIZE_PI, 1, WINDOW_SIZE_1};
    current_parameter.add_BOFs = {ADD_BOFS_PI, 1, ADD_BOFS_12};
    current_parameter.turnaround_time_min = {TURNAROUND_T_MIN_PI, 1, TURNAROUND_T_MIN_10MS};
    current_parameter.link_disconnect_threshold = {LINK_DISC_TH_T_PI, 1, LINK_DISC_TH_T_40};


    irphy->set_baud(BAUD_9600);

}

/**
 * @brief function to go into idle / sleep state
 * 
 */
void IrLAP_primary::deinit(){
    irphy->deinit();

}

/**
 * @brief calculates the CRC16 sum over a given data block
 * 
 * @param data data block to be calculated over
 * @param length size in bytes of the data block
 * @return uint16_t 16 bit CRC checksum over the data block
 */
uint16_t IrLAP_primary::calcualte_CRC(uint8_t* data, uint16_t length)
{
    int ii = 0;

    // initialize the crc unit of the msp430
    // with 0xFFFF as specified in the stanard. 
    // per default / factory, the following polynomial is set
    // x^16 + x^12 + x^5 + 1
    // it matches the one specified by IrDA

#ifndef UNIT_TESTING
    CRCINIRES = 0xFFFF;

    // feed all the data in the input gate
    for(ii = 0; ii < length; ii++){
        CRCDI = data[ii];
    }

    // read the output of the system
    return CRCINIRES;

#else
    return 0;

#endif
}

/**
 * @brief tick function to be called periodically
 * 
 */
void IrLAP_primary::tick(){ 

    // do some stuff in here

    // check if frame available
    if(receive_and_store()) {
        // frame available: 

        // notify upper layer: 
        if(wrapper_in.frame.control == UI_CMD)  {
            IrLAP_USERDATA_indication(wrapper_in.frame.information, 64);
        }
    }
}



/**
 * @brief function to send connectionless user data to a broadcast address
 * 
 * @param userData data to be sent
 * @param length length of data
 * @return int bytes pushed into send buffer
 */
int IrLAP_primary::IrLAP_USERDATA_request(uint8_t *userData, uint16_t length)
{

    // construct the data frame
    uint8_t data_buffer[64];

    data_buffer[0] = 0xFF;          // send to broadcast address and C/R set
    data_buffer[1] = UI_CMD;        // send a UI frame

    // copy the userData into buffer
    memcpy(data_buffer + 2, userData, length);

    // call IrPHY function

    int bytes_sent = irphy->send_frame(data_buffer, length+2);

    // free memory
    
    return bytes_sent;
}


bool IrLAP_primary::receive_and_store(){
    uint16_t length;
    // uint8_t *data_wrapper = (uint8_t*)malloc(current_parameter.data_size.parameter * 64);
    uint8_t data_wrapper[64];

    if( ! irphy->get_new_frame(data_wrapper, length)){
        // nothing available 
        // free(data_wrapper);
        return false;
    }


    if(data_wrapper == 0 && length == 0){ 
        // not data available
        // free(data_wrapper);
        return false;
    }
    // copy the incoming frame into the input wrapper
    // using field assignment
    wrapper_in.bof = data_wrapper[0];
    wrapper_in.eof = data_wrapper[length-1];
    wrapper_in.fcs = (data_wrapper[length-3] << 8 | data_wrapper[length-2]);


    // copy the frame content from data_wrapper to memory. length - 4, so bof, eof and fcs is not included.
    memcpy((uint8_t*)(& wrapper_in.frame), (uint8_t*)(& (data_wrapper[1])), length - 4);

    // release memory 
    // free(data_wrapper);

    // make crc check
    // check the CRC check
    if (calcualte_CRC((uint8_t*)(& wrapper_in.frame), length-4) != wrapper_in.fcs)
    {
        // CRC-check failed
        // TODO: do something :D

        
    }

    
    return true;
}
