#ifndef IRPHY_INTERFACE
#define IRPHY_INTERFACE

#include <cstdint>


/**
 * @brief interface class used to mock the PHY for gtest
 * 
 */
class IrPHY_Interface 
{

public:

#ifdef UNIT_TESTING
    virtual ~IrPHY_Interface();
#endif

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual uint16_t send_frame(uint8_t* data, uint16_t len) = 0;    
    virtual bool is_transmitting() const = 0;
    virtual void put_received_data(uint8_t data) = 0;
    virtual void clear_intput_buffer() = 0;
    virtual void send_next_data() = 0;
    virtual void set_baud(uint8_t baudrate) = 0;

    /**
     * @brief virtual function to notify L2 to process the received message
     * 
     * @param frame 
     * @param length 
     * 
     */
    virtual bool get_new_frame(uint8_t* frame, uint16_t &length) = 0;


};

#endif
