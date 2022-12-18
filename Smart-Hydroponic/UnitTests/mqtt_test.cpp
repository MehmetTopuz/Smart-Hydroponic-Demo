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
#include <cstring>
#include <cstdio>
//#include <stdlib.h>
#include "mqtt.h"

extern void UART_Transmit_Fake(uint8_t* data,size_t size);
extern uint8_t UART_Receive_Fake(void);
extern uint32_t getTick_Fake(void);
/* Test functions of the MQTT library-----------------------------------------------------------------------*/


TEST_GROUP(MqttTestGroup)
{

	void setup()
	{
		ESP_Init(UART_Transmit_Fake,
				  UART_Receive_Fake,
				  getTick_Fake,
				  255);
		mqtt_init(255);
	}

	void teardown()
	{
		mock().checkExpectations();
		mock().clear();
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


TEST(MqttTestGroup, SubscribePacketTest)
{
	char topicName[] = "Testtopic";
	uint16_t topicLength = strlen(topicName);
	uint8_t remainLength = (uint8_t)topicLength + 5;

	uint8_t outputBuffer[100] = {0};

	MQTT_Subscribe_Packet subscribePacket;

	subscribePacket.subscribePacketByte = 0x82;
	subscribePacket.remainLength = remainLength;
	subscribePacket.packetID = 1;
	subscribePacket.topicLength = topicLength;
	strcpy(subscribePacket.topic,topicName);
	subscribePacket.Qos = 0;

	int32_t numberOfBytes = mqtt_encode_packet(outputBuffer, &subscribePacket, SUBSCRIBE_PACKET);

	LONGS_EQUAL(remainLength+2,numberOfBytes);
	LONGS_EQUAL(0x82,outputBuffer[0]);
	LONGS_EQUAL(remainLength,outputBuffer[1]);
	LONGS_EQUAL(0x0001,(uint16_t)((outputBuffer[2] << 8) + outputBuffer[3]));
	LONGS_EQUAL(topicLength,(uint16_t)((outputBuffer[4] << 8) + outputBuffer[5]));

	int32_t index = 6;

	for(int16_t i=0;i<topicLength;i++)
	{
		BYTES_EQUAL(topicName[i],outputBuffer[index++]);
	}

	LONGS_EQUAL(0,outputBuffer[index]);


}

TEST(MqttTestGroup, PublishPacketTest)
{
	char topicName[] = "Testtopic";
	char message[] = "hello world!";

	uint16_t topicLength = strlen(topicName);
	uint16_t messageLength = strlen(message);
	uint8_t remainLength = (uint8_t)topicLength+ (uint8_t)messageLength + 2;

	uint8_t outputBuffer[100] = {0};

	MQTT_Publish_Packet publishPacket = {0};

	publishPacket.publishPacketByte = 0x30;
	publishPacket.remainLength = remainLength;
	publishPacket.topicLength = topicLength;
	strcpy(publishPacket.topic,topicName);
	publishPacket.sizeOfPayload = strlen(message);
	for(int i=0; i<publishPacket.sizeOfPayload; i++)
		publishPacket.payload[i] = (uint8_t)message[i];

	int32_t numberOfBytes = mqtt_encode_packet(outputBuffer, &publishPacket, PUBLISH_PACKET);

	LONGS_EQUAL(remainLength+2,numberOfBytes);
	LONGS_EQUAL(0x30,outputBuffer[0]);
	LONGS_EQUAL(remainLength,outputBuffer[1]);
	LONGS_EQUAL(topicLength,(uint16_t)((outputBuffer[2] << 8) + outputBuffer[3]));

	int32_t index = 4;

	for(int16_t i=0;i<topicLength;i++)
	{
		BYTES_EQUAL(topicName[i],outputBuffer[index++]);
	}

	for(int16_t i=0;i<messageLength;i++)
	{
		BYTES_EQUAL(message[i],outputBuffer[index++]);
	}


}

TEST(MqttTestGroup, MqttConnectBrokerTest)
{
	const char ip[] = "broker.mqttdashboard.com";
	const char port[] = "1883";

	const char esp_response[4][50] = {
			AT_RESPONSE_OK,
			AT_RESPONSE_CONNECT,
			AT_RESPONSE_GREATER_THAN,
			AT_RESPONSE_SEND_OK
	};

	const char esp_commands[3][60] = {
			AT_CIPMUX_SINGLE,
			"AT+CIPSTART=\"TCP\",\"broker.mqttdashboard.com\",1883\r\n",
			"AT+CIPSEND=30\r\n" // 30 represents the number of bytes to be sent.
	};

	for(int i=0;i<3;i++)
	{
		mock().expectOneCall("UART_Transmit_Fake").withParameter("data", (uint8_t*)esp_commands[i], strlen(esp_commands[i])).withIntParameter("size", strlen(esp_commands[i]));
	}
	// mocking connect packet
	uint8_t data_buffer[] = {0x10,0x1C,0x00,0x04,'M','Q','T','T',0x04,0x02,0x00,0x3C,0x00,0x10,'S','m','a','r','t','-','H','y','d','r','o','p','o','n','i','c'};

	mock().expectOneCall("UART_Transmit_Fake").withParameter("data",(uint8_t*)data_buffer,30).withIntParameter("size", 30);


	Status status;
	int i = 0;
	while(1)
	{

		status = mqtt_connect_broker(ip, port);

		if(status != IDLE)
		{
			break;
		}
		if(i<4)
		{
			for(int j=0;j<(int)strlen(esp_response[i]);j++)
			{
				mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)esp_response[i][j]);
				ESP_UART_ReceiveHandler();
			}
			i++;
		}
	}


	LONGS_EQUAL(STATUS_OK,status);

}

