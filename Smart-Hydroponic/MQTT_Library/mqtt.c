/**
 * @file 	mqtt.c
 * @author  Mehmet Topuz
 * @brief   Source file of the MQTT library. This library uses esp8266.h library to connect MQTT broker.
 *
 *  Website : <a href="https://mehmettopuz.net/">mehmettopuz.net</a>
 *  Created on: May 7, 2022
 *
 */

#include <string.h>
#include "mqtt.h"

RingBuffer *mqtt_rx_buffer;

/**
 * @brief	Initialize function of the MQTT library.
 * @param	rx_buffer_size: Size of the buffer that will be allocated dynamically for the ring buffer
 * 			to handle UART receive operations.
 * @retval	The function returns 1 when the memory was allocated successfully. Otherwise, it will return 0.
 */
uint32_t mqtt_init(size_t rx_buffer_size){

	mqtt_rx_buffer = ringBuffer_init(rx_buffer_size);
	if(mqtt_rx_buffer != NULL)
		return 1;

	return 0;
}

/**
 * @brief	This function serializes the MQTT packets according to given packet type.
 * @param	*buffer: The address of the buffer that will be representing the bytes of MQTT packets.
 * @param	*packet: The address of the packet structure.
 * @param	packetType: Enumerator type of packets. The function handles the void* parameter based on this enumerator type.
 * @retval	The function returns the size of bytes that written to the buffer. Otherwise, it will return -1.
 */
int32_t mqtt_encode_packet(uint8_t *buffer, void *packet, mqtt_packet_types packetType ){

	if(packetType == CONNECT_PACKET)
	{
		int32_t index = 0;

		MQTT_Connect_Packet *connectPacket = (MQTT_Connect_Packet*)packet;

		buffer[index++] = connectPacket->ConnectByte;
		buffer[index++] = connectPacket->RemainLength;
		buffer[index++] = connectPacket->ProtocolNameLength>>8;
		buffer[index++]= connectPacket->ProtocolNameLength & 0xFF;
		for(uint16_t i=0;i<connectPacket->ProtocolNameLength;i++)
		{
			if(i==4)
				return -1;
			buffer[index++] = connectPacket->ProtocolName[i];
		}
		buffer[index++] = connectPacket->Level;
		buffer[index++] = connectPacket->Flag;
		buffer[index++] = connectPacket->KeepAlive >> 8;
		buffer[index++] = connectPacket->KeepAlive & 0xFF;
		buffer[index++] = connectPacket->ClientIDLength >> 8;
		buffer[index++] = connectPacket->ClientIDLength & 0xFF;
		for(uint16_t i=0;i<connectPacket->ClientIDLength;i++)
		{
			if(i == MAX_LENGTH_OF_CLIENT_ID)
				return -1;
			buffer[index++] = connectPacket->ClientID[i];
		}

#ifdef	USE_MQTT_AUTHENTICATION
		uint16_t user_name_length = strlen(connectPacket->UserName);
		buffer[index++] = user_name_length >> 8;
		buffer[index++] = user_name_length & 0xFF;
		for(uint16_t i=0; i<user_name_length; i++){
			if(i == MAX_LENGTH_OF_USERNAME)
				return -1;
			buffer[index++] = connectPacket->UserName[i];
		}

		uint16_t password_length = strlen(connectPacket->Password);
		buffer[index++] = password_length >> 8;
		buffer[index++] = password_length & 0xFF;
		for(uint16_t i=0; i<password_length; i++){
			if(i == MAX_LENGTH_OF_PASSWORD)
				return -1;
			buffer[index++] = connectPacket->Password[i];
		}
#endif
		return index;

	}
	else if(packetType == SUBSCRIBE_PACKET)
	{

		int32_t index = 0;

		MQTT_Subscribe_Packet *subscribePacket = (MQTT_Subscribe_Packet*)packet;

		buffer[index++] = subscribePacket->subscribePacketByte;
		buffer[index++] = subscribePacket->remainLength;
		buffer[index++] = subscribePacket->packetID >> 8;
		buffer[index++] = subscribePacket->packetID & 0xFF;
		buffer[index++] = subscribePacket->topicLength >> 8;
		buffer[index++] = subscribePacket->topicLength & 0xFF;
		for(uint16_t i=0;i<subscribePacket->topicLength;i++)
		{
			if(i == MAX_LENGTH_OF_TOPIC_NAME)
				return -1;
			buffer[index++] = subscribePacket->topic[i];
		}

		buffer[index++] = subscribePacket->Qos;

		return index;


	}
	else if(packetType == PUBLISH_PACKET)
	{
		int32_t index = 0;

		MQTT_Publish_Packet *publishPacket = (MQTT_Publish_Packet*)packet;

		buffer[index++] = publishPacket->publishPacketByte;
		buffer[index++] = publishPacket->remainLength;
		buffer[index++] = publishPacket->topicLength >> 8;
		buffer[index++] = publishPacket->topicLength & 0xFF;

		for(uint16_t i=0;i<publishPacket->topicLength;i++)
		{
			if(i == MAX_LENGTH_OF_TOPIC_NAME)
				return -1;

			buffer[index++] = publishPacket->topic[i];
		}
		for(uint16_t i=0; i< publishPacket->sizeOfPayload ;i++)
		{
			if(i == MAX_LENGTH_OF_TOPIC_MESSAGE)
				return -1;

			buffer[index++] = publishPacket->payload[i];
		}
		return index;
	}

	return -1;
}

