/*
 * hydroponic_io.c
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#include "hydroponic_io.h"


extern void ESP_UART_ReceiveHandler(void);
extern void mqtt_receive_handler(void);

void esp_uart_send_bytes(uint8_t* array, size_t size){

	for(int i=0;i<size;i++)
	{
		ESP_UART_HANDLE->TDR = *array++;
		while(!(ESP_UART_HANDLE->ISR & (1<<6)));		// wait for transmit register(TC) to set.
	}
/* with HAL drivers-----------------------*/
//	HAL_UART_Transmit(&huart1, messageArray, strlen((char*)messageArray), HAL_MAX_DELAY);
}

uint8_t esp_uart_receive_byte(void){

	return ESP_UART_HANDLE->RDR;
/* with HAL drivers-----------------------*/
//	uint8_t buffer[10];
//
//	HAL_UART_Receive(&huart1, &buffer, 1, HAL_MAX_DELAY);
//
//	return buffer[0];
}

void ESP_UART_IRQHandler(void)
{
	  if(ESP_UART_HANDLE->ISR & (1<<5))			// rx interrupt
	  {
		 ESP_UART_ReceiveHandler(); 		// ESP receive handler function.
		 mqtt_receive_handler();
	  }
}

/*
 * TODO: Implement these functions when you decide which sensors you will use.
 */
float get_pH(void){

	return 0;
}

float get_conductivity(void){

	return 0;
}

float get_temperature(void){

	return 0;
}

int get_humidity(void){

	return 0;
}