TEST(MqttTestGroup, MqttPingRequestTest)
{
	// pingreq packet size is two bytes [ 0xC0, 0x00]
	uint8_t pingReqPacket[2] = {0xC0, 0x00};
	// pingresp packet size is two bytes [ 0xC1, 0x00]
	uint8_t pingRespPacket[2] = {0xD0, 0x00};

	char *response_buffer[2] =
	{
			(char*)AT_RESPONSE_GREATER_THAN,
			(char*)AT_RESPONSE_SEND_OK
	};

	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", (uint8_t*)"AT+CIPSEND=2\r\n", strlen("AT+CIPSEND=2\r\n")).withIntParameter("size", strlen("AT+CIPSEND=2\r\n"));
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", pingReqPacket, 2).withIntParameter("size", 2);

	Status response = IDLE;
	int i= 0;
	while(1){

		response = mqtt_ping_request();

		if(response != IDLE)
		{
			break;
		}
		if(i<2)
		{
			for(int j=0;j<(int)strlen(response_buffer[i]);j++)
			{
				mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)response_buffer[i][j]);
				ESP_UART_ReceiveHandler();
			}
			i++;
		}
		else{
			mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)pingRespPacket[0]);
			ESP_UART_ReceiveHandler();
			mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)pingRespPacket[1]);
			ESP_UART_ReceiveHandler();
		}

	}

	LONGS_EQUAL(STATUS_OK,response);


}

TEST(MqttTestGroup, MqttPublishTest)
{
	const char payload[] = "Test message";
	const char topic[] = "test/topic";


	int topicLength = strlen(topic);

	char *response_buffer[2] =
	{
			(char*)AT_RESPONSE_GREATER_THAN,
			(char*)AT_RESPONSE_SEND_OK
	};
	uint8_t remainLength = strlen(payload) + topicLength + 2;
	uint8_t publishPacket[] = {0x30,remainLength,(uint8_t)(topicLength >> 8),(uint8_t)(topicLength & 0xff),'t','e','s','t','/','t','o','p','i','c','T','e','s','t',' ','m','e','s','s','a','g','e'};
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", (uint8_t*)"AT+CIPSEND=26\r\n", strlen("AT+CIPSEND=26\r\n")).withIntParameter("size", strlen("AT+CIPSEND=26\r\n"));
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", publishPacket, 26).withIntParameter("size", 26);

	int i=0;
	Status response = IDLE;

	while(1){
		response = mqtt_publish_message(topic, (uint8_t*)payload, strlen(payload));

		if(response != IDLE)
		{
			break;
		}
		if(i<2){
			for(int j=0;j<(int)strlen(response_buffer[i]);j++)
			{
				mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)response_buffer[i][j]);
				ESP_UART_ReceiveHandler();
			}
			i++;
		}

	}

	LONGS_EQUAL(STATUS_OK,response);

}

