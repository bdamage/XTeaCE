// XTeaCE.cpp : Defines the entry point for the console application.
// XTEA encyption / decryption proof of concept Kjell Lloyd 2008

#include "stdafx.h"
#include <string.h>


typedef  unsigned __int32 uint32_t;


void encipher(unsigned int num_rounds, uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], i;
    uint32_t sum=0, delta=0x9E3779B9;
    for(i=0; i<num_rounds; i++) {
       v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
void decipher(unsigned int num_rounds, uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], i;
    uint32_t delta=0x9E3779B9, sum=delta*num_rounds;
    for(i=0; i<num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}


int _tmain(int argc, _TCHAR* argv[])
{
/*
 Important when doing cipher/decipher is to round up the length to next 128 bit (8 bytes).
 Otherwise stack corruption will occur and anything can goes wrong (crash).
*/
	char szMessage[] ={"This is a message tried to cipher!\0\0\0\0\0\0\0\0\0\0"}; // all those \0 is there to reserve space so stack corruption occurs

	unsigned int blockSize = sizeof(uint32_t)*2;
	unsigned int nLength = strlen(szMessage);
	uint32_t wKey[] = {0xdeadbabe,0xdeadface,0xdeadbeef,0x45112150}; //secret key
	
	for(unsigned int i=0; i<nLength;i+=blockSize)
		encipher(32, (uint32_t*)&szMessage[i], (uint32_t*) wKey);


	for(unsigned int i=0; i<nLength;i+=blockSize)
		decipher(32, (uint32_t*) &szMessage[i], (uint32_t*) wKey);


	return 0;
}

