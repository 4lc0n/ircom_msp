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

#ifndef IRLAP_SECONDARY
#define IRLAP_SECONDARY

#include "irphy.hpp"
#include <cstdint>
#include "irlap.hpp"




class IrLAP_secondary
{


private:

    IrPHY_Interface *irphy;

    uint32_t device_address;                    // uinque device address
    

    connection_parameter current_parameter;     // current configured parameters

    typedef struct {            
        uint8_t address;
        uint8_t control;
        uint8_t information[64];
    }frame_t; // frame_payload_out, frame_payload_in;     // rather use a frame ponter to the wrapper frame

    struct  wrapper{
        uint8_t bof;
        frame_t frame;
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
    
    bool receive_and_store();
    

public:
    IrLAP_secondary();

    void init(IrPHY_Interface *irphy);
    void deinit();

    void tick();

    // virtual void IrLAP_CONNECT_indication() = 0;
    // void IrLAP_CONNECT_response();

    // void IrLAP_DATA_request(uint8_t *userData);
    // virtual void IrLAP_DATA_indication(uint8_t *userData, uint16_t length) = 0;

    // void IrLAP_DISCONNECT_request();
    // virtual void IrLAP_DISCONNECT_indication() = 0;

    int IrLAP_USERDATA_request(uint8_t *userData, uint16_t length);
    // virtual void IrLAP_USERDATA_indication(uint8_t *userData, uint16_t length);


};


#endif
