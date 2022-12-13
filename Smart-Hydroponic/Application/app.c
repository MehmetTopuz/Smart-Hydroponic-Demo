/*
 * app.c
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#include "app.h"


static SemaphoreHandle_t publisher_task_sem, broker_connection_sem, listener_task_sem, broker_connected_sem;

static TimerHandle_t soft_timer;

static QueueHandle_t command_queue;

static const char *topic_list[50] = {
		"hydroponic/lights",
		"hydroponic/pump",
		"hydroponic/valve",
		"hydroponic/air_conditioner",
		"hydroponic/dosing_pump",
		"hydroponic/alarm",
		NULL};

static const char *command_list[50] = {
		"SHUT_DOWN",
		"PUMP_MOTOR_ON",
		"PUMP_MOTOR_OFF",
		"LIGHTS_ON",
		"LIGHTS_OFF",
		"VALVE_ON",
		"VALVE_OFF",
		"AIR_CONDITIONER_ON",
		"AIR_CONDITIONER_OFF",
		"DOSING_PUMP_ON",
		"DOSING_PUMP_OFF",
		"ALARM_ON",
		"ALARM_OFF",
		NULL
};

int app_init(void){

	if(ESP_Init(esp_uart_send_bytes,				// UART transmit function
				esp_uart_receive_byte,				// UART receive function
				ESP_IO_GetTick,						// get tick function
				ESP_UART_RING_BUFFER_SIZE) < 0)		// UART ring buffer size
		return -1;

	if(!mqtt_init(MQTT_RING_BUFFER_SIZE))
		return -1;

	ESP_UART_RX_IT_ENABLE();

	Status response = IDLE;

	while((response = Is_Echo_Mode_Disabled()) == IDLE);

	if(response == STATUS_ERROR)
	  while((response = Disable_Echo_Mode()) == IDLE);

	if(response == STATUS_ERROR)
		debug_printf("Error: Disable echo mode\n");

	/*
	 * TODO: IO init
	 */

	/* create semaphore for broker connect task*/

	broker_connection_sem = xSemaphoreCreateBinary();
	broker_connected_sem = xSemaphoreCreateBinary();

	/* create semaphore for broker connect task*/

	publisher_task_sem = xSemaphoreCreateBinary();

	/* create semaphore for broker connect task*/

	listener_task_sem = xSemaphoreCreateBinary();

	/* create a queue for command process.*/

	command_queue = xQueueCreate(10, sizeof(commands_t));

	/* Create Tasks */

	xTaskCreate(broker_connect_task,
				"Broker Connect Task",
				512,
				NULL,
				0,
				NULL);

	xTaskCreate(publisher_task,
				"Publish Task",
				512,
				NULL,
				0,
				NULL);

	xTaskCreate(listener_task,
				"Listener Task",
				512,
				NULL,
				0,
				NULL);

	xTaskCreate(command_process_task,
				"Command Process Task",
				512,
				NULL,
				1,
				NULL);

	soft_timer = xTimerCreate("Timer",
								pdMS_TO_TICKS(APP_TIMER_PERIOD),
								pdTRUE,
								0,
								timer_callback);

	xTimerStart(soft_timer,0);

	return 1;

}

void broker_connect_task(void *argument){

	Status response = IDLE;
	int number_of_tries = 0;

	for(;;){

		while((response = Is_Wifi_Connected()) == IDLE);

		if(response == STATUS_ERROR){

			while(1){
				number_of_tries++;

				while((response = Connect_Wifi(WIFI_SSID, WIFI_PASSWORD)) == IDLE);

				if(response != STATUS_OK)
					debug_printf("Error: Not connected to wifi.\n");

				if(number_of_tries >= 3){
					debug_printf("Error: Three times of connecting attempts.\n");
					while(1){
						// TODO: handle the error properly.
					}
				}

				while((response = Is_Wifi_Connected()) == IDLE);

				if(response == STATUS_OK)
					break;

			}

		}
		number_of_tries = 0;

		while(1){

			while((response = mqtt_connect_broker(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID)) == IDLE);

			if(response != STATUS_OK)
				debug_printf("Error: Not connected to broker.\n");

			else{
				/*
				 * Check broker connection.
				 */

				while((response = mqtt_ping_request()) == IDLE);

				if(response == STATUS_OK)
				 {
					debug_printf("Connected to the broker.\n");
					 number_of_tries = 0;
					 break;
				 }
			}

			if((response == STATUS_ERROR) || (response == TIMEOUT_ERROR))
				 number_of_tries++;

			if(number_of_tries >= 3){
				debug_printf("Error: Three unsuccessful attempts.\n");
				break;
			}

		}

		/* send subscribe requests.*/
		subscribe_topics(topic_list);

		/* release semaphore for use in other tasks.*/
		xSemaphoreGive(broker_connected_sem);
//		xSemaphoreGive(publisher_task_sem);
		xSemaphoreGive(listener_task_sem);

		xSemaphoreTake(broker_connection_sem,portMAX_DELAY);



	}
}

