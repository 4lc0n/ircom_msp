#ifndef IRLAP_SECONDARY_HPP
#define IRLAP_SECONDARY_HPP

#include <irphy.hpp>
#include <stdint.h>


class IrLAP_secondary : public IrPHY
{


private:

    
    

    struct frame{
        uint8_t address;
        uint8_t control;
        uint8_t* information;
    } frame_payload;

    struct  {
        uint8_t *xbof;
        uint8_t bof;
        frame *frame;
        uint16_t fcs;
        uint8_t eof; 
    } wrapper_out, wrapper_in;

    enum states { OFFLINE, NDM, CONN, NRMs, SCLOSE };

    enum states current_state = OFFLINE;


    // methods for interfacing with the PHY
    // could also be moved to the PHY? nope?
    uint16_t calcualte_CRC(uint8_t* data, uint16_t length);

    void handle_discovery_procedure(uint8_t *frame, uint16_t length);



    void notify_new_frame(uint8_t* frame, uint16_t length) override;
    

public:
    IrLAP_secondary();

    void init();
    void deinit();

    virtual void IrLAP_CONNECT_indication() = 0;
    void IrLAP_CONNECT_response();

    void IrLAP_DATA_request(uint8_t *userData);
    virtual void IrLAP_DATA_indication(uint8_t *userData, uint16_t length) = 0;

    void IrLAP_DISCONNECT_request();
    virtual void IrLAP_DISCONNECT_indication() = 0;


};


#endif