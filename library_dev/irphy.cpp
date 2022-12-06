#include <stdint.h>

#include <msp430fr5969.h>

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
    // initialize hardware
    // Configure USCI_A0 for UART mode
    UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA1BR0 = 52;                             // 8000000/16/9600
    UCA1BR1 = 0x00;
    UCA1MCTLW |= UCOS16; // | UCBRF_1;

    // enable IrDA encoding
    // pulse duration defined by UCIRTXPLx bits, specifying the number of one half clock periods of the clock selected by UCIRTXCLK
    // to set the pulse time to 3/16 bit period required by the IrDA standard, the BITCLK16 clock is selected with UCIRTXCLK=1 and the pulse length
    // is set to six one-half clock cycles with UCIRTXPLx = 6-1 = 5

    // set pulse length, enalbe BITCLK16, enable encoder/decoder
    UCA1IRCTL |= (5 << 2) | (UCIRTXCLK) | (UCIREN);

    // set the minimum pulse length for receiving: 
    // set to pulse length at 115200 bit/s: 1.41 µs
    // calculation: tMIN = (UCIRRXFLx + 4) / [2 × fIRTXCLK]
    

    // uncomment the following line and see the result, the compiler will produce
    // for a  fIRTXCLK = SMCLK = 8MHz and 1.41µs = tMIN, result is 18
    // uint16_t tmp = (uint16_t)(1.41 * (2.0 * 8.0) - 4.0);
    UCA1IRCTL &= ~(0x3F << 10);
    UCA1IRCTL |= ((18 & 0x3F) << 10) | (UCIRRXFE);


    UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA1IE |= UCRXIE | UCTXIFG;               // Enable USCI_A0 RX interrupt and TX interrupt (on transmit done)
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
    if(output_buffer.storage_available() < len){
        while(!output_buffer.is_full() && ii < len)
        {
            output_buffer.put(data[ii]);
            ii++;
        }
        // return number of inserted bytes

        if (!_is_transmitting)
        {
            send_next_data();
        }
        return ii;
    }   
    else
    {
        // copy entire buffer
        for(ii = 0; ii < len; ii++)
        {
            output_buffer.put(data[ii]);
        }

        if (!_is_transmitting)
        {
            send_next_data();
        }
        return len;
    }
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

            notify_new_frame(transfer_buffer, i);

            
        }
        else{
            input_buffer.put(data);
        }

        break;
    
    default:
        break;
    }
   
    


        

}


void IrPHY::clear_intput_buffer()
{
    input_buffer.clear_buffer();
}



void IrPHY::send_next_data()
{
    _is_transmitting = true;
    if(!output_buffer.is_empty())
    {
        // send data
        char d = output_buffer.pop();
        
        UCA1TXBUF = d;
        
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