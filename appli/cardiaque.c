/*
 * cardiaque.c
 *
 *  Created on: May 22, 2024
 *      Author: vauraudren
 */
#include "affichageTFT.h"

//Cardio
#define CAR_GPIO				GPIOA
#define CAR_PIN					GPIO_PIN_0
adc_id_e ADC_CAR = ADC_0;

int valeurMoyenne = 0;


void Cardio_init(void)
{
	//Initialisation du port du capteur cardiaque en sortie
	BSP_GPIO_PinCfg(CAR_GPIO, CAR_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
}

void addValue(int16_t *tableau,int taille,adc_id_e channel){
	int static i = 0;
	int static somme = 0;
	int16_t newValue = ADC_getValue(channel);
	//volatile int16_t min = newValue;
	//volatile int16_t max = newValue;

	valeurMoyenne += newValue;
	if(newValue < MIN_Plage){
		newValue = 0;
	}else if(newValue > MAX_Plage){
		newValue = MAX_Plage-MIN_Plage;
	}else {
		newValue -= MIN_Plage;
	}
	//Delay_ms(10);
	somme += newValue;

    	//int16_t tempon = tableau[i];
    	tableau[i] = newValue;
    	//newValue = tempon;
    	i++;
    	if(i==taille){
    		i=0;
    		printCardioGraphe(tableau,taille);
    		writeBPM(getBPM(tableau,taille));

    		valeurMoyenne= somme/taille;
    		somme = 0;
    	}


}
//Partie graphe
void initBuffer(int16_t *tableau,int taille){
	for(int i = 0; i < taille; i++){
		tableau[i] = -1;
	}
}

void updateRingBuffer(int16_t *tableau,int taille,adc_id_e channel) {
	int16_t newValue = getCardio(channel);
	if(newValue < MIN_Plage){
		newValue=0;
	}else if(newValue > MAX_Plage){
		newValue=MAX_Plage-MIN_Plage;
	}else {
		newValue-=MIN_Plage;
	}

    for (int i = 0; i < taille; i++) {
    	int16_t tempon = tableau[i];
    	tableau[i] = newValue;
    	newValue = tempon;

    }
}
