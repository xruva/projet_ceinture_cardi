#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
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
#define NBMSG 4


typedef enum {
    CHUTE,
    COEUR,
	RESPI

} StringAlerte;

typedef struct{
int32_t heure;
int32_t minute;

StringAlerte msg;
}dataAlerte;

int16_t getAmpliResp(adc_id_e channel);
void addMsg(int32_t temps, char* pMsg);
int16_t cardiographe[TAILLE];
//dataAlerte tabMsgErreur[5];

char tabMsg[NBMSG][20]={"                    ","                    ","                    ","                    "};

MPU6050_t datas;


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


	//Initialisation du port du pota en sortie Push-Pull
	BSP_GPIO_PinCfg(POT_GPIO, POT_PIN, GPIO_MODE_INPUT,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM);

	//Initialisation de la liaision I2C, ainsi que l'acc�l�rom�tre
	MPU6050_Init(&datas, GPIOC, GPIO_PIN_0, MPU6050_Device_0,MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);
	//MPU6050_demo();

	initTemplate();
	Cardio_init();






	int32_t updateGraphiqueCardiaque = HAL_GetTick();
	int32_t updateTemp = HAL_GetTick();
	int32_t updatePosition = HAL_GetTick();
	int32_t updateDetectionChute = HAL_GetTick();
	int16_t colorChute = ILI9341_COLOR_GREEN;

	while(1)
	{
		//Récupération des données
		if (HAL_GetTick()>=updatePosition+100){
			MPU6050_ReadAll(&datas);
			updatePosition = HAL_GetTick();
			printf("Ax : %4d | Ay : %4d | Az : %4d\n",datas.Accelerometer_X,datas.Accelerometer_Y,datas.Accelerometer_Z);
		}


		if (HAL_GetTick()>=updateGraphiqueCardiaque+20){

			addValue(cardiographe,TAILLE,ADC_CAR);
			updateGraphiqueCardiaque = HAL_GetTick();
		}


		if(HAL_GetTick()>=updateTemp+1000){
			float temp = getTemp(ADC_TEMP);
			writeTemp(temp);
			updateTemp= HAL_GetTick();
		}


		//Traitement des données

		//Detection de chute
		if(((datas.Accelerometer_Y<2000) && (datas.Accelerometer_Y>-2000)) && ((datas.Accelerometer_Z<-2000) || (datas.Accelerometer_Z>2000))){
			if((HAL_GetTick()>=updateDetectionChute+7000) && (colorChute != ILI9341_COLOR_RED)){
				colorChute = ILI9341_COLOR_RED;
				ILI9341_DrawFilledRectangle(271,21,299,49,colorChute);
				updateDetectionChute = HAL_GetTick();
				addMsg(HAL_GetTick(),"Chute");
			}
		}else{
			if(colorChute!=ILI9341_COLOR_GREEN){
				colorChute = ILI9341_COLOR_GREEN;
				ILI9341_DrawFilledRectangle(271,21,299,49,colorChute);
			}
			updateDetectionChute = HAL_GetTick();
		}

		//
	}
}


int16_t getAmpliResp(adc_id_e channel){
	int16_t input = ADC_getValue(channel);

	return input;
}

void addMsg(int32_t temps, char* pMsg){
	char newAlerte[20];
	sprintf(newAlerte,"%s",pMsg);

	//horodatage
    uint32_t total_seconds = temps / 1000;
    uint32_t seconds = total_seconds % 60;
    uint32_t total_minutes = total_seconds / 60;
    uint32_t minutes = total_minutes % 60;
    uint32_t hours = total_minutes / 60;


    for(int i = 0; i < NBMSG-1 ;i++){
    	sprintf(tabMsg[i],"%s",tabMsg[i+1]);

    	}
    sprintf(tabMsg[3],"%d h %d - %s",hours,minutes,pMsg);
}

void printMsg(dataAlerte *tabMsg){
	//Afficher rectangle blanc
	for(int i = 0 ; i<5 ; i++){
		//afficher heure et minute x, y-i*15
		//afficher msg x1, y1-i*15
	}
}
