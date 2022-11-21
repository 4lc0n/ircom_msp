#include <stdint.h>
#include <ringbuffer.hpp>

class IrPHY
{

private:
    char _input_buffer[255] = {0};
    char _output_buffer[255] = {0};
    Ringbuffer<char> input_buffer;
    Ringbuffer<char> output_buffer;

    bool _is_transmitting = false;
    bool _is_receiving = false;

    uint16_t last_receive_tick = 0;

    enum baudrate {
        b2400,
        b9600,
        b19200,
        b38400,
        b57600,
        b115200
    };

    /**
     * @brief virtual function to notify L2 to process the received message
     * 
     * @param frame 
     * @param length 
     */
    virtual void process_frame(uint8_t* frame, uint16_t length) = 0;

    uint16_t add_control_escape(uint8_t* data, uint16_t length);
    uint8_t* remove_control_escape(uint8_t* data, uint16_t length);

public:

    IrPHY();

    void init();
    void deinit();

    uint16_t send_frame(uint8_t *data, uint16_t len);
    
    bool is_transmitting() const;


    uint16_t receive(char *buff, uint16_t timeout_ms);
    uint16_t receive_available() const;
    bool is_receiving() const;

    void put_received_data(uint8_t data);
    void clear_intput_buffer();

    void send_next_data();

    void set_baud(enum baudrate);

};