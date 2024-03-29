#include <cstdint>

#include "msp430.h"
#include "board.h"
#include "time.h"

#include "irphy.hpp"
#include "ringbuffer.hpp"
#include "irlap.hpp"



IrPHY::IrPHY()
{

    // initialize the buffers
    input_buffer = Ringbuffer<char>(_input_buffer, sizeof(_input_buffer));
    output_buffer = Ringbuffer<char>(_output_buffer, sizeof(_output_buffer));


    
}

/**
 * @brief initializes the hardware for transmission and reception
 * 
 */
void IrPHY::init()
{
 #ifndef BITBANG
    // initialize hardware
    // Configure USCI_A0 for UART mode
    UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // F_CLOCK = 1 MHz
    // according to Table 30-5:
    // UCOS16 = 1
    // UCBRx = 6
    // UCBRF = 8

    // calculated:
    // N = F_CLOCK / (BAUD (115200)) = 104.17
    // FRAC = 0xAA (Table 30-4)
    // As of oversampling: 
    // UCBRx = INT(N) = 4
    // UCBRFx = INT( [ (N/16) - INT(N/16) ] * 16) = 5
    // UCBRSx = 0xAA
    UCA1BRW = 4;
    UCA1MCTLW = (49 << 8) | (UCBRF_5) | UCOS16; // | UCBRF_1;

#ifndef USE_REGULAR_UART


    // enable IrDA encoding
    // pulse duration defined by UCIRTXPLx bits, specifying the number of one half clock periods of the clock selected by UCIRTXCLK
    // to set the pulse time to 3/16 bit period required by the IrDA standard, the BITCLK16 clock is selected with UCIRTXCLK=1 and the pulse length
    // is set to six one-half clock cycles with UCIRTXPLx = 6-1 = 5

    // set pulse length, enalbe BITCLK16, enable encoder/decoder
    UCA1IRCTL |= (5 << 2) | (UCIRTXCLK) | (UCIREN) | (UCIRRXPL);

#if 0   // do not set input filter
    // set the minimum pulse length for receiving: 
    // set to pulse length at 115200 bit/s: 1.41 µs
    // calculation: tMIN = (UCIRRXFLx + 4) / [2 × fIRTXCLK]
    // uncomment the following line and see the result, the compiler will produce
    // for a  fIRTXCLK = SMCLK = 8MHz and 1.41µs = tMIN, result is 18
    // uint16_t tmp = (uint16_t)(1.41 * (2.0 * 8.0) - 4.0);
    UCA1IRCTL &= ~(0x3F << 10);
    UCA1IRCTL |= (18 << 10) | (UCIRRXFE) ;

#endif

#endif

    UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA1IE |= UCRXIE | UCTXIFG;               // Enable USCI_A0 RX interrupt and TX interrupt (on transmit done)
#else
    IRDA_DIR |= (IRDA_TX_PIN);                  // set to normal pin function
    IRDA_SEL0 &= ~(IRDA_TX_PIN);
    IRDA_SEL1 &= ~(IRDA_TX_PIN);

#ifdef USE_REGULAR_UART
    IRDA_OUT |= (IRDA_TX_PIN);                  // set to idle high
#else
    IRDA_OUT &= ~(IRDA_TX_PIN);                 // set to idle low
#endif
#endif
}

/**
 * @brief de-initializes the hardware and ensures lowest energy consumption
 * 
 */
void IrPHY::deinit()
{
    // put periphery in reset mode
    
    UCA1CTLW0 |= UCSWRST;
}

/**
 * @brief puts data into the transmit buffer and enables transmission
 *  
 * if the entire data object does not fit into the buffer, put until buffer
 * is full, return number of inserted bytes
 * @param data array to be sent
 * @param len length of data
 * @return uint16_t length of inserted data
 */
uint16_t IrPHY::send_frame(uint8_t *data, uint16_t len)
{
    uint16_t ii = 0;

    // if data does not fit entirely in the buffer: 
    // add until full
    // if(output_buffer.storage_available() < len){
    //     while(!output_buffer.is_full() && ii < len)
    //     {
    //         output_buffer.put(data[ii]);
    //         ii++;
    //     }
    //     // return number of inserted bytes

    //     if (!_is_transmitting)
    //     {
    //         send_next_data();
    //     }
    //     return ii;
    // }   
    // else
    // {
        // add frame begin thingy
        output_buffer.put(IRLAP_BOF);
        // copy entire buffer
        for(ii = 0; ii < len; ii++)
        {
            output_buffer.put(data[ii]);
        }

        if (!_is_transmitting)
        {
            send_next_data();
        }
        output_buffer.put(IRLAP_EOF);
        return len;
    // }
}

bool IrPHY::is_transmitting() const
{
    return _is_transmitting;
}


/**
 * @brief copies data from receive buffer, waits for timeout_ms, if no data is in buffer
 * be aware: buffer must be at least as big as the initialized buffer of IrPHY
 * 
 * @param buff buffer the received data is copied to
 * @param timeout_ms timeout until the function returns if no data available
 * @return uint16_t number of bytes received

uint16_t IrPHY::receive(char *buff, uint16_t timeout_ms = 0xFFFF)
{
    uint16_t ii = 0;
    while(input_buffer.is_empty())
    {
        // todo: make a better delay loop than this or verify this
        __delay_cycles(8e3);
        ii++;
        if(ii > timeout_ms)
        {
            break;
        }
    }

    ii = 0;

    while(!input_buffer.is_empty()){
        buff[ii] = input_buffer.pop();
        ii++;
    }

    return ii;

}
 */

/**
 * @brief check if data in input buffer is available
 * 
 * @return uint16_t bytes available

uint16_t IrPHY::receive_available() const
{
    return input_buffer.storage_available();
}
 */

