/**
 * @file 	mqtt_test.cpp
 * @author  Mehmet Topuz
 * @brief   Source file of the test functions.
 *
 *  Website : <a href="https://mehmettopuz.net/">mehmettopuz.net</a>
 *  Created on: Apr 23, 2022
 *
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <cstdio>
//#include <stdlib.h>
#include "mqtt.h"

/* Test functions of the MQTT library-----------------------------------------------------------------------*/

TEST_GROUP(MqttTestGroup)
{

	void setup()
	{

	}

	void teardown()
	{

	}
};

TEST(MqttTestGroup, ConnectPacketTest)
{
	char clientID[] = "Topuz";
	uint16_t ClientIDLength = strlen(clientID);
	uint8_t remainLength = (uint8_t)ClientIDLength + 12;  	// 12 represents the bytes in the packet between remain length and Client ID byte.

	uint8_t outputStream[100] ={0};							// for serialize

	MQTT_Connect_Packet connectPacket;

	connectPacket.ConnectByte = 0x10;
	connectPacket.RemainLength = remainLength;
	connectPacket.ProtocolNameLength = 0x0004;
	strcpy(connectPacket.ProtocolName,"MQTT");
	connectPacket.Level = 0x04;
	connectPacket.Flag = 0x02;
	connectPacket.KeepAlive = 60;
	connectPacket.ClientIDLength = ClientIDLength;
	strcpy(connectPacket.ClientID,clientID);

	int32_t status = mqtt_encode_packet(outputStream, &connectPacket, CONNECT_PACKET);

	LONGS_EQUAL(remainLength+2,status);												// check number of bytes that is written to buffer
	STRCMP_EQUAL("Topuz",(const char*)&outputStream[status-ClientIDLength]);		// check clientID that is written to buffer
	LONGS_EQUAL(0x10,outputStream[0]);												// check connectByte
	LONGS_EQUAL(remainLength,outputStream[1]);										// check remainlength
	LONGS_EQUAL(0x0004,(uint16_t)((outputStream[2] << 8) + outputStream[3]));		// check protocol name length
	LONGS_EQUAL('M',outputStream[4]); 												// check protocol name
	LONGS_EQUAL('Q',outputStream[5]);
	LONGS_EQUAL('T',outputStream[6]);
	LONGS_EQUAL('T',outputStream[7]);
	LONGS_EQUAL(0x04,outputStream[8]);													// check level byte
	LONGS_EQUAL(0x02,outputStream[9]);													// check flag byte
	LONGS_EQUAL((uint16_t)60,(uint16_t)((outputStream[10] << 8) + outputStream[11]));	// check keep alive bytes
	LONGS_EQUAL(ClientIDLength,(uint16_t)((outputStream[12] << 8) + outputStream[13]));	// check client id length bytes

}



