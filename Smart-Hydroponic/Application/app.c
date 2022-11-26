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

	while((response = Connect_Wifi(WIFI_SSID, WIFI_PASSWORD)) == IDLE);

	while((response = mqtt_connect_broker(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID)) == IDLE);

	while((response = mqtt_subcribe(MQTT_SUBSCRIBE_TOPIC)) == IDLE);

	return 1;

}

int app_run(void){

	  int result = 0;
	  MQTT_Publish_Packet received_packet = {0};
	  char payload[100];
	  uint32_t lastTick=0;
	  uint32_t local_time = 0;
	  uint32_t hour=0,minute=0,second=0;
	  Status response;
	  while (1)
	  {

		  if(HAL_GetTick() - lastTick >= 1000){
			  lastTick = HAL_GetTick();
			  local_time++;
			  hour = local_time/3600;
			  minute = local_time/60;
			  second = local_time%60;
			  sprintf(payload,"Local time:%.2d:%.2d:%.2d",hour,minute,second);
			  while((response = mqtt_publish_message("topuz/test", payload)) == IDLE);

		  }

		  result = mqtt_read_message(&received_packet, "topuz/sub");
		  if(result>0)
		  {
			  if(strcmp(received_packet.message,"LED_TOGGLE") == 0){
				  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				  //while((response = mqtt_disconnect_broker()) == IDLE );
			  }

			  mqtt_clear_buffer();
		  }
	  }
	  return 1;
}

