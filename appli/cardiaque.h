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

void initBuffer(int16_t *tableau,int taille);
void Cardio_init(void);
void updateRingBuffer(int16_t *tableau,int taille,adc_id_e channel);
void addValue(int16_t *tableau,int taille,adc_id_e channel);
void getValeurMoyenne(int16_t *signal, int16_t taille);

void getBPM(int16_t *tableau,int16_t taille);
void recentreSignal(int16_t *tableau,int16_t taille);
#endif /* CARDIAQUE_H_ */
