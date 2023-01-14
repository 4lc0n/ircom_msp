#ifndef IRPHY_HPP
#define IRPHY_HPP

#include <cstdint>
#include <ringbuffer.hpp>

#include "irphy_interface.hpp"
#include "irlap.hpp"

// #define BITBANG
#define BITBANG_PULSE_US 19
#define BITBANG_DELAY_PULSE __delay_cycles(19)
#define BITBANG_PAUSE_US 85
#define BITBANG_DELAY_PAUSE __delay_cycles(85)

// #define USE_REGULAR_UART
#define UART_PULSE_US 104
#define UART_DELAY_PULSE __delay_cycles(40)


class IrPHY : public IrPHY_Interface
{
public:

    IrPHY();



    void init();
    void deinit();

    /**
     * @brief Set the receive callback object for receiving a frame, argument is number of bytes
     * 
     * @param callback 
     */


    uint16_t send_frame(uint8_t *data, uint16_t len);
    
    bool is_transmitting() const;


    // not needed, as irLAP is handling things via virtual functions
    // uint16_t receive(char *buff, uint16_t timeout_ms);
    // uint16_t receive_available() const;
    // bool is_receiving() const;

    void put_received_data(uint8_t data);

    void clear_intput_buffer();

    void send_next_data();

    void set_baud(uint8_t baudrate);

    /**
     * @brief Get the new frame 
     * 
     * @param frame     frame pointer reference; will be set to address
     * @param length    length; will be set to number of bytes
     * @return bool     bool value
     * @retval true     data is ready
     * @retval false    no data ready
     */
    bool get_new_frame(uint8_t* frame, uint16_t &length);



private:
    // TODO: maybe get rid of this
    uint8_t transfer_buffer[256] = {0};


    char _input_buffer[100] = {0};
    char _output_buffer[100] = {0};
    Ringbuffer<char> input_buffer;
    Ringbuffer<char> output_buffer;

    bool _is_transmitting = false;
    bool _is_receiving = false;

    uint16_t _data_bytes_ready = 0;

    uint16_t last_receive_tick = 0;

    
    uint8_t current_baudrate = BAUD_9600;

    enum receive_states {state_a, state_b, state_c, state_d} receive_state;




};

#endif
