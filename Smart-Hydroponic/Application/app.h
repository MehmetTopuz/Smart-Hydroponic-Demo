/*
 * app.h
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#ifndef APP_H_
#define APP_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"


#define ESP_IO_GetTick				HAL_GetTick
#define ESP_UART_RING_BUFFER_SIZE	1024
#define MQTT_RING_BUFFER_SIZE		1024

#define WIFI_SSID					"Topuz"
#define	WIFI_PASSWORD				"tmhm4545."
#define MQTT_BROKER_IP				"192.168.137.1"
#define MQTT_BROKER_PORT			"1883"
#define MQTT_CLIENT_ID				"Topuz"
#define MQTT_SUBSCRIBE_TOPIC		"topuz/sub"

#define APP_TIMER_PERIOD			10000UL		// 10 seconds

int app_init(void);

int app_run(void);

void publisher_task(void *argument);

void listener_task(void *argument);

void broker_connect_task(void *argument);

void command_process_task(void *argument);

void timer_callback(TimerHandle_t xTimer);

#endif /* APP_H_ */
