/*
 * app.c
 *
 *  Created on: Nov 26, 2022
 *      Author: Topuz
 */

#include "app.h"
#include "hydroponic_io.h"
#include "mqtt.h"
#include "string.h"


static SemaphoreHandle_t publisher_task_sem, broker_connection_sem, listener_task_sem, broker_connected_sem;

static TimerHandle_t soft_timer;

static QueueHandle_t command_queue;

MQTT_Publish_Packet received_packet = {0};

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

	command_queue = xQueueCreate(10, sizeof(commands));

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

//	Status response = IDLE;
//
//	while((response = Is_Echo_Mode_Disabled()) == IDLE);
//
//	if(response == STATUS_ERROR)
//	  while((response = Disable_Echo_Mode()) == IDLE);
//
//	if(response != STATUS_OK)
//		return -1;
//
//	while((response = Connect_Wifi(WIFI_SSID, WIFI_PASSWORD)) == IDLE);
//
//	if(response != STATUS_OK)
//		return -1;
//
//	while((response = mqtt_connect_broker(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID)) == IDLE);
//
//	if(response != STATUS_OK)
//		return -1;
//
//	while((response = mqtt_subcribe(MQTT_SUBSCRIBE_TOPIC)) == IDLE);
//
//	if(response != STATUS_OK)
//		return -1;

	return 1;

}

int app_run(void){

//	  int result = 0;
//	  MQTT_Publish_Packet received_packet = {0};
//	  char payload[100];
//	  uint32_t lastTick=0;
//	  uint32_t local_time = 0;
//	  uint32_t hour=0,minute=0,second=0;
//	  Status response;
//	  while (1)
//	  {
//
//		  if(HAL_GetTick() - lastTick >= 1000){
//			  lastTick = HAL_GetTick();
//			  local_time++;
//			  hour = local_time/3600;
//			  minute = local_time/60;
//			  second = local_time%60;
//			  sprintf(payload,"Local time:%.2d:%.2d:%.2d",hour,minute,second);
//			  while((response = mqtt_publish_message("topuz/test", payload)) == IDLE);
//
//		  }
//
//		  result = mqtt_read_message(&received_packet, "topuz/sub");
//		  if(result>0)
//		  {
//			  if(strcmp(received_packet.message,"LED_TOGGLE") == 0){
//				  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//				  //while((response = mqtt_disconnect_broker()) == IDLE );
//			  }
//
//			  mqtt_clear_buffer();
//		  }
//	  }
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

		/*
		 * TODO: Create a wrapper function in order to subscribe multiple topics.
		 */
		while((response = mqtt_subcribe(MQTT_SUBSCRIBE_TOPIC)) == IDLE);

		if(response != STATUS_OK)
			debug_printf("Error: Subscribe is not successful -> %s.\n", MQTT_SUBSCRIBE_TOPIC);
		else{
			debug_printf("Subscribe is successful -> \"%s\".\n", MQTT_SUBSCRIBE_TOPIC);
		}
		/* release semaphore for use in other tasks.*/
		xSemaphoreGive(broker_connected_sem);
//		xSemaphoreGive(publisher_task_sem);
		xSemaphoreGive(listener_task_sem);

		xSemaphoreTake(broker_connection_sem,portMAX_DELAY);
//		clear_ESP_ring_buffer();


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
			response = mqtt_publish_message("topuz/test", payload);

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
//	MQTT_Publish_Packet received_packet = {0};
	commands cmd;

	for(;;){
		/*
		 * TODO: Write a wrapper function for read message.
		 * This function must have a string array that include topics.
		 *
		 */
		result = mqtt_read_message(&received_packet, "topuz/sub");

		if(result > 0){

			/*
			 * TODO: Add command to command queue.
			 */
			if(strcmp(received_packet.message,"PUMP_MOTOR_ON") == 0){
				cmd = pump_motor_on;
				xQueueSendToBack(command_queue, &cmd,0);
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				//memset(received_packet.message, 0, 100);
			}
			else if(strcmp(received_packet.message,"PUMP_MOTOR_OFF") == 0){
				cmd = pump_motor_off;
				xQueueSendToBack(command_queue, &cmd,0);
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				//memset(received_packet.message, 0, 100);
			}
			else if(strcmp(received_packet.message,"LIGHTS_ON") == 0){
				cmd = lights_on;
				xQueueSendToBack(command_queue, &cmd,0);
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				//memset(received_packet.message, 0, 100);
			}
			else if(strcmp(received_packet.message,"LIGHTS_OFF") == 0){
				cmd = lights_off;
				xQueueSendToBack(command_queue, &cmd,0);
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

			}
			memset(received_packet.message, 0, sizeof(received_packet.message));
			mqtt_clear_buffer();


		}
		vTaskDelay(pdMS_TO_TICKS(50));

	}
}

void command_process_task(void *argument){

	BaseType_t queue_status = pdPASS;
	commands received_command = shut_down;


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

void command_handler(commands cmd){

	switch (cmd) {
		case pump_motor_on:
			printf("Command captured: pump_motor_on\n");
			break;
		case pump_motor_off:
			printf("Command captured: pump_motor_off\n");
			break;
		case lights_on:
			printf("Command captured: lights_on\n");
			break;
		case lights_off:
			printf("Command captured: lights_off\n");
			break;
		case valve_on:
			printf("Command captured: valve_on\n");
			break;
		case valve_off:
			printf("Command captured: valve_off\n");
			break;
		case air_conditioner_on:
			printf("Command captured: air_conditioner_on\n");
			break;
		case air_conditioner_off:
			printf("Command captured: air_conditioner_off\n");
			break;
		case dosing_pump_on:
			printf("Command captured: dosing_pump_on\n");
			break;
		case dosing_pump_off:
			printf("Command captured: dosing_pump_off\n");
			break;
		case alarm_on:
			printf("Command captured: alarm_on\n");
			break;
		case alarm_off:
			printf("Command captured: alarm_off\n");
			break;
		default:
			break;
	}
}
