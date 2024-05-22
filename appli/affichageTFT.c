
#include "affichageTFT.h"
uint16_t MAX_Plage = 3800;
uint16_t MIN_Plage = 3600;
void initTemplate(void){

	ILI9341_Init();
	//320,240
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(0xC618);

	//Cadre graphe respiratoire
	ILI9341_DrawFilledRectangle(20,220,170,140,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20,220,170,140,ILI9341_COLOR_BLACK);

	//Cadre graphe cardio
	ILI9341_DrawFilledRectangle(20,20,170,120,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20,20,170,120,ILI9341_COLOR_GREEN);

	//Cadre BPM
	ILI9341_DrawFilledRectangle(190,20,260,60,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(190,20,260,60,ILI9341_COLOR_BLACK);
	ILI9341_Puts(235,25, "BPM", &Font_7x10, ILI9341_COLOR_RED,ILI9341_COLOR_WHITE);

	//Voyant
	ILI9341_DrawFilledRectangle(270,20,300,50,ILI9341_COLOR_GREEN);
	ILI9341_DrawRectangle(270,20,300,50,ILI9341_COLOR_BLACK);

	//Cadre température
	ILI9341_DrawFilledRectangle(190,80,300,110,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(190,80,300,110,ILI9341_COLOR_BLACK);
	//ILI9341_Puts(195,87, "T :", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
	ILI9341_Puts(285,87, "C", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);

	//Cadre vitesse
	ILI9341_DrawFilledRectangle(190,130,300,160,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(190,130,300,160,ILI9341_COLOR_BLACK);
	//ILI9341_Puts(195,87, "T :", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
	ILI9341_Puts(270,142, "km/h", &Font_7x10, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);

	//Cadre nb de km parcouru
	ILI9341_DrawFilledRectangle(190,180,300,220,ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(190,180,300,220,ILI9341_COLOR_BLACK);
	ILI9341_Puts(270,190, "km", &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);

}

void static writeValueInt(int16_t value, uint16_t x,uint16_t y){

	//Affichage
	ILI9341_printf(x, y, &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE, "%d",value);
}
void static writeValueFloat(float value, uint16_t x,uint16_t y){

	//Affichage
	ILI9341_printf(x, y, &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE, "%0.2f",value);
}

void writeTemp(float value){
	writeValueFloat(value,211,87);
}

void writeCardio(int16_t value){
	writeValueInt(value,200,37);
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
void addValue(int16_t *tableau,int taille,adc_id_e channel){
	int static i = 0;
	int16_t newValue = getCardio(channel);
	if(newValue < MIN_Plage){
		newValue=0;
	}else if(newValue > MAX_Plage){
		newValue=MAX_Plage-MIN_Plage;
	}else {
		newValue-=MIN_Plage;
	}
	Delay_ms(10);


    	//int16_t tempon = tableau[i];
    	tableau[i] = newValue;
    	//newValue = tempon;
    	i++;
    if(i==taille) {
    	printCardioGraphe(tableau,taille);
    	i=0;
    }
}

void printCardioGraphe(int16_t *tableau, int taille){
	ILI9341_DrawFilledRectangle(21,21,169,119,ILI9341_COLOR_WHITE);
	uint16_t static yPrec = 119;
	uint16_t static y = 119;

	for(uint16_t i = 1; i < taille-2; i+=2){

		uint16_t x = 22+i;
		yPrec = y;

		uint16_t calibrageValeur = (uint16_t)(tableau[i]*98/(MAX_Plage-MIN_Plage));
		y = 119-calibrageValeur;
		if(tableau[i]!=-1) ILI9341_DrawLine((x==1)?0:x-2, yPrec, x, y, ILI9341_COLOR_RED);//ILI9341_DrawPixel(x,y,ILI9341_COLOR_RED);
	}
	//ILI9341_DrawRectangle(20,20,170,120,ILI9341_COLOR_GREEN);
}
