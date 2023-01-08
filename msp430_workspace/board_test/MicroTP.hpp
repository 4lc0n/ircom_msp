/**
 * @file microTP.hpp
 * @author Juergen Markl (s-jmarkl@haw-landshut.de)
 * @brief minimal Transport Protocol (layer 4 OSI-model)
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * Transport Protocol to simply transfer chunks of data
 * 
 * Adds overhead to the data passed through: 
 * 
 * --------------------------
 * | 1B | 1B | n*B data     |
 * --------------------------
 *    |   |    |
 *    |   |     -> data bytes
 *    |    -> number of bytes of data (max. 255)
 *     -> packet identifier
 * 
 * 
 */

#ifndef MICROPTP
#define MICROPTP

#include "irlap_primary.hpp"
#include "irlap.hpp"
#include "irphy.hpp"

#include <cstdint>

#define microTP_identifier (uint8_t)(0x69)  // identifier for this packet type
#define BUFFER_LENGTH (64)                  // length of buffer
#define ADDED_OVERHEAD (2)                  // added overhead on wrapping


class MicroTP : protected IrLAP_primary {

private:

    
    bool is_initialized_ = false;
    
    uint16_t packet_available = 0;                  // indicator if a packet was received (!= 0) or not (== 0)


    uint8_t buffer_in[BUFFER_LENGTH];      // data puffer for received data, in plan information format
    uint8_t buffer_out[BUFFER_LENGTH];     // data puffer in packet format
    
    void IrLAP_USERDATA_indication(uint8_t *userData, uint16_t length);

    void packet_wrap(uint8_t* data, uint16_t length);
    bool packet_unwrap(uint8_t* data, uint16_t length);

public: 

    MicroTP();

    int send(uint8_t* data, uint16_t length);

    int receive(uint8_t* data, uint16_t* length);

    void init(IrPHY_Interface* irphy);
    void deinit();

};

#endif
