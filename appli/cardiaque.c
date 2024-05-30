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

#define DELTAFM 5
#define DELTAFD 3


adc_id_e ADC_CAR = ADC_0;

int valeurMoyenne = 0;
int16_t new_signal[150];

typedef struct {
    int16_t max;
    int16_t moy;
    int16_t min;
    int16_t bpm;
} Signal;

Signal sig = {0, 0, 0,0};  // Déclaration d'une instance globale de la structure Signal


void Cardio_init(void)
{
	//Initialisation du port du capteur cardiaque en sortie
	BSP_GPIO_PinCfg(CAR_GPIO, CAR_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
}

void addValue(int16_t *tableau,int taille,adc_id_e channel){
	int static i = 0;
	int16_t newValue = ADC_getValue(channel);
	//printf("1");

	if(newValue < MIN_Plage){
		newValue = 0;
	}else if(newValue > MAX_Plage){
		newValue = MAX_Plage-MIN_Plage;
	}else {
		newValue -= MIN_Plage;
	}
	//Delay_ms(10);
	//printf("2");

    	tableau[i] = newValue;

    	i++;
    	if(i==taille){
    		i=0;
    		//printf("3");

    		getValeurMoyenne(tableau,taille);
    		printCardioGraphe(tableau,taille);
    		writeBPM(getBPM(tableau,taille));
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

void getValeurMoyenne(int16_t *signal, int16_t taille) {
    sig.max = signal[0];
    sig.min = signal[0];
    int32_t sum = 0;

    for (int16_t i = 0; i < taille; i++) {
        if (signal[i] > sig.max) sig.max = signal[i];
        if (signal[i] < sig.min) sig.min = signal[i];
        sum += signal[i];
    }
    sig.moy = (int16_t)(sum / taille);
}

void getBPM(int16_t *tableau,int16_t taille){

	int16_t tempsPic[2]= {0,0};
	int16_t indexPic = 0;
	for(int16_t index; ((index < taille - 11) && (indexPic<2)) ; index++){
		if ((tableau[index] > sig.moy + DELTAFM) && (tableau[index + 10] <= sig.moy - DELTAFD)){
			tempsPic[indexPic] = index;
			indexPic++;
			index +=10;
		}
	}

	if(indexPic == 2) sig.bpm = (int16_t)(60000/((tempsPic[1]-tempsPic[0])*20));
	else sig.bpm = -1;

}

void recentreSignal(int16_t *tableau,int16_t taille){
	int static i = 0;

	int16_t plageMax = sig.max+10;
	int16_t plageMin = sig.min-10;

	//int16_t *new_signal = (int16_t*)malloc(taille * sizeof(int16_t));

	for(int16_t i ; i < taille ; i++){
		if(tableau[i] < plageMin){
			new_signal[i] = 0;
			}else if(tableau[i] > plageMax){
				new_signal[i] = plageMax-plageMin;
			}else {
				new_signal[i] = tableau[i] - plageMin;
			}
	}

	printCardioGraphe(new_signal, taille);

}

int8_t checkVariation(int16_t *tableau, int16_t taille, int16_t delta){
	int8_t result = 1;
	for(int16_t i = 0 ; i < taille ; i++){
		result *=((tableau[i]<sig.moy+delta) && (tableau[i]>sig.moy-delta));
	}
	return result;
}
