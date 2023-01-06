/**
 * @file microTP.hpp
 * @author Juergen Markl (s-jmarkl@haw-landshut.de)
 * @brief minimal Transport Protocol (layer 4 OSI-model)
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MICROPTP
#define MICROPTP

#include "irlap_primary.hpp"
#include "irlap.hpp"
#include "irphy.hpp"

#include <cstdint>


class MicroTP : private IrLAP_primary {

private:

    IrPHY irphy;
    
    bool is_initialized_ = false;
    uint8_t buffer_in*; 

    void IrLAP_USERDATA_indication(uint8_t *userData, uint16_t length);

public: 

    MicroTP();

    int send(uint8_t* data, uint16_t length);

    int receive(uint8_t* data, uint16_t* length);

    void init();
    void deinit();

}

#endif