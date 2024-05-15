/*
 * mpc9701.h
 *
 *  Created on: Apr 26, 2024
 *      Author: vauraudren
 */

#ifndef MPC9701_H_
#define MPC9701_H_
#include "stm32f1_adc.h"
#include "config.h"

#define ADC_TEMP ADC_1

float getTemp(adc_id_e channel);

#endif /* MPC9701_H_ */
