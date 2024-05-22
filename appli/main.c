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

#define ADC_CAR ADC_0


//Cardio
#define CAR_GPIO				GPIOA
#define CAR_PIN					GPIO_PIN_0

#define TAILLE 150


int16_t getCardio(adc_id_e channel);
int16_t cardiographe[TAILLE];


int Threshold = 2500;       // Determine which Signal to "count as a beat" and which to ignore.
int Wait = 0;
int Wait1 = 0;

// VARIABLES USED TO DETERMINE SAMPLE JITTER & TIME OUT
volatile unsigned int eventCounter, thisTime, lastTime, elapsedTime;
volatile int sumJitter, firstTime, secondTime, duration;
volatile int jitter;
unsigned int timeOutStart, dataRequestStart, m;
// VARIABLES USED TO DETERMINE BPM
volatile int Signal;
volatile unsigned int sampleCounter;
volatile int threshSetting,lastBeatTime,fadeLevel;
volatile int thresh = 550;      // SHOULD BE ADJUSTABLE ON COMMAND LINE
volatile int P = 512;           // set P default
volatile int T = 512;           // set T default
volatile int firstBeat = 1;     // set these to avoid noise
volatile int secondBeat = 0;    // when we get the heartbeat back
volatile int QS = 0;
volatile int rate[10];
volatile int BPM = 0;
volatile int IBI = 600;         // 600ms per beat = 100 Beats Per Minute (BPM)
volatile int Pulse = 0;         // set to 1 while Signal > Threshold
volatile int amp = 100;         // beat amplitude 1/10 of input range
volatile int sampleFlag = 0;

// FILE STUFF
char rawDatafilename [100];

struct tm *timenow;
// FUNCTION PROTOTYPES
void getPulse(void);
void initPulseSensorVariables(void);


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


	initTemplate();
	Cardio_init();
	initBuffer(&cardiographe,TAILLE);
	initPulseSensorVariables();

	//Initialisation de la liaision I2C, ainsi que l'acc�l�rom�tre
	//MPU6050_Init(&datas, GPIOA, GPIO_PIN_0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);


	int Wait2=0;
	int wait3=0;
	int static updateGraphiqueCardiaque = -1;
	while(1)
	{
		//getPulse2();
		uint16_t toto = getCardio(ADC_CAR);
		//updateRingBuffer(&cardiographe,TAILLE,ADC_CAR);
		//printCardioGraphe(&cardiographe,TAILLE);

		if (updateGraphiqueCardiaque ==-1) updateGraphiqueCardiaque = HAL_GetTick();
		if (HAL_GetTick()>=updateGraphiqueCardiaque+10){
			addValue(cardiographe,TAILLE,ADC_CAR);
			updateGraphiqueCardiaque = HAL_GetTick();
		}
/*		if(wait3==0){
			printCardioGraphe(&cardiographe,TAILLE);
			wait3=150;
		}else{
			wait3--;
		}*/


//		MPU6050_ReadAll(&datas);
//		if(datas.Accelerometer_X <0) writeLED(0);
//		else writeLED(1);

		//getTemp(ADC_TEMP);
		/*
		if (index==(TAILLE+1)) index=0;
		cardiographe[index] = ADC_getValue(ADC_0);

		if (index==TAILLE) cardiographe[0] = -1;
		else cardiographe[index+1] = -1;
		*/

		if(Wait2==0){
			writeTemp(getTemp(ADC_TEMP));
			//Réduction du temps de rafraichissement
			Wait2 = 20;
		}else{
			Wait2--;
		}
/*
		if(Wait1==0){

			writeCardio(getCardio(ADC_CAR));
			//Réduction du temps de rafraichissement
			Wait1 = 100000;
				}else{
					Wait1--;
				}
*/

		/*		Signal = getCardio(ADC_CAR);

		if(Signal > Threshold){                // If the signal is above threshold, turn on the LED
			ILI9341_DrawFilledRectangle(270,20,300,50,ILI9341_COLOR_GREEN);
			ILI9341_DrawRectangle(270,20,300,50,ILI9341_COLOR_BLACK);
			} else {
				ILI9341_DrawFilledRectangle(270,20,300,50,ILI9341_COLOR_RED);
				ILI9341_DrawRectangle(270,20,300,50,ILI9341_COLOR_BLACK);
			}
			HAL_Delay(10) ;
			*/

//		if((value>2045)readButton())
//else,
//			writeLED(1);
	}
}

int16_t getCardio(adc_id_e channel){
	int16_t value = ADC_getValue(channel);

	return value;
}



void Cardio_init(void)
{
	//Initialisation du port du capteur cardiaque en sortie Push-Pull
	BSP_GPIO_PinCfg(CAR_GPIO, CAR_PIN, GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	HAL_GPIO_WritePin(CAR_GPIO, CAR_PIN, GPIO_PIN_SET);
}
