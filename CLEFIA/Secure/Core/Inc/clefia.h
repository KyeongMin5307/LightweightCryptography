#ifndef _CLEFIA_H
#define _CLEFIA_H

void clefia_cbc_128_dec(char * plain, char * cipher, int length, unsigned int* iv, unsigned int *k);
void clefia_cbc_128_enc(char* plain, char * cipher, int length, unsigned int* iv, unsigned int *k);
int equal(unsigned int* a, unsigned int* b, unsigned int size);

#endif