#include <stdint.h>
#include <ringbuffer.hpp>

class IrPHY
{
public:

    IrPHY();

    enum baudrate {
        b2400,
        b9600,
        b19200,
        b38400,
        b57600,
        b115200
    };

    void init();
    void deinit();

    uint16_t send_frame(uint8_t *data, uint16_t len);
    
    bool is_transmitting() const;


    // not needed, as irLAP is handling things via virtual functions
    // uint16_t receive(char *buff, uint16_t timeout_ms);
    // uint16_t receive_available() const;
    // bool is_receiving() const;

    void put_received_data(uint8_t data);

    void clear_intput_buffer();

    void send_next_data();

    void set_baud(enum baudrate);

    /**
     * @brief virtual function to notify L2 to process the received message
     * 
     * @param frame 
     * @param length 
     */
    virtual void notify_new_frame(uint8_t* frame, uint16_t length) = 0;


private:
    // TODO: maybe get rid of this
    uint8_t transfer_buffer[255] = {0};


    char _input_buffer[255] = {0};
    char _output_buffer[255] = {0};
    Ringbuffer<char> input_buffer;
    Ringbuffer<char> output_buffer;

    bool _is_transmitting = false;
    bool _is_receiving = false;

    uint16_t last_receive_tick = 0;

    
    enum baudrate current_baudrate = b9600;

    enum receive_states {state_a, state_b, state_c, state_d} receive_state;


 
    uint16_t add_control_escape(uint8_t* in_data, uint8_t* out_data, uint16_t length);


    


};