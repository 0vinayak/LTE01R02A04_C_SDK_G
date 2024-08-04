#ifndef SM3_H
#define SM3_H

#include "osi_compiler.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int SM3(const unsigned char *msg, unsigned int msglen, unsigned char *out_hash);
bool sm3Calc(const void *input, unsigned ilen, void *output);
bool sm3Check(const void *p, unsigned size, const void *expected);

#endif
