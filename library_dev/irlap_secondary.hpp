/**
 * @file irlap_secondary.hpp
 * @author Juergen Markl (s-jmarkl@haw-landshut.de)
 * @brief 
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 * 
 * 
 */

#ifndef IRLAP_SECONDARY_HPP
#define IRLAP_SECONDARY_HPP

#include "irphy.hpp"
#include <stdint.h>
#include "irlap.hpp"




class IrLAP_secondary : public IrPHY
{


private:

    uint32_t device_address;                    // uinque device address
    

    connection_parameter current_parameter;     // current configured parameters

    typedef struct {            
        uint8_t address;
        uint8_t control;
        uint8_t information[64];
    }frame; // frame_payload_out, frame_payload_in;     // rather use a frame ponter to the wrapper frame

    struct  wrapper{
        uint8_t bof;
        frame frame;
        uint16_t fcs;
        uint8_t eof; 
    } wrapper_out, wrapper_in;

    enum states { OFFLINE, NDM, CONN, NRMs, SCLOSE };

    enum states current_state = OFFLINE;


    bool new_frame_available = false;
    bool media_busy_flag = false;           // flag to indicate if it is legal to transmit
    uint32_t last_media_busy = 0;           // used to store the last time, a media event was recevied

    bool in_frame = false;                  // indicates if a frame is being recevied


    // methods for interfacing with the PHY
    // could also be moved to the PHY? nope?
    uint16_t calcualte_CRC(uint8_t* data, uint16_t length);
    void notify_new_frame(uint8_t* data_wrapper, uint16_t length) override;
    

public:
    IrLAP_secondary();

    void init();
    void deinit();

    void tick();

    virtual void IrLAP_CONNECT_indication() = 0;
    void IrLAP_CONNECT_response();

    void IrLAP_DATA_request(uint8_t *userData);
    virtual void IrLAP_DATA_indication(uint8_t *userData, uint16_t length) = 0;

    void IrLAP_DISCONNECT_request();
    virtual void IrLAP_DISCONNECT_indication() = 0;


};


#endif