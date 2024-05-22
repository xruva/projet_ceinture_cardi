#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
//#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
//#include "macro_types.h"
//#include "stm32f1_gpio.h"
#include "stm32f1_adc.h"
#include "MPU6050/stm32f1_mpu6050.h"
#include "affichageTFT.h"
#include "mpc9701.h"
#include "cardiaque.h"

//Pota
#define POT_GPIO				GPIOC
#define POT_PIN					GPIO_PIN_1
#define ADC_POT ADC_11



#define TAILLE 150



int16_t getAmpliResp(adc_id_e channel);

int16_t cardiographe[TAILLE];
int getBPM(int16_t *tableau);


int Threshold = 2500;       // Determine which Signal to "count as a beat" and which to ignore.
int Wait = 0;
int Wait1 = 0;
uint16_t static tututu = 0;



int main(void)
{

	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	ADC_init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);


	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);


	//Initialisation du port du pota en sortie Push-Pull
	BSP_GPIO_PinCfg(POT_GPIO, POT_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM);


	initTemplate();
	Cardio_init();



	//Initialisation de la liaision I2C, ainsi que l'acc�l�rom�tre
	//MPU6050_Init(&datas, GPIOA, GPIO_PIN_0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);



	int32_t updateGraphiqueCardiaque = HAL_GetTick();
	int32_t updateTemp = HAL_GetTick();

	while(1)
	{

		if (HAL_GetTick()>=updateGraphiqueCardiaque+20){
			tututu = ADC_getValue(ADC_POT);//getAmpliResp(ADC_POT);
			tututu = ADC_getValue(ADC_POT);//getAmpliResp(ADC_POT);  //
			tututu = ADC_getValue(ADC_POT);//getAmpliResp(ADC_POT);  //
			tututu = ADC_getValue(ADC_POT);//getAmpliResp(ADC_POT);  //
			Delay_ms(1);
			addValue(cardiographe,TAILLE,ADC_CAR);
			updateGraphiqueCardiaque = HAL_GetTick();
		}


		if(HAL_GetTick()>=updateTemp+1000){
			float temp = getTemp(ADC_TEMP);
			writeTemp(temp);
			updateTemp= HAL_GetTick();
		}

	}
}


int16_t getAmpliResp(adc_id_e channel){
	int16_t input = ADC_getValue(channel);

	return input;
}



/*
int getBPM(int16_t *tableau,int taille){
	bool FM = false;
	bool FD = false;
	int borneFM = valeurMoyenne+5;
	int borneFD = valeurMoyenne-3;
	for(int i; i<taille;i++){
		if(tableau[i]>(valeurMoyenne+5))
	}
}*/