/*
bool IrPHY::is_receiving() const
{
    return _is_receiving;
}*/



/**
 * @brief helper function to allow data insertion from ISR, handles frame management
 * 
 * does not check if buffer is full, if so; data will be discarded.
 * 
 * @param data 
 */
void IrPHY::put_received_data(uint8_t data)
{
    
    /*
        in this function, the state machine is implemented as seen in IrLAP Version 1.1 on page 118
        to remove the Escape Sequence from the receiving frame
    */

    switch (receive_state)
    {
    case state_a:
        
        if(data == IRLAP_BOF)
        {
            // begin of frame: clear buffer
            clear_intput_buffer();
            input_buffer.put(data);
            receive_state = state_b;
        }
        else{
            // stay in here
        }

        break;

    case state_b:

        if(data ==IRLAP_BOF)
        {
            // begin of frame: clear buffer
            clear_intput_buffer();
            input_buffer.put(data);
            // do not change state
        }
        else if(data == IRLAP_CE)
        {
            // do not put to input buffer
            // just change state
            receive_state = state_c;
        }
        else if(data == IRLAP_EOF)
        {
            // abort
            // TODO: notify the abort
            receive_state = state_a;
        }
        else
        {
            input_buffer.put(data);
            receive_state = state_d;
        }

        break;

    case state_c:

        if(data == IRLAP_BOF)
        {
            // begin of frame: clear buffer
            clear_intput_buffer();
            input_buffer.put(data);
            // change state to B
            receive_state = state_b;
        }
        else if(data == IRLAP_EOF)
        {
            receive_state = state_a;
            // TODO: maybe inform the upper layer of an received aborted message
        }
        else 
        {
            input_buffer.put(data ^ 0x20);
            receive_state = state_d;
        }

        break;

    case state_d:

        if(data == IRLAP_CE)
        {
            // do not put to input buffer
            // just change state
            receive_state = state_c;
        }
        else if(data == IRLAP_BOF)
        {
            // begin of frame: clear buffer
            clear_intput_buffer();
            input_buffer.put(data);
            // change state to B
            receive_state = state_b;
        }
        else if(data == IRLAP_EOF)
        {
            input_buffer.put(data);

            // notify the upper level, that a new frame is received
            // transfer to transfer buffer
            uint16_t i = 0;
            while(!input_buffer.is_empty())
            {
                transfer_buffer[i] = input_buffer.pop();
                i++;
            }

            // change state to A
            receive_state = state_a;

            // set flag to data ready: set the amounts of bytes in the transfer buffer
            _data_bytes_ready = i;

            // notify upper layer
            // TODO

            
        }
        else{
            input_buffer.put(data);
        }

        break;
    
    default:
        break;
    }
   
}

/**
 * @brief Get the new frame 
 * 
 * @param frame     frame pointer reference; will be set to address
 * @param length    length; will be set to number of bytes
 * @return true     data is ready
 * @return false    no data ready
 */
bool IrPHY::get_new_frame(uint8_t* frame, uint16_t &length)
{
    if(_data_bytes_ready == 0)
    {
        return false;
    }

    length =  _data_bytes_ready;
    frame = transfer_buffer;

    // reset the data ready flag
    _data_bytes_ready = 0;

    return true;

}


void IrPHY::clear_intput_buffer()
{
    input_buffer.clear_buffer();
}



void IrPHY::send_next_data()
{
    if(!_is_transmitting) {
        
        _is_transmitting = true;
    }
   
    if(!output_buffer.is_empty())
    {
        // send data
        char d = output_buffer.pop();
        
#ifndef BITBANG

        UCA1TXBUF = d;

#else

        // send start

        uint32_t t = get_time();
        uint32_t start_t = t;

#ifdef USE_REGULAR_UART
        IRDA_OUT &= ~(IRDA_TX_PIN);
        
        UART_DELAY_PULSE;
#else
        IRDA_OUT |= IRDA_TX_PIN;

        BITBANG_DELAY_PULSE;
        IRDA_OUT &= ~IRDA_TX_PIN;

        BITBANG_DELAY_PAUSE;
#endif

        uint8_t bit = 0x80;
        do {
            t = get_time();
            start_t = t;
#ifdef USE_REGULAR_UART
            if(bit & d){
                IRDA_OUT |= IRDA_TX_PIN;
            }
            else {
                IRDA_OUT &= ~(IRDA_TX_PIN);
            }
            UART_DELAY_PULSE;
#else
            // if bit is a zero: send a pulse
            if(!(bit & d)) {
                IRDA_OUT |= IRDA_TX_PIN;
            }
            
            // wait pulse length
            BITBANG_DELAY_PULSE;
            // clear pulse
            IRDA_OUT &= ~IRDA_TX_PIN;

            // wait for pause
            BITBANG_DELAY_PAUSE;
#endif

            // advance a bit to LSB
            bit = bit >> 1;
        }
        while(bit != 0x0);

        // send stop bit
        t = get_time();
        start_t = t;

#ifdef USE_REGULAR_UART
        IRDA_OUT |= IRDA_TX_PIN;
        UART_DELAY_PULSE;
#else
        // wait for 1 pulse length, as stop bit is high --> is no pulse
        BITBANG_DELAY_PAUSE;
        BITBANG_DELAY_PULSE;
#endif
        // recursion: call until _is_transmitting is set to false; recursion stops
        // delay:
        __delay_cycles(2e3);
        send_next_data();
#endif
        
    }
    else
    {
        // not transmitting; everything in buffer was transmitted
        _is_transmitting = false;
    }

}


void IrPHY::set_baud(uint8_t baudrate)
{
    // TODO: this
    // required: global current clock frequency 
}
