/*
 * stm32f1_flash.c
 *
 *  Created on: 11 juil. 2019
 *      Author: Nirgal
 */


/*
 * Ce module logiciel se propose d'utiliser le dernier secteur de la m�moire FLASH pour enregistrer des variables non volatiles, utiles � votre application.
 *
 * Limitation de la version actuelle (qui se veut assez simplifi�e) :
 * - �criture d'entiers sur 32 bits seulement
 * - 256 entiers possibles
 *
 * ATTENTION, ce module ne fonctionne (en l'�tat) que sur un STM32F103RBT6 (dot� de 128KBytes !)
 * ATTENTION, Pour utiliser ce module logiciel, le fichier LinkerScript.ld doit limiter l'espace utilis� par le programme � 127K sur les 128K disponibles.
 *
 */
#include "config.h"
#include "stm32f1_flash.h"
#if USE_INTERNAL_FLASH_AS_EEPROM
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "macro_types.h"


static void FLASH_write_word(uint32_t index, uint32_t data);
static void FLASH_erase_everything_else(uint32_t index);
static void FLASH_erase(void);
extern void FLASH_PageErase(uint32_t PageAddress);

/*
 * @brief	Fonction de d�mo permettant de se familiariser avec les fonctions de ce module logiciel.
 * @brief	Cette fonction consulte et incr�mente le nombre inscrit dans la premi�re case. Puis, elle lance un affichage du contenu complet.
 * @pre	ATTENTION : ne pas appeler cette fonction en t�che de fond. Risque d'endommager la flash en cas d'�critures trop nombreuses.
 *
 */
void FLASH_demo(void)
{
	uint32_t current_word;
	current_word = FLASH_read_word(0);
	printf("mot pr�sent dans la case 0 : %ld = 0x%lx\n", current_word, current_word);
	current_word++;
	FLASH_set_word(0, current_word);
	current_word = FLASH_read_word(0);
	printf("mot pr�sent dans la case 0 apr�s incr�mentation : %ld = 0x%lx\n", current_word, current_word);
	FLASH_dump();
}

/*
 * @brief	Enregistre une donn�e dans la case souhait�e, sans toucher aux autres cases
 * @param  	index : Num�ro de la case (de 0 � 255).
 * @post  	ATTENTION : si la case est d�j� occup�e par une donn�e diff�rente de 0xFFFFFFFF (valeur par d�faut apr�s effacement), une sauvegarde compl�te du secteur est faite, puis un effacement, puis une restitution !
 * @post  	le temps d'ex�cution de cette fonction peut nettement varier !
 * @pre		//ATTENTION : ne pas appeler cette fonction trop fr�quemment. Risque d'endommager la flash en cas d'�critures trop nombreuses. (>10000 sur le cycle de vie complet du produit)
 */
void FLASH_set_word(uint32_t index, uint32_t data)
{
	uint32_t current_word;
	assert(index < SIZE_SECTOR);
	current_word = FLASH_read_word(index);
	if((current_word & data) != data)	//il n'est pas possible d'�crire le mot sans �tre pollu� par des z�ros qui seraient d�j� �crit ici
		FLASH_erase_everything_else(index);

	FLASH_write_word(index, data);
}

/*
 * @brief	Lit une donn�e situ�e dans la case souhait�e.
 * @param	index : Num�ro de la case (de 0 � 255).
 */
uint32_t FLASH_read_word(uint32_t index)
{
	uint32_t res = 0;
	assert(index < SIZE_SECTOR);

	uint16_t * p;
	p = (uint16_t *)(BASE_ADDRESS + 4*index);
	res = (uint32_t)(*p);
	p++;
	res |= ((uint32_t)(*p))<<16;

	return res;
}

/*
 * @brief Cette fonction affiche les SIZE_SECTOR donn�es (32 bits) disponibles dans le dernier secteur de la FLASH
 */
void FLASH_dump(void)
{
	uint32_t index;
	uint32_t v;
	printf("Affichage des %d donn�es (32 bits) disponibles dans le dernier secteur de la FLASH\n", SIZE_SECTOR);
	for(index = 0; index<SIZE_SECTOR; index++)
	{
		v = FLASH_read_word(index);
		printf("@%03ld : 0x%08lx = %ld\n", index, v, v);
	}
}



//______________________________________________________________________/
//___Fonctions priv�es__________________________________________________/

static void FLASH_erase_everything_else(uint32_t index)
{
	uint32_t saved_values[SIZE_SECTOR];
	uint32_t i;
	assert(index < SIZE_SECTOR);
	for(i=0; i<SIZE_SECTOR; i++)
		saved_values[i] = FLASH_read_word(i);
	FLASH_erase();
	for(i=0; i<SIZE_SECTOR; i++)
	{
		if(i!=index && saved_values[i]!=0xFFFFFFFF)
			FLASH_write_word(i, saved_values[i]);	//on r��crit tout sauf le mot
	}
}

static void FLASH_erase(void)
{
	HAL_FLASH_Unlock();
	FLASH_PageErase(BASE_ADDRESS);
	 /* Wait for last operation to be completed */
	FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

	/* If the erase operation is completed, disable the PER Bit */
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	HAL_FLASH_Lock();
}


static void FLASH_write_word(uint32_t index, uint32_t data)
{
	assert(index < SIZE_SECTOR);
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, BASE_ADDRESS+4*index, (uint64_t)(data));
	HAL_FLASH_Lock();
}

/*
 * fonction qui tue une case de la flash � petit feu...
 * ne pas utiliser...
 * un test de cette fonction en juillet 2019 a r�v�l� que la flash pouvait tenir 1,65 millions d'�critures avant de flancher.
 * attention � ne pas consid�rer ce test comme une preuve formelle... les fabriquants prennent des pr�cautions nettement plus restrictives, et conseillent de ne pas d�passer 10 000 �critures sur le m�me emplacement.
 */
void FLASH_kill(void)
{
	uint32_t nb_cycles = 0;
	uint32_t read;
	while(1)
	{
		FLASH_erase();
		read = FLASH_read_word(0);
		if(read != 0xFFFFFFFF)
			printf("fail to erase - nb_cycles = %ld ! - read 0x%lx\n", nb_cycles, read);
		FLASH_write_word(0, 0x55555555);
		read = FLASH_read_word(0);
		if(read != 0x55555555)
			printf("fail to write - nb_cycles = %ld ! - read 0x%lx\n", nb_cycles, read);
		else
		{
			nb_cycles++;
			if(nb_cycles % 1000 == 0)
				printf("%ld\n",nb_cycles);
		}
	}

}

#endif
