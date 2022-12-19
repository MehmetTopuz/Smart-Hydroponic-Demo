/*
 * app.c
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#include "app.h"


static SemaphoreHandle_t broker_connection_sem, listener_task_sem, broker_connected_sem;

static TimerHandle_t soft_timer;

static QueueHandle_t command_queue, publish_queue;

static temp_t temparature_t;

static ph_t pH_t;

static uint8_t humidity, tank_level, conductivity;

static const char *topic_list[50] = {
		"hydroponic/lights",
		"hydroponic/pump",
		"hydroponic/valve",
		"hydroponic/air_conditioner",
		"hydroponic/dosing_pump",
		"hydroponic/alarm",
		"hydroponic/system",
//		"hydroponic/heartbeat",
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

static const char *message_list[20] = {
		"Shutting down.",
		"Pump motor is running.",
		"Pump motor is stopped.",
		"The lights are on.",
		"The lights are off.",
		"Solenoid valve is on.",
		"Solenoid valve is off.",
		"Air conditioner is on.",
		"Air conditioner is off.",
		"Dosing pump is running.",
		"Dosing pump is stopped.",
		"Alarm has been activated.",
		"Alarm has been deactivated."

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

	/* create semaphore for listener task*/

	listener_task_sem = xSemaphoreCreateBinary();

	/* create a queue for command process.*/

	command_queue = xQueueCreate(10, sizeof(commands_t));

	/* create a queue for publish task.*/

	publish_queue = xQueueCreate(5, sizeof(message_t));

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

			while((response = mqtt_connect_broker(MQTT_BROKER_IP, MQTT_BROKER_PORT)) == IDLE);

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

	uint8_t heart_beat_packet[SIZE_OF_HEART_BEAT_PACKET] = {0};
	int number_of_tries = 0, payload_length = 0;

	message_t publish_message = {0};

	int index = 0;

	for(;;){

		if(xQueueReceive(publish_queue, &publish_message, portMAX_DELAY) == errQUEUE_EMPTY)
			continue;

		Status response = IDLE;
		while(1){
			/*
			 * TODO: Add a mutex here for UART.
			 */
			/* Publish heart-beat message*/
			if(strcmp(publish_message.message, "HEART_BEAT") == 0){
				humidity = 30;
				temparature_t.temp_f = 25.4;
				pH_t.ph_f = 6.6;
				tank_level = 80;
				conductivity = 20;

				payload_length = encode_heart_beat_packet(heart_beat_packet);

				response = mqtt_publish_message("hydroponic/heartbeat", heart_beat_packet, payload_length);
			}
			else if((index = message_to_index(&publish_message)) != -1)
				response = mqtt_publish_message("hydroponic/messages", (uint8_t*)message_list[index], payload_length = strlen(message_list[index]));

			if(response == IDLE){		// If the library is awaiting an answer from the ESP, execute the waiting task.
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
				debug_printf("Publish is successful.%d bytes sent.\n",payload_length);
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
			memset(received_packet.payload, 0, sizeof(received_packet.payload));
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
	message_t heart_beat = {0};
	strcpy(heart_beat.message, "HEART_BEAT");
	count_for_publish++;

	if(count_for_publish >= PUBLISH_PERIOD){
		count_for_publish = 0;
		xQueueSendToBack(publish_queue, &heart_beat, 0);
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

	message_t publish_message = {0};
	switch (cmd) {
		case shut_down:
			debug_printf("Command captured: shut_down\n");
			strcpy(publish_message.message, command_list[0]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			vTaskDelay(pdMS_TO_TICKS(1000));
			HAL_NVIC_SystemReset();
			break;
		case pump_motor_on:
			debug_printf("Command captured: pump_motor_on\n");
			strcpy(publish_message.message, command_list[1]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case pump_motor_off:
			debug_printf("Command captured: pump_motor_off\n");
			strcpy(publish_message.message, command_list[2]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case lights_on:
			debug_printf("Command captured: lights_on\n");
			strcpy(publish_message.message, command_list[3]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case lights_off:
			debug_printf("Command captured: lights_off\n");
			strcpy(publish_message.message, command_list[4]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case valve_on:
			debug_printf("Command captured: valve_on\n");
			strcpy(publish_message.message, command_list[5]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case valve_off:
			debug_printf("Command captured: valve_off\n");
			strcpy(publish_message.message, command_list[6]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case air_conditioner_on:
			debug_printf("Command captured: air_conditioner_on\n");
			strcpy(publish_message.message, command_list[7]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case air_conditioner_off:
			debug_printf("Command captured: air_conditioner_off\n");
			strcpy(publish_message.message, command_list[8]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case dosing_pump_on:
			debug_printf("Command captured: dosing_pump_on\n");
			strcpy(publish_message.message, command_list[9]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case dosing_pump_off:
			debug_printf("Command captured: dosing_pump_off\n");
			strcpy(publish_message.message, command_list[10]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case alarm_on:
			debug_printf("Command captured: alarm_on\n");
			strcpy(publish_message.message, command_list[11]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
			break;
		case alarm_off:
			debug_printf("Command captured: alarm_off\n");
			strcpy(publish_message.message, command_list[12]);
			xQueueSendToBack(publish_queue, &publish_message, 0);
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
		if(strcmp((char*)packet->payload, command_list[idx]) == 0)
			return (commands_t)idx;
		else
			idx++;
	}

	return idle;
}

int encode_heart_beat_packet(uint8_t *buffer){

	memset(buffer, 0, SIZE_OF_HEART_BEAT_PACKET);
	int index = 0;

	uint8_t status_byte = 0;
	/*
	 * TODO: fill status bits.
	 */
	buffer[index++] = status_byte;
	/* append temperature to buffer using union.*/
	buffer[index++] = temparature_t.temp_b[0];
	buffer[index++] = temparature_t.temp_b[1];
	buffer[index++] = temparature_t.temp_b[2];
	buffer[index++] = temparature_t.temp_b[3];

	buffer[index++] = humidity;
	buffer[index++] = tank_level;
	buffer[index++] = conductivity;

	/* append ph to buffer.*/
	buffer[index++] = pH_t.ph_b[0];
	buffer[index++] = pH_t.ph_b[1];
	buffer[index++] = pH_t.ph_b[2];
	buffer[index++] = pH_t.ph_b[3];

	return index;
}

int message_to_index(message_t *message){

	int index = 0;

	while(command_list[index]){
		if(strcmp(message->message, command_list[index]) == 0){
			return index;
		}
		else
			index++;
	}
	return -1;
}