void publisher_task(void *argument){

	/*
	 * All tasks need to be suspended at first.
	 * As a result, we need a blocking object, like a semaphore.
	 */
	xSemaphoreTake(broker_connected_sem, portMAX_DELAY);

	char payload[100] = "test message";
	int number_of_tries = 0;

	for(;;){

		xSemaphoreTake(publisher_task_sem, portMAX_DELAY);
//		vTaskDelay(pdMS_TO_TICKS(1000));
		/*
		 * TODO: Get all the variables that will be published.
		 */


		/*
		 * TODO: Create encode_publish function.
		 */


		/* Publish heart-beat message*/
		Status response = IDLE;
		while(1){
			/*
			 * TODO: Add a mutex here for UART.
			 */
			response = mqtt_publish_message("hydroponic/test", payload);

			if(response == IDLE){		// If the library is awaiting an answer from the ESP, execute the following task.
				taskYIELD();
			}
			else if(response == STATUS_ERROR){
				debug_printf("ERROR: Publish Error!\nReturn type: STATUS_ERROR.\n");
				number_of_tries++;
			}
			else if(response == TIMEOUT_ERROR){
				debug_printf("ERROR: Publish Error!\nReturn type: TIMEOUT_ERROR.\n");
				number_of_tries++;
			}
			else if(response == STATUS_OK){
				debug_printf("Publish is successful.\n \"%s\"->%d bytes sent.\n",payload, strlen(payload));
				number_of_tries = 0;
				break;
			}

			if(number_of_tries >= 3){
				xSemaphoreGive(broker_connection_sem);		// Execute the broker connect task if publish is not successful after 3 times.
				xSemaphoreTake(broker_connected_sem, portMAX_DELAY);
				number_of_tries = 0;
				break;
			}


		}


	}
}

void listener_task(void *argument){

	/*
	 * All tasks need to be suspended at first.
	 * As a result, we need a blocking object, like a semaphore.
	 */
	xSemaphoreTake(listener_task_sem, portMAX_DELAY);

	int32_t result = 0;
	MQTT_Publish_Packet received_packet = {0};
	commands_t cmd;

	for(;;){

		result = mqtt_read_command(&received_packet, topic_list);

		if(result > 0){

			cmd = string_to_cmd(&received_packet);
			xQueueSendToBack(command_queue, &cmd,0);
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			memset(received_packet.message, 0, sizeof(received_packet.message));
			mqtt_clear_buffer();

		}
		vTaskDelay(pdMS_TO_TICKS(50));

	}
}

void command_process_task(void *argument){

	BaseType_t queue_status = pdPASS;
	commands_t received_command = shut_down;


	for(;;){

		queue_status = xQueueReceive(command_queue, &received_command, portMAX_DELAY);

		if(queue_status == pdPASS){
			command_handler(received_command);
		}

	}
}

void timer_callback(TimerHandle_t xTimer){

	static uint32_t count_for_publish = 0;

	count_for_publish++;

	if(count_for_publish >= PUBLISH_PERIOD){
		count_for_publish = 0;
		xSemaphoreGive(publisher_task_sem);
	}
}
/* configCHECK_FOR_STACK_OVERFLOW must be set to 1 in order to use the vApplicationStackOverflowHook function. */

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName ){

	debug_printf("Stack Over Flow Error: %s\n", (char*)pcTaskName);
}
/* configUSE_MALLOC_FAILED_HOOK must be set to 1*/

void vApplicationMallocFailedHook( void ){

	debug_printf("Malloc Failed!\n");
}

void command_handler(commands_t cmd){

	switch (cmd) {
		case pump_motor_on:
			debug_printf("Command captured: pump_motor_on\n");
			break;
		case pump_motor_off:
			debug_printf("Command captured: pump_motor_off\n");
			break;
		case lights_on:
			debug_printf("Command captured: lights_on\n");
			break;
		case lights_off:
			debug_printf("Command captured: lights_off\n");
			break;
		case valve_on:
			debug_printf("Command captured: valve_on\n");
			break;
		case valve_off:
			debug_printf("Command captured: valve_off\n");
			break;
		case air_conditioner_on:
			debug_printf("Command captured: air_conditioner_on\n");
			break;
		case air_conditioner_off:
			debug_printf("Command captured: air_conditioner_off\n");
			break;
		case dosing_pump_on:
			debug_printf("Command captured: dosing_pump_on\n");
			break;
		case dosing_pump_off:
			debug_printf("Command captured: dosing_pump_off\n");
			break;
		case alarm_on:
			debug_printf("Command captured: alarm_on\n");
			break;
		case alarm_off:
			debug_printf("Command captured: alarm_off\n");
			break;
		default:
			break;
	}
}

int mqtt_read_command(MQTT_Publish_Packet *packet, const char **topic_array){

	int result = 0, idx = 0;

	while(topic_array[idx]){
		if((result = mqtt_read_message(packet, topic_array[idx++])) > 0)
			return result;
	}
	return result;
}


void subscribe_topics(const char **topics){

	Status response = IDLE;
	int idx = 0;
	while(topics[idx]){
		while((response = mqtt_subcribe(topics[idx])) == IDLE);

		if(response != STATUS_OK)
			debug_printf("Error: Subscribe is not successful -> %s.\n", topics[idx++]);
		else
			debug_printf("Subscribe is successful -> \"%s\".\n", topics[idx++]);
	}

}

commands_t string_to_cmd(MQTT_Publish_Packet *packet){

	int idx = 0;

	while(command_list[idx]){
		if(strcmp(packet->message, command_list[idx]) == 0)
			return (commands_t)idx;
		else
			idx++;
	}

	return idle;
}
