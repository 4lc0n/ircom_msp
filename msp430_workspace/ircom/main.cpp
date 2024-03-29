/**
 * @brief 
 * 
 * @return int 
 */

/*
 Entr�mpeln notwendig!
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


// ====== GPIO OUTPUT CHANNELS ====== // 

#define NTC_SUP_PIN BIT0
#define NTC_SUP_DIR P3DIR
#define NTC_SUP_OUT P3OUT

#define EN_U_MEAS_PIN BIT5
#define EN_U_MEAS_DIR P3DIR
#define EN_U_MEAS_OUT P3OUT

#define EN_IR_SMD_PIN BIT7
#define EN_IR_SMD_DIR P3DIR
#define EN_IR_SMD_OUT P3OUT


// ====== ANALOG CHANNELS ====== // 

#define NTC_SUP_MEAS_PIN BIT2
#define NTC_SUP_MEAS_DIR P1DIR
#define NTC_SUP_MEAS_SEL0 P1SEL0
#define NTC_SUP_MEAS_SEL1 P1SEL1
#define NTC_SUP_MEAS_AINCH ADC12INCH_2

#define NTC_U_MEAS_PIN BIT1
#define NTC_U_MEAS_DIR P1DIR
#define NTC_U_MEAS_SEL0 P1SEL0
#define NTC_U_MEAS_SEL1 P1SEL1
#define NTC_U_MEAS_AINCH ADC12INCH_1

#define U_MEAS_PIN BIT0
#define U_MEAS_DIR P1DIR
#define U_MEAS_SEL0 P1SEL0
#define U_MEAS_SEL1 P1SEL1
#define U_MEAS_AINCH ADC12INCH_0


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
//char SendStr[21];
volatile uint16_t  adc_batt_u, adc_vcc_u;
volatile uint32_t time_of = 0;
volatile uint32_t adc_ntc_sup;
volatile uint64_t adc_ntc_u;
volatile uint64_t SendAddr;


float internalSupplyVoltage = 0;
float BattCellVoltage = 0;
float NTCSupplyVoltage = 0;
float BattNTCTemperature = 0;
float NTCVoltage = 0;
float NTC_R = 0;

uint16_t cwBatVolt = 0;
uint32_t cwNtcSup = 0;
uint64_t cwNtcVolt = 0;
uint64_t SendData = 0;


//Address
uint64_t DeviceAddress = *((uint64_t*)(0x01A30));

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


  // setup all enable pins

  NTC_SUP_DIR |= NTC_SUP_PIN;
  NTC_SUP_OUT |= NTC_SUP_PIN;

  EN_U_MEAS_DIR |= EN_U_MEAS_PIN;
  EN_U_MEAS_OUT |= EN_U_MEAS_PIN;

  EN_IR_SMD_DIR |= EN_IR_SMD_PIN;
  EN_IR_SMD_OUT &= ~(EN_IR_SMD_PIN);




  // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers


  // setup ADC 
  // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_2 | REFON;             // Select internal ref = 2.5V
                                            // Internal Reference ON

  // Configure ADC12
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
  ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // ADCCLK = MODOSC; sampling timer, do sequence of channels
  ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
  ADC12CTL3 |= ADC12BATMAP;                 // select battery monitor
  ADC12IER0 |= ADC12IE3;                    // Enable ADC conv complete interrupt
  ADC12MCTL0 = ADC12INCH_31 | ADC12VRSEL_0; // VBAT ADC input select; Vref= VCC
  ADC12MCTL1 = U_MEAS_AINCH | ADC12VRSEL_0; // A0: Voltage of battery
  ADC12MCTL2 = NTC_SUP_MEAS_AINCH | ADC12VRSEL_0; // A1: Voltage of NTC Supply
  ADC12MCTL3 = NTC_U_MEAS_AINCH | ADC12VRSEL_0 | ADC12EOS;   // A2: Voltage on NTC, end of sequence

  while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator
                                            // to settle



  // initialize timer for time measuring
    // setup Timer for time tracking
  TA0CTL |= TACLR;
  TA0CTL |= (TASSEL__ACLK) | MC__UP | ID__1;
  TA0CCR0 = 10000;
  TA0CCTL0 |= CCIE;

  // setup IRDA Pins

  P2SEL1 |= IRDA_RX_PIN | IRDA_TX_PIN;                    // USCI_A0 UART operation
  P2SEL0 &= ~(IRDA_RX_PIN | IRDA_TX_PIN);

  // initialize the IR interface
  microTP.init(&irPHY);



  uint8_t i = 0;

  while (1)
  {
    
     /*
     // calc internal supply voltage �C
      internalSupplyVoltage = adc_vcc_u / 4096.0 * 2.8 * 2;              // 2.8 = Vref = VCC

     // calc battery cell voltage
      BattCellVoltage = adc_batt_u / 4096.0 * 2.8 / 1.44 + 2.2;          // 1.44 = opv gain 2.2 = z-diode

      // calc battery cell temperature
      NTCSupplyVoltage = adc_ntc_sup / 4096.0 * 2.8;                                    // check ntc supply voltage with 2.8 V reference
      NTCVoltage = adc_ntc_u/4096.0 * 2.8;                                              // check ntc voltage with 2.8 V reference
      NTC_R = (-NTCVoltage * 1000.0) / (NTCVoltage - NTCSupplyVoltage);                 // calc NTC resistent
      BattNTCTemperature = 1 / ((1/3977.0) * log(NTC_R/2700.0) + (1/298.0)) -273.15;    // calc temperature
      */
    // build data
      cwBatVolt = adc_batt_u;
      cwNtcSup = adc_ntc_sup*10000;
      cwNtcVolt = adc_ntc_u*100000000;

      SendAddr = DeviceAddress;
      SendAddr = SendAddr - SendAddr%1000000000000;
      SendData = SendAddr+cwNtcVolt+cwNtcSup+cwBatVolt;
    // send string to UART interface
    //microTP.send((uint8_t*)str, strlen(str));
    microTP.send((uint8_t*)&SendData, sizeof(SendData));

    // start ADC conversion

    ADC12CTL0 |= ADC12ENC | ADC12SC;         // Sampling and conversion start


    //__delay_cycles(40e6);                   //8e6 = 1s; 40e6==5s

    __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled
    __no_operation();                         // For debugger

  }
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



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:        break;        // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG:  break;        // Vector  2:  ADC12MEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG:  break;        // Vector  6:  ADC12BHI
    case ADC12IV_ADC12LOIFG:  break;        // Vector  8:  ADC12BLO
    case ADC12IV_ADC12INIFG:  break;        // Vector 10:  ADC12BIN
    case ADC12IV_ADC12IFG0:   break;        // Vector 12:  ADC12MEM0 Interrupt
    case ADC12IV_ADC12IFG1:   break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:   break;        // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3:           // Vector 18:  ADC12MEM3
      adc_vcc_u = ADC12MEM0;
      adc_batt_u = ADC12MEM1;
      adc_ntc_sup = ADC12MEM2;
      adc_ntc_u = ADC12MEM3;
      // __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;        
    case ADC12IV_ADC12IFG4:   break;        // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5:   break;        // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6:   break;        // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7:   break;        // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8:   break;        // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9:   break;        // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10:  break;        // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11:  break;        // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12:  break;        // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13:  break;        // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14:  break;        // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15:  break;        // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16:  break;        // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17:  break;        // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18:  break;        // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19:  break;        // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20:  break;        // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21:  break;        // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22:  break;        // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23:  break;        // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24:  break;        // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25:  break;        // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26:  break;        // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27:  break;        // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28:  break;        // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29:  break;        // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30:  break;        // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31:  break;        // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;        // Vector 76:  ADC12RDY
    default: break;
  }
}


// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  
    time_of += 1000;
    __bic_SR_register_on_exit(LPM3_bits);
      
  

}
