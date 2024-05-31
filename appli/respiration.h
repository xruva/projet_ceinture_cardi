/*
 * respiration.h
 *
 *  Created on: May 31, 2024
 *      Author: vauraudren
 */

#ifndef RESPIRATION_H_
#define RESPIRATION_H_

#include "macro_types.h"
#include "stm32f1_adc.h"
extern int8_t flagVM;

void addRespi(int16_t *tableau,int taille,adc_id_e channel);

#endif /* RESPIRATION_H_ */
