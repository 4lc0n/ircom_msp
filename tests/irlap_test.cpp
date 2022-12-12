#include "irlap.hpp"
#include "irlap_secondary.cpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "irphy_interface.hpp"

class IrPHY_Mock : public IrPHY_Interface
{

    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, deinit, ());
    MOCK_METHOD(uint16_t, send_frame, (uint8_t* data, uint16_t len));
    MOCK_METHOD(bool, is_transmitting, (), (const));
    MOCK_METHOD(void, put_received_data, (uint8_t data));
    MOCK_METHOD(void, clear_input_buffer, ());
    MOCK_METHOD(void, send_next_data, ());
    MOCK_METHOD(void, send_baud, (uint8_t baudrate));
    MOCK_METHOD(void, get_new_frame, (uint8_t* &frame, uint16_t &length));


};

