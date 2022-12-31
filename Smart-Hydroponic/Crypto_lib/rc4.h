/*
 * rc4.h
 *
 *  Created on: Dec 31, 2022
 *      Author: Topuz
 */

#ifndef RC4_H_
#define RC4_H_

/* C++ Detection Block ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <string.h>

#define USE_RNG_FOR_CRYPTO_LIB			0

#if USE_RNG_FOR_CRYPTO_LIB == 0

#define RC4_PUBLIC_KEY			"Lorem ipsum dolor sit amet"

#endif

#define SWAP(x, y) 		(x ^= y ^= x)




void rc4_init(uint8_t *key, size_t key_length);
void rc4_generate_key_stream(uint8_t *key_stream, size_t length);
void rc4_encrypt_decrypt(uint8_t *message, uint8_t *out, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* RC4_H_ */
