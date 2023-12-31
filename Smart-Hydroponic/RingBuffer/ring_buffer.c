/**
 * @file 	ring_buffer.c
 * @author  Mehmet Topuz
 * @brief   Source file of the Ring Buffer module.
 *
 *  Website : <a href="https://mehmettopuz.net/">mehmettopuz.net</a>
 *  Created on: Feb 2, 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "ring_buffer.h"
#include <stdlib.h>
#include <string.h>

/* Functions ------------------------------------------------------------------*/

/**
 * @brief 	Initialize the ring buffer. Ring buffer is allocated after this function.
 * @param 	size parameter specifies the size of ring buffer.
 * @retval	RingBuffer typedef structure.
 */
RingBuffer* ringBuffer_init(uint32_t size)
{
	RingBuffer* rBuf;

	rBuf = (RingBuffer*)malloc(1*sizeof(RingBuffer));
	if(rBuf == NULL)
	{
		// Error
		return 0;
	}

	rBuf->size = size;
	rBuf->head = 0;
	rBuf->tail = 0;

	rBuf->buffer = (uint8_t*)calloc((size_t)size,sizeof(uint8_t));
	if(rBuf->buffer == NULL)
		{
			// Error
			return 0;
		}

	return rBuf;
}

/**
 * @brief 	De-initialize the ring buffer.
 * @param 	RingBuffer typedef structure.
 * @retval	None.
 */
void ringBuffer_deInit(RingBuffer* buffer)
{
	if(buffer != NULL)
	{
		if(buffer->buffer != NULL)
		{
			free(buffer->buffer);
		}
		free(buffer);
	}
}

/**
 * @brief 	Write data into the buffer.
 * @param 	RingBuffer typedef structure.
 * @param 	data is an unsigned 8-bit integer variable to be pushed into the buffer.
 * @retval	None.
 */
void ringBuffer_push(RingBuffer* ringBuffer, uint8_t data)
{
	if(ringBuffer->head != (ringBuffer->size - 1))
	{
		ringBuffer->buffer[ringBuffer->head] = data;
		ringBuffer->head += 1;
	}
	else
	{
		ringBuffer->buffer[ringBuffer->head] = data;
		ringBuffer->head = 0;
	}
}

/**
 * @brief 	Read data from the buffer.
 * @param 	RingBuffer typedef structure.
 * @retval	unsigned 8-bit integer data to be read from the buffer.
 */
uint8_t ringBuffer_pop(RingBuffer* ringBuffer)
{
	uint8_t data;

	if(ringBuffer->tail != (ringBuffer->size - 1))
	{
		data = ringBuffer->buffer[ringBuffer->tail];
		ringBuffer->tail += 1;
		return data;
	}
	else
	{

		data = ringBuffer->buffer[ringBuffer->tail];
		ringBuffer->tail = 0;
		return data;
	}

}

/**
 * @brief 	Clear the ring buffer.
 * @param 	RingBuffer typedef structure.
 * @retval	None.
 */
void ringBuffer_flush(RingBuffer* ringBuffer)
{
	memset(ringBuffer->buffer,(uint8_t)0x00,(size_t)ringBuffer->size);
	ringBuffer->head = ringBuffer->tail = 0;
}

/**
 * @brief 	Push an array into ring buffer.
 * @param 	RingBuffer typedef structure.
 * @param	data is an unsigned 8-bit data array to be written into the buffer.
 * @retval	None.
 */
void ringBuffer_pushArray(RingBuffer* ringBuffer, uint8_t* data, size_t size)
{
	 for(int i = 0; i<size;i++)
		 ringBuffer_push(ringBuffer, data[i]);
}

/**
 * @brief 	Search a specific data array in the ring buffer .
 * @param 	RingBuffer typedef structure.
 * @param	data is an unsigned 8-bit array to be searched in the buffer.
 * @retval	1: data is available in the buffer.
 * @retval	0: data is not available in the buffer.
 */
uint32_t ringBuffer_lookFor(RingBuffer* ringBuffer, uint8_t* data)
{
//	char* str = strstr((char*)ringBuffer->buffer,(char*)data);
	uint8_t* str = mystrstr(ringBuffer->buffer, data);
	if(str != NULL)
		return 1;
	else
		return 0;

}

/**
 * @brief 	Check the size of the ring buffer.
 * @param 	RingBuffer typedef structure.
 * @retval	Size of ring buffer.
 */
uint32_t ringBuffer_capacity(RingBuffer* ringBuffer)
{
	return ringBuffer->size;
}

uint8_t *mystrstr(const uint8_t *arr, const uint8_t *subArr){

	for(;*arr;++arr){
		const uint8_t *p1 = arr, *p2 = subArr;
		for(;*p1 && *p2 && (*p1 == *p2); ++p1,++p2);

		if(!*p2)
			return (uint8_t*)arr;
	}
	return NULL;
}

uint32_t array_search(uint8_t *source, uint8_t *array, size_t size_of_source, size_t size_of_array){

	size_t count=0;

	for(int i=0;i<size_of_source;i++){
		if(source[i] == array[0]){
			for(int j=0;j<size_of_array;j++){
				if(size_of_source < (size_of_array+i))
					return 0;
				else{
					if(source[i+j] == array[j]){
						count++;
						if(count == size_of_array)
							return 1;
					}
				}
			}
		}
	}
	return 0;
}
