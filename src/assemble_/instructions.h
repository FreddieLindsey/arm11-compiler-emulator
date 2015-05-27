#ifndef INSTRUCTIONS_H 
#define INSTRUCTIONS_H 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "secondpass.h"

uint32_t and(char **args);
uint32_t eor(char **args);
uint32_t sub(char **args);
uint32_t rsb(char **args);
uint32_t add(char **args);
uint32_t orr(char **args);
uint32_t mov(char **args);

#endif
