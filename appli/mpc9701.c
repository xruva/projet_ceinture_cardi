/*
 * mpc9701.c
 *
 *  Created on: Apr 26, 2024
 *      Author: vauraudren
 */
#include "mpc9701.h"

float getTemp(adc_id_e channel){

	int16_t valeurAnalogique = 0;
	int16_t valeurTemporaire = 0;


	//Phase d'�chantillonage
	for (int i = 0; i < 30;i++){
		valeurTemporaire = ADC_getValue(channel);
		if(i>4){
			valeurAnalogique += valeurTemporaire;
		}
	}

	float echantillonage = valeurAnalogique / 25;


    // Convertir la valeur analogique en tension (en volts)
    float tension = echantillonage / 4095.0 * 3.3;

    // Convertir la tension en temp�rature en degr�s Celsius (sensibilit� de 19,5 mV/�C)
    float temperature = ((tension) * 19.53);

    return temperature;

}
