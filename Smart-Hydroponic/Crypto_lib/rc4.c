/*
 * rc4.c
 *
 *  Created on: Dec 31, 2022
 *      Author: Topuz
 */


#include "rc4.h"


static uint8_t state_vector[256], key_stream[256] = {0};;


void rc4_init(uint8_t *key, size_t key_length){

	uint8_t temporary_vector[256] = {0};

	/*Fill the vectors*/
	for(int i=0; i<256; ++i){

		state_vector[i] = i;
		temporary_vector[i] = key[ i % key_length];
	}

	/* Initial permutation of state vector (aka the key scheduling algorithm).*/
	int j = 0;

	for(int i=0; i<256; ++i){

		j = (j + state_vector[i] + temporary_vector[i]) % 256;
		SWAP(state_vector[i],state_vector[j]);

	}
	rc4_generate_key_stream(key_stream, sizeof(key_stream));

}

void rc4_generate_key_stream(uint8_t *key_stream, size_t length){

	/* Generate key stream with pseudo random number generator algorithm.*/
	uint8_t i=0,j=0;

	for(int k=0; k<length; ++k){

		i = (i+1) % 256;
		j = (j + state_vector[i]) % 256;

		SWAP(state_vector[i],state_vector[j]);
		key_stream[k] = state_vector[(state_vector[i] + state_vector[j]) % 256];

	}


}

void rc4_encrypt_decrypt(uint8_t *message, uint8_t *out, size_t length){

	/* This function can be used for both encryption and decryption. */
	memset(out,0,sizeof(out));
	for(int i=0; i<length; ++i)
		out[i] = message[i] ^ key_stream[i];
}
