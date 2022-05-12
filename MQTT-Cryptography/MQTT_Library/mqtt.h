/**
 * @file 	mqtt.h
 * @author  Mehmet Topuz
 * @brief   Header file of the MQTT library. This library uses esp8266.h library to connect MQTT broker.
 *
 *  Website : <a href="https://mehmettopuz.net/">mehmettopuz.net</a>
 *  Created on: May 7, 2022
 *
 */

#ifndef MQTT_H_
#define MQTT_H_

/* C++ Detection Block ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef enum {
	CONNECT_PACKET = 0,
	SUBSCRIBE_PACKET,
	PUBLISH_PACKET

}mqtt_packet_types;

typedef struct{
	uint8_t ConnectByte;					// Connect and Remain Length Bytes(2byte) named as "Fixed Header".
	uint8_t RemainLength;					// Remained Length is hex format of remained byte number after this byte.
	uint16_t ProtocolNameLength; 	// 16 bit length of protocol name. For MQTT, this variable setted as 0x04.
	char ProtocolName[4];				// For us, "MQTT".
	uint8_t Level;					// 0x04.
	uint8_t Flag;						// Connect Flag Bits. For unsafety connection, use 0x02. @tag MQTT_TypeDef_Flag_MACROS
	uint16_t KeepAlive;				// By Default 60.
	uint16_t ClientIDLength;			// This one will be user defined. Client ID Length.
	char ClientID[50];					// Client ID. User Defined.
}MQTT_Connect_Packet;


int32_t mqtt_encode_packet(uint8_t *buffer, void *packet, mqtt_packet_types packetType );


#ifdef __cplusplus
}
#endif

#endif /* MQTT_H_ */
