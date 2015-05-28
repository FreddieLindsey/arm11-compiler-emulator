#ifndef INSTRUCTIONS_H 
#define INSTRUCTIONS_H 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "secondpass.h"

// data processing
uint32_t and(char **args);
uint32_t eor(char **args);
uint32_t sub(char **args);
uint32_t rsb(char **args);
uint32_t add(char **args);
uint32_t orr(char **args);
uint32_t mov(char **args);
uint32_t tst(char **args);
uint32_t teq(char **args);
uint32_t cmp(char **args);

// multiply
uint32_t mul(char **args);
uint32_t mla(char **args); 

// single data transfer

// special
uint32_t andeq(char **args);
uint32_t lsl(char **args);

#endif