/**
 * @brief	This function tries to establish TCP connection to the MQTT broker.
 * @param	ip: IP address of the MQTT broker.
 * @param	port: Port of the MQTT broker.
 * @retval	The function returns STATUS_OK when the broker connection was successful.
 * 			Otherwise, it returns one of the error types of Status typedef.
 */
Status mqtt_connect_broker(const char* ip,const char* port){

	static int isFirstCall = 0;
	static uint8_t packetBuffer[100] = {0};
	static int32_t numberOfBytes = 0;


	if(!isFirstCall){
		MQTT_Connect_Packet packet = {0};
		uint16_t lengthOfClientID = strlen(MQTT_CLIENT_ID);
		uint8_t remainLength = 0;

#ifdef	USE_MQTT_AUTHENTICATION
		remainLength = lengthOfClientID + 12 + 4 + strlen(MQTT_USERNAME) + strlen(MQTT_PASSWORD);
		packet.Flag = 0xC2;
		strcpy(packet.UserName,MQTT_USERNAME);
		strcpy(packet.Password,MQTT_PASSWORD);
#else
		remainLength = lengthOfClientID + 12;
		packet.Flag = 0x02;
#endif
		packet.ConnectByte = 0x10;
		packet.RemainLength = remainLength;
		packet.ProtocolNameLength = 0x0004;
		strcpy(packet.ProtocolName,"MQTT");
		packet.Level = 0x04;
		packet.KeepAlive = MQTT_KEEP_ALIVE;
		packet.ClientIDLength = lengthOfClientID;
		strcpy(packet.ClientID,MQTT_CLIENT_ID);

		numberOfBytes = mqtt_encode_packet(packetBuffer, &packet, CONNECT_PACKET);

		if(numberOfBytes < 0)
			return STATUS_ERROR;

		isFirstCall = 1;
	}

	static int state = 0;
	Status status = IDLE;
	if(state == 0){
		status = Connect_TCP_Server(ip, port);
		if(status == STATUS_OK)
			state++;
		else if(status == STATUS_ERROR){
			isFirstCall = 0;
			return status;
		}
		else
			return status;
	}
	if(state == 1){
		status = Send_TCP_Bytes(packetBuffer, numberOfBytes);
		if(status == STATUS_OK){
			state = 0;
			isFirstCall = 0;
			return status;
		}
		else
			return status;
	}

	isFirstCall  = 0;
	return STATUS_ERROR;

}

/**
 * @brief	This function tries to disconnect TCP connection with the MQTT broker.
 * @param	None
 * @retval	The function returns STATUS_OK when the broker connection was successfully disconnected.
 * 			Otherwise, it returns one of the error types of Status typedef.
 */
Status mqtt_disconnect_broker(void){

	uint8_t disconnect_packet[2] = {MQTT_DISCONNECT_HEADER,0x00};

	Status response = IDLE;

	response = Send_TCP_Bytes(disconnect_packet, 2);
	if(response == FOUND){
		return IDLE;
	}
	else
		return response;

	return IDLE;
}

/**
 * @brief	This function sends ping request to the broker and waits ping response packet.
 * @param	None
 * @retval	It returns STATUS_OK when ping response was received.
 */
Status mqtt_ping_request(void){

	Status response = IDLE;
	uint8_t pingReqPacket[2] = {MQTT_PING_REQUEST, 0x00};
	uint8_t pingRespacket[2] = {MQTT_PING_RESPONSE, 0x00};
	static int state = 0;

	if(!state){
		response = Send_TCP_Bytes(pingReqPacket, 2);

		if(response == STATUS_OK){
			state++;
		}
		else
			return response;
	}
	if(state){
		response = Wait_Response((char*)pingRespacket, 2, TIMEOUT);		// TODO: There is a bug here. Fix it.
		if(response == FOUND){
			state = 0;
			response = STATUS_OK;
			return response;
		}

	}

	return response;
}

/**
 * @brief	This function publishes message over MQTT.
 * @param	topic:		The address of topic.
 * @param	payload:	Message bytes that will be sent to the broker.
 * @param	size:		The size of the payload.
 * @retval	It returns STATUS_OK when message was sent successfully. Otherwise, it returns one of the error types of Status typedef.
 * 			Note: This function does not check whether the message was delivered or not because the QoS is currently set to zero.
 */
