/*
 * app_test.cpp
 *
 *  Created on: Dec 19, 2022
 *      Author: Topuz
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <cstring>
#include <cstdio>

#include "app.h"

TEST_GROUP(AppTestGroup)
{

	void setup()
	{

	}

	void teardown()
	{

	}
};
extern temp_t temparature_t;

extern ph_t pH_t;

extern uint8_t humidity, tank_level, conductivity;

TEST(AppTestGroup, HeartBeatTest)
{
	uint8_t heart_beat_packet[SIZE_OF_HEART_BEAT_PACKET] = {0};
	int length = 0;

	temparature_t.temp_f = 26.7;
	pH_t.ph_f = 7.2;
	humidity = 50;
	tank_level = 80;
	conductivity = 20;

	length = encode_heart_beat_packet(heart_beat_packet);

	LONGS_EQUAL(12,length);
	LONGS_EQUAL(0, heart_beat_packet[0]);
	LONGS_EQUAL(temparature_t.temp_b[0], heart_beat_packet[1]);
	LONGS_EQUAL(temparature_t.temp_b[1], heart_beat_packet[2]);
	LONGS_EQUAL(temparature_t.temp_b[2], heart_beat_packet[3]);
	LONGS_EQUAL(temparature_t.temp_b[3], heart_beat_packet[4]);
	LONGS_EQUAL(humidity, heart_beat_packet[5]);
	LONGS_EQUAL(tank_level, heart_beat_packet[6]);
	LONGS_EQUAL(conductivity, heart_beat_packet[7]);
	LONGS_EQUAL(pH_t.ph_b[0], heart_beat_packet[8]);
	LONGS_EQUAL(pH_t.ph_b[1], heart_beat_packet[9]);
	LONGS_EQUAL(pH_t.ph_b[2], heart_beat_packet[10]);
	LONGS_EQUAL(pH_t.ph_b[3], heart_beat_packet[11]);
}


TEST(AppTestGroup, StringToCmdTest)
{

	MQTT_Publish_Packet packet = {0};
	strcpy((char*)packet.payload,"VALVE_ON");	// put a random command to the packet.

	commands_t command = string_to_cmd(&packet);

	LONGS_EQUAL(valve_on, command);

	strcpy((char*)packet.payload,"NO_COMMAND");	// put a command that is not available in the command_list.

	command = string_to_cmd(&packet);

	LONGS_EQUAL(idle, command);
}
