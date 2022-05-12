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
	uint8_t remainLength = (uint8_t)ClientIDLength + 12;  // 12 represents the bytes in the packets between remain length and Client ID byte.

	uint8_t outputStream[100];

	MQTT_Connect_Packet connectPacket;

	connectPacket.ConnectByte = 0x10;
	connectPacket.RemainLength = remainLength;
	connectPacket.ProtocolNameLength = 0x04;
	strcpy(connectPacket.ProtocolName,"MQTT");
	connectPacket.Level = 0x04;
	connectPacket.Flag = 0x02;
	connectPacket.KeepAlive = 60;
	connectPacket.ClientIDLength = ClientIDLength;
	strcpy(connectPacket.ClientID,clientID);

	int32_t status = mqtt_encode_packet(outputStream, &connectPacket, CONNECT_PACKET);

	LONGS_EQUAL(1,status);





}



