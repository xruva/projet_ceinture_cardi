/*
 * cardiaque.h
 *
 *  Created on: May 22, 2024
 *      Author: vauraudren
 */

#ifndef CARDIAQUE_H_
#define CARDIAQUE_H_

extern int valeurMoyenne;

void initBuffer(int16_t *tableau,int taille);
void updateRingBuffer(int16_t *tableau,int taille,adc_id_e channel);
void addValue(int16_t *tableau,int taille,adc_id_e channel);

#endif /* CARDIAQUE_H_ */
