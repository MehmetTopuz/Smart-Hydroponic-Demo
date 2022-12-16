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
#include "queue.h"

#include "hydroponic_io.h"
#include "mqtt.h"
#include "string.h"


#define UART_DEBUG					//  Change this line to comment if you do not want to print debug messages.

#ifdef	UART_DEBUG

#define debug_printf				printf

#else

#define debug_printf(...)			(void)0
#endif

#define ESP_IO_GetTick				HAL_GetTick
#define ESP_UART_RING_BUFFER_SIZE	1024UL
#define MQTT_RING_BUFFER_SIZE		1024UL

#define WIFI_SSID					"Topuz"
#define	WIFI_PASSWORD				"tmhm4545."
#define MQTT_BROKER_IP				"192.168.137.1"
#define MQTT_BROKER_PORT			"1883"

#define APP_TIMER_PERIOD			1000UL		// 1 second
#define PUBLISH_PERIOD				10UL		// second

typedef enum{
	shut_down = 0,
	pump_motor_on,
	pump_motor_off,
	lights_on,
	lights_off,
	valve_on,
	valve_off,
	air_conditioner_on,
	air_conditioner_off,
	dosing_pump_on,
	dosing_pump_off,
	alarm_on,
	alarm_off,
	idle
}commands_t;

int app_init(void);

int app_run(void);

void publisher_task(void *argument);

void listener_task(void *argument);

void broker_connect_task(void *argument);

void command_process_task(void *argument);

void timer_callback(TimerHandle_t xTimer);

void command_handler(commands_t cmd);

int mqtt_read_command(MQTT_Publish_Packet *packet, const char **topic_array);

void subscribe_topics(const char **topics);

commands_t string_to_cmd(MQTT_Publish_Packet *packet);

#endif /* APP_H_ */
