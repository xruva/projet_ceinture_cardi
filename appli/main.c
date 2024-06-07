#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
//#include "stm32f1_gpio.h"
#include "stm32f1_adc.h"
#include "MPU6050/stm32f1_mpu6050.h"
#include "stm32f1_rtc.h"
#include "affichageTFT.h"
#include "mpc9701.h"
#include "cardiaque.h"
#include "respiration.h"

#include <stdio.h>
#include <math.h>

//Pota
#define POT_GPIO				GPIOA
#define POT_PIN					GPIO_PIN_4
#define ADC_POT ADC_11



#define TAILLE 150
#define NBMSG 4
#define TAILLEMSG 20 //doit être un multiple de 4





void addMsg(char* pMsg);
//void addRespi(void);
void setMsgFlash(void);
void getMsgFlash(void);
int16_t cardiographe[TAILLE];
int16_t aerographe[TAILLE];


char tabMsg[NBMSG][TAILLEMSG]={"","","",""};
MPU6050_t datas;


int main(void)
{

	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	ADC_init();
	//RTC_init(FALSE);

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);


	//Initialisation du port du pota en sortie Push-Pull
	BSP_GPIO_PinCfg(POT_GPIO, POT_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM);


	//Initialisation de la liaision I2C, ainsi que l'acc�l�rom�tre
	MPU6050_Init(&datas, GPIOA, GPIO_PIN_5, MPU6050_Device_0,MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);
	//MPU6050_demo();

	initTemplate();
	Cardio_init();
	//Delay_ms(1000);
	getMsgFlash();
	ILI9341_Puts(200,140, tabMsg[0], &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,140+(22), tabMsg[1], &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,140+(44), tabMsg[2], &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,140+(66), tabMsg[3], &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);





	int32_t updateGraphiqueCardiaque = HAL_GetTick();
	int32_t updateTemp = HAL_GetTick();
	int32_t updatePosition = HAL_GetTick();
	int32_t updateDetectionChute = HAL_GetTick();
	int32_t updateRespi = HAL_GetTick();
	int32_t colorChute = ILI9341_COLOR_GREEN;
	int32_t updateDetectionApnee = HAL_GetTick();
	int8_t attenteChangement =0;

	int8_t flagStart =0;

	while(!flagStart){
		if(HAL_GPIO_ReadPin(BP_GPIO,BP_PIN)) flagStart=1;
		for(int16_t i =0 ; i<255;i++) printf("%d\n",FLASH_read_word(i));
		printf("fin !");
	}

	while(1)
	{



//-------------------------------------------------Récupération des données-------------------------------------------------




		if (HAL_GetTick()>=updatePosition+100){
			MPU6050_ReadAll(&datas);
			updatePosition = HAL_GetTick();
		}


		if (HAL_GetTick()>=updateGraphiqueCardiaque+20){

			addValue(cardiographe,TAILLE,ADC_CAR);
			updateGraphiqueCardiaque = HAL_GetTick();
		}

		if (HAL_GetTick()>=updateRespi+50){

			addRespi(aerographe,TAILLE,ADC_POT);
			updateRespi = HAL_GetTick();
		}

		if(HAL_GetTick()>=updateTemp+1000){
			float temp = getTemp(ADC_TEMP);
			writeTemp(temp);
			updateTemp= HAL_GetTick();
		}


//-------------------------------------------------Traitement des données-------------------------------------------------


		//		Detection de chute


		if(((datas.Accelerometer_Y<2000) && (datas.Accelerometer_Y>-2000)) && ((datas.Accelerometer_Z<-2000) || (datas.Accelerometer_Z>2000))){
			if((HAL_GetTick()>=updateDetectionChute+7000) && (colorChute != ILI9341_COLOR_RED)){
				colorChute = ILI9341_COLOR_RED;
				ILI9341_DrawFilledRectangle(271,21,299,49,colorChute);
				addMsg("Alerte chute");
				setMsgFlash();

			}
		}else{
			if(colorChute!=ILI9341_COLOR_GREEN){
				colorChute = ILI9341_COLOR_GREEN;
				ILI9341_DrawFilledRectangle(271,21,299,49,colorChute);

			}
			updateDetectionChute = HAL_GetTick();

		}


		//		Detection d'apnée

		if(flagVM!=0){

			if((checkVariation(aerographe, TAILLE, 100)==0)
					&& (HAL_GetTick() >= updateDetectionApnee+3000)
					&& (!attenteChangement)){
				addMsg("Alerte respi");
				attenteChangement =1;
				setMsgFlash();

			}else if((checkVariation(aerographe, TAILLE, 100)!=0)){
				updateDetectionApnee = HAL_GetTick();
				attenteChangement =0;
			}
			flagVM=0;
		}



	}
}



//--------------fonctions--------------

void addMsg(char* pMsg){
	char newAlerte[20];
	sprintf(newAlerte,"%s",pMsg);

    for(int i = 0; i < NBMSG ;i++){
    	if(i==3) sprintf(tabMsg[3],"%s",pMsg);
    	else sprintf(tabMsg[i],"%s",tabMsg[i+1]);
    	ILI9341_Puts(200,140+(i*22), tabMsg[i], &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);

    }

}


void setMsgFlash(void){
	int32_t compression = 0x0;

	for(int8_t i = 0 ; i < NBMSG ; i++){
		for(int8_t j = 0 ; j < TAILLEMSG ; j++){
			double result = (double)j / 4;
			double intPart;

			//Stockage de 4 char dans un int32_t
			compression |= (int32_t)tabMsg[i][j] << (int32_t)((modf(result, &intPart)*32));
			if((j+1)%4==0){
				FLASH_set_word((j/4)+(i*TAILLEMSG),compression);
				compression = 0x0;
			}
		}
	}
}

void getMsgFlash(void){
	int32_t decompression;

	for(int8_t i = 0 ; i < NBMSG ; i++){
		for(int8_t j = 0 ; j < TAILLEMSG ; j++){
			double result = (double)j / 4;
			double intPart;

			if(j%4==0) decompression = FLASH_read_word((i*TAILLEMSG)+j/4);
			//Récupération des 4 char
		    tabMsg[i][j] = (char)((decompression >> (int32_t)((modf(result, &intPart)*32))) & 0xFF);

		}
	}
}