Status mqtt_publish_message(const char* topic, const uint8_t* payload, size_t size){

	static int isFirstCall = 0;
	static uint8_t packetBuffer[100] = {0};
	static int32_t numberOfBytes = 0;


	if(!isFirstCall){
		MQTT_Publish_Packet packet = {0};
		uint16_t topicLength = strlen(topic);
		uint8_t remainLength = topicLength + size + 2;	// 2 represents topic length bytes.


		packet.publishPacketByte = MQTT_PUBLISH_HEADER;	// Qos, DUP flag and Retain bytes are equal to zero.
		packet.remainLength = remainLength;
		strcpy(packet.topic,topic);
		packet.topicLength = topicLength;
		packet.sizeOfPayload = size;
		for(int i=0; i<size; i++)
			packet.payload[i] = payload[i];

		numberOfBytes = mqtt_encode_packet(packetBuffer, &packet, PUBLISH_PACKET);

		if(numberOfBytes < 0)
			return STATUS_ERROR;

		isFirstCall = 1;
	}

	Status status = IDLE;

	status = Send_TCP_Bytes(packetBuffer, numberOfBytes);

	if( status == STATUS_ERROR || status == STATUS_OK)
		isFirstCall = 0;

	return status;

}

/**
 * @brief	This function sends subscribe request to the broker.
 * @param	topic: The name of the topic that will be subscribed.
 * @retval	It returns STATUS_OK when subscribe request was sent successfully.
 */
Status mqtt_subcribe(const char* topic){

	static int isFirstCall = 0;
	static uint8_t packetBuffer[100] = {0};
	static int32_t numberOfBytes = 0;


	if(!isFirstCall){
		MQTT_Subscribe_Packet packet = {0};
		uint16_t topicLength = strlen(topic);

		uint8_t remainLength = topicLength + 5;


		packet.subscribePacketByte = 0x82;
		packet.remainLength = remainLength;
		packet.Qos = 0;
		strcpy(packet.topic,topic);
		packet.packetID = 0x01;
		packet.topicLength = topicLength;

		numberOfBytes = mqtt_encode_packet(packetBuffer, &packet, SUBSCRIBE_PACKET);

		if(numberOfBytes < 0)
			return STATUS_ERROR;

		isFirstCall = 1;
	}

	Status status = IDLE;

	status = Send_TCP_Bytes(packetBuffer, numberOfBytes);

	if( status == STATUS_ERROR || status == STATUS_OK)
		isFirstCall = 0;

	return status;

	// handle SUBACK packet later.
}

extern Esp_Init_Typedef ESP8266;

/**
 * @brief	This function handles UART receive operations in background.
 * 			It must be called in the UART interrupt handler function.
 * @param 	None
 * @retval	None
 */
void mqtt_receive_handler(void){

	ringBuffer_push(mqtt_rx_buffer, ESP8266.UART_Receive());
}

/**
 * @brief	This function checks if there is a publish message in the ring buffer.
 * @param	packet: The address of the packet that will be filled with the function.
 * @param	topic:	The address of the topic name that will be searched in the ring buffer.
 * @retval	It returns -1 if message is not available. Otherwise, It returns the length of packet.
 */
int32_t mqtt_read_message(MQTT_Publish_Packet *packet, const char *topic){

	int position = 0;
	int count = 0;
	int topic_length = strlen(topic);
	// search topic name in the buffer
	for(int i=0; i<(mqtt_rx_buffer->size - strlen(topic));i++){
		for(int j=0;j<topic_length;j++){
			if((char)mqtt_rx_buffer->buffer[i+j] == topic[j]){
				count++;

				position = i;
				if(count == topic_length)
					break;
			}
			else{
				count = 0;
				position = 0;
			}

		}
		if(count == topic_length)
			break;

	}
	if(count < topic_length)
		return -1;
	// read properties of packet if it is available in the buffer

	packet->topicLength = mqtt_rx_buffer->buffer[position-1];
	packet->topicLength = (mqtt_rx_buffer->buffer[position-2] << 8) + packet->topicLength;
	packet->remainLength = mqtt_rx_buffer->buffer[position-3];
	packet->publishPacketByte = mqtt_rx_buffer->buffer[position-4];
	memcpy(packet->payload,&mqtt_rx_buffer->buffer[position+topic_length],packet->remainLength - packet->topicLength - 2);
//	packet->message[packet->remainLength - packet->topicLength - 1] = '\0';
	memcpy(packet->topic,&mqtt_rx_buffer->buffer[position],topic_length);
	ringBuffer_flush(mqtt_rx_buffer);
	// return message size
	return strlen((char*)packet->payload);
}

/**
 * @brief	This function clears the ring buffer that is used for MQTT messages.
 * @param	None
 * @retval	None
 */
void mqtt_clear_buffer(void){

	ringBuffer_flush(mqtt_rx_buffer);
}
