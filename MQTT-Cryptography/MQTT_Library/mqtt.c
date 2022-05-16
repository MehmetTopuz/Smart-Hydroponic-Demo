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
	return -1;
}
