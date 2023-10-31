#ifndef _UTIL_H
#define _UTIL_H

static void print_hex(const unsigned char* hex, int len)
{
	for (const unsigned char* p = hex; p - hex < len; p++)
		printf("%02X ", *p);
	printf("\n");
}

#endif