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


uint32_t mqtt_init(size_t rx_buffer_size){

	mqtt_rx_buffer = ringBuffer_init(rx_buffer_size);
	if(mqtt_rx_buffer != NULL)
		return 1;

	return 0;
}

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
		for(uint16_t i=0; i< strlen(publishPacket->message);i++)
		{
			if(i == MAX_LENGTH_OF_TOPIC_MESSAGE)
				return -1;

			buffer[index++] = publishPacket->message[i];
		}
		return index;
	}

	return -1;
}

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
		else
			return status;
	}
	if(state == 1){
		status = Send_TCP_Bytes(packetBuffer, numberOfBytes);
		if(status == STATUS_OK){
			state = 0;
			return status;
		}
		else
			return status;
	}

	return STATUS_ERROR;

}

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

Status mqtt_publish_message(const char* topic, const char* payload){

	static int isFirstCall = 0;
	static uint8_t packetBuffer[100] = {0};
	static int32_t numberOfBytes = 0;


	if(!isFirstCall){
		MQTT_Publish_Packet packet = {0};
		uint16_t topicLength = strlen(topic);
		uint16_t messageLength = strlen(payload);
		uint8_t remainLength = topicLength + messageLength + 2;	// 2 represents topic length bytes.


		packet.publishPacketByte = MQTT_PUBLISH_HEADER;	// Qos, DUP flag and Retain bytes are equal to zero.
		packet.remainLength = remainLength;
		strcpy(packet.topic,topic);
		packet.topicLength = topicLength;
		strcpy(packet.message,payload);

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

void mqtt_receive_handler(void){

	ringBuffer_push(mqtt_rx_buffer, ESP8266.UART_Receive());
}

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
	memcpy(packet->message,&mqtt_rx_buffer->buffer[position+topic_length],packet->remainLength - packet->topicLength - 2);
//	packet->message[packet->remainLength - packet->topicLength - 1] = '\0';
	memcpy(packet->topic,&mqtt_rx_buffer->buffer[position],topic_length);
	ringBuffer_flush(mqtt_rx_buffer);
	// return message size
	return strlen(packet->message);
}

void mqtt_clear_buffer(void){

	ringBuffer_flush(mqtt_rx_buffer);
}
