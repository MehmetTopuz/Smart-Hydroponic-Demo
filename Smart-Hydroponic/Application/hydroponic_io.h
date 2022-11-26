/*
 * hydroponic_io.h
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#ifndef HYDROPONIC_IO_H_
#define HYDROPONIC_IO_H_

#include <stdint.h>
#include <stddef.h>
#include "main.h"

#define ESP_UART_HANDLE				USART1
#define ESP_UART_IRQHandler			USART1_IRQHandler
#define ESP_UART_RX_IT_ENABLE()		(ESP_UART_HANDLE->CR1 |= (1<<5))



void esp_uart_send_bytes(uint8_t* array, size_t size);
uint8_t esp_uart_receive_byte(void);

#endif /* HYDROPONIC_IO_H_ */
