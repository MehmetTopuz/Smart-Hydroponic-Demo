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
#include "esp8266.h"


#define MQTT_KEEP_ALIVE					(uint16_t)60
#define MQTT_PING_REQUEST				(uint8_t)0xC0
#define MQTT_PING_RESPONSE				(uint8_t)0xD0
#define MQTT_PUBLISH_HEADER				(uint8_t)0x30
#define MQTT_DISCONNECT_HEADER			(uint8_t)0xE0

//#define USE_MQTT_AUTHENTICATION

#ifdef	USE_MQTT_AUTHENTICATION

#define MQTT_USERNAME					"mehmettopuz"
#define MQTT_PASSWORD					"12345"

#endif

#define MQTT_CLIENT_ID					"Smart-Hydroponic"

#define MAX_LENGTH_OF_CLIENT_ID 		50U
#define MAX_LENGTH_OF_TOPIC_NAME 		50U
#define MAX_LENGTH_OF_TOPIC_MESSAGE 	100U
#define MAX_LENGTH_OF_USERNAME			20U
#define MAX_LENGTH_OF_PASSWORD			20U


/**
 * @brief	Enumerator typedef definition of MQTT packet types.
 * TODO:	Declare other packet types later.
 */
typedef enum {
	CONNECT_PACKET = 0,
	SUBSCRIBE_PACKET,
	PUBLISH_PACKET

}mqtt_packet_types;


/**
 * @brief	MQTT_Connect_Packet typedef definition.
 * 			Represents members of connect packet of the MQTT protocol.
 *
 * TODO: 	Combine the structs given below in a single struct.
 */
typedef struct{
	uint8_t ConnectByte;
	uint8_t RemainLength;
	uint16_t ProtocolNameLength;
	char ProtocolName[4];
	uint8_t Level;
	uint8_t Flag;
	uint16_t KeepAlive;
	uint16_t ClientIDLength;
	char ClientID[MAX_LENGTH_OF_CLIENT_ID];
	char UserName[MAX_LENGTH_OF_USERNAME];
	char Password[MAX_LENGTH_OF_PASSWORD];
}MQTT_Connect_Packet;

/**
 * @brief	MQTT_Subscribe_Packet typedef definition.
 * 			Represents members of subscribe packet of the MQTT protocol.
 */
typedef struct{
	uint8_t subscribePacketByte;
	uint8_t remainLength;
	uint16_t packetID;
	uint16_t topicLength;
	char topic[MAX_LENGTH_OF_TOPIC_NAME];
	uint8_t Qos;
}MQTT_Subscribe_Packet;

/**
 * @brief	MQTT_Publish_Packet typedef definition.
 * 			Represents members of publish packet of the MQTT protocol.
 */
typedef struct{
	uint8_t publishPacketByte;
	uint8_t remainLength;
	uint16_t topicLength;
	char topic[MAX_LENGTH_OF_TOPIC_NAME];
	uint8_t payload[MAX_LENGTH_OF_TOPIC_MESSAGE];
	size_t sizeOfPayload;
}MQTT_Publish_Packet;

/**
 * @brief	Initialize function of the MQTT library.
 * @param	rx_buffer_size: Size of the buffer that will be allocated dynamically for the ring buffer
 * 			to handle UART receive operations.
 * @retval	The function returns 1 when the memory was allocated successfully. Otherwise, it will return 0.
 */
uint32_t mqtt_init(size_t rx_buffer_size);

/**
 * @brief	This function serializes the MQTT packets according to given packet type.
 * @param	*buffer: The address of the buffer that will be representing the bytes of MQTT packets.
 * @param	*packet: The address of the packet structure.
 * @param	packetType: Enumerator type of packets. The function handles the void* parameter based on this enumerator type.
 * @retval	The function returns the size of bytes that written to the buffer. Otherwise, it will return -1.
 */
int32_t mqtt_encode_packet(uint8_t *buffer, void *packet, mqtt_packet_types packetType );

/**
 * @brief	This function tries to establish TCP connection to the MQTT broker.
 * @param	ip: IP address of the MQTT broker.
 * @param	port: Port of the MQTT broker.
 * @retval	The function returns STATUS_OK when the broker connection was successful.
 * 			Otherwise, it returns one of the error types of Status typedef.
 */
Status mqtt_connect_broker(const char* ip,const char* port);

/**
 * @brief	This function tries to disconnect TCP connection with the MQTT broker.
 * @param	None
 * @retval	The function returns STATUS_OK when the broker connection was successfully disconnected.
 * 			Otherwise, it returns one of the error types of Status typedef.
 */
Status mqtt_disconnect_broker(void);

/**
 * @brief	This function sends ping request to the broker and waits ping response packet.
 * @param	None
 * @retval	It returns STATUS_OK when ping response was received.
 */
Status mqtt_ping_request(void);

/**
 * @brief	This function publishes message over MQTT.
 * @param	topic:		The address of topic.
 * @param	payload:	Message bytes that will be sent to the broker.
 * @param	size:		The size of the payload.
 * @retval	It returns STATUS_OK when message was sent successfully. Otherwise, it returns one of the error types of Status typedef.
 * 			Note: This function does not check whether the message was delivered or not because the QoS is currently set to zero.
 */
Status mqtt_publish_message(const char* topic, const uint8_t* payload, size_t size);

/**
 * @brief	This function sends subscribe request to the broker.
 * @param	topic: The name of the topic that will be subscribed.
 * @retval	It returns STATUS_OK when subscribe request was sent successfully.
 */
Status mqtt_subcribe(const char* topic);

/**
 * @brief	This function handles UART receive operations in background.
 * 			It must be called in the UART interrupt handler function.
 * @param 	None
 * @retval	None
 */
void mqtt_receive_handler(void);

/**
 * @brief	This function checks if there is a publish message in the ring buffer.
 * @param	packet: The address of the packet that will be filled with the function.
 * @param	topic:	The address of the topic name that will be searched in the ring buffer.
 * @retval	It returns -1 if message is not available. Otherwise, It returns the length of packet.
 */
int32_t mqtt_read_message(MQTT_Publish_Packet *packet, const char *topic);

/**
 * @brief	This function clears the ring buffer that is used for MQTT messages.
 * @param	None
 * @retval	None
 */
void mqtt_clear_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_H_ */
