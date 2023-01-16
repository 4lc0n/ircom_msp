/**
 * @brief 
 * 
 * @return int 
 */

#include <cstdlib>
#include <ctype.h>
#include <msp430.h>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "board.h"
#include "MicroTP.hpp"
#include "irphy.hpp"




// ====== IRDA CHANNELS ====== //

#define IRDA_RX_PIN BIT6
#define IRDA_TX_PIN BIT5
#define IRDA_DIR P2DIR
#define IRDA_SEL0 P2SEL0
#define IRDA_SEL1 P2SEL1

#define F_CPU (8e6)



MicroTP microTP = {};
IrPHY irPHY = {};

const char* str = "Hello World\n";

volatile uint32_t time_of = 0;

void sendn_uart(uint8_t* data, uint16_t length) {
  for(uint16_t i = 0; i < length; i++){
      while(UCA0STATW & UCBUSY);
      UCA0TXBUF = data[i];
    }
}

void send_uart(const char* data) {
    uint16_t length = strlen(data);
  for(uint16_t i = 0; i < length; i++){
      while(UCA0STATW & UCBUSY);
      UCA0TXBUF = data[i];
    }
}

void configure_uart() {
  // initialize hardware
  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
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
  UCA0BRW = 4;
  UCA0MCTLW = (0x55 << 8) | (UCBRF_5) | UCOS16; // | UCBRF_1;
  UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
  // do not enable interrupt


}



int main() {

  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  // Configure GPIO
  
  P1DIR = 0;
  P2DIR = 0;
  P3DIR = 0;
  P4DIR = 0;
  PJDIR = 0;

  P1OUT = 0;
  P2OUT = 0;
  P3OUT = 0;
  P4OUT = 0;
  PJOUT = 0;


  

  // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers


 


  // initialize timer for time measuring
    // setup Timer for time tracking
  TA0CTL |= TACLR;
  TA0CTL |= (TASSEL__SMCLK) | TAIE | MC__CONTINOUS | ID__8;

  // setup IRDA Pins

  P2SEL1 |= IRDA_RX_PIN | IRDA_TX_PIN;                    // USCI_A0 UART operation
  P2SEL0 &= ~(IRDA_RX_PIN | IRDA_TX_PIN);

  // debug uart
  P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT0 | BIT0);

  // initialize the IR interface#

  microTP.init(&irPHY);
  configure_uart();


  send_uart("setup complete\n");

  char input_buffer[64];
  uint16_t input_length = 0;

  __bis_SR_register(GIE);       // Enter LPM3, interrupts enabled

  while(1) {
      microTP.tick();
      if(microTP.receive((uint8_t*)input_buffer, &input_length) == 0) {
        sendn_uart((uint8_t*)input_buffer, input_length);
      }


  }

  // __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled
  // __no_operation();                         // For debugger
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      // insert received data
      irPHY.put_received_data(UCA1RXBUF);
      break;
    case USCI_UART_UCTXIFG:
      // the last char was successfully transferred
      // send next data
      irPHY.send_next_data();
      break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}


// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA0IV, TA0IV_TAIFG))
  {
    case TA0IV_NONE:   break;               // No interrupt
    case TA0IV_TACCR1: break;               // CCR1 not used
    case TA0IV_TACCR2: break;               // CCR2 not used
    case TA0IV_3:      break;               // reserved
    case TA0IV_4:      break;               // reserved
    case TA0IV_5:      break;               // reserved
    case TA0IV_6:      break;               // reserved
    case TA0IV_TAIFG:                       // overflow
      time_of += 1000;
      break;
    default:          break;
  }

}
