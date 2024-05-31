/*
 * respiration.c
 *
 *  Created on: May 31, 2024
 *      Author: vauraudren
 */

#include "respiration.h"
#include "affichageTFT.h"
int8_t flagVM=0;
void addRespi(int16_t *tableau,int taille,adc_id_e channel){
	int static i = 0;
	int16_t newValue = ADC_getValue(channel);
	tableau[i] = newValue;

	i++;
	if(i==taille){
		i=0;
		ILI9341_DrawFilledRectangle(21,219,169,141,ILI9341_COLOR_WHITE);
		uint16_t static yPrec = 219;
		uint16_t static y = 219;
		for(uint16_t i = 1; i < taille-2; i+=2){

				uint16_t x = 22+i;
				yPrec = y;

				uint16_t calibrageValeur = (uint16_t)(tableau[i]*78/4095);
				y = 219-calibrageValeur;
				if(tableau[i]!=-1) ILI9341_DrawLine((x==1)?0:x-2, yPrec, x, y, ILI9341_COLOR_BLUE);

				getValeurMoyenne(tableau,taille);
				flagVM = 1;
		}
	}
}

