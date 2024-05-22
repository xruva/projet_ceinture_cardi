/*
 * mpc9701.c
 *
 *  Created on: Apr 26, 2024
 *      Author: vauraudren
 */
#include "mpc9701.h"

float getTemp(adc_id_e channel){

	int16_t valeurAnalogique = 0;

	//Phase d'échantillonage
	for (int i = 0; i < 20;i++){
		valeurAnalogique += ADC_getValue(channel);
	}

	float echantillonage = valeurAnalogique / 20;


    // Convertir la valeur analogique en tension (en volts)
    float tension = echantillonage / 4095.0 * 5;

    // Convertir la tension en température en degrés Celsius (sensibilité de 19,5 mV/°C)
    float temperature = (tension) * 19.53;

    return temperature;

}
