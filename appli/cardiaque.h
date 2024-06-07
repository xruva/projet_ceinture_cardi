/*
 * cardiaque.h
 *
 *  Created on: May 22, 2024
 *      Author: vauraudren
 */

#ifndef CARDIAQUE_H_
#define CARDIAQUE_H_

extern int valeurMoyenne;
extern adc_id_e ADC_CAR;

void Cardio_init(void);
void addValue(int16_t *tableau,int taille,adc_id_e channel);
void getValeurMoyenne(int16_t *signal, int16_t taille);
int8_t checkVariation(int16_t *tableau, int16_t taille, int16_t delta);


void getBPM(int16_t *tableau,int16_t taille);
#endif /* CARDIAQUE_H_ */
