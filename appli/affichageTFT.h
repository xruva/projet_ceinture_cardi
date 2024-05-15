/*
 * affichage.h
 *
 *  Created on: Apr 26, 2024
 *      Author: vauraudren
 */

#ifndef AFFICHAGETFT_H_
#define AFFICHAGETFT_H_

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
//#include "stm32f1_uart.h"
//#include "stm32f1_sys.h"
//#include "macro_types.h"
//#include "stm32f1_gpio.h"
#include "stm32f1_adc.h"
//#include "MPU6050/stm32f1_mpu6050.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "tft_ili9341/stm32f1_xpt2046.h"



void initTemplate(void);

void writeTemp(float value);
void writeCardio(int16_t value);

void initBuffer(int16_t *tableau,int taille);
void updateRingBuffer(int16_t *tableau,int taille,adc_id_e channel);

void printCardioGraphe(int16_t *tableau, int taille);

#endif /* AFFICHAGETFT_H_ */
