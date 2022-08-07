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

Status mqtt_connect_broker(const char* ip,const char* port, const char* clientID){

	MQTT_Connect_Packet packet = {0};
	uint16_t lengthOfClientID = strlen(clientID);
	uint8_t remainLength = lengthOfClientID + 12;

	packet.ConnectByte = 0x10;
	packet.ConnectByte = 0x10;
	packet.RemainLength = remainLength;
	packet.ProtocolNameLength = 0x0004;
	strcpy(packet.ProtocolName,"MQTT");
	packet.Level = 0x04;
	packet.Flag = 0x02;
	packet.KeepAlive = MQTT_KEEP_ALIVE;
	packet.ClientIDLength = lengthOfClientID;
	strcpy(packet.ClientID,clientID);

	uint8_t packetBuffer[100] = {0};

	int32_t numberOfBytes = mqtt_encode_packet(packetBuffer, &packet, CONNECT_PACKET);

	if(numberOfBytes < 0)
		return STATUS_ERROR;

	Status tcpConnection = IDLE;

	 tcpConnection = Connect_TCP_Server(ip, port);

	if(tcpConnection != IDLE)
	{
		if(tcpConnection == STATUS_OK)	// connection is successful
		{
			return Send_TCP_Bytes(packetBuffer,numberOfBytes);

		}
		else
			return tcpConnection;
	}
	else
		return tcpConnection;

}
