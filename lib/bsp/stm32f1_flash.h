/*
 * stm32f1_flash.h
 *
 *  Created on: 11 juil. 2019
 *      Author: Nirgal
 */

#ifndef BSP_STM32F1_FLASH_H_
#define BSP_STM32F1_FLASH_H_

#include "config.h"
#if USE_INTERNAL_FLASH_AS_EEPROM


#define BASE_ADDRESS	0x0801FC00		//adresse du d�but de la derni�re page (2kBytes)
#define	SIZE_SECTOR		(1024/4)
#define PAGE_ADDRESS	127

uint32_t FLASH_read_word(uint32_t index);
void FLASH_set_word(uint32_t index, uint32_t data);
void FLASH_dump(void);

#endif
#endif /* BSP_STM32F1_FLASH_H_ */
