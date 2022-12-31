#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <cstring>
#include <cstdio>

#include "app.h"

TEST_GROUP(RC4TestGroup)
{

	void setup()
	{
		rc4_init((uint8_t*)RC4_PUBLIC_KEY, strlen(RC4_PUBLIC_KEY));
	}

	void teardown()
	{

	}
};

TEST(RC4TestGroup, EncryptDecryptTest)
{
	char test_message[] = "This is a test message.";

	char encrypted_message[50]={0}, decrypted_message[50]={0};

	rc4_encrypt_decrypt((uint8_t*)test_message, (uint8_t*)encrypted_message, strlen(test_message));

	/*
	 * TODO: verify whether the message was encrypted or not.
	 */
	rc4_encrypt_decrypt((uint8_t*)encrypted_message, (uint8_t*)decrypted_message, strlen(test_message));

	STRCMP_EQUAL(test_message, decrypted_message);

	/* User is responsible to clear buffers.*/
	memset(encrypted_message,0,sizeof(encrypted_message));
	memset(decrypted_message,0,sizeof(decrypted_message));

	strcpy(test_message, "hello world!");

	rc4_encrypt_decrypt((uint8_t*)test_message, (uint8_t*)encrypted_message, strlen(test_message));

	rc4_encrypt_decrypt((uint8_t*)encrypted_message, (uint8_t*)decrypted_message, strlen(test_message));

	STRCMP_EQUAL(test_message, decrypted_message);
}
