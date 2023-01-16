/*
 * board.h
 *
 *  Created on: 3 Jan 2023
 *      Author: h3l
 */

#ifndef BOARD_H_
#define BOARD_H_

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
#define IRDA_OUT P2OUT



#define F_CPU (1e6)


#endif /* BOARD_H_ */