TEST(MqttTestGroup, MqttSubscribeTest)
{
	const char topic[] = "test/topic";

	int topicLength = strlen(topic);
	uint8_t Qos= 0, packageID = 1;

	char *response_buffer[2] =
	{
			(char*)AT_RESPONSE_GREATER_THAN,
			(char*)AT_RESPONSE_SEND_OK
	};
	uint8_t remainLength = topicLength + 5;
	uint8_t subscribePacket[] = {0x82,remainLength,0x00,packageID,(uint8_t)(topicLength >> 8),(uint8_t)(topicLength & 0xff),'t','e','s','t','/','t','o','p','i','c',Qos};
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", (uint8_t*)"AT+CIPSEND=17\r\n", strlen("AT+CIPSEND=17\r\n")).withIntParameter("size", strlen("AT+CIPSEND=17\r\n"));
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", subscribePacket, 17).withIntParameter("size", 17);

	int i=0;
	Status response = IDLE;

	while(1){
		response = mqtt_subcribe(topic);

		if(response != IDLE)
		{
			break;
		}
		if(i<2){
			for(int j=0;j<(int)strlen(response_buffer[i]);j++)
			{
				mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)response_buffer[i][j]);
				ESP_UART_ReceiveHandler();
			}
			i++;
		}

	}

	LONGS_EQUAL(STATUS_OK,response);
}

extern RingBuffer *mqtt_rx_buffer;

TEST(MqttTestGroup, MqttRingBufferTest)
{
	const char msg[] = "Mqtt test message";

	for(int32_t i=0;i<(int32_t)strlen(msg);i++)
	{
		mock().expectOneCall("UART_Receive_Fake").andReturnValue((uint8_t)msg[i]);
		mqtt_receive_handler();
	}

	STRCMP_EQUAL("Mqtt test message",(char*)mqtt_rx_buffer->buffer);
	LONGS_EQUAL(strlen(msg),mqtt_rx_buffer->head);
	LONGS_EQUAL(255,mqtt_rx_buffer->size);

}

TEST(MqttTestGroup, MqttReadMessageTest)
{
	// create fake publish message for subscribtion
	MQTT_Publish_Packet pub_packet = {0};
	char message[] = "MOTOR_ON";
	pub_packet.sizeOfPayload = strlen(message);
	for(int i=0; i<pub_packet.sizeOfPayload; i++)
		pub_packet.payload[i] = (uint8_t)message[i];
	pub_packet.publishPacketByte = MQTT_PUBLISH_HEADER;
	strcpy(pub_packet.topic,"garden/hydroponic");
	pub_packet.topicLength = strlen("garden/hydroponic");
	pub_packet.remainLength = pub_packet.topicLength + strlen("MOTOR_ON") + 2;

	uint8_t buffer[100] = {0};
	int32_t numberOfBytes = mqtt_encode_packet(buffer, &pub_packet, PUBLISH_PACKET);

	if(numberOfBytes < 0)
		FAIL_TEST("encode packet error!");

	// push it into ring buffer
	ringBuffer_pushArray(mqtt_rx_buffer, buffer,numberOfBytes);
	// call read message function
	MQTT_Publish_Packet received_packet = {0};
	int32_t result = 0;
	result = mqtt_read_message(&received_packet, "garden/hydroponic");
	// check the properties of received message
	LONGS_EQUAL(strlen("MOTOR_ON"),result);
	STRCMP_EQUAL((char*)pub_packet.payload,(char*)received_packet.payload);
	STRCMP_EQUAL(pub_packet.topic,received_packet.topic);
	LONGS_EQUAL(pub_packet.remainLength,received_packet.remainLength);
	LONGS_EQUAL(pub_packet.publishPacketByte,received_packet.publishPacketByte);
	LONGS_EQUAL(pub_packet.topicLength,received_packet.topicLength);
}

TEST(MqttTestGroup, MqttDisconnectBrokerTest)
{
	char *response_buffer[2] =
	{
			(char*)AT_RESPONSE_GREATER_THAN,
			(char*)AT_RESPONSE_SEND_OK
	};

	uint8_t disconnect_packet[2] = { 0xE0, 0x00};

	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", (uint8_t*)"AT+CIPSEND=2\r\n", strlen("AT+CIPSEND=2\r\n")).withIntParameter("size", strlen("AT+CIPSEND=2\r\n"));
	mock().expectOneCall("UART_Transmit_Fake").withParameter("data", disconnect_packet, 2).withIntParameter("size", 2);

	int i=0;
	Status response = IDLE;

	while(1){
		response = mqtt_disconnect_broker();

		if(response != IDLE)
		{
			break;
		}
		if(i<2){
			for(int j=0;j<(int)strlen(response_buffer[i]);j++)
			{
				mock().expectOneCall("UART_Receive_Fake").andReturnValue((int)response_buffer[i][j]);
				ESP_UART_ReceiveHandler();
			}
			i++;
		}

	}

	LONGS_EQUAL(STATUS_OK,response);

}
